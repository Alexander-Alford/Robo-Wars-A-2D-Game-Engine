#include <Global.h>
#include <SDL.h>
#include <SDL_mixer.h>
#include <string.h>

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

S_Bind* TO_PLAY_QUEUE[100] = {NULL};


long int SOUND_COUNT = 0;

Mix_Music* BackgroundMusic = NULL;


//Function analogous to Assigm_texture and Check_Destroy_Texture.
S_Bind* Assign_Sound(S_Bind* reuse, char* PATH)
{
	if(reuse == NULL)
	{
		S_Bind* p_new = malloc(sizeof(S_Bind));
			
			if(p_new != NULL)
			{	
			p_new->p_sound = Mix_LoadWAV(PATH);
				
				if(p_new->p_sound == NULL)
				{
				printf("Error assigning sound! \n");	
				free(p_new);
				return NULL;
				}	

			p_new->pnts_to_chnk = 1;
			SOUND_COUNT++;
			unsigned int namebuffer = (strlen(PATH)+1);
			p_new->Name = malloc((namebuffer)*(sizeof(char)));
			strcpy(p_new->Name, PATH);
			p_new->Name[(namebuffer - 1)] = '\0';
			
			printf("Sound \"%s\" assigned. \n", p_new->Name);
			}
	return p_new;
	}
	
	else
	{
	reuse->pnts_to_chnk++;
	return reuse;
	}
}

void Check_Destroy_Sound(S_Bind* p_check)
{
	if(p_check != NULL)
	{
	p_check->pnts_to_chnk--;
	
		if(p_check->pnts_to_chnk < 1)
		{
			if(p_check->num_playing_sound != 0)
			{
			printf("Error! Attempt to destroy sound that is playing! \n");	
			}
			else
			{
			Mix_FreeChunk(p_check->p_sound);	
			SOUND_COUNT--;
			printf("	%s sound destroyed. \n", p_check->Name);
			free(p_check->Name);
			p_check->Name = NULL;
			free(p_check);	
			p_check = NULL;
			}
		}	
	}
	else
	{
	printf("Warning! Checking NULL sound.\n");	
	}
}


void PlaySounds()
{
	
	
	
	
}