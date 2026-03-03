/**
  ******************************************************************************
  * @file    aspep.c
  * @author  SRA
  * @brief
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "aspep_ctrl.h"
#include "usart_aspep_driver.h"
#include <stdint.h>
#include <string.h>
#include "cmsis_compiler.h"

/* Private typedef -----------------------------------------------------------*/

#ifndef ASPEP_CTRL_TX_SYNC_PAYLOAD_MAX
#define ASPEP_CTRL_TX_SYNC_PAYLOAD_MAX 1024
#endif

#ifndef ASPEP_CTRL_RX_SYNC_PAYLOAD_MAX
#define ASPEP_CTRL_RX_SYNC_PAYLOAD_MAX 1024
#endif

#ifndef ASPEP_CTRL_TX_ASYNC_PAYLOAD_MAX_A
#define ASPEP_CTRL_TX_ASYNC_PAYLOAD_MAX_A 2048
#endif

#define ASPEP_HEADER_SIZE     4
#define ASPEP_DATACRC_SIZE    2

/* Define buffer dimensions according to application settings */
#define ASPEP_CTRL_TX_SYNC_BUFFER_SIZE     (ASPEP_CTRL_TX_SYNC_PAYLOAD_MAX + ASPEP_HEADER_SIZE + ASPEP_DATACRC_SIZE)
#define ASPEP_CTRL_RX_SYNC_BUFFER_SIZE     (ASPEP_CTRL_RX_SYNC_PAYLOAD_MAX + ASPEP_DATACRC_SIZE) /* ASPEP_HEADER_SIZE is not stored in the RX buffer. */
#define ASPEP_CTRL_TX_ASYNC_BUFFER_SIZE    (ASPEP_CTRL_TX_ASYNC_PAYLOAD_MAX_A + ASPEP_HEADER_SIZE + ASPEP_DATACRC_SIZE)

#define ID_MASK               ((uint32_t)0xFU)
#define DATA_PACKET           ((uint32_t)0x9U)
#define PING                  ((uint32_t)0x6U)
#define BEACON                ((uint32_t)0x5U)
#define ERROR_PACKET          ((uint32_t)0x7U)
#define RESPONSE              ((uint32_t)0xAU)
#define ASYNC                 ((uint32_t)0x9U)

#define ID_MASK_PING                0x10U
#define ASPEP_CTRL_PING_CONFIGURED    0x10U
#define ASPEP_CTRL_PING_UNCONFIGURED  0x00U

#define ASPEP_OK              0
#define ASPEP_BAD_PACKET_TYPE 1
#define ASPEP_BAD_PACKET_SIZE 2
#define ASPEP_BAD_CRC_HEADER  4

#define ASPEP_RECEIVED_PACKET_QUEUE_LEN 3

#define MIN(a,b) ( (a < b) ? a : b )

/* Private macros ------------------------------------------------------------*/
typedef uint32_t ASPEP_CTRL_PacketType;
typedef uint8_t ASPEP_CTRL_PingConfType;

typedef bool (* ASPEP_send_cb_t)(void *pHW_Handle, void *txbuffer, uint16_t length);
typedef void (* ASPEP_receive_cb_t)(void *pHW_Handle, void *rxbuffer, uint16_t length);
typedef void (* ASPEP_hwinit_cb_t)(void *pHW_Handle);
typedef void (* ASPEP_hwsync_cb_t)(void *pHW_Handle);

/**
  * @enum ASPEP_CTRL_sm_state_t
  * @brief Represents the state machine types for the ASPEP_CTRL_sm.
  *
  */
typedef enum
{
  ASPEP_IDLE,         /**< The ASPEP state machine is idle and not performing any operation. */
  ASPEP_CONFIGURING,  /**< The ASPEP state machine is being configured. */
  ASPEP_CONNECTING,   /**< The ASPEP state machine is in the process of establishing a connection. */
  ASPEP_CONNECTED     /**< The ASPEP state machine is successfully connected. */
} ASPEP_CTRL_sm_state_t;

/**
  * @enum ASPEP_CTRL_TL_sm_t
  * @brief Represents the state machine types for the ASPEP Transport Layer (TL).
  *
  * This enumeration defines the states of the ASPEP Transport Layer state machine.
  */
typedef enum
{
  WAITING_PACKET,  /**< Transport layer is waiting for a new packet to arrive. */
  WAITING_PAYLOAD, /**< Transport layer is waiting for the payload of a received packet. */
} ASPEP_CTRL_TL_sm_t;

/**
  * @struct ASPEP_CTRL_Capabilities_def
  * @brief Defines the capabilities of the ASPEP Controller.
  */
typedef struct
{
  uint8_t DATA_CRC;      /**< Indicates whether CRC is enabled for data (1 = enabled, 0 = disabled). */
  uint8_t RX_maxSize;    /**< Maximum size of the receive p_buffer in bytes. */
  uint8_t TXS_maxSize;   /**< Maximum size of the synchronous transmit p_buffer in bytes. */
  uint8_t TXA_maxSize;   /**< Maximum size of the asynchronous transmit p_buffer in bytes. */
  uint8_t version;       /**< Version of the ASPEP capabilities structure. */
} ASPEP_CTRL_Capabilities_def;

/**
  * @enum ASPEP_CTRL_Buff_access_t
  * @brief Defines the access states of a buffer in the ASPEP Transport Layer (TL).
  */
typedef enum
{
  FREE_BUFF,   /**< The buffer is free and available for use. */
  WRITE_LOCK,  /**< The buffer is locked for writing. */
  PENDING,     /**< The buffer is pending for processing or transfer. */
  READ_LOCK    /**< The buffer is locked for reading. */
} ASPEP_CTRL_Buff_access_t;

/**
  * @struct ASPEP_CTRL_TL_Buff_t
  * @brief Represents a buffer used in the ASPEP Transport Layer (TL).
  *
  * This structure defines the properties of a buffer, including its pointer,
  * length, and access state, used in the ASPEP Transport Layer.
  */
typedef struct
{
  uint8_t *p_buffer;      /**< Pointer to the buffer memory. */
  uint16_t length;        /**< Length of the buffer in bytes. */
  ASPEP_CTRL_Buff_access_t state;    /**< Current access state of the buffer */
} ASPEP_CTRL_TL_Buff_t;

/**
  * @brief State machine event for ASPEP control.
  */
typedef struct
{
  bool SM_Event_Connected; /**< Indicates if the connection event has occurred. */
  bool SM_Event_Response;  /**< Indicates if a response event has occurred. */
  bool SM_Event_Ping;      /**< Indicates if a ping event has occurred. */
  bool SM_Event_Async;     /**< Indicates if an asynchronous event has occurred. */
} ASPEP_CTRL_Event_sm_t;

/**
  * @struct ASPEP_Received_packet_queue_t
  * @brief Represents a queue for managing received packets in the ASPEP module.
  *
  * This structure implements a circular queue to store and manage received packets
  * in the ASPEP module. It tracks the buffer, size, and the current state of the queue.
  */
typedef struct
{
  uint8_t p_buffer[ASPEP_RECEIVED_PACKET_QUEUE_LEN]; /**< Circular buffer to store received packets. */
  int max_size;                                      /**< Maximum size of the queue (number of packets it can hold). */
  int head;                                          /**< Index of the head of the queue (next packet to be dequeued). */
  int tail;                                          /**< Index of the tail of the queue (next position to enqueue a packet). */
  int count;                                         /**< Current number of packets in the queue. */
} ASPEP_Received_packet_queue_t;

/**
  * @struct ASPEP_Handle_t
  * @brief Represents the handle for the ASPEP module.
  */
typedef struct
{
  void *HWIp;                                /**< Pointer to the hardware interface. */
  uint8_t txHeader[ASPEP_HEADER_SIZE];       /**< Buffer containing the ASPEP 32-bit transmit header. */
  uint8_t rxHeader[ASPEP_HEADER_SIZE];       /**< Buffer containing the ASPEP 32-bit receive header. */
  ASPEP_CTRL_TL_Buff_t syncRxBuffer;             /**< Synchronous receive buffer. */
  ASPEP_CTRL_TL_Buff_t syncTxBuffer;             /**< Synchronous transmit buffer. */
  ASPEP_CTRL_TL_Buff_t asyncBufferA;             /**< Asynchronous buffer A. */
  ASPEP_CTRL_TL_Buff_t asyncBufferB;             /**< Asynchronous buffer B. */
  ASPEP_CTRL_TL_Buff_t *LastUsedAsyncBuff;       /**< Pointer to the last used asynchronous buffer. */
  uint16_t maxRXPayload;                    /**< Maximum payload size for received packets. */
  ASPEP_CTRL_sm_state_t ASPEP_State;                /**< Current state of the ASPEP state machine. */
  ASPEP_CTRL_PacketType rxPacketType;            /**< Type of the received packet. */
  ASPEP_CTRL_PingConfType rxPingCbit;            /**< Ping configuration bit for received packets. */
  ASPEP_CTRL_Capabilities_def Capabilities;      /**< Capabilities of the ASPEP module. */
  uint8_t badPacketFlag;                    /**< Flag indicating a bad packet was received. */
  uint16_t packetNumber;                    /**< Current packet number being processed. */
  uint8_t *rxFrameProcessData;              /**< Pointer to the data being processed in the received frame. */
  uint16_t rxFrameProcessLength;            /**< Length of the data being processed in the received frame. */
  ASPEP_Received_packet_queue_t received_packet_queue; /**< Queue for received packets. */
  ASPEP_CTRL_TL_sm_t ASPEP_TL_State;          /**< Current state of the ASPEP Transport Layer state machine. */
  ASPEP_CTRL_Event_t aspep_evt;             /**< Event type for ASPEP control. */
  bool PacketReady;                         /**< Flag indicating if a packet is ready for processing. */
  uint16_t txSyncMaxPayload;                /**< Maximum payload size for synchronous transmissions. */
  uint16_t txAsyncMaxPayload;               /**< Maximum payload size for asynchronous transmissions. */
  ASPEP_hwinit_cb_t fASPEP_HWInit;          /**< Pointer to the hardware initialization function. */
  ASPEP_hwsync_cb_t fASPEP_HWSync;          /**< Pointer to the hardware synchronization function. */
  ASPEP_config_reception_cb_t fASPEP_cfg_recept; /**< Pointer to the packet reception configuration function. */
  ASPEP_config_transmission_cb_t fASPEP_cfg_trans; /**< Pointer to the packet transmission configuration function. */

  /* Callback functions */
  ASPEP_send_complete_cb_t ASPEP_send_complete_cb; /**< Callback invoked when a send operation is complete. */
  ASPEP_after_sending_cb_t ASPEP_after_sending_cb; /**< Callback invoked after a packet is sent. */
  ASPEP_receive_new_packet_cb_t ASPEP_receive_new_packet_cb; /**< Callback invoked when a new packet is received. */
  ASPEP_delay_cb_t ASPEP_delay_cb;               /**< Callback for introducing delays. */
} ASPEP_CTRL_Handle_t;

/* Private function prototypes -----------------------------------------------*/
static void ASPEP_CTRL_TXframeProcess(void *txBuffer, uint16_t bufferLength);
static void ASPEP_CTRL_sm(ASPEP_CTRL_sm_state_t state, ASPEP_CTRL_PacketType packet);
static void ASPEP_CTRL_Configure_sm(ASPEP_CTRL_PacketType packet);
static void ASPEP_CTRL_Connecting_sm(ASPEP_CTRL_PacketType packet);
static void ASPEP_CTRL_Connected_sm(ASPEP_CTRL_PacketType packet);
static void ASPEP_CTRL_TXframeProcessNoCrc(void *txBuffer, uint16_t bufferLength);
static void ASPEP_CTRL_ComputeHeaderCRC(uint32_t *headerPtr);
static inline void ASPEP_CTRL_HW_Init(ASPEP_CTRL_Handle_t *const p_handle, void *p_HWIp);
static inline void ASPEP_CTRL_HW_Receive(ASPEP_CTRL_Handle_t *const p_handle, void *rxbuffer, uint16_t length);
static inline void ASPEP_CTRL_HW_Transmit(ASPEP_CTRL_Handle_t *const p_handle, void *txbuffer, uint16_t length);
static inline void ASPEP_CTRL_HW_Sync(ASPEP_CTRL_Handle_t *const p_handle);
static inline uint32_t ASPEP_CTRL_GetPacketLength(uint32_t *const p_header);
static inline void ASPEP_CTRL_GetAsyncBuffer(ASPEP_CTRL_Handle_t *handle, ASPEP_CTRL_TL_Buff_t **buffer);
static bool ASPEP_CTRL_ReceivedPacketQueueAddItem(ASPEP_Received_packet_queue_t *const p_queue,
                                                  ASPEP_CTRL_PacketType item);
static bool ASPEP_CTRL_ReceivedPacketQueueRemoveItem(ASPEP_Received_packet_queue_t *const p_queue,
                                                     ASPEP_CTRL_PacketType *p_item);
static bool ASPEP_CTRL_CheckBeacon(ASPEP_CTRL_Handle_t *pHandle, ASPEP_CTRL_Capabilities_def *SlaveCapabilities);
static void ASPEP_CTRL_SendBeacon(void);
static void ASPEP_CTRL_SendPing(void);
static bool ASPEP_CTRL_CheckHeaderCRC(uint32_t header);

/* Private variables ---------------------------------------------------------*/
uint8_t aspepTxSyncBuffer[ASPEP_CTRL_TX_SYNC_BUFFER_SIZE] __attribute__((aligned(4)));;
uint8_t aspepRxSyncBuffer[ASPEP_CTRL_RX_SYNC_BUFFER_SIZE] __attribute__((aligned(4)));;
uint8_t aspepAsyncBuffA[ASPEP_CTRL_TX_ASYNC_BUFFER_SIZE] __attribute__((aligned(4)));;
uint8_t aspepAsyncBuffB[ASPEP_CTRL_TX_ASYNC_BUFFER_SIZE] __attribute__((aligned(4)));;

static ASPEP_CTRL_Handle_t aspepCtrlHandle;

static UASPEP_Handle_t UASPEP_A =
{
#if ASPEP_CTRL_USE_DMA == 1
  .USARTx = ASPEP_CTRL_USART_A,
  .rxDMA = ASPEP_CTRL_DMA_RX_A,
  .txDMA = ASPEP_CTRL_DMA_TX_A,
  .rxChannel = ASPEP_CTRL_DMA_RX_CH_A,
  .txChannel = ASPEP_CTRL_DMA_TX_CH_A,
#else
  .huartn = ASPEP_USART_HANDLE,
  .rxDMA = 0,
  .txDMA = 0,
  .rxChannel = 0,
  .txChannel = 0,
#endif
};

uint8_t const CRC4_Lookup4[] =
{
  /*   0,    1,    2,    3,    4,    5,    6,    7,    8,    9,   A,     B,    C,    D,    E,    F */
  0x00, 0x07, 0x0e, 0x09, 0x0b, 0x0c, 0x05, 0x02, 0x01, 0x06, 0x0f, 0x08, 0x0a, 0x0d, 0x04, 0x03
};

#ifndef CRC4_PER_NIBBLE
uint8_t const CRC4_Lookup8[] =
{
  /*          0,    1,    2,    3,    4,    5,    6,    7,    8,    9,   A,     B,    C,    D,    E,    F */
  /* 0 */  0x00, 0x02, 0x04, 0x06, 0x08, 0x0a, 0x0c, 0x0e, 0x07, 0x05, 0x03, 0x01, 0x0f, 0x0d, 0x0b, 0x09,
  /* 1 */  0x07, 0x05, 0x03, 0x01, 0x0f, 0x0d, 0x0b, 0x09, 0x00, 0x02, 0x04, 0x06, 0x08, 0x0a, 0x0c, 0x0e,
  /* 2 */  0x0e, 0x0c, 0x0a, 0x08, 0x06, 0x04, 0x02, 0x00, 0x09, 0x0b, 0x0d, 0x0f, 0x01, 0x03, 0x05, 0x07,
  /* 3 */  0x09, 0x0b, 0x0d, 0x0f, 0x01, 0x03, 0x05, 0x07, 0x0e, 0x0c, 0x0a, 0x08, 0x06, 0x04, 0x02, 0x00,
  /* 4 */  0x0b, 0x09, 0x0f, 0x0d, 0x03, 0x01, 0x07, 0x05, 0x0c, 0x0e, 0x08, 0x0a, 0x04, 0x06, 0x00, 0x02,
  /* 5 */  0x0c, 0x0e, 0x08, 0x0a, 0x04, 0x06, 0x00, 0x02, 0x0b, 0x09, 0x0f, 0x0d, 0x03, 0x01, 0x07, 0x05,
  /* 6 */  0x05, 0x07, 0x01, 0x03, 0x0d, 0x0f, 0x09, 0x0b, 0x02, 0x00, 0x06, 0x04, 0x0a, 0x08, 0x0e, 0x0c,
  /* 7 */  0x02, 0x00, 0x06, 0x04, 0x0a, 0x08, 0x0e, 0x0c, 0x05, 0x07, 0x01, 0x03, 0x0d, 0x0f, 0x09, 0x0b,
  /* 8 */  0x01, 0x03, 0x05, 0x07, 0x09, 0x0b, 0x0d, 0x0f, 0x06, 0x04, 0x02, 0x00, 0x0e, 0x0c, 0x0a, 0x08,
  /* 9 */  0x06, 0x04, 0x02, 0x00, 0x0e, 0x0c, 0x0a, 0x08, 0x01, 0x03, 0x05, 0x07, 0x09, 0x0b, 0x0d, 0x0f,
  /* A */  0x0f, 0x0d, 0x0b, 0x09, 0x07, 0x05, 0x03, 0x01, 0x08, 0x0a, 0x0c, 0x0e, 0x00, 0x02, 0x04, 0x06,
  /* B */  0x08, 0x0a, 0x0c, 0x0e, 0x00, 0x02, 0x04, 0x06, 0x0f, 0x0d, 0x0b, 0x09, 0x07, 0x05, 0x03, 0x01,
  /* C */  0x0a, 0x08, 0x0e, 0x0c, 0x02, 0x00, 0x06, 0x04, 0x0d, 0x0f, 0x09, 0x0b, 0x05, 0x07, 0x01, 0x03,
  /* D */  0x0d, 0x0f, 0x09, 0x0b, 0x05, 0x07, 0x01, 0x03, 0x0a, 0x08, 0x0e, 0x0c, 0x02, 0x00, 0x06, 0x04,
  /* E */  0x04, 0x06, 0x00, 0x02, 0x0c, 0x0e, 0x08, 0x0a, 0x03, 0x01, 0x07, 0x05, 0x0b, 0x09, 0x0f, 0x0d,
  /* F */  0x03, 0x01, 0x07, 0x05, 0x0b, 0x09, 0x0f, 0x0d, 0x04, 0x06, 0x00, 0x02, 0x0c, 0x0e, 0x08, 0x0a
};
#endif /* CRC4_PER_NIBBLE */

/* Public function definition ------------------------------------------------*/

/**
  * @brief Initializes the ASPEP control and registers the ASPEP UART transport protocol.
  *
  * This function initializes the ASPEP control by registering UART transport Layer needed for
  * hardware communication and sets internal states and capabilities of the ASPEP handle.
  *
  * @param[in] p_ASPEP_Transport_Layer Pointer to the ASPEP transport layer definition structure.
  *
  * @return ASPEP_RetVal_t Returns ASPEP_NULL_POINTER_ERROR in case of bad input parameters.
  *                                ASPEP_SUCCESS if initialization is successful.
  */
ASPEP_RetVal_t ASPEP_CTRL_Init(ASPEP_Transport_Layer_def_t *const p_ASPEP_Transport_Layer)
{
  ASPEP_RetVal_t retVal = ASPEP_NULL_POINTER_ERROR;

  /* Check if the input pointers are not NULL */
  if (NULL != p_ASPEP_Transport_Layer)
  {
    /* Check if the required callbacks are not NULL */
    if (NULL != p_ASPEP_Transport_Layer->ASPEP_hwinit_cb &&
        NULL != p_ASPEP_Transport_Layer->ASPEP_hwsync_cb &&
        NULL != p_ASPEP_Transport_Layer->ASPEP_config_transmission_cb &&
        NULL != p_ASPEP_Transport_Layer->ASPEP_config_reception_cb)
    {
      /* Initialize the ASPEP handle with the provided UASPEP handle */
      aspepCtrlHandle.HWIp = (void *)&UASPEP_A;

      /* Register ASPEP Transport Layer function pointer */
      aspepCtrlHandle.fASPEP_HWInit = p_ASPEP_Transport_Layer->ASPEP_hwinit_cb;
      aspepCtrlHandle.fASPEP_HWSync = p_ASPEP_Transport_Layer->ASPEP_hwsync_cb;
      aspepCtrlHandle.fASPEP_cfg_recept = p_ASPEP_Transport_Layer->ASPEP_config_reception_cb;
      aspepCtrlHandle.fASPEP_cfg_trans = p_ASPEP_Transport_Layer->ASPEP_config_transmission_cb;

      /* Initialize the ASPEP hardware */
      ASPEP_CTRL_HW_Init(&aspepCtrlHandle, aspepCtrlHandle.HWIp);

      /* Set Master Capabilities */
      aspepCtrlHandle.Capabilities.DATA_CRC = 0;
      aspepCtrlHandle.Capabilities.RX_maxSize = (ASPEP_CTRL_RX_SYNC_BUFFER_SIZE >> 5) - 1;
      aspepCtrlHandle.Capabilities.TXS_maxSize = (ASPEP_CTRL_TX_SYNC_BUFFER_SIZE >> 5) - 1;
      aspepCtrlHandle.Capabilities.TXA_maxSize = ASPEP_CTRL_TX_ASYNC_BUFFER_SIZE >> 6;
      aspepCtrlHandle.Capabilities.version = 0x0;

      /* Set state machines initial states */
      aspepCtrlHandle.ASPEP_State = ASPEP_IDLE;
      aspepCtrlHandle.ASPEP_TL_State = WAITING_PACKET;

      /* Initialize packet number and counter */
      aspepCtrlHandle.packetNumber = 0;
      aspepCtrlHandle.badPacketFlag = 0;

      /* Initialize received packet queue */
      aspepCtrlHandle.received_packet_queue.max_size = ASPEP_RECEIVED_PACKET_QUEUE_LEN;
      aspepCtrlHandle.received_packet_queue.count = 0;
      aspepCtrlHandle.received_packet_queue.head = 0;
      aspepCtrlHandle.received_packet_queue.tail = 0;

      /* Allocate p_buffer for synchronous telemetry transmission */
      aspepCtrlHandle.syncTxBuffer.p_buffer = (uint8_t *)aspepTxSyncBuffer;

      /* Allocate p_buffer for synchronous telemetry reception */
      aspepCtrlHandle.syncRxBuffer.p_buffer = (uint8_t *)aspepRxSyncBuffer;

      /* Allocate buffers for asynchronous telemetry */
      aspepCtrlHandle.asyncBufferA.p_buffer = aspepAsyncBuffA;
      aspepCtrlHandle.asyncBufferB.p_buffer = aspepAsyncBuffB;

      /* Configure ASPEP master for receiving data */
      ASPEP_CTRL_HW_Receive(&aspepCtrlHandle, aspepCtrlHandle.rxHeader, ASPEP_HEADER_SIZE);

      retVal = ASPEP_SUCCESS;
    }
  }

  return retVal;
}

/**
  * @brief Starts the ASPEP connection process.
  *
  * This function initiates the ASPEP connection process by sending a beacon packet.
  * The establishment of the connection must be checked by verifying the reception of
  * ASPEP_CTRL_CONNECTED_EVT from ASPEP_CtrlGetEvent.
  */
void ASPEP_CTRL_StartConnection(void)
{
  ASPEP_CTRL_SendBeacon();
  aspepCtrlHandle.ASPEP_State = ASPEP_CONFIGURING;
}

/**
  * @brief Initiates the ASPEP disconnection process.
  *
  * This function initiates the ASPEP disconnection process by sending a beacon packet and setting
  * the ASPEP state to idle.
  */
void ASPEP_CTRL_Disconnection(void)
{
  aspepCtrlHandle.ASPEP_State = ASPEP_IDLE;
}

/**
  * @brief Sends an ASPEP packet.
  *
  * This function sends an ASPEP packet by preparing the header and processing the frame.
  * It ensures that the ASPEP state is connected before sending the packet.
  *
  * Before calling this function, the user must get the ASPEP transmission p_buffer by calling:
  * ASPEP_CtrlGetTxBuffer.
  *
  * @param[in] p_txBuffer Pointer to the transmission p_buffer.
  * @param[in] txBufferLength Length of the data to be transmitted.
  *
  * @return ASPEP_RetVal_t ASPEP_SUCCESS if the packet is sent successfully.
  *                        ASPEP_ERROR if there is an error in sending the packet.
  *                        ASPEP_NULL_POINTER_ERROR if the input p_buffer is NULL.
  */
ASPEP_RetVal_t ASPEP_CTRL_SendPacket(void *const p_txBuffer, uint16_t txBufferLength)
{
  uint8_t *packet;
  uint32_t *header;
  ASPEP_RetVal_t retVal = ASPEP_NULL_POINTER_ERROR;

  if (NULL != p_txBuffer)
  {
    retVal = ASPEP_ERROR;

    if (ASPEP_CONNECTED == aspepCtrlHandle.ASPEP_State)
    {
      packet = (uint8_t *) p_txBuffer;
      header = (uint32_t *)(packet - ASPEP_HEADER_SIZE);
      *header = (txBufferLength << 4) | DATA_PACKET;

      ASPEP_CTRL_ComputeHeaderCRC(header);
      ASPEP_CTRL_TXframeProcessNoCrc((uint8_t *)header, ASPEP_HEADER_SIZE);

      /* Add here a delay */
      aspepCtrlHandle.ASPEP_delay_cb(2);

      ASPEP_CTRL_TXframeProcessNoCrc(p_txBuffer, txBufferLength);

      retVal = ASPEP_SUCCESS;
    }
  }

  return retVal;
}

/**
  * @brief Gets the transmission p_buffer for ASPEP.
  *
  * This function provides a pointer to the transmission p_buffer for ASPEP. It ensures that the p_buffer
  * is available and locks it for writing.
  *
  * @param[out] p_buffer Pointer to the p_buffer pointer.
  *
  * @return ASPEP_RetVal_t ASPEP_SUCCESS if the p_buffer is successfully obtained.
  *                        ASPEP_ERROR if the p_buffer is not available.
  *                        ASPEP_NULL_POINTER_ERROR if the input p_buffer pointer is NULL.
  */
ASPEP_RetVal_t ASPEP_CTRL_GetTxBuffer(void **const p_buffer)
{
  ASPEP_RetVal_t retVal = ASPEP_NULL_POINTER_ERROR;

  if (NULL != p_buffer)
  {
    __disable_irq();

    retVal = ASPEP_ERROR;

    if (aspepCtrlHandle.syncTxBuffer.state <= WRITE_LOCK)
    {
      *p_buffer = &aspepCtrlHandle.syncTxBuffer.p_buffer[ASPEP_HEADER_SIZE];
      aspepCtrlHandle.syncTxBuffer.state = WRITE_LOCK;

      retVal = ASPEP_SUCCESS;
    }

    __enable_irq();
  }

  return retVal;
}

/**
  * @brief Processes the received ASPEP frame.
  *
  * This function processes the latest received frame by the ASPEP transport layer and, if a new packet is available,
  * returns it by setting the pointers *p_frame and *p_frameLength.
  *
  * @param[out] p_frame Pointer to the frame data.
  * @param[out] p_frameLength Pointer to the frame length.
  *
  * @return ASPEP_RetVal_t ASPEP_SUCCESS if a new packet is ready.
  *                        ASPEP_BAD_PACKET if a bad packet is detected.
  *                        ASPEP_NULL_POINTER_ERROR in case of bad input parameters.
  */
ASPEP_RetVal_t ASPEP_CTRL_RxFrameProcess(uint8_t **p_frame, uint16_t *p_frameLength)
{
  bool newPacketAvailable = false;
  ASPEP_CTRL_PacketType current_packet_type = 0;
  ASPEP_RetVal_t retVal = ASPEP_NULL_POINTER_ERROR;

  if (NULL != p_frame && NULL != p_frameLength)
  {
    *p_frame = NULL;
    *p_frameLength = 0;

    /* Check if a new packet is available and in case consume it. */
    newPacketAvailable = ASPEP_CTRL_ReceivedPacketQueueRemoveItem(&aspepCtrlHandle.received_packet_queue, &current_packet_type);

    if (newPacketAvailable)
    {

      /* Run ASPEP State machine */
      ASPEP_CTRL_sm(aspepCtrlHandle.ASPEP_State, (ASPEP_CTRL_PacketType) current_packet_type);

      *p_frame = aspepCtrlHandle.rxFrameProcessData;
      *p_frameLength = aspepCtrlHandle.rxFrameProcessLength;

      retVal = ASPEP_SUCCESS;
    }
    else if (aspepCtrlHandle.badPacketFlag > ASPEP_OK)
    {
      if (aspepCtrlHandle.badPacketFlag == ASPEP_BAD_CRC_HEADER || aspepCtrlHandle.badPacketFlag == ASPEP_BAD_PACKET_TYPE)
      {
        aspepCtrlHandle.badPacketFlag = ASPEP_OK;

        retVal = ASPEP_BAD_PACKET;
      }
      else
      {
        aspepCtrlHandle.badPacketFlag = ASPEP_OK;
      }
    }
  }

  return retVal;
}

/**
  * @brief Get the current ASPEP control event.
  *
  * This function retrieves the current event from the ASPEP handle. If the
  * clearEvent parameter is true, it will reset the event to ASPEP_CTRL_NONE_EVT.
  *
  * @param clearEvent A boolean flag indicating whether to clear the event after retrieval.
  * @return The current ASPEP control event.
  */
ASPEP_CTRL_Event_t ASPEP_CTRL_GetEvent(bool clearEvent)
{
  /* Retrieve the current event from the ASPEP handle */
  ASPEP_CTRL_Event_t retVal = aspepCtrlHandle.aspep_evt;

  /* If clearEvent is true, reset the event to none */
  if (clearEvent)
  {
    aspepCtrlHandle.aspep_evt = ASPEP_CTRL_NONE_EVT;
  }

  /* Return the current event */
  return retVal;
}

/**
  * @brief Frees the asynchronous buffer if it is in the READ_LOCK state.
  *
  * This function checks the state of the asynchronous buffers (asyncBufferA and asyncBufferB)
  * in the `aspepHandle` structure. If either buffer is in the `READ_LOCK` state, it changes
  * the state to `FREE_BUFF`, indicating that the buffer is now available for use.
  *
  * @note This function assumes that the `aspepHandle` structure and its buffers are properly
  * initialized before calling this function.
  */
void ASPEP_CTRL_FreeAsyncBuffer(void)
{
  if (aspepCtrlHandle.asyncBufferA.state == READ_LOCK)
  {
    aspepCtrlHandle.asyncBufferA.state = FREE_BUFF;
  }

  else if (aspepCtrlHandle.asyncBufferB.state == READ_LOCK)
  {
    aspepCtrlHandle.asyncBufferB.state = FREE_BUFF;
  }
}

/**
  * @brief Handles the reception of ASPEP packets.
  *
  * This function is called to process the received ASPEP packets based on the current state
  * of the ASPEP transport layer state machine. That are:
  * - WAITING_PACKET: Waiting for a new packet to be received.
  * - WAITING_PAYLOAD: Waiting for the payload of the packet to be received.
  */
void ASPEP_CTRL_ReceivedIT(void)
{
  switch (aspepCtrlHandle.ASPEP_TL_State)
  {
    case WAITING_PACKET:
    {
      if (ASPEP_CTRL_CheckHeaderCRC(*(uint32_t *) aspepCtrlHandle.rxHeader) == true)
      {
        aspepCtrlHandle.rxPacketType = (ASPEP_CTRL_PacketType)(aspepCtrlHandle.rxHeader[0] & ID_MASK);

        switch (aspepCtrlHandle.rxPacketType)
        {
          case RESPONSE:
          {
            /* Get incoming packet length */
            aspepCtrlHandle.syncRxBuffer.length = ASPEP_CTRL_GetPacketLength((uint32_t *) aspepCtrlHandle.rxHeader);

            /* Check packet length condition */
            if (aspepCtrlHandle.syncRxBuffer.length <= aspepCtrlHandle.maxRXPayload)
            {
              /* Compute received packet length payload in bytes*/
              uint16_t packet_length_byte = aspepCtrlHandle.syncRxBuffer.length + ASPEP_DATACRC_SIZE * aspepCtrlHandle.Capabilities.DATA_CRC;

              /* Configure UART to receive the incoming packet payload */
              ASPEP_CTRL_HW_Receive(&aspepCtrlHandle, aspepCtrlHandle.syncRxBuffer.p_buffer, packet_length_byte);

              aspepCtrlHandle.ASPEP_TL_State = WAITING_PAYLOAD;
            }
            else
            {
              aspepCtrlHandle.badPacketFlag = ASPEP_BAD_PACKET_SIZE;
            }
          }
          break;

          case ASYNC:
          {
            /* Get incoming payload packet length */
            uint32_t asynRxLength = ASPEP_CTRL_GetPacketLength((uint32_t *) aspepCtrlHandle.rxHeader);

            if (asynRxLength <= aspepCtrlHandle.txAsyncMaxPayload)
            {
              /* Get pointer to the async free buffer */
              ASPEP_CTRL_TL_Buff_t *p_async_buff = NULL;
              ASPEP_CTRL_GetAsyncBuffer(&aspepCtrlHandle, &p_async_buff);

              /* Set p_buffer incoming packet length */
              p_async_buff->length = asynRxLength;

              uint16_t paylaod_length_bytes = 0;
              paylaod_length_bytes = p_async_buff->length + ASPEP_DATACRC_SIZE * aspepCtrlHandle.Capabilities.DATA_CRC;

              /* Configure UART to receive the incoming packet payload */
              ASPEP_CTRL_HW_Receive(&aspepCtrlHandle, p_async_buff->p_buffer, paylaod_length_bytes);

              aspepCtrlHandle.ASPEP_TL_State = WAITING_PAYLOAD;
            }
          }
          break;

          case BEACON:
          {
            ASPEP_CTRL_ReceivedPacketQueueAddItem(&aspepCtrlHandle.received_packet_queue, BEACON);

            if (aspepCtrlHandle.ASPEP_receive_new_packet_cb)
            {
              aspepCtrlHandle.ASPEP_receive_new_packet_cb();
            }

            ASPEP_CTRL_HW_Receive(&aspepCtrlHandle, aspepCtrlHandle.rxHeader, ASPEP_HEADER_SIZE);
          }
          break;

          case PING:
          {
            ASPEP_CTRL_ReceivedPacketQueueAddItem(&aspepCtrlHandle.received_packet_queue, PING);

            if (aspepCtrlHandle.ASPEP_receive_new_packet_cb)
            {
              aspepCtrlHandle.ASPEP_receive_new_packet_cb();
            }

            aspepCtrlHandle.rxPingCbit = (ASPEP_CTRL_PingConfType)(aspepCtrlHandle.rxHeader[0] & ID_MASK_PING);

            ASPEP_CTRL_HW_Receive(&aspepCtrlHandle, aspepCtrlHandle.rxHeader, ASPEP_HEADER_SIZE);
          }
          break;

          default:
          {
            aspepCtrlHandle.badPacketFlag = ASPEP_BAD_PACKET_TYPE;
            ASPEP_CTRL_HW_Sync(&aspepCtrlHandle);
          }
          break;
        }
      }
      else
      {
        aspepCtrlHandle.badPacketFlag = ASPEP_BAD_CRC_HEADER;
      }
    }
    break;

    case WAITING_PAYLOAD:
    {
      /* Reload the UART for receiving the next packet header */
      ASPEP_CTRL_HW_Receive(&aspepCtrlHandle, aspepCtrlHandle.rxHeader, ASPEP_HEADER_SIZE);

      aspepCtrlHandle.ASPEP_TL_State = WAITING_PACKET;

      switch (aspepCtrlHandle.rxPacketType)
      {
        case RESPONSE:
        {
          /*Add the new RESPONSE packet into the queue, it will be consumed by ASPEP_RxFrameProcess */
          ASPEP_CTRL_ReceivedPacketQueueAddItem(&aspepCtrlHandle.received_packet_queue, RESPONSE);
        }
        break;

        case ASYNC:
        {
          /*Add the new  ASYNC packet into the queue, it will be consumed by ASPEP_RxFrameProcess */
          ASPEP_CTRL_ReceivedPacketQueueAddItem(&aspepCtrlHandle.received_packet_queue, ASYNC);

          if (aspepCtrlHandle.asyncBufferA.state == WRITE_LOCK)
          {
            aspepCtrlHandle.asyncBufferA.state = READ_LOCK;
          }
          else if (aspepCtrlHandle.asyncBufferB.state == WRITE_LOCK)
          {
            aspepCtrlHandle.asyncBufferB.state = READ_LOCK;
          }
        }
        break;

        default:
          break;
      }

      /*Notify registered observer that a new packet is available to be consumed */
      if (aspepCtrlHandle.ASPEP_receive_new_packet_cb)
      {
        aspepCtrlHandle.ASPEP_receive_new_packet_cb();
      }
    }
    break;
  }
}

void ASPEP_CTRL_DataTransmittedIT(void)
{
  if (NULL != aspepCtrlHandle.ASPEP_send_complete_cb)
  {
    aspepCtrlHandle.ASPEP_send_complete_cb();
  }
}

/* Private function definition -----------------------------------------------*/

/**
  * @brief Computes and appends a CRC4 checksum to the ASPEP header.
  *
  * @param[in,out] headerPtr Pointer to the 32-bit header. The CRC will be written
  *                          into the upper 4 bits of the value pointed to by headerPtr.
  */
static void ASPEP_CTRL_ComputeHeaderCRC(uint32_t *headerPtr)
{
  uint8_t crc = 0;
  uint32_t header = *headerPtr;

#ifndef CRC4_PER_NIBBLE
  header &= 0x0fffffff;

  crc = CRC4_Lookup8[ crc ^ (uint8_t)(header        & 0xff) ];
  crc = CRC4_Lookup8[ crc ^ (uint8_t)((header >> 8) & 0xff) ];
  crc = CRC4_Lookup8[ crc ^ (uint8_t)((header >> 16) & 0xff) ];
  crc = CRC4_Lookup4[ crc ^ (uint8_t)((header >> 24) & 0x0f) ];
#else /* CRC4_PER_NIBBLE */
  crc = CRC4_Lookup4[ crc ^ (uint8_t)(header        & 0xf) ];
  crc = CRC4_Lookup4[ crc ^ (uint8_t)((header >>  4) & 0xf) ];
  crc = CRC4_Lookup4[ crc ^ (uint8_t)((header >>  8) & 0xf) ];
  crc = CRC4_Lookup4[ crc ^ (uint8_t)((header >> 12) & 0xf) ];
  crc = CRC4_Lookup4[ crc ^ (uint8_t)((header >> 16) & 0xf) ];
  crc = CRC4_Lookup4[ crc ^ (uint8_t)((header >> 20) & 0xf) ];
  crc = CRC4_Lookup4[ crc ^ (uint8_t)((header >> 24) & 0xf) ];
#endif /* CRC4_PER_NIBBLE */

  *headerPtr |= (uint32_t)crc << 28;

  return;
}

/**
  * @brief Verifies the CRC4 checksum of the ASPEP header.
  *
  * This function checks the integrity of the given ASPEP header by recalculating
  * the CRC4 checksum and comparing it with the value stored in the header.
  *
  * @param[in] header The 32-bit ASPEP header containing the CRC4 checksum in its upper 4 bits.
  * @return true if the CRC4 checksum is valid, false otherwise.
  */
static bool ASPEP_CTRL_CheckHeaderCRC(uint32_t header)
{
  uint8_t crc = 0;

#ifndef CRC4_PER_NIBBLE
  crc = CRC4_Lookup8[ crc ^ (uint8_t)(header        & 0xff) ];
  crc = CRC4_Lookup8[ crc ^ (uint8_t)((header >> 8) & 0xff) ];
  crc = CRC4_Lookup8[ crc ^ (uint8_t)((header >> 16) & 0xff) ];
  crc = CRC4_Lookup8[ crc ^ (uint8_t)((header >> 24) & 0xff) ];
#else /* CRC4_PER_NIBBLE */
  crc = CRC4_Lookup4[ crc ^ (uint8_t)(header        & 0xf) ];
  crc = CRC4_Lookup4[ crc ^ (uint8_t)((header >>  4) & 0xf) ];
  crc = CRC4_Lookup4[ crc ^ (uint8_t)((header >>  8) & 0xf) ];
  crc = CRC4_Lookup4[ crc ^ (uint8_t)((header >> 12) & 0xf) ];
  crc = CRC4_Lookup4[ crc ^ (uint8_t)((header >> 16) & 0xf) ];
  crc = CRC4_Lookup4[ crc ^ (uint8_t)((header >> 20) & 0xf) ];
  crc = CRC4_Lookup4[ crc ^ (uint8_t)((header >> 24) & 0xf) ];
  crc = crc ^ (uint8_t)((header >> 28) & 0xf);
#endif /* CRC4_PER_NIBBLE */

  return crc == 0;
}

/**
  * @brief Checks the beacon packet and extracts slave capabilities.
  *
  * This function parses the received beacon packet header to extract the slave's capabilities,
  * such as protocol version, data CRC support, and maximum packet sizes. It then compares
  * these capabilities with the master's expected capabilities to ensure compatibility.
  *
  * @param[in]  pHandle           Pointer to the ASPEP control handle containing the master's capabilities and received header.
  * @param[out] SlaveCapabilities Pointer to a structure where the extracted slave capabilities will be stored.
  *
  * @return true if the slave's capabilities match the master's requirements, false otherwise.
  */
static bool ASPEP_CTRL_CheckBeacon(ASPEP_CTRL_Handle_t *pHandle, ASPEP_CTRL_Capabilities_def *SlaveCapabilities)
{
  uint32_t packetHeader = *((uint32_t *)pHandle->rxHeader);
  bool result = true;
  SlaveCapabilities->version = (packetHeader & 0x70) >> 4; /*Bits 4 to 6*/
  SlaveCapabilities->DATA_CRC = pHandle->rxHeader[0] >> 7 ;      /*Bit 7 */
  SlaveCapabilities->RX_maxSize =  pHandle->rxHeader[1] & 0x3F; /*Bits 8 to  13*/
  SlaveCapabilities->TXS_maxSize = (packetHeader & 0x1FC000)  >> 14; /*Bits 14 to 20 */
  SlaveCapabilities->TXA_maxSize = (packetHeader & 0xFE00000) >> 21; /*Bits 21 to 27  */
  if ((SlaveCapabilities->DATA_CRC != pHandle->Capabilities.DATA_CRC) ||
      (SlaveCapabilities->RX_maxSize != pHandle->Capabilities.RX_maxSize)
      || /* Data packet the master can send is bigger than slave can receive */
      (pHandle->Capabilities.TXS_maxSize != SlaveCapabilities->TXS_maxSize)
      ||  /*Sync packet size alignment is required in order for the master to be able to store it, and to not request a response bigger than slave capability */
      (pHandle->Capabilities.TXA_maxSize != SlaveCapabilities->TXA_maxSize)
      ||  /*Async packet the slave can send is bigger than the master can receive (but master must not request a bigger packet than slave can transmit */
      (SlaveCapabilities->version !=
       pHandle->Capabilities.version))  /* For the time being, master version has to match slave version */
  {
    result = false;
  }
  return result;
}

static void ASPEP_CTRL_SendPing(void)
{
  uint32_t *packet = (uint32_t *) aspepCtrlHandle.txHeader;
  uint8_t Cbit = 0; //In Master Ping: reserved and set to 0
  uint8_t Nbit = 0; //In Master Ping: reserved and set to 0
  uint8_t ipID = 0; //In Master Ping: reserved and set to 0
  *packet = PING | (Cbit << 4) | (Cbit << 5) | (Nbit << 6) | (Nbit << 7) | (ipID << 8) | ((aspepCtrlHandle.packetNumber) << 12);

  ASPEP_CTRL_TXframeProcess(aspepCtrlHandle.txHeader, ASPEP_HEADER_SIZE);

  aspepCtrlHandle.packetNumber++;
}

/**
  * @brief Sends a beacon packet.
  *
  * This function constructs and sends a beacon packet containing the ASPEP handle's capabilities.
  * The beacon packet includes information such as version, data CRC, and maximum sizes for
  * reception and transmission.
  *
  * @note Calling this function in a connection state will cause the slave to disconnect by setting it into a configuring state.
  */
static void ASPEP_CTRL_SendBeacon(void)
{
  uint32_t *packet = (uint32_t *) aspepCtrlHandle.txHeader;
  *packet = (BEACON |
             (aspepCtrlHandle.Capabilities.version << 4) |
             (aspepCtrlHandle.Capabilities.DATA_CRC << 7) |
             (((uint32_t)aspepCtrlHandle.Capabilities.RX_maxSize) << 8) |
             (((uint32_t)aspepCtrlHandle.Capabilities.TXS_maxSize) << 14) |
             (((uint32_t)aspepCtrlHandle.Capabilities.TXA_maxSize) << 21));

  ASPEP_CTRL_TXframeProcess(aspepCtrlHandle.txHeader, ASPEP_HEADER_SIZE);
}

static void ASPEP_CTRL_TXframeProcess(void *txBuffer, uint16_t bufferLength)
{
  ASPEP_CTRL_ComputeHeaderCRC((uint32_t *)txBuffer);

  ASPEP_CTRL_HW_Transmit(&aspepCtrlHandle, txBuffer, bufferLength);

  if (NULL != aspepCtrlHandle.ASPEP_after_sending_cb)
  {
    aspepCtrlHandle.ASPEP_after_sending_cb();
  }
}

static void ASPEP_CTRL_TXframeProcessNoCrc(void *txBuffer, uint16_t bufferLength)
{
  ASPEP_CTRL_HW_Transmit(&aspepCtrlHandle, txBuffer, bufferLength);

  if (NULL != aspepCtrlHandle.ASPEP_after_sending_cb)
  {
    aspepCtrlHandle.ASPEP_after_sending_cb();
  }
}

static inline void ASPEP_CTRL_HW_Init(ASPEP_CTRL_Handle_t *const p_handle, void *p_HWIp)
{
  p_handle->fASPEP_HWInit(p_HWIp);
}

static inline void ASPEP_CTRL_HW_Receive(ASPEP_CTRL_Handle_t *const p_handle, void *rxbuffer, uint16_t length)
{
  p_handle->fASPEP_cfg_recept(p_handle->HWIp, rxbuffer, length);
}

static inline void ASPEP_CTRL_HW_Transmit(ASPEP_CTRL_Handle_t *const p_handle, void *txbuffer, uint16_t length)
{
  p_handle->fASPEP_cfg_trans(p_handle->HWIp, txbuffer, length);
}

static inline void ASPEP_CTRL_HW_Sync(ASPEP_CTRL_Handle_t *const p_handle)
{
  p_handle->fASPEP_HWSync(p_handle->HWIp);
}

/**
  * @brief Adds a received packet to the ASPEP received packet queue.
  *
  * @param[in,out] p_queue Pointer to the received packet queue structure.
  * @param[in]     item    The received packet to be added to the queue.
  *
  * @return true if the packet was successfully added to the queue, false if the queue is full.
  */
static bool ASPEP_CTRL_ReceivedPacketQueueAddItem(ASPEP_Received_packet_queue_t *const p_queue,
                                                  ASPEP_CTRL_PacketType item)
{
  bool retVal = true;

  __disable_irq();
  if (p_queue->count == p_queue->max_size)
  {
    retVal = false;
  }
  else
  {
    p_queue->p_buffer[p_queue->tail] = item;
    p_queue->tail = (p_queue->tail + 1) % p_queue->max_size;
    p_queue->count++;
  }

  __enable_irq();

  return retVal;
}

/**
  * @brief Removes a received packet from the ASPEP received packet queue.
  *
  * @param[in,out] p_queue Pointer to the received packet queue structure.
  * @param[out]    p_item  Pointer to where the removed packet will be stored.
  *
  * @return true if a packet was successfully removed from the queue, false if the queue is empty.
  */
static bool ASPEP_CTRL_ReceivedPacketQueueRemoveItem(ASPEP_Received_packet_queue_t *const p_queue,
                                                     ASPEP_CTRL_PacketType *p_item)
{
  bool retVal = true;

  __disable_irq();

  if (p_queue->count == 0)
  {
    retVal = false;
  }
  else
  {
    *p_item = p_queue->p_buffer[p_queue->head];
    p_queue->head = (p_queue->head + 1) % p_queue->max_size;
    p_queue->count--;
  }
  __enable_irq();

  return retVal;
}

/**
  * @brief Get the packet length from the header.
  *
  * This function extracts the packet length from the provided header.
  * The length is extracted by masking the relevant bits and shifting them.
  *
  * @param[in] p_header Pointer to the header from which to extract the packet length.
  * @return uint32_t The extracted packet length.
  */
static inline uint32_t ASPEP_CTRL_GetPacketLength(uint32_t *const p_header)
{
  uint32_t length = 0;
  length = (*p_header & 0x0001FFF0) >> 4;
  return length;
}

/**
  * @brief  Selects the appropriate asynchronous p_buffer for receiving data.
  * @param  handle: Pointer to the ASPEP handle structure.
  * @param  p_buffer: Pointer to a BufferType* that will be set to the selected p_buffer.
  * @retval None
  *
  * This function checks the state of the asynchronous buffers (asyncBufferA and asyncBufferB).
  * If a p_buffer is free, it sets its state to writeLock and assigns it to the output parameter.
  * If both buffers are locked, it assigns the last used p_buffer to the output parameter.
  */
static inline void ASPEP_CTRL_GetAsyncBuffer(ASPEP_CTRL_Handle_t *handle, ASPEP_CTRL_TL_Buff_t **buffer)
{
  if (handle->asyncBufferA.state == FREE_BUFF)
  {
    handle->asyncBufferA.state = WRITE_LOCK;
    handle->LastUsedAsyncBuff = &handle->asyncBufferA;

    *buffer = &handle->asyncBufferA;
  }
  else if (handle->asyncBufferB.state == FREE_BUFF)
  {
    handle->asyncBufferB.state = WRITE_LOCK;
    handle->LastUsedAsyncBuff = &handle->asyncBufferB;

    *buffer = &handle->asyncBufferB;
  }
  else
  {
    handle->LastUsedAsyncBuff->state = WRITE_LOCK;
    *buffer = handle->LastUsedAsyncBuff;
  }
}

/**
  * @brief Handles the ASPEP controller configuration state machine.
  *
  * This function processes incoming packets during the configuration phase of the ASPEP controller.
  * It manages the negotiation of capabilities between the controller (master) and the target (slave)
  *
  * @param[in] packet The received packet type to process (e.g., PING, BEACON).
  *
  * @note
  * - On receiving a PING, the controller sends a BEACON to initiate configuration.
  * - On receiving a BEACON, the function checks the slave's capabilities:
  *   - If capabilities do not match, it updates the controller's capabilities to the minimum supported values and resends a BEACON.
  *   - If capabilities match, it calculates payload sizes, sends a PING, and transitions to the CONNECTING state.
  */
static void ASPEP_CTRL_Configure_sm(ASPEP_CTRL_PacketType packet)
{
  ASPEP_CTRL_Capabilities_def SlaveCapabilities;

  switch (packet)
  {
    case PING:
    {
      /* ASPEP Controller and target unconfigured, send BEACON to start configuration phase.*/
      ASPEP_CTRL_SendBeacon();
    }
    break;

    case BEACON:
    {
      if (ASPEP_CTRL_CheckBeacon(&aspepCtrlHandle, &SlaveCapabilities) == false) //Beacon NOK
      {
        aspepCtrlHandle.Capabilities.DATA_CRC = MIN(aspepCtrlHandle.Capabilities.DATA_CRC, SlaveCapabilities.DATA_CRC);
        aspepCtrlHandle.Capabilities.RX_maxSize = MIN(aspepCtrlHandle.Capabilities.RX_maxSize, SlaveCapabilities.RX_maxSize);
        aspepCtrlHandle.Capabilities.TXS_maxSize = MIN(aspepCtrlHandle.Capabilities.TXS_maxSize, SlaveCapabilities.TXS_maxSize);
        aspepCtrlHandle.Capabilities.TXA_maxSize = MIN(aspepCtrlHandle.Capabilities.TXA_maxSize, SlaveCapabilities.TXA_maxSize);

        ASPEP_CTRL_SendBeacon();

      }
      else
      {
        /*  beacon OK : Slave capabilities match Master capabilities.*/
        aspepCtrlHandle.txSyncMaxPayload = (aspepCtrlHandle.Capabilities.TXS_maxSize + 1) * 32;
        aspepCtrlHandle.txAsyncMaxPayload = (aspepCtrlHandle.Capabilities.TXA_maxSize) * 64;
        aspepCtrlHandle.maxRXPayload = (aspepCtrlHandle.Capabilities.RX_maxSize + 1) * 32;

        ASPEP_CTRL_SendPing();
        aspepCtrlHandle.ASPEP_State = ASPEP_CONNECTING;
      }
      break;

      case RESPONSE:
      case ERROR_PACKET:
      case ASYNC:
      default:
        break;
      }
  }
}

static void ASPEP_CTRL_Connecting_sm(ASPEP_CTRL_PacketType packet)
{
  switch (packet)
  {
    case BEACON:
    {
      ASPEP_CTRL_Capabilities_def SlaveCapabilities;
      if (ASPEP_CTRL_CheckBeacon(&aspepCtrlHandle, &SlaveCapabilities) == false) //Unexpected : Beacon NOK
      {
        aspepCtrlHandle.Capabilities.DATA_CRC =    MIN(aspepCtrlHandle.Capabilities.DATA_CRC, SlaveCapabilities.DATA_CRC);
        aspepCtrlHandle.Capabilities.TXS_maxSize = MIN(aspepCtrlHandle.Capabilities.TXS_maxSize, SlaveCapabilities.TXS_maxSize);
        aspepCtrlHandle.Capabilities.TXA_maxSize = MIN(aspepCtrlHandle.Capabilities.TXA_maxSize, SlaveCapabilities.TXA_maxSize);
        ASPEP_CTRL_SendBeacon();
        aspepCtrlHandle.ASPEP_State = ASPEP_CONFIGURING;

      }
      else /*  beacon OK : Slave capabilities match Master capabilities.*/
      {
        ASPEP_CTRL_SendPing();

      }
    }
    break;

    case PING: //configured

      if (ASPEP_CTRL_PING_CONFIGURED == aspepCtrlHandle.rxPingCbit)
      {
        aspepCtrlHandle.ASPEP_State = ASPEP_CONNECTED;
        aspepCtrlHandle.aspep_evt = ASPEP_CTRL_CONNECTED_EVT;
      }
      break;

    default: //RESPONSE or ERROR
      ASPEP_CTRL_SendBeacon();
      aspepCtrlHandle.ASPEP_State = ASPEP_CONFIGURING;

      break;
  }
}

/**
  * @brief Handles the ASPEP controller state machine during the connecting phase.
  *
  * This function processes incoming packets while the ASPEP controller is in the
  * CONNECTING state.
  *
  * @param[in] packet The received packet type to process (e.g., BEACON, PING).
  */
static void ASPEP_CTRL_Connected_sm(ASPEP_CTRL_PacketType packet)
{
  switch (packet)
  {
    case RESPONSE:

      aspepCtrlHandle.rxFrameProcessLength = aspepCtrlHandle.syncRxBuffer.length;
      aspepCtrlHandle.rxFrameProcessData   = aspepCtrlHandle.syncRxBuffer.p_buffer;


      aspepCtrlHandle.aspep_evt = ASPEP_CTRL_RESPONSE_EVT;

      aspepCtrlHandle.syncRxBuffer.length = 0;

      break;

    case ASYNC:
      if (aspepCtrlHandle.asyncBufferA.state == READ_LOCK)
      {
        aspepCtrlHandle.rxFrameProcessLength = aspepCtrlHandle.asyncBufferA.length;
        aspepCtrlHandle.rxFrameProcessData   = aspepCtrlHandle.asyncBufferA.p_buffer;
        aspepCtrlHandle.asyncBufferA.length = 0;
      }

      else if (aspepCtrlHandle.asyncBufferB.state == READ_LOCK)
      {
        aspepCtrlHandle.rxFrameProcessLength = aspepCtrlHandle.asyncBufferB.length;
        aspepCtrlHandle.rxFrameProcessData = aspepCtrlHandle.asyncBufferB.p_buffer;
        aspepCtrlHandle.asyncBufferB.length = 0;
      }

      aspepCtrlHandle.aspep_evt = ASPEP_CTRL_ASYNC_EVT;
      break;

    case ERROR_PACKET:
      aspepCtrlHandle.PacketReady = true; //Send again last sent packet

      break;

    default:
      break;
  }
}

/**
  * @brief Handles the ASPEP Control state machine transitions.
  *
  * @param state  The current state of the ASPEP control state machine.
  * @param packet The packet to be processed by the state machine.
  */
static void ASPEP_CTRL_sm(ASPEP_CTRL_sm_state_t state, ASPEP_CTRL_PacketType packet)
{
  switch (state)
  {
    case ASPEP_IDLE:
    case ASPEP_CONFIGURING:
    {
      ASPEP_CTRL_Configure_sm(packet);
    }
    break;

    case ASPEP_CONNECTING:
    {
      ASPEP_CTRL_Connecting_sm(packet);
    }
    break;

    case ASPEP_CONNECTED:
    {
      ASPEP_CTRL_Connected_sm(packet);
    }
    break;

    default:
      break;
  }
}

void ASPEP_CTRL_HWDMAReset(void)
{
  /* We must reset the RX state machine to be sure to not be in Waiting packet state */
  /* Otherwise the arrival of a new packet will trigger a NewPacketAvailable despite */
  /* the fact that bytes have been lost because of overrun (debugger paused for instance) */
  aspepCtrlHandle.asyncBufferA.state = FREE_BUFF;
  aspepCtrlHandle.asyncBufferB.state = FREE_BUFF;

  aspepCtrlHandle.ASPEP_TL_State = WAITING_PACKET;
  ASPEP_CTRL_HW_Receive(&aspepCtrlHandle, aspepCtrlHandle.rxHeader, ASPEP_HEADER_SIZE);
}

/* ASPEP Controller Callback -------------------------------------------------*/

/**
  * @brief Registers a callback for receiving new ASPEP packets.
  *
  * This function registers a callback function that will be called when a new ASPEP packet is received.
  *
  * **Attention:** This function is called within the ISR of the ASPEP transport layer.
  * It can be used to notify the application level but should not be used to implement
  * time-consuming procedures as it can cause loss of packets in the protocol.
  *
  * @param[in] ASPEP_receive_new_packet_cb The callback function to be registered.
  *
  * @return ASPEP_RetVal_t ASPEP_SUCCESS if the callback is registered successfully.
  *                        ASPEP_NULL_POINTER_ERROR if the input callback is NULL.
  */
ASPEP_RetVal_t ASPEP_CTRL_RegisterNewPacket_cb(ASPEP_receive_new_packet_cb_t ASPEP_receive_new_packet_cb)
{
  ASPEP_RetVal_t retVal = ASPEP_NULL_POINTER_ERROR;

  if (NULL != ASPEP_receive_new_packet_cb)
  {
    aspepCtrlHandle.ASPEP_receive_new_packet_cb = ASPEP_receive_new_packet_cb;

    retVal = ASPEP_SUCCESS;
  }

  return retVal;
}

/**
  * @brief Registers a callback for transmission completion of the ASPEP transport layer.
  *
  * This function registers a callback function that will be called upon the completion of a transmission
  * in the ASPEP transport layer.
  *
  * **Attention:** This function is called within the ISR of the ASPEP transport layer.
  * It can be used to notify the application level but should not be used to implement
  * time-consuming procedures as it can cause loss of packets in the protocol.
  *
  * @param[in] ASPEP_send_complete_cb The callback function to be registered.
  *
  * @return ASPEP_RetVal_t ASPEP_SUCCESS if the callback is registered successfully.
  *                        ASPEP_NULL_POINTER_ERROR if the input callback is NULL.
  */
ASPEP_RetVal_t ASPEP_CTRL_RegisterTxComplete_cb(ASPEP_send_complete_cb_t ASPEP_send_complete_cb)
{
  ASPEP_RetVal_t retVal = ASPEP_NULL_POINTER_ERROR;

  if (NULL != ASPEP_send_complete_cb)
  {
    aspepCtrlHandle.ASPEP_send_complete_cb = ASPEP_send_complete_cb;

    retVal = ASPEP_SUCCESS;
  }

  return retVal;
}

void ASPEP_CTRL_RegisterTransmitting_cb(ASPEP_after_sending_cb_t ASPEP_after_sending_cb)
{
  if (NULL != ASPEP_after_sending_cb)
  {
    aspepCtrlHandle.ASPEP_after_sending_cb = ASPEP_after_sending_cb;
  }
}

void ASPEP_CTRL_RegisterDelay_cb(ASPEP_delay_cb_t ASPEP_delay_cb)
{
  if (NULL != ASPEP_delay_cb)
  {
    aspepCtrlHandle.ASPEP_delay_cb = ASPEP_delay_cb;
  }
}
