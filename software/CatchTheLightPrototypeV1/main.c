#include <atmel_start.h>
#include "touch.h"
extern volatile uint8_t measurement_done_touch;

#include "GameSettings.h"


#if(DirectionTricking == 1)
#define TARGETLEDREV 0
static uint8_t RandomValue = 0xAC;
static uint8_t direction = 0;
static uint8_t seed = 0;
uint8_t DetermineNewRandom(void)
{
	//uint8_t temp = RandomValue;
	uint8_t CarryBit = 0x01&RandomValue;
	RandomValue = (RandomValue>>1);
	if(CarryBit)
	{
		RandomValue ^= 0xB8;
	}
	if(0 == RandomValue)
	{
		RandomValue = 0xAC;
	}
	RandomValue = RandomValue%0x80;
	return RandomValue;
}
#endif
#include "LED_Animations.h"
#include "ISR_SwitchCase.h"
#define F_CPU 8000000
#include <util/delay.h>





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

#if(FancyAnimations == 1)

void FillFromBottom(void)	// Animation fill from bottom...	Has to stay in this file because it uses delay...
{	
	uint8_t fillDelay = 30;
	ledMode = G0;
	_delay_ms(fillDelay);
	ledMode = G1;
	_delay_ms(fillDelay);
	ledMode = G2;
	_delay_ms(fillDelay);
	ledMode = G3;
	_delay_ms(fillDelay);
	ledMode = G4;
	_delay_ms(fillDelay);
	ledMode = G5;
	_delay_ms(fillDelay);
	ledMode = G6;
	_delay_ms(fillDelay);
	ledMode = G7;
	_delay_ms(fillDelay);
	ledMode = G8;
	_delay_ms(fillDelay);
	ledMode = G9;
	_delay_ms(fillDelay);
	ledMode = G10;
	_delay_ms(fillDelay);
	ledMode = G9;
	_delay_ms(fillDelay);
	ledMode = G8;
	_delay_ms(fillDelay);
	ledMode = G7;
	_delay_ms(fillDelay);
	ledMode = G6;
	_delay_ms(fillDelay);
	ledMode = G5;
	_delay_ms(fillDelay);
	ledMode = G4;
	_delay_ms(fillDelay);
	ledMode = G3;
	_delay_ms(fillDelay);
	ledMode = G2;
	_delay_ms(fillDelay);
	ledMode = G1;
	_delay_ms(fillDelay);
	ledMode = G0;
	_delay_ms(fillDelay);
}
#endif


void ReportScore(uint8_t S)			// Animation display score
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
		_delay_ms(REPORT_SCORE_DELAY);
		ledMode = LEDSOFF; 
		_delay_ms(REPORT_SCORE_DELAY_BETWEEN_DIGITS);
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
		_delay_ms(REPORT_SCORE_DELAY);
		ledMode = LEDSOFF;
		_delay_ms(REPORT_SCORE_DELAY_BETWEEN_DIGITS);
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
	_delay_ms(REPORT_SCORE_DELAY);
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
	
	#if(IncreaseSpeed == 1)
		GAME_SPEED = DEFAULT_GAME_SPEED;
	#endif	
}

void FlashX(void)	
{
	ledMode = DISPLAYX;
	_delay_ms(750);
	ledMode = LEDSOFF;
	_delay_ms(750);
}

#if( IncreaseSpeed ==1 )
void SpeedUp(void)
{
	double val = ((double) GAME_SPEED)*(1-SpeedUpPercent);
	GAME_SPEED = ceil(val);
}
#endif 

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
			
			#if(ReportScoreTest != 0)
				ReportScore(ReportScoreTest);
				ledMode = LEDSOFF;
				_delay_ms(5000);				
			#endif			
			
			#if(RandomNumberTest == 1)
				DetermineNewRandom();
				ReportScore(RandomValue%9);
				ledMode = LEDSOFF;
				_delay_ms(2000);
			#endif
				
			#if(NumberDisplayTest == 1)			
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
			}
			#endif
			
			#if( (NumberDisplayTest == 0) && (RandomNumberTest == 0) && (ReportScoreTest == 0) )
			{
				if ((0u != key_status0))
				{	
					#if(DirectionTricking == 1)
					if(!numberPresses)					// On first button press, determine starting random value for RNG, otherwise it will always follow same pattern.
					{
						RandomValue = delayScaler;		// squeezing 16 bit into 8 bit, but should atleast be random...
						DetermineNewRandom();			// Get a new random num for redundancy
						numberPresses++;				// Increment so code behaves normally from now on.
					}
					#endif 
					
					if(0 < loopFlag)					// If loopFlag is set
					{
						// Don't process touch!
					}
					
					#if(DirectionTricking == 0)			// If in normal game mode
					else if(TARGETLED == curLed)		// Target Hit!
					{
						score++;						// increment the player's score
						loopFlag = 1;					// set the loopFlag so that repeat touches are not processed
						curLedWhenPressed = curLed;		// store current led value for comparison later (to clear loopFlag lock)...
						
						delayScaler = 0;				// Reset Delay counter so that it doesn't hang on paused mode...
						ledMode = 2;//PAUSEDMODE;		// Put game into paused mode, LED holds on Target LED.
						#if(IncreaseSpeed == 1)
						SpeedUp();
						//GAME_SPEED -= SpeedIncrement;
						#endif
					}
					#endif
					
					#if(DirectionTricking == 1)
					else if((TARGETLED == curLed)&&(!direction))		// Target Hit in normal direction!
					{
						score++;						// increment the player's score
						loopFlag = 1;					// set the loopFlag so that repeat touches are not processed
						curLedWhenPressed = curLed;		// store current led value for comparison later (to clear loopFlag lock)...
						
						delayScaler = 0;				// Reset Delay counter so that it doesn't hang on paused mode...
						ledMode = 2;//PAUSEDMODE;		// Put game into paused mode, LED holds on Target LED.
						#if(IncreaseSpeed == 1)
						SpeedUp();
						//GAME_SPEED -= SpeedIncrement;
						#endif
					}
					else if((TARGETLEDREV == curLed)&&(direction))		//Target Hit in reverse direction!
					{
						score++;						// increment the player's score
						loopFlag = 1;					// set the loopFlag so that repeat touches are not processed
						curLedWhenPressed = curLed;		// store current led value for comparison later (to clear loopFlag lock)...
						
						delayScaler = 0;				// Reset Delay counter so that it doesn't hang on paused mode...
						ledMode = 2;//PAUSEDMODE;		// Put game into paused mode, LED holds on Target LED.
						#if(IncreaseSpeed == 1)
						SpeedUp();
						//GAME_SPEED -= SpeedIncrement;
						#endif
					}
					#endif
					else
					{	
						FlashX();
						loopFlag = 1;					// set the loopFlag so that repeat touches are not processed
						curLedWhenPressed = curLed;		// store current led value for comparison later (to clear loopFlag lock)... 
						ReportScore(score);				// Run the Score Report routine
						NewGame();						// Reset status, start new game!
					}
				}
			}
			#endif
			
			
		}
	}
}

ISR (TIMER0_COMPB_vect)
{
	isrSwitchCase();
}