/*
 * GameSettings.h
 *
 * Created: 12/5/2020 6:12:50 PM
 *  Author: Michael Bisbano
 */ 


#ifndef GAMESETTINGS_H_
#define GAMESETTINGS_H_

#define ReportScoreTest		0		// 0 is default game mode, numeric value will send the numeric value to ReportScore function.
#define RandomNumberTest	0		// 0 is default game mode, 1 puts board in random number test mode...
#define NumberDisplayTest	0		// 0 is default game mode, 1 puts board in number test mode...
#define IncreaseSpeed		1		// 0 is default game mode, 1 increases the speed of light slightly on successful hit.
#define DirectionTricking	0		// 0 is default game mode, 1 allows the game to reverse direction randomly to trick the player.
#define FancyAnimations		1		// 0 is default game mode, 1 allows fancy animations to occur.. (larger program size).

/*
Static Declarations and Definitions
*/
#define DEFAULT_GAME_SPEED 90
static uint8_t GAME_SPEED = DEFAULT_GAME_SPEED;
#define ANIMATION_SPEED 15
#define DELAY_SPEED 1000
#define REPORT_SCORE_DELAY 2000
#define REPORT_SCORE_DELAY_BETWEEN_DIGITS 500

static uint8_t score;

#if(IncreaseSpeed == 1)
#define SpeedIncrement 10
#define SpeedUpPercent 0.075
#endif










#endif /* GAMESETTINGS_H_ */