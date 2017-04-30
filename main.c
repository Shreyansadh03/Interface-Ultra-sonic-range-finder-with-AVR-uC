/*
 * UltraSonic.c
 *
 * Created: 8/27/2016 2:09:08 PM
 * Author : SAMSUNG
 */ 
 /*theory used
 clock frequency=8MHz
 after prescaling by 8 frequency is=1MHz
 speed of sound=343m/s or 58.31us/cm
 clock ticks once in 1us
 */

#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
#include "lcd.h"

void begin();
int pulse();

int main(void)
{
	DDRA=0b00000001;
	
	int p=0;
	float dis=0.0;
	char dist[15];
	begin();
	LCDWriteStringXY(5,0,"SONAR");
	LCDWriteStringXY(5,1,"TEST%3");
	_delay_ms(1500);
	LCDClear();
	_delay_ms(100);
	while(1)
	{
		PORTA|=0b00000001;
		_delay_us(12);
		PORTA&=0b00000000;
		_delay_us(15);
		
		p=pulse();
		if(p==0)
		{
			LCDClear();
			LCDWriteStringXY(0,0,"CLEAR!");
			_delay_ms(100);
		}
		else
		{
			dis=((p/58.31));
			sprintf(dist,"TARGET=%1.2f",dis);
			LCDWriteStringXY(0,0,dist);
			LCDWriteString("CM");
			_delay_ms(200);
		}
		
	}
	return 0;
}
void begin()
{
	LCDInit(LS_NONE);
	LCDClear();
}
int pulse()
{
	uint32_t t=24000,pul=0;
	while(t--)
	{
		if(!(PINA&0x08))
		continue;
		else
		break;
	}
	if(t==0)
	return 0;
	TCCR1B=0x02;
	TCNT1=0;
	while(PINA&0x08);
	pul=TCNT1;
	TCCR1B=0x00;
	if(pul>24000)
	return 0;
	else
	return (pul);
}

