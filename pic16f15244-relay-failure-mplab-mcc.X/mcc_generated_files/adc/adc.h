/**
 * ADC Generated Driver API Header File
 *
 * @file adc.h
 *  
 * @defgroup adc ADC
 *
 * @brief This file contains the API prototypes and data types for the ADC driver.
 *
 * @version ADC Driver Version 2.1.3
 */
/*
© [2024] Microchip Technology Inc. and its subsidiaries.

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

#ifndef ADC_H
#define ADC_H

#include <xc.h>
#include <stdint.h>
#include <stdbool.h>

/**
 @ingroup adc
 @typedef adc_result_t
 @brief Used for the result of the Analog-to-Digital (A/D) conversion.
 */

typedef uint16_t adc_result_t;

/**
 * @ingroup adc
 * @typedef adc_sync_double_result_t
 * @struct Used for the result for a Double ADC conversion value.
 */
typedef struct
{
    adc_result_t adcResult1;
    adc_result_t adcResult2;
} adc_sync_double_result_t;


/**
 * @ingroup adc
 * @enum adc_channel_t
 * @brief Contains the available ADC channels.
 */
typedef enum
{
    channel_AVSS =  0x24,
    channel_FVR_BUF1 =  0x1e,
    SENSE =  0x5
} adc_channel_t;

/**
  Section: ADC Module APIs
 */

/**
 * @ingroup adc
 * @brief Initializes the ADC module. This routine is called before any other ADC routine.
 * @param None.
 * @return None.
 */
void ADC_Initialize(void);

/**
 * @ingroup adc
 * @brief Selects the channel for the A/D conversion.
 * @param channel - Analog channel number on which the A/D conversion will be applied.
 *                  Refer to adc_channel_t  for the available channels.
 * @return None.
 */
void ADC_SelectChannel(adc_channel_t channel);

/**
 * @ingroup adc
 * @brief Starts A/D conversion.
 * @param None.
 * @return None.
 */
void ADC_StartConversion(void);
 
/**
 * @ingroup adc
 * @brief Checks if ongoing A/D conversion is complete.
 * @param None.
 * @retval True - A/D conversion is complete.
 * @retval False - A/D conversion is ongoing.
 */
bool ADC_IsConversionDone(void);
 
/**
 * @ingroup adc
 * @brief Retrieves the result of the latest A/D conversion.
 * @param None.
 * @return The result of A/D conversion. Refer to the adc_result_t.
 */
adc_result_t ADC_GetConversionResult(void);

/**
 * @ingroup adc
 * @brief Retrieves the result of a single A/D conversion on any given channel.
 * @param channel - Analog channel number on which the A/D conversion will be applied.
 *                  Refer to adc_channel_t  for the available channels.
* @return The result of A/D conversion. Refer to the adc_result_t.
 */
adc_result_t ADC_GetConversion(adc_channel_t channel);
 
/**
 * @ingroup adc
 * @brief Adds the acquisition delay for the temperature sensor.
 * @pre This function is called when temperature sensor is used.
 * @param None.
 * @return None.
 */
void ADC_TemperatureAcquisitionDelay(void);

/**
 * @ingroup adc
 * @brief Implements the ADC Interrupt(ADI) service routine for the interrupt-driven implementations.
 * @param None.
 * @return None.
 */
void ADC_ISR(void);

/**
 * @ingroup adc
 * @brief Sets the callback for the ADC Interrupt(ADI).
 * @param InterruptHandler - Callback function to be called on the interrupt event.
 * @return None.
 */
void ADC_SetInterruptHandler(void (* InterruptHandler)(void));
#endif	//ADC_H
