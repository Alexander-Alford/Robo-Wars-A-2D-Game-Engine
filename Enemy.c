#include <Global.h>
#include <Object.h>
#include <C_and_R.h>
#include <Graphics.h>


/*Flags[0]
a = 
b = 
c = 
*/

/*special
0 = x_vel pixels
1 = y_vel pixels
2 = x_vel subpixels
3 = y_vel subpixels
*/

/* iv
0 = x_buffer
1 = y_buffer
2 = x_sub_buffer
3 = y_sub_buffer
*/

void Bouncer_Run(idat* self)
{
	self->iv[0] = 0;
	self->iv[1] = 0;
	
	//Reverse directions on collision.
	if(self->cb[0].col.c == 1 || self->cb[0].col.d == 1) //Vertical reversal.
	{
	self->special[1] = -self->special[1];
	self->special[3] = -self->special[3];
	self->iv[3] = -self->iv[3];
	}
	if(self->cb[0].col.a == 1 || self->cb[0].col.b == 1) //Horizontal reversal.
	{
	self->special[0] = -self->special[0];
	self->special[2] = -self->special[2];
	self->iv[2] = -self->iv[2];
	}


	self->iv[0] = self->special[0];
	self->iv[1] = self->special[1];
	
	self->iv[2] += self->special[2];
	self->iv[3] += self->special[3];


	if(self->iv[2] > 100 ) //Increment x_buffer, decrease x_sub_buffer.
	{
		self->iv[2] -= 100;
		self->iv[0]++;
	}
	else if(self->iv[2] < -100)
	{
		self->iv[2] += 100;
		self->iv[0]--;
	}
	
	if(self->iv[3] > 100 ) //Increment y_buffer, decrease y_sub_buffer.
	{
		self->iv[3] -= 100;
		self->iv[1]++;		
	}
	else if(self->iv[3] < -100)
	{
		self->iv[3] += 100;
		self->iv[1]--;
	}

//Set velocity.
self->cb[0].vb.x_vel = self->special[0] + self->iv[0];
self->cb[0].vb.y_vel = self->special[1] + self->iv[1];

self->tb[0].dest.x = self->cb[0].vb.box.x;
self->tb[0].dest.y = self->cb[0].vb.box.y;
	
}