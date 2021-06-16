/*
 * _ONEWire.h
 *
 * Created: 16.11.2019 16:36:55
 *  Author: Sasha
 */ 


#ifndef ONEWIRE_H_
#define ONEWIRE_H_


#define F_CPU 8000000UL

#include <avr/io.h>
#include <util/delay.h>


#define OneWire_PORT PORTC
#define OneWire_PIN PINC
#define OneWire_DDR DDRC
#define OneWire_PIN_NUM PC3

void OneWire_Low (void);// Установка низкого уровня
void OneWire_High (void);// Установка высокого уровня
unsigned char OneWire_Read_Level (void);// Чтение уровня на шине
void OneWireReset (void);// Функция сброса на шине
unsigned char OneWireReceive();// Чтение данных
#endif /* ONEWIRE_H_ */