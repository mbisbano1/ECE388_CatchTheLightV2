#include <atmel_start.h>
#include "touch.h"
extern volatile uint8_t measurement_done_touch;

/*
Static Declarations and Definitions
*/
#define GAME_SPEED 90
#define ANIMATION_SPEED 30
#define DELAY_SPEED 1000

#define NumberTest 0

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
#define LEDSOFF		13
#define NEWGAMEANIMATE 14
#define DISPLAYX	15
static uint16_t delayScaler = 0;
static uint8_t skipFlag = 0;
static uint8_t loopFlag = 0;
static uint8_t curLedWhenPressed = 0;
static uint8_t wrappedAround = 0;


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
	curLed = ((curLed + 1)%22);
	if(curLed == ((curLedWhenPressed+2)%22))
	{
		loopFlag = 0;
	}
	//if(curLed == 0)
	//curLed = ((curLed + 1)%NUMLED);// + ((curLed + 1)/NUMLED);
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

void NextX(void)
{
	if(4 > curLed)		//A
	{
		PORTB = BMASK;
		PORTC = CMASK;	//&LED_BANKC_TAB[curLed];
		PORTD = DMASK;
		PORTE = EMASK;
	}
	
	if((6 > curLed)&&(4 <= curLed))	//B
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
	if( (10 > curLed) && (9 <= curLed))	//C
	{
		PORTB = BMASK;
		PORTC = CMASK;
		PORTD = DMASK&LED_BANKD_TAB[curLed - 9];
		PORTE = EMASK;
	}
	
	if( (15 > curLed) && (10 <= curLed))	//D
	{
		PORTB = BMASK;
		PORTC = CMASK;
		PORTD = DMASK;	//&LED_BANKD_TAB[curLed - 9];
		PORTE = EMASK;
	}
	
	if( (17 > curLed) && (15 <= curLed))	//E
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
	
	if( (21 > curLed) && (18 <= curLed))	//F
	{
		PORTB = BMASK&LED_BANKB_TAB[curLed - 17];
		PORTC = CMASK;
		PORTD = DMASK;
		PORTE = EMASK;
	}
	if(curLed == 21)
	{
		PORTB = BMASK;
		PORTC = CMASK;	//&LED_BANKC_TAB[curLed];
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
	
	curLed = ((curLed + 1)%NUMLED);
}




void NoLeds(void)
{
	PORTB = BMASK;
	PORTC = CMASK;
	PORTD = DMASK;
	PORTE = EMASK;
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
		_delay_ms(1500);
		ledMode = LEDSOFF; 
		_delay_ms(500);
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
		_delay_ms(1500);
		ledMode = LEDSOFF;
		_delay_ms(500);
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
	_delay_ms(1500);
	curLed = 0;
	ledMode = NEWGAMEANIMATE;
	//ledMode = NORMALMODE;
}

void NewGame(void)					// Restore the game to starting conditions!
{	
	//loopFlag = 1;
	//curLed = 0;
	//curLedWhenPressed = 0;
	cScaler = 0;
	delayScaler = 0;
	ClearLed();
	cScaler = 0;
	delayScaler = 0;
	score = 0;
	NewGameAnimation();
	
}
void NewGameAnimationNextLed(void)
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
	curLed -= 1;
	if(curLed > 22)
	{
		curLed = 22;
		wrappedAround++;
		
	}
	if((curLed == 0) && wrappedAround)
	{
		ledMode = NORMALMODE;
		curLedWhenPressed = 0;
		loopFlag = 1;
	}
}


void NewGameAnimation(void)			// basically backwards NextLed() function!
{
	//curLed = 0;
	ClearLed();
	ledMode = NEWGAMEANIMATE;
}

void FlashX(void)
{
	ledMode = DISPLAYX;
	_delay_ms(750);
	ledMode = LEDSOFF;
	_delay_ms(750);
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
	
	NewGameAnimation();

	
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
					if(0 < loopFlag)
					{
						// Don't process touch!
					}
					
					//ledMode = PAUSEDMODE;
					else if(TARGETLED == curLed)		// Target Hit!
					{
						score++;						// increment the player's score
						loopFlag = 1;					// set the loopFlag so that repeat touches are not processed
						curLedWhenPressed = curLed;		// store current led value for comparison later (to clear loopFlag lock)...
						ledMode = 2;//PAUSEDMODE;		// Put game into paused mode, LED holds on Target LED.
					}
					else
					{	
						//ledMode = DISPLAYX;
						//_delay_ms(750);
						//ledMode = LEDSOFF;
						//_delay_ms(750);
						FlashX();
						
						loopFlag = 1;
						curLedWhenPressed = curLed;
						ReportScore(score);				// Run the Score Report routine
						NewGame();
						
						//ReportScore(10*score + 2);	
						//NewGame();
						//score = 0;						
						//loopFlag = 1;					// set the loopFlag so that repeat touches are not processed
						//curLedWhenPressed = curLed;
						//ledMode = 2;//PAUSEDMODE;
						//skipFlag = 2;
						//score = 0;				//Reset game to start state...
						//ledMode = NORMALMODE;
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
		case 1://NORMALMODE
			if(GAME_SPEED == cScaler)	//150 for visual test
			{
				NextLed();
				cScaler = 0;
			}
			cScaler++;
			break;
		
		case 2://PAUSEDMODE:
			delayScaler++;
			if(DELAY_SPEED == delayScaler)
			{
				ledMode = 1;//NORMALMODE;
				delayScaler = 0;
			}
			break;
		
		case DISPLAY0:
			Next0();
			/*
			delayScaler++;
			if(6000 == delayScaler)
			{
				ledMode = NORMALMODE;
				score = 0;
				delayScaler = 0;	
			}
			*/
			break;
		
		case DISPLAY1:
			Next1();
			/*
			delayScaler++;
			if(6000 == delayScaler)
			{
				ledMode = NORMALMODE;
				score = 0;
				delayScaler = 0;
			}
			*/
			break;
		
		case DISPLAY2:
			Next2();
			/*
			delayScaler++;
			if(6000 == delayScaler)
			{
				ledMode = NORMALMODE;
				score = 0;
				delayScaler = 0;
			}
			*/
			break;
		
		case DISPLAY3:
			Next3();
			
			break;
		
		case DISPLAY4:
			Next4();
			/*
			delayScaler++;
			if(6000 == delayScaler)
			{
				ledMode = NORMALMODE;
				score = 0;
				delayScaler = 0;
			}
			*/
			break;
		
		case DISPLAY5:
			Next5();
			/*
			delayScaler++;
			if(6000 == delayScaler)
			{
				ledMode = NORMALMODE;
				score = 0;
				delayScaler = 0;
			}
			*/
			break;
		
		case DISPLAY6:
			Next6();
			/*
			delayScaler++;
			if(6000 == delayScaler)
			{
				ledMode = NORMALMODE;
				score = 0;
				delayScaler = 0;
			}
			*/
			break;
			
		case DISPLAY7:
			Next7();
			/*
			delayScaler++;
			if(6000 == delayScaler)
			{
				ledMode = NORMALMODE;
				score = 0;
				delayScaler = 0;
			}
			*/
			break;
			
		case DISPLAY8:
			Next8();
			/*
			delayScaler++;
			if(6000 == delayScaler)
			{
				ledMode = NORMALMODE;
				score = 0;
				delayScaler = 0;
			}
			*/
			break;
			
		case DISPLAY9:
			Next9();
			/*
			delayScaler++;
			if(9000 == delayScaler)
			{
				ledMode = NORMALMODE;
				score = 0;
				delayScaler = 0;
			}
			*/
			break;
		
		case LEDSOFF:
			NoLeds();
			//delayScaler++;
			//if(000 == delayScaler)
			//{
			//	ledMode = NORMALMODE;
			//	score = 0;
			//	delayScaler = 0;
			//}
			break;
			
		case NEWGAMEANIMATE:
			//NewGameAnimationNextLed();
			if(ANIMATION_SPEED == cScaler)	//150 for visual test
			{
				NewGameAnimationNextLed();
				cScaler = 0;
			}
			cScaler++;
			
			break;
			
		case DISPLAYX:
			//delayScaler++;
			NextX();
			/*
			if(DELAY_SPEED == delayScaler)
			{
				ledMode = NORMALMODE;
				delayScaler = 0;
			}
			*/
			break;
			
		default:
			//uhoh brother you don't belong in this town
		
		break;
	}
}