//Global.c contains all of the functions that will be used throughout the project.

#include <Global.h>



GBFS Assign_GBFS(unsigned int* buf)
{
GBFS t;
t.a = buf[0];
t.b = buf[1];
t.c = buf[2];
t.d = buf[3];
t.e = buf[4];
t.f = buf[5];
t.g = buf[6];	
t.h = buf[7];
return t;
}

/* DPS Functions
First_Null_In_DPS_Array = Returns index number of first array null in DPS.
Create_DPS = This function allocates and initializes an dynamic pointer array. 
DPS_shuffle = Shuffles DPS array so that NULL values are stored at the end. Used for decreasing memory allocation in DPS_reallocate.
DPS_reallocate = Allows for reallocation of a dynamic array. Will sort array if reducing size.
Destroy_DPS = Destroys a DPS. Assusmes all array pointers are to resources that will be completely freed with a single call to free().
DPS_Add_Member = Adds a new member to DPS array. Will resize if neccessary.
DPS_Remove_Member = Removes a member from a DPS array. Frees member in array.

If there is any undefined behavior/crashing then double check these functions.
Remember: Functions pass copies of their arguments, including pointers. Null assignment needs a pointer to the pointer.
*/

int First_Null_In_DPS_Array(DPS* pnt)
{
	if(!pnt)
	{
	printf("Warning. Cannot check for first null in null dps.\b");
	return -1;
	}
	
	for(register unsigned int I = 0; I < pnt->size; I++)
	{
		if(pnt->Array[I] == NULL)
		{
		printf("First Null is %d\n", I);	
		return I;	
		}
	}
	
	printf("Error! Cannot find null in DPS! \n");
	return -1;
} 

DPS* Create_DPS(unsigned int num)
{
	
	DPS* TBR;
	
	TBR = malloc(sizeof(DPS));
	
	if(TBR == NULL)
	{
	printf("Error allocating dynamic pointer structure! \n");		
	return NULL;
	}	
	
	else
	{	
		TBR->Array = malloc(num * sizeof(void*));
		
		if(TBR->Array == NULL)
		{
		printf("Error allocating pointer array! \n");	
		return NULL;
		}	
		
		else
		{	
			TBR->size = num;
			TBR->non_null = 0;
			
			for(unsigned register int I = 0; I < num; I++)
			{
			TBR->Array[I] = NULL;
			}
			
			printf("	Allocated DPS of size %d.\n", TBR->size);	
			
			return TBR;
		}	
	}
}

void DPS_shuffle(DPS* p_target)
{
	if(p_target->non_null == p_target->size)
	{
	printf("Warning! Cannot shuffle full dyanmic pointer array. \n");	
	}
	
	else
	{

		unsigned register int a_stack = 0;
		unsigned register int AI = 0;
		unsigned register int NI = p_target->size - 1;
		
		while(a_stack < p_target->non_null)
		{
			if(p_target->Array[AI] == NULL)
			{
				while(p_target->Array[NI] == NULL)
				{
				NI--;		
				}
					p_target->Array[AI] = p_target->Array[NI];
					p_target->Array[NI] = NULL;	
					}	
		AI++;	
		a_stack++;
		}	
			
	printf("Dyanmic pointer array has been shuffled. %d pointers have been stacked. \n", a_stack);			
	}	
}

void DPS_reallocate(DPS* p_target, unsigned int new_num)
{
	void** p_safety; //Safety pointer to avoid leaking memory on NULL realloc returns.
	
	if(p_target->Array == NULL)
	{
	printf("Warning! Attempt to reallocate null void double pointer! \n");		
	}
	
	else
	{	
		if(new_num < p_target->non_null || new_num == p_target->size)
		{
		printf("Warning! Attempt to reallocate pointer array to size smaller than or equal to number of non-null pointers in array. \n");
		}
	
		else 
		{
			if(new_num < p_target->size) //If new array size is smaller.
			{
			DPS_shuffle(p_target);
			}	
					
			p_safety = realloc(p_target->Array, new_num*sizeof(void*));
			
			if(p_safety == NULL)
			{
			printf("Error! Failed to reallocate pointer array. \n");	
			return;
			}
			
			else
			{
				
				if(new_num > p_target->size) //If new array size is larger.
				{
					for(register int I = p_target->size; I < new_num; I++)
					{
					p_safety[I] = NULL;
					}
				}		
				
			printf("Array of size %d reallocated to size %d. \n", p_target->size, new_num);
				
			p_target->size = new_num;			
			p_target->Array = p_safety;	
			}		
		}
	}	
}

void Destroy_DPS(DPS* p_target)
{
	if(p_target == NULL)
	{
	printf("Warning! Attempt to destroy DPS at null pointer address. \n");	
	}
	
	else
	{	
		for(register int I = 0; I < p_target->size; I++)
		{	
			if(p_target->Array[I])
			{
			free(p_target->Array[I]);			
			}
		}

	free(p_target->Array);
	printf("	DPS of size %d freed. \n", p_target->size);
	free(p_target);
	}
}

int DPS_Add_Member(DPS* target, void* addition, unsigned int possible_expansion)
{
	if(!target)
	{
	printf("Error! Cannot add member to null dps!\n");
	return -1;
	}
	
	int index = First_Null_In_DPS_Array(target);	
	
	if(index == -1) //If there is no free space in dps array.
	{
		if(possible_expansion > 0)
		{
		printf("array getting realocated\n");
		DPS_reallocate(target, (target->size + possible_expansion));
		index = First_Null_In_DPS_Array(target);	
		target->Array[index] = addition;
		target->non_null++;
		return index;
		}
		else
		{
		printf("Could not add member to DPS. No reallocation permission.\n");	
		return -1;
		}
	}
	else
	{
	target->Array[index] = addition;
	target->non_null++;
	return index;	
	}
}

int DPS_Remove_Member(DPS* holder, unsigned int index_tbr)
{
	if(index_tbr >= holder->size)
	{	
	printf("Warning! Attempt to remove DPS member outside of holder range!\n");
	return -1;
	}
		
	else	
	{	
		if(holder->Array[index_tbr] == NULL)
		{
			printf("Error. Attempted to remove NULL DPS member.\n");
			return -1;
		}
	
		free(holder->Array[index_tbr]);
		holder->Array[index_tbr] = NULL;
		holder->non_null--;
		return 0;	
	}
}


//General purpose function for ticking a value.
/* flag: 0 false, 1 true
a = reset holder once it reaches tick cap?
b = subtract cap from holder once cap is reached?
*/
int Tick(int *tick_holder, int tick_num, int tick_cap, char cap_reset, int holder_reset_val)
{
	*tick_holder += tick_num; 
	
	if(*tick_holder >= tick_cap && *tick_holder > 0)
	{
		if(cap_reset == 'Y')
		{
		*tick_holder = holder_reset_val;
		return 1;
		}
		else
		{
		*tick_holder -= tick_cap;	
		return 1;
		}
	}
	else if(*tick_holder <= -tick_cap && *tick_holder < 0)
	{
		if(cap_reset == 'Y')
		{
		*tick_holder = holder_reset_val;
		return -1;
		}
		else
		{
		*tick_holder -= tick_cap;	
		return -1;
		}		
	}
	else 
	{
	return 0;	
	}
}



//This function controls the main loop by capping its frame rate while optimizing time for intensive loading.
void FrameControl() 
{
	static unsigned int previous_time;
	static unsigned int current_time;
	static unsigned int difference_time;
	static unsigned int compensation_time;	//NOTE: SDL ticks even while delaying. Must be compensated for.	
	static unsigned int two_thirds_counter; //Helps more accurately emulate 60 fps.
	
	unsigned int frame_delay = (1000.f/TARGET_FPS); //16 at 60 fps.
	
	if(two_thirds_counter > 2)
	{
	frame_delay++;
	two_thirds_counter -= 3;	
	}
	
	current_time = SDL_GetTicks();
	
	difference_time = (current_time - previous_time - compensation_time);
		
	
	if(difference_time < frame_delay)
	{
	compensation_time = frame_delay - difference_time;	
	SDL_Delay(compensation_time);	
	}
	else
	{
	compensation_time = 0;	
	}
	
	previous_time = current_time;
	two_thirds_counter += 2;
}