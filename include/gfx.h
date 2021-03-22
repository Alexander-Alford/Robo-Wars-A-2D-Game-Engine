
#ifndef GFX_H
#define GFX_H

#include "global.h"





//Declaration of pointers to SDL surface, texture, window, and renderer structures.
SDL_Window* WINDOW;
SDL_Renderer* RENDERER;
SDL_Texture* SCREEN_TEXTURE;
SDL_Texture* Background;
extern long int TEXTURE_COUNT;
extern float AVERAGE_FPS;
extern uint32_t FRAMES_ELAPSED;

//Declaration of functions.
SDL_Surface* Load_Surface(const char* PATH);
SDL_Texture* Load_Texture(const char* PATH);


void Position_Screen(Box* center);

#endif