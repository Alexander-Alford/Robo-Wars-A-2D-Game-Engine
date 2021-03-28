#include "global.h"


struct EnemyDat{
	SDL_Texture* textPnt;
	
	uint16_t inst;
	
};







/*special
0 = x_vel pixels
1 = y_vel pixels
2 = x_vel subpixels
3 = y_vel subpixels
*/

/* iv
0 = x_sub_ticks
1 = y_sub_ticks
*/
/*
void Bouncer_Run(idat* self)
{

	//Reverse directions on collision.
	if(self->cb[0].col.c == 1 || self->cb[0].col.d == 1) //Vertical reversal.
	{
	self->special[1] = -self->special[1];
	self->special[3] = -self->special[3];
	self->iv[1] = -self->iv[1];
	Add_Sound_To_Play_Queue( ((S_Bind*)((Object*)self->parent_o)->p_S)->p_sounds[0]);
	}
	if(self->cb[0].col.a == 1 || self->cb[0].col.b == 1) //Horizontal reversal.
	{
	self->special[0] = -self->special[0];
	self->special[2] = -self->special[2];
	self->iv[0] = -self->iv[0];
	Add_Sound_To_Play_Queue( ((S_Bind*)((Object*)self->parent_o)->p_S)->p_sounds[0]);
	}
	
	

//Set velocity.
self->cb[0].vb.x_vel = self->special[0] + Tick(&self->iv[0], self->special[2], 100, 'N', 0);
self->cb[0].vb.y_vel = self->special[1] + Tick(&self->iv[1], self->special[3], 100, 'N', 0);

self->tb[0].dest.x = self->cb[0].vb.box.x;
self->tb[0].dest.y = self->cb[0].vb.box.y;
	
}
*/