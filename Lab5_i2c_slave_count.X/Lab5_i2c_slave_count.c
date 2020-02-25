/*
 * File:   Lab5_i2c_slave_count.c
 * Author: Peter
 *
 * Created on February 25, 2020, 2:55 PM
 */

// PIC16F887 Configuration Bit Settings

// 'C' source line config statements

// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>
#include <stdio.h>
#include <stdint.h>

unsigned char i = 0;
unsigned char j = 0;

void setup(void);

void main(void) {
    setup();
    while(1){
        if (PORTEbits.RE0 == 1){
            i = 1;
            if (PORTEbits.RE0 == 0 && i == 1){
                PORTA += PORTA;
                i = 0;
                if (PORTA == 16){
                    PORTA = 0;
                }
            }
        }
        else if (PORTEbits.RE1 == 1){
            j = 1;
            if (PORTEbits.RE1 == 0 && j == 1){
                PORTA -= PORTA;
                j = 0;
            }
            else if (PORTA == 0){
                j = 0;
            }
        }
    }
    return;
}

void setup(void){
    TRISEbits.TRISE0 = 1;
    TRISEbits.TRISE1 = 1;
    TRISA = 0xFF;
    ANSEL = 0;
    ANSELH = 0;
    PORTA = 0;
}