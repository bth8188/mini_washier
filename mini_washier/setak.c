/*
 * setak.c
 *
 * Created: 2019-07-10 오후 8:24:41
 *  Author: kccistc
 */ 

#include "setak.h"

DS1302 remains;
DS1302 pausing;
uint32_t endhours;
uint8_t checker = 0;
uint32_t standard[] = {1, 2, 2 , 2, 1};	//세탁모드 - 표준
uint32_t gangryuk[] = {1, 3, 2, 1, 1};	//세탁모드 - 강력
uint32_t boolim[] = {2, 2, 2, 1, 1};		//세탁모드 - 불림
uint32_t salmeum[] = {2, 2, 2, 1, 1};	//세탁모드 - 삶음
uint32_t custom[5] = {0, 0, 0, 0, 0};
int yeyakhours = 0, yeyakminutes = 0, daegi_flag = 0, hotwater = 0, water_Flag = 0;


uint32_t* setting_washer(uint32_t _mode)
{
	uint32_t *temp;  // 실제 사용될 모드와 커스텀 값이 들어갈 포인터

	switch(_mode)
	{
		case 0:
		temp = standard;
		break;
		
		case 1:
		temp = gangryuk;
		break;
		
		case 2:
		temp = boolim;
		break;
		
		case 3:
		temp = salmeum;
		hotwater = 1;
		break;
		
		case 4:
		temp = custom;
		break;
	}
	return temp;
}

void bulym(uint32_t _num)
{
	static uint32_t nowtime = 0, hbuff[16] = {0}, mbuff[16] = {0}, sbuff[16] = {0};
	uint8_t hh, mm, ss;
	uint8_t endhours;
	endhours = 10*_num;

	//급수등 on;
	LCD_write_XY_string(0, 0, "   Soaking...   ");
	//센서수치 - > 급수등 off
	
	DS1302_GetTime(&remains);
	nowtime = remains.hour*3600 + remains.minutes*60 + remains.seconds;
	hh = (endhours-nowtime)/3600;
	mm = (endhours-nowtime)%3600/60;
	ss = (endhours-nowtime)%60;
	sprintf(hbuff,"   %02d:", hh);
	sprintf(mbuff,"%02d:", mm);
	sprintf(sbuff,"%02d    ", ss);
	if(checker != remains.seconds)
	{
		LCD_write_XY_string(1, 0, hbuff);
		LCD_write_XY_string(1, 6, mbuff);
		LCD_write_XY_string(1, 9, sbuff);
		checker = remains.seconds;
	}
	if(((endhours-nowtime) == 0) || (_num == 0)) 
	{
		washer = sesese;
		chogi_timer();
		water_Flag ++;
	}
}


void setak(uint32_t _num1, uint32_t _num2)
{
	static uint32_t nowtime = 0, hbuff[16] = {0}, mbuff[16] = {0}, sbuff[16] = {0}, sttime = 0;
	static uint8_t hh, mm, ss, setakFlag=0;
	uint8_t endhours;
	endhours = 10*_num2;

	LCD_write_XY_string(0, 0, "   Washing...   ");

	sttime = millis();
	DS1302_GetTime(&remains);
	nowtime = remains.hour*3600 + remains.minutes*60 + remains.seconds;
	hh = (endhours-nowtime)/3600;
	mm = (endhours-nowtime)%3600/60;
	ss = (endhours-nowtime)%60;
	sprintf(hbuff,"   %02d:", hh);
	sprintf(mbuff,"%02d:", mm);
	sprintf(sbuff,"%02d    ", ss);
	
	if(checker != remains.seconds)
	{
		LCD_write_XY_string(1, 0, hbuff);
		LCD_write_XY_string(1, 6, mbuff);
		LCD_write_XY_string(1, 9, sbuff);
		checker = remains.seconds;
	}
	
	if(remains.seconds % 2 == 1)
	{
		setakFlag ++;
		if(setakFlag == 4) setakFlag = 0;
	}
	if(setakFlag == 0) DCmotorright(85*_num1);
	else if(setakFlag == 1) DCmotorleft(45*_num1);
	else if(setakFlag == 2)
	{
		DCmotorstop();
		DCmotorleft(85*_num1);
	}
	else if(setakFlag == 3) DCmotorright(45*_num1);
	if(((endhours-nowtime) == 0) || (_num2 == 0))
	{
		washer = dada;
		chogi_timer();
	}
}

void hang(uint32_t _num)
{
	static int32_t nowtime, hbuff[16] = {0}, mbuff[16] = {0}, sbuff[16] = {0}, sttime = 0, buff[16];
	uint8_t hh, mm, ss, hangFlag=0;
	static uint8_t times = 1, endhours;
	endhours = 10;

	sprintf(buff, "Now Rinse(%d/%d)... ", times, _num);
	LCD_write_XY_string(0, 0, buff);
	PORTD |= hotwater == 0? (1 << 4) : (1 << 5);
	
	sttime = millis();
	DS1302_GetTime(&remains);
	nowtime = remains.hour*3600 + remains.minutes*60 + remains.seconds;
	hh = (endhours-nowtime)/3600;
	mm = (endhours-nowtime)%3600/60;
	ss = (endhours-nowtime)%60;
	sprintf(hbuff,"   %02d:", hh);
	sprintf(mbuff,"%02d:", mm);
	sprintf(sbuff,"%02d    ", ss);
	
	if(checker != remains.seconds)
	{
		LCD_write_XY_string(1, 0, hbuff);
		LCD_write_XY_string(1, 6, mbuff);
		LCD_write_XY_string(1, 9, sbuff);
		checker = remains.seconds;
	}
	
	if(remains.seconds%3 == 1) hangFlag ^= 1;
	_delay_ms(10);
	hangFlag == 1 ? DCmotorright(200) : DCmotorleft(200);
	
	if((times != _num) && ( (endhours - nowtime) == 0) )
	{
		times ++;
		washer = dada;
		chogi_timer();
	}
	else if(((times == _num) && ( (endhours - nowtime) == 0) ) || (_num == 0) )
	{
		times = 1;
		washer = dada;
		daegi_flag = 1;
		chogi_timer();
	}
}

void tal(uint32_t _num)
{
	static int endtime , nowtime = 0, hbuff[16] = {0}, mbuff[16] = {0}, sbuff[16] = {0};
	uint8_t hh, mm, ss;
	
	endtime = 10*_num;
	LCD_write_XY_string(0, 0, "   Dehydrating  ");
	DS1302_GetTime(&remains);
	nowtime = remains.hour*3600 + remains.minutes*60 + remains.seconds;
	hh = (endtime-nowtime)/3600;
	mm = (endtime-nowtime)%3600/60;
	ss = (endtime-nowtime)%60;
	sprintf(hbuff,"   %02d:", hh);
	sprintf(mbuff,"%02d:", mm);
	sprintf(sbuff,"%02d    ", ss);
	DCmotorright(255);
	if(checker != remains.seconds)
	{
		LCD_write_XY_string(1, 0, hbuff);
		LCD_write_XY_string(1, 6, mbuff);
		LCD_write_XY_string(1, 9, sbuff);
		checker = remains.seconds;
	}
	if(((endtime - nowtime)==0) || (_num == 0))
	{
		DCmotorstop();
		LCD_clear();
		washer = watering;
		whole_state = setting;
		main_power = off;
		hotwater = 0;
		LCD_write_XY_string(0, 0, "      end       ");
		bear_sound();
	}
}

void daegi()
{
	DCmotorstop();
	PORTD &= ~(0x30);
	PORTD |= 0x40;
	LCD_write_XY_string(0, 0, " Release Water  ");
	LCD_write_XY_string(1, 0, "wait a minute.. ");
 	if((read_ADC(0) < 10) && (daegi_flag == 0)) 
	{
		washer = watering;
		chogi_timer();
		LCD_clear();
	}
 	if((read_ADC(0) < 10) && (daegi_flag == 1))
	 {
		 washer = talsu;
		 chogi_timer();
		 LCD_clear();
	 }
}  


void run_washer(uint32_t* _mode)
{
	switch (washer)
	{
		case watering:
			waterwatering();
			break;
			
		case bull:
			bulym(_mode[0]);
			break;
		
		case sesese:
			setak(_mode[1], _mode[2]);
			break;
		
		case dada:
			daegi();
			break;
		
		case haha:
			hang(_mode[3]);
			break;
		
		case talsu:
			tal(_mode[4]);
			break;
	}
}


uint8_t* step_setting_menu()
{
	char *menu_name[5] = {"   Standard    ", "   PowerFul   ", " Long Watering  ", "Boiling Washing" ," Custom Washing "};
	char *course_name[6] = {" Soaking Times ", " Washing Power ", " Washing length ", "  Rinse Times  " ," Spin-Dry Times ", " Boiling Water? "};
	uint8_t buff[16];
	uint8_t *pa = custom;
	static int settingmode = 0, k =0, setak_mode = 0, i= 0;

	switch(menu)
	{
		case normal:
		LCD_write_XY_string(0,0, "  Select Mode   ");
		if(button(UP_BUTTON)) settingmode ++;
		if(settingmode > 4) settingmode = 0;
		if(button(DOWN_BUTTON)) settingmode --;
		if(settingmode < 0) settingmode = 4;
		DS1302_Timer()? LCD_write_XY_string(1, 0, "                "): LCD_write_XY_string(1, 0, menu_name[settingmode]);
		if(button(MENU_BUTTON))
		{
			if(settingmode < 4)
			{
				whole_state = yeah;
				k = settingmode;
				settingmode = 0;
				return setting_washer(k);
			}
			else if(settingmode == 4)
			{
				menu = cuscus;
				settingmode = 0;
			}
		}
		break;
	
		case cuscus:
		LCD_write_XY_string(0, 0, course_name[i]);
		if(button(UP_BUTTON)) setak_mode ++;
		if(button(DOWN_BUTTON)) setak_mode --;
		setak_mode = setak_mode > 3 ? 0: setak_mode;
		setak_mode = setak_mode < 0 ? 3: setak_mode;\
		if(i==1)
		{
			sprintf(buff, "  %02d  power  ", setak_mode);
		}
		else if(i < 5) 
		{
			if(i == 3) sprintf(buff, "  %02d  times  ", setak_mode);
			else sprintf(buff, "  %02d  seconds  ", setak_mode *10);
		}
		else if(i > 4)
		{
			setak_mode % 2 == 0? sprintf(buff, "      No        "): sprintf(buff, "      Yes        ");
			hotwater = setak_mode % 2 == 0? 0: 1;
		}
		DS1302_Timer()? LCD_write_XY_string(1, 0, "                "): LCD_write_XY_string(1, 0, buff);
		if(button(MENU_BUTTON))
		{
			if(i < 5)
			{
				custom[i] = setak_mode;
				i++;
				setak_mode = 0;
				LCD_clear();
			}
			else if(i == 5)
			{
				i=0;
				LCD_clear();
				whole_state = yeah;
				menu = 0;
				setak_mode = 0;
				return setting_washer(4);
			}
		}
		break;
	}
	return 0;
}

void yeyakye()
{
	static int resve_Flag = 0, resve_time = 0, hmFlag = 0, set_Flag = 0;
	uint16_t buff[16];
	
	
	switch(set_Flag)
	{
		case 0:
		LCD_write_XY_string(0, 0, "    Reserve?    ");
		resve_Flag == 0 ? LCD_write_XY_string(1, 0, "     No      ") : LCD_write_XY_string(1, 0, "     Yes      ");
		if(button(UP_BUTTON) || button(DOWN_BUTTON)) resve_Flag ^= 1;
		if(button(MENU_BUTTON))
		{
			if(resve_Flag ==1) set_Flag = 1;
			else if(resve_Flag == 0)
			{
				yeyak =0;
				whole_state = run;
			}
		}
		break;
		
		case 1:
		if(hmFlag == 0)
		{
			LCD_write_XY_string(0, 0, " Setting Hour ");
			if(button(UP_BUTTON) && (yeyakhours < 23)) yeyakhours ++;
			if(button(DOWN_BUTTON) && (yeyakhours > 0)) yeyakhours --;
			sprintf(buff, "   %02d:  ", yeyakhours);
			DS1302_Timer()? LCD_write_XY_string(1, 0, "     :    after  ") : LCD_write_XY_string(1, 0, buff);
			if(button(MENU_BUTTON))
			{
				hmFlag = 1;
			}
		}
		else
		{
			LCD_write_XY_string(0, 0, "Setting minutes ");
			if(timer_button(UP_BUTTON) && (yeyakminutes < 59)) yeyakminutes ++;
			if(timer_button(DOWN_BUTTON) && (yeyakminutes > 0)) yeyakminutes --;
			sprintf(buff, "%02d", yeyakminutes);
			DS1302_Timer()? LCD_write_XY_string(1, 5, ":    after  ") : LCD_write_XY_string(1, 6, buff);
			if(button(MENU_BUTTON))
			{
				hmFlag = 0;
				resve_Flag = 0;
				chogi_timer();
				LCD_clear();
				yeyak = 1;
				whole_state = run;
			}
		}
	}
	chogi_timer();
}

void chogi_timer()
{
	remains.hour = 0;
	remains.minutes = 0;
	remains.seconds = 0;
	DS1302_SetTimeDates(remains);
}

void waiting_yeyak()
{
	uint32_t endtime, nowtime, remain, ss, mm, hh, sec[16], min[16], hou[16];
	endtime = yeyakhours*3600+ yeyakminutes*60;
	LCD_write_XY_string(0, 0, "    remains    ");
	DS1302_GetTime(&remains);
	nowtime = remains.hour*3600+remains.minutes*60+remains.seconds;
	remain = endtime - nowtime;
	hh = remain/3600;
	mm = (remain%3600)/60;
	ss = remain%60;
	sprintf(hou,"    %02d:", hh);
	sprintf(min,"%02d:", mm);
	sprintf(sec,"%02d   ", ss);
	LCD_write_XY_string(1, 0, hou);
	LCD_write_XY_string(1, 7, min);
	LCD_write_XY_string(1, 10, sec);
	if(remain == 0)
	{
		yeyak = 0;
		chogi_timer();
		yeyakhours = 0;
		yeyakminutes = 0;
	}
}

void pause_time()
{
	DS1302_GetTime(&pausing);
}

void restart_time()
{
	DS1302_SetTimeDates(pausing);
}

void waterwatering()
{
	LCD_write_XY_string(0,0, "    Watering    ");
	LCD_write_XY_string(1,0, "                ");
	PORTD &= ~(1 << 6);
	if(hotwater == 0) PORTD |= (1 << 4);
	else if(hotwater) PORTD |= (1 << 5);
		if((read_ADC(0) > 570) && (water_Flag == 0) )
	{
		PORTD &= ~(0xf0);
		washer = bull;
		chogi_timer();
	}
	else if((read_ADC(0) > 450)&&(water_Flag == 1))
	{
		washer = haha;
		chogi_timer();
	}
}

void water_Flag_reset()
{
	water_Flag =0;
}

void daegi_flag_reset()
{
	daegi_flag = 0;
}