/*
 * button.h
 *
 * Created: 2019-07-12 오전 10:22:06
 *  Author: kccistc
 */ 


#ifndef BUTTON_H_
#define BUTTON_H_

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "buzzer.h"

#define BUTTON_DDR DDRG
#define BUTTON_PIN PING
#define MENU_BUTTON PING1
#define UP_BUTTON PING2
#define DOWN_BUTTON PING3
#define DOOR PING4

void Button_Init();
int button(uint8_t _num);
int timer_button(uint8_t _num);
int open_door();



#endif /* BUTTON_H_ */