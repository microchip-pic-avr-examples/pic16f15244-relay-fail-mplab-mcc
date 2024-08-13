#ifndef RELAY_H
#define	RELAY_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>
    
//Defines the maximum length of time (~1ms per count) for the relay to close
#define RELAY_CLOSE_TIME_MAX 4

//Defines the maximum length of time (~1ms per count) for the relay to open
#define RELAY_OPEN_TIME_MAX 4

//Defines the timing margins (~1ms per count) for the relay to stabilize at the expected value
#define RELAY_MARGIN 2
    
//Defines the thresholds for the relay to be closed/open
#define ADC_THRESHOLD_HIGH 0x3F
#define ADC_THRESHOLD_LOW 0x10
    
//Error conditions for the program
typedef enum {
    ERROR_NONE = 0, ERROR_RELAY_STUCK = 1, ERROR_TRANSISTOR_SHORT = 2, 
        ERROR_SELF_TEST_FAIL = 4, ERROR_ILLEGAL_STATE = 8, ERROR_OUTPUT_BROWNOUT = 16
} error_state_t;

//Relay FSM States
typedef enum {
    RELAY_OPEN = 0, RELAY_OPEN_TRANSITION_CLOSED, 
    RELAY_CLOSED, RELAY_CLOSE_TRANSITION_OPEN, RELAY_ERROR
} relay_state_t;

//Sets the error flag
void Relay_SetError(error_state_t error);

//Set the relay state
void Relay_SetState(relay_state_t state);

//Clears the error flags
void Relay_ClearErrors(void);

//Switch the state of the relay
void Relay_SwitchState(void);

//Periodic Self-test of the relay
bool Relay_SelfTest(void);

//Returns a set of errors that have occurred
error_state_t Relay_GetErrorState(void);

//Returns the current state of the relay
relay_state_t Relay_GetRelayState(void);

//Returns true if a new error has occurred
bool Relay_hasErrorOccurred(void);

//Clears the error occurred flag
void Relay_clearErrorOccurred(void);

//Process data from the ADC
void Relay_onADCReady(void);

#ifdef	__cplusplus
}
#endif

#endif	/* RELAY_H */

