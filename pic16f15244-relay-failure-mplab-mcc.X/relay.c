#include "relay.h"

#include <stdint.h>
#include <stdbool.h>

#include "mcc_generated_files/system/system.h"

//State Variables
//2nd Copy is provided for protection against memory faults
volatile error_state_t errorState = ERROR_NONE;
volatile error_state_t errorState2 = ERROR_NONE;

volatile relay_state_t relayState = RELAY_OPEN;
volatile relay_state_t relayState2 = RELAY_OPEN;

//Transition timings
volatile uint8_t relayCount = 0;
volatile uint8_t relayCountMax = 0;

//Has a new error occurred (prevents extra prints)
volatile bool errorChanged = false;

//Sets the error flag
void Relay_SetError(error_state_t error)
{
    //Disable the relay drive
    RELAY_DRIVE_SetLow();
    
    //Verify the error parameter is valid
    if ((error != ERROR_RELAY_STUCK) && (error != ERROR_TRANSISTOR_SHORT) &&
            (error != ERROR_SELF_TEST_FAIL) && (error != ERROR_ILLEGAL_STATE))
    {
        error = ERROR_ILLEGAL_STATE;
    }
    
    error_state_t tempErr = errorState;
    
    //OR the state variables
    errorState |= error;
    errorState2 |= error;
    
    relayState = RELAY_ERROR;
    relayState2 = RELAY_ERROR;
    
    //Set the LEDs
    ERROR_LED_SetHigh();
    RELAY_STATE_LED_SetLow();
    
    if (tempErr != errorState)
    {
        errorChanged = true;
    }
}

//Set the relay state
void Relay_SetState(relay_state_t state)
{
    //Update State Variables
    relayState = state;
    relayState2 = state;
    
    switch (relayState)
    {
        case RELAY_OPEN:
        case RELAY_CLOSE_TRANSITION_OPEN:
        {
            //Relay drive is inactive
            RELAY_STATE_LED_SetLow();
            break;
        }
        case RELAY_CLOSED:
        case RELAY_OPEN_TRANSITION_CLOSED:
        {
            //Relay drive is active
            RELAY_STATE_LED_SetHigh();
            break;
        }
        case RELAY_ERROR:
        {
            //Stop driving the relay due to an error
            RELAY_STATE_LED_SetLow();
            break;
        }
        default:
        {
            //Unknown error
            Relay_SetError(ERROR_ILLEGAL_STATE);
        }
    }
}

//Clears the error flags
void Relay_ClearErrors(void)
{
    //Flag a change in error state
    if (errorState != ERROR_NONE)
    {
        errorChanged = true;
    }
    
    //Clear Error States
    errorState = 0x00;
    errorState2 = 0x00;
    
    //Update LED
    ERROR_LED_SetLow();
    
    //Update Relay State Machine
    Relay_SetState(RELAY_OPEN);
}

//Switch the state of the relay
void Relay_SwitchState(void)
{
    switch (relayState)
    {
        case RELAY_OPEN:
        {
            //Close the relay
            relayCount = 0;
            relayCountMax = RELAY_CLOSE_TIME_MAX + RELAY_MARGIN;
            
            RELAY_DRIVE_SetHigh();
            Relay_SetState(RELAY_OPEN_TRANSITION_CLOSED);
            break;
        }
        case RELAY_CLOSED:
        {
            //Open the relay
            relayCount = 0;
            relayCountMax = RELAY_OPEN_TIME_MAX + RELAY_MARGIN;
            
            RELAY_DRIVE_SetLow();
            Relay_SetState(RELAY_CLOSE_TRANSITION_OPEN);
            break;
        }
        case RELAY_ERROR:
        {
            //An Error Occurred
            break;
        }
        default:
        {
            //Switching error
            break;
        }
    }
}

//Periodic Self-test of the relay
bool Relay_SelfTest(void)
{
    //Check relay state variables
    if (relayState != relayState2)
    {
        //Unexpected mismatch
        Relay_SetError(ERROR_SELF_TEST_FAIL);
    }
    
    //Check error state variables
    if (errorState != errorState2)
    {
        //Unexpected mismatch
        Relay_SetError(ERROR_SELF_TEST_FAIL);
    }
    
    //Check drive transistor
    //Is the output as expected?
    if (RELAY_DRIVE_LAT != RELAY_DRIVE_PORT)
    {
        //Transistor failure
        Relay_SetError(ERROR_TRANSISTOR_SHORT);
    }
    
    //Check to see if we need to check for relay transition
    if ((relayState == RELAY_OPEN_TRANSITION_CLOSED) ||
            (relayState == RELAY_CLOSE_TRANSITION_OPEN))
    {
        //Relay should be switching
        
        //Has transition count expired?
        if (relayCount >= relayCountMax)
        {
            Relay_SetError(ERROR_RELAY_STUCK);
        }
        else
        {
            relayCount++;
        }
    }
    
    
    
    return false;
}

//Returns a set of errors that have occurred
error_state_t Relay_GetErrorState(void)
{
    return errorState;
}

//Returns the current state of the relay
relay_state_t Relay_GetRelayState(void)
{
    return relayState;
}

//Returns true if a new error has occurred
bool Relay_hasErrorOccurred(void)
{
    return errorChanged;
}

//Clears the error occurred flag
void Relay_clearErrorOccurred(void)
{
    errorChanged = false;
}

//Process data from the ADC
void Relay_onADCReady(void)
{
    adc_result_t result = ADC_GetConversionResult();
    
    //Update LED Status
    if (result >= ADC_THRESHOLD_HIGH)
    {
        //Relay is currently closed
        switch (relayState)
        {
            case RELAY_OPEN:
            {
                //Relay is open
                Relay_SetError(ERROR_RELAY_STUCK);
                break;
            }
            case RELAY_OPEN_TRANSITION_CLOSED:
            {
                if (relayCount >= RELAY_CLOSE_TIME_MAX)
                {
                    //Minimum switching time has passed
                    //Transitioning from Open -> Closed
                    Relay_SetState(RELAY_CLOSED);
                }
                break;
            }
            case RELAY_CLOSED:
            {
                //Relay is closed
                //Expected state
                break;
            }
            case RELAY_CLOSE_TRANSITION_OPEN:
            {
                //Transitioning from Closed -> Open
                //Keep waiting
                break;
            }
            case RELAY_ERROR:
            {
                //Error State
                break;
            }
            default:
            {
                //Something went wrong!
                Relay_SetError(ERROR_ILLEGAL_STATE);
            }
        }
    }
    else if (result <= ADC_THRESHOLD_LOW)
    {
        //Relay is currently open
        switch (relayState)
        {
            case RELAY_OPEN:
            {
                //Relay is open
                //Expected state
                break;
            }
            case RELAY_OPEN_TRANSITION_CLOSED:
            {
                //Transitioning from Open -> Closed
                //Keep waiting
                break;
            }
            case RELAY_CLOSED:
            {
                //Relay is closed
                Relay_SetError(ERROR_RELAY_STUCK);
                break;
            }
            case RELAY_CLOSE_TRANSITION_OPEN:
            {
                //Transitioning from Closed -> Open
                if (relayCount >= RELAY_OPEN_TIME_MAX)
                {
                    Relay_SetState(RELAY_OPEN);
                }
                
                break;
            }
            case RELAY_ERROR:
            {
                //Error State
                break;
            }
            default:
            {
                //Something went wrong!
                Relay_SetError(ERROR_ILLEGAL_STATE);
            }
        }
    }
    else
    {
        //Not open or closed... 
        switch (relayState)
        {
            case RELAY_OPEN:
            {
                //Relay is open
                Relay_SetError(ERROR_OUTPUT_BROWNOUT);
                break;
            }
            case RELAY_OPEN_TRANSITION_CLOSED:
            {
                //Transitioning from Open -> Closed
                break;
            }
            case RELAY_CLOSED:
            {
                //Relay is closed
                Relay_SetError(ERROR_OUTPUT_BROWNOUT);
                break;
            }
            case RELAY_CLOSE_TRANSITION_OPEN:
            {
                //Transitioning from Closed -> Open
                break;
            }
            case RELAY_ERROR:
            {
                //Error State
                break;
            }
            default:
            {
                //Something went wrong!
                Relay_SetError(ERROR_ILLEGAL_STATE);
            }
        }
    }

}