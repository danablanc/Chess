#include <stdio.h>

#include "GuiMode.h"

int startGuiMode() {
	if (SDL_Init(SDL_INIT_VIDEO) < 0) { //SDL2 INIT
		printf("ERROR: unable to init SDL: %s\n", SDL_GetError());
		return 1;
	}
	SPGuiManager* manager = spManagerCreate();
	if (manager == NULL ) {
		SDL_Quit();
		return 0;
	}
	SDL_Event event;
	int closed_request = 0;
	while (!closed_request) {
		if(requestUpdate(manager)){ // check if there is a close request + game status
			closed_request = 1;
			break;
		}
		while(SDL_PollEvent(&event)){
			if(requestUpdate(manager)){ // check if there is a close request + game status
				closed_request = 1;
				break;
			}
			if (spManagerHandleEvent(manager, &event) == SP_MANAGER_QUIT){ // check if there is a close request
				closed_request = 1;
				break;
			}
			spManagerDraw(manager);
		}
	}
	spManagerDestroy(manager);
	SDL_Quit();
	return 0;
}


int requestUpdate(SPGuiManager* manager){
	assert(manager != NULL);
	if(manager->game->game_status != REGULAR){ //check for change in status
		int result = statusMessege(manager->gameWin); // shows message if there is not regular status
		if(result == 2 || result == -1){ // need to quit
			return 1;
		}
		else if(result == 1){
			if(gameRestart(manager) == SP_MANAGER_QUIT){ //need to quit
				return 1;
			}
		}
	}
	return 0; // not quiting
}
