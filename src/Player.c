
#include "global.h"


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



struct Player Bijec = 
{
.text = NULL,
.vb.x_v = 0,
.vb.y_v = 0,
.vb.box = {128,128,15,20},
.HP = 10
};

void SetPlayer(uint32_t x, uint32_t y, uint8_t w, uint8_t h)
{
	
}



	
