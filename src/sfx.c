#include <Global.h>


typedef struct
{
Mix_Chunk** p_sounds;
GBFS flg; //a = 
unsigned int num_of_sounds;
unsigned int pnts_to_sbank;	
unsigned int ID;	//Bank ID. 
} S_Bind;

Mix_Chunk* TO_PLAY_QUEUE[100] = {NULL};


long int SOUND_COUNT = 0;

Mix_Music* BackgroundMusic = NULL;


S_Bind* Load_Sounds(const char* PATH, unsigned int bank_id)
{
	S_Bind* new = malloc(sizeof(S_Bind));
	
	if(!new)
	{
	printf("Error loading sounds!\n");	
	return NULL;
	}
	
	FILE* p_s_bank = NULL;
	p_s_bank = fopen(PATH, "r");
	
	if(!p_s_bank)
	{
	printf("Error attempting to open \"%s\" for sounds!\n", PATH);
	return NULL;	
	}
	
	char char_buff = 'Z';
	unsigned int num_buff = 0;
	char sound_path_buff[100] = {0};
	
		while(fscanf(p_s_bank, "%c", &char_buff) != EOF)
		{
			if(char_buff == '@')
			{
			fscanf(p_s_bank, "%u", &num_buff);
			
				if(bank_id == num_buff)
				{
				new->ID = bank_id;
			
				fscanf(p_s_bank, "%u", &num_buff);	
				new->num_of_sounds = num_buff;
				
				
				new->p_sounds = malloc(num_buff * sizeof(Mix_Chunk*));
					
					for(register unsigned int i = 0; i < num_buff; i++)
					{
						fscanf(p_s_bank, "%100s", sound_path_buff);
								
						new->p_sounds[i] = Mix_LoadWAV(sound_path_buff);
				
						if(!new->p_sounds[i])
						{
						printf("Error assigning sound! %s \n", SDL_GetError());	
						}
						else
						{
						SOUND_COUNT++;							
						}
					}
				
				printf("	%u sfx were allocated. \n", new->num_of_sounds);
				new->pnts_to_sbank = 1;
				
				fclose(p_s_bank);
				return new;
				}
			}
		}
	
	printf("Error! Could not find desired sound bank!\n");
	free(new->p_sounds);
	free(new);
	fclose(p_s_bank);
	return NULL;
}

//Function analogous to Assigm_texture and Check_Destroy_Texture. Acts as a wrapper for sfx loading.
S_Bind* Assign_Sound_Bank(S_Bind* reuse, const char* s_bank_PATH, unsigned int ID_buf)
{
	if(reuse == NULL)
	{
		
	S_Bind* new_sb = Load_Sounds(s_bank_PATH, ID_buf);						
			
	printf("Sound bank #%u assigned. \n", new_sb->ID);
			
	return new_sb;
	}
	
	else
	{
	reuse->pnts_to_sbank++;
	return reuse;
	}
}

void Check_Destroy_Sbank(S_Bind* p_check)
{
	if(p_check != NULL)
	{
	p_check->pnts_to_sbank--;
	
		if(p_check->pnts_to_sbank < 1)
		{
	
			for(unsigned register int i = 0; i < p_check->num_of_sounds; i++) //First PATH is name of bank.
			{				
					free(p_check->p_sounds[i]);						
					SOUND_COUNT--;											
			}	
			
			printf("%u sfx were deallocated. \n", p_check->num_of_sounds);
			printf("	sound bank #%u destroyed. \n", p_check->ID);
			free(p_check->p_sounds);
			free(p_check);			
		}	
	}
	else
	{
	printf("Warning! Checking NULL sound.\n");	
	}
}

int Add_Sound_To_Play_Queue(Mix_Chunk* addition)
{
	if(addition)
	{
		for(register int i = 0; i < 100; i++)
		{
			if(!TO_PLAY_QUEUE[i])
			{
			TO_PLAY_QUEUE[i] = addition;
			return i;			
			}
		}
	
	printf("Error! No room in que to add sound! \n");
	}
	else
	{
	printf("Error attempting to add sound to que! \n");	
	return -1;
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
			Mix_PlayChannel(-1, TO_PLAY_QUEUE[i], 0);
			TO_PLAY_QUEUE[i] = NULL;
		}
	}
}



void ControlSounds()
{
	
	Play_Sounds();
	
	
}