#include "stateMachine.h"

/**
  * @brief Initialize the state machine.
  *
  * @param sm Pointer to the state machine instance.
  * @param functions Array of state functions.
  * @param numStates Number of states.
  * @return StateMachineStatus Status of the initialization.
  */
StateMachineStatus StateMachine_Init(StateMachine *sm, StateFunction *functions, int numStates)
{
  if (sm == NULL || functions == NULL)
  {
    return SM_NULL_POINTER;
  }
  sm->stateFunctions = functions;
  sm->currentState = 0; // Start with the first state
  sm->numStates = numStates;
  return SM_SUCCESS;
}

/**
  * @brief Run the state machine.
  *
  * @param sm Pointer to the state machine instance.
  * @param event The event to handle.
  * @return StateMachineStatus Status of the operation.
  */
StateMachineStatus StateMachine_Run(StateMachine *sm, Event event, const void *param)
{
  if (sm == NULL)
  {
    return SM_NULL_POINTER;
  }
  if (sm->currentState >= sm->numStates)
  {
    return SM_INVALID_STATE;
  }
  // Call the function for the current state and get the next state
  State nextState = sm->stateFunctions[sm->currentState](event, param);
  if (nextState == SAME_STATE)
  {
    // Stay in the same state
    nextState = sm->currentState;
  }
  if (nextState < sm->numStates)
  {
    sm->currentState = nextState;
    return SM_SUCCESS;
  }
  else
  {
    return SM_INVALID_NEXT_STATE;
  }
}
