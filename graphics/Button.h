
#ifndef GRAPHICS_BUTTON_H_
#define GRAPHICS_BUTTON_H_

#include <SDL.h>
#include <SDL_video.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>


// MACRO
#define inRange(x,min,max) ((x>=min)&&(x<=max))


//structre that represents button
typedef struct button_t {
	bool isEnabled;//flag if the button is enabled
	bool clicked; //flag if the button was clicked in this session
	bool toShow;//flag if the button is showed right now
	SDL_Texture* enable_buttonTexture;//texture to be presented when the button is enabled
	SDL_Texture* disable_buttonTexture;//texture to be presented when the button is disabled
	SDL_Renderer* buttonRenderer;//renderer for button
	SDL_Rect* location;//rectangle to represent button's location
} Button;

/**
 *  Creates a button for gui mode, if an error occured, prints a suitable message.
 *
 *  @param location - rectangle that represents the buttons place
 *  @param buttonRenderer - renderer for the button
 *  @param enable_image - path that represents the image of enabled button
 *  @param disable_image - bpathmp that represents the image of disabled button
 *  @param isEnabled - flag that represents if the button is enabled
 *  @param toShow - flag that represents if the button needs to be shown
 *  @param isClicked - flag that represents if the button was clicked
 *
 *  @return
 *  NULL, if an allocation error occurred
 *  button - otherwise
 */
Button* create_button(SDL_Rect* location, SDL_Renderer* buttonRenderer,
		const char* enable_image, const char* disable_image, bool isEnabled, bool toShow, bool isClicked);

/**
 * Frees all memory resources associated with the source button. If the
 * source button is NULL, then the function does nothing.
 * @param button - the source button
 */
void destroyButton(Button* button);

/**
 * Draws the button according to the button state - is the button enabled or not
 * If the source button is NULL, then the function does nothing.
 *
 * @param button - the source button
 */
void drawButton(Button* button);

/**
 * Copies a given rectangle
 *
 * @param src - the source rectangle
 *
 *  @return
 *  NULL, if an allocation error occurred or  src == NULL
 *  copy of the rectangle - otherwise
 */
SDL_Rect* copyLocation(SDL_Rect* src);

/**
 * initializes all given buttons to not clicked mode
 *
 * @param buttonsList - the buttons that need to be initialized to not clicked
 * @param size - the size of the buttonsList
 *
 * the function is void
 */
void initButtons(Button** buttonsList, int size);

/**
 * function that prints fail message (ERROR + given string)
 *
 * @param string - the string that represents the error
 *
 * the function is void
 */
void failMessage(char* string);

#endif /* GRAPHICS_BUTTON_H_ */
