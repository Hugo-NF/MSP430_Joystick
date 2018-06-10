#ifndef I2C_H
#define I2C_H

#define PRESCALE 10

#include <msp430.h>
#include <stdint.h>

unsigned char TX_data;

void I2C_master_init(unsigned char slave_address);
void I2C_transmit();
inline void mili_delay(uint16_t t_mili);
inline void micro_delay(uint16_t t_micro);

#endif
