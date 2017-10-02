

#include "SettingWin.h"

int isClickOnSet(int x, int y,SPSettingsWin* src) {
	assert(src!=NULL);
	int start_x=SET_DRAG_INIT,end_x=SET_DRAG_INIT,start_y=SET_DRAG_INIT,end_y=SET_DRAG_INIT;
	bool shown=false;
	for(int i=0;i<src->buttonsCounter;i++){//is button shown
		start_x = src->buttonsList[i]->location->x;
		end_x = src->buttonsList[i]->location->x + src->buttonsList[i]->location->w;
		start_y = src->buttonsList[i]->location->y;
		end_y = src->buttonsList[i]->location->y + src->buttonsList[i]->location->h;
		shown = src->buttonsList[i]->toShow;
		if(inRange(x,start_x,end_x) && inRange(y,start_y,end_y) && shown)//check click
			return i;
	}
	return -1;
}

SPSettingsWin* spSettingsWindowCreate() {
	SPSettingsWin* res = NULL;
	bool check;
	res = (SPSettingsWin*) calloc(sizeof(SPSettingsWin), sizeof(char));
	if (res == NULL) {
		failMessage("Couldn't create settings window!");
		return NULL;
	}

	// Create an application window with the following settings:
	res->window = SDL_CreateWindow("Game Settings",SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED, SET_WINDOW_W, SET_WINDOW_H, SDL_WINDOW_OPENGL);
	// Check that the window was successfully created
	if (res->window == NULL) {
		failMessage("Couldn't create settings window!");
		spSettingsWindowDestroy(res);
		return NULL;
	}
	res->renderer = SDL_CreateRenderer(res->window, -1,
			SDL_RENDERER_ACCELERATED);
	if (res->renderer == NULL) { // In the case that the window could not be made...
		failMessage("Couldn't create settings window!");
		spSettingsWindowDestroy(res);
		return NULL;
	}
	res->curr_screen=GameModeScreen;
	res->buttonsCounter = NUM_OF_SET_BUTTONS; //get buttons number
	res->buttonsList = (Button**) malloc(sizeof(Button*)*(res->buttonsCounter));  //create buttons list
	if (res->buttonsList == NULL) {
		failMessage("Couldn't create settings window!");
		spSettingsWindowDestroy(res);
		return NULL;
	}
	check = setButtonsInit(res); // initializes buttons
	if(!check){ // an error occured
		failMessage("Couldn't create settings window!");
		spSettingsWindowDestroy(res);
		return NULL;
	}
	return res;
}


bool setButtonsInit(SPSettingsWin* res){
	assert(res != NULL);
	//create rectangles of buttons
	SDL_Rect noob = { .x = DIF_X, .y = DIF_START_Y, .h = DIF_H, .w = DIF_W };
	SDL_Rect easy = { .x = DIF_X, .y = DIF_START_Y+DIF_H+DIF_GAP, .h = DIF_H, .w = DIF_W };
	SDL_Rect moderate = { .x = DIF_X, .y = DIF_START_Y+DIF_H*2+DIF_GAP*2, .h = DIF_H, .w = DIF_W };
	SDL_Rect hard = { .x = DIF_X, .y = DIF_START_Y+DIF_H*3+DIF_GAP*3, .h = DIF_H, .w = DIF_W };
	SDL_Rect white = { .x = WHITE_X_BUTTON, .y = COLOR_BUTTON_Y, .h = COLOR_BUTTON_H, .w = DIF_W };
	SDL_Rect black = { .x = BLACK_X_BUTTON, .y = COLOR_BUTTON_Y, .h = COLOR_BUTTON_H, .w = DIF_W };
	SDL_Rect twoPlayer = { .x = PLAYERS_X_BUTTON, .y = PLAYERS_Y, .h = PLAYERS_H, .w = PLAYERS_W };
	SDL_Rect onePlayer = { .x = PLAYER_X_BUTTON, .y = PLAYERS_Y, .h = PLAYERS_H, .w = PLAYERS_W };
	SDL_Rect backR = { .x = 270, .y = OTHERS_BUT_Y, .h = OTHERS_BUT_H, .w = OTHERS_BUT_WIT };
	SDL_Rect startR = { .x = OTHERS_BUT_X, .y = OTHERS_BUT_Y, .h = OTHERS_BUT_H, .w = OTHERS_BUT_WIT };
	SDL_Rect nextR = { .x = OTHERS_BUT_X, .y = OTHERS_BUT_Y, .h = OTHERS_BUT_H, .w = OTHERS_BUT_WIT };

	//create buttons
	res->buttonsList[NOOB_BUTTON] = create_button(&noob, res->renderer,
			"./images/new/noobFinal.bmp", "./images/new/noob.bmp", false,false,false);
	res->buttonsList[EASY_BUTTON] = create_button(&easy, res->renderer,
			"./images/new/easyFinal.bmp", "./images/new/easy.bmp", true,false,true);
	res->buttonsList[MODERATE_BUTTON] = create_button(&moderate, res->renderer,
			"./images/new/moderateFinal.bmp", "./images/new/moderate.bmp", false, false,false);
	res->buttonsList[HARD_BUTTON] = create_button(&hard, res->renderer,
			"./images/new/hardFinal.bmp", "./images/new/hard.bmp", false, false,false);
	res->buttonsList[WHITE_BUTTON] = create_button(&white, res->renderer,
			"./images/new/k_b_wFinal.bmp", "./images/new/k_b_w.bmp", true, false,true);
	res->buttonsList[BLACK_BUTTON] = create_button(&black, res->renderer,
			"./images/new/k_b_bFinal.bmp", "./images/new/k_b_b.bmp", false, false,false);
	res->buttonsList[TWO_PLAYER] = create_button(&twoPlayer, res->renderer,
			"./images/new/twoPlayersFinal.bmp", "./images/new/twoPlayers.bmp", false, true,false);
	res->buttonsList[ONE_PLAYER] = create_button(&onePlayer, res->renderer,
			"./images/new/onePlayerFinal.bmp", "./images/new/onePlayer.bmp", true, true,true);
	res->buttonsList[SET_BACK_BUTTON] = create_button(&backR, res->renderer,
			"./images/new/Back.bmp", "./images/new/Back.bmp", true, true,false);
	res->buttonsList[SET_START_BUTTON] = create_button(&startR, res->renderer,
			"./images/new/startGame.bmp", "./images/new/startGame.bmp", false, false,false);
	res->buttonsList[SET_NEXT_BUTTON] = create_button(&nextR, res->renderer,
			"./images/new/next.bmp", "./images/new/next.bmp", false, true,false);
	for(int i=0;i<NUM_OF_SET_BUTTONS;i++){
		if(res->buttonsList[i] == NULL){ // check if all buttons created
			failMessage("Couldn't create button!");
			spSettingsWindowDestroy(res);
			return false;
		}
	}
	return true;
}


void spSettingsWindowDestroy(SPSettingsWin* src) {
	if (!src) { // no need to destroy
		return;
	} // destroy all struct fields
	if (src->buttonsList != NULL){
		for (int i=0;i<src->buttonsCounter;i++){
			if(src->buttonsList[i] != NULL)
				destroyButton(src->buttonsList[i]);
		}
		free(src->buttonsList);
	}
	src->buttonsList = NULL;
	if (src->renderer != NULL) {
		SDL_DestroyRenderer(src->renderer);
	}
	src->renderer = NULL;
	if (src->window != NULL) {
		SDL_DestroyWindow(src->window);
	}
	src->window = NULL;
	free(src);
}

void spSettingsWindowDraw(SPSettingsWin* src) {
	if (src == NULL) {
		return;
	}
	SDL_SetRenderDrawColor(src->renderer,  SET_R, SET_G, SET_B, 0);//draw background
	SDL_RenderClear(src->renderer);
	for (int i=0;i<src->buttonsCounter;i++){//draw buttons
		drawButton(src->buttonsList[i]);
	}
	SDL_RenderPresent(src->renderer);
}

void spSettingsChangeToGameMode(SPSettingsWin* src) {
	if (src == NULL) {
		return;
	} // changing window to mode window
	SDL_SetRenderDrawColor(src->renderer, SET_R, SET_G, SET_B, 0);
	SDL_RenderClear(src->renderer);
	for (int i=0;i<src->buttonsCounter;i++){ // updated buttons on window
		if (i==ONE_PLAYER||i==TWO_PLAYER||i==SET_BACK_BUTTON||i==SET_NEXT_BUTTON)
			src->buttonsList[i]->toShow=true;
		else src->buttonsList[i]->toShow=false;
		if (inRange(i,SET_BACK_BUTTON,SET_NEXT_BUTTON))
			src->buttonsList[i]->clicked=false;
	}
	src->curr_screen=GameModeScreen;
	SDL_RenderPresent(src->renderer);
}

void spSettingsChangeToDifficulty(SPSettingsWin* src) {
	if (src == NULL) {
		return;
	} // changing to difficulty window
	SDL_SetRenderDrawColor(src->renderer, SET_R, SET_G, SET_B, 0);
	SDL_RenderClear(src->renderer);
	for (int i=0;i<src->buttonsCounter;i++){ //updates buttons in window
		if ((i>=NOOB_BUTTON && i<NUM_OF_DIFFICULTIES)||(i==SET_BACK_BUTTON)||(i==SET_NEXT_BUTTON))
			src->buttonsList[i]->toShow=true;
		else src->buttonsList[i]->toShow=false;
		if (inRange(i,SET_BACK_BUTTON,SET_NEXT_BUTTON))
			src->buttonsList[i]->clicked=false;
	}
	src->curr_screen=DifficultyScreen;
	SDL_RenderPresent(src->renderer);
}

void spSettingsChangeToColor(SPSettingsWin* src) {
	if (src == NULL) {
		return;
	} // changing to color window
	SDL_SetRenderDrawColor(src->renderer, COLOR_R, COLOR_G, COLOR_B, 0);
	SDL_RenderClear(src->renderer);
	for (int i=0;i<src->buttonsCounter;i++){ // updates buttons in window
		if ((i==BLACK_BUTTON)||(i==WHITE_BUTTON)||(i==SET_BACK_BUTTON)||(i==SET_START_BUTTON))
			src->buttonsList[i]->toShow=true;
		else src->buttonsList[i]->toShow=false;
		if (inRange(i,SET_BACK_BUTTON,SET_NEXT_BUTTON))
			src->buttonsList[i]->clicked=false;
	}
	src->curr_screen=ColorScreen;
	SDL_RenderPresent(src->renderer);
}

SP_SETTINGS_EVENT spSettingsWindowHandleEvent(SPSettingsWin* src, SDL_Event* event,CH_Game* game) {
	if (!event) {
		return SP_SETTINGS_EVENT_INVALID_ARGUMENT;
	}
	int res = isClickOnSet(event->button.x, event->button.y,src); //get event
	switch (event->type) {
	case SDL_MOUSEBUTTONUP: //click on button
		switch (res){ // switch on which button was clicked
		case NOOB_BUTTON:
			gameDiffChanges(src,game,NOOB_BUTTON);
			return SP_SETTINGS_EVENT_NONE;
		case EASY_BUTTON:
			gameDiffChanges(src,game,EASY_BUTTON);
			return SP_SETTINGS_EVENT_NONE;
		case MODERATE_BUTTON:
			gameDiffChanges(src,game,MODERATE_BUTTON);
			return SP_SETTINGS_EVENT_NONE;
		case HARD_BUTTON:
			gameDiffChanges(src,game,HARD_BUTTON);
			return SP_SETTINGS_EVENT_NONE;
		case WHITE_BUTTON:
			gameColorChanges(src,game,1);
			return SP_SETTINGS_EVENT_NONE;
		case BLACK_BUTTON:
			gameColorChanges(src,game,0);
			return SP_SETTINGS_EVENT_NONE;
		case TWO_PLAYER:
			gameModeChanges(src,game,false);
			return SP_SETTINGS_EVENT_NONE;
		case ONE_PLAYER:
			gameModeChanges(src,game,true);
			return SP_SETTINGS_EVENT_NONE;
		case SET_BACK_BUTTON:
			return SP_SETTINGS_EVENT_BACK;
		case SET_START_BUTTON:
			return SP_SETTINGS_EVENT_START;
		case SET_NEXT_BUTTON:
			return SP_SETTINGS_EVENT_NEXT;
		}
		break;
	case SDL_WINDOWEVENT: //close the window
		if (event->window.event == SDL_WINDOWEVENT_CLOSE) {
			return SP_SETTINGS_EVENT_QUIT;
		}
		break;
	default:
		return SP_SETTINGS_EVENT_NONE;
	}
	return SP_SETTINGS_EVENT_NONE;
}

void gameModeChanges(SPSettingsWin* src, CH_Game* game,bool one_player){
	if(one_player){ // updates buttons and game settings
		game->mode=DEFAULT_MODE;
		src->buttonsList[ONE_PLAYER]->isEnabled = true;
		src->buttonsList[TWO_PLAYER]->isEnabled = false;
	}
	else{
		game->mode=TWO_PLAYER_MODE;
		src->buttonsList[ONE_PLAYER]->isEnabled = false;
		src->buttonsList[TWO_PLAYER]->isEnabled = true;
	}
	src->buttonsList[ONE_PLAYER]->clicked=one_player;
	src->buttonsList[TWO_PLAYER]->clicked=(!one_player);
	src->buttonsList[SET_NEXT_BUTTON]->toShow=one_player;
	src->buttonsList[SET_START_BUTTON]->toShow=(!one_player);
}

void gameDiffChanges(SPSettingsWin* src, CH_Game* game,int diff){
	for(int i=0;i<NUM_OF_DIFFICULTIES;i++){ // updates game difficulty setting and buttons
		if(i!=diff){
			src->buttonsList[i]->clicked=false;
			src->buttonsList[i]->isEnabled=false;
		}
		else{
			src->buttonsList[i]->clicked=true;
			src->buttonsList[i]->isEnabled=true;
			game->difficulty = i+1;
		}
	}
}

void gameColorChanges(SPSettingsWin* src, CH_Game* game,int color){
	assert(src!=NULL);
	assert(game!=NULL);
	game->user_color = color; //updates user color
	src->buttonsList[WHITE_BUTTON]->clicked = color;
	src->buttonsList[BLACK_BUTTON]->clicked = (!color);
	src->buttonsList[WHITE_BUTTON]->isEnabled = color;
	src->buttonsList[BLACK_BUTTON]->isEnabled = (!color);
}

void spSettingsWindowHide(SPSettingsWin* src) {
	SDL_HideWindow(src->window);
}

void spSettingsWindowShow(SPSettingsWin* src) {
	SDL_ShowWindow(src->window);
}
