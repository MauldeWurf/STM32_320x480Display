/*
 * sbc_lcd01.c
 *
 *  Created on: Jan 3, 2026
 *      Author: stevo
 */
#include "spi.h"
#include "sbc_lcd01.h"
#include <stdint.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>
         //    10 ms delay

uint8_t white[2] = { 0xFF, 0xFF };
uint8_t blue[2]  = { 0x00, 0x1F };
uint8_t black[2]  = { 0x00, 0x00 };
uint8_t red[2]      = {  0xF8, 0x00} ;
uint8_t orange[2] =  {0xFF, 0xAA};



void sendCommand(uint8_t commandByte, const uint8_t *dataBytes,
                                  uint8_t numDataBytes) {
	cs_enable();
	tft_dc_low();
	spi1_transmit(&commandByte,1); // Send the command byte
	tft_dc_high();
	if (numDataBytes >0 && dataBytes!=NULL){
		spi1_transmit(dataBytes,numDataBytes);
	}
	cs_disable();

}

/*from adafruit_ST77cc.cpp*/
void displayInit(const uint8_t *addr) {

  uint8_t numCommands, cmd, numArgs;
  uint16_t ms;

  numCommands = *addr++; // Number of commands to follow
  while (numCommands--) {              // For each command...
    cmd = *addr++;       // Read command
    numArgs = *addr++;   // Number of args to follow
    ms = numArgs & ST_CMD_DELAY;       // If hibit set, delay follows args
    numArgs &= ~ST_CMD_DELAY;          // Mask out delay bit
    //spi1_transmit(cmd,1);
    sendCommand(cmd, addr, numArgs);
    addr += numArgs;

    if (ms) {
      ms = *addr++; // Read post-command delay time (ms)
      if (ms == 255)
        ms = 500; // If 255, delay for 500 ms
      systick_msec_delay(ms);
    }
  }
}

void cs_enable(void)
{
	GPIOA->ODR &=~(1U<<11);

}

/*Pull high to disable*/
void cs_disable(void)
{
	GPIOA->ODR |=(1U<<11);
}

void tft_dc_low(void){
	GPIOA->ODR &=~(1U<<9);//???
}
void tft_dc_high(void){
	GPIOA->ODR |=(1U<<9);//???
}

void sbc_lcd01_init(){
		A0_init();
		A0_off();
		systick_msec_delay(50);
		A0_on();

		systick_msec_delay(5);
		spi_gpio_init();
		systick_msec_delay(5);
		spi1_config();
		systick_msec_delay(5);
		displayInit(generic_st7789);
	}

void fullScreenColor(uint8_t color){
	sendCommand(ST77XX_RAMWR, NULL, 0);
			tft_dc_high();
			for (uint32_t i=0; i<240*320; i++){
					spi1_transmit(white,2);
				}
			tft_dc_low();
}

void testScreen(void){
	sendCommand(ST77XX_CASET, (uint8_t[]){0,0,0,240}, 4);
	sendCommand(ST77XX_RASET, (uint8_t[]){0,0,1,63}, 4);
	sendCommand(ST77XX_RAMWR, NULL, 0);

			tft_dc_high();
			for (uint32_t i=0; i<240*320; i++){
				int j=floor(i/(40*240));
				switch(j){
				case 0:
					spi1_transmit(white,2);
					break;
				case 1:
					spi1_transmit(blue,2);
					break;
				case 2:
					spi1_transmit(red,2);
					break;
				case 3:
					spi1_transmit(black,2);
					break;
				case 4:
					spi1_transmit(orange,2);
					break;
				default:
					spi1_transmit(white,2);
				}
			}

			tft_dc_low();
}
