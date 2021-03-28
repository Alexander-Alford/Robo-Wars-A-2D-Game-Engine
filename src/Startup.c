/*
This file manages the functions involved in program startup and termination.
*/

#include "global.h"
#include "gfx.h"
#include "sfx.h"
#include "grid.h"

//This function creates the SDL Window that the game will be using.
void StartProgram()
{
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
	int imageflag = IMG_INIT_PNG;
	if (!(IMG_Init(imageflag) & imageflag))
		{
		printf("SDL_image could not be initiated! SDL_image Error: %s\n", IMG_GetError());
		}

	int mixflag = MIX_INIT_MP3;
	if(Mix_Init(mixflag) != mixflag)
		{
		printf("SDL mixer failed to initialize format support! SDL_mixer Error: %s\n", Mix_GetError());
		}


	puts("complete.");

	SDL_SetRenderDrawColor(RENDERER, 0xFF, 0x0, 0x0, 0xFF);	
	
	SCREEN_TEXTURE = SDL_CreateTexture(RENDERER, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, SCREEN.w, SCREEN.h);
	
	Bijec.text = LoadTexture("assets/Test.png");
	
	ReadMapData("assets/level.dat", 1);
	
	GAME_STATE = 2;
}

void TerminateProgram()
{
	puts("Program termination beginning...");

//Test Area------------------------


	
//----------------------------------

	//Destroy_Check_Texture(Background);
	//Background = NULL;

	SDL_DestroyTexture(SCREEN_TEXTURE);
	SDL_DestroyTexture(TileTexture);
	SDL_DestroyTexture(Bijec.text);
	free(LEVEL_TILES);
	
	
	//Frees loaded music.
	if(BGmusic)
	{
	Mix_FreeMusic(BGmusic);
	}
	//BackgroundMusic = NULL;
	
	//Destroy window and renderer.
	SDL_DestroyWindow(WINDOW);
	SDL_DestroyRenderer(RENDERER);
	WINDOW = NULL;
	RENDERER = NULL;
	
	//Quit SDL subsystems.
	Mix_CloseAudio();
	Mix_Quit();
	IMG_Quit();
	SDL_Quit();
}
	
	
