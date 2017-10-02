
#include "ChessGame.h"


CH_Game* gameCreate(int historySize,int diff,int col, int mode){
    if(historySize <= 0)
    	return NULL;
    CH_Game* new = (CH_Game*)malloc(sizeof(CH_Game)); //create new game
    if(!new)
    	return NULL; // allocation failed
    new->undo_hist = spArrayListCreate(historySize);
    if(!(new->undo_hist)){
        free(new);
        return NULL;
    }
    argInitialize(new); // initialize default arguments
    boardInitialize(new); //initialize gameBoard
    new->mode = mode; //game mode
    new->user_color = col; //player's color
    new->difficulty = diff; //game difficulty
    new->currentPlayer = CH_PLAYER_1; //player 1 starts
    return new;
}

void argInitialize(CH_Game* game){
	//initialize game params
	assert(game!=NULL);
	game->white[PAWN_INDEX]=game->black[PAWN_INDEX]=PAWN_NUM;
	game->white[KNIGHT_INDEX]=game->white[BISHOP_INDEX]=game->white[ROOK_INDEX]=B_R_K_NUM;
	game->black[KNIGHT_INDEX]=game->black[BISHOP_INDEX]=game->black[ROOK_INDEX]=B_R_K_NUM;
	game->white[QUEEN_INDEX]=game->white[KING_INDEX]=K_Q_NUM;
	game->black[QUEEN_INDEX]=game->black[KING_INDEX]=K_Q_NUM;
	game->black_king[START_ROW_PLACE]=(GRID-1); game->black_king[START_COL_PLACE]=(CELL_SIZE*CELL_SIZE);
	game->white_king[START_ROW_PLACE]=(GRID-GRID); game->white_king[START_COL_PLACE]=(CELL_SIZE*CELL_SIZE);
	game->game_status = REGULAR;
}

void boardInitialize(CH_Game *game){
	assert(game != NULL);
	for(int i=1; i<GRID-1;i++){ //initialize gameBoard
		for(int j=0;j<GRID;j++){
			if(i==1)
	        	game->gameBoard[i][j] = PAWN_W; //white pawns -whole row
	        else if(i==6)
	        	game->gameBoard[i][j] = PAWN_B; //black pawns - whole row
	       	else
	       		game->gameBoard[i][j] = CH_EMPTY_ENTRY;//no pieces in the middle of the board
	    }
    }
	    //initialize all the pieces which are not pawn - USING NUMBERS IN INITIALIZIATION WITH MOAB'S PERMISSION!
	   game->gameBoard[0][0]= ROOK_W;
	   game->gameBoard[7][0]= ROOK_B;
	   game->gameBoard[0][1]= KNIGHT_W;
	   game->gameBoard[7][1]= KNIGHT_B;
	   game->gameBoard[0][2]= BISHOP_W;
	   game->gameBoard[7][2]= BISHOP_B;
	   game->gameBoard[0][3]= QUEEN_W;
	   game->gameBoard[7][3]= QUEEN_B;
	   game->gameBoard[0][4]= KING_W;
	   game->gameBoard[7][4]= KING_B;
	   game->gameBoard[0][5]= BISHOP_W;
	   game->gameBoard[7][5]= BISHOP_B;
	   game->gameBoard[0][6]= KNIGHT_W;
	   game->gameBoard[7][6]= KNIGHT_B;
	   game->gameBoard[0][7]= ROOK_W;
	   game->gameBoard[7][7]= ROOK_B;
}


CH_Game* gameCopy(CH_Game* src){
    if(!src)
    	return NULL;
    CH_Game* gamecopy = (CH_Game*)malloc(sizeof(CH_Game));
    if (!gamecopy)
    	return NULL;
    gamecopy->undo_hist = spArrayListCopy(src->undo_hist);
    if(gamecopy->undo_hist == NULL){ //initializing failed
        free(gamecopy);
        return NULL;
    }
    copyParameters(src,gamecopy); //copies params from src to gamecopy
    return gamecopy;
}

void gameDestroy(CH_Game* src){
	if(src){	// destroying struct if needed
		spArrayListDestroy(src->undo_hist);
		free(src);
	}
}

int setMove(CH_Game* src, int* move){
	elem* element;
	int res =0;
	if((!src) || (!move)) //checks validity of given arguments
		return 1;
	for (int i=0;i<CELL_SIZE*CELL_SIZE;i++)
		if (move[i]<1 || move[i]>GRID) //the moves are inside board's borders
			return 1;
	element = transfer(src,move);
	if(element == NULL) //  mem alloc err.
		return ALLOCATION_ERROR;
	res = isValidMove(src,element);
	if(res == ALLOCATION_ERROR){
		free(element);
		return ALLOCATION_ERROR;
	}
	if(res != 1){//checks validity of the move
		free(element);
		return res;
	}
	doMove(src,element); //does the move
	src->game_status = updateStatus(src); //updates game status
	free(element);
	return 0;
}

int isValidMove(CH_Game* src , elem* element){
	assert(element!=NULL); // checks validity of arguments
	assert(src!=NULL);
	int row=0,col=0;
	if(element->fig == CH_EMPTY_ENTRY){ // checks if there is a figure in start point
		return ILLEGAL_FIGURE;}
	if(!playerFig(src,element->startRow,element->startCol)){ // checks if figure is current player's figure
		return ILLEGAL_FIGURE;}
	if(!figureMove(src,element)){ // checks if there is a valid figure move
		return ILLEGAL_MOVE;}
	if(!noOverlap(src,element)){ // checks if there is a overlap
		return ILLEGAL_MOVE;}
	CH_Game* copy = gameCopy(src);
	if(copy==NULL) //there is a mem alloc err.
		return ALLOCATION_ERROR;
	doMove(copy,element);
	switchPlayer(copy);
	if(copy->currentPlayer == CH_PLAYER_1){
		row = copy->white_king[0];
		col = copy->white_king[1];
	}
	else{
		row = copy->black_king[0];
		col = copy->black_king[1];
	}
	if(pieceUnderAttack(copy,row,col)){ //checks if making the move puts the player in Check status
		gameDestroy(copy);
		return ILLEGAL_MOVE;
	}
	gameDestroy(copy);
	return 1;
}

elem* transfer(CH_Game* src ,int* move){
	assert(move!=NULL); // checks validity of arguments
	assert(src!=NULL);
	elem* element;
	element = (elem*)malloc(sizeof(elem)); // memory allocation for move struct
	if(!element) // checks allocation
		return NULL;
	element->dstRow = move[(CELL_SIZE*CELL_SIZE)-DST_ROW_PLACE]-1; // sets move values in struct
	element->dstCol = move[(CELL_SIZE*CELL_SIZE)-START_COL_PLACE]-1;
	element->startRow = move[(CELL_SIZE*CELL_SIZE)-DST_COL_PLACE-1]-1;
	element->startCol = move[(CELL_SIZE*CELL_SIZE)-DST_COL_PLACE]-1;
	element->fig = src->gameBoard[element->startRow][element->startCol]; //the figure that the player wants to move
	element->gameStatus = statusToInt(src->game_status); //current game status
	element->player = getCurrentPlayer(src);
	return element;
}

bool playerFig(CH_Game* src, int row, int col){
	assert(src!=NULL);
	if (src->gameBoard[row][col]==CH_EMPTY_ENTRY) //asserts there's some fig in the cell
		return false;
	if(src->currentPlayer == CH_PLAYER_1){
		if(isupper(src->gameBoard[row][col]))
			return false;
	}
	else{
		if(!isupper(src->gameBoard[row][col]))
			return false;
	}
	return true;
}

bool figureMove(CH_Game* src, elem* element){
	assert(src != NULL);
	assert(element != NULL);
	switch(element->fig){ //checks  if the figure can move in the specific direction the user entered
	case PAWN_B:
	case PAWN_W: //checks for pawn
		return pawnMove(src,element);
	case BISHOP_B:
	case BISHOP_W: //checks for bishop
		return bishopMove(element);
	case ROOK_W:
	case ROOK_B: //checks for rook
		return rookMove(element);
	case KNIGHT_W:
	case KNIGHT_B: //checks for knight
		return knightMove(element);
	case QUEEN_W:
	case QUEEN_B: //checks for queen
		return queenMove(element);
	case KING_W:
	case KING_B: //checks for king
		return kingMove(element);
	}
	return false;
}

bool gameAssign(CH_Game* src,CH_Game* gamecopy){
    if(!src)
    	return false;
    if (!gamecopy)
    	return false;
	copyParams(src->undo_hist,gamecopy->undo_hist);//copies undo hist params
    copyParameters(src,gamecopy);//copies params from src to gamecopy
    return true;
}

void copyParameters(CH_Game* src,CH_Game* gamecopy){
	assert(src!=NULL);
	assert(gamecopy!=NULL);
    for(int i=0;i<GRID;i++) //copies the gameBoard
        for(int j=0;j<GRID;j++)
            (gamecopy->gameBoard)[i][j] = (src->gameBoard)[i][j];
    for(int i=0; i<NUM_OF_PIECES;i++){ // copies number of pieces that the player has
    	gamecopy->black[i] = src->black[i];
    	gamecopy->white[i] = src->white[i];
    }
    //copies all left parameters
    gamecopy->currentPlayer = getCurrentPlayer(src);
    gamecopy->mode = src->mode;
    gamecopy->black_king[0] = src->black_king[0];
    gamecopy->black_king[1] = src->black_king[1];
    gamecopy->white_king[0] = src->white_king[0];
    gamecopy->white_king[1] = src->white_king[1];
    gamecopy->difficulty = src->difficulty;
    gamecopy->user_color = src->user_color;
    gamecopy->game_status = src->game_status;
}

CH_GAME_MESSAGE gamePrintBoard(CH_Game* src){
	if (!src)
		return CH_INVALID_ARGUMENT;
	for(int i=GRID-1; i >= 0; i--){
		printf("%d| ",i+1);
		for(int j=0; j<GRID;j++){
			printf("%c ",src->gameBoard[i][j]); //prints the gameBoard
		}
		printf("|\n");
	}
	printf("  ");
	for(int i=0; i<((2*GRID)+1); i++) //prints the sign "-"
		printf("-");
	printf("\n");
	printf("   ");
	for(int i=0; i<GRID-1; i++) //prints the number of columns
		printf("%c ",'A'+i);
	printf("H");
	printf("\n");
	return CH_SUCCESS;
}


char getCurrentPlayer(CH_Game* src){
	if(!src)
		return CH_EMPTY_ENTRY;
	else
		return src->currentPlayer;
}

bool isOppPiece(CH_Game* src, int row, int col){
	assert(src!=NULL);
	char piece=src->gameBoard[row][col];
	if (getCurrentPlayer(src)==CH_PLAYER_2)
		return (piece==PAWN_W || piece==BISHOP_W || piece==ROOK_W || piece==KNIGHT_W || piece==QUEEN_W || piece==KING_W);
	else return (piece==PAWN_B || piece==BISHOP_B || piece==ROOK_B || piece==KNIGHT_B || piece==QUEEN_B || piece==KING_B);
}

bool isCellEmpty(CH_Game* src, int row, int col){
	assert(src!=NULL);
	if (col<0 ||col>(GRID-1) ||row<0 ||row>(GRID-1))
		return false;
	return src->gameBoard[row][col] == CH_EMPTY_ENTRY ;
}


CH_GAME_MESSAGE undoPrevMove(CH_Game* src){
	int scol=0, srow=0, dcol=0, drow=0;
	if(!src)
		return CH_INVALID_ARGUMENT;
	if(spArrayListIsEmpty(src->undo_hist)) //checks if the move can be done
		return CH_NO_HISTORY;
	scol = spArrayListGetLast(src->undo_hist)->startCol; //where to move back the figure (col)
	srow = spArrayListGetLast(src->undo_hist)->startRow; //where to move back the figure (row)
	dcol = spArrayListGetLast(src->undo_hist)->dstCol; //where to place the figure that was captured (col)
	drow = spArrayListGetLast(src->undo_hist)->dstRow; //where to place the figure that was captured (row)
	kingUpdate(src,spArrayListGetLast(src->undo_hist)->fig,spArrayListGetLast(src->undo_hist)->startRow,
			spArrayListGetLast(src->undo_hist)->startCol); //update's king's place
	src->gameBoard[srow][scol]= spArrayListGetLast(src->undo_hist)->fig; //moving back the figure
	src->gameBoard[drow][dcol]= spArrayListGetLast(src->undo_hist)->prevFig; //restore the captured figure
	src->game_status = intToStatus(spArrayListGetLast(src->undo_hist)->gameStatus);
	switchPlayer(src); //switch player
	piecesUpdate(src,spArrayListGetLast(src->undo_hist)->prevFig,true,getCurrentPlayer(src));
	spArrayListRemoveLast(src->undo_hist); //remove from history list;
	return CH_SUCCESS;
}


CH_STATUS intToStatus(int n){
	//represent game status by enum
	if(n==CHECK_REP)
		return CHECK;
	if(n==CHECKMATE_REP)
		return CHECKMATE;
	if(n==TIE_REP)
		return TIE;
	return REGULAR;
}

int statusToInt(CH_STATUS st){
	//represent game status by number
	if(st == CHECK)
		return CHECK_REP;
	if(st == CHECKMATE)
		return CHECKMATE_REP;
	if(st == TIE)
		return TIE_REP;
	return REG_REP;
}


char checkWinner(CH_Game* src){
	assert(src!=NULL); // invalid argument
	if(src->game_status==CHECKMATE){ //there is a checkmate - game over
		if(CH_PLAYER_1 == getCurrentPlayer(src)) //the loser is the one who's king cannot be saved
			return CH_PLAYER_2;
		else
			return CH_PLAYER_1;
	}
	if (src->game_status==TIE)//there is a tie -game over
		return CH_TIE;
	return '\0'; //continue playing
}



void doMove(CH_Game* src, elem* element){
	assert(src!=NULL);
	element->prevFig = src->gameBoard[element->dstRow][element->dstCol];//saves in the history list the captured piece
	src->gameBoard[element->dstRow][element->dstCol] = element->fig; //puts the player's symbol in the desired place
	src->gameBoard[element->startRow][element->startCol]=CH_EMPTY_ENTRY; //empty entry at the source cell
	piecesUpdate(src,element->prevFig,false,getCurrentPlayer(src));//updates the pieces vic
	kingUpdate(src,element->fig,element->dstRow,element->dstCol); //updates the king's position
	if(spArrayListIsFull(src->undo_hist)) //updates the history array list which contains previous moves
		spArrayListRemoveFirst(src->undo_hist);
	spArrayListAddLast(src->undo_hist, element); //updates the history array list which contains previous moves
	switchPlayer(src); //switch player
}

void switchPlayer(CH_Game* src){
	assert(src!=NULL); //checks that the game src is a valid arg
	if((getCurrentPlayer(src)) == CH_PLAYER_1)
		src->currentPlayer = CH_PLAYER_2;
	else
		src->currentPlayer = CH_PLAYER_1;
}

void kingUpdate(CH_Game* src, char fig, int row, int col){
	assert(src != NULL);
	if(fig == KING_W){ //updates the white king's place
		src->white_king[0] = row;
		src->white_king[1] = col;
	}
	else if(fig == KING_B){//updates the black king's place
		src->black_king[0] = row;
		src->black_king[1] = col;
	}
}


CH_STATUS updateStatus(CH_Game* src){
	if (isCheck(src)) {// there is a check
		if (isCheckMate(src))
			return CHECKMATE;// there is a checkmate
		return CHECK;
	}
	else{
		if (isTie(src)) return TIE;// there is a tie
	}
	return REGULAR;
}

bool isTie(CH_Game* src){
	assert(src!=NULL);
	return noPossibleMoves(src); // true if no moves / o/w false
}

bool isCheck(CH_Game* src){
	bool check1, check2;
	int row=0,col=0;
	if(src->currentPlayer == CH_PLAYER_1){ //get white king place
		row = src->white_king[0];
		col = src->white_king[1];
	}
	else{
		row = src->black_king[0]; //get black king space
		col = src->black_king[1];
	}
	check1 = pieceUnderAttack(src,row,col); //check if the current player's king is under attack
	if(check1)
		return true;
	switchPlayer(src); //check if the other player's king under attack
	if(src->currentPlayer == CH_PLAYER_1){
		row = src->white_king[0];
		col = src->white_king[1];
	}
	else{
		row = src->black_king[0];
		col = src->black_king[1];
	}
	check2 = pieceUnderAttack(src,row,col);//check if the other player's king is under attack
	switchPlayer(src);
	return check2;
}


bool isCheckMate(CH_Game* src){
	return noPossibleMoves(src); //true - if no moves, false - o/w
}

void piecesUpdate(CH_Game* src,char figure,bool add,char player){
	assert(src!=NULL);
	int index=-1;
	switch(figure){ //the index in the pieces vector of the piece that was captures
	case PAWN_B:
	case PAWN_W:
		index = PAWN_INDEX;
		break;
	case KNIGHT_W:
	case KNIGHT_B:
		index = KNIGHT_INDEX;
		break;
	case BISHOP_B:
	case BISHOP_W:
		index = BISHOP_INDEX;
		break;
	case ROOK_B:
	case ROOK_W:
		index = ROOK_INDEX;
		break;
	case QUEEN_W:
	case QUEEN_B:
		index = QUEEN_INDEX;
		break;
	case KING_W:
	case KING_B:
		index = KING_INDEX;
		break;
		}
	if(index != -1){
		if(!add){
			if(player == CH_PLAYER_2) //updates the pieces vector according to the current player
					src->white[index]--;
				else
					src->black[index]--;
		}
		else{
			if(player == CH_PLAYER_2) //updates the pieces vector according to the current player
					src->white[index]++;
				else
					src->black[index]++;
			}
		}
	}


char playerPC(CH_Game* src){
	if (src->user_color==0) //if the player is black - the pc is white
		return CH_PLAYER_1;
	return CH_PLAYER_2; //o.w
}


CH_GAME_MESSAGE getMovesBonus(CH_Game* src, int row, int col,char** board,bool gui){
	assert(src!=NULL);
	if(gui)
		assert(board!=NULL);
	char fig;
	row--; col--; //makes indexes zero dependent
	int	**arr=NULL,num_of_moves=0;
	CH_Game* copy = NULL;
	if(row<0||row>GRID-1||col<0||col>GRID-1){ //valid cell on board
		printf("Invalid position on the board\n");
		return CH_SUCCESS;
	}
	if((isCellEmpty(src,row,col))||(isOppPiece(src,row,col))){ //checks whether there is a fig piece
		if(src->currentPlayer==CH_PLAYER_1)
			printf("The specified position does not contain white player piece\n");
		else
			printf("The specified position does not contain black player piece\n");
		return CH_SUCCESS;
	}
	arr = possibleMoves(src,row,col);
	if (arr == NULL)
		return CH_INVALID_ARGUMENT;
	if(arr[0][0]==-1){
		printf("\n"); // no possible moves
		return CH_SUCCESS;
	}
	num_of_moves = numOfMoves(src->gameBoard[row][col]); //get num of fig moves
	qsort(arr,num_of_moves,sizeof(int*),comp); //sort the fig movves
	copy = gameCopy(src);
	if(copy==NULL){
		freeArray(arr,num_of_moves);
		return CH_INVALID_ARGUMENT;
	}
	for(int i=0; i<num_of_moves; i++){
		if(arr[i][0] != -1){
			fig = copy->gameBoard[arr[i][0]][arr[i][1]];
			copy->gameBoard[arr[i][0]][arr[i][1]] = copy->gameBoard[row][col]; //make the move
			copy->gameBoard[row][col] = CH_EMPTY_ENTRY;
			printf("<%d,%c>",arr[i][0]+1,arr[i][1]+65);
			if(gui)
				board[arr[i][0]][arr[i][1]]='0'; //can move to this cell
			if(pieceUnderAttack(copy,arr[i][0],arr[i][1])){ //checks if the piece is threatened
				printf("*");
				if(gui)
					board[arr[i][0]][arr[i][1]] = '1'; // piece can be attacked
			}
			if(isOppPiece(src,arr[i][0],arr[i][1])){ //check if the piece eats other fig
				printf("^");
				if(gui){
					if(board[arr[i][0]][arr[i][1]] != '1') // can eat opp piece
						board[arr[i][0]][arr[i][1]] = '2';
				}
			}
			printf("\n");
			copy->gameBoard[row][col] = copy->gameBoard[arr[i][0]][arr[i][1]]; //undo the move
			copy->gameBoard[arr[i][0]][arr[i][1]]= fig;
		}
	}
	freeArray(arr,num_of_moves);
	gameDestroy(copy);
	return CH_SUCCESS;
}


int comp(const void* arg1, const void* arg2){
	//compare function for qsort
	const int *a= *(const int**)arg1, *b= *(const int**)arg2;
	if (a[0]<b[0])
		return -1;
	if (a[0]>b[0])
		return 1;
	if (a[1]<b[1])
		return -1;
	else
		return 1;
}

void failGame(CH_Game *game,bool destroy){
	printf("ERROR: malloc has failed"); //allocation ERROR
	if(destroy)// need to destroy
		gameDestroy(game); //destroys the game
	exit(0);
}

 bool pawnMove(CH_Game* src, elem* element){
	 assert(element!=NULL);
	 assert(src!=NULL);
	 bool check = false;
	 if(element->player == CH_PLAYER_1){ // white player move
		 if(element->startRow == 1){ // can move two cells if it is in initial cell
			 if(element->dstRow <= 1 || element->dstRow > 3)
				 return false;
			 check = (element->dstRow-element->startRow)==2;
		 }
		 else{ // in other cases move must be by one cell
			 if(element->dstRow != element->startRow+1)
				 return false;
		 }
	 }
	 else{ // black player move
		 if(element->startRow == 6){ // can move two cells if it is in initial cell
			 if(element->dstRow >= 6 || element->dstRow < 4)
				 return false;
			 check = (element->startRow-element->dstRow)==2;
		 }
		 else{ // in other cases move must be by one cell
			 if(element->dstRow != element->startRow-1)
				 return false;
		 }
	 }
	 if(abs(element->dstCol - element->startCol) == 1){ // // diagonal move is ok if capture
		 if(!isOppPiece(src,element->dstRow,element->dstCol) || check)
			 return false;
	 }
	 else if(element->dstCol != element->startCol)
		 return false;
	 return true;
 }


bool bishopMove(elem* element){
	assert(element!=NULL);
	return isDiagonal(element); // checks for diagonal moves
}


bool rookMove(elem* element){
	assert(element!=NULL);
	if((element->dstCol-element->startCol)==0 && (element->dstRow-element->startRow)==0) // stay is not valid
		return false;
	return (!((element->dstCol != element->startCol) && (element->dstRow != element->startRow))); // move can not be in both row and col
}


bool knightMove(elem* element){
	assert(element!=NULL);
	if((abs(element->dstCol-element->startCol) == 2) && (abs(element->dstRow-element->startRow) == 1)) // two horizontal + one vertical
		return true;
	if((abs(element->dstCol-element->startCol) == 1) && (abs(element->dstRow-element->startRow) == 2)) // one horizontal + two vertical
		return true;
	return false;
}


bool queenMove(elem* element){
	assert(element!=NULL);
	return (rookMove(element) || bishopMove(element)); // diagonal move or horizontal/vertical move
}

bool kingMove(elem* element){
	assert(element!=NULL);
	if(abs(element->dstCol-element->startCol)>1 || abs(element->dstRow-element->startRow)>1)
		return false;
	if((element->dstCol-element->startCol)==0 && (element->dstRow-element->startRow)==0) // stay is not valid
		return false;
	return true; // one cell move in any direction
}


bool noOverlap(CH_Game* src, elem* element){
	assert(src != NULL);
	assert(element != NULL);
	switch(element->fig){ //Checks if the move given doesn't overlap other pieces
	case PAWN_W:
	case PAWN_B: //checks pawn overlap
		return pawnOverlap(src,element);
	case BISHOP_W:
	case BISHOP_B://checks bishop overlap
		return bishopOverlap(src,element);
	case ROOK_W:
	case ROOK_B://checks rook overlap
		return rookOverlap(src,element);
	case KNIGHT_W:
	case KNIGHT_B:
	case KING_B:
	case KING_W://checks knight/king overlap
		return kingOrKnightOverlap(src,element);
	case QUEEN_W:
	case QUEEN_B://checks queen overlap
		return queenOverlap(src,element);
	}
	return false;
}

bool isDiagonal(elem* element){
	assert(element!=NULL);
	int col,row;
	if((element->dstCol-element->startCol)==0 && (element->dstRow-element->startRow)==0) // stay is not valid
		return false;
	col=element->dstCol-element->startCol;
	row=element->dstRow-element->startRow;
	col=abs(col); row=abs(row);
	return (col==row); // checks for diagonal moves
}

bool pawnOverlap(CH_Game* src,elem* element){ //returns true if the move is fine- the overlap is legal or no overlap
	assert(element!=NULL);
	assert(src!=NULL);
	int curr_row_checked=0;
	if(abs(element->dstRow - element->startRow)==1) // moves one cell forward
		 if (element->startCol ==element->dstCol) // column not change
			 return isCellEmpty(src,element->dstRow,element->startCol); //if the cell is empty the move can be done
		 else return isOppPiece(src,element->dstRow,element->dstCol); //eating in diagonal
	 else{ //two moves forward
		 if(element->player == CH_PLAYER_1) // white player moves up on board
			 curr_row_checked=element->startRow+1;
		 else // black player moves down on board
			 curr_row_checked=element->startRow-1;
		 if (isCellEmpty(src,curr_row_checked,element->startCol)==false) //checks if the first step doesnt overlap
			 return false;
		 curr_row_checked=element->dstRow;
		 if (isCellEmpty(src,curr_row_checked,element->startCol)==false)//checks if the second step doesnt overlap
			 return false;
	 }
	 return true;
}

bool bishopOverlap(CH_Game* src,elem* element){ //returns true if the move is fine- the overlap is legal or no overlap
	assert(element!=NULL);
	assert(src!=NULL);
	int curr_row_checked=0, curr_col_checked=0;
	if (element->dstRow>element->startRow){ //moving forward
		if (element->startCol<element->dstCol){ //moving on the right diagonal
			curr_row_checked=element->startRow+1;
			curr_col_checked=element->startCol+1;
			while (curr_row_checked!=element->dstRow){ //while that's not the dst cell
				if (isCellEmpty(src,curr_row_checked,curr_col_checked)==false) //checks if the step is not an overlap
					return false;
				curr_row_checked=curr_row_checked+1;
				curr_col_checked=curr_col_checked+1;
			}
		}
		else //moving on the left diagonal forward
		{
			curr_row_checked=element->startRow+1;
			curr_col_checked=element->startCol-1;
			while (curr_row_checked!=element->dstRow){//while that's not the dst cell
				if (isCellEmpty(src,curr_row_checked,curr_col_checked)==false) //checks if the step is not an overlap
					return false;
				curr_row_checked=curr_row_checked+1;
				curr_col_checked=curr_col_checked-1;
			}
		}
	}
	else{ //moving backward
		if (element->startCol>element->dstCol){ //moving on the right diagonal
			curr_row_checked=element->startRow-1;
			curr_col_checked=element->startCol-1;
			while (curr_row_checked!=element->dstRow){ //while that's not the dst cell
				if (isCellEmpty(src,curr_row_checked,curr_col_checked)==false) //checks if the step is not an overlap
					return false;
				curr_row_checked=curr_row_checked-1;
				curr_col_checked=curr_col_checked-1;
			}
		}
		else //moving on the left diagonal forward
		{
			curr_row_checked=element->startRow-1;
			curr_col_checked=element->startCol+1;
			while (curr_row_checked!=element->dstRow){//while that's not the dst cell
				if (isCellEmpty(src,curr_row_checked,curr_col_checked)==false) //checks if the step is not an overlap
					return false;
				curr_row_checked=curr_row_checked-1;
				curr_col_checked=curr_col_checked+1;
			}
		}
	}
	return (isCellEmpty(src,element->dstRow,element->dstCol)||isOppPiece(src,element->dstRow,element->dstCol)); //we got to the dst cell - check if empty or opp
}


bool rookOverlap(CH_Game* src,elem* element){ //returns true if the move is fine- the overlap is legal or no overlap
	assert(element!=NULL);
	assert(src!=NULL);
	int curr_col_checked=0, curr_row_checked=0;
	if (element->startRow==element->dstRow){ //moving along it's line
		if (element->dstCol>element->startCol){//moving right
			curr_col_checked=element->startCol+1;
			while (curr_col_checked!=element->dstCol){
				if (isCellEmpty(src,element->startRow,curr_col_checked)==false) //checks if the step is not an overlap
					return false;
				curr_col_checked=curr_col_checked+1;
			}
		}
		else{ //moving left
			curr_col_checked=element->startCol-1;
			while (curr_col_checked!=element->dstCol){
				if (isCellEmpty(src,element->startRow,curr_col_checked)==false) //checks if the step is not an overlap
					return false;
				curr_col_checked=curr_col_checked-1;
			}
		}
	}
	else{ //moving forward/backwards
		if (element->dstRow>element->startRow){ //moving forward
			curr_row_checked=element->startRow+1;
			while (curr_row_checked!=element->dstRow){
				if (isCellEmpty(src,curr_row_checked,element->startCol)==false) //checks if the step is not an overlap
					return false;
				curr_row_checked=curr_row_checked+1;
			}
		}
		else{ //moving backwards
			curr_row_checked=element->startRow-1;
			while (curr_row_checked!=element->dstRow){
				if (isCellEmpty(src,curr_row_checked,element->startCol)==false) //checks if the step is not an overlap
					return false;
				curr_row_checked=curr_row_checked-1;
			}
		}
	}
	return (isCellEmpty(src,element->dstRow,element->dstCol)||isOppPiece(src,element->dstRow, element->dstCol)); //we got to the dst cell - check if empty or opp

}

bool queenOverlap(CH_Game* src,elem* element){ //returns true if the move is fine- the overlap is legal or no overlap
	assert(element!=NULL);
	assert(src!=NULL);
	if (isDiagonal(element)) //if the queen wants to move diagonal
		return bishopOverlap(src,element); //checks if "bishop" doesn't overlap
	else return rookOverlap(src,element); //the queen moves left or right - checks if "rook" doesn't overlap

}

bool kingOrKnightOverlap(CH_Game* src,elem* element){//returns true if the move is fine- the overlap is legal or no overlap
	assert(element!=NULL);
	assert(src!=NULL);
	return (isCellEmpty(src,element->dstRow,element->dstCol)||isOppPiece(src,element->dstRow,element->dstCol)); //we got to the dst cell - check if empty or opp

}

bool pieceUnderAttack(CH_Game* src,int row,int col){
	assert(src != NULL);
	elem* element=NULL;
	int move[4]={0};
	int pos[2]={0}; //represents the king's place according to the current player
	pos[0]=row; pos[1]=col;
	for (int i=0;i<GRID;i++){ //scan all board
		for (int j=0;j<GRID;j++){
			if (isOppPiece(src,i,j)){ //check if the figure is the opponnent's figure
				move[START_ROW_PLACE]=i+1; move[START_COL_PLACE]=j+1; move[DST_ROW_PLACE]=pos[0]+1;move[DST_COL_PLACE]=pos[1]+1;
				switchPlayer(src);
				element=transfer(src,move); //try to capture the king
				if(element==NULL) // allocation error
					failGame(src,true);
				if (figureMove(src,element)&&(noOverlap(src,element))){ //if the move is valid
					free(element);
					switchPlayer(src);
					return true;
				}
				switchPlayer(src);
				free(element);
			}
		}
	}
	return false;
}

int** possibleMoves(CH_Game* src, int row, int col){
	assert(src != NULL);
	switch(src->gameBoard[row][col]){ //Checks if the move given doesn't overlap other pieces
	case PAWN_W:
	case PAWN_B: // pawn moves
		return possiblePawnMoves(src,row,col);
	case BISHOP_W:
	case BISHOP_B: // bishop moves
		return possibleBishopMoves(src,row,col);
	case ROOK_W:
	case ROOK_B: // rook moves
		return possibleRookMoves(src,row,col);
	case KNIGHT_W:
	case KNIGHT_B: // knight moves
		return possibleKnightMoves(src,row,col);
	case QUEEN_W:
	case QUEEN_B: // queen moves
		return possibleQueenMoves(src,row,col);
	case KING_W:
	case KING_B: // king moves
		return possibleKingMoves(src,row,col);
	}
	return NULL;
}


int** possibleKingMoves(CH_Game* src,int row, int col){
	int index=0;
	bool add_move=false;
	int** arr= arrayInit(KING_MOVES); //creates an array of all possible moves
	if(!arr)
		return NULL;
	for (int i=row-1;i<=row+1;i++){ //the king has 8 moves options
		for (int j=col-1;j<=col+1;j++){
			if ((i>=0 && i<=(GRID-1)) && (j>=0 && j<=(GRID-1))){
				add_move=addToMoveArray(src,row,col,i,j,arr,index);//checks if the move valid
				if (add_move) index++; //returns true if the move valid
			}
		}
	}
	return arr;
}

int** possibleQueenMoves(CH_Game* src, int row, int col){
	int index=0;
	int** rookarr=NULL;
	int** bishoparr=NULL;
	int** arr= arrayInit(QUEEN_MOVES);//creates an array of all possible moves
	if(!arr)
		return NULL;
	//queen moves:bishop moves+rook moves
	rookarr = possibleRookMoves(src,row,col);
	bishoparr = possibleBishopMoves(src,row,col);
	if((!rookarr) || (!bishoparr)){
		freeArray(arr,QUEEN_MOVES);
		if((!rookarr)&&(!bishoparr)){
			freeArray(rookarr,ROOK_MOVES);
			freeArray(bishoparr,BISHOP_MOVES);
		}
		else if((!bishoparr))
			freeArray(bishoparr,BISHOP_MOVES);
		else if((!rookarr))
			freeArray(rookarr,ROOK_MOVES);
		return NULL;
	}
	for(int i=0;i<ROOK_MOVES;i++){//add possible rook moves to array
		if(rookarr[i][0] != -1){
			arr[index][0] = rookarr[i][0];
			arr[index][1] = rookarr[i][1];
			index ++;
		}
		free(rookarr[i]);
	}
	for(int i=0;i<BISHOP_MOVES;i++){//add possible bishop moves to array
		if(bishoparr[i][0] != -1){
			arr[index][0] = bishoparr[i][0];
			arr[index][1] = bishoparr[i][1];
			index ++;
		}
		free(bishoparr[i]);
	}
	free(rookarr);
	free(bishoparr);
	return arr;
}

int** possibleKnightMoves(CH_Game* src, int row, int col){
	int index=0;
	int** arr= arrayInit(KNIGHT_MOVES);//creates an array of all possible moves
	if(!arr)
		return NULL;
	index=0;
	bool add_move=false;
	//checking all 8 possible moves of the knight if they are valid
	add_move=addToMoveArray(src,row,col,row+2,col+1,arr,index);
	if (add_move) index++;
	add_move=addToMoveArray(src,row,col,row+1,col+2,arr,index);
	if (add_move) index++;
	add_move=addToMoveArray(src,row,col,row+2,col-1,arr,index);
	if (add_move) index++;
	add_move=addToMoveArray(src,row,col,row+1,col-2,arr,index);
	if (add_move) index++;
	add_move=addToMoveArray(src,row,col,row-2,col-1,arr,index);
	if (add_move) index++;
	add_move=addToMoveArray(src,row,col,row-1,col-2,arr,index);
	if (add_move) index++;
	add_move=addToMoveArray(src,row,col,row-2,col+1,arr,index);
	if (add_move) index++;
	add_move=addToMoveArray(src,row,col,row-1,col+2,arr,index);
	if (add_move) index++;
	return arr;
}

int** possibleRookMoves(CH_Game* src, int row, int col){
	int index=0;
	bool add_move=false;
	int** arr= arrayInit(ROOK_MOVES);//creates an array of all possible moves
	if(!arr)
		return NULL;
	//checking all 14 possible moves of the knight if they are valid
	for (int i=row+1;i<GRID;i++){//moving forward
		add_move=addToMoveArray(src,row,col,i,col,arr,index);
		if (add_move) index++;
	}
	for (int i=row-1;i>=0;i--){//moving backwards
		add_move=addToMoveArray(src,row,col,i,col,arr,index);
		if (add_move) index++;
	}
	for (int j=col+1;j<GRID;j++){//moving right
		add_move=addToMoveArray(src,row,col,row,j,arr,index);
		if (add_move) index++;
	}
	for (int j=col-1;j>=0;j--){//moving left
		add_move=addToMoveArray(src,row,col,row,j,arr,index);
		if (add_move) index++;
	}
	return arr;
}

int** possibleBishopMoves(CH_Game* src,int row, int col){
	int i=0,j=0,index=0;
	bool add_move=false;
	int** arr= arrayInit(BISHOP_MOVES);//creates an array of all possible moves
	if(!arr)
		return NULL;
	i=row-1;j=col-1;
	//checking all 13 possible moves of the knight if they are valid
	while((i>=0 && i<=(GRID-1)) && (j>=0 && j<=(GRID-1))) //right backwards diagonal
	{
		add_move=addToMoveArray(src,row,col,i,j,arr,index);
		if (add_move) index++;
		i--; j--;
	}
	i=row-1;j=col+1;
	while((i>=0 && i<=(GRID-1)) && (j>=0 && j<=(GRID-1))) //left backwards diagonal
	{
		add_move=addToMoveArray(src,row,col,i,j,arr,index);
		if (add_move) index++;
		i--; j++;
	}
	i=row+1;j=col-1;
	while((i>=0 && i<=(GRID-1)) && (j>=0 && j<=(GRID-1))) //left forward diagonal
	{
		add_move=addToMoveArray(src,row,col,i,j,arr,index);
		if (add_move) index++;
		i++; j--;
	}
	i=row+1;j=col+1;
	while((i>=0 && i<=(GRID-1)) && (j>=0 && j<=(GRID-1))) //left forward diagonal
	{
		add_move=addToMoveArray(src,row,col,i,j,arr,index);
		if (add_move) index++;
		i++; j++;
	}
	return arr;
}

int** possiblePawnMoves(CH_Game* src,int row, int col){
	int index=0;
	bool add_move=false;
	int** arr= arrayInit(PAWN_MOVES);//creates an array of all possible moves
	if(!arr)
		return NULL;
	if(src->currentPlayer == CH_PLAYER_1){
		add_move=addToMoveArray(src,row,col,row+2,col,arr,index); //move two steps forwards
		if (add_move) index++;
		add_move=addToMoveArray(src,row,col,row+1,col,arr,index);//move one step forward
		if (add_move) index++;
		add_move=addToMoveArray(src,row,col,row+1,col+1,arr,index);//diagonal right
		if (add_move) index++;
		add_move=addToMoveArray(src,row,col,row+1,col-1,arr,index);//diagonal left
		if (add_move) index++;
	}
	else{
		add_move=addToMoveArray(src,row,col,row-2,col,arr,index); //move two steps forwards
		if (add_move) index++;
		add_move=addToMoveArray(src,row,col,row-1,col,arr,index);//move one step forward
		if (add_move) index++;
		add_move=addToMoveArray(src,row,col,row-1,col+1,arr,index);//diagonal right
		if (add_move) index++;
		add_move=addToMoveArray(src,row,col,row-1,col-1,arr,index);//diagonal left
		if (add_move) index++;
	}
	return arr;
}


int** arrayInit(int size){
	int** arr= (int**)malloc(size*sizeof(int*));//creates an array of all possible moves
	if(!arr)
		return NULL;
	for(int i=0;i<size;i++){
		arr[i] = (int*)malloc(CELL_SIZE*sizeof(int));//initializes array
		if(!arr[i]){
			freeArray(arr,i);
			return NULL;
		}
		arr[i][0] = -1;//initializes to -1 the array
		arr[i][1] = -1;
	}
	return arr;
}

bool addToMoveArray(CH_Game* src,int row, int col,int i, int j,int** arr, int index){
	assert(src!=NULL);
	assert(arr!=NULL);
	elem* element=NULL;
	int res, move[CELL_SIZE*CELL_SIZE]={0};
	if (row<0||row>(GRID-1)||col<0||col>(GRID-1)||i<0||i>(GRID-1)||j<0||j>(GRID-1)||(row==i && col==j))
		return false;
	move[START_ROW_PLACE]=row+1; move[START_COL_PLACE]=col+1; move[DST_ROW_PLACE]=i+1; move[DST_COL_PLACE]=j+1;
	element = transfer(src,move);//creates an element from the move
	if(element==NULL)
		failGame(src,true);
	res = isValidMove(src,element);
	if(res == ALLOCATION_ERROR){
		free(element);
		failGame(src,true);
	}
	if(res==1){//adds to move array if the move is valid
		arr[index][0] =i;
		arr[index][1]=j;
		free(element);
		return true;
	}
	free(element);
	return false;
}

void freeArray(int** arr,int size){
	for(int k=0;k<size;k++)
		free(arr[k]);
	free(arr);
}

bool noPossibleMoves(CH_Game* src){
	int** arr=NULL;
	int num_of_moves=0;
	assert(src!=NULL);
	for(int i=0;i<GRID;i++){ // scan board
		for(int j=0;j<GRID;j++){
			if(playerFig(src,i,j)){ // there is player's figure in this cell
				arr = possibleMoves(src,i,j); // gets this figure possible moves
				if(arr==NULL)
					failGame(src,true);
				num_of_moves = numOfMoves(src->gameBoard[i][j]);
				if(arr[0][0] != -1){ // there is a possible move
					freeArray(arr,num_of_moves);
					return false; // not a tie
				}
				freeArray(arr,num_of_moves);
			}
		}
	}
	return true;
}

int numOfMoves(char ch){
	switch(ch){ //the number of piece moves
	case PAWN_B:
	case PAWN_W:
		return PAWN_MOVES;
	case BISHOP_B:
	case BISHOP_W:
		return BISHOP_MOVES;
	case ROOK_B:
	case ROOK_W:
		return ROOK_MOVES;
	case KNIGHT_W:
	case KNIGHT_B:
		return KNIGHT_MOVES;
	case QUEEN_W:
	case QUEEN_B:
		return  QUEEN_MOVES;
	case KING_W:
	case KING_B:
		return KING_MOVES;
	}
	return 0;
}
