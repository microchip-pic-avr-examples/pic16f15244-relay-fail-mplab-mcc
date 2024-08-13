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
#include "relay.h"

#include "mcc_generated_files/system/system.h"
#include "mcc_generated_files/diagnostics/diag_library/memory/non_volatile/diag_flash_crc16.h"
#include "mcc_generated_files/diagnostics/diag_library/cpu/diag_cpu_registers.h"
#include "mcc_generated_files/diagnostics/diag_library/wdt/diag_wdt_startup.h"

//Defines the number of timer counts (~1ms per count) before the relay toggles
#define RELAY_TOGGLE_TIME 5000

//How often should the PFM be scanned? (~1ms per count)
#define FUSA_MEM_SELF_TEST 120000

//Set to true to run a CRC scan of the PFM
volatile bool runMemTest = false;

//1ms Periodic Function
void PeriodicScan(void)
{
    static uint16_t toggleCounter = 0;
    static uint32_t fusaCounter = 0;
    
    //Periodic Self-Test of the Relay Systems
    Relay_SelfTest();
    
    //Clear any errors
    if (SW_GetValue() == 0)
    {
        Relay_ClearErrors();
    }
    
    //Time to switch the relay
    if (toggleCounter >= RELAY_TOGGLE_TIME)
    {
        toggleCounter = 0;
        Relay_SwitchState();
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

#define EEPROM_VERSION 0x01

//Address where the marker byte is located
#define EEPROM_VERSION_ADDR 0xFE0

int main(void)
{
    SYSTEM_Initialize();
    
    printf("PIC16F15244 Relay Failure Detector\r\n");
    
    //Check to see if the CRC has been programmed
    if (FLASH_Read(EEPROM_VERSION_ADDR) != EEPROM_VERSION)
    {
        //Need to write the CRC
        printf("Writing CRC for First Time Boot...\r\n");
                
        flash_address_t addr = FLASH_PageAddressGet(EEPROM_VERSION_ADDR);

        //Erase ROW
        NVM_UnlockKeySet(UNLOCK_KEY);
        if (FLASH_PageErase(addr) == NVM_ERROR)
        {
            printf("An NVM erase error has occurred\r\n");
        }
        NVM_UnlockKeyClear();
        
        flash_data_t data[PROGMEM_PAGE_SIZE];
        data[0] = EEPROM_VERSION;
        
        //Fill the rest of the row
        for (uint8_t i = 1; i < PROGMEM_PAGE_SIZE; i++)
        {
            data[i] = 0x3FFF;
        }
        
        NVM_UnlockKeySet(UNLOCK_KEY);
        if (FLASH_RowWrite(addr, &data[0]) == NVM_ERROR)
        {
            printf("An NVM row write error has occurred\r\n");
        }
        NVM_UnlockKeyClear();
        
        //Write the CRC
        if (DIAG_FLASH_CalculateStoreCRC(DIAG_FLASH_START_ADDR, 
                    DIAG_FLASH_LENGTH, DIAG_FLASH_CRC_STORE_ADDR) == DIAG_PASS)
        {
            printf("Successfully wrote CRC\r\n");
        }
        else
        {
            printf("An error has occurred while writing CRC\r\n");
        }        
        
        printf("CRC Write Complete\r\n");
    }
    else
    {
        printf("Memory ID OK\r\n");
    }
    
    //Check Functional Safety Tests
    CheckStartupTests();
    
    //TODO: Implement periodic FUSA 
    //TODO: Implement debouncing/one-shot for button

    printf("\r\n");
    
    //Switch the state of the relay
    Timer2_OverflowCallbackRegister(&PeriodicScan);
    
    //Handle the ADC Results
    ADC_SetInterruptHandler(&Relay_onADCReady);

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
                Relay_SetError(ERROR_SELF_TEST_FAIL);
            }
        }
        
        if (Relay_hasErrorOccurred())
        {
            Relay_clearErrorOccurred();
            
            error_state_t errState = Relay_GetErrorState();
            
            printf("System Error State: 0x%x\r\n", errState);
            
            
            //Print the error codes
            
            if (errState == ERROR_NONE)
            {
                printf("> All Error Cleared\r\n");
                continue;
            }
            
            if (errState & ERROR_RELAY_STUCK)
            {
                printf("> RELAY_STUCK\r\n");
            }
            
            if (errState & ERROR_TRANSISTOR_SHORT)
            {
                printf("> TRANSISTOR_SHORT\r\n");
            }
            
            if (errState & ERROR_SELF_TEST_FAIL)
            {
                printf("> SELF_TEST_FAIL\r\n");
            }
            
            if (errState & ERROR_ILLEGAL_STATE)
            {
                printf("> ILLEGAL STATE\r\n");
            }
            
            if (errState & ERROR_OUTPUT_BROWNOUT)
            {
                printf("> OUTPUT_BROWNOUT\r\n");
            }
        }
    }    
}