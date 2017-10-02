#include "Cell.h"



Cell* createCell(SDL_Rect* location, SDL_Renderer* renderer,const char* cell_image) {
	assert(renderer!=NULL); //assertions
	assert(location!=NULL);

	Cell* cell=(Cell*)malloc(sizeof(Cell)); //creates new cell
	if (!cell){
		printf("ERROR: couldn't create cell\n");
		return NULL;
	}
	//cell's fields update
	cell->img = cell_image;
	cell->cellTexture=NULL;
	cell->renderer=renderer;
	cell->location = NULL;
	cell->location = copyLocation(location);
	if(cell->location==NULL){
		destroyCell(cell);
		return NULL;
	}
	bool check = textureUpdate(cell,cell_image); //updates cell's texture
	if(!check)
		return NULL;
	return cell;
}

void destroyCell(Cell* cell) { //destroys the given cell
	if (cell == NULL ){
		return;
	}
	if(cell->location!=NULL) // allocation error
		free(cell->location);
	cell->location = NULL;
	if(cell->cellTexture!=NULL)
		SDL_DestroyTexture(cell->cellTexture);
	cell->cellTexture = NULL;
	cell->renderer=NULL;
	free(cell);
}

void drawCell(Cell* cell) {
	if (cell == NULL) {
		return; // not drawing the cell at all
	}

	SDL_RenderCopy(cell->renderer, cell->cellTexture, NULL, cell->location); //draw cell
}

bool textureUpdate(Cell* cell, const char* cell_image){
	assert(cell != NULL);
	if(cell_image!=NULL){
		SDL_Surface* imageSurface = SDL_LoadBMP(cell_image); //create surface for enable_image
		if (imageSurface == NULL) {
			printf("ERROR: couldn't create surface\n");
			return false;
		}

		SDL_Texture* cellTexture = SDL_CreateTextureFromSurface(cell->renderer,imageSurface);//create texture for enable button
				if (cellTexture == NULL) {
					SDL_FreeSurface(imageSurface);
					printf("ERROR: couldn't create texture\n");
					return false;
				}
		SDL_FreeSurface(imageSurface);
		cell->cellTexture=cellTexture;	//update texture's fields
	}
	return true;
}
