#include "stm32f4xx.h"
#include "systick.h"
#include "gpio.h"
#include "spi.h"
#include "sbc_lcd01.h"
#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include "spi_dma.h"
#include "myGFX.h"



int main(void){
	sbc_lcd01_init();
	testScreen_16();
	systick_msec_delay(500);
	fullScreenColor(COLOR16_WHITE);
	static const char oneWord[]="HUHU DU !!! ";
	static const char secondWord[] = " LIEBE GRuSSE!";
	//rectangle(100,10,10,10,COLOR16_RED);
	int16_t pos=0;
	textInit(1,COLOR16_WHITE,COLOR16_WHITE);
	writeWord(secondWord,220,120,COLOR16_RED);
	systick_msec_sleep(150);
	rectangle(0,30,240,10,COLOR16_RED);
	textInit(1,COLOR16_WHITE,COLOR16_BLACK);
	rectangle(0,157,240,21,COLOR16_BLACK);
	rectangle_empty(0,0,230,230,10,COLOR16_BLUE);
	uint16_t angled = 2;

	fullScreenColor(COLOR16_WHITE);
	/*for (uint16_t i=0;i<240;i+=1){
		rectangle(i,90+((sin_deg(i*4))>>2),3,3,COLOR16_GREEN);
			rectangle(i,90+((sin_deg(-i*4))>>2),3,3,COLOR16_LIGHTBLUE);
			rectangle(i,90+((cos_deg(i*4))>>2),3,3,COLOR16_RED);
		}*/
	for (uint8_t d=5;d<30;d+=7)	drawCircle(150+d, 100, d,3 , COLOR16_RED);
	for (uint8_t d=5;d<30;d+=7)	drawCircle(120-d, 100, d,3 , COLOR16_BLUE);
	drawCircle(132,65,10,7,COLOR16_RED);
	drawCircle_part(132, 40, 26, 7, 180, 359, COLOR16_BLUE);
	while(1){
		writeWord(oneWord,pos,160,COLOR16_GREEN);
		rectangle_empty(0,0,230,230,10,COLOR16_BLUE);
		systick_msec_sleep(5);

		pos =(pos+1)%450;
	}

}


