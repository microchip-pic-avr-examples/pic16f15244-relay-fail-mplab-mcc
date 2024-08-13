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

#include <stdint.h>
#include <stdio.h>
#include "../examples/diag_common_example.h"
#include "../config/diag_config.h"
#include "../../diag_library/memory/volatile/diag_sram_checkerboard.h"

#if (EXAMPLE_SRAM_FULL == 1)
uint8_t buffer1[DIAG_SRAM_MARCH_LENGTH] __at(BANK3_START_ADDRESS);
uint8_t buffer2[DIAG_SRAM_MARCH_LENGTH] __at(BANK4_START_ADDRESS);

#define BANK3 3
#define GPRCOUNT _GPRCOUNT_

static const uint16_t startAddressTable[GPRCOUNT] = {
    BANK0_START_ADDRESS,
    BANK1_START_ADDRESS,
    BANK2_START_ADDRESS,
    BANK3_START_ADDRESS,
    BANK4_START_ADDRESS,
    BANK5_START_ADDRESS,
    BANK6_START_ADDRESS,
};
static const uint16_t endAddressTable[GPRCOUNT] = {
    BANK0_END_ADDRESS,
    BANK1_END_ADDRESS,
    BANK2_END_ADDRESS,
    BANK3_END_ADDRESS,
    BANK4_END_ADDRESS,
    BANK5_END_ADDRESS,
    BANK6_END_ADDRESS,
};
#endif

#if (EXAMPLE_SRAM_BANK == 1)

void DIAG_SRAM_CheckerboardExample(void)
{
    diag_result_t result;

    result = DIAG_SRAM_Checkerboard(SRAM_CHECKERBOARD_START_ADDRESS, SRAM_CHECKERBOARD_LENGTH, SRAM_CHECKERBOARD_BUFFER_ADDRESS);

    switch (result)
    {
    case DIAG_PASS:
        printf("\r\nPassed : SRAM Checkerboard Test - Bank starting at 0x%x\r\n", SRAM_CHECKERBOARD_START_ADDRESS);
        break;
    case DIAG_FAIL:
        printf("\r\nFailed : SRAM Checkerboard Test - Bank starting at 0x%x\r\n", SRAM_CHECKERBOARD_START_ADDRESS);
        break;
    case DIAG_INVALID_ARG:
        printf("\r\nAborted : SRAM Checkerboard Test - Invalid arguments\r\n");
        break;
    default:
        printf("\r\nUndefined : SRAM Checkerboard Test\r\n");
        break;
    }
}

#elif (EXAMPLE_SRAM_FULL == 1)

void DIAG_SRAM_CheckerboardExample(void)
{
    diag_result_t result;
    volatile uint16_t bufferAddress;
    volatile uint8_t bankLength;

    for (uint8_t testBank = 0; testBank < GPRCOUNT; testBank++)
    {
        if (testBank < (GPRCOUNT - 1))
        {
            bufferAddress = startAddressTable[testBank + 1];
        }
        else
        {
            bufferAddress = startAddressTable[testBank - 1];
        }

        bankLength = (uint8_t)(endAddressTable[testBank] - startAddressTable[testBank]);

        result = DIAG_SRAM_Checkerboard(startAddressTable[testBank], bankLength, bufferAddress);

        switch (result)
        {
        case DIAG_PASS:
            printf("\r\nPassed : SRAM Checkerboard Test - Bank starting at 0x%x\r\n", startAddressTable[testBank]);
            break;
        case DIAG_FAIL:
            printf("\r\nFailed : SRAM Checkerboard Test - Bank starting at 0x%x\r\n", startAddressTable[testBank]);
            break;
        case DIAG_INVALID_ARG:
            printf("\r\nAborted : SRAM Checkerboard Test - Bank starting at 0x%x\r\n", startAddressTable[testBank]);
            break;
        default:
            printf("\r\nUndefined : SRAM Checkerboard Test - Bank starting at 0x%x\r\n", startAddressTable[testBank]);
            break;
        }
    }
}
#endif

