
#ifndef GRAPHICS_SETTINGWIN_H_
#define GRAPHICS_SETTINGWIN_H_

#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include "Button.h"
#include "../ConsoleMode.h"

// BUTTONS DEFINING
#define NOOB_BUTTON 0
#define EASY_BUTTON 1
#define MODERATE_BUTTON 2
#define HARD_BUTTON 3
#define WHITE_BUTTON 4
#define BLACK_BUTTON 5
#define TWO_PLAYER 6
#define ONE_PLAYER 7
#define SET_BACK_BUTTON 8
#define SET_START_BUTTON 9
#define SET_NEXT_BUTTON 10

#define NUM_OF_SET_BUTTONS 11
#define NUM_OF_DIFFICULTIES 4

#define DIF_X 175
#define DIF_H 50
#define DIF_W 150
#define DIF_START_Y 100
#define PLAYERS_W 220
#define PLAYERS_H 100
#define PLAYERS_Y 100
#define PLAYERS_X_BUTTON 16
#define PLAYER_X_BUTTON 264
#define WHITE_X_BUTTON 70
#define BLACK_X_BUTTON 270
#define COLOR_BUTTON_Y 160
#define COLOR_BUTTON_H 150
#define DIF_GAP 20

#define OTHERS_BUT_WIT 180
#define OTHERS_BUT_H 75
#define OTHERS_BUT_Y 500
#define OTHERS_BUT_X 65



//WINDOW SIZES
#define SET_WINDOW_H 600
#define SET_WINDOW_W 500

//COLORS DEFINING
#define SET_R 244
#define SET_G 208
#define SET_B 159
#define COLOR_R 185
#define COLOR_G 122
#define COLOR_B 87

//MASC
#define SET_DRAG_INIT -1
//enum that represents all settings events possible

typedef enum {
	//general
	SP_SETTINGS_EVENT_BACK,
	SP_SETTINGS_EVENT_START,
	SP_SETTINGS_EVENT_NEXT,
	//game mode
	SP_SETTINGS_EVENT_ONE_PLAYER,
	SP_SETTINGS_EVENT_TWO_PLAYER,
	//difficulty
	SP_SETTINGS_EVENT_NOOB,
	SP_SETTINGS_EVENT_EASY,
	SP_SETTINGS_EVENT_MODERATE,
	SP_SETTINGS_EVENT_HARD,
	//color
	SP_SETTINGS_EVENT_WHITE,
	SP_SETTINGS_EVENT_BLACK,
	//errors
	SP_SETTINGS_EVENT_INVALID_ARGUMENT,
	SP_SETTINGS_EVENT_NONE,
	SP_SETTINGS_EVENT_QUIT
} SP_SETTINGS_EVENT;

//enum that represents sub window's of settings
typedef enum {
	//general
	GameModeScreen,
	DifficultyScreen,
	ColorScreen
} SUB_WINDOW;

//enum that represents settings window
typedef struct{
	SDL_Window* window;//window name
	SDL_Renderer* renderer;//window's renderer
	Button** buttonsList;//settings window button's list
	int buttonsCounter;//num of buttons
	SUB_WINDOW curr_screen; //current sub screen
}SPSettingsWin;

/**
 *  function that checks whether there was a click on a button in settings screen
 *
 *	@param x - the x coordinate of the click
 *	@param y - the y ccoordinate of the click
 *	@param src - the source settings window
 *
 *  @return
 *  number that represents the button clicked
 *  -1 - if no button was clicked
 */
int isClickOnSet(int x, int y,SPSettingsWin* src);

/**
 *  Creates a settings window, if an error occured, prints a suitable message.
 *
 *  @return
 *  NULL, if an allocation error occurred
 *  settings window - otherwise
 */
SPSettingsWin* spSettingsWindowCreate();

/**
 *  function that draws the settings window, prints a suitable message if src == NULL
 *
 *  @param src - the source settings window
 *
 */
void spSettingsWindowDraw(SPSettingsWin*);

/**
 * Frees all memory resources associated with the source window. If the
 * source window is NULL, then the function does nothing.
 * @param src - the source window
 */
void spSettingsWindowDestroy(SPSettingsWin*);

/**
 *  Handles the load window to the settings event
 *
 *	@param event - the event that happened in the settings window
 *	@param game - the source game
 *	@param src - the source settings window
 *
 *  @return
 *  SP_SETTINGS_EVENT_INVALID_ARGUMENT - if one of the arguments is invalid
 *  otherwise -the relevant event
 */
SP_SETTINGS_EVENT spSettingsWindowHandleEvent(SPSettingsWin* src, SDL_Event* event,CH_Game* game);

/**
 *  Hides the relevant window
 *	@param src - the source settings window
 *
 */
void spSettingsWindowHide(SPSettingsWin* src);

/**
 *  Shows the relevant window
 *	@param src - the source settings window
 *
 */
void spSettingsWindowShow(SPSettingsWin* src);

/**
 *  Changes the screen to the relevant sub screen - game mode
 *  Shows the relevant buttons
 *
 *	@ param src - the source settings window
 *	@ param game - the chess game
 *	@ param one player - flag that shows if the buttons clicked is one player button
 *
 */
void gameModeChanges(SPSettingsWin* src, CH_Game* game,bool one_player);

/**
 *  Changes the screen to the relevant sub screen - difficulty
 *  Shows the relevant buttons
 *
 *	@ param src - the source settings window
 *	@ param game - the chess game
 *	@ param diff - represents the difficulty choosen
 *
 */
void gameDiffChanges(SPSettingsWin* src, CH_Game* game,int diff);

/**
 *  Changes the screen to the relevant sub screen - color
 *  Shows the relevant buttons
 *
 *	@ param src - the source settings window
 *	@ param game - the chess game
 *	@ param color - represents the color choosen
 *
 */
void gameColorChanges(SPSettingsWin* src, CH_Game* game,int color);

/**
 *  Helper function that switches to the relevant screen - difficulty
 *
 *	@ param src - the source settings window
 *
 */
void spSettingsChangeToDifficulty(SPSettingsWin* src);

/**
 *  Helper function that switches to the relevant screen - game mode
 *
 *	@ param src - the source settings window
 *
 */
void spSettingsChangeToGameMode(SPSettingsWin* src);

/**
 *  Helper function that switches to the relevant screen - color
 *
 *	@ param src - the source settings window
 *
 */
void spSettingsChangeToColor(SPSettingsWin* src);

/**
 *  initializes settings buttons
 *
 *	@param res - the source settings window
 *
 *  @return
 *  true - if succeeded
 *  false -if an error occurred
 */
bool setButtonsInit(SPSettingsWin* res);

#endif /* GRAPHICS_SETTINGWIN_H_ */
