/*
 * PWM.c
 *
 * Created: 01.12.2018 21:24:14
 * Author : Sasha
 */ 
#include "PWM.h"
#include <avr/io.h>
#include <util/delay.h>

char i = 0;

// ВКЛ/ВЫКЛ ШИМ A1 Накачка высокого напряжения
void SwitchPwmSolder(char value)
{
	if (value)
	{
		TCCR1A |= (1 << COM1A1); // канал A включил
		OCR1A = 0; //
	}
	else
	TCCR1A &= ~(1 << COM1A1);
}


void sound_generator(char n)// n - колличество писков, t - длительность сигнала (1 - это 100 мс)
{
	TCCR0A |= (1 << COM0A1); // включаем ШИМ
	while(i < n)
	{
		TCCR0A |= (1 << COM0A1);
		OCR0A = 0x7F;// 75% 150
		_delay_ms(5); // длительность
		OCR0A = 0x00;
		TCCR0A &= ~(1 << COM0A1);
		_delay_ms(5); // пауза
		i++;
	}
	TCCR0A &= ~(1 << COM0A1); // выключаем ШИМ
	i = 0;// обнулили счётчик
}

void init_pwm(void)// инициализация портов для ШИМ
{
	// Обязательно сделать порт на выход, но не делать на нём лог 1
//    PORTB = 0b00000000;
	DDRB |= (1 << PB1);
//	PORTD = 0b00000000; 
	DDRD |= (1 << PD6);
	
	// ШИМ 0-го канала
	TCCR0A |= ((0 << COM0A1) | (0 << COM0A0) | (0 << COM0B1) | (0 << COM0B0) | (1 << WGM01) | (1 << WGM00)); // FAST PWM
	TCCR0B |= ((0 << FOC0A)  | (0 << FOC0B) | (0 << WGM02) | (0 << CS02) | (1 << CS01) | (0 << CS00));
	//OCR0A = 0x7D; // Оба работают
	//OCR0B = 0xDC; // на 50% скважности
	
	// ШИМ первого канала  COM1A1 (COM1B1) - включают ШИМ /// COM1A0 (COM1B0) - инвертирует сигнал
	TCCR1A |= ((1 << COM1A1) | (0 << COM1A0) | (0 << COM1B1) | (0 << COM1B0)  | (1 << WGM11) | (1 << WGM10)); 
	//Таймер B
	TCCR1B |= ((0 << ICNC1) | (0 << ICES1) | (0 << WGM13) | (1 << WGM12) | (0 << CS12) | (0 << CS11) | (1 << CS10));
	TIMSK1 |= ((0 << ICIE1) | (0 << OCIE1B) | (1 << OCIE1A) | (0 << TOIE1));
	//OCR1AH = 0b01111010; //записываем в регистр число для сравнения
	//OCR1AL = 0b00010010;

	TCCR1C |= ((0 << FOC1A)  | (0 << FOC1B));
	
	// ШИМ второго канала
		
	/*CS22 CS21 CS20	Description
		0	0	0		No clock source (Timer/Counter stopped)
		0	0	1		clkT2S/(No prescaling)
		0	1	0		clkT2S/8 (From prescaler)
		0	1	1		clkT2S/32 (From prescaler)
		1	0	0		clkT2S/64 (From prescaler)
		1	0	1		clkT2S/128 (From prescaler)
		1	1	0		clkT2S/256 (From prescaler)
		1	1	1		clkT2S/1024 (From prescaler)*/
		
	// Fpwm = Fclk/(N*510) (должно быть 1-3кГц для динамика ((0 << CS22) | (1 << CS21) | (0 << CS20))) F = 1960 Гц
	//TCCR2A |= ((0 << FOC1A) | (0 << COM2A1) | (0 << COM2A0) | (1 << WGM20) | (0 << WGM21) | (0 << CS22) | (1 << CS21) | (0 << CS20));
	//TIMSK0 |= ((0 << TOIE2) | (0 << OCIE1A) | (0 << OCIE1B) | (0 << TOIE1) | (0 << TOIE0));
	//OCR2A = 0x7F; // 50% коэффициент заполнения
}

