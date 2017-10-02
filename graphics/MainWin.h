#ifndef MAINWIN_H_
#define MAINWIN_H_

#include <stdio.h>
#include "Button.h"


// BUTTONS DEFINING
#define MAIN_START_BUTTON 0
#define MAIN_LOAD_BUTTON 1
#define MAIN_QUIT_BUTTON 2

#define NUM_OF_MAIN_BUTTONS 3

//WINDOW SIZES
#define MAIN_WINDOW_H 450
#define MAIN_WINDOW_W 400

//COLORS DEFINING
#define MAIN_R 244
#define MAIN_G 208
#define MAIN_B 159

//BUTTONS DIM
#define MAIN_X 75
#define MAIN_Y 25
#define MAIN_OFFSET 25
#define MAIN_H 100
#define MAIN_W 250

//FLAGS
#define ERROR_FLAG_MAIN -1

//OTHER
#define MAIN_ARGS_INIT -1

//enum that represents all main events possible
typedef enum {
	SP_MAIN_EXIT,
	SP_MAIN_START,
	SP_MAIN_INVALID_ARGUMENT,
	SP_MAIN_NONE,
	SP_MAIN_LOAD
} SP_MAIN_EVENT;

//structure that represents main window

typedef struct {
	SDL_Window* mainWindow;//window name
	SDL_Renderer* mainRenderer;//window's renderer
	Button** buttonsList;//main window button's list
	int buttonsCounter;//num of buttons
} SPMainWin;

/**
 *  function that checks whether there was a click on a button in main screen
 *
 *	@param x - the x coordinate of the click
 *	@param y - the y ccoordinate of the click
 *	@param src - the source main window
 *
 *  @return
 *  number that represents the button clicked
 *  -1 - if no button was clicked
 */
int isClickOnMain(int x, int y,SPMainWin* src);

/**
 *  Creates a main window, if an error occured, prints a suitable message.
 *
 *  @return
 *  NULL, if an allocation error occurred
 *  main window - otherwise
 */
SPMainWin* spMainWindowCreate();

/**
 *  function that draws the main window, prints a suitable message if src == NULL
 *
 *  @param src - the source main window
 *
 */
void spMainWindowDraw(SPMainWin* src);

/**
 * Frees all memory resources associated with the source window. If the
 * source window is NULL, then the function does nothing.
 * @param src - the source window
 */
void spMainWindowDestroy(SPMainWin* src);

/**
 *  Hides the relevant window
 *	@param src - the source main window
 *
 */
void spMainWindowHide(SPMainWin* src);

/**
 *  Shows the relevant window
 *	@param src - the source main window
 *
 */
void spMainWindowShow(SPMainWin* src);

/**
 *  Handles the main window to the main event
 *
 *	@param src - the source main window
 *
 *  @return
 *  SP_MAIN_EVENT_INVALID_ARGUMENT - if one of the arguments is invalid
 *  otherwise -the relevant event
 */
SP_MAIN_EVENT spMainWindowHandleEvent(SPMainWin* src, SDL_Event* event);

#endif
