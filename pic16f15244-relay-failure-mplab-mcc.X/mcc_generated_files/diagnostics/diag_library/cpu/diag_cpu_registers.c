/**
 * © 2023 Microchip Technology Inc. and its subsidiaries.
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
 *  @file  diag_cpu_registers.c
 *  @brief This file contains the implementation of the the APIs to test CPU Registers.
 *  @ingroup diag_cpu_register
 * @note 
 * Microchip Technology Inc. has followed development methods required by 
 * Functional Safety Standards and performed extensive validation and static
 * testing to ensure that the code operates as intended. Any modification to the 
 * code can invalidate the results of Microchip's validation and testing.
 */

#include "diag_cpu_registers.h"
#include <stdint.h>
#include <stdbool.h>
#include <xc.h>

/* This API restores the values of the CPU registers from RAM after the diagnostic test is performed */
static void DIAG_CPU_RegistersRestore(void)
{
    FSR0L = FSR0L_SHAD;
    FSR0H = FSR0H_SHAD;
    FSR1L = FSR1L_SHAD;
    FSR1H = FSR1H_SHAD;
    BSR = BSR_SHAD;
}

diag_result_t DIAG_CPU_Registers(void)
{
    volatile diag_result_t testResult = DIAG_FAIL;
    static uint8_t tempBSR;

    tempBSR = BSR;
    bool globalInt;
    volatile bool diag_error = false;

    // Save global interrupt enable bit value for restoration
    globalInt = INTCONbits.GIE;

    // Disable the interrupts
    INTCONbits.GIE = 0U;

    /*****************************************************************************
     *   Test WREG
     ******************************************************************************/
    // Test WREG with 0x55
    __asm("MOVLW 0x55");
    __asm("XORLW 0x55"); // XOR the content of WREG register with 0x55

    if (WREG != 0U)
    {
        asm("BRA done");
    }

    // Test WREG with 0xAA
    __asm("MOVLW 0xAA");
    __asm("XORLW 0xAA");

    if (WREG != 0U)
    {
        asm("BRA done");
    }

    /*****************************************************************************
     *   Test WREG_SHAD
     ******************************************************************************/
    // Test WREG_SHAD with 0xAA
    WREG_SHAD = 0xAAU;
    if (WREG_SHAD != 0xAAU)
    {
        asm("BRA done");
    }
    // Test WREG_SHAD with 0x55
    WREG_SHAD = 0x55U;
    if (WREG_SHAD != 0x55U)
    {
        asm("BRA done");
    }

    /*****************************************************************************
     *   Test BSR_SHAD
     ******************************************************************************/
    // Test BSR_SHAD with 0x0A
    BSR_SHAD = 0x0AU;
    if (BSR_SHAD != 0x0AU)
    {
        asm("BRA done");
    }
    // Test BSR_SHAD with 0x15
    BSR_SHAD = 0x15U;
    if (BSR_SHAD != 0x15U)
    {
        asm("BRA done");
    }

    /*****************************************************************************
     *   Test FSR0L_SHAD
     ******************************************************************************/
    // Test FSR0L_SHAD with 0xAA
    FSR0L_SHAD = 0xAAU;
    if (FSR0L_SHAD != 0xAAU)
    {
        asm("BRA done");
    }
    // Test FSR0L_SHAD with 0x55
    FSR0L_SHAD = 0x55U;
    if (FSR0L_SHAD != 0x55U)
    {
        asm("BRA done");
    }

    /*****************************************************************************
     *   Test FSR0H_SHAD
     ******************************************************************************/
    // Test FSR0H_SHAD with 0xAA
    FSR0H_SHAD = 0xAAU;
    if (FSR0H_SHAD != 0xAAU)
    {
        asm("BRA done");
    }
    // Test FSR0H_SHAD with 0x55
    FSR0H_SHAD = 0x55U;
    if (FSR0H_SHAD != 0x55U)
    {
        asm("BRA done");
    }

    /*****************************************************************************
     *   Test FSR1L_SHAD
     ******************************************************************************/
    // Test FSR1L_SHAD with 0xAA
    FSR1L_SHAD = 0xAAU;
    if (FSR1L_SHAD != 0xAAU)
    {
        asm("BRA done");
    }
    // Test FSR1L_SHAD with 0x55
    FSR1L_SHAD = 0x55U;
    if (FSR1L_SHAD != 0x55U)
    {
        asm("BRA done");
    }

    /*****************************************************************************
     *   Test FSR1H_SHAD
     ******************************************************************************/
    // Test FSR1H_SHAD with 0xAA
    FSR1H_SHAD = 0xAAU;
    if (FSR1H_SHAD != 0xAAU)
    {
        asm("BRA done");
    }
    // Test FSR1H_SHAD with 0x55
    FSR1H_SHAD = 0x55U;
    if (FSR1H_SHAD != 0x55U)
    {
        asm("BRA done");
    }

    /*****************************************************************************
     *   Save User Data in shadow registers
     ******************************************************************************/
    BSR_SHAD = tempBSR;
    FSR0L_SHAD = FSR0L;
    FSR0H_SHAD = FSR0H;
    FSR1L_SHAD = FSR1L;
    FSR1H_SHAD = FSR1H;

    /*****************************************************************************
     *   Test BSR
     ******************************************************************************/
    // Only BSR<4:0> are R/W
    // Test BSR with 0x0A

    __asm("MOVLB 0x0A");
    __asm("MOVLW 0x0A");
    __asm("SUBWF BSR, W");
    if (WREG != 0x00U)
    {
        asm("BRA done");
    }
    // Test BSR with 0x15
    __asm("MOVLB 0x15");
    __asm("MOVLW 0x15");
    __asm("SUBWF BSR, W");
    if (WREG != 0x00U)
    {
        asm("BRA done");
    }

    /*****************************************************************************
     *   Test FSR
     ******************************************************************************/
    // Clear FSR
    FSR0 = 0U;
    FSR1 = 0U;
    //Test FSR0 with 0x5555
    FSR0 = 0x5555U;
    if (FSR0 != 0x5555U)
    {
        asm("BRA done");
    }
    // Test FSR0 with 0xAAAA
    FSR0 = 0xAAAAU;
    if (FSR0 != 0xAAAAU)
    {
        asm("BRA done");
    }
    // Test FSR1 with 0x5555
    FSR1 = 0x5555U;
    if (FSR1 != 0x5555U)
    {
    }
    // Test FSR1 with 0xAAAA
    FSR1 = 0xAAAAU;
    if (FSR1 != 0xAAAAU)
    {
        asm("BRA done");
    }

    // Reaching this point indicates that all tests have passed
    testResult = DIAG_PASS;

    asm("done : ");
    DIAG_CPU_RegistersRestore();

    // Restore global interrupt enable bit status
    INTCONbits.GIE = globalInt;

    return testResult;
}