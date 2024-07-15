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
 *  @file  diag_sram_checkerboard.c
 *  @brief This file contains the implementation of the APIs to perform Checkerboard test on the SRAM.
 *  @ingroup diag_sram_checkerboard
 *
 * @note 
 * Microchip Technology Inc. has followed development methods required by 
 * Functional Safety Standards and performed extensive validation and static
 * testing to ensure that the code operates as intended. Any modification to the 
 * code can invalidate the results of Microchip's validation and testing.
 */
 
#include "diag_sram_checkerboard.h"
#include "diag_sram_definitions.h"

/**
 * @ingroup diag_sram_checkerboard
 * @brief This function runs the Checkerboard algorithm on the seven RAM addresses used to run the rest of the Checkerboard test.
 * @return @ref DIAG_PASS - SRAM CF is not detected.
 * @return @ref DIAG_FAIL - SRAM CF is detected.
 */
static diag_result_t Checkerboard_Global(void);

/**
@ingroup diag_sram_checkerboard
@brief This function stores DIAG_CheckerboardStartAddress into FSR1
 */
static void DIAG_SRAM_CheckerboardStoreInFSR1(void);

/**
@ingroup diag_sram_checkerboard
@brief This function saves the context of the SRAM test variables
*/
static void DIAG_SRAM_CheckerboardSave(void);

/**
@ingroup diag_sram_checkerboard
@brief This function restores the context of the SRAM test variables
*/
static void DIAG_SRAM_CheckerboardRestore(void);

/**
@ingroup diag_sram_checkerboard
@brief This function re-stores DIAG_CheckerboardStartAddress into FSR1
*/
static void DIAG_SRAM_RestoreFSR1x(void);

/**
 @misradeviation{@advisory,15.5} The function should return as soon as a Fault is
 detected. Avoiding the use of multiple return statements would require having 
 multiple additional checks which would add overhead, diminish code readability 
 and would require defaulting the test result to @ref DIAG_PASS. The latter is 
 deemed less safe than only returning @ref DIAG_PASS when all sections have been
 tested with no Fault detected. Since there is no functional termination code in
 this function, it cannot accidentally be omitted and since the function does not
 have any persistent side effects, there is no issue in discerning which side 
 effects have been applied when the function is executed/exits and thus the 
 deviation is deemed safe.
 */
diag_result_t DIAG_SRAM_Checkerboard(uint16_t startAddress, uint8_t length, uint16_t bufferAddress)
{
    uint8_t globalInt;

	//Saving the state of interrupts before test
	globalInt = INTCONbits.GIE;

	//Disable the interrupts
	INTCONbits.GIE = 0U;
	
	DIAG_CheckerboardTempAddress = 0x00U;
	DIAG_CheckerboardStartAddress = startAddress;
    DIAG_CheckerboardLength = length;
    DIAG_CheckerboardBufferAddress = bufferAddress;

    volatile uint8_t testResult;    
    volatile uint8_t testAddr;
	
	//Reset FSR1x Shadow Registers
    FSR1L_SHAD = 0x00U;
    FSR1H_SHAD = 0x00U;
	
    /* Return an error if the addresses are within the SFR RAM Space */
    testAddr = ((uint8_t) DIAG_CheckerboardStartAddress) & ADDR_MASK;
    if ((testAddr <= SFR_REGION_LOC1) || ((testAddr >= SFR_REGION_LOC2) && (testAddr <= SFR_REGION_LOC3)))
    {
        INTCONbits.GIE = globalInt;
        /* cppcheck-suppress misra-c2012-15.5 */
		return DIAG_INVALID_ARG;
    }

    testAddr = ((uint8_t) DIAG_CheckerboardBufferAddress) & ADDR_MASK;
    if ((testAddr <= SFR_REGION_LOC1) || ((testAddr >= SFR_REGION_LOC2) && (testAddr <= SFR_REGION_LOC3)))
    {
        INTCONbits.GIE = globalInt;
		/* cppcheck-suppress misra-c2012-15.5 */
        return DIAG_INVALID_ARG;
    }

    testAddr = ((uint8_t) (DIAG_CheckerboardBufferAddress + DIAG_CheckerboardLength) - 1U) & ADDR_MASK;
    if ((testAddr <= SFR_REGION_LOC1) || ((testAddr >= SFR_REGION_LOC2) && (testAddr <= SFR_REGION_LOC3)))
    {
        INTCONbits.GIE = globalInt;
		/* cppcheck-suppress misra-c2012-15.5 */
        return DIAG_INVALID_ARG;
    }
    
    /* Save memory for test in a known location designated by DIAG_CheckerboardBufferAddress
     * The DIAG_SRAM_Checkerboard... variables will not be saved 
     */

    if (DIAG_CheckerboardBufferAddress != 0x00U)
    {
        DIAG_CheckerboardTempAddress = DIAG_CheckerboardStartAddress;
        for (WREG_SHAD = DIAG_CheckerboardLength; WREG_SHAD != 0U; WREG_SHAD--)
        {
            *(uint8_t*)DIAG_CheckerboardBufferAddress = *(uint8_t*)DIAG_CheckerboardTempAddress;
            DIAG_CheckerboardBufferAddress++;
            DIAG_CheckerboardTempAddress++;
        }
    }

    DIAG_SRAM_CheckerboardStoreInFSR1();     //save the start address value before the global test
                                            
    /* Run Checkerboard Test on global RAM used by Checkerboard test */
    if (DIAG_CheckerboardStartAddress < (LENGTH_LOC + 1U))
    {
		if (DIAG_CheckerboardLength < SRAM_GLOBAL_LENGTH )
        {
            INTCONbits.GIE = globalInt;
			/* cppcheck-suppress misra-c2012-15.5 */
            return DIAG_INVALID_ARG;
        }
        testResult = Checkerboard_Global();
        if ((testResult) != DIAG_PASS)
        {
            INTCONbits.GIE = globalInt;
			/* cppcheck-suppress misra-c2012-15.5 */
            return DIAG_FAIL;
        }
        DIAG_CheckerboardLength -= (((LENGTH_LOC + 2U)) - (uint8_t) DIAG_CheckerboardStartAddress);
        DIAG_CheckerboardStartAddress = (LENGTH_LOC + 2U);
    }

    /**************************************************************************
     * Run the Checkerboard test                                              *
     **************************************************************************/

    WREG_SHAD = 0x55U; //checker
    for (BSR_SHAD = 0U; BSR_SHAD < 2U; BSR_SHAD++) 
    {
		//write checker pattern in RAM
		for (FSR0L_SHAD = 0U; FSR0L_SHAD <(DIAG_CheckerboardLength); FSR0L_SHAD++) 
        {
			(*(uint8_t *)DIAG_CheckerboardStartAddress) = WREG_SHAD;
			DIAG_CheckerboardStartAddress++;
			WREG_SHAD = ((uint8_t)~WREG_SHAD);
        }

        //Restore FSR1L & FSR1H
        DIAG_SRAM_RestoreFSR1x();
        //Ignoring global variable SRAM space 
        if(DIAG_CheckerboardStartAddress < (LENGTH_LOC + 2U))
        {
            DIAG_CheckerboardStartAddress = (LENGTH_LOC + 2U);
        }
        //Read checker pattern in RAM
        for (FSR0L_SHAD = 0U; FSR0L_SHAD < DIAG_CheckerboardLength; FSR0L_SHAD++) 
        {
            if ((*(uint8_t *)DIAG_CheckerboardStartAddress) != WREG_SHAD) 
            {
				if (DIAG_CheckerboardBufferAddress != 0x00U)
				{
					DIAG_CheckerboardTempAddress = (DIAG_CheckerboardBufferAddress - DIAG_CheckerboardLength);
					for (WREG_SHAD = DIAG_CheckerboardLength; WREG_SHAD != 0U; WREG_SHAD--)
					{
						*(uint8_t *)DIAG_CheckerboardStartAddress = *(uint8_t *)DIAG_CheckerboardTempAddress;
						DIAG_CheckerboardStartAddress++;
						DIAG_CheckerboardTempAddress++;
					}
				}
            INTCONbits.GIE = globalInt;
			/* cppcheck-suppress misra-c2012-15.5 */
            return DIAG_FAIL;
            }
			DIAG_CheckerboardStartAddress++;
			WREG_SHAD = ((uint8_t)~WREG_SHAD);
        }
        //invert checker pattern
        WREG_SHAD = ((uint8_t)~WREG_SHAD);
        //Restore FSR1L & FSR1H
        DIAG_SRAM_RestoreFSR1x();
        if (DIAG_CheckerboardStartAddress < (LENGTH_LOC + 2U)) 
        {
            DIAG_CheckerboardStartAddress = (LENGTH_LOC + 2U);
        }
    }

 /* Checkerboard Test Passed */
    /* Restore User data from the buffer. */
    if (DIAG_CheckerboardBufferAddress != 0x00U)
    {
        DIAG_CheckerboardTempAddress = (DIAG_CheckerboardBufferAddress - DIAG_CheckerboardLength);
        for (WREG_SHAD = DIAG_CheckerboardLength; WREG_SHAD != 0U; WREG_SHAD--)
        {
            *(uint8_t*)DIAG_CheckerboardStartAddress = *(uint8_t *)DIAG_CheckerboardTempAddress;
			DIAG_CheckerboardStartAddress++;
			DIAG_CheckerboardTempAddress++;
        }
    }

    INTCONbits.GIE = globalInt;
	return DIAG_PASS;
}

static diag_result_t Checkerboard_Global(void)
{
    //Save essential values to safe locations
	DIAG_SRAM_CheckerboardSave();

    // Write 0x55, 0xAA
    FSR1 = STARTADDR_LOC;
    for (BSR_SHAD = 0U; BSR_SHAD < 4U; BSR_SHAD++) 
    {
		INDF1 = 0x55U;
		FSR1++;
		INDF1 = 0xAAU;
		FSR1++;
    }

    // Read back, check and write inverse checkerboard
    FSR1 = STARTADDR_LOC;
    for (BSR_SHAD = 0U; BSR_SHAD < 4U; BSR_SHAD++) 
    {
        WREG_SHAD = 0x55U;
        if ((INDF1 ^ WREG_SHAD)!= 0U) 
        {
			/* cppcheck-suppress misra-c2012-15.5 */
            return DIAG_FAIL;
        } 
        else 
        {
            INDF1 = ((uint8_t)~WREG_SHAD);
        }
        FSR1++;

        WREG_SHAD = 0xAAU;
        if ((INDF1 ^ WREG_SHAD)!= 0U) 
        {
			/* cppcheck-suppress misra-c2012-15.5 */
            return DIAG_FAIL;
        } 
        else 
        {
            INDF1 = ((uint8_t)~WREG_SHAD);
        }
        FSR1++;
    }

    // Read back, and check
    FSR1 = STARTADDR_LOC;
    for (BSR_SHAD = 0U; BSR_SHAD < 4U; BSR_SHAD++) 
    {
        WREG_SHAD = 0xAAU;
        if ((INDF1 ^ WREG_SHAD)!= 0U)  
        {
			/* cppcheck-suppress misra-c2012-15.5 */
            return DIAG_FAIL;
        } 
        else 
        {
            INDF1 = ((uint8_t)~WREG_SHAD);
        }
        FSR1++;
        WREG_SHAD = 0x55U;
        if ((INDF1 ^ WREG_SHAD)!= 0U)  
        {
			/* cppcheck-suppress misra-c2012-15.5 */
            return DIAG_FAIL;
        }   
        else  
        {
            INDF1 = ((uint8_t)~WREG_SHAD);
        }
        FSR1++;
    }

    //Clear test values as a precaution
    FSR1 = STARTADDR_LOC;
    for (BSR_SHAD = 0U; BSR_SHAD < 10U; BSR_SHAD++) 
    {
        INDF1 = 0x00U;
        FSR1++;
    }
    // Restore values to original locations
	DIAG_SRAM_CheckerboardRestore();

	return DIAG_PASS;
}

static void DIAG_SRAM_CheckerboardStoreInFSR1(void)
{
	__asm("MOVLB 0x00"); 
	__asm("MOVF" ___mkstr(BANKMASK(_DIAG_CheckerboardStartAddress))", W");
	__asm("BANKSEL FSR1L_SHAD"); 
	__asm("MOVWF" ___mkstr(BANKMASK(FSR1L_SHAD)));
	__asm("MOVLB 0x00"); 
	__asm("MOVF 0x21, W");
	__asm("BANKSEL FSR1H_SHAD");
	__asm("MOVWF"  ___mkstr(BANKMASK(FSR1H_SHAD)));	
}
	    
static void DIAG_SRAM_CheckerboardSave(void)
{
	__asm("MOVLB	0x00");	    
	__asm("MOVF" ___mkstr(BANKMASK(_DIAG_CheckerboardStartAddress))", W");
	__asm("MOVWF	FSR1L");
	__asm("MOVF	0x21, W");	    //DIAG_CheckerboardStartAddress MSB
	__asm("MOVWF	FSR1H");
	__asm("MOVF" ___mkstr(BANKMASK(_DIAG_CheckerboardBufferAddress))", W");
	__asm("MOVWF	FSR0H");
	__asm("MOVF" ___mkstr(BANKMASK(_DIAG_CheckerboardLength))", W");
	__asm("BANKSEL FSR0L_SHAD");	    
	__asm("MOVWF"	___mkstr(BANKMASK(FSR0L_SHAD)));  //DIAG_CheckerboardLength
	__asm("MOVF	FSR1L, W");
	__asm("MOVWF"	___mkstr(BANKMASK(FSR1L_SHAD)));  //DIAG_CheckerboardStartAddres LSB
	__asm("MOVF	FSR1H, W");
	__asm("MOVWF"	___mkstr(BANKMASK(FSR1H_SHAD)));  //DIAG_CheckerboardStartAddres MSB
	__asm("MOVF	FSR0H, W");
	__asm("MOVWF"	___mkstr(BANKMASK(FSR0H_SHAD)));  //DIAG_CheckerboardBufferAddress
}	    

static void DIAG_SRAM_CheckerboardRestore(void)
{
	__asm("BANKSEL FSR1L_SHAD");	    
	__asm("MOVF"	___mkstr(BANKMASK(FSR1L_SHAD))", W"); //DIAG_CheckerboardStartAddress
	__asm("MOVWF	FSR1L");
	__asm("MOVF"	___mkstr(BANKMASK(FSR1H_SHAD))", W"); //DIAG_CheckerboardStartAddress
	__asm("MOVWF	FSR1H");
	__asm("MOVF"	___mkstr(BANKMASK(FSR0H_SHAD))", W"); //DIAG_CheckerboardBufferAddress
	__asm("MOVWF	FSR0H");
	__asm("MOVF"	___mkstr(BANKMASK(FSR0L_SHAD))", W");
	__asm("MOVLB	0x00");	    
	__asm("MOVWF	0x26");	    //DIAG_CheckerboardLength
	__asm("MOVF	FSR1L, W");
	__asm("MOVWF" ___mkstr(BANKMASK(_DIAG_CheckerboardStartAddress))); //LSB
	__asm("MOVF	FSR1H, W");
	__asm("MOVWF	0x21");	    //DIAG_CheckerboardStartAddress MSB
	__asm("MOVF	FSR0H, W");
	__asm("MOVWF	0x22");	    //DIAG_CheckerboardBufferAddress
}    

static void DIAG_SRAM_RestoreFSR1x(void)
{
	__asm("BANKSEL FSR1L_SHAD");
	__asm("MOVF" ___mkstr(BANKMASK(FSR1L_SHAD))", W");
	__asm("MOVLB 0x00");
	__asm("MOVWF" ___mkstr(BANKMASK(_DIAG_CheckerboardStartAddress))); //LSB
	__asm("BANKSEL FSR1H_SHAD");
	__asm("MOVF" ___mkstr(BANKMASK(FSR1H_SHAD))", W");
	__asm("MOVLB 0x00");
	__asm("MOVWF 0x21"); //DIAG_CheckerboardStartAddress MSB 
}
