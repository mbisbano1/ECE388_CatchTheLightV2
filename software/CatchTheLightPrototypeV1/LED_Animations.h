/*
 * LED_Animations.h
 *
 * Created: 12/5/2020 6:17:45 PM
 *  Author: Michael Bisbano
 */ 


#ifndef LED_ANIMATIONS_H_
#define LED_ANIMATIONS_H_
#endif /* LED_ANIMATIONS_H_ */


#define NUMLED 25

static uint8_t curLed;

#define	BMASK  0b11111111
#define	CMASK  0b00111111
#define	DMASK  0b11111111
#define	EMASK  0b00000111
static uint8_t cScaler;
#define TARGETLED 2



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
#if(FancyAnimations == 1)
#define G0		16
#define G1		17
#define G2		18
#define G3		19
#define G4		20
#define G5		21
#define G6		22
#define G7		23
#define G8		24
#define G9		25
#define G10		26
#endif



static uint16_t delayScaler = 0;
static uint8_t skipFlag = 0;
static uint8_t loopFlag = 0;
static uint8_t curLedWhenPressed = 0;
static uint8_t wrappedAround = 0;
static uint8_t numberPresses = 0;



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

void ClearLed(void)
{
	PORTD |= DMASK;
	PORTB |= BMASK;
	PORTC |= CMASK;
	PORTE |= EMASK;
}

void NextLedReverse(void)
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
	if(21 < curLed)
	{
		curLed = 21;
	}
	if(curLed == ((curLedWhenPressed+20)%22))
	{
		loopFlag = 0;
	}
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

#if(FancyAnimations == 1)
void Group0(void)				// each group is a horizontal row of leds, with Group0 being the bottom row (Segment D), and Group 10 being the top row (Segment A).
{
	if(3 > curLed)		//A
	{
		PORTB = BMASK;
		PORTC = CMASK;//&LED_BANKC_TAB[curLed];
		PORTD = DMASK;
		PORTE = EMASK;
	}
	
	if((6 > curLed)&&(3 <= curLed))	//B
	{
		PORTB = BMASK;
		PORTC = CMASK;//&LED_BANKC_TAB[curLed];
		PORTD = DMASK;
		PORTE = EMASK;
	}
	if((7 > curLed)&&(6 <= curLed))	//B
	{
		PORTB = BMASK;
		PORTC = CMASK;
		PORTD = DMASK;
		PORTE = EMASK;//&LED_BANKE_TAB[curLed - 6];
	}
	
	if((9> curLed)&&(7 <= curLed)) //C
	{
		PORTB = BMASK;
		PORTC = CMASK;
		PORTD = DMASK;
		PORTE = EMASK;//&LED_BANKE_TAB[curLed - 6];
	}
	if( (11 > curLed) && (9 <= curLed))	//C
	{
		PORTB = BMASK;
		PORTC = CMASK;
		PORTD = DMASK;//&LED_BANKD_TAB[curLed - 9];
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
		PORTD = DMASK;//&LED_BANKD_TAB[curLed - 9];
		PORTE = EMASK;
	}
	if( (18 > curLed) && (17 <= curLed))	//E
	{
		PORTB = BMASK;//&LED_BANKB_TAB[curLed - 17];
		PORTC = CMASK;
		PORTD = DMASK;
		PORTE = EMASK;
	}
	
	if( (22 > curLed) && (18 <= curLed))	//F
	{
		PORTB = BMASK;//&LED_BANKB_TAB[curLed - 17];
		PORTC = CMASK;
		PORTD = DMASK;
		PORTE = EMASK;
	}
	
	if( (25 > curLed) && (22 <= curLed))	//G
	{
		PORTB = BMASK;//&LED_BANKB_TAB[curLed - 17];
		PORTC = CMASK;
		PORTD = DMASK;
		PORTE = EMASK;
	}
	
	curLed = ((curLed + 1)%NUMLED);// + ((curLed + 1)/NUMLED);
}
void Group1(void)
{
	if(3 > curLed)		//A
	{
		PORTB = BMASK;
		PORTC = CMASK;//&LED_BANKC_TAB[curLed];
		PORTD = DMASK;
		PORTE = EMASK;
	}
	
	if((6 > curLed)&&(3 <= curLed))	//B
	{
		PORTB = BMASK;
		PORTC = CMASK;//&LED_BANKC_TAB[curLed];
		PORTD = DMASK;
		PORTE = EMASK;
	}
	if((7 > curLed)&&(6 <= curLed))	//B
	{
		PORTB = BMASK;
		PORTC = CMASK;
		PORTD = DMASK;
		PORTE = EMASK;//&LED_BANKE_TAB[curLed - 6];
	}
	
	if((9> curLed)&&(7 <= curLed)) //C
	{
		PORTB = BMASK;
		PORTC = CMASK;
		PORTD = DMASK;
		PORTE = EMASK;//&LED_BANKE_TAB[curLed - 6];
	}
	if( (10 > curLed) && (9 <= curLed))	//C
	{
		PORTB = BMASK;
		PORTC = CMASK;
		PORTD = DMASK;//&LED_BANKD_TAB[curLed - 9];
		PORTE = EMASK;
	}
	if( (11 > curLed) && (10 <= curLed))
	{
		PORTB = BMASK;
		PORTC = CMASK;
		PORTD = DMASK&LED_BANKD_TAB[curLed - 9];		//group 1
		PORTE = EMASK;
	}
	
	if( (14 > curLed) && (11 <= curLed))	//D
	{
		PORTB = BMASK;
		PORTC = CMASK;
		PORTD = DMASK&LED_BANKD_TAB[curLed - 9];		//group 0
		PORTE = EMASK;
	}
	if( (15 > curLed) && (14 <= curLed))
	{
		PORTB = BMASK;
		PORTC = CMASK;
		PORTD = DMASK&LED_BANKD_TAB[curLed - 9];		//group 1
		PORTE = EMASK;
	}
	if( (17 > curLed) && (15 <= curLed))	//E
	{
		PORTB = BMASK;
		PORTC = CMASK;
		PORTD = DMASK;//&LED_BANKD_TAB[curLed - 9];
		PORTE = EMASK;
	}
	if( (18 > curLed) && (17 <= curLed))	//E
	{
		PORTB = BMASK;//&LED_BANKB_TAB[curLed - 17];
		PORTC = CMASK;
		PORTD = DMASK;
		PORTE = EMASK;
	}
	
	if( (22 > curLed) && (18 <= curLed))	//F
	{
		PORTB = BMASK;//&LED_BANKB_TAB[curLed - 17];
		PORTC = CMASK;
		PORTD = DMASK;
		PORTE = EMASK;
	}
	
	if( (25 > curLed) && (22 <= curLed))	//G
	{
		PORTB = BMASK;//&LED_BANKB_TAB[curLed - 17];
		PORTC = CMASK;
		PORTD = DMASK;
		PORTE = EMASK;
	}
	curLed = ((curLed + 1)%NUMLED);
}
void Group2(void)
{
	if(3 > curLed)		//A
	{
		PORTB = BMASK;
		PORTC = CMASK;//&LED_BANKC_TAB[curLed];
		PORTD = DMASK;
		PORTE = EMASK;
	}
	
	if((6 > curLed)&&(3 <= curLed))	//B
	{
		PORTB = BMASK;
		PORTC = CMASK;//&LED_BANKC_TAB[curLed];
		PORTD = DMASK;
		PORTE = EMASK;
	}
	if((7 > curLed)&&(6 <= curLed))	//B
	{
		PORTB = BMASK;
		PORTC = CMASK;
		PORTD = DMASK;
		PORTE = EMASK;//&LED_BANKE_TAB[curLed - 6];
	}
	
	if((9> curLed)&&(7 <= curLed)) //C
	{
		PORTB = BMASK;
		PORTC = CMASK;
		PORTD = DMASK;
		PORTE = EMASK;//&LED_BANKE_TAB[curLed - 6];
	}
	
	if( (11 > curLed) && (9 <= curLed))
	{
		PORTB = BMASK;
		PORTC = CMASK;
		PORTD = DMASK&LED_BANKD_TAB[curLed - 9];		//group 1 & 2
		PORTE = EMASK;
	}
	
	if( (14 > curLed) && (11 <= curLed))	//D
	{
		PORTB = BMASK;
		PORTC = CMASK;
		PORTD = DMASK&LED_BANKD_TAB[curLed - 9];		//group 0
		PORTE = EMASK;
	}
	if( (16 > curLed) && (14 <= curLed))
	{
		PORTB = BMASK;
		PORTC = CMASK;
		PORTD = DMASK&LED_BANKD_TAB[curLed - 9];		//group 1 & 2
		PORTE = EMASK;
	}
	if( (17 > curLed) && (16 <= curLed))	//E
	{
		PORTB = BMASK;
		PORTC = CMASK;
		PORTD = DMASK;//&LED_BANKD_TAB[curLed - 9];
		PORTE = EMASK;
	}
	if( (18 > curLed) && (17 <= curLed))	//E
	{
		PORTB = BMASK;//&LED_BANKB_TAB[curLed - 17];
		PORTC = CMASK;
		PORTD = DMASK;
		PORTE = EMASK;
	}
	
	if( (22 > curLed) && (18 <= curLed))	//F
	{
		PORTB = BMASK;//&LED_BANKB_TAB[curLed - 17];
		PORTC = CMASK;
		PORTD = DMASK;
		PORTE = EMASK;
	}
	
	if( (25 > curLed) && (22 <= curLed))	//G
	{
		PORTB = BMASK;//&LED_BANKB_TAB[curLed - 17];
		PORTC = CMASK;
		PORTD = DMASK;
		PORTE = EMASK;
	}
	curLed = ((curLed + 1)%NUMLED);
}
void Group3(void)
{
	if(3 > curLed)		//A
	{
		PORTB = BMASK;
		PORTC = CMASK;//&LED_BANKC_TAB[curLed];
		PORTD = DMASK;
		PORTE = EMASK;
	}
	
	if((6 > curLed)&&(3 <= curLed))	//B
	{
		PORTB = BMASK;
		PORTC = CMASK;//&LED_BANKC_TAB[curLed];
		PORTD = DMASK;
		PORTE = EMASK;
	}
	if((7 > curLed)&&(6 <= curLed))	//B
	{
		PORTB = BMASK;
		PORTC = CMASK;
		PORTD = DMASK;
		PORTE = EMASK;//&LED_BANKE_TAB[curLed - 6];
	}
	
	if((8> curLed)&&(7 <= curLed)) //C
	{
		PORTB = BMASK;
		PORTC = CMASK;
		PORTD = DMASK;
		PORTE = EMASK;//&LED_BANKE_TAB[curLed - 6];
	}
	if((9> curLed)&&(8 <= curLed)) //C
	{
		PORTB = BMASK;
		PORTC = CMASK;
		PORTD = DMASK;
		PORTE = EMASK&LED_BANKE_TAB[curLed - 6];		//group 3
	}
	
	
	if( (11 > curLed) && (9 <= curLed))
	{
		PORTB = BMASK;
		PORTC = CMASK;
		PORTD = DMASK&LED_BANKD_TAB[curLed - 9];		//group 1 & 2
		PORTE = EMASK;
	}
	
	if( (14 > curLed) && (11 <= curLed))	//D
	{
		PORTB = BMASK;
		PORTC = CMASK;
		PORTD = DMASK&LED_BANKD_TAB[curLed - 9];		//group 0
		PORTE = EMASK;
	}
	if( (17 > curLed) && (14 <= curLed))
	{
		PORTB = BMASK;
		PORTC = CMASK;
		PORTD = DMASK&LED_BANKD_TAB[curLed - 9];		//group 1 & 2 & 3
		PORTE = EMASK;
	}
	if( (18 > curLed) && (17 <= curLed))	//E
	{
		PORTB = BMASK;//&LED_BANKB_TAB[curLed - 17];
		PORTC = CMASK;
		PORTD = DMASK;
		PORTE = EMASK;
	}
	
	if( (22 > curLed) && (18 <= curLed))	//F
	{
		PORTB = BMASK;//&LED_BANKB_TAB[curLed - 17];
		PORTC = CMASK;
		PORTD = DMASK;
		PORTE = EMASK;
	}
	
	if( (25 > curLed) && (22 <= curLed))	//G
	{
		PORTB = BMASK;//&LED_BANKB_TAB[curLed - 17];
		PORTC = CMASK;
		PORTD = DMASK;
		PORTE = EMASK;
	}
	curLed = ((curLed + 1)%NUMLED);
}
void Group4(void)
{
	if(3 > curLed)		//A
	{
		PORTB = BMASK;
		PORTC = CMASK;//&LED_BANKC_TAB[curLed];
		PORTD = DMASK;
		PORTE = EMASK;
	}
	
	if((6 > curLed)&&(3 <= curLed))	//B
	{
		PORTB = BMASK;
		PORTC = CMASK;//&LED_BANKC_TAB[curLed];
		PORTD = DMASK;
		PORTE = EMASK;
	}
	if((7 > curLed)&&(6 <= curLed))	//B
	{
		PORTB = BMASK;
		PORTC = CMASK;
		PORTD = DMASK;
		PORTE = EMASK;//&LED_BANKE_TAB[curLed - 6];
	}
	
	if((9> curLed)&&(7 <= curLed)) //C
	{
		PORTB = BMASK;
		PORTC = CMASK;
		PORTD = DMASK;
		PORTE = EMASK&LED_BANKE_TAB[curLed - 6];		//group 3 & 4
	}
	
	
	if( (11 > curLed) && (9 <= curLed))
	{
		PORTB = BMASK;
		PORTC = CMASK;
		PORTD = DMASK&LED_BANKD_TAB[curLed - 9];		//group 1 & 2
		PORTE = EMASK;
	}
	
	if( (14 > curLed) && (11 <= curLed))	//D
	{
		PORTB = BMASK;
		PORTC = CMASK;
		PORTD = DMASK&LED_BANKD_TAB[curLed - 9];		//group 0
		PORTE = EMASK;
	}
	if( (17 > curLed) && (14 <= curLed))
	{
		PORTB = BMASK;
		PORTC = CMASK;
		PORTD = DMASK&LED_BANKD_TAB[curLed - 9];		//group 1 & 2 & 3
		PORTE = EMASK;
	}
	if( (18 > curLed) && (17 <= curLed))	//E
	{
		PORTB = BMASK&LED_BANKB_TAB[curLed - 17];		//group 4
		PORTC = CMASK;
		PORTD = DMASK;
		PORTE = EMASK;
	}
	
	if( (22 > curLed) && (18 <= curLed))	//F
	{
		PORTB = BMASK;//&LED_BANKB_TAB[curLed - 17];
		PORTC = CMASK;
		PORTD = DMASK;
		PORTE = EMASK;
	}
	
	if( (25 > curLed) && (22 <= curLed))	//G
	{
		PORTB = BMASK;//&LED_BANKB_TAB[curLed - 17];
		PORTC = CMASK;
		PORTD = DMASK;
		PORTE = EMASK;
	}
	curLed = ((curLed + 1)%NUMLED);
}
void Group5(void)
{
	if(3 > curLed)		//A
	{
		PORTB = BMASK;
		PORTC = CMASK;//&LED_BANKC_TAB[curLed];
		PORTD = DMASK;
		PORTE = EMASK;
	}
	
	if((6 > curLed)&&(3 <= curLed))	//B
	{
		PORTB = BMASK;
		PORTC = CMASK;//&LED_BANKC_TAB[curLed];
		PORTD = DMASK;
		PORTE = EMASK;
	}
	if((7 > curLed)&&(6 <= curLed))	//B
	{
		PORTB = BMASK;
		PORTC = CMASK;
		PORTD = DMASK;
		PORTE = EMASK;//&LED_BANKE_TAB[curLed - 6];
	}
	
	if((9> curLed)&&(7 <= curLed)) //C
	{
		PORTB = BMASK;
		PORTC = CMASK;
		PORTD = DMASK;
		PORTE = EMASK&LED_BANKE_TAB[curLed - 6];		//group 3 & 4
	}
	
	
	if( (11 > curLed) && (9 <= curLed))
	{
		PORTB = BMASK;
		PORTC = CMASK;
		PORTD = DMASK&LED_BANKD_TAB[curLed - 9];		//group 1 & 2
		PORTE = EMASK;
	}
	
	if( (14 > curLed) && (11 <= curLed))	//D
	{
		PORTB = BMASK;
		PORTC = CMASK;
		PORTD = DMASK&LED_BANKD_TAB[curLed - 9];		//group 0
		PORTE = EMASK;
	}
	if( (17 > curLed) && (14 <= curLed))
	{
		PORTB = BMASK;
		PORTC = CMASK;
		PORTD = DMASK&LED_BANKD_TAB[curLed - 9];		//group 1 & 2 & 3
		PORTE = EMASK;
	}
	if( (18 > curLed) && (17 <= curLed))	//E
	{
		PORTB = BMASK&LED_BANKB_TAB[curLed - 17];		//group 4
		PORTC = CMASK;
		PORTD = DMASK;
		PORTE = EMASK;
	}
	
	if( (22 > curLed) && (18 <= curLed))	//F
	{
		PORTB = BMASK;//&LED_BANKB_TAB[curLed - 17];
		PORTC = CMASK;
		PORTD = DMASK;
		PORTE = EMASK;
	}
	
	if( (25 > curLed) && (22 <= curLed))	//G
	{
		PORTB = BMASK&LED_BANKB_TAB[curLed - 17];	//group 5
		PORTC = CMASK;
		PORTD = DMASK;
		PORTE = EMASK;
	}
	curLed = ((curLed + 1)%NUMLED);
}
void Group6(void)
{
	if(3 > curLed)		//A
	{
		PORTB = BMASK;
		PORTC = CMASK;//&LED_BANKC_TAB[curLed];
		PORTD = DMASK;
		PORTE = EMASK;
	}
	
	if((6 > curLed)&&(3 <= curLed))	//B
	{
		PORTB = BMASK;
		PORTC = CMASK;//&LED_BANKC_TAB[curLed];
		PORTD = DMASK;
		PORTE = EMASK;
	}
	
	if((9> curLed)&&(6 <= curLed)) //C
	{
		PORTB = BMASK;
		PORTC = CMASK;
		PORTD = DMASK;
		PORTE = EMASK&LED_BANKE_TAB[curLed - 6];		//group 3 & 4 & 6
	}
	
	
	if( (11 > curLed) && (9 <= curLed))
	{
		PORTB = BMASK;
		PORTC = CMASK;
		PORTD = DMASK&LED_BANKD_TAB[curLed - 9];		//group 1 & 2
		PORTE = EMASK;
	}
	
	if( (14 > curLed) && (11 <= curLed))	//D
	{
		PORTB = BMASK;
		PORTC = CMASK;
		PORTD = DMASK&LED_BANKD_TAB[curLed - 9];		//group 0
		PORTE = EMASK;
	}
	if( (17 > curLed) && (14 <= curLed))
	{
		PORTB = BMASK;
		PORTC = CMASK;
		PORTD = DMASK&LED_BANKD_TAB[curLed - 9];		//group 1 & 2 & 3
		PORTE = EMASK;
	}
	if( (19 > curLed) && (17 <= curLed))	//E
	{
		PORTB = BMASK&LED_BANKB_TAB[curLed - 17];		//group 4 & 6
		PORTC = CMASK;
		PORTD = DMASK;
		PORTE = EMASK;
	}
	
	if( (22 > curLed) && (19 <= curLed))	//F
	{
		PORTB = BMASK;//&LED_BANKB_TAB[curLed - 17];
		PORTC = CMASK;
		PORTD = DMASK;
		PORTE = EMASK;
	}
	
	if( (25 > curLed) && (22 <= curLed))	//G
	{
		PORTB = BMASK&LED_BANKB_TAB[curLed - 17];	//group 5
		PORTC = CMASK;
		PORTD = DMASK;
		PORTE = EMASK;
	}
	curLed = ((curLed + 1)%NUMLED);
}
void Group7(void)
{
	if(3 > curLed)		//A
	{
		PORTB = BMASK;
		PORTC = CMASK;//&LED_BANKC_TAB[curLed];
		PORTD = DMASK;
		PORTE = EMASK;
	}
	
	if((5 > curLed)&&(3 <= curLed))	//B
	{
		PORTB = BMASK;
		PORTC = CMASK;//&LED_BANKC_TAB[curLed];
		PORTD = DMASK;
		PORTE = EMASK;
	}
	if((6 > curLed)&&(5 <= curLed))	//B
	{
		PORTB = BMASK;
		PORTC = CMASK&LED_BANKC_TAB[curLed];			//group 7
		PORTD = DMASK;
		PORTE = EMASK;
	}
	
	if((9> curLed)&&(6 <= curLed)) //C
	{
		PORTB = BMASK;
		PORTC = CMASK;
		PORTD = DMASK;
		PORTE = EMASK&LED_BANKE_TAB[curLed - 6];		//group 3 & 4 & 6
	}
	
	
	if( (11 > curLed) && (9 <= curLed))
	{
		PORTB = BMASK;
		PORTC = CMASK;
		PORTD = DMASK&LED_BANKD_TAB[curLed - 9];		//group 1 & 2
		PORTE = EMASK;
	}
	
	if( (14 > curLed) && (11 <= curLed))	//D
	{
		PORTB = BMASK;
		PORTC = CMASK;
		PORTD = DMASK&LED_BANKD_TAB[curLed - 9];		//group 0
		PORTE = EMASK;
	}
	if( (17 > curLed) && (14 <= curLed))
	{
		PORTB = BMASK;
		PORTC = CMASK;
		PORTD = DMASK&LED_BANKD_TAB[curLed - 9];		//group 1 & 2 & 3
		PORTE = EMASK;
	}
	if( (20 > curLed) && (17 <= curLed))	//E
	{
		PORTB = BMASK&LED_BANKB_TAB[curLed - 17];		//group 4 & 6 & 7
		PORTC = CMASK;
		PORTD = DMASK;
		PORTE = EMASK;
	}
	
	if( (22 > curLed) && (20 <= curLed))	//F
	{
		PORTB = BMASK;//&LED_BANKB_TAB[curLed - 17];
		PORTC = CMASK;
		PORTD = DMASK;
		PORTE = EMASK;
	}
	
	if( (25 > curLed) && (22 <= curLed))	//G
	{
		PORTB = BMASK&LED_BANKB_TAB[curLed - 17];	//group 5
		PORTC = CMASK;
		PORTD = DMASK;
		PORTE = EMASK;
	}
	curLed = ((curLed + 1)%NUMLED);
}
void Group8(void)
{
	if(3 > curLed)		//A
	{
		PORTB = BMASK;
		PORTC = CMASK;//&LED_BANKC_TAB[curLed];
		PORTD = DMASK;
		PORTE = EMASK;
	}
	
	if((4 > curLed)&&(3 <= curLed))	//B
	{
		PORTB = BMASK;
		PORTC = CMASK;//&LED_BANKC_TAB[curLed];
		PORTD = DMASK;
		PORTE = EMASK;
	}
	if((6 > curLed)&&(4 <= curLed))	//B
	{
		PORTB = BMASK;
		PORTC = CMASK&LED_BANKC_TAB[curLed];			//group 7 & 8
		PORTD = DMASK;
		PORTE = EMASK;
	}
	
	if((9> curLed)&&(6 <= curLed)) //C
	{
		PORTB = BMASK;
		PORTC = CMASK;
		PORTD = DMASK;
		PORTE = EMASK&LED_BANKE_TAB[curLed - 6];		//group 3 & 4 & 6
	}
	
	
	if( (11 > curLed) && (9 <= curLed))
	{
		PORTB = BMASK;
		PORTC = CMASK;
		PORTD = DMASK&LED_BANKD_TAB[curLed - 9];		//group 1 & 2
		PORTE = EMASK;
	}
	
	if( (14 > curLed) && (11 <= curLed))	//D
	{
		PORTB = BMASK;
		PORTC = CMASK;
		PORTD = DMASK&LED_BANKD_TAB[curLed - 9];		//group 0
		PORTE = EMASK;
	}
	if( (17 > curLed) && (14 <= curLed))
	{
		PORTB = BMASK;
		PORTC = CMASK;
		PORTD = DMASK&LED_BANKD_TAB[curLed - 9];		//group 1 & 2 & 3
		PORTE = EMASK;
	}
	if( (21 > curLed) && (17 <= curLed))	//E
	{
		PORTB = BMASK&LED_BANKB_TAB[curLed - 17];		//group 4 & 6 & 7 & 8
		PORTC = CMASK;
		PORTD = DMASK;
		PORTE = EMASK;
	}
	
	if( (22 > curLed) && (21 <= curLed))	//F
	{
		PORTB = BMASK;//&LED_BANKB_TAB[curLed - 17];
		PORTC = CMASK;
		PORTD = DMASK;
		PORTE = EMASK;
	}
	
	if( (25 > curLed) && (22 <= curLed))	//G
	{
		PORTB = BMASK&LED_BANKB_TAB[curLed - 17];	//group 5
		PORTC = CMASK;
		PORTD = DMASK;
		PORTE = EMASK;
	}
	curLed = ((curLed + 1)%NUMLED);
}
void Group9(void)
{
	if(3 > curLed)		//A
	{
		PORTB = BMASK;
		PORTC = CMASK;//&LED_BANKC_TAB[curLed];
		PORTD = DMASK;
		PORTE = EMASK;
	}
	
	
	if((6 > curLed)&&(3 <= curLed))	//B
	{
		PORTB = BMASK;
		PORTC = CMASK&LED_BANKC_TAB[curLed];			//group 7 & 8 & 9
		PORTD = DMASK;
		PORTE = EMASK;
	}
	
	if((9> curLed)&&(6 <= curLed)) //C
	{
		PORTB = BMASK;
		PORTC = CMASK;
		PORTD = DMASK;
		PORTE = EMASK&LED_BANKE_TAB[curLed - 6];		//group 3 & 4 & 6
	}
	
	
	if( (11 > curLed) && (9 <= curLed))
	{
		PORTB = BMASK;
		PORTC = CMASK;
		PORTD = DMASK&LED_BANKD_TAB[curLed - 9];		//group 1 & 2
		PORTE = EMASK;
	}
	
	if( (14 > curLed) && (11 <= curLed))	//D
	{
		PORTB = BMASK;
		PORTC = CMASK;
		PORTD = DMASK&LED_BANKD_TAB[curLed - 9];		//group 0
		PORTE = EMASK;
	}
	if( (17 > curLed) && (14 <= curLed))
	{
		PORTB = BMASK;
		PORTC = CMASK;
		PORTD = DMASK&LED_BANKD_TAB[curLed - 9];		//group 1 & 2 & 3
		PORTE = EMASK;
	}
	if( (22 > curLed) && (17 <= curLed))	//E
	{
		PORTB = BMASK&LED_BANKB_TAB[curLed - 17];		//group 4 & 6 & 7 & 8 & 9
		PORTC = CMASK;
		PORTD = DMASK;
		PORTE = EMASK;
	}
	
	if( (25 > curLed) && (22 <= curLed))	//G
	{
		PORTB = BMASK&LED_BANKB_TAB[curLed - 17];	//group 5
		PORTC = CMASK;
		PORTD = DMASK;
		PORTE = EMASK;
	}
	curLed = ((curLed + 1)%NUMLED);
}
void Group10(void)
{
	if(3 > curLed)		//A
	{
		PORTB = BMASK;
		PORTC = CMASK&LED_BANKC_TAB[curLed];			//group 10
		PORTD = DMASK;
		PORTE = EMASK;
	}
	
	
	if((6 > curLed)&&(3 <= curLed))	//B
	{
		PORTB = BMASK;
		PORTC = CMASK&LED_BANKC_TAB[curLed];			//group 7 & 8 & 9
		PORTD = DMASK;
		PORTE = EMASK;
	}
	
	if((9> curLed)&&(6 <= curLed)) //C
	{
		PORTB = BMASK;
		PORTC = CMASK;
		PORTD = DMASK;
		PORTE = EMASK&LED_BANKE_TAB[curLed - 6];		//group 3 & 4 & 6
	}
	
	
	if( (11 > curLed) && (9 <= curLed))
	{
		PORTB = BMASK;
		PORTC = CMASK;
		PORTD = DMASK&LED_BANKD_TAB[curLed - 9];		//group 1 & 2
		PORTE = EMASK;
	}
	
	if( (14 > curLed) && (11 <= curLed))	//D
	{
		PORTB = BMASK;
		PORTC = CMASK;
		PORTD = DMASK&LED_BANKD_TAB[curLed - 9];		//group 0
		PORTE = EMASK;
	}
	if( (17 > curLed) && (14 <= curLed))
	{
		PORTB = BMASK;
		PORTC = CMASK;
		PORTD = DMASK&LED_BANKD_TAB[curLed - 9];		//group 1 & 2 & 3
		PORTE = EMASK;
	}
	if( (22 > curLed) && (17 <= curLed))	//E
	{
		PORTB = BMASK&LED_BANKB_TAB[curLed - 17];		//group 4 & 6 & 7 & 8 & 9
		PORTC = CMASK;
		PORTD = DMASK;
		PORTE = EMASK;
	}
	
	if( (25 > curLed) && (22 <= curLed))	//G
	{
		PORTB = BMASK&LED_BANKB_TAB[curLed - 17];	//group 5
		PORTC = CMASK;
		PORTD = DMASK;
		PORTE = EMASK;
	}
	curLed = ((curLed + 1)%NUMLED);
}
#endif

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
		curLed = 21;
		wrappedAround++;
		
	}
	if((curLed == 1))// && wrappedAround)
	{
		ledMode = NORMALMODE;
		curLedWhenPressed = 1;
		loopFlag = 1;
	}
}

void NewGameAnimation(void)			// basically backwards NextLed() function!
{
	//curLed = 0;
	ClearLed();
	
	#if(FancyAnimations == 1)
	FillFromBottom();
	curLed = 13;
	ledMode = NEWGAMEANIMATE;
	#endif
	
	#if(FancyAnimations == 0)
	ledMode = NEWGAMEANIMATE;
	#endif
	//ledMode = NEWGAMEANIMATE;
}