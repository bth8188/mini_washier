﻿/*
 * lcd.h
 *
 * Created: 2019-07-02 오후 2:11:48
 *  Author: kccistc
 */ 


#ifndef LCD_H_
#define LCD_H_

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>


#define PORT_DATA  PORTC
#define PORT_CONTROL  PORTB
#define DDR_DATA  DDRC
#define DDR_CONTROL  DDRB
#define BACKLIGHT_DDR DDRG
#define BACKLIGHT_PORT PORTG

#define RS_PIN  2
#define RW_PIN  3
#define E_PIN  4

#define COMMAND_CLEAR_DISPLAY   0X01
#define COMMAND_8_BIT_MODE   0X38
#define COMMAND_4_BIT_MODE   0X28

#define COMMAND_DISPLAY_ON_OFF_BIT   2
#define COMMAND_CURSOR_ON_OFF_BIT   1
#define COMMAND_BLINK_ON_OFF_BIT   0


void LCD_pulse_enable();
void LCD_write_data(uint8_t data);
void LCD_write_command(uint8_t command);
void LCD_clear();
void LCD_init();
void LCD_write_string(char *string);
void LCD_goto_XY(uint8_t row, uint8_t col);
void LCD_write_XY_string(uint8_t row, uint8_t col, char *string);
void Time_Frame();





#endif /* LCD_H_ */