
#include "ADC.h"
#include <avr/io.h>
#include <inttypes.h>
#include <util/delay.h>


unsigned int adc_device(unsigned char ADC_channel) // когда вызываем, то в скобочках указываем номер канала АЦП, чтобы мерить на нескольких
{
	ADMUX = ADC_channel | (ADMUX & 0xF0); // устанавливаем канал, при этом сбрасываем все значения установки каналов, не задевая тругие регистры
	
	//задержка для стабилизации входного напряжения
	_delay_us(10);
	
	//начинаем преобразование (ADSC = 1)
	ADCSRA |= (1 << ADSC);
	while((ADCSRA & (1 << ADIF)) == 0 ); //ждем, пока АЦП закончит преобразование (ADIF = 0)
	ADCSRA |= (1 << ADIF); //устанавливаем ADIF
	return ADCW; //ADCW - содержит ADCH и ADCL
}

void init_adc(void)
{
	PORTC = 0b00000000; // порты
	DDRC = 0b00000000; // порты
	
	// ADC
	ADMUX |= ((0 << REFS1) | (1 << REFS0) | (0 << ADLAR) | (0 << MUX3) | (0 << MUX2) | (0 << MUX1) | (0 << MUX0));
	ADCSRA |= ((1 << ADEN) | (0 << ADSC) | (0 << ADATE) | (0 << ADIF) | (0 << ADIE) | (1 << ADPS2) | (0 << ADPS1) | (1 << ADPS0));
}

