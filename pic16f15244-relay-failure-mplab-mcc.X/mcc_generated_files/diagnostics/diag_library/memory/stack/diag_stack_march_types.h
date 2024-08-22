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
 *  @file   diag_stack_march_types.h
 *  @defgroup diag_stack_marchc_minus Stack March C- Diagnostic Test
 *  @brief This file contains variables used in the Stack March C- algorithm.
 *
 * @note 
 * Microchip Technology Inc. has followed development methods required by 
 * Functional Safety Standards and performed extensive validation and static
 * testing to ensure that the code operates as intended. Any modification to the 
 * code can invalidate the results of Microchip's validation and testing.
 */

#ifndef DIAG_STACK_MARCH_TYPES_H
/* cppcheck-suppress misra-c2012-2.5 */
#define DIAG_STACK_MARCH_TYPES_H

#include <stdint.h>

/**
 * @def STACKBUFFER_LOC
 * This macro defines the RAM address where the march test buffer address is stored.
 */
#define STACKBUFFER_LOC (0x2D)

/**
 * @def STACK_BUFFER_ADDRESS
 * This macro defines the buffer address to store the contents of the stack.
 */
/* cppcheck-suppress misra-c2012-2.5 */
#define STACK_BUFFER_ADDRESS ((uint8_t*) 0x2F)

/**
 * @def STACK_LEVEL This macro defines the stack level on the enhanced mid-range PIC16F devices.
 */
/* cppcheck-suppress misra-c2012-2.5 */
#define STACK_LEVEL (16)

/**
 * @brief This test variable is used for the Stack March tests to store the Buffer Address.
 */
uint8_t* DIAG_MarchBufferStackAddress __at( STACKBUFFER_LOC);

#endif //DIAG_STACK_MARCH_TYPES_H

