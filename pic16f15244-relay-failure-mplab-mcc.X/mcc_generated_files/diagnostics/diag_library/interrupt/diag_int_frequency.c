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
 *  @file    diag_int_frequency.c
 *  @brief   This file contains APIs for the Interrupt Frequency test.
 *  @ingroup diag_int_frequency
 *
 * @note 
 * Microchip Technology Inc. has followed development methods required by 
 * Functional Safety Standards and performed extensive validation and static
 * testing to ensure that the code operates as intended. Any modification to the 
 * code can invalidate the results of Microchip's validation and testing.
 */

#include "diag_int_frequency.h"

/**
 @enum diag_int_mon_status_t
 @brief Contains the constants to indicate if the interrupt is being monitored or ignored 
 from monitoring. When failure is detected for any registered interrupt, the diagnostic
 test will stop monitoring that interrupt by assigning @ref DIAG_INT_OFF to int_mon_status
 @var diag_int_mon_status_t:: DIAG_INT_ON
 0 -  Interrupt is registered and monitored \n
 @var diag_int_mon_status_t:: DIAG_INT_OFF
 2 -  Interrupt is registered but not monitored \n
 */
typedef enum
{
    DIAG_INT_ON,
    DIAG_INT_OFF,
} diag_int_mon_status_t;

/**
 @struct diag_interrupt_params_t
 @brief Holds the initial and run-time values of all the parameters required for
 interrupt frequency diagnosis performed by the @ref DIAG_INT_Frequency function.
 Initially, for each interrupt that is registered and monitored for the frequency test,
 the structure parameters are initialized with user-defined values received in the @ref DIAG_INT_FrequencyRegister API.
 Later, when the test is running, parameters are continuously updated to hold the actual values to be used
 to evaluate the test.

 @var diag_int_params_t::int_id
 Unique interrupt identifier for every interrupt source as listed in diag_config.h.

 @var diag_int_params_t::int_counter
 Counter that gets incremented in the ISR of the respective interrupt,
 and holds the actual number of interrupts which occured in the current monitoring period defined by
 the reference RTC interrupt.

 @var diag_int_params_t::int_ref_counter
 Holds the number of interrupts expected to occur within a given monitoring period.

 @var diag_int_params_t::int_counter_tolerance
 Tolerance band for the number of interrupts occuring in the defined FDTI
 and for the @ref int_ref_counter user-defined tolerance.

 @var diag_int_params_t::int_fdti_counter
 FDTI for the interrupt registered for monitoring.
 FDTI is the time interval between the occurrence of a Fault and its detection.
 This count gets decremented every time the reference ISR is called.
 When this counter reaches zero, it is time to check the @ref int_counter with the expected @ref int_ref_counter
 for the interrupt under test with the given @ref int_counter_tolerance.

 @var diag_int_params_t::int_test_status
 Indicates the status of interrupt monitoring.

 @var diag_int_params_t::int_mon_status
 Indicates if an interrupt is being monitored or ignored from monitoring.
 When failure is detected for any registered interrupt, the diagnostic test will stop monitoring
 that interrupt by setting this variable to @ref DIAG_INT_OFF.
 **/
typedef struct
{
    diag_int_id_t int_id;
    uint16_t int_counter;
    uint16_t int_ref_counter;
    uint16_t int_counter_tolerance;
    uint16_t int_fdti_counter;
    uint16_t int_fdti;
    diag_result_t int_test_status;
    diag_int_mon_status_t int_mon_status;
} diag_int_params_t;

/* This is an array of structure to hold the parameters of interrupts
 * which are registered for monitoring.
 */
static diag_int_params_t monitored_interrupts[MAX_INT_LIMIT];

/* Calculates the difference between actual and reference
 * interrupt count.
 */
static uint16_t abs_diff(uint16_t a, uint16_t b)
{
    uint16_t retDiff;

    if (a > b)
    {
        retDiff = a - b;
    }
    else
    {
        retDiff = b - a;
    }

    return retDiff;
}

/* Checks the frequency of registered interrupts
 * This function needs to be called on every RTC interrupt
 */

/* This API is called in RTC interrupt service routine. This API monitors
 * the frequency of registered interrupts by comparing the respective counter with
 * reference considering the given tolerance. Interrupts are monitored only if
 * those are registered and enabled for monitoring.
 */
static void DIAG_INT_Frequency(void)
{
    uint8_t nInt;

    for (nInt = 1U; nInt < (uint8_t) MAX_INT_LIMIT; nInt++)
    {
        //Skip the iteration if interrupt ID is invalid
        if (monitored_interrupts[nInt].int_id == (diag_int_id_t) 0)
        {
            continue;
        }

        //Skip the diagnosis if monitoring status is not ON
        if (DIAG_INT_ON != monitored_interrupts[nInt].int_mon_status)
        {
            monitored_interrupts[nInt].int_test_status = DIAG_UNDEFINED;
            continue;
        }

        //Wait for fault detection time interval
        if (1U != monitored_interrupts[nInt].int_fdti_counter)
        {
            monitored_interrupts[nInt].int_fdti_counter--;
            continue;
        }

        //Proceed with diagnosis
        monitored_interrupts[nInt].int_fdti_counter = monitored_interrupts[nInt].int_fdti;

        uint16_t counterDiff = abs_diff(monitored_interrupts[nInt].int_counter,
                                        monitored_interrupts[nInt].int_ref_counter);

        if (counterDiff > monitored_interrupts[nInt].int_counter_tolerance)
        {
            monitored_interrupts[nInt].int_counter = 0;
            monitored_interrupts[nInt].int_test_status = DIAG_FAIL;
            continue;
        }

        monitored_interrupts[nInt].int_counter = 0;
        monitored_interrupts[nInt].int_test_status = DIAG_PASS;
    }
}

/* Registers an interrupt for monitoring and should be called from application */
diag_result_t DIAG_INT_FrequencyRegister(diag_int_id_t intId, uint16_t refCount, uint16_t tolerance, uint16_t fdtiCount)
{
    diag_result_t status;

    if ((intId == INVALID_INT_ID) || (intId >= MAX_INT_LIMIT))
    {
        status = DIAG_INVALID_ARG;
    }
    else
    {
        monitored_interrupts[intId].int_id = intId;
        monitored_interrupts[intId].int_counter = 0;
        monitored_interrupts[intId].int_ref_counter = refCount;
        monitored_interrupts[intId].int_counter_tolerance = tolerance;
        monitored_interrupts[intId].int_fdti_counter = fdtiCount;
        monitored_interrupts[intId].int_fdti = fdtiCount;
        monitored_interrupts[intId].int_test_status = DIAG_UNDEFINED;
        monitored_interrupts[intId].int_mon_status = DIAG_INT_ON;

        status = DIAG_PASS;
    }

    return status;
}

/* Unregisters an interrupt for monitoring and should be called from application */
diag_result_t DIAG_INT_FrequencyUnregister(diag_int_id_t intId)
{
    diag_result_t status;

    if ((intId == INVALID_INT_ID) || (intId >= MAX_INT_LIMIT))
    {
        status = DIAG_INVALID_ARG;
    }
    else
    {
        monitored_interrupts[intId].int_id = intId;
        monitored_interrupts[intId].int_counter = 0;
        monitored_interrupts[intId].int_ref_counter = 0;
        monitored_interrupts[intId].int_counter_tolerance = 0;
        monitored_interrupts[intId].int_fdti_counter = 0;
        monitored_interrupts[intId].int_test_status = DIAG_UNDEFINED;
        monitored_interrupts[intId].int_mon_status = DIAG_INT_OFF;

        status = DIAG_PASS;
    }

    return status;
}

/* Updates the interrupt counter and should be called from respective ISR */
void DIAG_INT_FrequencyCountInterrupt(diag_int_id_t intId)
{
    monitored_interrupts[intId].int_counter++;
}

/* Initializes RTC for interrupt monitoring */
void DIAG_INT_FrequencyInitialize(void)
{
    Timer0_OverflowCallbackRegister(DIAG_INT_Frequency);
    Timer0_Start();
}

bool DIAG_INT_FrequencyIsResultReady(diag_int_id_t intId)
{
    return ((monitored_interrupts[intId].int_test_status != DIAG_UNDEFINED) ? true : false);
}

/* Returns the status of interrupt test for given ID */
diag_result_t DIAG_INT_FrequencyGetResult(diag_int_id_t intId)
{
    diag_result_t status;

    if ((intId == INVALID_INT_ID) || (intId >= MAX_INT_LIMIT))
    {
        status = DIAG_INVALID_ARG;
    }
    else
    {
        status = monitored_interrupts[intId].int_test_status;

        monitored_interrupts[intId].int_test_status = DIAG_UNDEFINED;
    }

    return status;
}
