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
 * @file  diag_sram_checkerboard.h
 * @defgroup diag_sram_checkerboard SRAM Checkerboard Diagnostic Test
 * @brief This file contains APIs for SRAM Checkerboard test.
 * 
 * @note 
 * Microchip Technology Inc. has followed development methods required by 
 * Functional Safety Standards and performed extensive validation and static
 * testing to ensure that the code operates as intended. Any modification to the 
 * code can invalidate the results of Microchip's validation and testing.
 */
 
#ifndef DIAG_SRAM_CHECKERBOARD_H
/* cppcheck-suppress misra-c2012-2.5 */
#define DIAG_SRAM_CHECKERBOARD_H

#include "../../../diag_common/diag_result_type.h"
#include "../../../diag_common/config/diag_config.h"

/**
@ingroup diag_sram_checkerboard
@brief The Checkerboard RAM test writes the checkerboard patterns to a sequence of adjacent memory locations.
This test is performed in units (memory chunks) of four bytes. This is a non-destructive memory test.
This test performs the following major tasks:
1. Saves the contents of the memory locations to be tested in the buffer defined by bufferAddress.
2. Writes the binary value (length is dependent upon architecture) 101010... to the memory location, 'N', and the inverted binary value,
010101..., to the memory location, 'N+1', and so on, until the whole memory chunk is filled.
3. Reads the contents of all the memory locations in the current chunk and verifies its contents. If the values match, 
the function continues; otherwise, it stops and returns an error.
4. Step 2 and 3 are repeated by writing the inverted pattern to the same locations.
5. Once a memory chunk is completed the test of the next chunk is started until all of the requested memory areas are tested.
@param [in] startAddress Address to an array declared in user memory for test
@param [in] length Number of bytes to test
@param [in] bufferAddress Address to location in memory where array will be stored during test
@return @ref DIAG_PASS - SRAM CF is not detected
@return @ref DIAG_FAIL - SRAM CF is detected
@return @ref DIAG_INVALID_ARG - When SRAM test addresses, test length, test region is invalid
 */
diag_result_t DIAG_SRAM_Checkerboard(uint16_t startAddress, uint8_t length, uint16_t bufferAddress);

#endif //DIAG_SRAM_CHECKERBOARD_H