#include "stm32f4xx.h"
#include "systick.h"
#include "gpio.h"
#include "spi.h"
#include <stdio.h>
#include <stdint.h>
#include <math.h>

#define GPIOAEN		(1U<<0)

/*#define PIN5		(1U<<5)

#define LED_PIN		PIN5
*/
/*from adafruit_ST7789.cpp*/
#define ST77XX_SWRESET 0x01
#define ST_CMD_DELAY 0x80 // special signifier for command lists
#define ST77XX_SLPOUT 0x11
#define ST77XX_COLMOD 0x3A
#define ST77XX_MADCTL 0x36
#define ST77XX_CASET 0x2A
#define ST77XX_RASET 0x2B
#define ST77XX_INVON 0x21
#define ST77XX_NORON 0x13
#define ST77XX_DISPON 0x29
#define ST7789_DISPON 0x29
#define ST77XX_RAMWR 0x2C
#define ST7789_DISPOFF 0x28


bool btn_state;

uint8_t data = 1;

/*from adafruit_ST7789.cpp*/
static const uint8_t generic_st7789[] ={                // Init commands for 7789 screens
	    9,                              //  9 commands in list:
	    ST77XX_SWRESET,   ST_CMD_DELAY, //  1: Software reset, no args, w/delay
	      150,                          //     ~150 ms delay
	    ST77XX_SLPOUT ,   ST_CMD_DELAY, //  2: Out of sleep mode, no args, w/delay
	      10,                          //      10 ms delay
	    ST77XX_COLMOD , 1+ST_CMD_DELAY, //  3: Set color mode, 1 arg + delay:
	      0x55,                         //     16-bit color
	      10,                           //     10 ms delay
	    ST77XX_MADCTL , 1,              //  4: Mem access ctrl (directions), 1 arg:
	      0x00,                         //     Row/col addr, bottom-top refresh
	    ST77XX_CASET  , 4,              //  5: Column addr set, 4 args, no delay:
	      0x00,
	      0,        //     XSTART = 0
	      0,
	      240,  //     XEND = 240
	    ST77XX_RASET  , 4,              //  6: Row addr set, 4 args, no delay:
	      0x00,
	      0,             //     YSTART = 0
	      239>>8,
	      239&0xFF,  //     YEND = 320
	    ST77XX_INVON  ,   ST_CMD_DELAY,  //  7: hack
	      10,
	    ST77XX_NORON  ,   ST_CMD_DELAY, //  8: Normal display on, no args, w/delay
	      10,                           //     10 ms delay
	    ST77XX_DISPON ,   ST_CMD_DELAY, //  9: Main screen turn on, no args, delay
	      10 };                          //    10 ms delay


void test_output(void){

	GPIOA->BSRR |= (1U<<6);
	systick_msec_delay(19);
	GPIOA->BSRR |= (1U<<22);
	systick_msec_delay(19);
}


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

int main(void){
	data = (uint8_t)200;
	if(0){
	/*Enable clock access to GPIOA*/
	RCC->AHB1ENR |= GPIOAEN;

	/*PA6 output mode 10*/
	GPIOA->MODER |=(1U<<12);
	GPIOA->MODER &=~(1U<<13);
	}

	A0_init();
	A0_off();
	systick_msec_delay(50);
	A0_on();

	while(0){
		test_output();
	}
	systick_msec_delay(5);
	spi_gpio_init();
	systick_msec_delay(5);
	spi1_config();
	systick_msec_delay(5);
	displayInit(generic_st7789);

	uint8_t white[2] = { 0xFF, 0xFF };
	uint8_t blue[2]  = {  0xF8, 0x00};
	uint8_t black[2]  = { 0x00, 0x00 };
	uint8_t red[2]      = { 0x00, 0x1F } ;
	uint8_t orange[2] =  {0xFF, 0xAA};
	systick_msec_delay(500);
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
	while(1){
		data+=10;
		//sendCommand(ST7789_DISPOFF, NULL,0);
		systick_msec_delay(500);
		/*
		sendCommand(ST77XX_CASET, (uint8_t[]){0,0,0,239}, 4);
		sendCommand(ST77XX_RASET, (uint8_t[]){0,0,1,63}, 4);
		sendCommand(ST77XX_RAMWR, NULL, 0);

		tft_dc_high();
		for (uint32_t i=0; i<240*320; i++){
		    spi1_transmit(white,2);
		}
		tft_dc_low();
		*/
		systick_msec_delay(500);
		//sendCommand(ST77XX_DISPON, NULL, 0);
	}
	/*
	RCC->AHB1ENR |= GPIOAEN;

	GPIOA->MODER |= (1U<<10);
	GPIOA->MODER &= ~(1U<<11);

	led_init();
	button_init();
	A0_init();

	while(1)
	{
		led_on();
		A0_on();
		systick_msec_delay(10);
		led_off();
		A0_off();
		systick_msec_delay(20);
		btn_state = get_btn_state();
		while(btn_state){
			btn_state = get_btn_state();
		}

	}*/
}
