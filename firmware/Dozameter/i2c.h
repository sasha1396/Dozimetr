/*
 * i2c.h
 *
 * Created: 29.07.2019 23:11:15
 *  Author: Sasha
 */ 

#ifndef I2C_H_
#define I2C_H_

#include <inttypes.h>

void I2C_Init(void);
void I2C_StartCondition(void);
void I2C_StopCondition(void);
void I2C_SendByte(unsigned char c);
unsigned char I2C_ReceiveByte(void);
void I2C_write(uint8_t data, uint8_t address);
uint8_t I2C_read(uint8_t address);

#endif /* I2C_H_ */