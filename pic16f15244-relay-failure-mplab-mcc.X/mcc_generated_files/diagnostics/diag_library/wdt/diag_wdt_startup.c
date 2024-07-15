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
 *  @file  diag_wdt_startup.c
 *  @brief This file contains APIs to test WDT in Simple Timer Mode using Independent Time-Slot Monitoring method.
 *
 * @note 
 * Microchip Technology Inc. has followed development methods required by 
 * Functional Safety Standards and performed extensive validation and static
 * testing to ensure that the code operates as intended. Any modification to the 
 * code can invalidate the results of Microchip's validation and testing.
 */
#include "diag_wdt_startup.h"
#include "../../diag_common/config/diag_config.h"

/**
 @ingroup diag_wdt_startup
 @def F_CPU
 Defines the system clock.
 */
#ifndef F_CPU
#define F_CPU (1000000U)
#endif

#if defined(WDTCON)
#define WDTCONREG    WDTCONbits
#else
#define WDTCONREG    WDTCON0bits
#endif

/**
 @ingroup diag_wdt_startup
 @def F_TIMER
 Defines the TMR0 clock.
 */
#define F_TIMER (F_CPU/4U)

/**
 @ingroup diag_wdt_startup
 @enum diag_wdt_fsm_state_t
 @brief Contains the valid test states for the WDT test.
 @var diag_wdt_fsm_state_t:: DIAG_WDT_FSM_FAULT
 0 - Faults detected
 @var diag_wdt_fsm_state_t:: DIAG_WDT_FSM_TEST
 1 - Performs the WDT Test
 @var diag_wdt_fsm_state_t:: DIAG_WDT_FSM_OK
 2 - WDT Test state on error-free execution of @ref DIAG_WDT_FSM_TEST
 */
typedef enum
{
    DIAG_WDT_FSM_FAULT = 0,
    DIAG_WDT_FSM_TEST = 1,
    DIAG_WDT_FSM_OK = 2
} diag_wdt_fsm_state_t;

static volatile __persistent diag_result_t testResult;
volatile __persistent diag_wdt_fsm_state_t diag_wdt_teststate;

// This variable stores the reload value for timer0 period
static volatile uint16_t timer0ReloadVal16bit;

diag_result_t DIAG_WDT_GetResult(void)
{
    return testResult;
}

/**
 @ingroup diag_wdt_startup
 @def TMR0_OVERFLOW_TIME
 TMR0 overflow time in seconds.
 */
#define TMR0_OVERFLOW_TIME  (0.001)

/**
 @ingroup diag_wdt_startup
 @def PERIOD_VALUE
 Makes TMR0 achieve the overflow time.
 */
#define PERIOD_VALUE    (uint16_t)(0xFFFFU-(TMR0_OVERFLOW_TIME * F_TIMER))

/**
 @ingroup diag_wdt_startup
 @def TC_MAX_COUNT_BEFORE_ERROR
 The maximum overflow value of TMR0 before WDT is assumed not to be able to Reset the device is approximately 11 ms, 
 considering the frequency tolerance. 
 WDT time-out is 8 ms + ~35% frequency tolerance of the clock. 
 TC_MAC_COUNT_BEFORE_ERROR is 8 ms + 2.8 ms = 10.8 ms (~11 ms).
 */
#define TC_MAX_COUNT_BEFORE_ERROR   (11U)

/**
 @ingroup diag_wdt_startup
 @def TC_MIN_COUNT_BEFORE_ERROR
 The minimum overflow value of the TMR0 before WDT is allowed to Reset the device is approximately 5 ms, 
 considering the frequency tolerance. 
 WDT time-out is 8 ms - 35% frequency tolerance of the clock. 
 TC_MIC_COUNT_BEFORE_ERROR is 8 ms - 35% tolerance = 8 ms / 2.8 ms = 5.2 ms (5 ms).
 */
#define TC_MIN_COUNT_BEFORE_ERROR   (5U)

/**
 @ingroup diag_wdt_startup
 @def WDTPS_SEL_TEST
 Defines the period value as 8ms for the WDT module in the WDTCON register.
 */
#define WDTPS_SEL_TEST  (3U)

/**
 @ingroup diag_wdt_startup
 @brief Sets the period so that Timer0 (TMR0) overflows approximately every 1 ms.
 */
static void configure_TMR0_for_time_taking(void)
{
    // T0CS FOSC/4; T0CKPS 1:1; T0ASYNC synchronised;
    T0CON1 = 0x40U;

    // Write to the Timer0 register
    TMR0H = (PERIOD_VALUE >> 8U);
    TMR0L = (PERIOD_VALUE & 0xFFU);

    // Load TMR0 value to the 16-bit reload variable
    timer0ReloadVal16bit = ((uint16_t) TMR0H << 8U) | (uint16_t) TMR0L;

    // Clearing IF flag
    PIR0bits.TMR0IF = 0U;

    // T0OUTPS 1:1; T0EN enabled; T016BIT 16-bit;
    T0CON0 = 0x90U;
}

/**
 @ingroup diag_wdt_startup
 @brief Resets TMR0 to zero and starts counting from zero.
 */
static void restart_TMR0(void)
{
    // Write to the Timer0 register
    TMR0H = timer0ReloadVal16bit >> 8U;
    TMR0L = (uint8_t) timer0ReloadVal16bit;

    // Start the Timer by writing to TMR0ON bit
    T0CON0bits.T0EN = 1U;
}

/**
 @ingroup diag_wdt_startup
 @brief Creates a fixed delay for the WDT test.
 */
static void watchdog_delay(uint8_t i)
{
    restart_TMR0();
    uint8_t temp = i;
    while (temp != 0U)
    {
        while (!(PIR0bits.TMR0IF))
        {
            //do nothing
        }
        PIR0bits.TMR0IF = 0U;
        TMR0H = timer0ReloadVal16bit >> 8U;
        TMR0L = (uint8_t) timer0ReloadVal16bit;
        temp--;
    }
}

/**
 @ingroup diag_wdt_startup
 @def diag_restore_timer_defaults
 This is an API to restore the timer registers to their default values.
 */
static void diag_restore_timer_defaults(void)
{
    T0CON0 = 0x00U;
    T0CON1 = 0x00U;
    TMR0H = 0x00U;
    TMR0L = 0x00U;
    timer0ReloadVal16bit = 0x00U;
    PIR0bits.TMR0IF = 0U;
}

/**
 @ingroup diag_wdt_startup
 @brief Tests the WDT, protects against runaway code, deadlocks and the program counter not working.
 @par This API will not be called from main. The test will be executed before main is entered as long as diag_wdt_startup.h/.c and powerup_xx.as files are included in the compilation.
 */
void DIAG_WDT_Startup(void)
{

    // This variable is used to count TMR0 periods
    static volatile uint8_t diag_tc_count;

    // This variable is used as an increment and decrement counter for the TMR0 count
    static uint8_t counter;

    configure_TMR0_for_time_taking();

    /* Check Reset cause.
     * If a power-on, or external Reset has occurred the first part of the test
     * is executed. This checks that WDT issues a system Reset on timeout.*/
    if ((PCON0bits.nPOR == 0U) || (PCON0bits.nRMCLR == 0U))
    {
        //Clear the Reset flags
        PCON0bits.nPOR = 1U;
        PCON0bits.nRMCLR = 1U;

        // Set the next state of the test
        diag_wdt_teststate = DIAG_WDT_FSM_TEST;

        /* Configure the TMR0, which is used as an independent time source.
         * In this section we are going to measure the number of TMR0 periods
         * before the WDT timeout issues a system Reset. */

        // WDT Configuration :
        WDTCONREG.WDTPS = WDTPS_SEL_TEST;

        // Enable the WDT by setting the SEN bit
        WDTCONREG.SWDTEN = 1U;

        /* Count number of TMR0 periods until a WDT timeout.
         There is a configurable maximum number of TMR0 periods before
         an error in the WDT is assumed. */
        diag_tc_count = 0U;
        counter = TC_MAX_COUNT_BEFORE_ERROR;

        restart_TMR0();
        while (counter != 0U)
        {
            diag_tc_count++;
            while (!(PIR0bits.TMR0IF))
			{
				//Do nothing
			}
            PIR0bits.TMR0IF = 0U;
            TMR0H = timer0ReloadVal16bit >> 8U;
            TMR0L = (uint8_t) timer0ReloadVal16bit;
            counter--;
        }

        /* This will only be executed if there is an error in the WDT,
         * i.e. if the WDT did not timeout before the maximum number of
         * TMR0 periods was exceeded.*/
        diag_wdt_teststate = DIAG_WDT_FSM_FAULT;
    }
    // If a WDT Reset has occurred, this is not the first iteration.
    else if (PCON0bits.nRWDT == 0U)
    {
        // Clear the Reset cause register.
        PCON0bits.nRWDT = 1U;

        // Handle the different test stages.
        switch (diag_wdt_teststate)
        {
        case DIAG_WDT_FSM_TEST:
        {
            /* Tests WDT timing and that WDT can be Reset
             * Assume WDT Fault for now.*/
            diag_wdt_teststate = DIAG_WDT_FSM_FAULT;
            // Make sure that the estimated WDT period is equal or lower than measured WDT period
            if (diag_tc_count >= TC_MIN_COUNT_BEFORE_ERROR)
            {
                /* WDT Configuration
                 * Enable WDT with configurable period.*/
                WDTCONREG.WDTPS = WDTPS_SEL_TEST;
                // Enable the WDT by setting the SEN bit
                WDTCONREG.SWDTEN = 1U;

                /* Wait approximately 0.75 * T_WDT, where T_WDT is the estimated
                 * WDT period, before a WDT Reset. This checks that the WDT does
                 * not expire earlier than expected.*/
                counter = (diag_tc_count * 3U);
                counter = counter >> 2U;
                watchdog_delay(counter);
                CLRWDT();


                // Wait approximately 0.75 * T_WDT
                counter = (diag_tc_count * 3U);
                counter = counter >> 2U;
                watchdog_delay(counter);

                /* Exiting the above while loop should only occur if WDT Reset worked, otherwise there would
                 * have been a system Reset before this point
                 * (this would be executed approximately 1.5*T_WDT after the WDT was initially set).
                 * Note that in that case the state would still be |DIAG_WDT_FSM_FAULT|.
                 * Set next state and wait for the WDT to issue a system reset.
                 * That should approximately happen in 0.25*T_WDT */

                // Set next test state
                diag_wdt_teststate = DIAG_WDT_FSM_OK;

                // Wait approx 0.5*T_WDT
                counter = (diag_tc_count >> 1U);
                watchdog_delay(counter);

            }

            // Set error flag if WDT has not issued a Reset.
            diag_wdt_teststate = DIAG_WDT_FSM_FAULT;
            break;
        }

        case DIAG_WDT_FSM_OK:
        {
            // Configurable actions
            testResult = DIAG_PASS;
            break;
        }
            //Otherwise assume error
        default:
        {
            diag_wdt_teststate = DIAG_WDT_FSM_FAULT;
            break;
        }
        }
    }
    else
    {
        /* Once the Simple WDT gets diagnosed, this diag_restore_timer_defaults() is responsible to
         restore the timer registers back to their default values.*/
        diag_restore_timer_defaults();
    }

    /* Actions to take if there is an error.
     * The test would be on fault state because:
     * - WDT could not be reset
     * - WDT did not issue a system reset on either timeout or untimely reset*/
    if (diag_wdt_teststate == DIAG_WDT_FSM_FAULT)
    {
        testResult = DIAG_FAIL;
    }
}