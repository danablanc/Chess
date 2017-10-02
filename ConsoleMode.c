
#include "ConsoleMode.h"

void startGame(){
	CH_Game* game = gameCreate(HISTORY_SIZE,DEFAULT_DIFFICULTY,DEFAULT_COLOR,DEFAULT_MODE);//creates a new game
	if(game==NULL)
		fail(game,false); //allocation ERROR
	settingState(game); //settings state
	if(game->mode == TWO_PLAYER_MODE)
		twoPlayerMode(game); //invokes a game for two players
	else
		onePlayerMode(game); //invokes a game player vs pc
}

void settingState(CH_Game* game){
	SetCommand command;
	assert(game != NULL);
	printf("Specify game setting or type 'start' to begin a game with the current setting:\n");
	while(true){ //get the setting command from user
		command = getNewSetCommand();
		if(command.cmd == CH_GAME_MODE) //act according to the specifed game command
			gameMode(game,command.arg);
		else if(command.cmd == CH_DIFFICULTY)
			gameDifficulty(game,command.arg);
		else if(command.cmd == CH_USER_COLOR)
			gameColor(game,command.arg);
		else if(command.cmd == CH_LOAD)
			gameLoad(game,command.path);
		else if(command.cmd == CH_DEFAULT)
			gameDefault(game);
		else if(command.cmd == CH_PRINT_SETTING)
			gamePrintSetting(game);
		else if(command.cmd == CH_QUIT)
			gameQuit(game,true);
		else if(command.cmd == CH_START)
			break;
	}
}

bool gameState(CH_Game* game, GameCommand command){
	assert(game != NULL);
	int move[CELL_SIZE*CELL_SIZE];
	for(int i=0;i<CELL_SIZE*CELL_SIZE;i++) //initialize a move
		move[i]=command.move[i];
	if(command.cmd == CH_MOVE) //act according to the specifed game command
		return gameMove(game,move);
	else if(command.cmd == CH_GET_MOVES)
		return gameGetMoves(game,move);
	else if(command.cmd == CH_SAVE)
		return gameSave(game,command.path);
	else if(command.cmd == CH_UNDO)
		return gameUndoMove(game);
	else if(command.cmd == CH_RESET)
		gameReset(game);
	else if(command.cmd == CH_GQUIT)
		gameQuit(game,true);
	return true;
}

SetCommand getNewSetCommand(){
	SetCommand command;
	char input[SP_MAX_LINE_LENGTH];
	while(true){ //loop and reading from stdin until gets a valid command
		fgets(input,SP_MAX_LINE_LENGTH,stdin); //gets input from user
		int i = strlen(input)-1;
		if(input[i] == '\n') // puts null character
			input[i] = '\0';
		command = parserSetPraseLine(input);  //parsing the read string into command
		if(command.cmd != CH_INVALID_LINE) // command is valid
			return command;
		printf("ERROR: invalid command\n");
	}
}


GameCommand getNewGameCommand(CH_Game* game,bool to_print){
	GameCommand command;
	char input[SP_MAX_LINE_LENGTH];
	if(to_print) // checks if needs to print move message and board
		gamePrintBoard(game);//prints game board
	while(true){ //loop and reading from stdin until gets a valid command
		printf("%s player - enter your move:\n",color(game->currentPlayer));
		fgets(input,SP_MAX_LINE_LENGTH,stdin); //gets input from user
		int i = strlen(input)-1;
		if(input[i] == '\n') // puts null character
			input[i] = '\0';
		command = parserGamePraseLine(input);  //parsing the read string into command
		if(command.cmd != CH_GINVALID_LINE) // command is valid
			return command;
		printf("ERROR: invalid command\n");
	}
}


void gameMode(CH_Game* game, int arg){
	assert(game!=NULL);
	if(arg == DEFAULT_MODE){ //player vs pc
		game->mode = arg;
		printf("Game mode is set to 1 player\n");
	}
	else if(arg == TWO_PLAYER_MODE){ // user vs other user
		game->mode = arg;
		printf("Game mode is set to 2 players\n");
	}
	else //error
		printf("Wrong game mode\n");
}

void gameDifficulty(CH_Game* game, int arg){
	assert(game!=NULL);
	if(game->mode == TWO_PLAYER_MODE) //if mode is 2 players - the difficulty cannot be set
		printf("ERROR: invalid command\n");
	else{
		if(arg == EXPERT_LEVEL) //expert level
			printf("Expert level not supported, please choose a value between 1 to 4:\n");
		else if(arg<1 || arg>EXPERT_LEVEL) //error
			printf("Wrong difficulty level. The value should be between 1 to 5\n");
		else
			game->difficulty=arg; //sets difficulty
	}

}

void gameColor(CH_Game* game, int arg){
	assert(game!=NULL);
	if(game->mode == TWO_PLAYER_MODE) //if mode is 2 players - the user color cannot be set
		printf("ERROR: invalid command\n");
	else
		game->user_color = arg; //sets user color
}

void gameLoad(CH_Game* game, char* path){
	assert(game!=NULL);
	assert(path!=NULL);
	CH_Game* loadgame;
	FILE* xml = fopen(path, "r"); //open the file
	if (!xml){ //no file
		printf("Error: File doesn't exist or cannot be opened\n");
	}
	else{
		loadgame=fileToGame(xml); //loads a game from file
		if(loadgame == NULL)
			fail(game,true);
		gameAssign(loadgame,game); //copies the loaded game to the game that the console has
		gameDestroy(loadgame); //destroys the load game
		if(ferror(xml)) //if there was an error
			printf("Error: File doesn't exist or cannot be opened\n");
		else if (fclose(xml)!=0) //close the file - if the close of the file failed
			printf("Error: File doesn't exist or cannot be opened\n");
	}
}

void gameDefault(CH_Game* game){
	assert(game!=NULL);
	game->mode = DEFAULT_MODE;
	game->difficulty = DEFAULT_DIFFICULTY;
	game->user_color = DEFAULT_COLOR;
}

void gamePrintSetting(CH_Game* game){
	assert(game!=NULL);
	if(game->mode != DEFAULT_MODE){ //2 players mode print
		printf("SETTINGS:\n");
		printf("GAME_MODE: 2\n");
	}
	else{//1 player mode print
		printf("SETTINGS:\n");
		printf("GAME_MODE: 1\n");
		printf("DIFFICULTY_LVL: %d\n",game->difficulty);
		if(game->user_color == 0)
			printf("USER_CLR: BLACK\n");
		else
			printf("USER_CLR: WHITE\n");
	}
}

void gameQuit(CH_Game *game, bool check){
	assert(game!=NULL);
	gameDestroy(game); //free game resources
	if(check)
		printf("Exiting...\n");
	exit(0);
}


bool gameMove(CH_Game* game, int* move){
	assert(game!=NULL);
	assert(move!=NULL);
	int result = setMove(game,move); //make a move
	if(result == ALLOCATION_ERROR)
		fail(game,true);
	if(result == POSITION_INVALID){ //if the position given invalid
		printf("Invalid position on the board\n");
		return false;
	}
	else if(result == ILLEGAL_FIGURE){//if the src cell doesn't contains players piece
		printf("The specified position does not contain your piece\n");
		return false;
	}
	else if(result == ILLEGAL_MOVE){ //illegal move
		printf("Illegal move\n");
		return false;
	}
	return true;
}

bool gameGetMoves(CH_Game* game, int* move){
	assert(game!=NULL);
	assert(move!=NULL);
	if(game->mode == TWO_PLAYER_MODE || game->difficulty>DEFAULT_DIFFICULTY){ //if get moves cannot be invoked
		printf("ERROR: invalid command\n");
		return false;
	}
	if(getMovesBonus(game,move[START_ROW_PLACE],move[START_COL_PLACE],NULL,false)==CH_INVALID_ARGUMENT) //gets the move according to the move given
		fail(game,true);
	return true;
}

bool gameSave(CH_Game* game, char* path){
	assert(game!=NULL);
	assert(path!=NULL);
	XML_MESSAGE msg;
	FILE* xml=fopen(path,"w"); //opens the game
	if (xml==NULL){ //open failed
		printf("File cannot be created or modified\n");
		 return false;
	}
	else
		msg=gameToFile(game,xml);
	if(msg == XML_ALLOC){
		fclose(xml);
		fail(game,true);
	}
	if ((msg==XML_FAILED)||(ferror(xml))){ //game is null or there was an error in the file
		printf("File cannot be created or modified\n");
		return false;
	}
	if (fclose(xml)!=0){//closing the file failed
		printf("Error: File doesn't exist or cannot be opened\n");
		return false;
	}

	return true;
}

bool gameUndoMove(CH_Game* game){
	assert(game!=NULL);
	CH_GAME_MESSAGE status;
	elem* element;
	if(game->mode==TWO_PLAYER_MODE){ //if thats 2 players mode the command is illegal
		printf("Undo command not available in 2 players mode\n");
		return false;
	}
	if(game->undo_hist->actualSize<2){//if there is no history == the command is illegal
		printf("Empty history, move cannot be undone\n");
		return false;
	}
	element = spArrayListGetLast(game->undo_hist); //gets the move (1)
	status = undoPrevMove(game); // undo the move
	if(status != CH_SUCCESS)
		return false;
	printf("Undo move for player %s : <%d,%c> -> <%d,%c>\n",color(game->currentPlayer),element->dstRow+1,
			element->dstCol+ASCII_OFFSET+1,element->startRow+1,element->startCol+ASCII_OFFSET+1);
	element = spArrayListGetLast(game->undo_hist);//gets the move (2)
	status = undoPrevMove(game);// undo the move
	printf("Undo move for player %s : <%d,%c> -> <%d,%c>\n",color(game->currentPlayer),element->dstRow+1,
			element->dstCol+ASCII_OFFSET+1,element->startRow+1,element->startCol+ASCII_OFFSET+1);
	return true;
}

void gameReset(CH_Game* game){
	assert(game!=NULL);
	gameDestroy(game);
	printf("Restarting...\n");
	startGame(); //start new game
}


void fail(CH_Game *game,bool destroy){
	printf("ERROR: malloc has failed"); //allocation ERROR
	if(destroy)// need to destroy
		gameDestroy(game); //destroys the game
	exit(0);
}


void computerMove(CH_Game* game){
	assert(game!=NULL);
	int res;
	int* move = miniMaxPcMove(game,game->difficulty); //get the best move from minimax
	elem* element;
	if(!move)
		fail(game,true);
	for(int i=0;i<CELL_SIZE*CELL_SIZE;i++)
		move[i]++;
	res = setMove(game,move); //set the move
	free(move);
	if(res == ALLOCATION_ERROR)
		fail(game,true);
	element = spArrayListGetLast(game->undo_hist); //update history
	printf("Computer: move %s at <%d,%c> to <%d,%c>\n",figure(element->fig),element->startRow+1,
			toChar(element->startCol),element->dstRow+1,toChar(element->dstCol)); //print the move
}

void onePlayerMode(CH_Game* game){
	assert(game!=NULL);
	bool check =true, to_print = (game->user_color==1);
	GameCommand command;
	while(true){
		if(game->currentPlayer == playerPC(game)){ //the computer plays
			computerMove(game); //make a computer move
			to_print = true;
			if(checkWinner(game) != '\0'){
				if(game->game_status == CHECKMATE){  //ggame checkmate
					switchPlayer(game);
					printf("Checkmate! %s player wins the game\n",color(game->currentPlayer));
					gameQuit(game,false);
				}
				else{ //game tie
					printf("The game ends in a tie\n");
					gameQuit(game,false);
				}
			}
			if(game->game_status==CHECK) //check
				printf("Check!\n");
		}
		else{ //get command from user
		command = getNewGameCommand(game,to_print&&check);
		check = gameState(game,command);
		if(command.cmd == CH_GET_MOVES)
			check = false;
		if (command.cmd == CH_SAVE){
			to_print=false;
			check=false;
		}
		if (check!=false)
			statusCheck(game); //check for a winner or change in game status
		}
	}
}


void twoPlayerMode(CH_Game* game){
	assert(game!=NULL); //checks alloc.
	bool check =true;  // flag for command succession evaluation
	GameCommand command;
	while(true){
		command = getNewGameCommand(game,check); //gets new game command from user
		check = gameState(game,command);
		if (command.cmd ==CH_SAVE)
			check=false;
		if (check!=false)
			statusCheck(game);//check for a winner or change in game status
	}
}


char* color(char ch){
	if(ch=='F')
		return "white";
	else
		return "black";
}


char* figure(char ch){
	switch(ch){ //Checks which piece it is
	case PAWN_W:
	case PAWN_B: // pawn
		return "pawn";
	case BISHOP_W:
	case BISHOP_B: // bishop
		return "bishop";
	case ROOK_W:
	case ROOK_B: // rook
		return "rook";
	case KNIGHT_W:
	case KNIGHT_B: // knight
		return "knight";
	case QUEEN_W:
	case QUEEN_B: // queen
		return "queen";
	case KING_W:
	case KING_B: // king
		return "king";
	}
	return NULL;
}


void statusCheck(CH_Game* game){
	assert(game!=NULL);
	if(checkWinner(game) != '\0'){
		if(game->game_status == CHECKMATE){
			switchPlayer(game);
			printf("Checkmate! %s player wins the game\n",color(game->currentPlayer));
			gameQuit(game,false);
		}
		else{
			printf("The game is tied\n");
			gameQuit(game,false);
		}
	}
	if(game->game_status==CHECK)
		printf("Check: %s King is threatened!\n",color(game->currentPlayer));
}
