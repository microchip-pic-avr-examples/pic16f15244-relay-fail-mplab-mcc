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
 *  @file  diag_stack_marchc_minus.c
 *  @brief This file contains the API implementation to test Stack using the March C- algorithm.
 *  @ingroup diag_stack_marchc_minus
 * @note
 * Microchip Technology Inc. has followed development methods required by
 * Functional Safety Standards and performed extensive validation and static
 * testing to ensure that the code operates as intended. Any modification to the
 * code can invalidate the results of Microchip's validation and testing.
 */

#include "diag_stack_marchc_minus.h"
#include "diag_stack_march_types.h"
#include <xc.h>

diag_result_t DIAG_STACK_MarchCMinus(void)
{
    uint8_t stackTestResult; //local variable to test the return value of the diagnostic test
    diag_result_t status;

    /*Uncomment the following code when using Optimization level 's' -- valid only for XC8 v2.20 and later */
    __asm("ASMOPT PUSH");
    __asm("ASMOPT OFF");

    uint8_t globalInt;

    //Saving the state of interrupts before test
    globalInt = INTCONbits.GIE;

    //Disable the interrupts
    INTCONbits.GIE = 0;

    // Save Stack here
    *DIAG_MarchBufferStackAddress = STKPTR;
    DIAG_MarchBufferStackAddress++;

    for (WREG_SHAD = 0; WREG_SHAD <= (STACK_LEVEL - 1); WREG_SHAD++)
    {
        STKPTR = WREG_SHAD;
        *DIAG_MarchBufferStackAddress = TOSL;
        DIAG_MarchBufferStackAddress++;
        *DIAG_MarchBufferStackAddress = TOSH;
        DIAG_MarchBufferStackAddress++;
    }

    // Stack test starts here
    // Write zero
    __asm("BANKSEL   STKPTR");
    __asm("MOVLW   0x1F");
    __asm("MOVWF" ___mkstr(BANKMASK(STKPTR)));
    __asm("Wr0:");
    __asm("INCF" ___mkstr(BANKMASK(STKPTR)));
    __asm("CLRF" ___mkstr(BANKMASK(TOSL)));
    __asm("CLRF" ___mkstr(BANKMASK(TOSH)));
    __asm("MOVF" ___mkstr(BANKMASK(STKPTR))", W");
    __asm("XORLW   0x0F");
    __asm("BTFSS   STATUS, 0x02");
    __asm("GOTO" ___mkstr(PAGEMASK(Wr0)));

    // Read zero, Write one - ascending //
    __asm("MOVLW   0x1F");
    __asm("MOVWF" ___mkstr(BANKMASK(STKPTR)));
    __asm("Rd0Wr1ASC:");
    __asm("INCF" ___mkstr(BANKMASK(STKPTR)));
    __asm("BTFSC" ___mkstr(BANKMASK(TOSL))",0");
    __asm("GOTO" ___mkstr(PAGEMASK(STACKFAIL)));
    __asm("BSF" ___mkstr(BANKMASK(TOSL))",0");
    __asm("BTFSC" ___mkstr(BANKMASK(TOSL))",1");
    __asm("GOTO" ___mkstr(PAGEMASK(STACKFAIL)));
    __asm("BSF" ___mkstr(BANKMASK(TOSL))",1");
    __asm("BTFSC" ___mkstr(BANKMASK(TOSL))",2");
    __asm("GOTO" ___mkstr(PAGEMASK(STACKFAIL)));
    __asm("BSF" ___mkstr(BANKMASK(TOSL))",2");
    __asm("BTFSC" ___mkstr(BANKMASK(TOSL))",3");
    __asm("GOTO" ___mkstr(PAGEMASK(STACKFAIL)));
    __asm("BSF" ___mkstr(BANKMASK(TOSL))",3");
    __asm("BTFSC" ___mkstr(BANKMASK(TOSL))",4");
    __asm("GOTO" ___mkstr(PAGEMASK(STACKFAIL)));
    __asm("BSF" ___mkstr(BANKMASK(TOSL))",4");
    __asm("BTFSC" ___mkstr(BANKMASK(TOSL))",5");
    __asm("GOTO" ___mkstr(PAGEMASK(STACKFAIL)));
    __asm("BSF" ___mkstr(BANKMASK(TOSL))",5");
    __asm("BTFSC" ___mkstr(BANKMASK(TOSL))",6");
    __asm("GOTO" ___mkstr(PAGEMASK(STACKFAIL)));
    __asm("BSF" ___mkstr(BANKMASK(TOSL))",6");
    __asm("BTFSC" ___mkstr(BANKMASK(TOSL))",7");
    __asm("GOTO" ___mkstr(PAGEMASK(STACKFAIL)));
    __asm("BSF" ___mkstr(BANKMASK(TOSL))",7");
    __asm("BTFSC" ___mkstr(BANKMASK(TOSH))",0");
    __asm("GOTO" ___mkstr(PAGEMASK(STACKFAIL)));
    __asm("BSF" ___mkstr(BANKMASK(TOSH))",0");
    __asm("BTFSC" ___mkstr(BANKMASK(TOSH))",1");
    __asm("GOTO" ___mkstr(PAGEMASK(STACKFAIL)));
    __asm("BSF" ___mkstr(BANKMASK(TOSH))",1");
    __asm("BTFSC" ___mkstr(BANKMASK(TOSH))",2");
    __asm("GOTO" ___mkstr(PAGEMASK(STACKFAIL)));
    __asm("BSF" ___mkstr(BANKMASK(TOSH))",2");
    __asm("BTFSC" ___mkstr(BANKMASK(TOSH))",3");
    __asm("GOTO" ___mkstr(PAGEMASK(STACKFAIL)));
    __asm("BSF" ___mkstr(BANKMASK(TOSH))",3");
    __asm("BTFSC" ___mkstr(BANKMASK(TOSH))",4");
    __asm("GOTO" ___mkstr(PAGEMASK(STACKFAIL)));
    __asm("BSF" ___mkstr(BANKMASK(TOSH))",4");
    __asm("BTFSC" ___mkstr(BANKMASK(TOSH))",5");
    __asm("GOTO" ___mkstr(PAGEMASK(STACKFAIL)));
    __asm("BSF" ___mkstr(BANKMASK(TOSH))",5");
    __asm("BTFSC" ___mkstr(BANKMASK(TOSH))",6");
    __asm("GOTO" ___mkstr(PAGEMASK(STACKFAIL)));
    __asm("BSF" ___mkstr(BANKMASK(TOSH))",6");
    __asm("MOVF" ___mkstr(BANKMASK(STKPTR))", W");
    __asm("XORLW   0x0F");
    __asm("BTFSS   STATUS, 0x02");
    __asm("GOTO" ___mkstr(PAGEMASK(Rd0Wr1ASC)));

    // Read one, Write zero - ascending
    __asm("MOVLW   0x1F");
    __asm("MOVWF" ___mkstr(BANKMASK(STKPTR)));
    __asm("Rd1Wr0ASC:");
    __asm("INCF" ___mkstr(BANKMASK(STKPTR)));
    __asm("BTFSS" ___mkstr(BANKMASK(TOSL))",0");
    __asm("GOTO" ___mkstr(PAGEMASK(STACKFAIL)));
    __asm("BCF" ___mkstr(BANKMASK(TOSL))",0");
    __asm("BTFSS" ___mkstr(BANKMASK(TOSL))",1");
    __asm("GOTO" ___mkstr(PAGEMASK(STACKFAIL)));
    __asm("BCF" ___mkstr(BANKMASK(TOSL))",1");
    __asm("BTFSS" ___mkstr(BANKMASK(TOSL))",2");
    __asm("GOTO" ___mkstr(PAGEMASK(STACKFAIL)));
    __asm("BCF" ___mkstr(BANKMASK(TOSL))",2");
    __asm("BTFSS" ___mkstr(BANKMASK(TOSL))",3");
    __asm("GOTO" ___mkstr(PAGEMASK(STACKFAIL)));
    __asm("BCF" ___mkstr(BANKMASK(TOSL))",3");
    __asm("BTFSS" ___mkstr(BANKMASK(TOSL))",4");
    __asm("GOTO" ___mkstr(PAGEMASK(STACKFAIL)));
    __asm("BCF" ___mkstr(BANKMASK(TOSL))",4");
    __asm("BTFSS" ___mkstr(BANKMASK(TOSL))",5");
    __asm("GOTO" ___mkstr(PAGEMASK(STACKFAIL)));
    __asm("BCF" ___mkstr(BANKMASK(TOSL))",5");
    __asm("BTFSS" ___mkstr(BANKMASK(TOSL))",6");
    __asm("GOTO" ___mkstr(PAGEMASK(STACKFAIL)));
    __asm("BCF" ___mkstr(BANKMASK(TOSL))",6");
    __asm("BTFSS" ___mkstr(BANKMASK(TOSL))",7");
    __asm("GOTO" ___mkstr(PAGEMASK(STACKFAIL)));
    __asm("BCF" ___mkstr(BANKMASK(TOSL))",7");
    __asm("BTFSS" ___mkstr(BANKMASK(TOSH))",0");
    __asm("GOTO" ___mkstr(PAGEMASK(STACKFAIL)));
    __asm("BCF" ___mkstr(BANKMASK(TOSH))",0");
    __asm("BTFSS" ___mkstr(BANKMASK(TOSH))",1");
    __asm("GOTO" ___mkstr(PAGEMASK(STACKFAIL)));
    __asm("BCF" ___mkstr(BANKMASK(TOSH))",1");
    __asm("BTFSS" ___mkstr(BANKMASK(TOSH))",2");
    __asm("GOTO" ___mkstr(PAGEMASK(STACKFAIL)));
    __asm("BCF" ___mkstr(BANKMASK(TOSH))",2");
    __asm("BTFSS" ___mkstr(BANKMASK(TOSH))",3");
    __asm("GOTO" ___mkstr(PAGEMASK(STACKFAIL)));
    __asm("BCF" ___mkstr(BANKMASK(TOSH))",3");
    __asm("BTFSS" ___mkstr(BANKMASK(TOSH))",4");
    __asm("GOTO" ___mkstr(PAGEMASK(STACKFAIL)));
    __asm("BCF" ___mkstr(BANKMASK(TOSH))",4");
    __asm("BTFSS" ___mkstr(BANKMASK(TOSH))",5");
    __asm("GOTO" ___mkstr(PAGEMASK(STACKFAIL)));
    __asm("BCF " ___mkstr(BANKMASK(TOSH))",5");
    __asm("BTFSS" ___mkstr(BANKMASK(TOSH))",6");
    __asm("GOTO" ___mkstr(PAGEMASK(STACKFAIL)));
    __asm("BCF " ___mkstr(BANKMASK(TOSH))",6");
    __asm("MOVF " ___mkstr(BANKMASK(STKPTR))", W");
    __asm("XORLW   0x0F");
    __asm("BTFSS   STATUS, 0x02");
    __asm("GOTO" ___mkstr(PAGEMASK(Rd1Wr0ASC)));

    // Read zero, Write 1 - descending
    __asm("MOVLW   0x10");
    __asm("MOVWF" ___mkstr(BANKMASK(STKPTR)));
    __asm("Rd0Wr1DES:");
    __asm("DECF" ___mkstr(BANKMASK(STKPTR)));
    __asm("BTFSC" ___mkstr(BANKMASK(TOSH))",6");
    __asm("GOTO" ___mkstr(PAGEMASK(STACKFAIL)));
    __asm("BSF" ___mkstr(BANKMASK(TOSH))",6");
    __asm("BTFSC" ___mkstr(BANKMASK(TOSH))",5");
    __asm("GOTO" ___mkstr(PAGEMASK(STACKFAIL)));
    __asm("BSF" ___mkstr(BANKMASK(TOSH))",5");
    __asm("BTFSC" ___mkstr(BANKMASK(TOSH))",4");
    __asm("GOTO" ___mkstr(PAGEMASK(STACKFAIL)));
    __asm("BSF" ___mkstr(BANKMASK(TOSH))",4");
    __asm("BTFSC" ___mkstr(BANKMASK(TOSH))",3");
    __asm("GOTO" ___mkstr(PAGEMASK(STACKFAIL)));
    __asm("BSF" ___mkstr(BANKMASK(TOSH))",3");
    __asm("BTFSC" ___mkstr(BANKMASK(TOSH))",2");
    __asm("GOTO" ___mkstr(PAGEMASK(STACKFAIL)));
    __asm("BSF" ___mkstr(BANKMASK(TOSH))",2");
    __asm("BTFSC" ___mkstr(BANKMASK(TOSH))",1");
    __asm("GOTO" ___mkstr(PAGEMASK(STACKFAIL)));
    __asm("BSF" ___mkstr(BANKMASK(TOSH))",1");
    __asm("BTFSC" ___mkstr(BANKMASK(TOSH))",0");
    __asm("GOTO" ___mkstr(PAGEMASK(STACKFAIL)));
    __asm("BSF" ___mkstr(BANKMASK(TOSH))",0");
    __asm("BTFSC" ___mkstr(BANKMASK(TOSL))",7");
    __asm("GOTO" ___mkstr(PAGEMASK(STACKFAIL)));
    __asm("BSF" ___mkstr(BANKMASK(TOSL))",7");
    __asm("BTFSC" ___mkstr(BANKMASK(TOSL))",6");
    __asm("GOTO" ___mkstr(PAGEMASK(STACKFAIL)));
    __asm("BSF" ___mkstr(BANKMASK(TOSL))",6");
    __asm("BTFSC" ___mkstr(BANKMASK(TOSL))",5");
    __asm("GOTO" ___mkstr(PAGEMASK(STACKFAIL)));
    __asm("BSF" ___mkstr(BANKMASK(TOSL))",5");
    __asm("BTFSC" ___mkstr(BANKMASK(TOSL))",4");
    __asm("GOTO" ___mkstr(PAGEMASK(STACKFAIL)));
    __asm("BSF" ___mkstr(BANKMASK(TOSL))",4");
    __asm("BTFSC" ___mkstr(BANKMASK(TOSL))",3");
    __asm("GOTO" ___mkstr(PAGEMASK(STACKFAIL)));
    __asm("BSF" ___mkstr(BANKMASK(TOSL))",3");
    __asm("BTFSC" ___mkstr(BANKMASK(TOSL))",2");
    __asm("GOTO" ___mkstr(PAGEMASK(STACKFAIL)));
    __asm("BSF" ___mkstr(BANKMASK(TOSL))",2");
    __asm("BTFSC" ___mkstr(BANKMASK(TOSL))",1");
    __asm("GOTO" ___mkstr(PAGEMASK(STACKFAIL)));
    __asm("BSF" ___mkstr(BANKMASK(TOSL))",1");
    __asm("BTFSC" ___mkstr(BANKMASK(TOSL))",0");
    __asm("GOTO" ___mkstr(PAGEMASK(STACKFAIL)));
    __asm("BSF " ___mkstr(BANKMASK(TOSL))",0");
    __asm("MOVF" ___mkstr(BANKMASK(STKPTR))", F");
    __asm("BTFSS   STATUS, 0x02");
    __asm("GOTO" ___mkstr(PAGEMASK(Rd0Wr1DES)));

    // Read one, Write zero - descending
    __asm("MOVLW 0x10");
    __asm("MOVWF" ___mkstr(BANKMASK(STKPTR)));
    __asm("Rd1Wr0DES :");
    __asm("DECF" ___mkstr(BANKMASK(STKPTR)));
    __asm("BTFSS" ___mkstr(BANKMASK(TOSH))", 6");
    __asm("GOTO" ___mkstr(PAGEMASK(STACKFAIL)));
    __asm("BCF" ___mkstr(BANKMASK(TOSH))", 6");
    __asm("BTFSS" ___mkstr(BANKMASK(TOSH))", 5");
    __asm("GOTO" ___mkstr(PAGEMASK(STACKFAIL)));
    __asm("BCF" ___mkstr(BANKMASK(TOSH))", 5");
    __asm("BTFSS" ___mkstr(BANKMASK(TOSH))", 4");
    __asm("GOTO" ___mkstr(PAGEMASK(STACKFAIL)));
    __asm("BCF" ___mkstr(BANKMASK(TOSH))", 4");
    __asm("BTFSS " ___mkstr(BANKMASK(TOSH))", 3");
    __asm("GOTO" ___mkstr(PAGEMASK(STACKFAIL)));
    __asm("BCF" ___mkstr(BANKMASK(TOSH))", 3");
    __asm("BTFSS " ___mkstr(BANKMASK(TOSH))", 2");
    __asm("GOTO" ___mkstr(PAGEMASK(STACKFAIL)));
    __asm("BCF" ___mkstr(BANKMASK(TOSH))", 2");
    __asm("BTFSS" ___mkstr(BANKMASK(TOSH))", 1");
    __asm("GOTO" ___mkstr(PAGEMASK(STACKFAIL)));
    __asm("BCF" ___mkstr(BANKMASK(TOSH))", 1");
    __asm("BTFSS" ___mkstr(BANKMASK(TOSH))", 0");
    __asm("GOTO" ___mkstr(PAGEMASK(STACKFAIL)));
    __asm("BCF" ___mkstr(BANKMASK(TOSH))", 0");
    __asm("BTFSS" ___mkstr(BANKMASK(TOSL))", 7");
    __asm("GOTO" ___mkstr(PAGEMASK(STACKFAIL)));
    __asm("BCF" ___mkstr(BANKMASK(TOSL))", 7");
    __asm("BTFSS" ___mkstr(BANKMASK(TOSL))", 6");
    __asm("GOTO" ___mkstr(PAGEMASK(STACKFAIL)));
    __asm("BCF" ___mkstr(BANKMASK(TOSL))", 6");
    __asm("BTFSS" ___mkstr(BANKMASK(TOSL))", 5");
    __asm("GOTO" ___mkstr(PAGEMASK(STACKFAIL)));
    __asm("BCF" ___mkstr(BANKMASK(TOSL))", 5");
    __asm("BTFSS" ___mkstr(BANKMASK(TOSL))", 4");
    __asm("GOTO" ___mkstr(PAGEMASK(STACKFAIL)));
    __asm("BCF" ___mkstr(BANKMASK(TOSL))", 4");
    __asm("BTFSS" ___mkstr(BANKMASK(TOSL))", 3");
    __asm("GOTO" ___mkstr(PAGEMASK(STACKFAIL)));
    __asm("BCF" ___mkstr(BANKMASK(TOSL))", 3");
    __asm("BTFSS" ___mkstr(BANKMASK(TOSL))", 2");
    __asm("GOTO" ___mkstr(PAGEMASK(STACKFAIL)));
    __asm("BCF" ___mkstr(BANKMASK(TOSL))", 2");
    __asm("BTFSS" ___mkstr(BANKMASK(TOSL))",1 ");
    __asm("GOTO" ___mkstr(PAGEMASK(STACKFAIL)));
    __asm("BCF" ___mkstr(BANKMASK(TOSL))", 1");
    __asm("BTFSS" ___mkstr(BANKMASK(TOSL))", 0");
    __asm("GOTO" ___mkstr(PAGEMASK(STACKFAIL)));
    __asm("BCF" ___mkstr(BANKMASK(TOSL))", 0");
    __asm("MOVF" ___mkstr(BANKMASK(STKPTR))", F");
    __asm("BTFSS STATUS, 0x02");
    __asm("GOTO" ___mkstr(PAGEMASK(Rd1Wr0DES)));

    // Read zero - ascending
    __asm("MOVLW 0x1F");
    __asm("MOVWF" ___mkstr(BANKMASK(STKPTR)));
    __asm("Rd0ASC :");
    __asm("INCF" ___mkstr(BANKMASK(STKPTR)));
    __asm("MOVLW 0x00");
    __asm("XORWF" ___mkstr(BANKMASK(TOSL)));
    __asm("BTFSS STATUS, 2");
    __asm("GOTO" ___mkstr(PAGEMASK(STACKFAIL)));
    __asm("MOVLW 0X00");
    __asm("XORWF" ___mkstr(BANKMASK(TOSH)));
    __asm("BTFSS STATUS, 2");
    __asm("GOTO" ___mkstr(PAGEMASK(STACKFAIL)));
    __asm("MOVF" ___mkstr(BANKMASK(STKPTR))", W");
    __asm("XORLW 0x0F");
    __asm("BTFSS STATUS, 0x02");
    __asm("GOTO" ___mkstr(PAGEMASK(Rd0ASC)));

    // The Stack test passed
    __asm("GOTO" ___mkstr(PAGEMASK(STACKEND)));

    // The Stack test failed
    __asm("STACKFAIL :");
    stackTestResult = 0xFFU;
    __asm("STACKEND :");
    __asm("NOP");

    // Restore Stack here
    DIAG_MarchBufferStackAddress--;
    for (WREG_SHAD = STACK_LEVEL; WREG_SHAD >= 1; WREG_SHAD--)
    {
        STKPTR = WREG_SHAD - 1;
        TOSH = *DIAG_MarchBufferStackAddress;
        DIAG_MarchBufferStackAddress--;
        TOSL = *DIAG_MarchBufferStackAddress;
        DIAG_MarchBufferStackAddress--;
    }

    STKPTR = *DIAG_MarchBufferStackAddress;
    INTCONbits.GIE = globalInt;

    if (stackTestResult == (uint8_t) 0xFF)
    {
        status = DIAG_FAIL;
    }
    else
    {
        status = DIAG_PASS;
    }

    /*Uncomment the following line when using with Optimization level 's' -- valid only for XC8 v2.20 and later */
    __asm("ASMOPT POP");

    return status;
}