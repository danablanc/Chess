

#ifndef GRAPHICS_PANEL_H_
#define GRAPHICS_PANEL_H_

#include "Cell.h"

#define NUM_OF_SET_PANEL_BUTTONS 6

//WINDOW DEFINE
#define BOARD_START_X_PANEL 200
#define BOARD_BORDER 27
#define CELL_W 68
#define BOARD_END 572

//BUTTONS DIM
#define SET_PANEL_BUTTON_X 25
#define SET_PANEL_BUTTON_H 50
#define SET_PANEL_BUTTON_Y 75
#define SET_PANEL_BUTTON_W 150



typedef struct panel
{
	Button** buttonsList; //load window button's list
	Cell** cellsList;
	int buttonsCounter; //num of buttons
	int cellsCounter;
	bool isSetPanel;
	SDL_Renderer* renderer;
	SDL_Texture* panelTexture;
	SDL_Rect* location;
} Panel;


/**
 *  Creates a panel for gui mode, if an error occured, prints a suitable message.
 *
 *
 *  @return
 *  NULL, if an allocation error occurred
 *  panel - otherwise
 */
Panel* createPanel(SDL_Renderer* renderer,SDL_Surface* img,SDL_Rect* location, bool isSetPanel);

/**
 *  Creates cells for game panel - 64 cells eah cell represents board entry
 *
 *
 *  @return
 *  NULL, if an allocation error occurred
 *  cell array - otherwise
 */
Cell** createGamePanelCells(SDL_Renderer* renderer);

/**
 *  Creates buttons for settings panel - 6 buttons
 *
 *  @return
 *  NULL, if an allocation error occurred
 *  cell array - otherwise
 */
Button** createSetPanelButtons(SDL_Renderer* renderer);



/**
 * Frees all memory resources associated with the source button. If the
 * source button is NULL, then the function does nothing.
 * @param button - the source button
 */
void destroyPanel(Panel* panel);

/**
 * Draws the button according to the button state - is the button enabled or not
 * If the source button is NULL, then the function does nothing.
 *
 * @param button - the source button
 */
void drawPanel(Panel* panel);

#endif /* GRAPHICS_PANEL_H_ */
