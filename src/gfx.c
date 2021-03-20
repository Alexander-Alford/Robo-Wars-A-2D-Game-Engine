/*
This file contains all of the graphical functions and constants.
*/

#include "global.h"



uint16_t WINDOW_WIDTH = 768;
uint16_t WINDOW_HEIGHT = 768;
float TARGET_FPS = 60.000;
float AVERAGE_FPS = 0;
uint32_t FRAMES_ELAPSED = 0;


SDL_Rect SCREEN = {0, 0, 256, 256};

uint32_t TEXTURE_COUNT = 0;


//Pointers to the window, renderer, and screen texture. Render->Screen_Texture->Window
SDL_Window* WINDOW = NULL;
SDL_Renderer* RENDERER = NULL;
SDL_Texture* SCREEN_TEXTURE = NULL; 

//Background T_Bind.
T_Bind* Background = NULL;


uint8_t isOnScreen(SDL_Rect* test)
{
	return boxTestColl(test, &SCREEN);
}

//Centers screen on box and does not overstep level bounds.
void Position_Screen(Box* center)
{
	int16_t box_x_p_half = center->x + (center->w / 2);
	int16_t box_y_p_half = center->y + (center->h / 2);

	int16_t half_screenw = SCREEN.w / 2;
	int16_t half_screenh = SCREEN.h / 2;

	SCREEN.x = box_x_p_half - half_screenw;
	SCREEN.y = box_y_p_half - half_screenh;

	if (SCREEN.x < 0)
	{
	SCREEN.x = 0;	
	}	
		else if ((SCREEN.x + SCREEN.w) > (HORIZONTAL_LEVEL_TILES*BASE_TILE_SIZE))
		{
		SCREEN.x = 	(HORIZONTAL_LEVEL_TILES*BASE_TILE_SIZE) - SCREEN.w;
		}

	if (SCREEN.y < 0)
	{
	SCREEN.y = 0;	
	}	
		else if ((SCREEN.y + SCREEN.h) > (VERTICAL_LEVEL_TILES*BASE_TILE_SIZE))
		{
		SCREEN.y = (VERTICAL_LEVEL_TILES*BASE_TILE_SIZE) - SCREEN.h;
		}
	
}


//Returns a loaded surface. Loads a surface, converts it to a texture, then returns it.
SDL_Surface* Load_Surface(const char* PATH)
{
	SDL_Surface* retSurf = IMG_Load(PATH); 

		if (retSurf == NULL)	
			{
			printf( "Image %s failed to load! SDL_image Error: %s\n", PATH, IMG_GetError());
			}

return retSurf; 
}	

SDL_Texture* Load_Texture(const char* PATH)
{
	SDL_Texture* retTex = NULL; 
	SDL_Surface* origSurf = NULL; 

		origSurf = Load_Surface(PATH); 

		retTex = SDL_CreateTextureFromSurface(RENDERER, origSurf); 

		
		if (retTex == NULL)
			{
			printf("Unable to create texture from %s! SDL Error: %s\n", PATH, SDL_GetError());
			}
		else
			{
			TEXTURE_COUNT++;		
			}	
		
		SDL_FreeSurface(origSurf); 
	
return retTex; 	
}




//Renders a given background texture repeated across the screen.
void renderBackground(SDL_Texture* bg)
{
	if(bg)
	{
		SDL_Rect rr = {0};
		uint8_t buf = 0x0;
	
		SDL_QueryTexture(bg, NULL, NULL, &rr.w, &rr.h);

	
		while(rr.y < (SCREEN.y + SCREEN.h))
		{
			while(rr.x < (SCREEN.x + SCREEN.w))
			{
			buf = isOnScreen(&rr);
			
				if( BIT_GET(buf, BIT0, uint8_t) )
				{
				rr.x -= SCREEN.x;	
				rr.y -= SCREEN.y;
				SDL_RenderCopy(RENDERER, bg, NULL, &rr);	
				rr.y += SCREEN.y;
				rr.x += SCREEN.x;	
			}
		
			rr.x += rr.w;
			}
	
		rr.y += rr.h;
		rr.x = 0;
		}
	
	}
}



//The core loop master function.
void CoreGraphic()
{

	//Sets the render target to the texture SCREEN_TEXTURE.
	SDL_SetRenderTarget(RENDERER, SCREEN_TEXTURE);

		SDL_RenderClear(RENDERER); //Unneccessary?
		



	SDL_SetRenderTarget(RENDERER, NULL);	
	


		
	SDL_RenderClear(RENDERER);	
	
	SDL_RenderCopy(RENDERER, SCREEN_TEXTURE, NULL, NULL);	
	SDL_RenderPresent(RENDERER);
	
	FRAMES_ELAPSED++;
	AVERAGE_FPS = (FRAMES_ELAPSED / (SDL_GetTicks()/1000.f));
}