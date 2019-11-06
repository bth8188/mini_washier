/*
 * main.h
 *
 * Created: 2019-07-10 오후 8:25:14
 *  Author: kccistc
 */ 


#ifndef MAIN_H_
#define MAIN_H_

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include "buzzer.h"
#include "lcd.h"
#include "time.h"
#include "dcmotor.h"
#include "DS1302.h"
#include "button.h"
#include "ADC.h"
#include "setak.h"



enum {setting, yeah, pause, run} whole_state;	//동작 여부
enum {no, yes} yeyak;	//예약 여부
enum {off, on} main_power;


#endif /* MAIN_H_ */