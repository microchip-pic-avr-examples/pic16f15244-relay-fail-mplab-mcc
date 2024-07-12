 /*
 * MAIN Generated Driver File
 * 
 * @file main.c
 * 
 * @defgroup main MAIN
 * 
 * @brief This is the generated driver implementation file for the MAIN driver.
 *
 * @version MAIN Driver Version 1.0.0
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

#define ADC_THRESHOLD 0x3F

volatile uint32_t counter = 0;

//Process the ADC Result
void OnADCResultReady(void)
{
    //Update LED Status
    if (ADC_GetConversionResult() >= ADC_THRESHOLD)
    {
        if (RELAY_DRIVE_LAT)
        {
            ERROR_LED_SetLow();
        }
        else
        {
            ERROR_LED_SetHigh();
        }
    }
    else
    {
        if (RELAY_DRIVE_LAT)
        {
            ERROR_LED_SetHigh();
        }
        else
        {
            ERROR_LED_SetLow();
        }

    }
}

//Monitor the Timer 1
void OnRTCOverflow(void)
{
    
}

//Switch the state of the relay
void SwitchState(void)
{
    //Toggle Relay
    RELAY_DRIVE_Toggle();
    
    //Set LED State
    if (RELAY_DRIVE_LAT == 1)
    {
        RELAY_STATE_LED_SetHigh();
    }
    else
    {
        RELAY_STATE_LED_SetLow();
    }
}


int main(void)
{
    SYSTEM_Initialize();

    //Switch the state of the relay
    Timer2_OverflowCallbackRegister(&SwitchState);
    
    //Handle Results
    ADC_SetInterruptHandler(&OnADCResultReady);
    
    // If using interrupts in PIC18 High/Low Priority Mode you need to enable the Global High and Low Interrupts 
    // If using interrupts in PIC Mid-Range Compatibility Mode you need to enable the Global and Peripheral Interrupts 
    // Use the following macros to: 

    // Enable the Global Interrupts 
    INTERRUPT_GlobalInterruptEnable(); 

    // Disable the Global Interrupts 
    //INTERRUPT_GlobalInterruptDisable(); 

    // Enable the Peripheral Interrupts 
    INTERRUPT_PeripheralInterruptEnable(); 

    // Disable the Peripheral Interrupts 
    //INTERRUPT_PeripheralInterruptDisable(); 

    ADC_StartConversion();

    while(1)
    {
    }    
}