#include <SDL_mixer.h>

Mix_Music* BackgroundMusic;


typedef struct
{
Mix_Chunk* p_sound;
unsigned int num_playing_sound;
int channel;
int repeat;
int volume;
unsigned int pnts_to_chnk;	
char* Name;
} S_Bind;


S_Bind* Assign_Sound(S_Bind* reuse, char* PATH);
void PlaySounds();