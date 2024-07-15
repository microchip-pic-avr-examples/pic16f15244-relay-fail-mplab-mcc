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
 *  @file  diag_sram_definitions.h
 *  @brief This file contains common macros and return types for SRAM diagnostics modules
 * 
 * @note
 * Microchip Technology Inc. has followed development methods required by 
 * Functional Safety Standards and performed extensive validation and static
 * testing to ensure that the code operates as intended. Any modification to the 
 * code can invalidate the results of Microchip's validation and testing.
 */

#ifndef DIAG_SRAM_DEFINITIONS_H
#define DIAG_SRAM_DEFINITIONS_H
 
#include <stdint.h>

/**
@def SRAM_GLOBAL_LENGTH 
Defines the length of the SRAM space reserved for global variables to run the test.
 */
#define SRAM_GLOBAL_LENGTH  (0x07U)
/**
@def MIN_LENGTH
Defines the minimum length of a test bank.
 */
#define MIN_LENGTH	   (0x00U)

/**
@def MIN_LENGTH
Defines the maximum length of a test bank.
 */
#define MAX_LENGTH	   (0x5FU)
/**
@def RESERVED_SRAM
Defines the reserved SRAM locations for the March test.
 */
#define RESERVED_SRAM	   (sizeof(struct DIAG_Variables))
/**
@def DIAG_MEM_START
Defines the start address where the test variables are stored.
 */
#define DIAG_MEM_START     (0x20U)
/**
@def DIAG_MEM_END
Defines the last location used by the test variables.
 */
#define DIAG_MEM_END       (0x2CU)
/**
@def STARTADDR_LOC
Defines the RAM address where the March test start address is stored.
 */
#define STARTADDR_LOC   (0x20U)

/**
@def BUFFERADDR_LOC
Defines the RAM address where the March test buffer address is stored.
 */
#define BUFFERADDR_LOC  (0x22U)

/**
@def TEMPADDR_LOC
Defines the RAM address where the March test temporary address is stored.
 */
#define TEMPADDR_LOC    (0x24U)

/**
@def INDEX_LOC
Defines the RAM address where the March test index is stored.
 */
#define INDEX_LOC  (0x28U)

/**
@def LENGTH_LOC
Defines the RAM address where the March test length is stored.
 */
#define LENGTH_LOC  (0x26U)

/**
@def TEST_INDEX_LOC
Defines the RAM address where the bit under test is stored.
 */
#define TEST_BIT_LOC  (0x2AU)

/**
@def TEST_VARIABLE1_LOC
Defines the RAM address where the March test variable is stored.
 */
#define TEST_VARIABLE1_LOC  (0x2BU)

/**
@def TEST_VARIABLE2_LOC
Defines the RAM address where the March test variable is stored.
 */
#define TEST_VARIABLE2_LOC  (0x2CU)

/**
@def ERROR_FLAG_LOC
Defines the RAM address where the March test variable is stored.
 */
#define ERROR_FLAG_LOC  (0x2DU)

/**
@def SFR_REGION_LOC1
Defines the last address in the first SFR region.
 */
#define SFR_REGION_LOC1 (0x1FU)

/**
@def SFR_REGION_LOC2
Defines the first address in the second SFR region.
 */
#define SFR_REGION_LOC2 (0x80U)

/**
@def SFR_REGION_LOC3
Defines the last address in the second SFR region.
 */
#define SFR_REGION_LOC3 (0x9FU)

/**
@def ADDR_MASK
Used to get the lower byte of a test address for validation.
 */
#define ADDR_MASK (0x00FFU)

/**
 * @brief Variables used for the SRAM March tests are stored at the first seven RAM addresses.
 * This struct holds the variables needed for the SRAM test.
 */
struct DIAG_Variables 
{
    uint16_t startAddress;          // This variable holds the start address of the SRAM under test.
    uint16_t bufferAddress;         // This variable holds the start address of the buffer used to save the data already present in SRAM.
    uint16_t length;                // This variable holds the number of bytes to test.
    uint16_t tempAddress;           // This variable is used for copying data between the SRAM under test and the buffer.
    uint16_t index;                 // This variable is an index for looping through the SRAM under test.
    uint8_t testBit;                // This variable is an index for looping through each bit of SRAM under test.
    uint8_t var1;                   // This variable for writing and testing the individual byte of SRAM.
    diag_result_t errorFlag;        // This variable holds the error status of the test.
};

/**
 * @brief This is a global instance of the DIAG_Variables struct.
 * The members of this struct can provide error details in the
 * case of a test failure. 
 */
 struct  DIAG_Variables __at(DIAG_MEM_START) DIAG_SRAM;

/**
 * @brief These test variables used for the SRAM Checkerboard
 * tests are stored at the first 14 RAM addresses.
 */

 uint16_t DIAG_CheckerboardStartAddress __at(STARTADDR_LOC);
 uint16_t DIAG_CheckerboardBufferAddress __at(BUFFERADDR_LOC);
 uint16_t DIAG_CheckerboardTempAddress __at(TEMPADDR_LOC);
 uint8_t DIAG_CheckerboardLength __at(LENGTH_LOC);

#endif //DIAG_SRAM_DEFINITIONS_H

