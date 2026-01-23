/*
 * myGFX.c
 *
 *  Created on: Jan 12, 2026
 *      Author: stevo
 */

#include "myGFX.h"
#include "sbc_lcd01.h"

uint16_t localBuffer[8][8];
uint16_t singleColor = 0xFFFFF;
uint16_t* singleBuffer = &singleColor;

const uint8_t MAXWORDLENGTH = 25;

typedef struct {
	bool doubleSized;
	uint16_t color;
	uint16_t bgColor;
} textOptions;

typedef struct {
	uint32_t rows[2];
} Glyph;

static const Glyph font[] = {
		['A'] = { .rows ={0x3C3C667E, 0x7E666666} },
		['B'] = { .rows ={0x7C7C667E, 0x7C667E7C} },
		['C'] = { .rows ={0x3C3C6666, 0x60667E3C} },
		['D'] = { .rows ={0x7C7C6666, 0x66667E7C} },
		['E'] = { .rows ={0x7E7E607C, 0x7C607E7E} },
		['F'] = { .rows ={0x7E7E607C, 0x7C606060} },
		['G'] = { .rows ={0x3E3E606E, 0x6E667E3E} },
		['H'] = { .rows ={0x6666667E, 0x7E666666} },
		['I'] = { .rows ={0x60606060, 0x60606060} },
		['J'] = { .rows ={0x0C0C0C0C, 0x6C6C3030} },
		['K'] = { .rows ={0x66666C7C, 0x706C6E66} },
		['L'] = { .rows ={0x60606060, 0x60607C7C} },
		['M'] = { .rows ={0x6161777F, 0x6D616161} },
		['N'] = { .rows ={0x6666767E, 0x6E666666} },
		['O'] = { .rows ={0x3C3C6666, 0x66667E3C} },
		['P'] = { .rows ={0x7C7C667E, 0x7C606060} },
		['Q'] = { .rows ={0x3C3C6666, 0x666C7E36} },
		['R'] = { .rows ={0x7C7C667E, 0x7C666666} },
		['S'] = { .rows ={0x3E3E607C, 0x3C067E7C} },
		['T'] = { .rows ={0x7C7C3030, 0x30303030} },
		['U'] = { .rows ={0x66666666, 0x66663C3C} },
		['V'] = { .rows ={0x6C6C6C6C, 0x6C303030} },
		['W'] = { .rows ={0x6D6D6D6D, 0x6D363636} },
		['X'] = { .rows ={0x6C6C6C30, 0x306C6C6C} },
		['Y'] = { .rows ={0x6C6C6C30, 0x30303030} },
		['Z'] = { .rows ={0x7C7C0C3C, 0x30607C7C} },
		['0'] = { .rows ={0x30306C6C, 0x6C6C7C30} },
		['1'] = { .rows ={0x30307070, 0x30303030} },
		['2'] = { .rows ={0x70700C3C, 0x30607C7C} },
		['3'] = { .rows ={0x70700C3C, 0x300C7C70} },
		['4'] = { .rows ={0x0C0C3C7C, 0x6C7C7C0C} },
		['5'] = { .rows ={0x7C7C6070, 0x700C7C70} },
		['6'] = { .rows ={0x3C3C6070, 0x706C7C30} },
		['7'] = { .rows ={0x7C7C0C30, 0x30303030} },
		['8'] = { .rows ={0x30306C7C, 0x306C7C30} },
		['9'] = { .rows ={0x30306C7C, 0x3C0C7C70} },
		['!'] = { .rows ={0x60606060, 0x60006060} },
		['u'] = { .rows ={0x6C006C6C, 0x6C6C7C3C}},
};

// sine_table: 0..255 corresponds to sin(x)*255
static const uint8_t sine_table[] = {0,4,8,13,17,22,26,31,35,39,44,48,53,57,61,66,70,74,78,83,87,91,95,99,103,107,111,115,119,123,127,131,135,138,142,146,149,153,156,160,163,167,170,173,177,180,183,186,189,192,195,198,200,203,206,208,211,213,216,218,220,223,225,227,229,231,232,234,236,238,239,241,242,243,245,246,247,248,249,250,251,251,252,253,253,254,254,254,254,254,255,};

static textOptions TEXT_OPT ={1,COLOR16_BLACK,COLOR16_WHITE};

void textInit(bool doubleSize, uint16_t color, uint16_t backgroundColor){
	TEXT_OPT.doubleSized = doubleSize;
	TEXT_OPT.color = color;
	TEXT_OPT.bgColor = backgroundColor;
}


void rectangle( uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t color){
	*singleBuffer=color;
	fillRectangle_oneColor(singleBuffer,x,y,width,height);
}

void rectangle_empty(uint16_t x, uint16_t y, uint16_t width, uint16_t height,uint8_t thickness, uint16_t color){
	*singleBuffer=color;
	uint8_t pixelNo[2]={width*thickness,height*thickness};
	fillRectangle_oneColor(singleBuffer,x,y,width,thickness);
	fillRectangle_oneColor(singleBuffer,x,y+height,width+thickness,thickness);
	fillRectangle_oneColor(singleBuffer,x,y,thickness,width);
	fillRectangle_oneColor(singleBuffer,x+width,y,thickness, width);


}

void writeLetter(char letter, uint16_t x, uint16_t y,uint16_t color,uint16_t background){
	const Glyph *g = &font[(uint8_t)letter];
	uint8_t letterHeight = 8;
	uint8_t letterWidth = 8;
	initAdressWindow(x, y, letterWidth, letterHeight);
    for (uint8_t i = 0 ;i<4;i++){
    	for(uint8_t j=0; j<8;j++){
    			//fist half of letter
    			localBuffer[i][j] = (uint16_t)((g->rows[1]>>(i*8+j)) & 0x00000001) *color;
    			//second half of letter
    			localBuffer[4+i][j] = (uint16_t)((g->rows[0]>>(i*8+j)) & 0x00000001) *color;
    			//background
    			if(!((g->rows[0]>>(i*8+j)) & 0x00000001)) localBuffer[4+i][j]=background;
    			if(!((g->rows[1]>>(i*8+j)) & 0x00000001)) localBuffer[i][j]=background;
    	}
    }
	if (TEXT_OPT.doubleSized) fillSquare_scaleup(localBuffer,x,y,letterHeight);
	else fillRectangle(localBuffer,x,y,letterHeight,letterWidth);
}

void writeWord(const char *word, uint16_t x, uint16_t y,uint16_t color){
		uint8_t spacing = 15;
		int16_t cursor = -spacing;
		for(uint8_t i=0 ; word[i] !='\0'; i++){
			switch(word[i-1]){
			case('I'):
				cursor += (spacing-8);
				break;
			case('L'):
					cursor += (spacing-2);
					break;
			default:
			cursor += spacing;
			}
			writeLetter((char)word[i],x-cursor,y,color,TEXT_OPT.bgColor);

			if (i==MAXWORDLENGTH) break;
		}
}

int16_t sin_deg(int16_t x){
	//NOTE: -720<x<720 is required outside undefined
	// return values -255...+255
	int8_t sign=1;
	int16_t output=0;

	//check for negative x, fold back to [0,90) with right sign
	if (x<0) {
		sign = -sign;
		x =-x;
	}
	// accept 720° range,
	if(x>=360) x= x-360;

	//set output to correspondig value of sine_table
	// sine_table: 0..255 corresponds to sin(x)*255
	if (0 <= x && x <= 90) output = sine_table[x];
	if (90 < x && x <= 180) output = sine_table[-x+180];
	if (180 < x && x <= 270) output = -sine_table[x-180];
	if (270 < x && x <= 360) output = -(sine_table[-x+360]);
	return sign * output;
}

int16_t cos_deg(int16_t x){
	//NOTE: -720<x<720 is required outside undefined
	// return values -255...+255
	// accept 720° range
	if(x>=360) x= x-360;
	//NOTE: -360<x<360 is required
	return sin_deg(x-90);
}

void drawCircle(uint8_t x, uint8_t y, uint8_t d,uint8_t thickness , uint16_t color){
	for (uint16_t deg=0;deg <= 360; deg+=5){
		rectangle(x+sin_deg(deg)*d/255,y+cos_deg(deg)*d/255,thickness,thickness,color);
	}
	return;
}

void drawCircle_part(uint8_t x, uint8_t y, uint8_t d,uint8_t thickness ,int8_t phi_start ,int8_t phi_stop, uint16_t color){
	for (int16_t deg=phi_start;deg <= phi_stop; deg+=5){
		rectangle(x+sin_deg(deg)*d/255,y+cos_deg(deg)*d/255,thickness,thickness,color);
	}
	return;
}

