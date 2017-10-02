#include "GUIManager.h"


SPGuiManager* spManagerCreate() {
	SPGuiManager* res = (SPGuiManager*) malloc(sizeof(SPGuiManager));
	if (res == NULL){
		failMessage("Couldn't create gui manager window!");
		return NULL ;
	}
	res->mainWin = spMainWindowCreate(); //creates main window
	if (res->mainWin == NULL ) {
		failMessage("Couldn't create gui manager window!");
		free(res);
		return NULL ;
	}
	//initializes gui manager's parameters
	res->game = NULL;
	res->settingsWin = NULL;
	res->loadWin = NULL;
	res->gameWin = NULL;
	res->activeWin = SP_MAIN_WINDOW;
	res->prevWin=SP_MAIN_WINDOW;
	res->game=gameCreate(HISTORY_SIZE,DEFAULT_DIFFICULTY,DEFAULT_COLOR,DEFAULT_MODE); //create game
	if(res->game == NULL){
		failMessage("Couldn't create gui manager window!");
		spManagerDestroy(res);
		return NULL;
	}
	for(int i=0;i<MAX_OF_SAVED_FILES;i++) //initializes the arrays
		res->saved_games[i] = "";
	for(int i=0;i<NUM_OF_IMAGES;i++)
		res->board_images[i] = "";
	updateImages(res);
	initSaves(res);
	return res;
}

void updateImages(SPGuiManager* src){
	//init all board images
	src->board_images[WHITE_PAWN_IMG]="./images/new/WhiteP.bmp";
	src->board_images[WHITE_BISHOP_IMG]="./images/new/WhiteB.bmp";
	src->board_images[WHITE_ROOK_IMG]="./images/new/WhiteR.bmp";
	src->board_images[WHITE_KNIGHT_IMG]="./images/new/WhiteN.bmp";
	src->board_images[WHITE_QUEEN_IMG]="./images/new/WhiteQ.bmp";
	src->board_images[WHITE_KING_IMG]="./images/new/WhiteK.bmp";
	src->board_images[BLACK_PAWN_IMG]="./images/new/BlackP.bmp";
	src->board_images[BLACK_BISHOP_IMG]="./images/new/BlackB.bmp";
	src->board_images[BLACK_ROOK_IMG]="./images/new/BlackR.bmp";
	src->board_images[BLACK_KNIGHT_IMG]="./images/new/BlackN.bmp";
	src->board_images[BLACK_QUEEN_IMG]="./images/new/BlackQ.bmp";
	src->board_images[BLACK_KING_IMG]="./images/new/BlackK.bmp";
	src->board_images[RED_CELL_IMG]="./images/new/redCell.bmp";
	src->board_images[YELLOW_CELL_IMG]="./images/new/yellowCell.bmp";
	src->board_images[GREEN_CELL_IMG]="./images/new/greenCell.bmp";
}

void initSaves(SPGuiManager* src){
	//init all save paths
	src->saved_games[FILE_SLOT_1]="./graphics/saved_games/file_0.xml";
	src->saved_games[FILE_SLOT_2]="./graphics/saved_games/file_1.xml";
	src->saved_games[FILE_SLOT_3]="./graphics/saved_games/file_2.xml";
	src->saved_games[FILE_SLOT_4]="./graphics/saved_games/file_3.xml";
	src->saved_games[FILE_SLOT_5]="./graphics/saved_games/file_4.xml";
}

void spManagerDestroy(SPGuiManager* src) {
	if (!src){ // no need to destroy
		return;
	} // destroy all struct fields
	if (src->gameWin != NULL)
		spGameWindowDestroy(src->gameWin);
	src->gameWin = NULL;
	if (src->loadWin != NULL)
		spLoadWindowDestroy(src->loadWin);
	src->loadWin = NULL;
	if (src->settingsWin != NULL)
		spSettingsWindowDestroy(src->settingsWin);
	src->settingsWin = NULL;
	if (src->mainWin != NULL)
		spMainWindowDestroy(src->mainWin);
	src->mainWin = NULL;
	if (src->game != NULL)
		gameDestroy(src->game);
	free(src);
}

void spManagerDraw(SPGuiManager* src) {
	assert(src!=NULL);
	//draws according to the active window
	if (src->activeWin == SP_MAIN_WINDOW)
		spMainWindowDraw(src->mainWin);
	else if (src->activeWin==SP_GAME_WINDOW)
		spGameWindowDraw(src->gameWin);
	else if (src->activeWin==SP_SETTINGS_WINDOW)
		spSettingsWindowDraw(src->settingsWin);
	else
		spLoadWindowDraw(src->loadWin);
}

SP_MANAGER_EVENET handleManagerDueToMainEvent(SPGuiManager* src,
		SP_MAIN_EVENT event) {
	assert(src!=NULL);
	if (event == SP_MAIN_START) { //if the user clicked on start
		spMainWindowHide(src->mainWin);
		if(src->settingsWin != NULL)
			spSettingsWindowDestroy(src->settingsWin);
		src->settingsWin = spSettingsWindowCreate(); //go to settings window
		if (src->settingsWin == NULL ) {
			failMessage("Couldn't create settings window!");
			return SP_MANAGER_QUIT; //quit game
		}
		src->activeWin = SP_SETTINGS_WINDOW; //set active window to settings window
		spSettingsWindowShow(src->settingsWin);
	}
	if (event == SP_MAIN_LOAD) { //if the user clicked on load
		spMainWindowHide(src->mainWin);
		src->prevWin = SP_MAIN_WINDOW;
		if(src->loadWin != NULL)
			spLoadWindowDestroy(src->loadWin);
		src->loadWin = spLoadWindowCreate(num_of_saved_files(src));  //go to load window
		if (src->loadWin == NULL ) {
			failMessage("Couldn't create load window!");
			return SP_MANAGER_QUIT;  //quit game
		}
		src->activeWin = SP_LOAD_WINDOW; //set active window to load window
		spLoadWindowShow(src->loadWin);
	}
	if (event == SP_MAIN_EXIT) { //quit game if the user clicked on exit
		return SP_MANAGER_QUIT;
	}
	return SP_MANAGER_NONE;
}


SP_MANAGER_EVENET handleManagerDueToLoadEvent(SPGuiManager* src,SP_LOAD_EVENT event) {
	assert(src!=NULL);
	int correct;
	if (event == SP_lOAD_EVENT_BACK) { //if the user clicked on back
		spLoadWindowHide(src->loadWin);
		if (src->prevWin==SP_MAIN_WINDOW){
			spMainWindowShow(src->mainWin); //show main window
			src->activeWin = SP_MAIN_WINDOW;
		}
		else{ //back from game board window
			correct = boardUpdate(src->gameWin->boardPanel,src->game,src->board_images); // updates board
			if(!correct)
				return SP_MANAGER_QUIT;
			spGameWindowShow(src->gameWin);
			src->activeWin=SP_GAME_WINDOW;
		}
		src->prevWin=SP_LOAD_WINDOW;
		return SP_MANAGER_NONE;
	}
	if (event == SP_lOAD_EVENT_lOAD){ // load button was clicked
		return loadEvent(src);
	}
	if (event == SP_LOAD_EVENT_QUIT) //if the user clicked on quit
		return SP_MANAGER_QUIT;
	return SP_MANAGER_NONE;
}

SP_MANAGER_EVENET loadEvent(SPGuiManager* src){
	assert(src != NULL);
	int correct = 0;
	for (int i=SLOT_BUTTONS_START_INDEX;i<src->loadWin->buttonsCounter;i++){
		if (src->loadWin->buttonsList[i-SLOT_BUTTONS_START_INDEX]->clicked==true){
			FILE* xml = fopen(src->saved_games[i-SLOT_BUTTONS_START_INDEX], "r"); //open the file
			if (!xml){ //no file
				failMessage("File doesn't exist or cannot be opened");
				break;
			}
			CH_Game* loadgame=fileToGame(xml);
			if(loadgame == NULL){ //error occurred
				failMessage("File doesn't exist or cannot be opened");
				break;
			}
			gameAssign(loadgame,src->game); //copies the loaded game to the game that the console has
			gameDestroy(loadgame); //destroys the load game
			spArrayListClear(src->game->undo_hist);
			if(ferror(xml)){ //if there was an error
				failMessage("File doesn't exist or cannot be opened");
				break;
			}
			else if (fclose(xml)!=0){ //close the file - if the close of the file failed
				failMessage("File doesn't exist or cannot be opened");
				break;
			}
			spLoadWindowHide(src->loadWin);
			src->prevWin=SP_LOAD_WINDOW;
			if (src->gameWin==NULL){ //create game windo
				src->gameWin=spGameWindowCreate(src->game,src->board_images);
				if(src->gameWin == NULL){//checks allocation
					failMessage("Couldn't create gui manager window!");
					return SP_MANAGER_QUIT;
				}
			}
			else{ //update game window
				correct = boardUpdate(src->gameWin->boardPanel,src->game,src->board_images);
				if(!correct)
					return SP_MANAGER_QUIT;
				undoUpdate(src->gameWin->setPanel,src->game); // upadtes undo button
				src->gameWin->is_up_to_date = true; // game is up to date
				src->gameWin->check_printed = false; // check was not printed yet
			}
			spGameWindowShow(src->gameWin); //show game window
			src->activeWin=SP_GAME_WINDOW;
			if(!PCMove(src->gameWin,src->board_images)) // computer move if needed
				return SP_MANAGER_QUIT;
			break;
		}
	}
	return SP_MANAGER_NONE;
 }

SP_MANAGER_EVENET handleManagerDueToGameEvent(SPGuiManager* src,SP_GAME_EVENT event) {
	if (event == SP_GAME_EVENT_NONE || src == NULL )
		return SP_MANAGER_NONE;
	if (event == SP_GAME_EVENT_QUIT) //the user clicked on quit
		return SP_MANAGER_QUIT;
	if (event == SP_GAME_EVENT_RESTART) //the user clicked on quit
		return gameRestart(src);
	if ((event == SP_GAME_EVENT_MAIN_MENU)|| (event==SP_GAME_EVENT_SAVE_MAIN_MENU)){
		if (event == SP_GAME_EVENT_SAVE_MAIN_MENU) //if the user wanted to save before going to main menu
			saveGameGui(src); //save game
		spGameWindowHide(src->gameWin);
		src->prevWin = SP_MAIN_WINDOW;
		spMainWindowShow(src->mainWin); //show main window
		src->activeWin = SP_MAIN_WINDOW;
		CH_Game* new_game=gameCreate(HISTORY_SIZE,DEFAULT_DIFFICULTY,DEFAULT_COLOR,DEFAULT_MODE); //create new game
		if (new_game == NULL){
			failMessage("Couldn't allocate memory!");
			return SP_MANAGER_QUIT;
		}
		gameAssign(new_game,src->game); // copies game params
		gameDestroy(new_game);
		spArrayListClear(src->game->undo_hist); // clears history
		return SP_MANAGER_NONE;
	}
	if (event == SP_GAME_EVENT_LOAD){
		spGameWindowHide(src->gameWin);
		int num = num_of_saved_files(src); //get num of saved files
		if (src->loadWin==NULL){// no load window
			src->loadWin=spLoadWindowCreate(num_of_saved_files(src));//create load window
			if (src->loadWin == NULL){
				failMessage("Couldn't allocate memory!");
				return SP_MANAGER_QUIT;
			}
		}
		else{
		initButtons(src->loadWin->buttonsList,src->loadWin->buttonsCounter); //initialize buttons
		src->loadWin->buttonsList[LOAD_LOAD_BUTTON]->isEnabled=false;
		}
		for (int i=0;i<num;i++){ // initialize buttons
			src->loadWin->buttonsList[i]->isEnabled=false;
			src->loadWin->buttonsList[i]->toShow=true;
		}
		spLoadWindowDraw(src->loadWin);
		src->prevWin=SP_GAME_WINDOW;
		spLoadWindowShow(src->loadWin); //show load window
		src->activeWin = SP_LOAD_WINDOW;
		return SP_MANAGER_NONE;
	}
	if (event == SP_GAME_EVENT_SAVE || event == SP_GAME_EVENT_SAVE_EXIT){
		saveGameGui(src); //save game
		src->gameWin->is_up_to_date = true;
		if(event == SP_GAME_EVENT_SAVE_EXIT) // need to save and then exit
			return SP_MANAGER_QUIT;
		return SP_MANAGER_NONE;
	}
	if (event == SP_GAME_EVENT_UNDO)
		undoGameGui(src); //undo move
	return SP_MANAGER_NONE;
}


SP_MANAGER_EVENET handleManagerDueToSetEvent(SPGuiManager* src,
		SP_SETTINGS_EVENT event) {
	assert(src!=NULL);
	if (event == SP_SETTINGS_EVENT_NONE) { //no event
		return SP_MANAGER_NONE;
	}
	if (event ==SP_SETTINGS_EVENT_BACK){ //the user clicked on back
		drawSetPrevScreen(src); //go to back settings sub screen
		return SP_MANAGER_NONE;
	}
	else if (event==SP_SETTINGS_EVENT_NEXT){ //the user clicked on next
		drawSetNextScreen(src); //go to next settings sub screen
		return SP_MANAGER_NONE;
	}
	else if (event==SP_SETTINGS_EVENT_START){ //the user clicked on start
		spSettingsWindowHide(src->settingsWin);
		if(src->gameWin != NULL)
			spGameWindowDestroy(src->gameWin);
		src->gameWin = spGameWindowCreate(src->game,src->board_images); //go to game window
		if (src->gameWin == NULL){
			failMessage("Couldn't create game window!");
			return SP_MANAGER_QUIT;
		}
		spGameWindowShow(src->gameWin);
		src->activeWin = SP_GAME_WINDOW;
		if(!PCMove(src->gameWin,src->board_images)) // computer makes move if needed
			return SP_MANAGER_QUIT;
	}
	else if (event == SP_SETTINGS_EVENT_QUIT) {//the user clicked on quit
		return SP_MANAGER_QUIT;
	}
	return SP_MANAGER_NONE;
}

void drawSetNextScreen(SPGuiManager* src){
	assert(src!=NULL);
	if (src->settingsWin->curr_screen==GameModeScreen){ //if the user's curr screen is game mode
		spSettingsChangeToDifficulty(src->settingsWin); //go to difficulty screen
		src->settingsWin->curr_screen=DifficultyScreen;
	}
	else if(src->settingsWin->curr_screen==DifficultyScreen){ //if the user's curr screen is difficulty
		spSettingsChangeToColor(src->settingsWin); //go to color screen
		src->settingsWin->curr_screen=ColorScreen;
	}
}

void drawSetPrevScreen(SPGuiManager* src){
	assert(src!=NULL);
	if (src->settingsWin->curr_screen==ColorScreen) //if user's curr screen is color screen
		spSettingsChangeToDifficulty(src->settingsWin); //go to difficulty screen
	else if (src->settingsWin->curr_screen==DifficultyScreen) //if user's curr screen is difficulty screen
		spSettingsChangeToGameMode(src->settingsWin); //go to settings screen
	else if (src->settingsWin->curr_screen==GameModeScreen){ //if user's curr screen is game mode
			spSettingsWindowHide(src->settingsWin);
			spMainWindowShow(src->mainWin); //go to main window
			src->activeWin=SP_MAIN_WINDOW;
			src->prevWin=SP_MAIN_WINDOW;
	}
}

SP_MANAGER_EVENET spManagerHandleEvent(SPGuiManager* src, SDL_Event* event) {
	assert(src!=NULL);
	if (event == NULL ) {
		return SP_MANAGER_NONE;
	}
	if (src->activeWin == SP_MAIN_WINDOW) { //active window is main window
		SP_MAIN_EVENT mainEvent = spMainWindowHandleEvent(src->mainWin, event); //handle the event
		return handleManagerDueToMainEvent(src, mainEvent); //handle the windows due to the event of main window
	}
	else if (src->activeWin==SP_LOAD_WINDOW){ //active window is load window
		SP_LOAD_EVENT loadEvent = spLoadWindowHandleEvent(src->loadWin, event); //handle the event
		return handleManagerDueToLoadEvent(src, loadEvent);//handle the windows due to the event of load window
	}
	else if (src->activeWin==SP_SETTINGS_WINDOW){ //active window is settings window
		SP_SETTINGS_EVENT setEvent = spSettingsWindowHandleEvent(src->settingsWin, event,src->game); //handle the event
		return handleManagerDueToSetEvent(src, setEvent); //handle the windows due to the event of settings window
	}
	else { //active window is game window
		SP_GAME_EVENT gameEvent = spGameWindowHandleEvent(src->gameWin, event,src->board_images); //handle the event
		spManagerDraw(src);
		return handleManagerDueToGameEvent(src, gameEvent); //handle the windows due to the event of game window
	}
	return SP_MANAGER_NONE;
}

int num_of_saved_files(SPGuiManager* src){
	int counter=0; // counts how many files are saved
	for (int i=0; i<MAX_OF_SAVED_FILES;i++){
		if( access(src->saved_games[i],F_OK)!=-1)
			counter++;
	}
	return counter;
}

void saveUpdate(SPGuiManager* src){ // updates names and saved files
	int saved_counter = num_of_saved_files(src);
	if (saved_counter==MAX_OF_SAVED_FILES){
		remove(src->saved_games[saved_counter-1]);
		saved_counter--;
	}
	for (int i=saved_counter;i>0;i--)
		rename(src->saved_games[i-1], src->saved_games[i]);


}

SP_MANAGER_EVENET gameRestart(SPGuiManager* src){
	CH_Game* copy = gameCreate(HISTORY_SIZE,src->game->difficulty,src->game->user_color,src->game->mode);
	if (copy == NULL){ //allocation error
		failMessage("Couldn't allocate memory!");
		return SP_MANAGER_QUIT;
	}
	gameAssign(copy,src->game); // assigns the new game
	gameDestroy(copy);
	spArrayListClear(src->game->undo_hist); // clears history
	int correct = boardUpdate(src->gameWin->boardPanel,src->game,src->board_images); //updates board
	if(!correct)
		return SP_MANAGER_QUIT;
	undoUpdate(src->gameWin->setPanel,src->game); // updates undo button
	src->gameWin->is_up_to_date = true; // is up do date
	src->gameWin->check_printed = false; // check was not printed yet
	if(!PCMove(src->gameWin,src->board_images)) // computer move if needed
		return SP_MANAGER_QUIT;
	return SP_MANAGER_NONE;
 }



XML_MESSAGE saveGameGui(SPGuiManager* src){
	saveUpdate(src);
	XML_MESSAGE msg;
	FILE* xml=fopen(src->saved_games[0],"w"); //opens the game
	if (xml==NULL){ //open failed
		failMessage("File cannot be created or modified!");
		 return XML_FAILED;
	}
	msg=gameToFile(src->game,xml);
	if(msg == XML_ALLOC)
		fclose(xml);
	if ((msg==XML_FAILED)||(ferror(xml))){ //game is null or there was an error in the file
		failMessage("File cannot be created or modified!");
		return XML_FAILED;
	}
	if (fclose(xml)!=0){//closing the file failed
		failMessage("File cannot be created or modified!");
		return XML_FAILED;
	}
	return XML_SUCCESS;
}

void undoGameGui(SPGuiManager* src){
	assert(src!=NULL);
	gameUndoMove(src->game); //undo the move
	boardUpdate(src->gameWin->boardPanel,src->game,src->board_images);//update the board
	undoUpdate(src->gameWin->setPanel,src->game);
}
