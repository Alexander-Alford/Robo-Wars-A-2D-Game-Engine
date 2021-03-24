/*
This file deals with all sound related functions and variables.
*/

#include "global.h"




long int MIX_CHUNK_COUNT = 0;
int8_t MUSIC_VOL = 25;

Mix_Music* BGmusic = NULL;

void LoadBGMusic(const char *PATH)
{
    if(!BGmusic)
    {
    BGmusic = Mix_LoadMUS(PATH);
    }
    if(!BGmusic) 
    {
    printf("Error loading background music \"%s\": %s\n", PATH, Mix_GetError());
    }
    else
    {
    Mix_VolumeMusic(MUSIC_VOL);
    Mix_PlayMusic(BGmusic, -1);
    }
}



void CoreSound()
{
	
	
	
}