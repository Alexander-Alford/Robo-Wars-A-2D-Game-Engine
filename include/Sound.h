#include <SDL_mixer.h>


typedef struct
{
Mix_Chunk* p_sound;
GBFS flg; 
unsigned int num_playing_sound;
int channel;
int repeat;
int volume;
unsigned int pnts_to_chnk;	
char* Name;
} S_Bind;

Mix_Music* BackgroundMusic;
extern long int SOUND_COUNT;

S_Bind* TO_PLAY_QUEUE[100];

void Load_Music(char* PATH);
void Add_Sound_To_Play_Queue(S_Bind* addition);
S_Bind* Assign_Sound(S_Bind* reuse, char* PATH);
void ControlSounds();