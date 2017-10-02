

#ifndef GRAPHICS_LOADWIN_H_
#define GRAPHICS_LOADWIN_H_

#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include "Button.h"
#include "../ConsoleMode.h"


//BUTTONS DEFINING
#define SLOT1_BUTTON 0
#define SLOT2_BUTTON 1
#define SLOT3_BUTTON 2
#define SLOT4_BUTTON 3
#define SLOT5_BUTTON 4
#define LOAD_BACK_BUTTON 5
#define LOAD_LOAD_BUTTON 6
#define SLOTS_X_VAL 100
#define SLOTS_Y_VAL 25
#define SLOTS_Y_OFFSET 15
#define SLOTS_H 50
#define SLOTS_W 200
#define OTHER_H 75
#define OTHER_W 170
#define OTHER_Y 500
#define BACK_X 20
#define LOAD_X 210

//COLORS DEFINING
#define LOAD_R 244
#define LOAD_G 208
#define LOAD_B 159

//WINDOW SIZES
#define LOAD_WINDOW_H 600
#define LOAD_WINDOW_W 400

//MASC DEFINING
#define NUM_OF_LOAD_BUTTONS 7
#define SLOT_BUTTONS_START_INDEX 2

//FLAGS
#define ERROR_FLAG_LOAD -1
#define LOAD_ARGS_INIT -1

//enum that represents all load events possible
typedef enum {
	SP_lOAD_EVENT_BACK,
	SP_lOAD_EVENT_lOAD,
	SP_lOAD_EVENT_SLOT1,
	SP_lOAD_EVENT_SLOT2,
	SP_lOAD_EVENT_SLOT3,
	SP_lOAD_EVENT_SLOT4,
	SP_lOAD_EVENT_SLOT5,
	SP_lOAD_EVENT_INVALID_ARGUMENT,
	SP_lOAD_EVENT_NONE,
	SP_LOAD_EVENT_QUIT
} SP_LOAD_EVENT;

//structure that represents load window

typedef struct{
	SDL_Window* windowLoad; //window name
	SDL_Renderer* rendererLoad; //window's renderer
	Button** buttonsList; //load window button's list
	int buttonsCounter; //num of buttons
}SPLoadWin;

/**
 *  function that checks whether there was a click on a button in load screen
 *
 *	@param x - the x coordinate of the click
 *	@param y - the y ccoordinate of the click
 *	@param src - the source load window
 *
 *  @return
 *  number that represents the button clicked
 *  -1 - if no button was clicked
 */
int isClickOnLoad(int x, int y,SPLoadWin* src);

/**
 *  Creates a load window, if an error occured, prints a suitable message.
 *
 *	@param num_of_saved_games - number of games saved until now.
 *
 *  @return
 *  NULL, if an allocation error occurred
 *  load window - otherwise
 */
SPLoadWin* spLoadWindowCreate(int num_of_saved_games);

/**
 *  function that draws the load window, prints a suitable message if src == NULL
 *
 *  @param src - the source load window
 *
 */
void spLoadWindowDraw(SPLoadWin*);

/**
 * Frees all memory resources associated with the source window. If the
 * source window is NULL, then the function does nothing.
 * @param src - the source window
 */
void spLoadWindowDestroy(SPLoadWin* src);

/**
 *  Handles the load window to the load event
 *
 *	@param event - the event that happened in the load window
 *	@param game - the source game
 *	@param src - the source load window
 *
 *  @return
 *  SP_lOAD_EVENT_INVALID_ARGUMENT - if one of the arguments is invalid
 *  otherwise -the relevant event
 */
SP_LOAD_EVENT spLoadWindowHandleEvent(SPLoadWin* src, SDL_Event* event);

/**
 *  Hides the relevant window
 *	@param src - the source load window
 *
 */
void spLoadWindowHide(SPLoadWin* src);

/**
 *  Shows the relevant window
 *	@param src - the source load window
 *
 */
void spLoadWindowShow(SPLoadWin* src);

/**
 *  Updates the buttons according to a click on a valid game slot
 *	@param src - the source load window
 *	@param event - the number of the game slot which was clicked
 *
 */
void activateAfterClick(SPLoadWin* src, int event);

/**
 *  initializes the buttons on load window
 *
 *	@param num_of_saved_games - how many games are saved in folder right now
 *	@param res - the source load window
 *
 *  @return
 *  true - if succeeded
 *  false -if an error occurred
 */
bool loadButtonsInit(SPLoadWin* res,int num_of_saved_games);


#endif /* GRAPHICS_LOADWIN_H_ */
