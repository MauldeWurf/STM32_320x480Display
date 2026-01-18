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
	static const char oneWord[]="HALLO JESSICA";
	static const char secondWord[] = "GLUECKWUNSCH";
	while(1){
		writeWord(oneWord,220,160,COLOR16_RED,COLOR16_WHITE);
		writeWord(secondWord,220,120,COLOR16_BLACK,COLOR16_WHITE);
		systick_msec_sleep(50);

	}

}


