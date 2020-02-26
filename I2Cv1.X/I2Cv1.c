/*
 * File:   I2Cv1.c
 * Author: Peter
 *
 * Created on February 24, 2020, 10:46 AM
 */


#include <xc.h>
#include "I2Cv1.h"

void i2c_master_init(unsigned long c){
    SSPCON = 0b00101000;
    SSPCON2 = 0;
    SSPADD = (_XTAL_FREQ/(4*c))-1;
    SSPSTAT = 0;
    TRISCbits.TRISC3 = 1;
    TRISCbits.TRISC4 = 1;
}

void i2c_masterWait(void){
    while ((SSPSTAT & 0x04) || (SSPCON2 & 0x1F));
}

void i2c_masterStart(void){
    i2c_masterWait();
    SSPCON2bits.SEN = 1;
}

void i2c_master_RStart(void){
    i2c_masterWait();
    SSPCON2bits.RSEN = 1;
}

void i2c_masterStop(void){
    i2c_masterWait();
    SSPCON2bits.PEN = 1;           //Initiate stop condition
}

void i2c_masterWrite(unsigned int data){
    i2c_masterWait();
    SSPBUF = data;         //Write data to SSPBUF
}

unsigned short  i2c_masterRead(unsigned short a){
    unsigned short temp;
    i2c_masterWait();
    SSPCON2bits.RCEN = 1;
    i2c_masterWait();
    temp = SSPBUF;      //Read data from SSPBUF
    i2c_masterWait();
    ACKDT = (a) ? 0:1;    //Acknowledge bit
    ACKEN = 1;          //Acknowledge sequence
    return temp;
}