
#include "Panel.h"

Panel* createPanel(SDL_Renderer* renderer, SDL_Surface* img, SDL_Rect* location,
		bool isSetPanel) {
	if (img == NULL || renderer == NULL)
		return NULL;
	Panel* panel = (Panel*) malloc(sizeof(Panel));
	if (!panel) {
		printf("ERROR: couldn't create panel\n");
		return NULL;
	}
	//init params
	panel->buttonsCounter = 0;
	panel->cellsCounter = 0;
	panel->cellsList=NULL,panel->buttonsList=NULL, panel->location=NULL;
	panel->renderer = renderer;
	panel->location = copyLocation(location);
	SDL_Texture* panelTexture = SDL_CreateTextureFromSurface(panel->renderer,
			img); //create texture for enable button
	if (panelTexture == NULL) {
		destroyPanel(panel);
		printf("ERROR: couldn't create texture\n");
		return NULL;
	}
	panel->isSetPanel = isSetPanel;
	if (isSetPanel) {//if setting panel
		panel->buttonsCounter = NUM_OF_SET_PANEL_BUTTONS;
		panel->buttonsList = createSetPanelButtons(panel->renderer); //create buttons for set panel
		if (panel->buttonsList == NULL){
			destroyPanel(panel);
			return NULL;
		}
	} else { //board panel
		panel->cellsCounter = GRID*GRID;
		panel->cellsList = createGamePanelCells(panel->renderer);//create buttons for game panel
		if (panel->cellsList == NULL){
			destroyPanel(panel);
			return NULL;
		}
	}
	panel->panelTexture = panelTexture;
	return panel;
}

Cell** createGamePanelCells(SDL_Renderer* renderer) {
	int j=0;
	Cell** cellsList = (Cell**) malloc(sizeof(Cell*) * (GRID*GRID));
	if (cellsList == NULL) {
		printf("Could not create buttons list: %s\n", SDL_GetError());
		return NULL;
	}
	//create rect to represent buttons
	SDL_Rect** rects = (SDL_Rect**) malloc(sizeof(SDL_Rect*) * (GRID*GRID));
	if (rects == NULL)
		return NULL;
	for (int i = 0; i < GRID*GRID; i++) {
		rects[i] = (SDL_Rect*) malloc(sizeof(SDL_Rect));
		if (rects[i] == NULL){
			for(int j=0;j<i;j++)
				free(rects[j]);
			free(rects);
			return NULL;
		}
		if(i%GRID == 0)
			j++;
		rects[i]->x = BOARD_START_X_PANEL+(BOARD_BORDER + (CELL_W * (i%GRID)));
		rects[i]->y = (BOARD_END - (CELL_W * j));
		rects[i]->h = CELL_W+1;
		rects[i]->w = CELL_W+2;
	}

	for (int i = 0; i < GRID*GRID; i++) {
			cellsList[i] = createCell(rects[i], renderer, NULL);
			if (cellsList[i] == NULL) { //error in init - destroy
				for(int j=0;j<GRID*GRID;j++)
					free(rects[j]);
				for(int k=0;k<i;k++)
					destroyCell(cellsList[k]);
				free(cellsList);
				free(rects);
				printf("Could not create cells: %s\n", SDL_GetError());
				return NULL;
			}
	}
	for(int j=0;j<GRID*GRID;j++)
		free(rects[j]);
	free(rects);
	return cellsList;
}



Button** createSetPanelButtons(SDL_Renderer* renderer) {
	Button** buttonsList = (Button**) malloc(sizeof(Button*) * NUM_OF_SET_PANEL_BUTTONS);
	if (buttonsList == NULL) {
		printf("Could not create buttons list: %s\n", SDL_GetError());
		return NULL;
	}
	//create rect to represent buttons
	SDL_Rect restartR = { .x = SET_PANEL_BUTTON_X, .y = SET_PANEL_BUTTON_Y,
			.h = SET_PANEL_BUTTON_H, .w = SET_PANEL_BUTTON_W };
	SDL_Rect saveR = { .x = SET_PANEL_BUTTON_X, .y = SET_PANEL_BUTTON_Y+SET_PANEL_BUTTON_H+SET_PANEL_BUTTON_X,
			.h = SET_PANEL_BUTTON_H, .w = SET_PANEL_BUTTON_W };
	SDL_Rect loadR = { .x = SET_PANEL_BUTTON_X, .y = SET_PANEL_BUTTON_Y+SET_PANEL_BUTTON_H*2+SET_PANEL_BUTTON_X*2,
			.h = SET_PANEL_BUTTON_H, .w = SET_PANEL_BUTTON_W };
	SDL_Rect undoR = { .x = SET_PANEL_BUTTON_X, .y = SET_PANEL_BUTTON_Y+SET_PANEL_BUTTON_H*3+SET_PANEL_BUTTON_X*3,
			.h = SET_PANEL_BUTTON_H, .w = SET_PANEL_BUTTON_W };
	SDL_Rect mainMenuR = { .x = SET_PANEL_BUTTON_X, .y = SET_PANEL_BUTTON_Y+4*SET_PANEL_BUTTON_H+4*SET_PANEL_BUTTON_X,
			.h = SET_PANEL_BUTTON_H, .w = SET_PANEL_BUTTON_W };
	SDL_Rect exitR = { .x = SET_PANEL_BUTTON_X, .y = SET_PANEL_BUTTON_Y+SET_PANEL_BUTTON_H*5+SET_PANEL_BUTTON_X*5,
			.h = SET_PANEL_BUTTON_H, .w = SET_PANEL_BUTTON_W };

	//buttons create
	buttonsList[0] = create_button(&restartR, renderer,
			"./images/new/restart.bmp", "./images/new/restart.bmp", true, true,
			false);
	buttonsList[1] = create_button(&saveR, renderer,
			"./images/new/saveGame.bmp", "./images/new/saveGame.bmp", true,
			true, false);
	buttonsList[2] = create_button(&loadR, renderer,
			"./images/new/LoadGameFinal.bmp", "./images/new/LoadGameFinal.bmp",
			true, true, false);
	buttonsList[3] = create_button(&undoR, renderer,
			"./images/new/UndoFinal.bmp", "./images/new/undo.bmp", false,
			true, false);
	buttonsList[4] = create_button(&mainMenuR, renderer,
			"./images/new/mainMenu.bmp", "./images/new/mainMenu.bmp", true,
			true, false);
	buttonsList[5] = create_button(&exitR, renderer,
			"./images/new/QuitGameFinal.bmp", "./images/new/QuitGameFinal.bmp", true,
			true, false);

	//destroy outside the func!
	for (int i = 0; i < NUM_OF_SET_PANEL_BUTTONS; i++) {
		if (buttonsList[i] == NULL) {
			for(int j=0;j<i;j++)
				destroyButton(buttonsList[j]);
			free(buttonsList);
			printf("Could not create button: %s\n", SDL_GetError());
			return NULL;
		}
	}
	return buttonsList;
}

void destroyPanel(Panel* panel) {
	if (panel == NULL) { // no need to destroy
		return;
	} // destroy all struct fields
	if (panel->buttonsList!=NULL){
		for (int i = 0; i < panel->buttonsCounter; i++){
			if (panel->buttonsList[i]!=NULL)
				destroyButton(panel->buttonsList[i]);
		}
		free(panel->buttonsList);
	}
	panel->buttonsList = NULL;
	if (panel->cellsList!=NULL){
		for (int i = 0; i < panel->cellsCounter; i++){
			if (panel->cellsList[i]!=NULL)
				destroyCell(panel->cellsList[i]);
		}
		free(panel->cellsList);
	}
	panel->cellsList = NULL;
	if(panel->location!=NULL)
		free(panel->location);
	panel->location = NULL;
	if (panel->panelTexture!=NULL)
		SDL_DestroyTexture(panel->panelTexture);
	panel->panelTexture = NULL;
	free(panel);
}

void drawPanel(Panel* panel) {
	if (panel == NULL) {
		return; //not drawing the panel at all
	}
	if (panel->isSetPanel) { //if button is enabled - draw enabled texture
		SDL_RenderCopy(panel->renderer, panel->panelTexture, NULL,panel->location);
		for (int i = 0; i < panel->buttonsCounter; i++) {
			drawButton(panel->buttonsList[i]);
		}
	} else { //draw disabled texture
		SDL_RenderCopy(panel->renderer, panel->panelTexture, NULL,panel->location);
		for (int i = 0; i < panel->cellsCounter; i++) {
			drawCell(panel->cellsList[i]);
		}
	}
}
