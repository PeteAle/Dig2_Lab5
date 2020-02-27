/*
 * File:   i2c_master.c
 * Author: Peter
 *
 * Created on February 24, 2020, 10:34 AM
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
#include <stdint.h>
#include <stdio.h>
#include "LCDv1.h"
#include "I2C.h"

float temp_pot1 = 0;
uint16_t centenas = 0, decenas = 0, unidades = 0;

void setup(void);

void main(void) {
    setup();
    lcd8_init();
    i2c_master_init(100000);
    while(1){
        //--------------------- Set el LCD --------------------------------
        lcd8_setCursor(1,1);
        delay_1ms();
        lcd8_dispChar("S1:");
        delay_1ms();
        lcd8_setCursor(1,6);
        delay_1ms();
        lcd8_dispChar("S2:");
        delay_1ms();
        lcd8_setCursor(1,11);
        delay_1ms();
        lcd8_dispChar("S3:");
        delay_1ms();
        //--------------------- Para los datos del pot --------------------
        i2c_masterStart();
        i2c_masterWrite(0x11);
        temp_pot1 = i2c_masterRead(1);
        i2c_masterStop();
        temp_pot1 = (temp_pot1*5.0)/255.0;
        temp_pot1 = temp_pot1*100;
        centenas = temp_pot1/100;
        temp_pot1 = temp_pot1 - (centenas*100);
        decenas = temp_pot1/10;
        unidades = temp_pot1 - (decenas*10);
        //-------------------- Desplegar datos del Pot en LCD -------------
        lcd8_setCursor(2,1);
        delay_1ms();
        lcd8_dispNum(centenas);
        delay_1ms();
        lcd8_dispChar(".");
        delay_1ms();
        lcd8_dispNum(decenas);
        delay_1ms();
        lcd8_dispNum(unidades);
    }
    return;
}

void setup(void){
    TRISA = 0x00;
    TRISE = 0xFF;
    ANSEL = 0x00;
    ANSELH = 0x00;
}