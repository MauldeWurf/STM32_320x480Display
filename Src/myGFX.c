/*
 * myGFX.c
 *
 *  Created on: Jan 12, 2026
 *      Author: stevo
 */

#include "myGFX.h"
#include "sbc_lcd01.h"

uint16_t localBuffer[25];

uint8_t letter_A[5][5]={{1,1,1,1,0},
						{1,0,0,0,1},
						{1,1,1,1,1},
						{1,0,0,0,1},
						{1,0,0,0,1},
						{1,0,0,0,1}};

void rectangle( uint16_t line, uint16_t row, uint16_t width, uint16_t height, uint16_t color){
	initAdressWindow(line, row, width, height);
	uint16_t pixelNo = width*height;
	for (uint16_t i=0; i < pixelNo ;i++){
		windowBuffer[i]=color;
	}
	fillRectangle(localBuffer,pixelNo); //Das ist total inkonsequent, weil hier der windowBuffer aus der sbc_lcd01 direkt benutzt wird - aber effizent ist es


}

void letter( uint16_t x, uint16_t y,uint16_t color,uint16_t background){
	//fehler wenn vordergrund schwarz =0
	uint8_t letterHeight = 5;
	uint8_t letterWidth = 5;
	initAdressWindow(x, y, letterHeight, letterWidth);
	for(uint8_t i=0;i<letterWidth;i++){
		for(uint8_t j=0;j<letterHeight;j++){
			localBuffer[(i*letterWidth)+j]= (uint16_t)letter_A[letterHeight-i-1][j]*color; //Höhe und breite sind hier noch falsch !!!
			if(localBuffer[(i*letterWidth)+j]==0) localBuffer[(i*letterWidth)+j]=background;
	}}

	fillRectangle(localBuffer,sizeof(localBuffer)/sizeof(localBuffer[0])); //inefficient
}

