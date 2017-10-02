
#include "CHMiniMax.h"


int* miniMaxPcMove(CH_Game* currentGame, unsigned int maxDepth){
	if(!currentGame || maxDepth <= 0) //checks validity of input
		return NULL;
	return minMax(currentGame,maxDepth); //returns the suggested move
}

