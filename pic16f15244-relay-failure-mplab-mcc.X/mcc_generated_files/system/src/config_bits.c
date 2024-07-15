/**
 * CONFIGURATION BITS Generated Driver Source File
 * 
 * @file config_bits.c
 * 
 * @ingroup config_bitsdriver
 * 
 * @brief This file contains the API Implementation for the Device Configuration Bits driver.
 *
 * @version Driver Version 2.0.2
 *
 * @version Package Version 4.1.3
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

// Configuration bits: selected in the GUI

//CONFIG1
#pragma config FEXTOSC = OFF     // External Oscillator Mode Selection bits->Oscillator not enabled
#pragma config RSTOSC = HFINTOSC_1MHZ     // Power-up Default Value for COSC bits->HFINTOSC (1 MHz)
#pragma config CLKOUTEN = OFF     // Clock Out Enable bit->CLKOUT function is disabled; I/O function on RA4
#pragma config VDDAR = HI     // VDD Range Analog Calibration Selection bit->Internal analog systems are calibrated for operation between VDD = 2.3V - 5.5V

//CONFIG2
#pragma config MCLRE = EXTMCLR     // Master Clear Enable bit->If LVP = 0, MCLR pin is MCLR; If LVP = 1, RA3 pin function is MCLR
#pragma config PWRTS = PWRT_OFF     // Power-up Timer Selection bits->PWRT is disabled
#pragma config WDTE = SWDTEN     // WDT Operating Mode bits->WDT enabled/disabled by SEN bit
#pragma config BOREN = ON     // Brown-out Reset Enable bits->Brown-out Reset Enabled, SBOREN bit is ignored
#pragma config BORV = LO     // Brown-out Reset Voltage Selection bit->Brown-out Reset Voltage (VBOR) set to 1.9V
#pragma config PPS1WAY = ON     // PPSLOCKED One-Way Set Enable bit->The PPSLOCKED bit can be set once after an unlocking sequence is executed; once PPSLOCKED is set, all future changes to PPS registers are prevented
#pragma config STVREN = ON     // Stack Overflow/Underflow Reset Enable bit->Stack Overflow or Underflow will cause a reset

//CONFIG4
#pragma config BBSIZE = BB512     // Boot Block Size Selection bits->512 words boot block size
#pragma config BBEN = OFF     // Boot Block Enable bit->Boot Block is disabled
#pragma config SAFEN = OFF     // SAF Enable bit->SAF is disabled
#pragma config WRTAPP = OFF     // Application Block Write Protection bit->Application Block is not write-protected
#pragma config WRTB = OFF     // Boot Block Write Protection bit->Boot Block is not write-protected
#pragma config WRTC = OFF     // Configuration Registers Write Protection bit->Configuration Registers are not write-protected
#pragma config WRTSAF = OFF     // Storage Area Flash (SAF) Write Protection bit->SAF is not write-protected
#pragma config LVP = ON     // Low Voltage Programming Enable bit->Low Voltage programming enabled. MCLR/Vpp pin function is MCLR. MCLRE Configuration bit is ignored.

//CONFIG5
#pragma config CP = OFF     // User Program Flash Memory Code Protection bit->User Program Flash Memory code protection is disabled

/**
 End of File
*/