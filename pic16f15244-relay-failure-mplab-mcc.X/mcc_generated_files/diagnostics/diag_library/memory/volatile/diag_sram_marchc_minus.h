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
 * @file diag_sram_marchc_minus.h
 * @defgroup diag_sram_marchc_minus SRAM March C- Diagnostic Test
 * @brief This file contains APIs to test SRAM using March C- algorithm.
 * 
 * @note 
 * Microchip Technology Inc. has followed development methods required by 
 * Functional Safety Standards and performed extensive validation and static
 * testing to ensure that the code operates as intended. Any modification to the 
 * code can invalidate the results of Microchip's validation and testing.
 */

#ifndef DIAG_SRAM_MARCHC_MINUS_H
/* cppcheck-suppress misra-c2012-2.5 */
#define DIAG_SRAM_MARCHC_MINUS_H

#include "../../../diag_common/diag_result_type.h"
#include "../../../diag_common/config/diag_config.h"

/**
 @ingroup diag_sram_marchc_minus
 @brief Implements the March C- test on the SRAM.
It is a complete and irredundant test capable of detecting stuck-at faults,
addressing faults, transition faults, and coupling faults.
Since this test is a destructive test, a buffer must be provided
to save and restore user memory during the test.

March C- test algorithm is executed in six steps:
Step 1: Ascending(w0)
Step 2: Ascending(r0, w1)
Step 3: Ascending(r1, w0)
Step 4: Descending(r0, w1)
Step 5: Descending(r1, w0)
Step 6: Ascending(r0)

@param [in] startAddress Address of the first SRAM location to test
@param [in] length Number of bytes to test
@param [in] bufferAddress Address of buffer where user data will be stored during test

@return @ref DIAG_PASS - No faults detected
@return @ref DIAG_FAIL - Fault detected
@return @ref DIAG_INVALID_ARG - Invalid length or address is passed to this function
 */
diag_result_t DIAG_SRAM_MarchCMinus(uint16_t startAddress, uint8_t length, uint16_t bufferAddress);

#endif //DIAG_SRAM_MARCHC_MINUS_H