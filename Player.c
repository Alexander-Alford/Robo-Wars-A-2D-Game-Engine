/*

					  
		(left)->Left
		 |			  
Middle-->|
		 |				Grounded--(down)-->Crouching-|-(up)-->Standing
		(right)->Right--|				  			 |		  ^
						Ungrounded--Jump			 Sliding--|
									|
									Fall
*/
#include <Global.h>
#include <Input.h>
#include <Object.h>
#include <C_and_R.h>
#include <Graphics.h>





//iVal
/*
0 = player master state 	left,right,middle; should be used to reduce code redundancy
1 = x velocity sub pixels
2 = y velocity sub pixels
3 = walk speed 
12 = walk tick
4 = slide speed
5 = slide timer
6 = gravity counter
7 = gravity tick
8 = gravity reset
9 = max fall speed
10 = jump impulse
11 = walk frame
*/

//flg 0
/*
# =     0/1
a = left/right
b = grounded/in air
c = walking/not
d = in jump/not
e = crouching/not
f = slide/not
g =
h = 
*/


void Decide_Player_State(idat* self)
{
	//Set flags.
	if(self->cb[0].col.c == 1) //hit ground.
	{
		self->fl[0].b = 0;
		self->fl[0].d = 1;
		self->cb[0].vb.y_vel = 1; //Temporary fix.
		self->iv[6] = 0; //reset gravity counter.
	}
	else
	{
		self->fl[0].b = 1;
	}
	
	if(self->cb[0].col.d == 1) //hit ceiling.
	{
		self->fl[0].d = 1;
		self->cb[0].vb.y_vel = 0; //Reset y velocity.
	}
	
	if(self->cb[0].vb.y_vel >= 0) //End of jump.
	{
	self->fl[0].d = 1;
	}
	
	
	
	
	
	
	
	
	
	if(F_KeyState[RIGHT] == PRESSED || F_KeyState[RIGHT] == HELD)
	{
		self->fl[0].a = 1;
		self->cb[0].vb.x_vel = self->iv[3];
		self->tb[0].flip = 0; //Sprite flip.
	}	
	else if(F_KeyState[LEFT] == PRESSED || F_KeyState[LEFT] == HELD)
	{
		self->fl[0].a = 0;
		self->cb[0].vb.x_vel = -self->iv[3];	
		self->tb[0].flip = 1;	//Sprite flip.	
	}
	else
	{
	self->cb[0].vb.x_vel = 0;
	
	}
	
	
	
	
	if(self->fl[0].b == 1) //If in air.
	{			
			if(self->cb[0].vb.y_vel < self->iv[9]) //Gravity ticks if not past max y_val.
			{
				self->cb[0].vb.y_vel += Tick(&self->iv[6], self->iv[7], self->iv[8], 'N', 0);				
			}
			
		if(self->fl[0].d == 0) //Early jump end.
		{
			if(F_KeyState[X] == RELEASED || F_KeyState[X] == STANDBY)
			{
			self->cb[0].vb.y_vel = 0;
			self->fl[0].d = 1;	
			}
		}			
						
			
	}
	else //If grounded.
	{
		if(F_KeyState[X] == PRESSED) //Jump.
		{
			self->cb[0].vb.y_vel = -self->iv[10];
			self->fl[0].b = 1;
			self->fl[0].d = 0;	
		}
	}
	


//Set up graphics based on flags.

self->tb[0].dest.x = self->cb[0].vb.box.x - 3;
self->tb[0].dest.y = self->cb[0].vb.box.y - 1;


	if(F_KeyState[RIGHT] == HELD || F_KeyState[LEFT] == HELD)
	{
		if(self->fl[0].b == 0) //grounded
		{
		self->iv[11]++;
	
		int TEST = (self->iv[11]/12);
	
			for(register int I = 0; I < TEST; I++)
			{
			self->tb[0].src.x = self->tb[0].src.w*(I + 1);
			}	
		
			//Very messy, poorly fumctional.
			if(TEST > 3)
			{
			self->iv[11] = 0;	
			}
			else if(TEST == 1)
			{
			self->tb[0].dest.x -= 2;	
			}		
			else if(TEST == 2)
			{
			self->tb[0].dest.x -= 2;	
			}
			else if(TEST == 3)
			{
			self->tb[0].dest.x -= 2;		
			}	
		}

						
	}
	else //Default standing sprite.
	{
		self->iv[11] = 0;
		self->tb[0].src.x = 0;
	}

	if(self->fl[0].b == 1) //in air sprite
	{
	self->tb[0].src.x = self->tb[0].src.w*5;	
	}



//Debug_VBox_Mover_two(&self->cb[0].vb, 5);



Position_Screen(&self->cb[0].vb.box); //Center screen.

}





	
