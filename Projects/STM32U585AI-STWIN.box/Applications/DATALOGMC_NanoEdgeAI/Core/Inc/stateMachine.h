#ifndef STATEMACHINE_H
#define STATEMACHINE_H

#include <stdio.h>
#include <stdlib.h>

/**
  * @brief Define a type for states.
  */
typedef int State;

/**
  * @brief Define a type for events.
  */
typedef int Event;

/**
  * @brief Define a type for state functions.
  *
  * @param event The event to handle.
  * @return The next state.
  */
typedef State(*StateFunction)(Event, const void *param);

/**
  * @brief Define a structure for the state machine.
  */
typedef struct
{
  StateFunction *stateFunctions; /**< Pointer to array of state functions */
  State currentState;            /**< Current state */
  int numStates;                 /**< Number of states */
} StateMachine;

/**
  * @brief Define a constant for staying in the same state.
  */
#define SAME_STATE -1

/**
  * @brief Define an enum for return values.
  */
typedef enum
{
  SM_SUCCESS,          /**< Operation successful */
  SM_INVALID_STATE,    /**< Invalid state */
  SM_INVALID_NEXT_STATE, /**< Invalid next state */
  SM_NULL_POINTER      /**< Null pointer error */
} StateMachineStatus;

/**
  * @brief Initialize the state machine.
  *
  * @param sm Pointer to the state machine instance.
  * @param functions Array of state functions.
  * @param numStates Number of states.
  * @return StateMachineStatus Status of the initialization.
  */
StateMachineStatus StateMachine_Init(StateMachine *sm, StateFunction *functions, int numStates);

/**
  * @brief Run the state machine.
  *
  * @param sm Pointer to the state machine instance.
  * @param event The event to handle.
  * @return StateMachineStatus Status of the operation.
  */
StateMachineStatus StateMachine_Run(StateMachine *sm, Event event, const void *param);

#endif // STATEMACHINE_H
