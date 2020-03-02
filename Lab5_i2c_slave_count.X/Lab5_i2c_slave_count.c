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
#include "I2C.h"

unsigned char i = 0;
unsigned char j = 0;
short x = 0;
short z = 0;

void setup(void);

void __interrupt() isr(){
    di();
    if (PIR1bits.SSPIF == 1){
            SSPCONbits.CKP = 0;
            if (SSPCONbits.WCOL == 1 || SSPCONbits.SSPOV == 1){
                x = SSPBUF;
                SSPCONbits.WCOL = 0;
                SSPCONbits.SSPOV = 0;
                SSPCONbits.CKP = 1;
            }
            if(!SSPSTATbits.D_nA && !SSPSTATbits.R_nW){
                x = SSPBUF;
                while(!SSPSTATbits.BF);
                z = SSPBUF;
                SSPCONbits.CKP = 1;
            }
            else if (!SSPSTATbits.D_nA && SSPSTATbits.R_nW){
                x = SSPBUF;
                SSPSTATbits.BF = 0;
                SSPBUF = PORTB;
                SSPCONbits.CKP = 1;
                __delay_us(300);
                while(SSPSTATbits.BF);
            }
            PIR1bits.SSPIF = 0;
        }
    ei();
}

void main(void) {
    setup();
    i2c_slave_init(0x20);
    while(1){
        while (PORTDbits.RD2 == 1){
            di();
            i = 1;
            if (PORTDbits.RD2 == 0 && i == 1){
                PORTB++;
                i = 1;
                if (PORTB > 15){
                    PORTB = 0;
                }
            }
            ei();
        }
        while (PORTDbits.RD1 == 1){
            di();
            j = 1;
            if (PORTDbits.RD1 == 0 && j == 1){
                if (PORTB == 0){
                    j = 0;
                }
                else{
                    PORTB--;
                    j = 0;   
                }
            }
            ei();
        }
    }
    return;
}

void setup(void){
    TRISDbits.TRISD2 = 1;
    TRISDbits.TRISD1 = 1;
    PORTD = 0;
    TRISB = 0x00;
    ANSEL = 0;
    ANSELH = 0;
    PORTB = 0;
}