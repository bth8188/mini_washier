/*
 * button.c
 *
 * Created: 2019-07-12 오전 10:21:48
 *  Author: kccistc
 */ 

#include "button.h"

static uint8_t button_state[] = {0, 0, 0, 0, 0, 0, 0, 0};

void Button_Init()
{
	BUTTON_DDR &= ~(0xfe);  //DDR 입력 설정
	DDRD &= ~(1 << 3);
}

int button(uint8_t _num)
{
		
	if( ((BUTTON_PIN & (1<<_num)) == 0) && (button_state[_num] == 0) )
	{
		setbuzzer(2000);
		playbuzzer();
		button_state[_num] = 1;
		return 0;
	}
	else if( ((BUTTON_PIN & (1 << _num)) == (1 << _num)) && (button_state[_num] == 1) )
	{
		nobuzzer();
		button_state[_num] = 0;
		return 1;
	}
	return 0;
}

int timer_button(uint8_t _num)
{
	
	if( ((BUTTON_PIN & (1<<_num)) == 0) && (button_state[_num] == 0) )
	{
		setbuzzer(2000);
		playbuzzer();
		button_state[_num] = 1;
		return 0;
	}
	if( ((BUTTON_PIN & (1<<_num)) == 0) && (button_state[_num] == 1) )
	{
		nobuzzer();
		_delay_ms(30);
		setbuzzer(2000);
		playbuzzer();
		return 1;
	}
	else if( ((BUTTON_PIN & (1 << _num)) == (1 << _num)) && (button_state[_num] == 1) )
	{
		nobuzzer();
		button_state[_num] = 0;
		return 1;
	}
	return 0;
}

int open_door()
{
	if( (PIND & (1 << 3)) == 0 ) return 0;
	else if( (PIND & (1 << 3)) == (1 << 3) ) return 1;
}