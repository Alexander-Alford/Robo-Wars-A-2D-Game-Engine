#include <SDL_mixer.h>


typedef struct
{
Mix_Chunk** p_sounds;
GBFS flg; 
unsigned int num_of_sounds;
unsigned int pnts_to_sbank;	
unsigned int ID;	
} S_Bind;

Mix_Music* BackgroundMusic;
extern long int SOUND_COUNT;

S_Bind* TO_PLAY_QUEUE[100];

void Load_Music(const char* PATH);
int Add_Sound_To_Play_Queue(Mix_Chunk* addition);
S_Bind* Assign_Sound_Bank(S_Bind* reuse, const char* s_bank_PATH, unsigned int ID_buf);
void Check_Destroy_Sbank(S_Bind* p_check);
void ControlSounds();