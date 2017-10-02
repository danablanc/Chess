#ifndef CHSETTINGSPARSER_H_
#define CHSETTINGSPARSER_H_
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

#define SP_MAX_LINE_LENGTH 1024 //specify the maximum line length
#define PATH_COMMANDS_FLAG 2
#define INT_COMMANDS_FLAG 1
//a type used to represent a command
typedef enum {
	CH_INVALID_LINE,
	CH_GAME_MODE,
	CH_DIFFICULTY,
	CH_USER_COLOR,
	CH_LOAD,
	CH_DEFAULT,
	CH_QUIT,
	CH_START,
	CH_PRINT_SETTING
} SET_COMMAND_NAME;

//a new type that is used to encapsulate a parsed line
typedef struct command_t {
	SET_COMMAND_NAME cmd;
	bool validArg; //is set to true if the line contains a valid argument
	int arg; //argument for a command with a number (for example difficulty etc.)
	char path[SP_MAX_LINE_LENGTH];
} SetCommand;

/**
* checks which command the user wrote. The function gets a string that represents the command,
*  if the command is unknown  returns INVALID_LINE
* @return
* Enum that represents the command - if the command is valid
* CH_INVALID_LINE - otherwise
*/
SET_COMMAND_NAME parserIsSetCommand(const char* str);

/**
 * Checks if a specified string represents a valid integer.
 *
 * @return
 * true if the string represents a valid integer, and false otherwise.
 */
bool parserIsInt(const char* str);

/**
 * Parses a specified line.
 * If the line is a command which has an integer argument then the argument is parsed
 * and is saved in the field arg and the field validArg is set to true.
 * If the line is a command which has an argument that represents a path then the argument is parsed
 * and is saved in the field path and the field validArg is set to true.
 * If the line is a command which has an argument that represents a move then the argument is parsed
 * and is saved in the field move and the field validArg is set to true.
 * In any other case then 'validArg' is set to false and the values 'arg', 'path' and 'move' are undefined
 *
 * @return
 * A parsed line such that:
 *   cmd - contains the command type, if the line is invalid then this field is
 *         set to INVALID_LINE
 *   validArg - is set to true if the command has a relevant extra information: path, number or move.
 *   arg      - the integer argument in case validArg is set to true
 */
SetCommand parserSetPraseLine(char* str);



#endif
