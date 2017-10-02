
#include "CHMiniMaxNode.h"

int* minMax(CH_Game* currentGame, unsigned int maxDepth){
	assert(currentGame!=NULL); //asserts allocation
	int* move=NULL;
	CH_Game* copy=NULL;
	copy = gameCopy(currentGame);
	if(!copy) // allocation check
		return NULL;
	Node* node = nodeCreate(copy); //creates root node
	if(!node){ // allocation check
		gameDestroy(copy);
		return NULL;
	}
	move = (int*)malloc(CELL_SIZE*CELL_SIZE*sizeof(int));
	if(!move){ // allocation check
		gameDestroy(copy);
		nodeDestroy(node);
		return NULL;
	}
	minMaxRec(node, 0, maxDepth, getCurrentPlayer(copy)==playerPC(copy),INT_MIN,INT_MAX); //invokes the recursion of minimax
	if(node->best_move[0]==ALLOC_ERROR_FLAG){ // there was an allocation error
		gameDestroy(copy);
		nodeDestroy(node);
		free(move);
		return NULL;
	}
	for(int i=0;i<CELL_SIZE*CELL_SIZE;i++) //copies the best move
		move[i]=node->best_move[i];
	gameDestroy(copy); //destroying copy
	nodeDestroy(node); //destroying struct
	return move;
}


int minMaxRec(Node* src, int level, int maxDepth, bool maximize,int a, int b){
	assert(src!=NULL); //asserts allocation
	int val=0,move[CELL_SIZE*CELL_SIZE]={0},size=0;
	bool updated = false; // best move was not updated yet
	elem* element;
	int** arr = NULL;
	int check = callerWin(src->game_copy); // checks for winner
	if(check != 0) //halting cond - there is a winner
		return check; //return max or min value - according to the player
	if(level == maxDepth) //halting cond - max depth
		return scoreFunction(src->game_copy); //return the value of score function computed
	for(int i=0;i<GRID;i++){ //for every cell in the game board
		for(int j=0;j<GRID;j++){
			if(playerFig(src->game_copy,i,j)){ // checks if player's figure
				arr = possibleMoves(src->game_copy,i,j); // checks for all figure's possible moves
				if (arr == NULL){ // allocation error
					src->best_move[START_ROW_PLACE]= ALLOC_ERROR_FLAG;
					return ALLOC_ERROR_VAL;
				}
				size = numOfMoves(src->game_copy->gameBoard[i][j]); // maximal number of possible moves of that figure
				for(int k=0;(k<size)&&(a<b);k++){ //for each possible move and while alpha<beta - do
					if(arr[k][0]==NO_MOVES_FLAG) // no more moves
						break;
					move[START_ROW_PLACE]=i+1;move[START_COL_PLACE]=j+1;move[DST_ROW_PLACE]=arr[k][0]+1;move[DST_COL_PLACE]=arr[k][1]+1;
					element = transfer(src->game_copy,move); // make it an element
					if(element==NULL){ //alloc. error
						freeArray(arr,size);
						src->best_move[START_ROW_PLACE] = ALLOC_ERROR_FLAG;
						return ALLOC_ERROR_VAL;
					}
					doMove(src->game_copy,element); // makes the move on the board
					src->game_copy->game_status=updateStatus(src->game_copy); // updates the new status of the board
					val = minMaxRec(src, level+1, maxDepth, !maximize,a,b); //calls the recursion
					if(src->best_move[START_ROW_PLACE]==ALLOC_ERROR_FLAG){ // there was an allocation error.
						freeArray(arr,size);
						free(element);
						return ALLOC_ERROR_VAL;
					}
					if(level == 0){ //we got to the root - compute the value
						val = value(a,val,true); //computes the new max val
						if((val != a) || (!updated)){ //checks if val was modified or move was never updated, if so, need to update
							src->best_move[START_ROW_PLACE] = element->startRow; src->best_move[START_COL_PLACE] = element->startCol; //update
							src->best_move[DST_ROW_PLACE] = element->dstRow; src->best_move[DST_COL_PLACE] = element->dstCol; //update
							updated = true; // move was already updated
						}
					}
					if(maximize) // updates alpha/beta
						a = value(a,val,maximize); //computes the new alpha (maybe not changed)
					else
						b = value(b,val,maximize); //computes the new beta (maybe not changed)
					free(element); // frees element
					undoPrevMove(src->game_copy); //undo the move
				}
				freeArray(arr,size); // frees array of moves
			}
		}
	}
	if(maximize) // if max node - returns alpha, o/w - returns beta
		return a;
	return b;
}



Node* nodeCreate(CH_Game* src){
	Node* node =NULL;
	if(!src) //checks allocation
		return NULL;
	node = (Node*) malloc(sizeof(Node)); //allocates space for the SPFiarGame
	if(!node) //checks allocation
		return NULL;
	node->game_copy = src;
	for(int i=0;i<CELL_SIZE*CELL_SIZE;i++) //initializes the best move field
		node->best_move[i]=NO_MOVES_FLAG;
	node->player= src->currentPlayer;
	return node;
}


void nodeDestroy(Node* src){
	if(src != NULL){ //destroying struct if needed
		free(src);
	}
}


Node* nodeCopy(Node* src){
	Node* res = NULL;
	if(!src)
		return NULL;
	res = nodeCreate(src->game_copy);
	if(!res)
		return NULL;
	for(int i=0;i<CELL_SIZE*CELL_SIZE;i++)
		res->best_move[i] = src->best_move[i];
	res->player = src->player;
	return res;
}


int scoreFunction(CH_Game* src){
	assert(src!=NULL);
	int value=0;
	int vec[NUM_OF_PIECES]=VEC;
	value = callerWin(src);
	if(value != 0)
		return value;
	if(playerPC(src)==CH_PLAYER_1){
		for(int i=0; i<NUM_OF_PIECES; i++) //computes the scoring func according to the weight vector & the span array
			value += (src->white[i]*vec[i]) - (src->black[i]*vec[i]);
	}
	else{
		for(int i=0; i<NUM_OF_PIECES; i++) //computes the scoring func according to the weight vector & the span array
			value += (src->black[i]*vec[i]) - (src->white[i]*vec[i]);
	}
	return value;
}


int callerWin(CH_Game* src){
	assert(src!=NULL);
	if(checkWinner(src) == playerPC(src)) //checks if the pc won
		return INT_MAX;
	else if((checkWinner(src) != CH_TIE) && (checkWinner(src) != '\0'))
		//checks if the player won
		return INT_MIN;
	return 0;
}

int value(int a, int b, bool maximize){
	if(maximize) //checks if need to maximize or minimize
		return max(a,b);
	else
		return min(a,b);
}
