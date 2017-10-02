
#include "LoadWin.h"

//Inner functions
int isClickOnLoad(int x, int y,SPLoadWin* src) {
	assert(src!=NULL);
	int start_x=LOAD_ARGS_INIT,end_x=LOAD_ARGS_INIT,start_y=LOAD_ARGS_INIT,end_y=LOAD_ARGS_INIT;
	bool shown=false;
	for(int i=0;i<src->buttonsCounter;i++){ //copies parameters needed
		start_x = src->buttonsList[i]->location->x;
		end_x = src->buttonsList[i]->location->x + src->buttonsList[i]->location->w;
		start_y = src->buttonsList[i]->location->y;
		end_y = src->buttonsList[i]->location->y + src->buttonsList[i]->location->h;
		shown = src->buttonsList[i]->toShow;
		if(inRange(x,start_x,end_x) && inRange(y,start_y,end_y) && shown) //checks where there was a click on button
			return i; //returns on whick button there was a click
	}
	return ERROR_FLAG_LOAD;
}

SPLoadWin* spLoadWindowCreate(int num_of_saved_games) {
	SPLoadWin* res = NULL;
	bool check;
	res = (SPLoadWin*) calloc(sizeof(SPLoadWin), sizeof(char));
	if (res == NULL) {
		failMessage("Couldn't create load window!");
		return NULL;
	}
	// Create an application window with the following settings:
	res->windowLoad = SDL_CreateWindow("Load Game",SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,LOAD_WINDOW_W,LOAD_WINDOW_H,SDL_WINDOW_OPENGL);

	// Check that the window was successfully created
	if (res->windowLoad == NULL) {
		failMessage("Couldn't create load window!");
		spLoadWindowDestroy(res);
		return NULL;
	}
	//create renderer
	res->rendererLoad = SDL_CreateRenderer(res->windowLoad, -1,
			SDL_RENDERER_ACCELERATED);
	if (res->rendererLoad == NULL) {
		// In the case that the window could not be made...
		failMessage("Couldn't create load window!");
		spLoadWindowDestroy(res);
		return NULL;
	}
	res->buttonsCounter = NUM_OF_LOAD_BUTTONS;
	res->buttonsList = (Button**) malloc(
			sizeof(Button*) * (res->buttonsCounter));
	if (res->buttonsList == NULL) {
		failMessage("Couldn't create load window!");
		spLoadWindowDestroy(res);
		return NULL;
	}
	check = loadButtonsInit(res,num_of_saved_games); // initializes buttons
	if(!check)
		return NULL;
	return res;
}

bool loadButtonsInit(SPLoadWin* res,int num_of_saved_games){
	//create rect to represent buttons
	SDL_Rect slot1 = { .x = SLOTS_X_VAL, .y = SLOTS_Y_VAL, .h = SLOTS_H, .w = SLOTS_W };
	SDL_Rect slot2 = { .x = SLOTS_X_VAL, .y = SLOTS_Y_VAL+SLOTS_H+SLOTS_Y_OFFSET, .h = SLOTS_H, .w = SLOTS_W };
	SDL_Rect slot3 = { .x = SLOTS_X_VAL, .y = SLOTS_Y_VAL+SLOTS_H*2+SLOTS_Y_OFFSET*2, .h = SLOTS_H, .w = SLOTS_W };
	SDL_Rect slot4 = { .x = SLOTS_X_VAL, .y = SLOTS_Y_VAL+SLOTS_H*3+SLOTS_Y_OFFSET*3, .h = SLOTS_H, .w = SLOTS_W };
	SDL_Rect slot5 = { .x = SLOTS_X_VAL, .y = SLOTS_Y_VAL+SLOTS_H*4+SLOTS_Y_OFFSET*4, .h = SLOTS_H, .w = SLOTS_W };
	SDL_Rect backR = { .x = BACK_X, .y = OTHER_Y, .h = OTHER_H, .w = OTHER_W };
	SDL_Rect loadR = { .x = LOAD_X, .y = OTHER_Y, .h = OTHER_H, .w = OTHER_W };
	//buttons create
	res->buttonsList[SLOT1_BUTTON] = create_button(&slot1, res->rendererLoad,
			"./images/new/GameSlot1Final.bmp", "./images/new/GameSlot1.bmp", false,false,false);
	res->buttonsList[SLOT2_BUTTON] = create_button(&slot2, res->rendererLoad,
			"./images/new/GameSlot2Final.bmp", "./images/new/GameSlot2.bmp", false,false,false);
	res->buttonsList[SLOT3_BUTTON] = create_button(&slot3, res->rendererLoad,
			"./images/new/GameSlot3Final.bmp", "./images/new/GameSlot3.bmp", false, false,false);
	res->buttonsList[SLOT4_BUTTON] = create_button(&slot4, res->rendererLoad,
			"./images/new/GameSlot4Final.bmp", "./images/new/GameSlot4.bmp", false, false,false);
	res->buttonsList[SLOT5_BUTTON] = create_button(&slot5, res->rendererLoad,
			"./images/new/GameSlot5Final.bmp", "./images/new/GameSlot5.bmp", false, false,false);
	res->buttonsList[LOAD_BACK_BUTTON] = create_button(&backR, res->rendererLoad,
			"./images/new/Back.bmp", "./images/new/Back.bmp", true, true,false);
	res->buttonsList[LOAD_LOAD_BUTTON] = create_button(&loadR, res->rendererLoad,
			"./images/new/LoadGameFinal.bmp", "./images/new/LoadGame.bmp", false, true,false);
	for (int i=0;i<num_of_saved_games;i++){
		res->buttonsList[i]->isEnabled=false;
		res->buttonsList[i]->toShow=true;
	}
	for(int i=0;i<NUM_OF_LOAD_BUTTONS;i++) {
		if(res->buttonsList[i] == NULL){
			failMessage("Couldn't create load window!");
			spLoadWindowDestroy(res);
			return false;
		}
	}
	return true;
}

void spLoadWindowDestroy(SPLoadWin* src) {
	if (!src) { // no need to destroy
		return;
	}
	if(src->buttonsList != NULL){
		for (int i=0;i<src->buttonsCounter;i++){ //destroy buttons list
			if(src->buttonsList[i] != NULL)
				destroyButton(src->buttonsList[i]);
		}
		free(src->buttonsList);
	} // destroy all struct fields
	src->buttonsList = NULL;
	if (src->rendererLoad != NULL) {
		SDL_DestroyRenderer(src->rendererLoad);
	}
	src->rendererLoad = NULL;
	if (src->windowLoad != NULL) {
		SDL_DestroyWindow(src->windowLoad);
	}
	src->windowLoad = NULL;
	free(src);
}

void spLoadWindowDraw(SPLoadWin* src) {
	assert(src!=NULL);
	SDL_SetRenderDrawColor(src->rendererLoad, LOAD_R, LOAD_G, LOAD_B, 0); //set background color
	SDL_RenderClear(src->rendererLoad);
	for (int i=0;i<src->buttonsCounter;i++){//draw all buttons
		drawButton(src->buttonsList[i]);
	}
	SDL_RenderPresent(src->rendererLoad);
}

SP_LOAD_EVENT spLoadWindowHandleEvent(SPLoadWin* src, SDL_Event* event) {
	if ((!event) ||(!src)) {
		printf("ERROR: INVALID ARGUMENT\n");
		return SP_lOAD_EVENT_INVALID_ARGUMENT;
	}
	int res = isClickOnLoad(event->button.x, event->button.y,src);
	switch (event->type) {
	case SDL_MOUSEBUTTONUP:
		switch (res){ //checks which button was clicked
		case SLOT1_BUTTON:
			activateAfterClick(src,SP_lOAD_EVENT_SLOT1);
			return SP_lOAD_EVENT_NONE;
		case SLOT2_BUTTON:
			activateAfterClick(src,SP_lOAD_EVENT_SLOT2);
			return SP_lOAD_EVENT_NONE;
		case SLOT3_BUTTON:
			activateAfterClick(src,SP_lOAD_EVENT_SLOT3);
			return SP_lOAD_EVENT_NONE;
		case SLOT4_BUTTON:
			activateAfterClick(src,SP_lOAD_EVENT_SLOT4);
			return SP_lOAD_EVENT_NONE;
		case SLOT5_BUTTON:
			activateAfterClick(src,SP_lOAD_EVENT_SLOT5);
			return SP_lOAD_EVENT_NONE;
		case LOAD_BACK_BUTTON:
			return SP_lOAD_EVENT_BACK;
		case LOAD_LOAD_BUTTON:
			return SP_lOAD_EVENT_lOAD;
		}
		break;
	case SDL_WINDOWEVENT:
		if (event->window.event == SDL_WINDOWEVENT_CLOSE) { //checks whether the user tried to close the window
			return SP_LOAD_EVENT_QUIT;
		}
		break;
	default:
		return SP_lOAD_EVENT_NONE;
	}
	return SP_lOAD_EVENT_NONE;
}

void activateAfterClick(SPLoadWin* src, int event){
	for (int i=SLOT_BUTTONS_START_INDEX;i<src->buttonsCounter;i++){ // activates the button was clicked
		if (event == i) {
			if (src->buttonsList[i-SLOT_BUTTONS_START_INDEX]->toShow){
				src->buttonsList[i-SLOT_BUTTONS_START_INDEX]->clicked=true;
				src->buttonsList[LOAD_LOAD_BUTTON]->isEnabled=true;
				drawButton(src->buttonsList[LOAD_LOAD_BUTTON]);
				break;
			}
		}
	}
	for(int i=SLOT_BUTTONS_START_INDEX; i<src->buttonsCounter;i++){ // unactivates the other buttons
		if(event != i){
			src->buttonsList[i-SLOT_BUTTONS_START_INDEX]->clicked = false;
			src->buttonsList[i-SLOT_BUTTONS_START_INDEX]->isEnabled = false;
		}
		else{
			src->buttonsList[i-SLOT_BUTTONS_START_INDEX]->clicked = true;
			src->buttonsList[i-SLOT_BUTTONS_START_INDEX]->isEnabled = true;
		}
	}
}

void spLoadWindowHide(SPLoadWin* src) {
	assert(src!=NULL);
	SDL_HideWindow(src->windowLoad);
}

void spLoadWindowShow(SPLoadWin* src) {
	assert(src!=NULL);
	SDL_ShowWindow(src->windowLoad);
}
