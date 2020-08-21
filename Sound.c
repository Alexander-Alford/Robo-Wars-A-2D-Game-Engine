#include <Global.h>
#include <string.h>

typedef struct
{
Mix_Chunk* p_sound;
GBFS flg; //a = 
unsigned int num_of_sounds;
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
			//if(p_check->num_playing_sound != 0)
			//{
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
			//}
		}	
	}
	else
	{
	printf("Warning! Checking NULL sound.\n");	
	}
}

void Add_Sound_To_Play_Queue(S_Bind* addition)
{
	if(addition)
	{
		for(register int i = 0; i < 100; i++)
		{
			if(!TO_PLAY_QUEUE[i])
			{
			TO_PLAY_QUEUE[i] = addition;
			return;			
			}
		}
	
	printf("Error! No room in que to add sound! \n");
	}
	else
	{
	printf("Error attempting to add sound to que! \n");	
	}
}

void Load_Music(const char* PATH)
{
	if(PATH)
	{
	BackgroundMusic = Mix_LoadMUS(PATH);
		
		if(!BackgroundMusic)
		{
		printf("Failed to load music! SDL_mixer Error: %s", Mix_GetError());	
		}
		else
		{
		Mix_PlayMusic(BackgroundMusic, -1);	
		Mix_VolumeMusic(20);	
		printf("\"%s\" loaded as BGM. \n", PATH);
		}
	}
	else
	{
	printf("Error! Music PATH is NULL! /n");	
	}		
}

void Play_Sounds()
{
	for(register int i = 0; i < 100; i++)
	{
		if(TO_PLAY_QUEUE[i])
		{
			Mix_PlayChannel(-1, TO_PLAY_QUEUE[i]->p_sound, 0);
			TO_PLAY_QUEUE[i] = NULL;
		}
	}
}



void ControlSounds()
{
	
	Play_Sounds();
	
	
}