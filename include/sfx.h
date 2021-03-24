#ifndef SFX_H
#define SFX_H

#include <SDL_mixer.h>



Mix_Music* BGmusic;

void LoadBGMusic(const char *PATH);
void CoreSound();

#endif