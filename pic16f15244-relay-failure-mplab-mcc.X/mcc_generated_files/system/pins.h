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