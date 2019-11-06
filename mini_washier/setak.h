/*
 * setak.h
 *
 * Created: 2019-07-10 오후 8:24:57
 *  Author: kccistc
 */ 


#ifndef SETAK_H_
#define SETAK_H_

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <stdio.h>
#include "lcd.h"
#include "time.h"
#include "dcmotor.h"
#include "DS1302.h"
#include "button.h"
#include "main.h"

enum {watering, bull, sesese, dada, haha, talsu} washer;
enum {normal, cuscus} menu;

	
uint32_t* setting_washer(uint32_t _mode);
void bulym(uint32_t _num);
void setak(uint32_t _num1, uint32_t _num2);
void hang(uint32_t _num);
void tal(uint32_t _num);
void daegi();
void run_washer(uint32_t* _mode);
uint8_t* step_setting_menu();
void yeyakye();
void chogi_timer();
void waiting_yeyak();
void pause_time();
void restart_time();
void waterwatering();
void water_Flag_reset();
void daegi_flag_reset();



#endif /* SETAK_H_ */