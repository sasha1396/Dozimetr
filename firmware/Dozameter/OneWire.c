/*
* _ONEWire.c
*
* Created: 16.11.2019 16:37:11
*  Author: Sasha
*/

#include "OneWire.h"

unsigned char c = 0;

// ��������� ������� ������
void OneWire_Low (void)
{
	OneWire_DDR |= (1 << OneWire_PIN_NUM);
}

// ��������� �������� ������
void OneWire_High (void)
{
	OneWire_DDR &= ~(1 << OneWire_PIN_NUM);
}

// ������ ������ �� ����
unsigned char OneWire_Read_Level ()
{
	return OneWire_PIN & (1 << OneWire_PIN_NUM);
}

// ����� � �������� �������� ����������� �� ����������
void OneWireReset (void)
{
	OneWire_Low();
	_delay_ms(18);
	OneWire_High();
	while(OneWire_Read_Level());
	while(OneWire_Read_Level() == 0);
	while(OneWire_Read_Level());
}

unsigned char OneWireReceive()
{
	for (int i = 0; i < 8; i++)
	{
		while (!OneWire_Read_Level()); // ���� �� ���� 0
		_delay_us(30);
		if (OneWire_Read_Level())// ���� ������� ������� ������ 30 ���
		{
			c = (c<<1)|(0x01);// ���������� 1
		}
		else
		{
			c = (c<<1);// ����� ������ ��������, ��� ����� ������������ 0
		}
		while (OneWire_Read_Level());
	}
	return c;
}