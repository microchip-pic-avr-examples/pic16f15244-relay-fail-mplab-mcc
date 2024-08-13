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
#include "diagnostics/diag_library/memory/volatile/diag_sram_checkerboard.h"
#include "diagnostics/diag_library/memory/stack/diag_stack_marchc_minus.h"

//Defines the number of timer counts (~1ms per count) before the relay toggles
#define RELAY_TOGGLE_TIME 5000

//How often should the PFM be scanned? (~1ms per count)
#define FUSA_MEM_SELF_TEST 120000

//How often should the standard FUSA tests run? (~1ms per count)
#define FUSA_SELF_TEST 6000

#define GPRCOUNT _GPRCOUNT_

static const uint16_t startAddressTable[GPRCOUNT] = {
    BANK0_START_ADDRESS,
    BANK10_START_ADDRESS,
    BANK11_START_ADDRESS,
    BANK12_START_ADDRESS,
    BANK1_START_ADDRESS,
    BANK2_START_ADDRESS,
    BANK3_START_ADDRESS,
    BANK4_START_ADDRESS,
    BANK5_START_ADDRESS,
    BANK6_START_ADDRESS,
    BANK7_START_ADDRESS,
    BANK8_START_ADDRESS,
    BANK9_START_ADDRESS,
};
static const uint16_t endAddressTable[GPRCOUNT] = {
    BANK0_END_ADDRESS,
    BANK10_END_ADDRESS,
    BANK11_END_ADDRESS,
    BANK12_END_ADDRESS,
    BANK1_END_ADDRESS,
    BANK2_END_ADDRESS,
    BANK3_END_ADDRESS,
    BANK4_END_ADDRESS,
    BANK5_END_ADDRESS,
    BANK6_END_ADDRESS,
    BANK7_END_ADDRESS,
    BANK8_END_ADDRESS,
    BANK9_END_ADDRESS,
};

//Bank used for buffer
#define BANK3 3

//EEPROM Version ID
#define EEPROM_VERSION 0x01

//Address where the version ID byte is located
#define EEPROM_VERSION_ADDR 0x1FE0

//Set to true to run a CRC scan of the PFM
static volatile bool runMemTest = false;

//Memory Bank to scan
static volatile uint8_t memBank = 0;

//Set to true when a periodic fusa test has passed (for printing only)
static volatile bool periodicFusaPass = false;

//Tests a memory bank
diag_result_t Application_testMemoryBank(uint8_t bank)
{
    //Invalid Bank
    if (bank >= GPRCOUNT)
    {
        return DIAG_FAIL;
    }
    
    volatile uint16_t bufferAddress;
    volatile uint8_t bankLength;
    diag_result_t result = DIAG_FAIL;
    
    if (bank == BANK3)
    {
        //Last memory bank, so choose the previous for a buffer
        bufferAddress = BANK4_START_ADDRESS;
    }
    else
    {
        //Use the next memory bank for a buffer
        bufferAddress = BANK3_START_ADDRESS;
    }
    
    //Get the memory bank length
    bankLength = (uint8_t)(endAddressTable[bank] - startAddressTable[bank]);
    
    //Run the test
    result = DIAG_SRAM_Checkerboard(startAddressTable[bank], bankLength, bufferAddress);
    
    return result;
}

//Run a periodic self-test
diag_result_t Application_runPeriodicSelfTest(void)
{
    //CPU Test
    if (DIAG_CPU_Registers() != DIAG_PASS)
    {
        return DIAG_FAIL;
    }
    
    //Rotate through SRAM
    if (Application_testMemoryBank(memBank) != DIAG_PASS)
    {
        return DIAG_FAIL;
    }
    
    //Increment to the next memory bank
    if (memBank < GPRCOUNT)
    {
        memBank++;
    }
    else
    {
        memBank = 0;
    }
    
    periodicFusaPass = true;
    
    return DIAG_PASS;
}

//1ms Periodic Function
void Application_PeriodicScan(void)
{
    static uint16_t toggleCounter = 0;
    static uint32_t memScanCounter = 0;
    static uint32_t fusaCounter = 0;
        
    //Periodic Self-Test of the Relay Systems
    Relay_SelfTest();
    
    //Clear any errors
    if (SW_GetValue() == 0)
    {
        Relay_ClearErrors();
        toggleCounter = 0;
    }
    
    //Time to switch the relay?
    if (toggleCounter >= RELAY_TOGGLE_TIME)
    {
        toggleCounter = 0;
        Relay_SwitchState();
    }
    else
    {
        toggleCounter++;
    }
    
    //Time to run standard fusa self-test?
    if (fusaCounter >= FUSA_SELF_TEST)
    {
        fusaCounter = 0;
        
        //Periodic Fusa Tests
        if (Application_runPeriodicSelfTest() != DIAG_PASS)
        {
            Relay_SetError(ERROR_SELF_TEST_FAIL);
        }
        
    }
    else
    {
        fusaCounter++;
    }
    
    //Time to run memory self-test?
    if (memScanCounter >= FUSA_MEM_SELF_TEST)
    {
        memScanCounter = 0;
        
        //Queue Memory Scan
        runMemTest = true;
    }
    else
    {
        memScanCounter++;
    }
    
    //Clear WDT
    CLRWDT();
}

//Check the self-tests
void Application_CheckStartupTests(void)
{
    if (DIAG_CPU_Registers() != DIAG_PASS)
    {
        //CPU Register Self-Test Failed
        printf("CPU Self-Test Failed\r\n");
        Relay_SetError(ERROR_SELF_TEST_FAIL);
    }
    else
    {
        printf("CPU Self-Test OK\r\n");
    }
    
    if (DIAG_WDT_GetResult() != DIAG_PASS)
    {
        //WDT Self-Test Failed
        printf("WDT Self-Test Failed\r\n");
        Relay_SetError(ERROR_SELF_TEST_FAIL);
    }
    else
    {
        printf("WDT Self-Test OK\r\n");
    }
    
    if (DIAG_FLASH_ValidateCRC(DIAG_FLASH_START_ADDR, 
                    DIAG_FLASH_LENGTH, DIAG_FLASH_CRC_STORE_ADDR) != DIAG_PASS)
    {
        //Flash CRC Failed
        printf("Flash CRC Self-Test Failed\r\n");
        Relay_SetError(ERROR_SELF_TEST_FAIL);
    }
    else
    {
        printf("Flash CRC Self-Test OK\r\n");
    }
    
    //SRAM
    //Test all banks    
    for (uint8_t i = 0; i < GPRCOUNT; i++)
    {
        diag_result_t t = Application_testMemoryBank(i);
        if (t == DIAG_FAIL)
        {
            printf("SRAM Bank %u Self-Test Failed - Memory Bad\r\n", i);
            Relay_SetError(ERROR_SELF_TEST_FAIL);
        }
        else if (t == DIAG_INVALID_ARG)
        {
            printf("SRAM Bank %u Self-Test Failed - Invalid Argument Passed\r\n", i);
            Relay_SetError(ERROR_SELF_TEST_FAIL);
        }
        else if (t == DIAG_PASS)
        {
            printf("SRAM Bank %u Self-Test OK\r\n", i);
        }
        else
        {
            //Unknown
            printf("SRAM Bank %u Self-Test Failed - Unknown Error\r\n", i);
            Relay_SetError(ERROR_SELF_TEST_FAIL);
        }
    }
    
//    //Stack
//    if (DIAG_STACK_MarchCMinus() == DIAG_PASS)
//    {
//        printf("Stack Self-Test OK\r\n");
//    }
//    else
//    {
//        printf("Stack Self-Test Failed\r\n");
//    }
}

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
            Relay_SetError(ERROR_MEMORY_WRITE_FAIL);
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
            Relay_SetError(ERROR_MEMORY_WRITE_FAIL);
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
            Relay_SetError(ERROR_MEMORY_WRITE_FAIL);
        }        
        
        printf("CRC Write Complete\r\n");
    }
    else
    {
        printf("Memory ID OK\r\n");
    }
    
    //Check Functional Safety Tests
    Application_CheckStartupTests();
    
    //TODO: Implement periodic FUSA 

    printf("\r\n");
    
    //Switch the state of the relay
    Timer2_OverflowCallbackRegister(&Application_PeriodicScan);
    
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
            //Enable WDT
            WDTCONbits.SEN = 0;
            
            //Clear flag
            runMemTest = false;
            
            //Run CRC Scan
            diag_result_t result = DIAG_FLASH_ValidateCRC(DIAG_FLASH_START_ADDR, 
                    DIAG_FLASH_LENGTH, DIAG_FLASH_CRC_STORE_ADDR);

            if (result == DIAG_PASS)
            {
                printf("Periodic Flash CRC Self-Test OK\r\n");
            }
            else
            {
                //Set a self-test error
                printf("Periodic Flash CRC Self-Test Failed\r\n");
                Relay_SetError(ERROR_SELF_TEST_FAIL);
            }
            
//            //Run a Stack Test
//            if (DIAG_STACK_MarchCMinus() == DIAG_PASS)
//            {
//                printf("Periodic Stack Self-Test OK\r\n");
//            }
//            else
//            {
//                printf("Periodic Stack Self-Test Failed\r\n");
//            }
            
            //Enable WDT
            WDTCONbits.SEN = 1;
        }
        
        if (periodicFusaPass)
        {
            periodicFusaPass = false;
            printf("Periodic Self-Test OK\r\n");
        }
        
        if (Relay_hasErrorOccurred())
        {
            Relay_clearErrorOccurred();
            
            error_state_t errState = Relay_GetErrorState();
            
            printf("System Error State: 0x%x\r\n", errState);
            
            //Print the error codes            
            if (errState == ERROR_NONE)
            {
                printf("> All Errors Cleared\r\n");
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
            
            if (errState & ERROR_MEMORY_WRITE_FAIL)
            {
                printf("> MEMORY_WRITE_FAIL\r\n");
            }
        }
    }    
}