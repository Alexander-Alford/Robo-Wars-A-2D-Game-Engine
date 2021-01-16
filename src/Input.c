/**
This file manages all forms of input by the user for this program.
Depending on the direction this project will take, the functions may be improved to include 
imput from mouse and gamepads. Ultimately, completely customizable controls are an ideal    
goal, although I'm not quite sure they will be necessary or used at all.
**/
#include <SDL.h>
#include <Global.h>


SDL_Event Input_E;





//Enumerators for key states and keyboard keys. Quite useful for not getting confused when coding.
enum KEY_STATES
{
STANDBY,
PRESSED,
HELD,
RELEASED
};

enum KEYS
{
UNKNOWN,
UP,
DOWN,
LEFT,
RIGHT,
A,
B,
C,
D,
E,
F,
G,
H,
I,
J,
K,
L,
M,
N,
O,
P,
Q,
R,
S,
T,
U,
V,
W,
X,
Y,
Z,
TOTAL_KEYS
};

//Array that keeps track of every keyboard key's state. Gets updated first in the core loop.
char F_KeyState[TOTAL_KEYS] = {0};

//Sets pressed and released key states to held and standby every core loop cycle. 
void Set_Cont_Key()
{
	
	//Register int is more optimized for counting.
	for(register int i = 0; i < TOTAL_KEYS; i++)
	{
		
		if(F_KeyState[i] == PRESSED)
		{
		F_KeyState[i] = HELD;
		}

		else if(F_KeyState[i] == RELEASED)
		{
		F_KeyState[i] = STANDBY;
		}
		
			
	}	
}







//A simple switch statement that returns the KEYS value of the key that was polled.
enum KEYS KeyCASE()
{
//Enumerated variable that will be returned. Set to unknown by default.
enum KEYS Returned_key = UNKNOWN;

	switch(Input_E.key.keysym.sym)
	{
	case SDLK_UP:
	Returned_key = UP;
	break;
	case SDLK_DOWN:
	Returned_key = DOWN;
	break;
	case SDLK_LEFT:
	Returned_key = LEFT;
	break;
	case SDLK_RIGHT:
	Returned_key = RIGHT;
	break;
	case SDLK_a:
	Returned_key = A;
	break;
	case SDLK_b:
	Returned_key = B;
	break;
	case SDLK_c:
	Returned_key = C;
	break;
	case SDLK_d:
	Returned_key = D;
	break;
	case SDLK_e:
	Returned_key = E;
	break;
	case SDLK_f:
	Returned_key = F;
	break;	
	case SDLK_g:
	Returned_key = G;
	break;	
	case SDLK_h:
	Returned_key = H;
	break;
	case SDLK_i:
	Returned_key = I;
	break;	
	case SDLK_j:
	Returned_key = J;
	break;	
	case SDLK_k:
	Returned_key = K;
	break;	
	case SDLK_l:
	Returned_key = L;
	break;
	case SDLK_m:
	Returned_key = M;
	break;
	case SDLK_n:
	Returned_key = N;
	break;	
	case SDLK_o:
	Returned_key = O;
	break;	
	case SDLK_p:
	Returned_key = P;
	break;	
	case SDLK_q:
	Returned_key = Q;
	break;
	case SDLK_r:
	Returned_key = R;
	break;
	case SDLK_s:
	Returned_key = S;
	break;	
	case SDLK_t:
	Returned_key = T;
	break;	
	case SDLK_u:
	Returned_key = U;
	break;	
	case SDLK_v:
	Returned_key = V;
	break;
	case SDLK_w:
	Returned_key = W;
	break;
	case SDLK_x:
	Returned_key = X;
	break;	
	case SDLK_y:
	Returned_key = Y;
	break;	
	case SDLK_z:
	Returned_key = Z;
	break;	
	
	}

	return Returned_key;
}

//Core loop master function.
void TakeInputs()
{
	
	//Sets keys as held/unpressed.
	Set_Cont_Key();
	
	
	
	//Handle events on poll while there are any.
	while (SDL_PollEvent( &Input_E ) != 0)
	{
		switch(Input_E.type)
		{
			//Exit Game if an SDL_QUIT event occurs by setting Core_Loop flag to "F".
			case SDL_QUIT:
			F_Core_Loop = 'F';
			printf("SDL_QUIT event has been initiated\n");
			break;
		
			//Switch for when a key is pressed downwards.
			case SDL_KEYDOWN:
				if(Input_E.key.repeat == 0)
				{
				F_KeyState[KeyCASE()] = PRESSED;
				}
			break;
		
			//Switch for when a key is released.
			case SDL_KEYUP:
			F_KeyState[KeyCASE()] = RELEASED;
			break;
		
		}
	}
	
		
	
	
};