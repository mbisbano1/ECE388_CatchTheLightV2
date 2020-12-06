/*
 * ISR_SwitchCase.h
 *
 * Created: 12/5/2020 6:59:00 PM
 *  Author: Michael Bisbano
 */ 


#ifndef ISR_SWITCHCASE_H_
#define ISR_SWITCHCASE_H_

#endif /* ISR_SWITCHCASE_H_ */

void isrSwitchCase(void)
{
	switch(ledMode)
	{
		case 1://NORMALMODE
		
		#if(DirectionTricking == 0)
		if(GAME_SPEED == cScaler)	//150 for visual test
		{
			NextLed();
			cScaler = 0;
		}
		#endif
		
		#if(DirectionTricking == 1)
		if((GAME_SPEED == cScaler)&&(direction == 0))	// Forwards
		{
			NextLed();
			cScaler = 0;
		}
		if((GAME_SPEED == cScaler)&&(direction == 1))	// Reverse
		{
			NextLedReverse();
			cScaler = 0;
			//direction = 0;
		}
		if((RandomValue*35 < delayScaler) &&(numberPresses))
		{
			delayScaler = 0;
			DetermineNewRandom();
			direction^=0x01;
		}
		#endif
		
		delayScaler++;
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
		
		case LEDSOFF:
		NoLeds();
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
		break;
		
		case G0:
		Group0();
		break;
		
		case G1:
		Group1();
		break;
		
		case G2:
		Group2();
		break;
		
		case G3:
		Group3();
		break;
		
		case G4:
		Group4();
		break;
		
		case G5:
		Group5();
		break;
		
		case G6:
		Group6();
		break;
		
		case G7:
		Group7();
		break;
		
		case G8:
		Group8();
		break;
		
		case G9:
		Group9();
		break;
		
		case G10:
		Group10();
		break;
		
		default:
		//uhoh brother you don't belong in this town
		break;
	}
}