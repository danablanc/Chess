
#include "Button.h"
#include "../ConsoleMode.h"

//struct that represents cell in game board
typedef struct cell
{
	SDL_Texture* cellTexture;//texture to be presented when the cell is enabled
	SDL_Renderer* renderer;//renderer for cell
	SDL_Rect* location;//rectangle to represent cell's location
	const char* img;
} Cell;


Cell* createCell(SDL_Rect* location, SDL_Renderer* renderer,
		const char* cell_image);

/**
 * Frees all memory resources associated with the source cell. If the
 * source cell is NULL, then the function does nothing.
 * @param cell - the source cell
 */
void destroyCell(Cell* cell);

/**
 * Draws the cell according to the cell state - is the cell enabled or not
 * If the source cell is NULL, then the function does nothing.
 *
 * @param cell - the source cell
 */
void drawCell(Cell* cell);

/**
 * Updates the cell's texture according to the given image and prints messages if neccesary
 *
 * @param cell - the source cell
 * @param cell_image - the cell's image to be shown on texture
 *
 * @return
 * true - if the update was successful
 * false - otherwise
 */
bool textureUpdate(Cell* cell, const char* cell_image);
