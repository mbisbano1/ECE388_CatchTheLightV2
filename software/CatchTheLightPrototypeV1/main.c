#include <atmel_start.h>
#include "touch.h"
extern volatile uint8_t measurement_done_touch;

/*
Static Declarations and Definitions
*/
#define NUMLED 25
static uint8_t curLed;
#define F_CPU 8000000
#include <util/delay.h>
#define	BMASK  0b11111111
#define	CMASK  0b00111111
#define	DMASK  0b11111111
#define	EMASK  0b00000111
static uint8_t cScaler;
#define TARGETLED 0

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
			//key_status1 = get_sensor_state(1) & 0x80;

			if ((0u != key_status0))
			{
				//LED_set_level(true);
				//ledIntDisable();
				//_delay_ms(3000);
				//ledIntEnable();	
				ledMode = PAUSEDMODE;	
				if(TARGETLED == curLed)
				{
					score++;
				}	
			}
			
		}
	}
}

ISR (TIMER0_COMPB_vect)
{
	switch(ledMode)
	{
		case NORMALMODE:
			if(80 == cScaler)	//cScaler of 16 is fast but okay, 100 is slow enough to check each LED
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
		
			break;
		
		case DISPLAY1:
		
			break;
		
		case DISPLAY2:
		
			break;
		
		case DISPLAY3:
		
			break;
		
		case DISPLAY4:
		
			break;
		
		case DISPLAY5:
		
			break;
		
		case DISPLAY6:
		
			break;
			
		case DISPLAY7:
		
			break;
			
		case DISPLAY8:
			
			break;
			
		case DISPLAY9:
		
			break;
		
		default:
		break;
	}
}