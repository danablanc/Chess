

#ifndef CHGAMEPARSER_H_
#define CHGAMEPARSER_H_
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

#define SP_MAX_LINE_LENGTH 1024 //specify the maximum line length
#define CELL_SIZE 2 //specify the number of coordinates that represent a cell
#define PATH_COMMANDS_FLAG_GAME 3
#define MOVES_COMMANDS_FLAG 2
#define ASCII_OFFSET 64
#define INVALID_POSITION_FLAG 9


//a type used to represent a command
typedef enum {
	CH_GINVALID_LINE,
	CH_MOVE,
	CH_SAVE,
	CH_UNDO,
	CH_RESET,
	CH_GET_MOVES,
	CH_GQUIT,
} GAME_COMMAND_NAME;

//a new type that is used to encapsulate a parsed line
typedef struct command_gt {
	GAME_COMMAND_NAME cmd;
	bool validArg; //is set to true if the line contains a valid argument
	char path[SP_MAX_LINE_LENGTH]; //argument for a command with a folder path
	int move[CELL_SIZE*CELL_SIZE]; //move is represented via 2 cells = 4 coordinates
} GameCommand;

/**
 * Checks if a specified string represents a valid tuple (move).
 *
 * @return
 * true if the string represents a valid tuple, and false otherwise.
 */
bool parserIsTuple(const char* str,GameCommand *curr, int cell);

/**
 * Parses a specified line.
 * If the line is a command which has an argument that represents a path then the argument is parsed
 * and is saved in the field path and the field validArg is set to true.
 * If the line is a command which has an argument that represents a move then the argument is parsed
 * and is saved in the field move and the field validArg is set to true.
 * In any other case then 'validArg' is set to false and the values  'path' and 'move' are undefined
 *
 * @return
 * A parsed line such that:
 *   cmd - contains the command type, if the line is invalid then this field is
 *         set to INVALID_LINE
 *   validArg - is set to true if the command has a relevant extra information: path, number or move.
 *   path - the path argument in case validArg is set to true
 *   move - the move argument in case validArg is set to true
 */
GameCommand parserGamePraseLine(char* str);


/**
* checks which command the user wrote. The function gets a string that represents the command,
*  if the command is unknown  returns INVALID_LINE
* @return
* Enum that represents the command - if the command is valid
* CH_GINVALID_LINE - otherwise
*/

GAME_COMMAND_NAME parserIsGameCommand(const char* str);


#endif /* CHGAMEPARSER_H_ */
