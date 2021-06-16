/*
* DNT11.c
*
* Created: 16.11.2019 16:20:58
* Author : User
*/

#define  F_CPU 8000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "OneWire.h"
#include "Nokia5110.h"
#include "i2c.h"
#include "BMP180.h"
#include "image.h"
#include "PWM.h"
#include "ADC.h"

//����� ������ ��������
#define oss 1

//������
#define KEY_1  PIND&(1 << PD1)
#define KEY_2  PIND&(1 << PD2)
#define KEY_3  PIND&(1 << PD3)
#define KEY_4  PIND&(1 << PD4)

unsigned char KEY_1_previous; // ���������� ��� �������� ����������� ��������� ������ 1
unsigned char KEY_2_previous; // ���������� ��� �������� ����������� ��������� ������ 2
unsigned char KEY_3_previous; // ���������� ��� �������� ����������� ��������� ������ 3
unsigned char KEY_4_previous; // ���������� ��� �������� ����������� ��������� ������ 4

unsigned char KEY_1_current; // ���������� ��� �������� �������� ��������� ������ 1
unsigned char KEY_2_current; // ���������� ��� �������� �������� ��������� ������ 2
unsigned char KEY_3_current; // ���������� ��� �������� �������� ��������� ������ 3
unsigned char KEY_4_current; // ���������� ��� �������� �������� ��������� ������ 4

// ������� ���������
#define counter PINC&(1 << PC1)
unsigned char counter_previous; // ���������� ��� �������� ����������� ��������� ������ 4
unsigned char counter_current; // ���������� ��� �������� �������� ��������� ������ 1

float temp = 0, hum = 0;
double u_bat = 0, u_hv = 0;
unsigned char I_RH,D_RH, I_Temp, D_Temp, CheckSum, x = 0, t = 0, lcd = 0;

short AC1, AC2, AC3, count = 0;
unsigned short AC4, AC5, AC6;
short B1, B2, MB, MC, MD;
long B5, P, X1, X2, X3;

// ��������� ��������������� �������� �������
void read_comp_REG(void)
{
	AC1 = (I2C_read(0xAA) << 8) + I2C_read(0xAB);
	AC2 = (I2C_read(0xAC) << 8) + I2C_read(0xAD);
	AC3 = (I2C_read(0xAE) << 8) + I2C_read(0xAF);
	AC4 = (I2C_read(0xB0) << 8) + I2C_read(0xB1);
	AC5 = (I2C_read(0xB2) << 8) + I2C_read(0xB3);
	AC6 = (I2C_read(0xB4)<< 8) + I2C_read(0xB5);
	B1 = (I2C_read(0xB6) << 8) + I2C_read(0xB7);
	B2 = (I2C_read(0xB8) << 8) + I2C_read(0xB9);
	//MB = I2C_read(0xBA)*256 + I2C_read(0xBB);
	MC = (I2C_read(0xBC) << 8) + I2C_read(0xBD);
	MD = (I2C_read(0xBE) << 8) + I2C_read(0xBF);
}


float Temp(void)
{
	//�������� ������ ����������� ��������� ������ � ������� 0xF4
	I2C_write(0x2E, 0xF4); // �������� 0x2E � ������� 0xF4 � �������� �����������
	_delay_ms(5); 	//�������� 4.5 ��
	long UT = (I2C_read(0xF6) << 8) + I2C_read(0xF7); 	//��������� �������� �����������
	long X1 = ((UT - AC6)*AC5) >> 15;
	long X2 = ((MC << 11)/(X1 + MD));
	B5 = X1 + X2;
	long T = (B5 + 8) >> 4;
	//T = (float)T / 10;
	return T;
}

long Press(void)
{
	//�������� ������ �������� ��������� ������ � ������� 0xF4
	I2C_write(0x34 + (oss << 6), 0xF4); // �������� 0xB4 � ������� 0xF4 � �������� �������� � ������� ��������������
	_delay_ms(8); 	//�������� 8 ��, ��� �������� �������� ���������
	long U = (I2C_read(0xF6) << 8);
	long UP = ((U << 8) + (I2C_read(0xF7) << 8) + I2C_read(0xF8)) >> (8 - oss); 	//��������� �������� ��������
	long B6 = B5 - 4000;
	X1 = (B2 * ((B6 * B6) >> 12)) >> 11;
	X2 = (AC2 * B6) >> 11;
	X3 = X1 + X2;
	long B3 = (((AC1 * 4 + X3) << oss) + 2) >> 2;
	X1 = (AC3 * B6) >> 13;
	X2 = (B1 * ((B6 * B6) >> 12)) >> 16;
	X3 = ((X1 + X2) + 2) >> 2;
	unsigned long B4 = (AC4 * (unsigned long)(X3 + 32768)) >> 15;
	unsigned long B7 = ((unsigned long)UP - B3) * (50000 >> oss);
	if (B7 < 0x80000000)
	{
		P = (B7 * 2) / B4;
	}else
	{
		P = (B7 / B4) * 2;
	}
	
	X1 = (P >> 8) * (P >> 8);
	X1 = (X1 * 3038) >> 16;
	X2 = (-7357 * P) >> 16;
	P = P + ((X1 + X2 + 3791) >> 4);
	P = (P * 760) / 101325; // ������� � ��.��.��.
	return P;
}

void counter(void)
{
	counter_current = counter;
	if ((counter_previous != 0) && (counter_current == 0))
	{
		count++;
	}
	counter_previous = counter_current;
}

void ()
{
	if (times == 46) // ��������� 46 � 
	{
		times = 0;
		if (count == 105) // ���� 105, �� ��� 1 ���/�
		{
			doza++;
			count = 0;
		}
	}
}

ISR (TIMER1_COMPA_vect)
{
	if (x == 30)
	{
		//OneWireReset(); // �������� ����� � ������� ������� �����������
//
		//// ������� ����� �������� ��� ���� ����������
		//I_RH = OneWireReceive();// ������ ���� - ����� ����� ���������
		//D_RH = OneWireReceive();// ������ ���� - ���������� �������� ��������� ����� �����
		//I_Temp = OneWireReceive();// ������ ���� - ����� ����� �����������
		//D_Temp = OneWireReceive();// �������� ���� - ���������� �������� ����������� ����� �����
		//CheckSum = OneWireReceive();// ����� ���� - ����������� �����
		x = 0;
	}
	x += 1;
}

int main(void)
{

	AC1 = (I2C_read(0xAA) << 8) + I2C_read(0xAB);
	AC2 = (I2C_read(0xAC) << 8) + I2C_read(0xAD);
	AC3 = (I2C_read(0xAE) << 8) + I2C_read(0xAF);
	AC4 = (I2C_read(0xB0) << 8) + I2C_read(0xB1);
	AC5 = (I2C_read(0xB2) << 8) + I2C_read(0xB3);
	AC6 = (I2C_read(0xB4)<< 8) + I2C_read(0xB5);
	B1 = (I2C_read(0xB6) << 8) + I2C_read(0xB7);
	B2 = (I2C_read(0xB8) << 8) + I2C_read(0xB9);
	//MB = I2C_read(0xBA)*256 + I2C_read(0xBB);
	MC = (I2C_read(0xBC) << 8) + I2C_read(0xBD);
	MD = (I2C_read(0xBE) << 8) + I2C_read(0xBF);
	
	DDRB = 0xFF;
	
	I2C_Init();// ������������� ����
	LCD_init();// Nokia
	init_pwm();
	init_adc();

	DDRC |= (1 << PC0);
	
	PORTD |= (1 << PD7);
	//sound_generator(1);
	//OCR0A = 0x7F;
	
	//for(int n = 0; n < sizeof(cool_boy); n++)
	//{
	//LCD_write_byte(pgm_read_byte (cool_boy + n), 0);
	//_delay_ms(2);
	//}
	//_delay_ms(2000);
	//
	//for(int n = 0; n < sizeof(orel); n++)
	//{
	//LCD_write_byte(pgm_read_byte (orel + n), 0);
	//_delay_ms(2);
	//}
	//_delay_ms(2000);
	//
	//for(int n = 0; n < sizeof(girl); n++)
	//{
	//LCD_write_byte(pgm_read_byte (girl + n), 0);
	//_delay_ms(2);
	//}
	//_delay_ms(2000);
	//
	//for(int n = 0; n < sizeof(face); n++)
	//{
	//LCD_write_byte(pgm_read_byte (face + n), 0);
	//_delay_ms(2);
	//}
	//_delay_ms(2000);
	//
	//for(int n = 0; n < sizeof(frame_1); n++)
	//{
	//LCD_write_byte(pgm_read_byte (frame_1 + n), 0);
	//_delay_ms(2);
	//}
	//_delay_ms(2000);
	OCR1A = 10;
	
	LCD_clear();
	//	DDRD |= (1 << PD7);
	sei();
	
	while (1)
	{
		u_bat = ((adc_device(6) * 6.68) / 1023);
		u_hv = adc_device(7);
		//if ()
		//	{
		//OneWireReset(); // �������� ����� � ������� ������� �����������
		//
		//// ������� ����� �������� ��� ���� ����������
		//I_RH = OneWireReceive();// ������ ���� - ����� ����� ���������
		//D_RH = OneWireReceive();// ������ ���� - ���������� �������� ��������� ����� �����
		//I_Temp = OneWireReceive();// ������ ���� - ����� ����� �����������
		//D_Temp = OneWireReceive();// �������� ���� - ���������� �������� ����������� ����� �����
		//CheckSum = OneWireReceive();// ����� ���� - ����������� �����
		//
		//}

		////if ((I_RH + D_RH + I_Temp + D_Temp) != CheckSum) // ���� ����������� ����� �� �������
		////	{
		////		lcd_clr();
		////		lcd_gotoxy(6,0);
		////		lcd_string("Error",5);
		////		_delay_ms(1000);
		////	}
		////	else
		////	{
		//
		if (lcd == 0)
		{
			LCD_write_english_string(0,0,"Temper: ");
			LCD_var_str(45,0, I_Temp, 2); // ��� �������� ������ ����� �������� ��� ������ �� X
			LCD_write_english_string(57,0,".");
			LCD_var_str(63,0, D_Temp, 1);
			LCD_write_english_string(70,0,"C");
			LCD_write_english_string(0,1,"Humidity: ");
			LCD_var_str(57,1, I_RH, 2);
			LCD_write_english_string(72,1,"%");
			
			//LCD_var_str(0,2, adc_device(6), 3);
			
			LCD_write_english_string(0,3,"Temp: ");
			LCD_var_str(35,3, Temp(), 3); // ��� �������� ������ ����� �������� ��� ������ �� X
			//LCD_write_english_string(47,3,".");
			//LCD_var_str(52,3, fmod(Temp(),10), 1);
			LCD_write_english_string(65,3,"C");
			LCD_write_english_string(0,4,"Press: ");
			LCD_var_str(40,4, Press(), 4);// ��������
			LCD_write_english_string(68,4,"Pa");
		}
		else
		{
			LCD_write_english_string(0,0,"Battery: ");
			LCD_var_str(55,0, u_bat, 1); // ��� �������� ������ ����� �������� ��� ������ �� X
			LCD_write_english_string(61,0,".");
			LCD_var_str(66,0, fmod (u_bat,10), 1);
			LCD_write_english_string(0,1,"HV:");
			LCD_var_str(25,1, u_hv, 4);
		}

		KEY_1_current = KEY_1;
		if ((KEY_1_previous != 0) && (KEY_1_current == 0))
		{
			OCR1A += 10;
		}
		KEY_1_previous = KEY_1_current;
		
		KEY_2_current = KEY_2;
		if ((KEY_2_previous != 0) && (KEY_2_current == 0))
		{
			OCR1A -= 10;
		}
		KEY_2_previous = KEY_2_current;
		LCD_var_str(0,5,x, 3);// ��������
		
		KEY_3_current = KEY_3;
		if ((KEY_3_previous != 0) && (KEY_3_current == 0))
		{
			lcd ^= 1;
			LCD_clear();
		}
		KEY_3_previous = KEY_3_current;
		
		KEY_4_current = KEY_4;
		if ((KEY_4_previous != 0) && (KEY_4_current == 0))
		{
			PORTC ^= (1 << PC0);
		}
		KEY_4_previous = KEY_4_current;
		LCD_var_str(0,5,OCR1A, 3);// ��������
		
		
		
		//}
	}
}

