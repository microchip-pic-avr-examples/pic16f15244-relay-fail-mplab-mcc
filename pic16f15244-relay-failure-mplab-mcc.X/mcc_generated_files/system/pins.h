/**
 * Generated Pins header File
 * 
 * @file pins.h
 * 
 * @defgroup  pinsdriver Pins Driver
 * 
 * @brief This is generated driver header for pins. 
 *        This header file provides APIs for all pins selected in the GUI.
 *
 * @version Driver Version  3.0.0
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

#ifndef PINS_H
#define PINS_H

#include <xc.h>

#define INPUT   1
#define OUTPUT  0

#define HIGH    1
#define LOW     0

#define ANALOG      1
#define DIGITAL     0

#define PULL_UP_ENABLED      1
#define PULL_UP_DISABLED     0

// get/set IO_RA2 aliases
#define RELAY_DRIVE_TRIS                 TRISAbits.TRISA2
#define RELAY_DRIVE_LAT                  LATAbits.LATA2
#define RELAY_DRIVE_PORT                 PORTAbits.RA2
#define RELAY_DRIVE_WPU                  WPUAbits.WPUA2
#define RELAY_DRIVE_OD                   ODCONAbits.ODCA2
#define RELAY_DRIVE_ANS                  ANSELAbits.ANSA2
#define RELAY_DRIVE_SetHigh()            do { LATAbits.LATA2 = 1; } while(0)
#define RELAY_DRIVE_SetLow()             do { LATAbits.LATA2 = 0; } while(0)
#define RELAY_DRIVE_Toggle()             do { LATAbits.LATA2 = ~LATAbits.LATA2; } while(0)
#define RELAY_DRIVE_GetValue()           PORTAbits.RA2
#define RELAY_DRIVE_SetDigitalInput()    do { TRISAbits.TRISA2 = 1; } while(0)
#define RELAY_DRIVE_SetDigitalOutput()   do { TRISAbits.TRISA2 = 0; } while(0)
#define RELAY_DRIVE_SetPullup()          do { WPUAbits.WPUA2 = 1; } while(0)
#define RELAY_DRIVE_ResetPullup()        do { WPUAbits.WPUA2 = 0; } while(0)
#define RELAY_DRIVE_SetPushPull()        do { ODCONAbits.ODCA2 = 0; } while(0)
#define RELAY_DRIVE_SetOpenDrain()       do { ODCONAbits.ODCA2 = 1; } while(0)
#define RELAY_DRIVE_SetAnalogMode()      do { ANSELAbits.ANSA2 = 1; } while(0)
#define RELAY_DRIVE_SetDigitalMode()     do { ANSELAbits.ANSA2 = 0; } while(0)
// get/set IO_RA5 aliases
#define SENSE_TRIS                 TRISAbits.TRISA5
#define SENSE_LAT                  LATAbits.LATA5
#define SENSE_PORT                 PORTAbits.RA5
#define SENSE_WPU                  WPUAbits.WPUA5
#define SENSE_OD                   ODCONAbits.ODCA5
#define SENSE_ANS                  ANSELAbits.ANSA5
#define SENSE_SetHigh()            do { LATAbits.LATA5 = 1; } while(0)
#define SENSE_SetLow()             do { LATAbits.LATA5 = 0; } while(0)
#define SENSE_Toggle()             do { LATAbits.LATA5 = ~LATAbits.LATA5; } while(0)
#define SENSE_GetValue()           PORTAbits.RA5
#define SENSE_SetDigitalInput()    do { TRISAbits.TRISA5 = 1; } while(0)
#define SENSE_SetDigitalOutput()   do { TRISAbits.TRISA5 = 0; } while(0)
#define SENSE_SetPullup()          do { WPUAbits.WPUA5 = 1; } while(0)
#define SENSE_ResetPullup()        do { WPUAbits.WPUA5 = 0; } while(0)
#define SENSE_SetPushPull()        do { ODCONAbits.ODCA5 = 0; } while(0)
#define SENSE_SetOpenDrain()       do { ODCONAbits.ODCA5 = 1; } while(0)
#define SENSE_SetAnalogMode()      do { ANSELAbits.ANSA5 = 1; } while(0)
#define SENSE_SetDigitalMode()     do { ANSELAbits.ANSA5 = 0; } while(0)
// get/set IO_RB5 aliases
#define RELAY_STATE_LED_TRIS                 TRISBbits.TRISB5
#define RELAY_STATE_LED_LAT                  LATBbits.LATB5
#define RELAY_STATE_LED_PORT                 PORTBbits.RB5
#define RELAY_STATE_LED_WPU                  WPUBbits.WPUB5
#define RELAY_STATE_LED_OD                   ODCONBbits.ODCB5
#define RELAY_STATE_LED_ANS                  ANSELBbits.ANSB5
#define RELAY_STATE_LED_SetHigh()            do { LATBbits.LATB5 = 1; } while(0)
#define RELAY_STATE_LED_SetLow()             do { LATBbits.LATB5 = 0; } while(0)
#define RELAY_STATE_LED_Toggle()             do { LATBbits.LATB5 = ~LATBbits.LATB5; } while(0)
#define RELAY_STATE_LED_GetValue()           PORTBbits.RB5
#define RELAY_STATE_LED_SetDigitalInput()    do { TRISBbits.TRISB5 = 1; } while(0)
#define RELAY_STATE_LED_SetDigitalOutput()   do { TRISBbits.TRISB5 = 0; } while(0)
#define RELAY_STATE_LED_SetPullup()          do { WPUBbits.WPUB5 = 1; } while(0)
#define RELAY_STATE_LED_ResetPullup()        do { WPUBbits.WPUB5 = 0; } while(0)
#define RELAY_STATE_LED_SetPushPull()        do { ODCONBbits.ODCB5 = 0; } while(0)
#define RELAY_STATE_LED_SetOpenDrain()       do { ODCONBbits.ODCB5 = 1; } while(0)
#define RELAY_STATE_LED_SetAnalogMode()      do { ANSELBbits.ANSB5 = 1; } while(0)
#define RELAY_STATE_LED_SetDigitalMode()     do { ANSELBbits.ANSB5 = 0; } while(0)
// get/set IO_RB6 aliases
#define ERROR_LED_TRIS                 TRISBbits.TRISB6
#define ERROR_LED_LAT                  LATBbits.LATB6
#define ERROR_LED_PORT                 PORTBbits.RB6
#define ERROR_LED_WPU                  WPUBbits.WPUB6
#define ERROR_LED_OD                   ODCONBbits.ODCB6
#define ERROR_LED_ANS                  ANSELBbits.ANSB6
#define ERROR_LED_SetHigh()            do { LATBbits.LATB6 = 1; } while(0)
#define ERROR_LED_SetLow()             do { LATBbits.LATB6 = 0; } while(0)
#define ERROR_LED_Toggle()             do { LATBbits.LATB6 = ~LATBbits.LATB6; } while(0)
#define ERROR_LED_GetValue()           PORTBbits.RB6
#define ERROR_LED_SetDigitalInput()    do { TRISBbits.TRISB6 = 1; } while(0)
#define ERROR_LED_SetDigitalOutput()   do { TRISBbits.TRISB6 = 0; } while(0)
#define ERROR_LED_SetPullup()          do { WPUBbits.WPUB6 = 1; } while(0)
#define ERROR_LED_ResetPullup()        do { WPUBbits.WPUB6 = 0; } while(0)
#define ERROR_LED_SetPushPull()        do { ODCONBbits.ODCB6 = 0; } while(0)
#define ERROR_LED_SetOpenDrain()       do { ODCONBbits.ODCB6 = 1; } while(0)
#define ERROR_LED_SetAnalogMode()      do { ANSELBbits.ANSB6 = 1; } while(0)
#define ERROR_LED_SetDigitalMode()     do { ANSELBbits.ANSB6 = 0; } while(0)
// get/set IO_RC0 aliases
#define IO_RC0_TRIS                 TRISCbits.TRISC0
#define IO_RC0_LAT                  LATCbits.LATC0
#define IO_RC0_PORT                 PORTCbits.RC0
#define IO_RC0_WPU                  WPUCbits.WPUC0
#define IO_RC0_OD                   ODCONCbits.ODCC0
#define IO_RC0_ANS                  ANSELCbits.ANSC0
#define IO_RC0_SetHigh()            do { LATCbits.LATC0 = 1; } while(0)
#define IO_RC0_SetLow()             do { LATCbits.LATC0 = 0; } while(0)
#define IO_RC0_Toggle()             do { LATCbits.LATC0 = ~LATCbits.LATC0; } while(0)
#define IO_RC0_GetValue()           PORTCbits.RC0
#define IO_RC0_SetDigitalInput()    do { TRISCbits.TRISC0 = 1; } while(0)
#define IO_RC0_SetDigitalOutput()   do { TRISCbits.TRISC0 = 0; } while(0)
#define IO_RC0_SetPullup()          do { WPUCbits.WPUC0 = 1; } while(0)
#define IO_RC0_ResetPullup()        do { WPUCbits.WPUC0 = 0; } while(0)
#define IO_RC0_SetPushPull()        do { ODCONCbits.ODCC0 = 0; } while(0)
#define IO_RC0_SetOpenDrain()       do { ODCONCbits.ODCC0 = 1; } while(0)
#define IO_RC0_SetAnalogMode()      do { ANSELCbits.ANSC0 = 1; } while(0)
#define IO_RC0_SetDigitalMode()     do { ANSELCbits.ANSC0 = 0; } while(0)
// get/set IO_RC1 aliases
#define IO_RC1_TRIS                 TRISCbits.TRISC1
#define IO_RC1_LAT                  LATCbits.LATC1
#define IO_RC1_PORT                 PORTCbits.RC1
#define IO_RC1_WPU                  WPUCbits.WPUC1
#define IO_RC1_OD                   ODCONCbits.ODCC1
#define IO_RC1_ANS                  ANSELCbits.ANSC1
#define IO_RC1_SetHigh()            do { LATCbits.LATC1 = 1; } while(0)
#define IO_RC1_SetLow()             do { LATCbits.LATC1 = 0; } while(0)
#define IO_RC1_Toggle()             do { LATCbits.LATC1 = ~LATCbits.LATC1; } while(0)
#define IO_RC1_GetValue()           PORTCbits.RC1
#define IO_RC1_SetDigitalInput()    do { TRISCbits.TRISC1 = 1; } while(0)
#define IO_RC1_SetDigitalOutput()   do { TRISCbits.TRISC1 = 0; } while(0)
#define IO_RC1_SetPullup()          do { WPUCbits.WPUC1 = 1; } while(0)
#define IO_RC1_ResetPullup()        do { WPUCbits.WPUC1 = 0; } while(0)
#define IO_RC1_SetPushPull()        do { ODCONCbits.ODCC1 = 0; } while(0)
#define IO_RC1_SetOpenDrain()       do { ODCONCbits.ODCC1 = 1; } while(0)
#define IO_RC1_SetAnalogMode()      do { ANSELCbits.ANSC1 = 1; } while(0)
#define IO_RC1_SetDigitalMode()     do { ANSELCbits.ANSC1 = 0; } while(0)
// get/set IO_RC7 aliases
#define SW_TRIS                 TRISCbits.TRISC7
#define SW_LAT                  LATCbits.LATC7
#define SW_PORT                 PORTCbits.RC7
#define SW_WPU                  WPUCbits.WPUC7
#define SW_OD                   ODCONCbits.ODCC7
#define SW_ANS                  ANSELCbits.ANSC7
#define SW_SetHigh()            do { LATCbits.LATC7 = 1; } while(0)
#define SW_SetLow()             do { LATCbits.LATC7 = 0; } while(0)
#define SW_Toggle()             do { LATCbits.LATC7 = ~LATCbits.LATC7; } while(0)
#define SW_GetValue()           PORTCbits.RC7
#define SW_SetDigitalInput()    do { TRISCbits.TRISC7 = 1; } while(0)
#define SW_SetDigitalOutput()   do { TRISCbits.TRISC7 = 0; } while(0)
#define SW_SetPullup()          do { WPUCbits.WPUC7 = 1; } while(0)
#define SW_ResetPullup()        do { WPUCbits.WPUC7 = 0; } while(0)
#define SW_SetPushPull()        do { ODCONCbits.ODCC7 = 0; } while(0)
#define SW_SetOpenDrain()       do { ODCONCbits.ODCC7 = 1; } while(0)
#define SW_SetAnalogMode()      do { ANSELCbits.ANSC7 = 1; } while(0)
#define SW_SetDigitalMode()     do { ANSELCbits.ANSC7 = 0; } while(0)
/**
 * @ingroup  pinsdriver
 * @brief GPIO and peripheral I/O initialization
 * @param none
 * @return none
 */
void PIN_MANAGER_Initialize (void);

/**
 * @ingroup  pinsdriver
 * @brief Interrupt on Change Handling routine
 * @param none
 * @return none
 */
void PIN_MANAGER_IOC(void);


#endif // PINS_H
/**
 End of File
*/