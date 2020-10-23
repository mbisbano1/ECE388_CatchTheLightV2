#include <atmel_start.h>
#include "touch.h"
extern volatile uint8_t measurement_done_touch;

/*
Static Declarations and Definitions
*/

#define NumberTest 1

#define NUMLED 25
#define NUMLED_0 22
#define NUMLED_1 7
#define NUMLED_2 17
#define NUMLED_3 17
#define NUMLED_4 15
#define NUMLED_5 17
#define NUMLED_6 21
#define NUMLED_7 11
#define NUMLED_8 25
#define NUMLED_9 21

static uint8_t curLed;
#define F_CPU 8000000
#include <util/delay.h>
#define	BMASK  0b11111111
#define	CMASK  0b00111111
#define	DMASK  0b11111111
#define	EMASK  0b00000111
static uint8_t cScaler;
#define TARGETLED 2

static uint8_t score;
static uint8_t ledMode;
#define NORMALMODE	1
#define PAUSEDMODE	2
#define DISPLAY0	3
#define DISPLAY1	4
#define DISPLAY2	5
#define DISPLAY3	6
#define DISPLAY4	7
#define DISPLAY5	8
#define DISPLAY6	9
#define DISPLAY7	10
#define DISPLAY8	11
#define DISPLAY9	12
static uint16_t delayScaler;
static uint8_t skipFlag = 0;



//static uint8_t	LED_BANKB_TAB[] = {	0xFE,	0xFD,	0xFB,	0xF7,	0xEF,	0xDF,	0xFF,	0xFF};
const uint8_t	LED_BANKB_TAB[] = {	(BMASK&~(0b1<<0)),	(BMASK&~(0b1<<1)),	(BMASK&~(0b1<<2)),	(BMASK&~(0b1<<3)),	(BMASK&~(0b1<<4)),	(BMASK&~(0b1<<5)),	(BMASK&~(0b1<<6)),	(BMASK&~(0b1<<7))};

//static uint8_t	LED_BANKC_TAB[] = {	0xFE,	0xFD,	0xFB,	0xF7,	0xEF,	0xDF,	0xFF,	0xFF};
const uint8_t	LED_BANKC_TAB[] = {	(CMASK&~(0b1<<0)),	(CMASK&~(0b1<<1)),	(CMASK&~(0b1<<2)),	(CMASK&~(0b1<<3)),	(CMASK&~(0b1<<4)),	(CMASK&~(0b1<<5)),	(CMASK&~(0b1<<6)),	(CMASK&~(0b1<<7))};
	
//curLedVal							0		1		2		3		4		5		6		7
const uint8_t	LED_BANKD_TAB[] = {	(DMASK&~(0b1<<0)),	(DMASK&~(0b1<<1)),	(DMASK&~(0b1<<2)),	(DMASK&~(0b1<<3)),	(DMASK&~(0b1<<4)),	(DMASK&~(0b1<<5)),	(DMASK&~(0b1<<6)),	(DMASK&~(0b1<<7))};
	
//static uint8_t	LED_BANKE_TAB[] = {	0xFE,	0xFD,	0xFB,	0xF7,	0xFF,	0xFF,	0xFF,	0xFF};
const uint8_t	LED_BANKE_TAB[] = {	(EMASK&~(0b1<<0)),	(EMASK&~(0b1<<1)),	(EMASK&~(0b1<<2)),	(EMASK&~(0b1<<3)),	(EMASK&~(0b1<<4)),	(EMASK&~(0b1<<5)),	(EMASK&~(0b1<<6)),	(EMASK&~(0b1<<7))};


void ledIntSetup(void)
{
	OCR2B = 0xFF;				//Set Timer 0 INT B Compare Register value.
}

void ledIntEnable(void)
{
	TIMSK0 |= (0b1<<OCIE0B);	//Enable Timer 0 INT B
}

void ledIntDisable(void)
{
	TIMSK0 &= ~(0b1<<OCIE0B);	//Disable Timer 0 INT B
}

void init(void)
{
	DDRD |= DMASK;
	DDRB |= BMASK;
	DDRC |= CMASK;
	DDRE |= EMASK;
	
	PORTD |= DMASK;
	PORTB |= BMASK;
	PORTC |= CMASK;
	PORTE |= EMASK;
	
	curLed = 0;
	score = 0;
	ledMode = NORMALMODE;
}

void ClearLed(void)
{
		PORTD |= DMASK;
		PORTB |= BMASK;
		PORTC |= CMASK;
		PORTE |= EMASK;
}

void NextLed(void)
{	
	if(6 > curLed)
	{
		PORTB = BMASK;
		PORTC = CMASK&LED_BANKC_TAB[curLed];
		PORTD = DMASK;
		PORTE = EMASK;
	}
	if( (9 > curLed) && (6 <= curLed))	// 8 + 6
	{
		PORTB = BMASK;
		PORTC = CMASK;
		PORTD = DMASK;
		PORTE = EMASK&LED_BANKE_TAB[curLed - 6];
	}
	if( (17 > curLed) && (9 <= curLed))
	{
		PORTB = BMASK;
		PORTC = CMASK;
		PORTD = DMASK&LED_BANKD_TAB[curLed - 9];
		PORTE = EMASK;
	}
	if( (25 > curLed) && (17 <= curLed))
	{
		PORTB = BMASK&LED_BANKB_TAB[curLed - 17];
		PORTC = CMASK;
		PORTD = DMASK;
		PORTE = EMASK;		
	}
	curLed = ((curLed + 1)%NUMLED);// + ((curLed + 1)/NUMLED);
}
// Segments A - G
//	A: D1 - D3
//	B: D4 - D7
//	C: D8 -D11
//	D: D12-D14
//	E: D15-D18
//	F: D19-D22
//	G: D23-D25
void Next0(void)	//A B C D E F
{
	if(3 > curLed)		//A
	{
		PORTB = BMASK;
		PORTC = CMASK&LED_BANKC_TAB[curLed];
		PORTD = DMASK;
		PORTE = EMASK;
	}
	
	if((6 > curLed)&&(3 <= curLed))	//B
	{
		PORTB = BMASK;
		PORTC = CMASK&LED_BANKC_TAB[curLed];
		PORTD = DMASK;
		PORTE = EMASK;
	}
	if((7 > curLed)&&(6 <= curLed))	//B
	{
		PORTB = BMASK;
		PORTC = CMASK;
		PORTD = DMASK;
		PORTE = EMASK&LED_BANKE_TAB[curLed - 6];
	}
	
	if((9> curLed)&&(7 <= curLed)) //C
	{
		PORTB = BMASK;
		PORTC = CMASK;
		PORTD = DMASK;
		PORTE = EMASK&LED_BANKE_TAB[curLed - 6];
	}
	if( (11 > curLed) && (9 <= curLed))	//C
	{
		PORTB = BMASK;
		PORTC = CMASK;
		PORTD = DMASK&LED_BANKD_TAB[curLed - 9];
		PORTE = EMASK;
	}
	
	if( (14 > curLed) && (11 <= curLed))	//D
	{
		PORTB = BMASK;
		PORTC = CMASK;
		PORTD = DMASK&LED_BANKD_TAB[curLed - 9];
		PORTE = EMASK;
	}
	
	if( (17 > curLed) && (14 <= curLed))	//E
	{
		PORTB = BMASK;
		PORTC = CMASK;
		PORTD = DMASK&LED_BANKD_TAB[curLed - 9];
		PORTE = EMASK;
	}
	if( (18 > curLed) && (17 <= curLed))	//E
	{
		PORTB = BMASK&LED_BANKB_TAB[curLed - 17];
		PORTC = CMASK;
		PORTD = DMASK;
		PORTE = EMASK;
	}
	
	if( (22 > curLed) && (18 <= curLed))	//F
	{
		PORTB = BMASK&LED_BANKB_TAB[curLed - 17];
		PORTC = CMASK;
		PORTD = DMASK;
		PORTE = EMASK;
	}
	
	curLed = ((curLed + 1)%NUMLED);// + ((curLed + 1)/NUMLED);
}

void Next1(void)	//B C
{
	
	if((6 > curLed)&&(3 <= curLed))	//B
	{
		PORTB = BMASK;
		PORTC = CMASK&LED_BANKC_TAB[curLed];
		PORTD = DMASK;
		PORTE = EMASK;
	}
	if((7 > curLed)&&(6 <= curLed))	//B
	{
		PORTB = BMASK;
		PORTC = CMASK;
		PORTD = DMASK;
		PORTE = EMASK&LED_BANKE_TAB[curLed - 6];
	}
	if((9> curLed)&&(7 <= curLed)) //C
	{
		PORTB = BMASK;
		PORTC = CMASK;
		PORTD = DMASK;
		PORTE = EMASK&LED_BANKE_TAB[curLed - 6];
	}
	if( (11 > curLed) && (9 <= curLed))	//C
	{
		PORTB = BMASK;
		PORTC = CMASK;
		PORTD = DMASK&LED_BANKD_TAB[curLed - 9];
		PORTE = EMASK;
	}
	
	
	
	curLed = ((curLed + 1)%NUMLED);// + ((curLed + 1)/NUMLED);
}

void Next2(void)	//A B D E G
{
	if(3 > curLed)		//A
	{
		PORTB = BMASK;
		PORTC = CMASK&LED_BANKC_TAB[curLed];
		PORTD = DMASK;
		PORTE = EMASK;
	}
	
	if((6 > curLed)&&(3 <= curLed))	//B
	{
		PORTB = BMASK;
		PORTC = CMASK&LED_BANKC_TAB[curLed];
		PORTD = DMASK;
		PORTE = EMASK;
	}
	if((7 > curLed)&&(6 <= curLed))	//B
	{
		PORTB = BMASK;
		PORTC = CMASK;
		PORTD = DMASK;
		PORTE = EMASK&LED_BANKE_TAB[curLed - 6];
	}
	
	if( (14 > curLed) && (11 <= curLed))	//D
	{
		PORTB = BMASK;
		PORTC = CMASK;
		PORTD = DMASK&LED_BANKD_TAB[curLed - 9];
		PORTE = EMASK;
	}
	
	if( (17 > curLed) && (14 <= curLed))	//E
	{
		PORTB = BMASK;
		PORTC = CMASK;
		PORTD = DMASK&LED_BANKD_TAB[curLed - 9];
		PORTE = EMASK;
	}
	if( (18 > curLed) && (17 <= curLed))	//E
	{
		PORTB = BMASK&LED_BANKB_TAB[curLed - 17];
		PORTC = CMASK;
		PORTD = DMASK;
		PORTE = EMASK;
	}
	
	if( (25 > curLed) && (22 <= curLed))	//G
	{
		PORTB = BMASK&LED_BANKB_TAB[curLed - 17];
		PORTC = CMASK;
		PORTD = DMASK;
		PORTE = EMASK;
	}
	curLed = ((curLed + 1)%NUMLED);// + ((curLed + 1)/NUMLED);
}

void Next3(void)	//A B C D G
{
	if(3 > curLed)		//A
	{
		PORTB = BMASK;
		PORTC = CMASK&LED_BANKC_TAB[curLed];
		PORTD = DMASK;
		PORTE = EMASK;
	}
	
	if((6 > curLed)&&(3 <= curLed))	//B
	{
		PORTB = BMASK;
		PORTC = CMASK&LED_BANKC_TAB[curLed];
		PORTD = DMASK;
		PORTE = EMASK;
	}
	if((7 > curLed)&&(6 <= curLed))	//B
	{
		PORTB = BMASK;
		PORTC = CMASK;
		PORTD = DMASK;
		PORTE = EMASK&LED_BANKE_TAB[curLed - 6];
	}
	
	if((9> curLed)&&(7 <= curLed)) //C
	{
		PORTB = BMASK;
		PORTC = CMASK;
		PORTD = DMASK;
		PORTE = EMASK&LED_BANKE_TAB[curLed - 6];
	}
	if( (11 > curLed) && (9 <= curLed))	//C
	{
		PORTB = BMASK;
		PORTC = CMASK;
		PORTD = DMASK&LED_BANKD_TAB[curLed - 9];
		PORTE = EMASK;
	}
	
	if( (14 > curLed) && (11 <= curLed))	//D
	{
		PORTB = BMASK;
		PORTC = CMASK;
		PORTD = DMASK&LED_BANKD_TAB[curLed - 9];
		PORTE = EMASK;
	}
	
	if( (25 > curLed) && (22 <= curLed))	//G
	{
		PORTB = BMASK&LED_BANKB_TAB[curLed - 17];
		PORTC = CMASK;
		PORTD = DMASK;
		PORTE = EMASK;
	}
	
	
	curLed = ((curLed + 1)%NUMLED);// + ((curLed + 1)/NUMLED);
}

void Next4(void)	//B C F G
{
	if((6 > curLed)&&(3 <= curLed))	//B
	{
		PORTB = BMASK;
		PORTC = CMASK&LED_BANKC_TAB[curLed];
		PORTD = DMASK;
		PORTE = EMASK;
	}
	if((7 > curLed)&&(6 <= curLed))	//B
	{
		PORTB = BMASK;
		PORTC = CMASK;
		PORTD = DMASK;
		PORTE = EMASK&LED_BANKE_TAB[curLed - 6];
	}
	
	if((9> curLed)&&(7 <= curLed)) //C
	{
		PORTB = BMASK;
		PORTC = CMASK;
		PORTD = DMASK;
		PORTE = EMASK&LED_BANKE_TAB[curLed - 6];
	}
	if( (11 > curLed) && (9 <= curLed))	//C
	{
		PORTB = BMASK;
		PORTC = CMASK;
		PORTD = DMASK&LED_BANKD_TAB[curLed - 9];
		PORTE = EMASK;
	}
	
	if( (22 > curLed) && (18 <= curLed))	//F
	{
		PORTB = BMASK&LED_BANKB_TAB[curLed - 17];
		PORTC = CMASK;
		PORTD = DMASK;
		PORTE = EMASK;
	}
	
	if( (25 > curLed) && (22 <= curLed))	//G
	{
		PORTB = BMASK&LED_BANKB_TAB[curLed - 17];
		PORTC = CMASK;
		PORTD = DMASK;
		PORTE = EMASK;
	}
	
	curLed = ((curLed + 1)%NUMLED);// + ((curLed + 1)/NUMLED);
}

void Next5(void)	//A C D F G
{
	if(3 > curLed)		//A
	{
		PORTB = BMASK;
		PORTC = CMASK&LED_BANKC_TAB[curLed];
		PORTD = DMASK;
		PORTE = EMASK;
	}
	
	if((9> curLed)&&(7 <= curLed)) //C
	{
		PORTB = BMASK;
		PORTC = CMASK;
		PORTD = DMASK;
		PORTE = EMASK&LED_BANKE_TAB[curLed - 6];
	}
	if( (11 > curLed) && (9 <= curLed))	//C
	{
		PORTB = BMASK;
		PORTC = CMASK;
		PORTD = DMASK&LED_BANKD_TAB[curLed - 9];
		PORTE = EMASK;
	}
	
	if( (14 > curLed) && (11 <= curLed))	//D
	{
		PORTB = BMASK;
		PORTC = CMASK;
		PORTD = DMASK&LED_BANKD_TAB[curLed - 9];
		PORTE = EMASK;
	}
	
	if( (22 > curLed) && (18 <= curLed))	//F
	{
		PORTB = BMASK&LED_BANKB_TAB[curLed - 17];
		PORTC = CMASK;
		PORTD = DMASK;
		PORTE = EMASK;
	}
	
	if( (25 > curLed) && (22 <= curLed))	//G
	{
		PORTB = BMASK&LED_BANKB_TAB[curLed - 17];
		PORTC = CMASK;
		PORTD = DMASK;
		PORTE = EMASK;
	}
	
	curLed = ((curLed + 1)%NUMLED);// + ((curLed + 1)/NUMLED);
}

void Next6(void)	//A C D E F G
{
	
	if(3 > curLed)		//A
	{
		PORTB = BMASK;
		PORTC = CMASK&LED_BANKC_TAB[curLed];
		PORTD = DMASK;
		PORTE = EMASK;
	}
	
	if((9> curLed)&&(7 <= curLed)) //C
	{
		PORTB = BMASK;
		PORTC = CMASK;
		PORTD = DMASK;
		PORTE = EMASK&LED_BANKE_TAB[curLed - 6];
	}
	if( (11 > curLed) && (9 <= curLed))	//C
	{
		PORTB = BMASK;
		PORTC = CMASK;
		PORTD = DMASK&LED_BANKD_TAB[curLed - 9];
		PORTE = EMASK;
	}
	
	if( (14 > curLed) && (11 <= curLed))	//D
	{
		PORTB = BMASK;
		PORTC = CMASK;
		PORTD = DMASK&LED_BANKD_TAB[curLed - 9];
		PORTE = EMASK;
	}
	
	if( (17 > curLed) && (14 <= curLed))	//E
	{
		PORTB = BMASK;
		PORTC = CMASK;
		PORTD = DMASK&LED_BANKD_TAB[curLed - 9];
		PORTE = EMASK;
	}
	if( (18 > curLed) && (17 <= curLed))	//E
	{
		PORTB = BMASK&LED_BANKB_TAB[curLed - 17];
		PORTC = CMASK;
		PORTD = DMASK;
		PORTE = EMASK;
	}
	
	if( (22 > curLed) && (18 <= curLed))	//F
	{
		PORTB = BMASK&LED_BANKB_TAB[curLed - 17];
		PORTC = CMASK;
		PORTD = DMASK;
		PORTE = EMASK;
	}
	
	if( (25 > curLed) && (22 <= curLed))	//G
	{
		PORTB = BMASK&LED_BANKB_TAB[curLed - 17];
		PORTC = CMASK;
		PORTD = DMASK;
		PORTE = EMASK;
	}
	
	curLed = ((curLed + 1)%NUMLED);// + ((curLed + 1)/NUMLED);
}

void Next7(void)	//A B C
{
	
	if(3 > curLed)		//A
	{
		PORTB = BMASK;
		PORTC = CMASK&LED_BANKC_TAB[curLed];
		PORTD = DMASK;
		PORTE = EMASK;
	}
	
	if((6 > curLed)&&(3 <= curLed))	//B
	{
		PORTB = BMASK;
		PORTC = CMASK&LED_BANKC_TAB[curLed];
		PORTD = DMASK;
		PORTE = EMASK;
	}
	if((7 > curLed)&&(6 <= curLed))	//B
	{
		PORTB = BMASK;
		PORTC = CMASK;
		PORTD = DMASK;
		PORTE = EMASK&LED_BANKE_TAB[curLed - 6];
	}
	
	if((9> curLed)&&(7 <= curLed)) //C
	{
		PORTB = BMASK;
		PORTC = CMASK;
		PORTD = DMASK;
		PORTE = EMASK&LED_BANKE_TAB[curLed - 6];
	}
	if( (11 > curLed) && (9 <= curLed))	//C
	{
		PORTB = BMASK;
		PORTC = CMASK;
		PORTD = DMASK&LED_BANKD_TAB[curLed - 9];
		PORTE = EMASK;
	}
	
	curLed = ((curLed + 1)%NUMLED);// + ((curLed + 1)/NUMLED);
}

void Next8(void)	//A B C D E F G
{
		if(3 > curLed)		//A
		{
			PORTB = BMASK;
			PORTC = CMASK&LED_BANKC_TAB[curLed];
			PORTD = DMASK;
			PORTE = EMASK;
		}
		
		if((6 > curLed)&&(3 <= curLed))	//B
		{
			PORTB = BMASK;
			PORTC = CMASK&LED_BANKC_TAB[curLed];
			PORTD = DMASK;
			PORTE = EMASK;
		}
		if((7 > curLed)&&(6 <= curLed))	//B
		{
			PORTB = BMASK;
			PORTC = CMASK;
			PORTD = DMASK;
			PORTE = EMASK&LED_BANKE_TAB[curLed - 6];
		}
		
		if((9> curLed)&&(7 <= curLed)) //C
		{
			PORTB = BMASK;
			PORTC = CMASK;
			PORTD = DMASK;
			PORTE = EMASK&LED_BANKE_TAB[curLed - 6];
		}
		if( (11 > curLed) && (9 <= curLed))	//C
		{
			PORTB = BMASK;
			PORTC = CMASK;
			PORTD = DMASK&LED_BANKD_TAB[curLed - 9];
			PORTE = EMASK;
		}
		
		if( (14 > curLed) && (11 <= curLed))	//D
		{
			PORTB = BMASK;
			PORTC = CMASK;
			PORTD = DMASK&LED_BANKD_TAB[curLed - 9];
			PORTE = EMASK;
		}
		
		if( (17 > curLed) && (14 <= curLed))	//E
		{
			PORTB = BMASK;
			PORTC = CMASK;
			PORTD = DMASK&LED_BANKD_TAB[curLed - 9];
			PORTE = EMASK;
		}
		if( (18 > curLed) && (17 <= curLed))	//E
		{
			PORTB = BMASK&LED_BANKB_TAB[curLed - 17];
			PORTC = CMASK;
			PORTD = DMASK;
			PORTE = EMASK;
		}
		
		if( (22 > curLed) && (18 <= curLed))	//F
		{
			PORTB = BMASK&LED_BANKB_TAB[curLed - 17];
			PORTC = CMASK;
			PORTD = DMASK;
			PORTE = EMASK;
		}
		
		if( (25 > curLed) && (22 <= curLed))	//G
		{
			PORTB = BMASK&LED_BANKB_TAB[curLed - 17];
			PORTC = CMASK;
			PORTD = DMASK;
			PORTE = EMASK;
		}
		
	curLed = ((curLed + 1)%NUMLED);// + ((curLed + 1)/NUMLED);
}

void Next9(void)	//A B C D F G
{
	
	if(3 > curLed)		//A
	{
		PORTB = BMASK;
		PORTC = CMASK&LED_BANKC_TAB[curLed];
		PORTD = DMASK;
		PORTE = EMASK;
	}
	
	if((6 > curLed)&&(3 <= curLed))	//B
	{
		PORTB = BMASK;
		PORTC = CMASK&LED_BANKC_TAB[curLed];
		PORTD = DMASK;
		PORTE = EMASK;
	}
	if((7 > curLed)&&(6 <= curLed))	//B
	{
		PORTB = BMASK;
		PORTC = CMASK;
		PORTD = DMASK;
		PORTE = EMASK&LED_BANKE_TAB[curLed - 6];
	}
	
	if((9> curLed)&&(7 <= curLed)) //C
	{
		PORTB = BMASK;
		PORTC = CMASK;
		PORTD = DMASK;
		PORTE = EMASK&LED_BANKE_TAB[curLed - 6];
	}
	if( (11 > curLed) && (9 <= curLed))	//C
	{
		PORTB = BMASK;
		PORTC = CMASK;
		PORTD = DMASK&LED_BANKD_TAB[curLed - 9];
		PORTE = EMASK;
	}
	
	if( (14 > curLed) && (11 <= curLed))	//D
	{
		PORTB = BMASK;
		PORTC = CMASK;
		PORTD = DMASK&LED_BANKD_TAB[curLed - 9];
		PORTE = EMASK;
	}
	
	if( (22 > curLed) && (18 <= curLed))	//F
	{
		PORTB = BMASK&LED_BANKB_TAB[curLed - 17];
		PORTC = CMASK;
		PORTD = DMASK;
		PORTE = EMASK;
	}
	
	if( (25 > curLed) && (22 <= curLed))	//G
	{
		PORTB = BMASK&LED_BANKB_TAB[curLed - 17];
		PORTC = CMASK;
		PORTD = DMASK;
		PORTE = EMASK;
	}
	
	curLed = ((curLed + 1)%NUMLED);// + ((curLed + 1)/NUMLED);
}

void ReportScore(uint8_t S)
{
	uint8_t hundreds = S/100;
	uint8_t remainder = S%100;
	uint8_t tens = remainder/10;
	remainder = remainder%10;
	uint8_t ones = remainder;
	if(0 != hundreds)
	{
		//ledMode = hundreds + 3;			//Definitions of ledModes for displaying digits is the digit value + 3...
		switch(hundreds)
		{
			case 1:
				ledMode = DISPLAY1;
				break;
			case 2:
				ledMode = DISPLAY2;
				break;
			case 3:
				ledMode = DISPLAY3;
				break;
			case 4:
				ledMode = DISPLAY4;
				break;
			case 5:
				ledMode	= DISPLAY5;
				break;
			case 6:
				ledMode = DISPLAY6;
				break;
			case 7:
				ledMode = DISPLAY7;
				break;
			case 8:
				ledMode = DISPLAY8;
				break;
			case 9:
				ledMode = DISPLAY9;
				break;
			default:
				//uh oh! Your score is probably really high dude!
				break;
		}
		
		_delay_ms(750);
	}
	if(tens || hundreds)
	{
		switch(tens)
		{
			case 0:
				ledMode = DISPLAY0;
				break;
			case 1:
				ledMode = DISPLAY1;
				break;
			case 2:
				ledMode = DISPLAY2;
				break;
			case 3:
				ledMode = DISPLAY3;
				break;
			case 4:
				ledMode = DISPLAY4;
				break;
			case 5:
				ledMode	= DISPLAY5;
				break;
			case 6:
				ledMode = DISPLAY6;
				break;
			case 7:
				ledMode = DISPLAY7;
				break;
			case 8:
				ledMode = DISPLAY8;
				break;
			case 9:
				ledMode = DISPLAY9;
				break;
			default:
			//uh oh! Your score is probably really high dude!
			break;
		}
		_delay_ms(750);
	}
	
	
	
	switch(ones)
	{
		case 0:
		ledMode = DISPLAY0;
		break;
		case 1:
		ledMode = DISPLAY1;
		break;
		case 2:
		ledMode = DISPLAY2;
		break;
		case 3:
		ledMode = DISPLAY3;
		break;
		case 4:
		ledMode = DISPLAY4;
		break;
		case 5:
		ledMode	= DISPLAY5;
		break;
		case 6:
		ledMode = DISPLAY6;
		break;
		case 7:
		ledMode = DISPLAY7;
		break;
		case 8:
		ledMode = DISPLAY8;
		break;
		case 9:
		ledMode = DISPLAY9;
		break;
		default:
		//uh oh! Your score is probably really high dude!
		break;
	}
	//_delay_ms(750);
}



int main(void)
{
	uint8_t key_status0 = 0;
	//uint8_t key_status1 = 0;

	/* Initializes MCU, drivers and middleware */
	atmel_start_init();
	init();
	ledIntEnable();
	ledIntSetup();
	/* Enable interrupts */
	cpu_irq_enable();

	/** If any of the two self-capacitance buttons is touched, the LED is turned ON
	 * When touch is released, the LED is turned OFF
	 */
	while (1) {
		/* Does acquisition and post-processing */
		touch_process();
		if (measurement_done_touch == 1) {
			measurement_done_touch = 0;

			key_status0 = get_sensor_state(0) & 0x80;
			
			#if(NumberTest == 1)
			{
				ledMode = 3 + score;
				if((0u != key_status0))
				{
					if(0 < skipFlag)
					{
						skipFlag--;
					}
					else
					{
					score++;
					ledMode = 3 + score;
					skipFlag+=2;	
					}
					_delay_ms(20);
				}
				//ReportScore();
				
			}
			#endif
			
			#if(NumberTest == 0)
			{
				if ((0u != key_status0))
				{
					if(0 < skipFlag)
					{
						skipFlag = 0;
						break;
					}
					
					ledMode = PAUSEDMODE;
					if(TARGETLED == curLed)		// Target Hit!
					{
						score++;
					}
					else
					{
						ReportScore(score);
						skipFlag = 1;
						score = 0;				//Reset game to start state...
						ledMode = NORMALMODE;
					}
				}
			}
			#endif
			
			
		}
	}
}

ISR (TIMER0_COMPB_vect)
{
	switch(ledMode)
	{
		case NORMALMODE:
			if(150 == cScaler)	//cScaler of 16 is fast but okay, 100 is slow enough to check each LED
			{
				NextLed();
				cScaler = 0;
			}
			cScaler++;
			break;
		
		case PAUSEDMODE:
			delayScaler++;
			if(3000 == delayScaler)
			{
				ledMode = NORMALMODE;
				delayScaler = 0;
			}
			break;
		
		case DISPLAY0:
			Next0();
			break;
		
		case DISPLAY1:
			Next1();
			break;
		
		case DISPLAY2:
			Next2();
			break;
		
		case DISPLAY3:
			Next3();
			break;
		
		case DISPLAY4:
			Next4();
			break;
		
		case DISPLAY5:
			Next5();
			break;
		
		case DISPLAY6:
			Next6();
			break;
			
		case DISPLAY7:
			Next7();
			break;
			
		case DISPLAY8:
			Next8();
			break;
			
		case DISPLAY9:
			Next9();
			break;
		
		default:
		break;
	}
}