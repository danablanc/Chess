
#ifndef XMLPARSER_H_
#define XMLPARSER_H_

#include <unistd.h>
#include "ChessGame.h"
#include "CHSettingsParser.h"
#include "CHGameParser.h"


typedef enum xml_messages{
    XML_SUCCESS,
    XML_FAILED,
	XML_ALLOC,
} XML_MESSAGE;

/**
 * gets a game and writes to file the game params according to the Conventions
 *
 * @param game - the game that should be saved
 * @param f - the dst file
 *
 * @return
 * XML_FAILED - in case of an error
 * XML_SUCCESS - otherwise
 */
XML_MESSAGE gameToFile(CH_Game* src,FILE* xml);

/**
 * returns a string that represent a gameboard row
 *
 * @param src - the game that should be saved
 * @param row - the line that needs to be written to file
 *
 * @return
 * 8 chars string that represent the row
 */
char* boardRowToLine(CH_Game* src,int row);

/**
 * gets a file and parser each line that represents game's params
 *
 * @param f - the src file
 *
 * return - the game that the file represents
 */
CH_Game* fileToGame(FILE* f);

#endif /* XMLPARSER_H_ */
