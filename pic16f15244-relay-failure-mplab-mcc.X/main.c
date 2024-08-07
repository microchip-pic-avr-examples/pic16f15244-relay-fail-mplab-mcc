 /*
 * MAIN Generated Driver File
 * 
 * @file main.c
 * 
 * @defgroup main MAIN
 * 
 * @brief This is the generated driver implementation file for the MAIN driver.
 *
 * @version MAIN Driver Version 1.0.2
 *
 * @version Package Version: 3.1.2
*/

/*
© [2024] Microchip Technology Inc. and its subsidiaries.

    Subject to your compliance with these terms, you may use Microchip 
    software and any derivatives exclusively with Microchip products. 
    You are responsible for complying with 3rd party license terms  
    applicable to your use of 3rd party software (including open source  
    software) that may accompany Microchip software. SOFTWARE IS ?AS IS.? 
    NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS 
    SOFTWARE, INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT,  
    MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT 
    WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY 
    KIND WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF 
    MICROCHIP HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE 
    FORESEEABLE. TO THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP?S 
    TOTAL LIABILITY ON ALL CLAIMS RELATED TO THE SOFTWARE WILL NOT 
    EXCEED AMOUNT OF FEES, IF ANY, YOU PAID DIRECTLY TO MICROCHIP FOR 
    THIS SOFTWARE.
*/
#include "mcc_generated_files/system/system.h"
#include "mcc_generated_files/diagnostics/diag_library/memory/non_volatile/diag_flash_crc16.h"
#include "mcc_generated_files/diagnostics/diag_library/cpu/diag_cpu_registers.h"
#include "mcc_generated_files/diagnostics/diag_library/wdt/diag_wdt_startup.h"

//Defines the thresholds for the relay to be closed/open
#define ADC_THRESHOLD_HIGH 0x3F
#define ADC_THRESHOLD_LOW 0x10

//Defines the maximum length of time (~1ms per count) for the relay to close
#define RELAY_CLOSE_TIME_MAX 4

//Defines the maximum length of time (~1ms per count) for the relay to open
#define RELAY_OPEN_TIME_MAX 4

//Defines the margin of error for the relay switching
#define RELAY_MARGIN 2

//Defines the number of timer counts (~1ms per count) before the relay toggles
#define RELAY_TOGGLE_TIME 5000

//How often should the PFM be scanned? (~1ms per count)
#define FUSA_MEM_SELF_TEST 120000

//Error conditions for the program
typedef enum {
    ERROR_NONE = 0, ERROR_RELAY_STUCK = 1, ERROR_TRANSISTOR_SHORT = 2, 
        ERROR_SELF_TEST_FAIL = 4, ERROR_ILLEGAL_STATE = 8, ERROR_OUTPUT_BROWNOUT = 16
} error_state_t;

//Relay FSM States
typedef enum {
    RELAY_OPEN = 0, RELAY_OPEN_TRANSITION_CLOSED, 
    RELAY_CLOSED, RELAY_CLOSE_TRANSITION_OPEN
} relay_state_t;

//State Variables
//2nd Copy is provided for protection against memory faults
volatile error_state_t errorState = ERROR_NONE;
volatile error_state_t errorState2 = ERROR_NONE;

volatile relay_state_t relayState = RELAY_OPEN;
volatile relay_state_t relayState2 = RELAY_OPEN;

//Set to true to run a CRC scan of the PFM
volatile bool runMemTest = false;
volatile bool errorChanged = false;

//Transition timings
volatile uint8_t relayCount = 0;
volatile uint8_t relayCountMax = 0;

//Sets the error flag
void SetError(error_state_t error)
{
    //Verify the error parameter is valid
    if ((error != ERROR_RELAY_STUCK) && (error != ERROR_TRANSISTOR_SHORT) &&
            (error != ERROR_SELF_TEST_FAIL) && (error != ERROR_ILLEGAL_STATE))
    {
        error = ERROR_ILLEGAL_STATE;
    }
    
    //OR the state variables
    errorState |= error;
    errorState2 |= error;
    
    //Set the LED
    ERROR_LED_SetHigh();
    
    errorChanged = true;
}

//Set the relay state
void SetRelayState(relay_state_t state)
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
        default:
        {
            //Unknown error
            SetError(ERROR_ILLEGAL_STATE);
        }
    }
}

//Process the ADC Result
void OnADCResultReady(void)
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
                SetError(ERROR_RELAY_STUCK);
                break;
            }
            case RELAY_OPEN_TRANSITION_CLOSED:
            {
                if (relayCount >= RELAY_CLOSE_TIME_MAX)
                {
                    //Minimum switching time has passed
                    //Transitioning from Open -> Closed
                    SetRelayState(RELAY_CLOSED);
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
            default:
            {
                //Something went wrong!
                SetError(ERROR_ILLEGAL_STATE);
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
                SetError(ERROR_RELAY_STUCK);
                break;
            }
            case RELAY_CLOSE_TRANSITION_OPEN:
            {
                //Transitioning from Closed -> Open
                if (relayCount >= RELAY_OPEN_TIME_MAX)
                {
                    SetRelayState(RELAY_OPEN);
                }
                
                break;
            }
            default:
            {
                //Something went wrong!
                SetError(ERROR_ILLEGAL_STATE);
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
                SetError(ERROR_OUTPUT_BROWNOUT);
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
                SetError(ERROR_OUTPUT_BROWNOUT);
                break;
            }
            case RELAY_CLOSE_TRANSITION_OPEN:
            {
                //Transitioning from Closed -> Open
                break;
            }
            default:
            {
                //Something went wrong!
                SetError(ERROR_ILLEGAL_STATE);
            }
        }
    }
}

//Switch the state of the relay
void SwitchState(void)
{
    switch (relayState)
    {
        case RELAY_OPEN:
        {
            //Close the relay
            relayCount = 0;
            relayCountMax = RELAY_CLOSE_TIME_MAX + RELAY_MARGIN;
            
            RELAY_DRIVE_SetHigh();
            SetRelayState(RELAY_OPEN_TRANSITION_CLOSED);
            break;
        }
        case RELAY_CLOSED:
        {
            //Open the relay
            relayCount = 0;
            relayCountMax = RELAY_OPEN_TIME_MAX + RELAY_MARGIN;
            
            RELAY_DRIVE_SetLow();
            SetRelayState(RELAY_CLOSE_TRANSITION_OPEN);
            break;
        }
    }
}

//1ms Periodic Function
void PeriodicScan(void)
{
    static uint16_t toggleCounter = 0;
    static uint32_t fusaCounter = 0;
    
    //Check relay state variables
    if (relayState != relayState2)
    {
        //Unexpected mismatch
        SetError(ERROR_SELF_TEST_FAIL);
    }
    
    //Check error state variables
    if (errorState != errorState2)
    {
        //Unexpected mismatch
        SetError(ERROR_SELF_TEST_FAIL);
    }
    
    //Check drive transistor
    //Is the output as expected?
    if (RELAY_DRIVE_LAT != RELAY_DRIVE_PORT)
    {
        //Transistor failure
        SetError(ERROR_TRANSISTOR_SHORT);
    }
    
    //Check to see if we need to check for relay transition
    if ((relayState == RELAY_OPEN_TRANSITION_CLOSED) ||
            (relayState == RELAY_CLOSE_TRANSITION_OPEN))
    {
        //Relay should be switching
        
        //Has transition count expired?
        if (relayCount >= relayCountMax)
        {
            SetError(ERROR_RELAY_STUCK);
        }
        else
        {
            relayCount++;
        }
    }
    
    //Time to switch the relay
    if (toggleCounter >= RELAY_TOGGLE_TIME)
    {
        toggleCounter = 0;
        SwitchState();
    }
    else
    {
        toggleCounter++;
    }
    
    //Time to run fusa self-test?
    if (fusaCounter >= FUSA_MEM_SELF_TEST)
    {
        fusaCounter = 0;
        
        //Queue Memory Scan
        runMemTest = true;
    }
    else
    {
        fusaCounter++;
    }
    
    //Clear WDT
    CLRWDT();
}

//Check the self-tests
void CheckStartupTests(void)
{
    if (DIAG_CPU_Registers() != DIAG_PASS)
    {
        //CPU Register Self-Test Failed
        printf("CPU Self-Test Failed\r\n");
    }
    
    if (DIAG_WDT_GetResult() != DIAG_PASS)
    {
        //WDT Self-Test Failed
        printf("WDT Self-Test Failed\r\n");
    }
    
    if (DIAG_FLASH_ValidateCRC(DIAG_FLASH_START_ADDR, 
                    DIAG_FLASH_LENGTH, DIAG_FLASH_CRC_STORE_ADDR) != DIAG_PASS)
    {
        //Flash CRC Failed
        printf("Flash CRC Self-Test Failed\r\n");
    }
    
    //SRAM
}

int main(void)
{
    SYSTEM_Initialize();
    
    printf("PIC16F15244 Relay Failure Detector\r\n");
    printf("Starting up...\r\n");
    
    if (DIAG_FLASH_CalculateStoreCRC(DIAG_FLASH_START_ADDR, 
                    DIAG_FLASH_LENGTH, DIAG_FLASH_CRC_STORE_ADDR) == DIAG_PASS)
    {
        printf("Successfully wrote CRC\r\n");
    }
    
    //Check Functional Safety Tests
    CheckStartupTests();

    printf("\r\n");
    
    //Switch the state of the relay
    Timer2_OverflowCallbackRegister(&PeriodicScan);
    
    //Handle the ADC Results
    ADC_SetInterruptHandler(&OnADCResultReady);

    // Enable the Global Interrupts 
    INTERRUPT_GlobalInterruptEnable(); 

    // Enable the Peripheral Interrupts 
    INTERRUPT_PeripheralInterruptEnable(); 

    //Start ADC Monitoring
    ADC_StartConversion();
    
    //Enable WDT
    WDTCONbits.SEN = 1;
    
    while(1)
    {
        if (runMemTest)
        {
            //Clear flag
            runMemTest = false;
            //Run CRC Scan
            diag_result_t result = DIAG_FLASH_ValidateCRC(DIAG_FLASH_START_ADDR, 
                    DIAG_FLASH_LENGTH, DIAG_FLASH_CRC_STORE_ADDR);

            if (result == DIAG_FAIL)
            {
                //Set a self-test error
                SetError(ERROR_SELF_TEST_FAIL);
            }
        }
        
        if (errorChanged)
        {
            printf("System Error State: 0x%x\r\n", errorState);
            errorChanged = false;
            
            //Print the error codes
            
            if (errorState & ERROR_RELAY_STUCK)
            {
                printf("> RELAY_STUCK\r\n");
            }
            
            if (errorState & ERROR_TRANSISTOR_SHORT)
            {
                printf("> TRANSISTOR_SHORT\r\n");
            }
            
            if (errorState & ERROR_SELF_TEST_FAIL)
            {
                printf("> SELF_TEST_FAIL\r\n");
            }
            
            if (errorState & ERROR_ILLEGAL_STATE)
            {
                printf("> ILLEGAL STATE\r\n");
            }
            
            if (errorState & ERROR_OUTPUT_BROWNOUT)
            {
                printf("> OUTPUT_BROWNOUT\r\n");
            }
        }
    }    
}