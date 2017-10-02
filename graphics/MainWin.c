#include "MainWin.h"


//Inner functions
int isClickOnMain(int x, int y,SPMainWin* src) {
	assert(src!=NULL);
	int start_x=MAIN_ARGS_INIT,end_x=MAIN_ARGS_INIT,start_y=MAIN_ARGS_INIT,end_y=MAIN_ARGS_INIT;
	bool shown=false; //is button shown
	for(int i=0;i<src->buttonsCounter;i++){  //get location
		start_x = src->buttonsList[i]->location->x;
		end_x = src->buttonsList[i]->location->x + src->buttonsList[i]->location->w;
		start_y = src->buttonsList[i]->location->y;
		end_y = src->buttonsList[i]->location->y + src->buttonsList[i]->location->h;
		shown = src->buttonsList[i]->toShow;
		if(inRange(x,start_x,end_x) && inRange(y,start_y,end_y) && shown) //check click
			return i;
	}
	return ERROR_FLAG_MAIN;
}

SPMainWin* spMainWindowCreate() {
	SPMainWin* res = NULL;
	res = (SPMainWin*) calloc(sizeof(SPMainWin), sizeof(char));
	if (res == NULL ) {
		failMessage("Couldn't create main window!");
		return NULL ;
	}
	// Create an application window with the following settings:
	res->mainWindow = SDL_CreateWindow("Chess",SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,MAIN_WINDOW_W,MAIN_WINDOW_H,SDL_WINDOW_OPENGL);
	// Check that the window was successfully created
	if (res->mainWindow == NULL ) {
		failMessage("Couldn't create main window!");
		spMainWindowDestroy(res);
		return NULL ;
	}
	res->mainRenderer = SDL_CreateRenderer(res->mainWindow, -1,
			SDL_RENDERER_ACCELERATED);
	if (res->mainRenderer == NULL ) {
		failMessage("Couldn't create main window!");
		spMainWindowDestroy(res);
		return NULL ;
	}
	res->buttonsCounter=NUM_OF_MAIN_BUTTONS; //get buttons number
	res->buttonsList=(Button**)malloc(sizeof(Button*)*(res->buttonsCounter)); //create buttons list
	if (res->buttonsList==NULL){
		failMessage("Couldn't create main window!");
		spMainWindowDestroy(res);
		return NULL;
	}
	//create rectangles of buttons
	SDL_Rect startR = { .x = MAIN_X, .y = MAIN_Y, .h = MAIN_H, .w = MAIN_W };
	SDL_Rect loadR = { .x = MAIN_X, .y = MAIN_Y+MAIN_H+MAIN_OFFSET, .h = MAIN_H, .w = MAIN_W };
	SDL_Rect exitR = { .x = MAIN_X, .y = MAIN_Y+MAIN_H*2+MAIN_OFFSET*2, .h = MAIN_H, .w = MAIN_W };
	//create buttons
	res->buttonsList[MAIN_START_BUTTON]=create_button(&startR, res->mainRenderer,
			"./images/new/newGameFinal.bmp","./images/new/newGameFinal.bmp", true,true,false);
	res->buttonsList[MAIN_LOAD_BUTTON]=create_button(&loadR, res->mainRenderer,
			"./images/new/LoadGameFinal.bmp", "./images/new/LoadGameFinal.bmp", true,true,false);
	res->buttonsList[MAIN_QUIT_BUTTON]=create_button(&exitR, res->mainRenderer,
			"./images/new/QuitGameFinal.bmp", "./images/new/QuitGameFinal.bmp", true,true,false);
	for(int i=0;i<NUM_OF_MAIN_BUTTONS;i++){
		if(res->buttonsList[i]==NULL){
			failMessage("Couldn't create button!");
			spMainWindowDestroy(res);
			return NULL ;
		}
	}
	return res;
}

void spMainWindowDestroy(SPMainWin* src) {
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
	if (src->mainRenderer != NULL ) {
		SDL_DestroyRenderer(src->mainRenderer);
	}
	src->mainRenderer = NULL;
	if (src->mainWindow != NULL ) {
		SDL_DestroyWindow(src->mainWindow);
	}
	src->mainWindow = NULL;
	free(src);
}

void spMainWindowDraw(SPMainWin* src) {
	assert(src!=NULL);
	SDL_SetRenderDrawColor(src->mainRenderer, MAIN_R, MAIN_G, MAIN_B, 0);//draw background
	SDL_RenderClear(src->mainRenderer);
	for (int i=0;i<src->buttonsCounter;i++){ //draw buttons
		drawButton(src->buttonsList[i]);
	}
	SDL_RenderPresent(src->mainRenderer);
}

SP_MAIN_EVENT spMainWindowHandleEvent(SPMainWin* src, SDL_Event* event) {
	if ((!event) || (!src)) {
		return SP_MAIN_INVALID_ARGUMENT;
	}
	int res = isClickOnMain(event->button.x, event->button.y,src); //get event
	switch (event->type) {
	case SDL_MOUSEBUTTONUP: //click on button
		switch(res){
		case MAIN_START_BUTTON:
			return SP_MAIN_START;
		case MAIN_LOAD_BUTTON:
			return SP_MAIN_LOAD;
		case MAIN_QUIT_BUTTON:
			return SP_MAIN_EXIT;
		}
		break;
	case SDL_WINDOWEVENT:
		if (event->window.event == SDL_WINDOWEVENT_CLOSE) { //close window
			return SP_MAIN_EXIT;
		}
		break;
	default:
		return SP_MAIN_NONE;
	}
	return SP_MAIN_NONE;
}

void spMainWindowHide(SPMainWin* src) {
	SDL_HideWindow(src->mainWindow);
}

void spMainWindowShow(SPMainWin* src) {
	SDL_ShowWindow(src->mainWindow);
}
