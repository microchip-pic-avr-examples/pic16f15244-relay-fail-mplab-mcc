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
 * @file    diag_int_frequency.h
 * @defgroup diag_int_frequency Interrupt Frequency Test
 * @brief   This file contains API prototypes and other data types for the Interrupt Frequency diagnostic test
 * 
 * @note 
 * Microchip Technology Inc. has followed development methods required by 
 * Functional Safety Standards and performed extensive validation and static
 * testing to ensure that the code operates as intended. Any modification to the 
 * code can invalidate the results of Microchip's validation and testing.
 *
 */

#ifndef DIAG_INT_FREQUENCY_H
/* cppcheck-suppress misra-c2012-2.5 */
#define DIAG_INT_FREQUENCY_H

#include <stdbool.h>
#include <stdint.h>
#include "../../diag_common/config/diag_config.h"
#include "../../diag_common/diag_result_type.h"

/**
 * @ingroup diag_int_frequency
 * @brief Registers an interrupt for diagnosis.
 * When any interrupt is registered, the @ref diag_int_params_t parameters are initialized with user-defined
 * values and are later updated or monitored by the @ref DIAG_INT_Frequency function.
 * @param [in] intId - Unique ID for the interrupt to be monitored
 * @param [in] refCount - Number of interrupts expected to occur within a given monitoring period
 * @param [in] tolerance - Tolerance band for the number of interrupts which occur in the defined Fault Detection Time Interval (FDTI)
 * @param [in] fdtiCount - Count to derive the FDTI for the interrupt registered for monitoring.
 *                    FDTI is the time interval between the occurrence of a Fault and its detection.
 *                    This count gets decremented every time the reference ISR is called.
 *                    When this counter reaches 1, it is time to check the intr_counter with the
 *                    expected intr_ref_counter for the interrupt under test with the given intr_counter_tolerance.
 * @return @ref DIAG_PASS - Given interrupt source is registered successfully \n
 * @return @ref DIAG_FAIL - Given interrupt source is not registered \n
 * @return @ref DIAG_INVALID_ARG - Invalid input arguments when calling the function \n
 **/
diag_result_t DIAG_INT_FrequencyRegister(diag_int_id_t intId, uint16_t refCount, uint16_t tolerance, uint16_t fdtiCount);

/**
 * @ingroup diag_int_frequency
 * @brief  Unregisters an interrupt from the frequency diagnosis.
 * @param [in] intId - ID for the interrupt to be unregistered
 * @return @ref DIAG_PASS - Given interrupt source is unregistred successfully \n
 * @return @ref DIAG_FAIL - Given interrupt source is not unregistred \n
 * @return @ref DIAG_INVALID_ARG - Invalid input arguments when calling the function \n
 **/
diag_result_t DIAG_INT_FrequencyUnregister(diag_int_id_t intId);

/**
 * @ingroup diag_int_frequency
 * @brief Configures the callback function for the reference RTC periodic interrupt.
 * @param None.
 * @return None.
 **/
void DIAG_INT_FrequencyInitialize(void);

/**
 * @ingroup diag_int_frequency
 * @brief Increments the intr_counter for the registered interrupt.
 * @param [in] intId - ID for the interrupt being diagnosed
 * @return None.
 * @note Call this function in the Interrupt Service Routine (ISR) of the respective interrupt.
 **/
void DIAG_INT_FrequencyCountInterrupt(diag_int_id_t intId);

/**
 * @ingroup diag_int_frequency
 * @brief Returns the status of the interrupt diagnosis.
 * @param [in] intId - ID for the interrupt being diagnosed
 * @return @ref True - Diagnosis is finished and the result is ready
 * @return @ref False - Diagnosis is ongoing
 **/
bool DIAG_INT_FrequencyIsResultReady(diag_int_id_t intId);

/**
 * @ingroup diag_int_frequency
 * @brief Returns the result frequency diagnosis for the required interrupt.
 *        The result is set to DIAG_UNDEFINED after returning PASS or FAIL to ensure 
 *        that the latest result from the diagnostic is returned.
 * @pre Call the @ref DIAG_INT_FrequencyIsResultReady function to determine if the diagnosis result is ready.
 * @param [in] intId - ID of the interrupt for which test status is to be obtained
 * @return @ref DIAG_PASS - Success \n
 * @return @ref DIAG_FAIL - Failure \n
 * @return @ref DIAG_INVALID_ARG - Invalid input arguments when calling the function \n
 * @return @ref DIAG_UNDEFINED - Result not ready \n
 **/
diag_result_t DIAG_INT_FrequencyGetResult(diag_int_id_t intId);

#endif //DIAG_INT_FREQUENCY_H