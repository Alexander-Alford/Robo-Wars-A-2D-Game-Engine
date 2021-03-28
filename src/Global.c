/*
This file contains global function data.
*/
#include "global.h"


/*
0 = startup
1 = title screen
2 = standardgame
3 = in between levels
*/
uint8_t GAME_STATE = 0;






/* 
General purpose function for ticking a value.
0 = reset holder once it reaches tick cap?
1 = subtract cap from holder once cap is reached?

*/
int Tick(int *holder, int tick, int cap, char reset, int resetVal)
{

	*holder += tick; 
	
	if(*holder >= cap && *holder > 0)
	{
		if(reset == 'Y')
		{
		*holder = resetVal;
		return 1;
		}
		else
		{
		*holder -= cap;	
		return 1;
		}
	}
	else if(*holder <= -cap && *holder < 0)
	{
		if(reset == 'Y')
		{
		*holder = resetVal;
		return -1;
		}
		else
		{
		*holder -= cap;	
		return -1;
		}		
	}
	else 
	{
	return 0;	
	}
}



/*
This function controls the main loop by capping its frame rate while optimizing time for intensive loading.
*/
void FrameControl() 
{
	static unsigned int previous_time;
	static unsigned int current_time;
	static unsigned int difference_time;
	static unsigned int compensation_time;	//NOTE: SDL ticks even while delaying. Must be compensated for.	
	static unsigned int two_thirds_counter; //Helps more accurately emulate 60 fps. Should be revised so any fps can be used.
	
	unsigned int frame_delay = (1000.f/TARGET_FPS); //The rounded amount of milliseconds between each frame. 16 at 60 fps.
	
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