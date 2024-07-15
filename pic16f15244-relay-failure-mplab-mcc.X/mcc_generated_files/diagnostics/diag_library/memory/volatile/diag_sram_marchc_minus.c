 /**
 * © 2024 Microchip Technology Inc. and its subsidiaries.
 *
 * Subject to your compliance with these terms, you may use Microchip 
 * software and any derivatives exclusively with Microchip products. 
 * It is your responsibility to comply with third party license terms 
 * applicable to your use of third party software (including open 
 * source software) that may accompany Microchip software.
 *
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, 
 * WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, 
 * INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT, 
 * MERCHANTABILITY, AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 * IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, 
 * PUNITIVE, INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE 
 * OF ANY KIND WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, 
 * EVEN IF MICROCHIP HAS BEEN ADVISED OF THE POSSIBILITY OR THE 
 * DAMAGES ARE FORESEEABLE. TO THE FULLEST EXTENT ALLOWED BY LAW, 
 * MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN ANY WAY RELATED TO 
 * THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY, THAT YOU 
 * HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 *  @file diag_sram_marchc_minus.c
 *  @brief This file contains the implementation of the APIs to test SRAM using the March C- algorithm.
 *  @ingroup diag_sram_marchc_minus
 *  @note 
 * Microchip Technology Inc. has followed development methods required by 
 * Functional Safety Standards and performed extensive validation and static
 * testing to ensure that the code operates as intended. Any modification to the 
 * code can invalidate the results of Microchip's validation and testing.
 */

#include "diag_sram_marchc_minus.h"
#include "diag_sram_definitions.h"
#include <stdbool.h>

/* This function runs the March C test on the DIAG_SRAM test variables. */
static diag_result_t MarchC_Minus_Global(void);

/**
 @misradeviation{@advisory,15.5} The function should return as soon as a fault is
 detected. Avoiding the use of multiple return statements would require having 
 multiple additional checks which would add overhead, diminish code readability 
 and would require defaulting the test result to @ref DIAG_PASS. The latter is 
 deemed less safe than only returning @ref DIAG_PASS when all sections have been
 tested with no fault detected. Since there is no functional termination code in
 this function, it cannot accidentally be omitted and since the function does not
 have any persistent side effects, there is no issue in discerning which side 
 effects have been applied when the function is executed/exits and thus the 
 deviation is deemed safe.
 */
diag_result_t DIAG_SRAM_MarchCMinus(uint16_t startAddress, uint8_t length, uint16_t bufferAddress)
{
    uint8_t globalInt;

    //Saving the state of interrupts before test
    globalInt = INTCONbits.GIE;

    //Disable the interrupts
    INTCONbits.GIE = 0U;

    DIAG_SRAM.startAddress = startAddress;
    DIAG_SRAM.length = length;
    DIAG_SRAM.bufferAddress = bufferAddress;

    volatile uint8_t testAddr;
    volatile uint16_t testLength;

    // Return an error if the addresses are within the SFR RAM Space
    testAddr = DIAG_SRAM.startAddress;
    if (((testAddr <= SFR_REGION_LOC1)) || ((testAddr >= SFR_REGION_LOC2) && (testAddr <= SFR_REGION_LOC3)))
    {
        INTCONbits.GIE = globalInt;
        /* cppcheck-suppress misra-c2012-15.5 */
        return DIAG_INVALID_ARG;
    }

    testAddr = DIAG_SRAM.bufferAddress;
    if (((testAddr <= SFR_REGION_LOC1)) || ((testAddr >= SFR_REGION_LOC2) && (testAddr <= SFR_REGION_LOC3)))
    {
        INTCONbits.GIE = globalInt;
        /* cppcheck-suppress misra-c2012-15.5 */
        return DIAG_INVALID_ARG;
    }

    testAddr = (DIAG_SRAM.bufferAddress + DIAG_SRAM.length) - 1U;
    if (((testAddr <= SFR_REGION_LOC1)) || ((testAddr >= SFR_REGION_LOC2) && (testAddr <= SFR_REGION_LOC3)))
    {
        INTCONbits.GIE = globalInt;
        /* cppcheck-suppress misra-c2012-15.5 */
        return DIAG_INVALID_ARG;
    }

    // Return an error if the test length is 0 or exceeds the Bank Length
    testLength = DIAG_SRAM.length;
    if ((testLength == MIN_LENGTH) || (testLength > MAX_LENGTH))
    {
        INTCONbits.GIE = globalInt;
        /* cppcheck-suppress misra-c2012-15.5 */
        return DIAG_INVALID_ARG;
    }
    // Run March C- Test on the global RAM used by March C- test [STARTADDR_LOC - LENGTH_LOC]
    if (DIAG_SRAM.startAddress < (DIAG_MEM_END + 1U))
    {
        if (DIAG_SRAM.length < SRAM_GLOBAL_LENGTH)
        {
            INTCONbits.GIE = globalInt;
            /* cppcheck-suppress misra-c2012-15.5 */
            return DIAG_INVALID_ARG;
        }
        if (MarchC_Minus_Global() == DIAG_FAIL)
        {
            INTCONbits.GIE = globalInt;
            /* cppcheck-suppress misra-c2012-15.5 */
            return DIAG_FAIL;
        }
        DIAG_SRAM.length -= ((DIAG_MEM_END + 1U) - DIAG_SRAM.startAddress);
        DIAG_SRAM.startAddress = (DIAG_MEM_END + 1U);
    }

    /* Save the test memory in a known location designated by *DIAG_SRAM.bufferAddress
	   The variables currently stored in *DIAG_SRAM.bufferAddress will not be saved */
    if (DIAG_SRAM.bufferAddress != 0x00U)
    {
        DIAG_SRAM.tempAddress = DIAG_SRAM.startAddress;
        for (DIAG_SRAM.index = DIAG_SRAM.length; DIAG_SRAM.index != 0U; DIAG_SRAM.index--)
        {
            *(uint8_t *) DIAG_SRAM.bufferAddress = *(uint8_t *) DIAG_SRAM.tempAddress;
            DIAG_SRAM.tempAddress++;
            DIAG_SRAM.bufferAddress++;
        }
    }

    // Step-1: Wr0 - write zero to all memory for test
    DIAG_SRAM.tempAddress = DIAG_SRAM.startAddress;
    for (DIAG_SRAM.index = 0U; DIAG_SRAM.index < DIAG_SRAM.length; DIAG_SRAM.index++)
    {
        *(uint16_t *) DIAG_SRAM.startAddress = 0x00U;
        DIAG_SRAM.startAddress++;
    }

    DIAG_SRAM.errorFlag = DIAG_PASS;

    // Step-2: Rd0Wr1 - read zero, write one - ascending
    DIAG_SRAM.startAddress = DIAG_SRAM.tempAddress;
    for (DIAG_SRAM.index = 0U; DIAG_SRAM.index < DIAG_SRAM.length; DIAG_SRAM.index++)
    {
        DIAG_SRAM.var1 = 0x00U;
        for (DIAG_SRAM.testBit = 0U; DIAG_SRAM.testBit < 8U; DIAG_SRAM.testBit++)
        {
            FSR1 = DIAG_SRAM.startAddress;
            __asm("MOVF    INDF1, W");
            __asm("BANKSEL _DIAG_SRAM + 11");
            __asm("XORWF" ___mkstr(BANKMASK(_DIAG_SRAM + 11))", W");

            if (STATUSbits.Z == 0U)
            {
                DIAG_SRAM.errorFlag = DIAG_FAIL;
            }

            DIAG_SRAM.var1 = (uint8_t)(DIAG_SRAM.var1 << 1U) | 0x01U;
            INDF1 = INDF1 | DIAG_SRAM.var1;

            if (DIAG_SRAM.errorFlag == DIAG_FAIL)
            {
                INTCONbits.GIE = globalInt;
                /* cppcheck-suppress misra-c2012-15.5 */
                return DIAG_FAIL;
            }
        }
        DIAG_SRAM.startAddress++;
    }

    // Step-3: Rd1Wr0 - read one, write zero - ascending
    DIAG_SRAM.startAddress = DIAG_SRAM.tempAddress;
    for (DIAG_SRAM.index = 0U; DIAG_SRAM.index < DIAG_SRAM.length; DIAG_SRAM.index++)
    {
        DIAG_SRAM.var1 = 0xFFU;
        for (DIAG_SRAM.testBit = 0U; DIAG_SRAM.testBit < 8U; DIAG_SRAM.testBit++)
        {
            FSR1 = DIAG_SRAM.startAddress;
            __asm("MOVF    INDF1, W");
            __asm("BANKSEL _DIAG_SRAM + 11");
            __asm("XORWF" ___mkstr(BANKMASK(_DIAG_SRAM + 11))", W");

            if (STATUSbits.Z == 0U)
            {
                DIAG_SRAM.errorFlag = DIAG_FAIL;
            }

            INDF1 = (unsigned char)(DIAG_SRAM.var1 << 1U);
            DIAG_SRAM.var1 = (uint8_t)(DIAG_SRAM.var1 << 1U);

            if (DIAG_SRAM.errorFlag == DIAG_FAIL)
            {
                INTCONbits.GIE = globalInt;
                /* cppcheck-suppress misra-c2012-15.5 */
                return DIAG_FAIL;
            }
        }
        DIAG_SRAM.startAddress++;
    }

    // Step-4: Rd0Wr1 - read zero, write one - descending
    DIAG_SRAM.startAddress--;
    DIAG_SRAM.tempAddress = DIAG_SRAM.startAddress;
    for (DIAG_SRAM.index = DIAG_SRAM.length; DIAG_SRAM.index > 0U; DIAG_SRAM.index--)
    {
        DIAG_SRAM.var1 = 0x00U;
        for (DIAG_SRAM.testBit = 0U; DIAG_SRAM.testBit < 8U; DIAG_SRAM.testBit++)
        {
            FSR1 = DIAG_SRAM.startAddress;
            __asm("MOVF    INDF1, W");
            __asm("BANKSEL _DIAG_SRAM + 11");
            __asm("XORWF" ___mkstr(BANKMASK(_DIAG_SRAM + 11))", W");

            if (STATUSbits.Z == 0U)
            {
                DIAG_SRAM.errorFlag = DIAG_FAIL;
            }

            DIAG_SRAM.var1 = (DIAG_SRAM.var1 >> 1U) | 0x80U;
            INDF1 = INDF1 | DIAG_SRAM.var1;

            if (DIAG_SRAM.errorFlag == DIAG_FAIL)
            {
                INTCONbits.GIE = globalInt;
                /* cppcheck-suppress misra-c2012-15.5 */
                return DIAG_FAIL;
            }
        }
        DIAG_SRAM.startAddress--;
    }

    // Step-5: Rd1Wr0 - read one, write zero - descending
    DIAG_SRAM.startAddress = DIAG_SRAM.tempAddress;
    for (DIAG_SRAM.index = DIAG_SRAM.length; DIAG_SRAM.index > 0U; DIAG_SRAM.index--)
    {
        DIAG_SRAM.var1 = 0xFFU;
        for (DIAG_SRAM.testBit = 0U; DIAG_SRAM.testBit < 8U; DIAG_SRAM.testBit++)
        {
            FSR1 = DIAG_SRAM.startAddress;
            __asm("MOVF    INDF1, W");
            __asm("BANKSEL _DIAG_SRAM + 11");
            __asm("XORWF" ___mkstr(BANKMASK(_DIAG_SRAM + 11))", W");

            if (STATUSbits.Z == 0U)
            {
                DIAG_SRAM.errorFlag = DIAG_FAIL;
            }

            INDF1 = DIAG_SRAM.var1 >> 1U;
            DIAG_SRAM.var1 = DIAG_SRAM.var1 >> 1U;

            if (DIAG_SRAM.errorFlag == DIAG_FAIL)
            {
                INTCONbits.GIE = globalInt;
                /* cppcheck-suppress misra-c2012-15.5 */
                return DIAG_FAIL;
            }
        }
        DIAG_SRAM.startAddress--;
    }

    // Step-6: Rd0 - read zero - descending
    DIAG_SRAM.startAddress = DIAG_SRAM.tempAddress;
    for (DIAG_SRAM.index = DIAG_SRAM.length; DIAG_SRAM.index > 0U; DIAG_SRAM.index--)
    {
        DIAG_SRAM.var1 = 0x00U;
        for (DIAG_SRAM.testBit = 0U; DIAG_SRAM.testBit < 8U; DIAG_SRAM.testBit++)
        {
            FSR1 = DIAG_SRAM.startAddress;
            __asm("MOVF    INDF1, W");
            __asm("BANKSEL _DIAG_SRAM + 11");
            __asm("XORWF" ___mkstr(BANKMASK(_DIAG_SRAM + 11))", W");

            if (STATUSbits.Z == 0U)
            {
                DIAG_SRAM.errorFlag = DIAG_FAIL;
            }

            if (DIAG_SRAM.errorFlag == DIAG_FAIL)
            {
                INTCONbits.GIE = globalInt;
                /* cppcheck-suppress misra-c2012-15.5 */
                return DIAG_FAIL;
            }
        }
        DIAG_SRAM.startAddress--;
    }
    DIAG_SRAM.startAddress++;
    /* March C- Test Passed
       Restore the user data from the buffer. */
    if (DIAG_SRAM.bufferAddress != 0x00U)
    {
        DIAG_SRAM.tempAddress = (DIAG_SRAM.bufferAddress - DIAG_SRAM.length);
        for (DIAG_SRAM.index = DIAG_SRAM.length; DIAG_SRAM.index != 0U; DIAG_SRAM.index--)
        {
            *(uint8_t *) DIAG_SRAM.startAddress = *(uint8_t *) DIAG_SRAM.tempAddress;
            DIAG_SRAM.tempAddress++;
            DIAG_SRAM.startAddress++;
        }
    }

    INTCONbits.GIE = globalInt;
    return DIAG_PASS;
}

static diag_result_t MarchC_Minus_Global(void)
{
    volatile uint8_t* tempAddrPTR;
    volatile uint16_t storeValue_StartAddr;
    volatile uint16_t storeValue_BufferAddr;
    volatile uint16_t storeValue_Length;
    volatile uint8_t testAddr;
    volatile uint8_t testBit;
    volatile uint8_t bitMask;

    storeValue_StartAddr = DIAG_SRAM.startAddress;
    storeValue_BufferAddr = DIAG_SRAM.bufferAddress;
    storeValue_Length = DIAG_SRAM.length;

    // Step-1: Wr0 - ascending 
    tempAddrPTR = (uint8_t*) STARTADDR_LOC;
    for (testAddr = 0U; testAddr < RESERVED_SRAM; testAddr++)
    {
        *tempAddrPTR = 0U;
        tempAddrPTR++;
    }

    // Step-2: Rd0Wr1 - read zero, write one - ascending
    tempAddrPTR = (uint8_t*) STARTADDR_LOC;
    for (testAddr = 0U; testAddr < RESERVED_SRAM; testAddr++)
    {
        bitMask = 0x01U;
        for (testBit = 0U; testBit < 8U; testBit++)
        {
            if ((*tempAddrPTR & bitMask) != 0U)
            {
                /* cppcheck-suppress misra-c2012-15.5 */
                return DIAG_FAIL;
            }
            else
            {
                *tempAddrPTR = *tempAddrPTR | bitMask;
            }
            bitMask <<= 1U;
        }
        tempAddrPTR++;
    }

    // Step-3: Rd1Wr0 - read one, write zero - ascending
    tempAddrPTR = (uint8_t*) STARTADDR_LOC;
    for (testAddr = 0U; testAddr < RESERVED_SRAM; testAddr++)
    {
        bitMask = 0x01U;
        for (testBit = 0U; testBit < 8U; testBit++)
        {
            if ((*tempAddrPTR & bitMask) != 0U)
            {
                *tempAddrPTR = *tempAddrPTR & ~bitMask;
            }
            else
            {
                /* cppcheck-suppress misra-c2012-15.5 */
                return DIAG_FAIL;
            }
            bitMask <<= 1U;
        }
        tempAddrPTR++;
    }

    // Step-4: Rd0Wr1 - read zero, write one - descending
    tempAddrPTR = (uint8_t*) (DIAG_MEM_END);
    for (testAddr = 0U; testAddr < RESERVED_SRAM; testAddr++)
    {
        bitMask = 0x80U;
        for (testBit = 0U; testBit < 8U; testBit++)
        {
            if ((*tempAddrPTR & bitMask) != 0U)
            {
                /* cppcheck-suppress misra-c2012-15.5 */
                return DIAG_FAIL;
            }
            else
            {
                *tempAddrPTR = *tempAddrPTR | bitMask;
            }
            bitMask >>= 1U;
        }
        tempAddrPTR--;
    }

    // Step-5: Rd1Wr0 - read one, write zero - descending
    tempAddrPTR = (uint8_t*) (DIAG_MEM_END);
    for (testAddr = 0U; testAddr < RESERVED_SRAM; testAddr++)
    {
        bitMask = 0x80U;
        for (testBit = 0U; testBit < 8U; testBit++)
        {
            if ((*tempAddrPTR & bitMask) != 0U)
            {
                *tempAddrPTR = *tempAddrPTR & ~bitMask;
            }
            else
            {
                /* cppcheck-suppress misra-c2012-15.5 */
                return DIAG_FAIL;
            }
            bitMask >>= 1U;
        }
        tempAddrPTR--;
    }

    // Step-6: Rd0 - ascending
    tempAddrPTR = (uint8_t*) STARTADDR_LOC;
    for (testAddr = 0U; testAddr < RESERVED_SRAM; testAddr++)
    {
        if (*tempAddrPTR != 0U)
        {
            /* cppcheck-suppress misra-c2012-15.5 */
            return DIAG_FAIL;
        }
        tempAddrPTR++;
    }

    // Restore the test variable data from the registers
    DIAG_SRAM.startAddress = storeValue_StartAddr;
    DIAG_SRAM.bufferAddress = storeValue_BufferAddr;
    DIAG_SRAM.length = storeValue_Length;

    return DIAG_PASS;
}
