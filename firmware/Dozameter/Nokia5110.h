/*
 * Nokia5110.h
 *
 * Created: 17.11.2019 18:24:57
 *  Author: User
 */ 

#ifndef NOKIA5110_H_
#define NOKIA5110_H_

#define F_CPU 8000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>


#define LCD_RST_PORT PORTB
#define LCD_RST_PIN 0
#define LCD_RST_set  LCD_RST_PORT |=  (1 << LCD_RST_PIN)    //external reset input
#define LCD_RST_clr  LCD_RST_PORT &=~ (1 << LCD_RST_PIN)

#define LCD_DC_PORT PORTB
#define LCD_DC_PIN 3
#define LCD_DC_set   LCD_DC_PORT |=  (1 << LCD_DC_PIN)    //data/commande
#define LCD_DC_clr   LCD_DC_PORT &=~ (1 << LCD_DC_PIN)

#define SDIN_PORT PORTB
#define SDIN_PIN 5
#define SDIN_set     SDIN_PORT |=  (1 << SDIN_PIN)    //serial data input
#define SDIN_clr     SDIN_PORT &=~ (1 << SDIN_PIN)

#define SCLK_PORT PORTB
#define SCLK_PIN 4
#define SCLK_set     SCLK_PORT |=  (1 << SCLK_PIN)    //serial clock input
#define SCLK_clr     SCLK_PORT &= ~(1 << SCLK_PIN)


void LCD_write_byte(unsigned char dat, unsigned char command);
void LCD_init();
void LCD_clear();
void LCD_set_XY(unsigned char X, unsigned char Y);
void LCD_write_char(unsigned char c);
void LCD_write_english_string(unsigned char X,unsigned char Y,char *s);
void Read_Image (unsigned char data); // Читаем наше изображение загруженное в память
void LCD_var_str(unsigned char X, unsigned char Y, uint16_t value, uint8_t nDigit); // можно выводить переменную до 4 знаков

#endif /* NOKIA5110_H_ */