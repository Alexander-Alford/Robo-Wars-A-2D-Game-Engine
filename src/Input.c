/**
This file manages all forms of input by the user for this program.
Depending on the direction this project will take, the functions may be improved to include 
imput from mouse and gamepads. Completely customizable controls are an ideal    
goal.
**/
#include "global.h"


SDL_Event INPUT_E;


//The raw array keeps track of every keyboard key's actual state, while the final array is used to  
//provide a possibly modified version of the raw array to the other source files. 
uint16_t KEY_ARR_RAW[TOTAL_KEYS], KEY_ARR_FINAL[TOTAL_KEYS] = {0};

//Sets pressed and released key states to held and standby every core loop cycle. 
void SetContKey()
{
	for(uint16_t i = 0; i < TOTAL_KEYS; i++)
	{
		if(KEY_ARR_RAW[i] == PRESSED)
		{
		KEY_ARR_RAW[i] = HELD;
		}
		else if(KEY_ARR_RAW[i] == RELEASED)
		{
		KEY_ARR_RAW[i] = STANDBY;
		}		
	}	
}

//Switch statement that returns the polled KEYS value.
enum KEYS RetKey(){
	switch(INPUT_E.key.keysym.sym)
	{
	case SDLK_UP:
	return UP;
	case SDLK_DOWN:
	return DOWN;
	case SDLK_LEFT:
	return LEFT;
	case SDLK_RIGHT:
	return RIGHT;
	case SDLK_a:
	return A;
	case SDLK_b:
	return B;
	case SDLK_c:
	return C;
	case SDLK_d:
	return D;
	case SDLK_e:
	return E;
	case SDLK_f:
	return F;	
	case SDLK_g:
	return G;
	case SDLK_h:
	return H;
	case SDLK_i:
	return I;
	case SDLK_j:
	return J;
	case SDLK_k:
	return K;	
	case SDLK_l:
	return L;
	case SDLK_m:
	return M;
	case SDLK_n:
	return N;
	case SDLK_o:
	return O;	
	case SDLK_p:
	return P;	
	case SDLK_q:
	return Q;
	case SDLK_r:
	return R;
	case SDLK_s:
	return S;
	case SDLK_t:
	return T;
	case SDLK_u:
	return U;
	case SDLK_v:
	return V;
	case SDLK_w:
	return W;
	case SDLK_x:
	return X;
	case SDLK_y:
	return Y;	
	case SDLK_z:
	return Z;
	default:
	return UNKNOWN_KEY;		
	}
}

//Core loop master function.
void CoreInput()
{
	SetContKey();

	while (SDL_PollEvent( &INPUT_E ) != 0)
	{
		switch(INPUT_E.type)
		{
			
			case SDL_QUIT: //Window closing.
			coreloop_f = 'F';
			puts("SDL_QUIT event has been initiated");
			break;
		
			case SDL_KEYDOWN: //Key pressed.
				if(InputE.key.repeat == 0)
				{
				KEY_ARR_RAW[RetKey()] = PRESSED;
				}
			break;		
			case SDL_KEYUP: //Key released.
			KEY_ARR_RAW[RetKey()] = RELEASED;
			break;
		
		}
	}
	
	for(uint16_t i = 0; i < TOTAL_KEYS; i++)
	{
		KEY_ARR_FINAL[i] = KEY_ARR_RAW[i];		
	}


	//if(KEY_ARR[P] == PRESSED) //Debug.
	//{
	//Dump_Data_To_Console();
	//}
}