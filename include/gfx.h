
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
SDL_Surface* LoadSurface(const char* PATH);
SDL_Texture* LoadTexture(const char* PATH);


void PositionScreen(Box* center);

#endif