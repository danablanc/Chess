
#ifndef CONSOLEMODE_H_
#define CONSOLEMODE_H_

#include "CHMiniMax.h"
#include "XMLParser.h"
#include <assert.h>

#define EXPERT_LEVEL 5
#define TWO_PLAYER_MODE 2

// MACRO

#define toChar(a) (a+65)

/**
 * invokes a new game according to the game settings in console mode
 *
 */
void startGame();
/**
 * invokes the settings state - the user enters a valid command:
 * game_mode, difficulty, user_color, load, quit, default or print setting
 * until the start command is entered.
 *
 */
void settingState(CH_Game* game);

/**
 * invokes the game state - the user enters a valid game command:
 * move, get moves, save, undo reset
 * until the quit command is entered.
 *
 * @param game - th src game
 * @param command - the game command the user entered
 * @return
 * false in case of an error
 * otherwise - true
 */
bool gameState(CH_Game* game, GameCommand command);

/**
 * returns the set command that the user entered after being parsed
 *
 * @return - setcommand as listed in the enum
 */
SetCommand getNewSetCommand();

/**
 * returns the gamme command that the user entered after being parsed
 *
 * @param game - the src game
 * @param to_print - flag that determins whether to print the board or not
 *
 * @return - setcommand as listed in the enum
 */
GameCommand getNewGameCommand(CH_Game* game,bool to_print);

/**
 * changes the game mode according to the user's wishes, prints a suitable message
 *
 * @param game - the src game
 * @param arg - the game mode that the user wants
 *
 * no return - a void function
 */
void gameMode(CH_Game* game, int arg);

/**
 * changes the game difficulty according to the user's wishes, prints a suitable message
 *
 * @param game - the src game
 * @param arg - the game difficulty that the user wants
 *
 * no return - a void function
 */
void gameDifficulty(CH_Game* game, int arg);

/**
 * changes the game difficulty according to the user's wishes, prints a suitable message
 *
 * @param game - the src game
 * @param arg - the game difficulty that the user wants - only 0 or 1
 *
 * no return - a void function
 */
void gameColor(CH_Game* game, int arg);

/**
 * loads a game from the file that the user specified and prints a message if there was an error
 *
 * @param game - the src game
 * @param path - the file path that the user specifies
 *
 * no return - a void function
 */
void gameLoad(CH_Game* game, char* path);

/**
 * sets the game to default params
 *
 * @param game - the src game
 *
 * no return - a void function
 */
void gameDefault(CH_Game* game);

/**
 * prints the game settings according to the mode
 *
 * @param game - the src game
 *
 * no return - a void function
 */
void gamePrintSetting(CH_Game* game);

/**
 * quits the game and frees all resources
 *
 * @param game - the src game
 *
 * @param check - boolean
 *
 * no return - a void function
 */
void gameQuit(CH_Game *game,bool check);

/**
 * sets a game move - moves the players piece according to the gives move.
 * checks whether the move is valid and prints a message accordingly
 *
 * @param game - the src game
 * @param move - an array that represent the src cell and dst cell
 *
 * @return
 * false - if the move cannot be done
 * true - if the move is legal
 */
bool gameMove(CH_Game* game, int* move);

/**
 * prints all possible game moves of the given piece
 * checks whether the invoke of the command is valid and prints accordingly
 *
 * @param game - the src game
 * @param move - an array that represent the piece's cell
 *
 * @return
 * false - if the command is illegal
 * true - if the command is legal
 */
bool gameGetMoves(CH_Game* game, int* move);

/**
 * saves the current game to the given path
 * tries to do the command is valid and prints accordingly
 *
 * @param game - the src game
 * @param path - represents the dst file
 *
 * @return
 * false - if the command failed
 * true - o.w
 */
bool gameSave(CH_Game* game, char* path);

/**
 * undo the 2 last moves in the game.
 *
 * @param game - the src game
 *
 * @return
 * false - if the command failed
 * true - o.w
 */
bool gameUndoMove(CH_Game* game);

/**
 * resets the game to the setting mode
 *
 * @param game - the src game
 *
 * @return
 * void function - no return
 */
void gameReset(CH_Game* game);

/**
 * prints a message if there was an allocation error
 *
 * @param game - the src game
 * @param bool - flag that says whether to destroy the game or not
 *
 * @return
 * void function - no return
 */
void fail(CH_Game *game,bool destroy);

/**
 * function that calls the minimax and computes the best move for the computer
 * the function makes that move and prints the information
 *
 * @param game - the src game
 *
 * @return
 * void function - no return
 */
void computerMove(CH_Game* game);

/**
 * managment of one player  mode:
 * gets the commands, checks whether there was a winner and executes the command given by the user
 * makes the computer move
 *
 * @param game - the src game
 *
 * @return
 * void function - no return
 */
void onePlayerMode(CH_Game* game);

/**
 * managment of two player  mode:
 * gets the commands, checks whether there was a winner and executes the command given by the user
 *
 * @param game - the src game
 *
 * @return
 * void function - no return
 */
void twoPlayerMode(CH_Game* game);

/**
 * the function returns the player's colour according to the char that it represents
 *
 * @param ch - the players char
 *
 * @return
 * black - if ch='F'
 * white - if ch='S'
 */
char* color(char ch);

/**
 * the function returns the figure's name according to the char that it represents
 *
 * @param ch - the players char
 *
 * @return
 * rook - R/r
 * bishop - b/B
 * queen - q/Q
 * king - k/K
 * knight - N/n
 * pawn- m/M
 * NULL - otherwise
 */
char* figure(char ch);

/**
 * checks whether the game status changed to tie/check/checkmate and prints a message accordingly
 *
 * @param game - the src game
 *
 * @return
 * void function - no return
 */
void statusCheck(CH_Game* game);


#endif /* CONSOLEMODE_H_ */
