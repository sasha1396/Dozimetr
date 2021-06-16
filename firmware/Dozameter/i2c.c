/*
 * i2c.c
 *
 * Created: 29.07.2019 23:11:02
 *  Author: Sasha
 */ 

#include <avr/io.h>
#include <util/twi.h>
#include "i2c.h"
#include "BMP180.h"

char err1 = 0;// сюда вернем ошибку

void I2C_Init(void)
{
	TWBR=0x20; //скорость передачи (при 8 м√ц получаетс€ 100 к√ц)
}


void I2C_StartCondition(void)
{
	TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
	while(!(TWCR&(1<<TWINT)));//подождем пока установитс€ TWIN
}


void I2C_StopCondition(void)
{
	TWCR = (1<<TWINT)|(1<<TWSTO)|(1<<TWEN);
}

void I2C_SendByte(unsigned char c)
{
	TWDR = c;//запишем байт в регистр данных
	TWCR = (1<<TWINT)|(1<<TWEN);//включим передачу байта
	while (!(TWCR & (1<<TWINT)));//подождем пока установитс€ TWIN
}


unsigned char I2C_ReceiveByte(void)
{
	err1 = 0;
	TWCR = (1<<TWINT)|(1<<TWEN);//включим прием данных
	while(!(TWCR & (1<<TWINT)));//подождем пока установитс€ TWIN
	if ((TWSR & 0xF8) != TW_MR_DATA_NACK) err1 = 1;
	else err1 = 0;
	return TWDR;
}


void I2C_write(uint8_t data, uint8_t address) // записываем данные в указанный регистр
{
	I2C_StartCondition(); //ќтправим условие START
	I2C_SendByte(ADDR_W); //передаем адрес и бит записи (0)
	I2C_SendByte(address);//переходим на 0x0000 Ч старший байт адреса пам€ти
	I2C_SendByte(data); // Ч младший байт адреса пам€ти
	I2C_StopCondition(); //ќтправим условие STOP
}


uint8_t I2C_read(uint8_t address)
{
	uint8_t data;
	I2C_StartCondition(); //ќтправим условие START
	I2C_SendByte(ADDR_W);//передаем адрес устройства и бит записи (0)
	I2C_SendByte(address);//передаем адрес регистра
	I2C_StartCondition(); //ќтправим условие START
	I2C_SendByte(ADDR_R);//передаем адрес устройства и бит чтени€ (1)
	data = I2C_ReceiveByte();
	I2C_StopCondition(); //ќтправим условие STOP
	return data;
}



