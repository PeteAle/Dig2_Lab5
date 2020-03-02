/*
 * File:   Lab5_i2c_slave_pot_v1.c
 * Author: Peter
 *
 * Created on February 25, 2020, 2:19 PM
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
#include "lib_osccon.h"
#include "lib_adc.h"
#include "I2C.h"

#define _XTAL_FREQ 4000000

short x = 0;
short z = 0;
unsigned char pot = 0;

void setup(void);
void globalAdcIntEnable(void);

void __interrupt() isr(){
    if (PIR1bits.ADIF == 1){
        pot = ADRESH;
        PORTB = pot;
        PIR1bits.ADIF = 0;
    }
    if (PIR1bits.SSPIF == 1){
        SSPCONbits.CKP = 0;
        if (SSPCONbits.WCOL == 1 || SSPCONbits.SSPOV == 1){
            x = SSPBUF;
            SSPCONbits.WCOL = 0;
            SSPCONbits.SSPOV = 0;
            SSPCONbits.CKP = 1;
        }
        if(!SSPSTATbits.D_nA && !SSPSTATbits.R_nW){
            x = SSPBUF;                 // Lectura del SSBUF para limpiar el buffer y la bandera BF
            //__delay_us(2);
            PIR1bits.SSPIF = 0;         // Limpia bandera de interrupción recepción/transmisión SSP
            SSPCONbits.CKP = 1;         // Habilita entrada de pulsos de reloj SCL
            while(!SSPSTATbits.BF);     // Esperar a que la recepción se complete
            x = SSPBUF;             // Guardar en el PORTD el valor del buffer de recepción
            __delay_us(250);
        }
        else if (!SSPSTATbits.D_nA && SSPSTATbits.R_nW){
            x = SSPBUF;
            SSPSTATbits.BF = 0;
            SSPBUF = pot;
            SSPCONbits.CKP = 1;
            __delay_us(250);
            while(SSPSTATbits.BF);
        }
        PIR1bits.SSPIF = 0;
    }
}

void main(void) {
    setup();
    oscInit(1);
    adcSetup();
    analogInSel(5);
    adcFoscSel(1);
    adcInterrupt(1);
    i2c_slave_init(0x10);
    while(1){
        if (ADCON0bits.GO_DONE == 0){
            ADCON0bits.GO_DONE = 1;
        }
    }
    return;
}

void setup(void){
    TRISEbits.TRISE0 = 1;
    ANSELbits.ANS5 = 1;
    TRISB = 0;
    PORTB = 0;
}

void globalAdcIntEnable(void){
    INTCONbits.GIE = 1;
    INTCONbits.PEIE = 1;
}