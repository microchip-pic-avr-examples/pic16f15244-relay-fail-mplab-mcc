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
 *  @file  diag_wdt_startup.h
 *  @defgroup diag_wdt_startup Watchdog Timer Diagnostic Test
 *  @brief This file contains API prototypes for the Watchdog test (Simple Timer Mode) using Independent Time-Slot Monitoring.
 * 
 * @note 
 * Microchip Technology Inc. has followed development methods required by 
 * Functional Safety Standards and performed extensive validation and static
 * testing to ensure that the code operates as intended. Any modification to the 
 * code can invalidate the results of Microchip's validation and testing.
 */

#ifndef DIAG_WDT_STARTUP_H
/* cppcheck-suppress misra-c2012-2.5 */
#define DIAG_WDT_STARTUP_H

#include "../../diag_common/diag_result_type.h"


/**
 * @ingroup diag_wdt_startup
 * @brief This API checks the WDT resets when the timer is being cleared either too early or too late.
 * This test will not be called from main. It will be executed before main is entered, as long as 
 * diag_wdt_startup.h/.c and powerup_xx.as are included in the compilation.
 * This is the simple timer based WDT self-diagnostic test.
 * It tests that the WDT resets the device when the WDT counter is not cleared. 
 * The next step tests that the WDT counter will be cleared without resetting the device. 
 * Once this is confirmed, the device is again reset by the WDT. 
 * This function tests the following:
 * 1. WDT issues a system Reset after time-out.
 * 2. WDT timing and that it can be cleared.
 *
 * The expected (error-free) execution flow:
 * 1. Power-On Reset or External Reset has occurred
 * 2. Check that WDT can issue a system Reset. Set test state to Simple Timer and system Reset
 * 3. Check that WDT can be cleared
 * 4. Set test state to "ok" and continue to main()
 * TMR0 is used as an independent clock source to the WDT clock, which is
 * used to check the timing of the oscillator used by the WDT.
 * If the device and WDT was Reset correctly in each step of the test, then the @ref diag_wdt_teststate is set to
 * @ref DIAG_WDT_FSM_OK and @ref diag_wdt_status is set to @ref DIAG_PASS, otherwise the @ref diag_wdt_teststate is set to @ref DIAG_WDT_FAULT
 * and @ref diag_wdt_status is set to @ref DIAG_FAIL thereby indicating fault.
 * Error reporting:
 * Call @ref DIAG_WDT_GetResult() from main to see the status of the WDT diagnosis.
 * @param None.
 * @return See @ref DIAG_WDT_GetResult()
 */
void DIAG_WDT_Startup(void);

/**
 * @ingroup diag_wdt_startup
 * @brief Returns the status of the WDT diagnosis. 
 * @param None.
 * @return @ref DIAG_PASS - No errors detected
 * @return @ref DIAG_FAIL - Error detected

 */
diag_result_t DIAG_WDT_GetResult(void);

#endif // DIAG_WDT_STARTUP_H