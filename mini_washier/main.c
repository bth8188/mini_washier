/*
 * mini_washier.c
 *
 * Created: 2019-07-09 오후 1:58:42
 * Author : kccistc
 */ 
#include "main.h"

uint8_t reset_Flag = 0, door_Flag = 0;



ISR(INT0_vect)
{
	_delay_ms(500);
	EIFR |= (1 << 3);
	
	if(main_power == off)
	{
		main_power = on;
		start_sound();
	}
	else if(main_power == on)
	{
		off_sound();
		whole_state = setting;
		yeyak = no;
		washer = watering;
		water_Flag_reset();
		daegi_flag_reset();
		DCmotorstop();
		nobuzzer();
		menu = 0;
		PORTG &= ~(1);
		PORTE = 0x00;
		main_power = off;
	}
	LCD_clear();
}


ISR(INT1_vect)
{
	_delay_ms(500);
	
	switch(whole_state)
	{
		case run:
		if(reset_Flag == 0)
		{
			whole_state = pause;
			reset_Flag =1;
			DCmotorstop();
			pause_time();
		}
		break;
	
		case pause:
		if(reset_Flag == 1)
		{
			whole_state = run;
			restart_time();
			reset_Flag = 0;
		}
		break;
	}
	LCD_clear();
}

ISR(INT2_vect)
{
	_delay_ms(500);
	
	if(open_door())
	{
		if(whole_state == run)
		{
			whole_state = pause;
			DCmotorstop();
			pause_time();
			door_Flag = 1;
		}
	}
	
	else if((whole_state == pause) && (door_Flag == 1))
	{
		whole_state = run;
		restart_time();
		door_Flag = 0;
	}
	LCD_clear();
}


ISR(TIMER0_COMP_vect)
{
	incMilliSec();
	incTime();
}

int main(void)
{
	uint8_t *dongjak;
	
	ADC_Init();
	LCD_init();
	buzzerInit();
	timer0init();
	DCmotorInit();
	DS1302_Init();
	Button_Init();
	DS1302_chogi();
	
	EIMSK |= (1 << INT0) | (1 << INT1) | (1 << INT2) ;
	EICRA |= (1 << ISC01) | (1 << ISC00);	//인터럽트 0 rising edge
	EICRA |= (1 << ISC11) | (1 << ISC10);	//인터럽트 1 rising edge
	EICRA |= (0 << ISC21) | (1 << ISC20);	//인터럽트 1 falling edge
	DDRG = 0x01;
	PORTG = 0x01;
	DDRD |= 0xf0;
	PORTD &= ~(0xf0);
	
	sei();
	
	LCD_write_XY_string(0,0, "start");
	start_sound();

	chogi_timer();
	whole_state = setting;
	yeyak = no;
	washer = watering;
	main_power = on;
	
	while (1)
	{
		switch(main_power)
		{
			case off:
			LCD_write_XY_string(0, 0, "                ");
			LCD_write_XY_string(1, 0, "   Sleeping...zz  ");
			whole_state = setting;
			reset_Flag = 0;
			yeyak = no;
			washer = watering;
			water_Flag_reset();
			daegi_flag_reset();
			DCmotorstop();
			nobuzzer();
			menu = 0;
			PORTG &= ~(1);
			PORTE = 0x00;
			PORTD = 0x00;
			break;
			
			case on:
			DDRG |= 0x01;
			PORTG = 0x01;
			switch(whole_state)
			{
				case setting:
				DCmotorstop();
				dongjak = step_setting_menu();
				break;
				
				case yeah:
				yeyakye();
				break;
				
				case pause:
				LCD_write_XY_string(0, 0, "     pause     ");
				LCD_write_XY_string(1, 0, "                ");
				DCmotorstop();
				break;
				
				case run:
				if(yeyak) waiting_yeyak();
				else run_washer(dongjak);
				break;
			}
			break;
		}
	}
}


