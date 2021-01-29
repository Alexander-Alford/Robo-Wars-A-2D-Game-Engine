//This file manages all of the startup functions for the program.



#include <Global.h>
#include <Graphics.h>
#include <Object.h>
#include <Grid.h>
#include <Experiment.h>
#include <C_and_R.h>
#include <Sound.h>	


//This function creates the SDL Window that the game will be using.
void StartProgram(){
	puts("	Initialize SDL systems and subsystems...");
	
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
		{ 
		printf("SDL failed to initialize! SDL_Error: %s\n", SDL_GetError());		
		}
	
	WINDOW = SDL_CreateWindow("Title", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
	if (WINDOW == NULL)
		{ 
		printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
		}

	RENDERER = SDL_CreateRenderer(WINDOW, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (RENDERER == NULL)
		{
		printf("Renderer could not be created! SDL_image Error: %s\n", IMG_GetError());
		}
	
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
		{
		printf("SDL mixer failed to initialize! SDL_mixer Error: %s \n", Mix_GetError());	
		}
		
		
	//Initialize SDL_Image and PNG loading.
	int F_image = IMG_INIT_PNG;
	if (!(IMG_Init(F_image) & F_image))
		{
		printf("SDL_image could not be initiated! SDL_image Error: %s\n", IMG_GetError());
		}

	puts("complete.");

	SDL_SetRenderDrawColor(RENDERER, 0xFF, 0x0, 0x0, 0xFF);	
	
	SCREEN_TEXTURE = SDL_CreateTexture(RENDERER, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, SCREEN.w, SCREEN.h);
}

void TerminateProgram(){
	puts("Program termination beginning...");

//Test Area------------------------

	SDL_DestroyTexture(SCREEN_TEXTURE);
	Destroy_Object_Array(OBJECT_P_ARRAY);
	
//----------------------------------

	Destroy_Check_Texture(Background);
	Background = NULL;

	Free_Tile_Texture();
	Free_Tile_Array();
	
	printf("%d objects after termination. \n", OBJECT_COUNT);
	
	//Frees loaded music.
	Mix_FreeMusic(BackgroundMusic);
	BackgroundMusic = NULL;
	
	//Destroy window and renderer.
	SDL_DestroyWindow(WINDOW);
	SDL_DestroyRenderer(RENDERER);
	WINDOW = NULL;
	RENDERER = NULL;
	
	//Quit SDL subsystems.
	Mix_Quit();
	IMG_Quit();
	SDL_Quit();
}
	
	
