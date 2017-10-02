
#include "Button.h"

Button* create_button(SDL_Rect* location, SDL_Renderer* buttonRenderer,
		const char* enable_image, const char* disable_image, bool isEnabled, bool toShow, bool isClicked) {
	assert(buttonRenderer!=NULL); //assertions
	assert(location!=NULL);
	if ( enable_image == NULL || disable_image == NULL)
		return NULL;
	Button* button=(Button*)malloc(sizeof(Button));
	if (!button){ // allocation error
		failMessage("Couldn't create a button! ");
		return NULL;
	} //button's fields update
	button->isEnabled=isEnabled;
	button->clicked=isClicked; button->toShow=toShow;
	button->buttonRenderer=buttonRenderer;
	button->location = NULL;
	button->location = copyLocation(location);
	if(button->location==NULL){
		free(button);
		return NULL;
	}
	SDL_Surface* loadingSurfaceEnable = SDL_LoadBMP(enable_image); //create surface for enable_image
	if (loadingSurfaceEnable == NULL) {
		free(button->location);
		free(button);
		failMessage("Couldn't create surface disable button! ");
		return NULL;
	}
	SDL_Surface* loadingSurfaceDisable = SDL_LoadBMP(disable_image);//create surface for disable_image
	if (loadingSurfaceDisable == NULL) {
		SDL_FreeSurface(loadingSurfaceEnable);
		free(button->location); free(button);
		failMessage("Couldn't create surface disable button! ");
		return NULL;
	}
	SDL_Texture* enableTexture = SDL_CreateTextureFromSurface(buttonRenderer,loadingSurfaceEnable);//create texture for enable button
	if (enableTexture == NULL) {
		SDL_FreeSurface(loadingSurfaceEnable);
		SDL_FreeSurface(loadingSurfaceDisable);
		free(button->location); free(button);
		failMessage("Couldn't create texture! ");
		return NULL;
	}
	SDL_FreeSurface(loadingSurfaceEnable);
	SDL_Texture* disableTexture = SDL_CreateTextureFromSurface(buttonRenderer,loadingSurfaceDisable);//create texture for disable button
		if (disableTexture == NULL) {
			SDL_FreeSurface(loadingSurfaceEnable);
			SDL_FreeSurface(loadingSurfaceDisable);
			SDL_DestroyTexture(enableTexture);
			free(button->location); free(button);
			failMessage("Couldn't create texture! ");
			return NULL;
		}
	SDL_FreeSurface(loadingSurfaceDisable); //update texture's fields
	button->disable_buttonTexture=disableTexture;
	button->enable_buttonTexture=enableTexture;
	return button;
}

void destroyButton(Button* button) {
	if (button == NULL ){
		return;
	}
	if(button->location!=NULL)
		free((button->location));
	button->location = NULL;
	if(button->disable_buttonTexture!=NULL)
		SDL_DestroyTexture(button->disable_buttonTexture);
	button->disable_buttonTexture = NULL;
	if(button->enable_buttonTexture!=NULL)
		SDL_DestroyTexture(button->enable_buttonTexture);
	button->enable_buttonTexture = NULL;
	free(button);
}

void drawButton(Button* button) {
	if (button == NULL || (!(button->toShow))) {
		return; /* not drawing the button at all */
	}
	if (button->isEnabled) { //if button is enabled - draw enabled texture
		SDL_RenderCopy(button->buttonRenderer, button->enable_buttonTexture, NULL, button->location);
	} else {//draw disabled texture
		SDL_RenderCopy(button->buttonRenderer, button->disable_buttonTexture, NULL, button->location);
	}
}

SDL_Rect* copyLocation(SDL_Rect* src){
	if(!src) //assertion
		return NULL;
	SDL_Rect* copy = malloc(sizeof(SDL_Rect));
	if(!copy)
		return NULL;
	//copies parameters
	copy->h = src->h;copy->w = src->w;
	copy->x = src->x;copy->y = src->y;
	return copy;
}

void initButtons(Button** buttonsList, int size){
	assert(buttonsList != NULL);
	for (int i=0;i<size;i++){
		buttonsList[i]->clicked=false; //initialize all buttons to not clicked
	}
}

void failMessage(char* string){
	assert(string != NULL); //assertion
	SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,"ERROR:",string,NULL); //pop up message
}
