/*
 * File:   I2Cv1.c
 * Author: Peter
 *
 * Created on February 24, 2020, 10:46 AM
 */


#include <xc.h>
#include "I2Cv1.h"

void i2c_master_init(unsigned long address){
    SSPCON = 0b00101000;
    SSPCON2 = 0;
    
}
