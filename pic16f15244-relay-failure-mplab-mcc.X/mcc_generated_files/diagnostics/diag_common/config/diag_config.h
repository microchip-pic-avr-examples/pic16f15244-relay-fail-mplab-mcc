/*
� [2024] Microchip Technology Inc. and its subsidiaries.

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

#ifndef DIAG_CONFIG_H
#define DIAG_CONFIG_H

#include <xc.h>
#include "../../../nvm/nvm.h"

#define DIAG_FLASH_START_ADDR (0x0U)
#define DIAG_FLASH_LENGTH (2047U)
#define DIAG_FLASH_CRC_STORE_ADDR (0xffeU)

#define SRAM_CHECKERBOARD_START_ADDRESS (0x20U)
#define SRAM_CHECKERBOARD_BUFFER_ADDRESS    (0xa0U)
#define SRAM_CHECKERBOARD_LENGTH  (80U)

#define NO_OF_BANKS (7U)

// The following bank information is used as reference for SRAM Tests
#define BANK6_START_ADDRESS (0x320U)
#define BANK6_END_ADDRESS (0x330U)
#define BANK5_START_ADDRESS (0x2a0U)
#define BANK5_END_ADDRESS (0x2f0U)
#define BANK0_START_ADDRESS (0x20U)
#define BANK0_END_ADDRESS (0x70U)
#define BANK4_START_ADDRESS (0x220U)
#define BANK4_END_ADDRESS (0x270U)
#define BANK3_START_ADDRESS (0x1a0U)
#define BANK3_END_ADDRESS (0x1f0U)
#define BANK2_START_ADDRESS (0x120U)
#define BANK2_END_ADDRESS (0x170U)
#define BANK1_START_ADDRESS (0xa0U)
#define BANK1_END_ADDRESS (0xf0U)

#endif //DIAG_CONFIG_H