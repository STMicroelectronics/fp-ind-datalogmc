/**
  ******************************************************************************
  * @file    mcptl.h
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
 
/* Define to prevent recursive inclusion -------------------------------------*/ 
#ifndef MC_TRANSPORT_LAYER
#define MC_TRANSPORT_LAYER

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stdint.h"
#include "stdbool.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
//MCTL
typedef struct MCTL_Handle MCTL_Handle_t;

typedef bool (* MCTL_GetBuf ) (MCTL_Handle_t *pHandle, void **buffer);
typedef void (* MCTL_SendPacket ) (MCTL_Handle_t *pHandle, void *txBuffer, uint16_t txDataLength);

typedef void (* MCTL_StartConnection ) (MCTL_Handle_t *pSupHandle);
typedef void (* MCTL_Disconnection ) (MCTL_Handle_t *pSupHandle);
typedef uint8_t (* MCTL_GetStatus ) (MCTL_Handle_t *pSupHandle, uint8_t Flag, bool ClearFlag);
typedef void (* MCTL_FreeBuffer) (MCTL_Handle_t *pSupHandle);

typedef uint8_t* (* MCTL_RXpacketProcess ) (MCTL_Handle_t *pSupHandle, uint16_t *packetLength);

typedef enum {
  freeBuf = 0,
  writeLock = 1,
  pending = 2,
  readLock =3,
}buff_access_t;

typedef struct {
  uint8_t *buffer;
  uint16_t length;
  buff_access_t state;
} MCTL_Buff_t;

struct MCTL_Handle
{
  MCTL_GetBuf fGetBuffer; 
  MCTL_SendPacket fSendPacket;
  MCTL_RXpacketProcess fRXPacketProcess;
  MCTL_StartConnection fStartConnection;
  MCTL_Disconnection fDisconnection;
  MCTL_GetStatus fGetStatus;
  MCTL_FreeBuffer fFreeBuffer;

  uint16_t txSyncMaxPayload;
  uint16_t txAsyncMaxPayload;
} ;

#ifdef __cplusplus
}
#endif

#endif /*MC_TRANSPORT_LAYER*/
