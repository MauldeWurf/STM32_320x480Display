#include "stm32f4xx.h"
#include "systick.h"
#include "gpio.h"
#include "spi.h"
#include "sbc_lcd01.h"
#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include "spi_dma.h"


int main(void){
	sbc_lcd01_init();
	testScreen_16();
	systick_msec_delay(500);
	while(1){
		//fullScreenColor(COLOR16_WHITE);
		//systick_msec_delay(500);
		fullScreenColor(COLOR16_BLUE);
		systick_msec_sleep(1000);
		fullScreenColor(COLOR16_RED);
		systick_msec_sleep(1000);

		//systick_ms++;
		//systick_msec_delay(500);
	}

}


