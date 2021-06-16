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

char err1 = 0;// ���� ������ ������

void I2C_Init(void)
{
	TWBR=0x20; //�������� �������� (��� 8 ��� ���������� 100 ���)
}


void I2C_StartCondition(void)
{
	TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
	while(!(TWCR&(1<<TWINT)));//�������� ���� ����������� TWIN
}


void I2C_StopCondition(void)
{
	TWCR = (1<<TWINT)|(1<<TWSTO)|(1<<TWEN);
}

void I2C_SendByte(unsigned char c)
{
	TWDR = c;//������� ���� � ������� ������
	TWCR = (1<<TWINT)|(1<<TWEN);//������� �������� �����
	while (!(TWCR & (1<<TWINT)));//�������� ���� ����������� TWIN
}


unsigned char I2C_ReceiveByte(void)
{
	err1 = 0;
	TWCR = (1<<TWINT)|(1<<TWEN);//������� ����� ������
	while(!(TWCR & (1<<TWINT)));//�������� ���� ����������� TWIN
	if ((TWSR & 0xF8) != TW_MR_DATA_NACK) err1 = 1;
	else err1 = 0;
	return TWDR;
}


void I2C_write(uint8_t data, uint8_t address) // ���������� ������ � ��������� �������
{
	I2C_StartCondition(); //�������� ������� START
	I2C_SendByte(ADDR_W); //�������� ����� � ��� ������ (0)
	I2C_SendByte(address);//��������� �� 0x0000 � ������� ���� ������ ������
	I2C_SendByte(data); // � ������� ���� ������ ������
	I2C_StopCondition(); //�������� ������� STOP
}


uint8_t I2C_read(uint8_t address)
{
	uint8_t data;
	I2C_StartCondition(); //�������� ������� START
	I2C_SendByte(ADDR_W);//�������� ����� ���������� � ��� ������ (0)
	I2C_SendByte(address);//�������� ����� ��������
	I2C_StartCondition(); //�������� ������� START
	I2C_SendByte(ADDR_R);//�������� ����� ���������� � ��� ������ (1)
	data = I2C_ReceiveByte();
	I2C_StopCondition(); //�������� ������� STOP
	return data;
}



