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
 * @file    diag_flash_crc16_lookup.c
 * @brief   Contains the implementation of the APIs to test the Flash memory using 
 *          CRC16 CCITT Look-up Table algorithm.
 * @ingroup diag_flash_crc16
 * @note 
 * Microchip Technology Inc. has followed development methods required by 
 * Functional Safety Standards and performed extensive validation and static
 * testing to ensure that the code operates as intended. Any modification to the 
 * code can invalidate the results of Microchip's validation and testing.
 */

#include "diag_flash_crc16.h"
#include "diag_crc16_lookup_table.h"
#include "../../../diag_common/config/diag_config.h"

/*
 * reference_crc variable is located at the Flash address to be used to store reference CRC.
 * As program code will not occupy these locations compile time, those will be reserved
 * for storing reference CRC.
 */
const uint16_t reference_crc __at(DIAG_FLASH_CRC_STORE_ADDR) = 0x0;

static nvm_status_t DIAG_FLASH_StoreCRC16(flash_address_t flash_adr, uint16_t data)
{
    /* cppcheck-suppress misra-c2012-18.8 */    
	flash_data_t wrData[PROGMEM_PAGE_SIZE];
    flash_address_t pageStart;
    uint16_t offset;
    nvm_status_t status = NVM_OK;

    //Get the starting address of the page containing flash_adr
    pageStart = FLASH_ErasePageAddressGet(flash_adr);

    //Read the entire page in temporary buffer
    if (!NVM_IsBusy())
    {
        for (offset = 0; offset < PROGMEM_PAGE_SIZE; offset++)
        {
            wrData[offset] = FLASH_Read(pageStart + offset);
        }
    }
	if(NVM_ERROR != status)
	{

		//Get the offset to flash_adr and update temporary buffer with new data
		offset = FLASH_ErasePageOffsetGet(flash_adr);
		wrData[offset] = (data >> 8U);
		wrData[offset + 1U] = (flash_data_t) data;

		//Erase entire Flash page
		if (!NVM_IsBusy())
    	{
        NVM_UnlockKeySet(UNLOCK_KEY);
        status = FLASH_PageErase(pageStart);

        NVM_UnlockKeyClear();

        if (status == NVM_ERROR)
        {
            NVM_StatusClear();
			}
		}
	}
	else
	{
		//Do nothing
	}
	if(NVM_ERROR != status)
	{
		//Write updated data to Flash row
		if (!NVM_IsBusy())
		{
			NVM_UnlockKeySet(UNLOCK_KEY);
			status = FLASH_RowWrite(pageStart, wrData);

			NVM_UnlockKeyClear();
		}
	}
	else
	{
		//Do nothing
	}

    return status;
}

static void DIAG_FLASH_CalculateCRC(flash_address_t startAddress, uint32_t length, uint16_t *crcSeed)
{
    uint32_t i;
    uint8_t readByte;

    for (i = 0U; i < length; i++)
    {
        readByte = FLASH_Read(((flash_address_t)(startAddress + i)));
        readByte ^= *crcSeed >> 8U;
        *crcSeed = DIAG_CRC16Table[readByte] ^ (*crcSeed << 8U);
    }
}

diag_result_t DIAG_FLASH_CalculateStoreCRC(flash_address_t startAddress, uint32_t length, flash_address_t storeAddress)
{
    diag_result_t testStatus;
    uint16_t crc = CRC16_INITIAL_SEED;
    bool storeAddrInsideEvaluatedArea = (((storeAddress + CRC_LSB_POS_16BIT) >= startAddress) && (storeAddress < (startAddress + length)));
    bool storeAddrOutsideFlash = ((storeAddress + CRC_LSB_POS_16BIT) >= PROGMEM_SIZE);

    //Check for valid length
    if ((length == 0U) || ((startAddress + length) > PROGMEM_SIZE))
    {
        testStatus = DIAG_INVALID_ARG;
    }
        //Check if storeAddress is not included in flash region to be scanned for CRC
    else if (storeAddrInsideEvaluatedArea || storeAddrOutsideFlash)
    {
        testStatus = DIAG_INVALID_ARG;
    }
    else
    {
        //Calculate CRC-16 CCITT for given Flash region
        (void) DIAG_FLASH_CalculateCRC(startAddress, length, &crc);
        
        //Store the calculated CRC to given address
        if (NVM_ERROR == DIAG_FLASH_StoreCRC16(storeAddress, crc))
        {
            testStatus = DIAG_NVM_STORE_ERROR;
        }
        else
        {
            testStatus = DIAG_PASS;
        }
    }

    return testStatus;
}

diag_result_t DIAG_FLASH_ValidateCRC(flash_address_t startAddress, uint32_t length, flash_address_t refAddress)
{
    diag_result_t testStatus;
    uint16_t crc = CRC16_INITIAL_SEED;
    uint16_t refCRC;
    bool refAddrInsideEvaluatedArea = (((refAddress + CRC_LSB_POS_16BIT) >= startAddress) && (refAddress < (startAddress + length)));
    bool refAddrOutsideFlash = ((refAddress + CRC_LSB_POS_16BIT) >= PROGMEM_SIZE);

    //Check for valid length
    if ((length == 0U) || ((startAddress + length) > PROGMEM_SIZE))
    {
        testStatus = DIAG_INVALID_ARG;
    }
        //Check if refAddress is not included in flash region to be scanned for CRC
    else if (refAddrInsideEvaluatedArea || refAddrOutsideFlash)
    {
        testStatus = DIAG_INVALID_ARG;
    }
    else
    {
        //Calculate CRC-16 CCITT for given Flash region
        (void) DIAG_FLASH_CalculateCRC(startAddress, length, &crc);

        //Read the reference CRC
        refCRC = (uint16_t) (
                ((uint16_t) FLASH_Read(refAddress + 1U)) |
                ((uint16_t) FLASH_Read(refAddress) << 8U)
                );

        if (refCRC != crc)
        {
            testStatus = DIAG_FAIL;
        }
        else
        {
            testStatus = DIAG_PASS;
        }
    }

    return testStatus;
}