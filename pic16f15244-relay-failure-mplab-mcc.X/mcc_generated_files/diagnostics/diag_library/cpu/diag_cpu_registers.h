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
 * @file  diag_cpu_registers.h
 * @defgroup diag_cpu_register CPU Register Diagnostic Test
 * @brief This file contains API prototypes and other data types to test CPU Registers \n
 * 
 * @note 
 * Microchip Technology Inc. has followed development methods required by 
 * Functional Safety Standards and performed extensive validation and static
 * testing to ensure that the code operates as intended. Any modification to the 
 * code can invalidate the results of Microchip's validation and testing.
 */



#ifndef DIAG_CPU_REGISTERS_H
/* cppcheck-suppress misra-c2012-2.5 */
#define DIAG_CPU_REGISTERS_H

#include <stdbool.h>
#include <stdint.h>
#include "../../diag_common/diag_result_type.h"

/**
 @ingroup diag_cpu_register
 @brief Validates the CPU Registers for stuck bits and coupling faults.
The registers are tested in the following order:
 1. WREG
 2. WREG_SHAD
 3. BSR_SHAD
 4. FSR0L_SHAD
 5. FSR0H_SHAD
 6. FSR1L_SHAD
 7. FSR1H_SHAD
 8. BSR
 9. FSR\n
 This test checks the integrity of the registers by testing for stuck bits and coupling faults, ensuring that the bits
 in the registers are not stuck at a value. The test can be done successively doing a write, read and 
 check of 0x55 and 0xAA value into the CPU registers.
 The register is saved into RAM before the test is performed and restored after the test is done.
 @param None.
 @return DIAG_PASS - indicates that CPU Register test did not detect a fault.
 @return DIAG_FAIL - indicates that CPU Register detected a fault.
 */
diag_result_t DIAG_CPU_Registers(void);

#endif  //DIAG_CPU_REGISTERS_H