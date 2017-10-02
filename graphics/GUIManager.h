#ifndef GUIMANAGER_H_
#define GUIMANAGER_H_

#include <stdio.h>
#include <stdlib.h>
#include "SettingWin.h"
#include "GameWin.h"
#include "LoadWin.h"
#include "MainWin.h"

#define MAX_OF_SAVED_FILES 5
#define NUM_OF_IMAGES 15

#define FILE_SLOT_1 0
#define FILE_SLOT_2 1
#define FILE_SLOT_3 2
#define FILE_SLOT_4 3
#define FILE_SLOT_5 4


//enum that represents all existing windows
typedef enum{
	SP_MAIN_WINDOW,
	SP_GAME_WINDOW,
	SP_LOAD_WINDOW,
	SP_SETTINGS_WINDOW
}WINDOWS_LIST;

//enum that represents events like quit window

typedef enum{
	SP_MANAGER_QUIT,
	SP_MANAGER_NONE,
}SP_MANAGER_EVENET;

//structre that represents the gui manager
typedef struct {
	SPGameWin* gameWin; //game window
	SPMainWin* mainWin; //main window
	SPLoadWin* loadWin; //load window
	SPSettingsWin* settingsWin; //settings window
	WINDOWS_LIST activeWin; //which window is active at the moment
	WINDOWS_LIST prevWin; //previous window
	CH_Game* game; //chess game
	const char* board_images[NUM_OF_IMAGES];
	const char* saved_games[MAX_OF_SAVED_FILES];

} SPGuiManager;

/**
 *  Creates a gui manager, if an error occured, prints a suitable message.
 *
 *  @return
 *  NULL, if an allocation error occurred
 *  gui manager - otherwise
 */
SPGuiManager* spManagerCreate();

/**
 * Frees all memory resources associated with the source gui manager. If the
 * source gui manager is NULL, then the function does nothing.
 * @param src - the source gui manager
 */
void spManagerDestroy(SPGuiManager* src);

/**
 * Draws the active window, prints a suitable message if an error occured
 *
 * @param src - the source gui manager
 *
 */
void spManagerDraw(SPGuiManager* src);

/**
 *  Handles the different events from the different windows.
 *  handles manager due to manager events (show/hide window)
 *  prints an error message if an allocation error occured
 *
 * @param src - the source gui manager
 * @param event - sdl event
 *
 *  @return
 *  SP_MANAGER_QUTT, if there was a click on X or there was an allocation error
 *  SP_MANAGER_NONE, if src==NULL or event that wasn't mentioned above
 */
SP_MANAGER_EVENET spManagerHandleEvent(SPGuiManager* src, SDL_Event* event) ;

/**
 *  function that handles all the events occurrs on game window
 *
 * @param src - the source gui manager
 * @param event - the event occurred
 *
 * @return
 * SP_MANAGER_EVENET
 *
 */
SP_MANAGER_EVENET handleManagerDueToGameEvent(SPGuiManager* src,
		SP_GAME_EVENT event);


/**
 *  function that makes the needed changes following load event click
 *
 * @param src - the source gui manager
 *
 * @return
 * SP_MANAGER_EVENET
 *
 */
SP_MANAGER_EVENET loadEvent(SPGuiManager* src);

/**
 *  Handles the different events that load has.
 *  handles manager due to load events (show/hide window)
 *  prints an error message if an allocation error occured
 *
 * @param src - the source gui manager
 * @param event - game event
 *
 *  @return
 *  SP_MANAGER_QUTT, if there was a click on X or there was an allocation error
 *  SP_MANAGER_NONE, if src==NULL or event that wasn't mentioned above
 */
SP_MANAGER_EVENET handleManagerDueToLoadEvent(SPGuiManager* src,
		SP_LOAD_EVENT event);

/**
 *  Handles the different events that main has.
 *  handles manager due to main events (show/hide window)
 *  prints an error message if an allocation error occured
 *
 * @param src - the source gui manager
 * @param event - main event
 *
 *  @return
 *  SP_MANAGER_QUTT, if there was a click on X or there was an allocation error
 *  SP_MANAGER_NONE, if src==NULL or event that wasn't mentioned above
 */
SP_MANAGER_EVENET handleManagerDueToMainEvent(SPGuiManager* src,
		SP_MAIN_EVENT event);

/**
 *  Handles the different events that settings has.
 *  handles manager due to settings events (show/hide window)
 *  prints an error message if an allocation error occured
 *
 * @param src - the source gui manager
 * @param event - settings event
 *
 *  @return
 *  SP_MANAGER_QUTT, if there was a click on X or there was an allocation error
 *  SP_MANAGER_NONE, if src==NULL or event that wasn't mentioned above
 */
SP_MANAGER_EVENET handleManagerDueToSetEvent(SPGuiManager* src,
		SP_SETTINGS_EVENT event);

/**
 *  Draws next settings sub screen according to the flow
 *  Prints suitable message if src == NULL
 *
 * @param src - the source gui manager
 *
 */
void drawSetNextScreen(SPGuiManager* src);

/**
 *  Draws previous settings sub screen according to the flow
 *  Prints suitable message if src == NULL
 *
 * @param src - the source gui manager
 *
 */
void drawSetPrevScreen(SPGuiManager* src);

/**
 *  init the game board images gui manager field
 *
 * @param src - the source gui manager
 *
 */
void updateImages(SPGuiManager* src);

/**
 *  function that computes number of saved games
 *
 * @param src - the source gui manager
 *
 * @return
 * number of games that the user saved
 *
 */
int num_of_saved_files(SPGuiManager* src);

/**
 *  function that saves the gui game board to xml file to be used in the future
 *
 * @param src - the source gui manager
 *
 * @return
 * XML_SUCCESS - on success of save
 * XML_FAILED - otherwise
 */
XML_MESSAGE saveGameGui(SPGuiManager* src);

/**
 *  function that saves the gui game board to xml file to be used in the future
 *
 * @param src - the source gui manager
 *
 * @return
 * XML_SUCCESS - on success of save
 * XML_FAILED - otherwise
 */
void saveUpdate(SPGuiManager* src);

/**
 *  function that initializes all save games paths
 *
 * @param src - the source gui manager
 *
 */
void initSaves(SPGuiManager* src);

/**
 *  function that undoes the previous two moves on the game board
 *
 * @param src - the source gui manager
 *
 */
void undoGameGui(SPGuiManager* src);

/**
 *  function that restarts the game board
 *
 * @param src - the source gui manager
 *
 * @return
 * SP_MANAGER_EVENET
 *
 */
SP_MANAGER_EVENET gameRestart(SPGuiManager* src);

#endif
