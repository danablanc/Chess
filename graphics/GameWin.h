#ifndef GAMEWIN_H_
#define GAMEWIN_H_

#include <assert.h>
#include <stdio.h>
#include <math.h>
#include "../ConsoleMode.h"
#include "Panel.h"
#include "Button.h"
#include <time.h>


//enum that represents all events in game window
typedef enum {
	SP_GAME_EVENT_PLAYER_1_WON,
	SP_GAME_EVENT_PLAYER_2_WON,
	SP_GAME_EVENT_TIE,
	SP_GAME_EVENT_CHECK,
	SP_GAME_EVENT_QUIT,
	SP_GAME_EVENT_RESTART,
	SP_GAME_EVENT_LOAD,
	SP_GAME_EVENT_MAIN_MENU,
	SP_GAME_EVENT_INVALID_ARGUMENT,
	SP_GAME_EVENT_SAVE_EXIT,
	SP_GAME_EVENT_SAVE,
	SP_GAME_EVENT_SAVE_MAIN_MENU,
	SP_GAME_EVENT_UNDO,
	SP_GAME_EVENT_GET_MOVES,
	SP_GAME_EVENT_NONE
} SP_GAME_EVENT;


// MACROS

#define borderX(a) ((abs(a-800))<=(abs(a-200)) ? (770) : (230))
#define borderY(a) ((abs(a-600))<=(a) ? (570) : (30))
#define xValue(a,b,c) ((a-b)/c)
#define yValue(a,b,c) ((a-1-b)/(c-1))
#define dist(a,b) (sqrt((a*a)+(b*b)))


// BUTTONS DEFINING
#define RESTART_GAME_BUTTON 64
#define SAVE_GAME_BUTTON 65
#define LOAD_GAME_BUTTON 66
#define UNDO_GAME_BUTTON 67
#define MAIN_MENU_GAME_BUTTON 68
#define EXIT_GAME_BUTTON 69

// LOCATION ON WINDOW DEFINING
#define BOARD_START_X 227
#define BOARD_END_X 773
#define BOARD_START_Y 27
#define BOARD_END_Y 573

//WINDOW SIZES DEFINING
#define BOARD_PANEL_START_X 200
#define BOARD_PANEL_START_Y 1
#define BOARD_HEIGHT 600
#define BOARD_WIDTH 600
#define SET_WINDOW_START_X 0
#define SET_WINDOW_START_Y 0
#define SET_HEIGHT 600
#define SET_WIDTH 200
#define WINDOW_HIGHT 600
#define WINDOW_WIDTH 800

//BOARD SIZES DEFINING
#define BOARD_CELL_DIM 70
#define CELL_OFFSET 34
#define BOARD_MIN_X 230
#define BOARD_MAX_X 770
#define BOARD_MIN_Y 30
#define BOARD_MAX_Y 570

// ERRORS FLAG DEFINING
#define ERROR_FLAG -1

//DELAYS DEFINING
#define PC_MOVE_DELAY 1500

//COLORS DEFINING
#define BLACK 0
#define WHITE 255
#define BOARD_R 244
#define BOARD_G 208
#define BOARD_B 159

//INIT VALUES DEFINING
#define GAME_ARGS_INIT -1

//MESSAGE BOX STATUS DEFINING
#define CHECK_FLAG 0
#define RESTART_BUTTONID 1
#define QUIT_BUTTONID 2
#define YES_BUTTONID 1
#define NO_BUTTONID 2
#define CANCEL_BUTTONID 3


//IMAGES NUM INDEX DEFINING
#define WHITE_PAWN_IMG 0
#define WHITE_BISHOP_IMG 1
#define WHITE_ROOK_IMG 2
#define WHITE_KNIGHT_IMG 3
#define WHITE_QUEEN_IMG 4
#define WHITE_KING_IMG 5
#define BLACK_PAWN_IMG 6
#define BLACK_BISHOP_IMG 7
#define BLACK_ROOK_IMG 8
#define BLACK_KNIGHT_IMG 9
#define BLACK_QUEEN_IMG 10
#define BLACK_KING_IMG 11
#define RED_CELL_IMG 12
#define YELLOW_CELL_IMG 13
#define GREEN_CELL_IMG 14

//GET MOVES IMG CHARS
#define DEFAULT_CELL '3'
#define RED_CELL '1'
#define YELLOW_CELL '2'
#define GREEN_CELL '0'

// MOVE MACROS
#define QUIT 0
#define RESTART 1
#define NONE 2
#define NO_EVENT 555
#define DRAG_TIME 5

//OTHERS
#define HISTORY_MIN_LIMIT 2

//struct that represents game window
typedef struct{
	SDL_Window* window; //window
	SDL_Renderer* renderer; //renderer
	Panel* setPanel; //panel for the settings
	Panel* boardPanel; //panel for the game board
	CH_Game* game; //the chess game
	Cell* moving_cell; // the cell that is moving
	bool to_drag; // flag that represnts whether to drag or not
	// source and target place
	int target_x;
	int target_y;
	int source_x;
	int source_y;
	bool is_up_to_date; //flag if there is a need to offer to save the game
	bool check_printed;
}SPGameWin;


/**
 *  Creates a game window, if an error occured, prints a suitable message.
 *
 *	@param game - the source chess game
 *	@param board_images - all board images
 *
 *  @return
 *  NULL, if an allocation error occurred
 *  game window - otherwise
 */
SPGameWin* spGameWindowCreate(CH_Game* game,const char** board_images);

/**
 *  function that draws the game window, prints a suitable message if src == NULL
 *
 *  @param src - the source game window
 *
 */
void spGameWindowDraw(SPGameWin* src);

/**
 * Frees all memory resources associated with the source window. If the
 * source window is NULL, then the function does nothing.
 *
 * @param src - the source window
 */
void spGameWindowDestroy(SPGameWin* src);

/**
 *  Handles the game window according to the game event
 *
 *	@param event - the event that happened in the game window
 *	@param board_images - the board images that can be shown according to each event
 *	@param src - the source game window
 *
 *  @return
 *  SP_GAME_EVENT_INVALID_ARGUMENT - if one of the arguments is invalid
 *  otherwise -the relevant event
 */
SP_GAME_EVENT spGameWindowHandleEvent(SPGameWin* src, SDL_Event* event,const char** board_images);

/**
 *  Hides the relevant window
 *	@param src - the source game window
 *
 */
void spGameWindowHide(SPGameWin* src);

/**
 *  Shows the relevant window
 *	@param src - the source game window
 *
 */
void spGameWindowShow(SPGameWin* src);

/**
 *  function that checks whether there was a click on a butto/cell in window screen
 *
 *	@param x - the x coordinate of the click
 *	@param y - the y ccoordinate of the click
 *	@param src - the source game window
 *
 *  @return
 *  number that represents the button/cell clicked
 *  -1 - if no button was clicked
 */
int isClickOnGame(int x, int y,SPGameWin* src);

/**
 *  function that updates the board's textures that are shown according to the current state
 *
 *	@param panel - the relevant panel to be updated - board/settings
 *	@param game - the relevant chess game
 *	@param board_images - the board images that can be used during the update
 */
int boardUpdate(Panel* panel,CH_Game* game,const char** board_images);

/**
 *  function that gets a char that represents a game board cell and returns the relevant image to be presented on the board
 *
 *	@param ch - the char that represents a cell in the game board
 *	@param board_images - the board images that can be used
 *
 *	@return
 *	NULL - in case the cell is empty
 *	the relevant image - otherwise
 */
const char* cellToImage(char ch,const char** board_images);

/**
 *  function that updates the board after the player invokes the get moves command (right mouse click)
 *
 *	@param src - the source chess game
 *	@param row - the row of the piece that we want to see it's available moves
 *	@param col - the col of the piece that we want to see it's available moves
 *	@param board_images - the board images that can be used
 *
 *	@return
 *	false - in case of an error
 *	true - otherwise
 */
bool getMovesGui(SPGameWin* src,int row ,int col ,const char** board_images);

/**
 *  function that initializes the drag arguments to -1 (default values)
 *
 *	@param src - the source chess game
 */
void initDragArgs(SPGameWin* src);

/**
 *  function that gets a pixel and returns the index of the relevant cell
 *
 *	@param x - the x coordinate of the pixel
 *	@param y - the y coordinate of the pixel
 *
 *	@return
 *	the cell index
 *
 */
int pixelToIndex(int x, int y);

/**
 *  function that makes the computer move, in case of an error - prints a relevant message
 *
 *	@param src - the source game
 *
 *	@return
 *	0 - in case of an error
 *	1 - on success
 *
 */
int PCMove(SPGameWin* src,const char** board_images);

/**
 *  prints game status message according to the status of the game
 *
 *	@param src - the source game
 *
 *	@return
 *	-1 - in case of an error
 *	button id - otherwise
 *
 */
int statusMessege(SPGameWin* src);

/**
 *  prints exit message (would you like to save the game?)
 *
 *	@param src - the source game
 *	@param flag - true if the player wants to exit and false if the player wants to go to the main menu
 *
 *	@return
 *	SP_GAME_EVENT_QUIT - in case the player wants to quit
 *	SP_GAME_EVENT_MAIN_MENU - in case the player wants to go to the main menu
 *	SP_GAME_EVENT_SAVE - in case the player wants to save and quit
 *	SP_GAME_EVENT_SAVE_MAIN_MENU - in case the player wants to save and go to main menu
 *	SP_GAME_EVENT_NONE - otherwise
 *
 */
SP_GAME_EVENT exitMessege(SPGameWin* src, bool flag);

/**
 *  function that updates the undo button if needed
 *
 *	@param panel - the panel
 *	@param game - the source game
 *
 */
void undoUpdate(Panel* panel, CH_Game* game);

/**
 *  function that makes the computer move, in case of an error - prints a relevant message
 *
 *	@param src - the source game
 *	@param cell_src - the source cell of the event
 *	@param res - the place of the event
 *	@param board_images - the images of game board
 *
 *	@return
 *	0 - if need to quit
 *	1 - if need to restart
 *	2 - if none
 *
 */
int moving(SPGameWin* src,int cell_src,int res ,const char** board_images);

/**
 *  function that updates dragging params is needed
 *
 *	@param src - the source game
 *
 */
void drag(SPGameWin* src);

/**
 *  function that makes the necessary changes following button up event
 *
 *	@param src - the source game
 *	@param event - the source event
 *	@param res - the place of the event
 *	@param board_images - the images of game board
 *
 *	@return
 *	SP_GAME_EVENT
 *
 */
SP_GAME_EVENT buttonUp(SPGameWin* src, SDL_Event* event, int res, const char** board_images);

/**
 *  function that makes the necessary changes following button down event
 *
 *	@param src - the source game
 *	@param event - the source event
 *	@param res - the place of the event
 *	@param board_images - the images of game board
 *
 *	@return
 *	0 - if need to quit
 *	NO_EVENT - if no event
 *
 */
int buttonDown(SPGameWin* src, SDL_Event* event, int res, const char** board_images);

#endif
