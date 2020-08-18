//Code that is no longer useful or used belongs here. 


//This file will NOT be compiled.


//////////////////////Startup.c////////////////////////////
	//The screen that will be rendered to.
	SDL_Surface* Screensurface = NULL;
	
	
	//Get window Surface.
	Screensurface = SDL_GetWindowSurface(WINDOW);
	
		
	//Load the image to Image_Surface.
	Image_Surface = Load_Surface("Test.png");
	
	//Apply the image loaded to the image surface to the screen surface that renders to the window.
	SDL_BlitSurface( Image_Surface, NULL, Screensurface, NULL );
		
	//Deallocate the surface and texture specified in Graphics.h.
	SDL_FreeSurface(Image_Surface);
	Image_Surface = NULL;	
	
//////////////////////Graphics.c + .h////////////////////////////

int SCREEN_WIDTH = 256;
int SCREEN_HEIGHT = 256;
int SCREEN_X = 0;
int SCREEN_Y = 0;

	//Pointer to Temporary surface that will be used to draw to.
	SDL_Surface* Image_Surface = NULL;
	
	//Update the surface.
	SDL_UpdateWindowSurface(WINDOW);
	
	SDL_Surface* Image_Surface;

	typedef struct {SDL_Texture* c_Texture; int c_Height; int c_Width;} Bound_Texture;

	Bound_Texture* Create_BTexture(const char* PATH);
	void Destroy_BTexture(Bound_Texture* ToBeDestroyed);

Bound_Texture* TestTexture;
	
T_Bind* Create_T_Bind(const char* PATH, SDL_RendererFlip c_flip, unsigned int Layer)
{
	T_Bind* p_return_t_bind = malloc(sizeof(T_Bind));
	
	if (p_return_t_bind == NULL)
	{
	printf("Error. Texture Bind could not be allocated. \n");
	}
	
	else
	{
	p_return_t_bind->F_flip = c_flip;
	p_return_t_bind->layer = Layer;
	p_return_t_bind->source_rect = NULL;
	p_return_t_bind->destin_rect = NULL;	
	
	if(PATH != NULL)
	{	
	p_return_t_bind->p_texture = Load_Texture(PATH);
	}
	
	
	return p_return_t_bind;
	}


	
}	
	
void T_Bind_Rect_Switch(T_Bind* p_self, SDL_Rect* u_source, SDL_Rect* u_destin)
{
	p_self->source_rect = u_source;
	p_self->destin_rect = u_destin;
}	

void T_Bind_Render(T_Bind* p_self)
{
	SDL_RenderCopyEx( RENDERER, p_self->p_texture, p_self->source_rect, p_self->destin_rect, 0, NULL, p_self->F_flip );
}
	
void Destroy_T_Bind(T_Bind* p_target)
{
	if (p_target->p_texture != NULL)
	{
	SDL_DestroyTexture(p_target->p_texture);
	p_target->p_texture = NULL;	
	TEXTURE_COUNT--;
	}
	
	
	free(p_target);
	p_target = NULL;
}
	
//Dependence chain for debugging.
void Render_Instance(idat* target, unsigned int tb_num, SDL_Texture* text)
{
	if(target != NULL)
	{	
		for(unsigned register int I = 0; I < tb_num; I++)
		{
			if(target->tb[I].flg.a == 1)
			{	
			SDL_Rect D = target->tb[I].dest;
			D.x -= SCREEN.x;
			D.y -= SCREEN.y;
			
				if(Is_On_Screen(&target->tb[I].dest).a == 1)
				{
				SDL_RenderCopyEx(RENDERER, text, &target->tb[I].src, &D, 0.0, NULL, target->tb[I].flip);
				}
			}	
		}
	}
	else
	{
	printf("Error. instance null \n");	
	}		
}

void Render_Object(Object* obj)
{
	if(obj != NULL && obj->dps_ins != NULL)
	{
		for(unsigned register int I = 0; I < obj->dps_ins->size; I++)
		{
			if(obj->dps_ins->Array[I] != NULL)
			{	
			Render_Instance(obj->dps_ins->Array[I], obj->size[1], ((T_Bind*)obj->p_T)->p_texture);
			}
		}
		
	}
	else
	{
	printf("Error. object or its instance dps null \n");		
	}
}

void Render_O_Array(DPS* o_dps)
{
	if(o_dps != NULL)
	{
		for(unsigned register int I = 0; I < o_dps->size; I++)
		{
			if(o_dps->Array[I] != NULL)
			{
			Render_Object(o_dps->Array[I]);	
			}	
		}
	
	}
	else
	{
	printf("Error. object array is null \n");		
	}
}
	
//This structure will make handling textures easier by including image size along with the texture.
typedef struct 
{
	
	SDL_Texture* c_Texture;
	
	int c_Height;
	int c_Width;
	
	
} Bound_Texture;
	
	
void Initiate_BTexture(Bound_Texture* p_self, const char* PATH)
{
	
	SDL_Surface* Temporary_Surface = NULL;
	SDL_Texture* Return_texture = NULL;
	
	Temporary_Surface = Load_Surface(PATH);
	
	p_self -> c_Height = Temporary_Surface -> h;
	p_self -> c_Width = Temporary_Surface -> w;
	
	Return_texture = SDL_CreateTextureFromSurface(RENDERER, Temporary_Surface);
	
	SDL_FreeSurface(Temporary_Surface);
	
	p_self -> c_Texture = Return_texture;
	
	
}

Bound_Texture* Create_BTexture(const char* PATH)
{
	
	Bound_Texture* p_Bound_Texture = malloc(sizeof(Bound_Texture));
	
	Initiate_BTexture(p_Bound_Texture, PATH);
	
	TEXTURE_COUNT++;
	printf("Texture created. \n");
	
	return p_Bound_Texture;
}

void Destroy_BTexture(Bound_Texture* ToBeDestroyed)
{
	
	SDL_DestroyTexture(ToBeDestroyed->c_Texture);
	
	ToBeDestroyed->c_Height = 0;
	ToBeDestroyed->c_Width = 0;
	
	TEXTURE_COUNT--;
	printf("Texture destroyed. \n");
	free(ToBeDestroyed);
	
}

void Render_BTexture(Bound_Texture* RenderTarget, int X_POS, int Y_POS)
{
	
	SDL_Rect RenderQuad = {X_POS, Y_POS, RenderTarget->c_Width, RenderTarget->c_Height};
	SDL_RenderCopy(RENDERER, RenderTarget->c_Texture, NULL, &RenderQuad);
	
}

void T_Bind_Update(T_Bind* p_self, int u_frame, int u_s_x, int u_s_y, int u_s_w, int u_s_h, int u_d_x, int u_d_y, int u_d_w, int u_d_h)
{
	p_self->Frame = u_frame;	
	
	p_self->source_rect->x = u_s_x;
	p_self->source_rect->y = u_s_y;
	p_self->source_rect->w = u_s_w;
	p_self->source_rect->h = u_s_h;
	
	p_self->destin_rect->x = u_d_x;
	p_self->destin_rect->y = u_d_y;
	p_self->destin_rect->w = u_d_w;
	p_self->destin_rect->h = u_d_h;	
}
	
//////////////////////Input.c///////////////////////////////////////////////////////////////////////////////////////////////////////

	printf("The %d key has been pressed down.\n", KeyCASE());
	printf( "The %d key is in the %d state.\n", KeyCASE(),F_KeyState[KeyCASE()]);
	printf("The %d has been released.\n", KeyCASE());
	printf( "The %d key is in the %d state.\n", KeyCASE(),F_KeyState[KeyCASE()]);
	
/////////////////////Grid.c//////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
This will be the dynamically allocated 2D array of tiles that will be generated and filled for each room.
This array will be arranged in the format Vertical member [V] by Horizontal member [H]
The array's dimensions will always be the map size dimensions multiplied by the quadrant size.
----------------------------------------------------------------------------------------------------------------------------------------------
	0	1	2	H
	
0	[1][2][3]
1	[4][5][6] <----- Value 6 is in the array position [1][2]
2	[7][8][9]
	
V
----------------------------------------------------------------------------------------------------------------------------------------------
Array name:						Vertical:				Horizontal:
pointer to pointer to tile		pointer to tile			tile structures
---------------------|			|---|				 	|---------|
pp_PRIMARY_TILE_ARRAY|--------->|[0]|------------------>|[1][2][3]|<----These are the individual blocks of data for each tile structure.
---------------------|			|[1]|					|---------|
Holds an address to a block		|[2]|
of addresses that each hold 	|[3]|
a block of tile structures.		|---|
								  ^
								  |
								  |
								  Each member of this block holds the address to another block of Tile structures.
----------------------------------------------------------------------------------------------------------------------------------------------
**/

//(In map scanning in ReadMapData function)Bizarre bug where a scanned value of A-F is read as hexidecimal. Fixed by using integer scan.

	while ( V < VERTICAL_LEVEL_TILES)
		{
			
			
		while ( H < HORIZONTAL_LEVEL_TILES)	
			{
			//Creates and initiates the tile's base object while assigning it to the pointer.
			pp_PRIMARY_TILE_ARRAY[V][H].Base_O = Create_Object((H*BASE_TILE_SIZE),(V*BASE_TILE_SIZE),BASE_TILE_SIZE ,BASE_TILE_SIZE);
				
			//Increments the horizontal counter.
			H++;
			}
		//Resets horizontal counter.
		H = 0;
		//Increments the vertical counter.
		V++;
		}
}

		//For each pointer in the array allocate the number of horizontal tiles as an array of objects. 
		while ( V < VERTICAL_LEVEL_TILES )
		{
		pp_PRIMARY_TILE_ARRAY[V] = malloc(HORIZONTAL_LEVEL_TILES * sizeof(Object));
		V++;
		}
	
		//Checks if second allocation was successful. V is deincremented by one to account for its final incrementation before escaping the while loop.
		if (pp_PRIMARY_TILE_ARRAY[--V] == NULL)
		{
		printf("Error allocating memory for tiles. \n");
		}
		
	((Tile*)pp_PRIMARY_TILE_ARRAY[I]->p_special->p_tile)->TileCode_G = transfer;	
	
		int C = 0;

	for (register int V = 0; V < VERTICAL_LEVEL_TILES; V++)
	{
		for (register int H = 0; H < HORIZONTAL_LEVEL_TILES; H++)
		{
		C = H + (V*HORIZONTAL_LEVEL_TILES);
		pp_PRIMARY_TILE_ARRAY[C] = Create_Object(0, 0);
		Initiate_Object(pp_PRIMARY_TILE_ARRAY[C], BASE_TILE_SIZE * H, BASE_TILE_SIZE * V, (void**) All_Box_Arr(1), NULL, All_FSPS_t());
		}
	
	}
	
Tile* Create_Tile(char BP, SDL_RendererFlip F_tf, unsigned int TC_G, unsigned int CC)
{
	Tile* ret = malloc(sizeof(Tile));
	
	ret->BlockPropety = BP;
	ret->tF_flip = F_tf;
	ret->TileCode_G = TC_G;
	ret->CollisionCode = CC;
	
return ret;	
}	

		do
		{
		
			do
			{
			
			
			TILE_TEXTURE_ARRAY[I].x = X*BASE_TILE_SIZE;
			TILE_TEXTURE_ARRAY[I].y = Y*BASE_TILE_SIZE;
			TILE_TEXTURE_ARRAY[I].w = BASE_TILE_SIZE;
			TILE_TEXTURE_ARRAY[I].h = BASE_TILE_SIZE;
			X++;
			I++;
			} while(X<H);
		
		X=0;
		Y++;
	
		} while(Y<V);

	
/////////////////////////Object.c/////////////////////////////////////////////////////////////////////////////////////////////////////

/**
Almost every kind of game and a large array of programs depend on an object-orientated style of function for their organization.
To correctly implement and use an object orientated style of programming in c, it is neccessary to take full advantage of that style of programming.
In this game objects will be used to dynamically allocate and deallocate data of ingame "objects" and organize their properties.
Polymorphism will need to be used.

	
Object(position/ID) -------\o_table\-------->Hit Boxes container(position, dimensions, collision)              
									\
									 \------->Sound container(sound, conditions to play, effects)
									  \
									   \-------->Texture container(texture pointer, position and render size, frame counter, special effects)						
										\
										 \-------->Action coordinator(AI, collision for hit boxes, playing/altering sounds, displaying textures/FX, input interpretation)
										  \
										   \--------->Special properties Set (FSPS)(tick values, properties neccessary for AI)
**/

	if (A->x > (B->x + B->w / 2)) //Overlap right of B center.
	{
		if (A->y + A->h < B->y + B->h / 2) //Overlap above B center
		{
			if(A_over_B_right < A_over_B_top)
			{
				Resolve_Right(A, B);
			}	
			else
			{
				Resolve_Top(A, B);
			}	
		
		}
		
		else //A_over_B_bottom > 0. Overlap below B center.
		{
			if(A_over_B_right < A_over_B_bottom)
			{
				Resolve_Right(A, B);
			}
			else
			{
				Resolve_Bottom(A, B);
			}
		}	
		
	}
	
	else //A->x + A->w < B->x + B->w / 2. Overlap left of B center.
	{
		
		if (A->y + A->h < B->y + B->h / 2) //Overlap above B center
		{
			if(A_over_B_left < A_over_B_top)
			{
				Resolve_Left(A, B);
			}	
			else
			{
				Resolve_Top(A, B);
			}	
		
		}
		
		else //A_over_B_bottom > 0. Overlap below B center.
		{
			if(A_over_B_left < A_over_B_bottom)
			{
				Resolve_Left(A, B);
			}
			else
			{
				Resolve_Bottom(A, B);
			}
		}	
	}		
	
//Calculates box A's overlap of box B from all four sides. Negative and zero values indicate no overlap.
int A_over_B_right = ((B->x + B->w) - A->x); 
int A_over_B_left = ((A->x + A->w) - B->x);
int A_over_B_top = ((A->y + A->h) - B->y);
int A_over_B_bottom = ((B->y + B->h) - A->y);

	switch (F_H)
	{
	case 'R':
	Resolve_Left(&V->box, B);
	printf("res L \n");
	break;
	case 'L':
	Resolve_Right(&V->box, B);
	printf("res R \n");
	break;
	default:
	break;
	}
	
	switch (F_V)
	{
	case 'U':
	Resolve_Bottom(&V->box, B);
	printf("res B \n");
	break;
	case 'D':
	Resolve_Top(&V->box, B);
	printf("res T \n");
	break;
	default:
	break;
	}

//These collision resolution functions are in desperate need of a rewrite.
void Resolve_Collision(Box* A, Box* B)
{
//Flags for how the resolution should go. H is horizontal flag, v is vertical flag. N = No resolution, R/L = Right/Left, and T/B = Top/Bottom.
char F_over_h = 'N';
char F_over_v = 'N';

//Arg_One is the horizontal overlap from a given direction. Arg_Two is the vertical overlap from a given direction.
int Arg_One = 0;
int Arg_Two = 3;

//If A is right or left of B center, can not run at all.
if (A->x > (B->x + B->w / 2)) 
	{
	F_over_h = 	'R';
	Arg_One = ((B->x + B->w) - A->x);
	}
else //if (A->x + A->w < (B->x + B->w / 2))
	{
	F_over_h = 'L';	
	Arg_One = ((A->x + A->w) - B->x);
	}	

//If A is is above or below B center, can not run at all.
if (A->y + A->h < (B->y + B->h / 2))
	{
	F_over_v = 'T';
	Arg_Two = ((A->y + A->h) - B->y);
	}
else //if (A->y > (B->y + B->h / 2))
	{
	F_over_v = 'B';
	Arg_Two = ((B->y + B->h) - A->y);
	}

//Resolve horizontal if true.
if (Arg_One < Arg_Two)
{
	switch (F_over_h)
	{
	case 'R':
	Resolve_Right(A, B);
	break;
	
	case 'L':
	Resolve_Left(A, B);
	break;
	
	default:
	//printf("def 1");
	break;
	}
}
//Resolve vertical otherwise.
else
{
	switch (F_over_v)
	{
	case 'T':
	Resolve_Top(A, B);
	break;
	
	case 'B':
	Resolve_Bottom(A, B);
	break;
	
	default:
	//printf("def 2");
	break;
	}
}	


}


	//(y_considered - B->box.y) >= (x_considered - B->box.x)*B->m + B->v_off + B->box.y
	y_considered >= ((x_considered)*B->m + B->v_off)	
		
void Move_Box(Box* p_self)
{

	if (F_KeyState[1] == 1 || F_KeyState[1] == 2)
	{
	p_self->y -= 3;
	}

	if (F_KeyState[2] == 1 || F_KeyState[2] == 2)
	{
	p_self->y += 3;
	}

	if (F_KeyState[3] == 1 || F_KeyState[3] == 2)
	{
	p_self->x -= 3;
	}
	
	if (F_KeyState[4] == 1 || F_KeyState[4] == 2)
	{
	p_self->x += 3;
	}
	
}		
		
//First Special Properties Set
typedef struct
{
	void* p_tile;	
} FSPS;

typedef struct 
{
	int ID;	
	signed int x_pos; 
	signed int y_pos;
	void** pp_boxes;
	void** pp_tbinds;	
	FSPS* p_special;
} Object;




void Initiate_Object(Object* p_self, signed int X_POS, signed int Y_POS, void** i_boxes, void** i_tbinds, FSPS* i_special)
{
	p_self->x_pos = X_POS;
	p_self->y_pos = Y_POS;
	p_self->ID = OBJECT_COUNT;
	p_self->pp_boxes = i_boxes;
	p_self->pp_tbinds = i_tbinds;
	p_self->p_special = i_special;
}

Object* Create_Object(signed int X_POS, signed int Y_POS)
{
	Object* p_Object = malloc(sizeof(Object));
	
	Initiate_Object(p_Object, X_POS, Y_POS, NULL, NULL, NULL);
	OBJECT_COUNT++;
	
return p_Object;
}

void Destroy_Object(Object* p_ToBeDestroyed)
{
	
		if (p_ToBeDestroyed->pp_boxes != NULL)
		{
		d_free(p_ToBeDestroyed->pp_boxes[0]);
		free(p_ToBeDestroyed->pp_boxes);	
		}
	
		if (p_ToBeDestroyed->pp_tbinds != NULL)
		{
		Destroy_T_Bind(p_ToBeDestroyed->pp_tbinds[0]);	
		free(p_ToBeDestroyed->pp_tbinds);
		}
	
		if (p_ToBeDestroyed->p_special != NULL)
		{
			if( p_ToBeDestroyed->p_special->p_tile != NULL )
			{
			d_free( p_ToBeDestroyed->p_special->p_tile );
			}
			
		free(p_ToBeDestroyed->p_special);
		}
		

	
	free(p_ToBeDestroyed);
	p_ToBeDestroyed = NULL;
	OBJECT_COUNT--;
}


Box** All_Box_Arr(signed int num)
{
	Box** pp_return = malloc(num * sizeof(Box*));
	
	for (register int I = 0; I < num; I++)
	{
	pp_return[I] = Create_Box(0, 0, 0, 0);	
	}

return pp_return;
}

FSPS* All_FSPS_t()
{
FSPS* p_ret = malloc(sizeof(FSPS));
p_ret->p_tile = NULL;
	
return p_ret;	
}


void Box_Update(Box* p_self, int u_x, int u_y, int u_w, int u_h)
{
	p_self->x = u_x;
	p_self->y = u_y;
	p_self->w = u_w;
	p_self->h = u_h;
}

Box* Create_Box(int c_x, int c_y, int c_w, int c_h)
{
	Box* p_return_box = malloc(sizeof(Box));
	if (p_return_box == NULL)
	{
	printf("Error. Box could not be allocated. \n");	
	}
	else
	
	Box_Update(p_return_box, c_x, c_y, c_w, c_h);
	return p_return_box;
}

typedef struct I_link
{
	void* p_data_i;
	
	struct I_link* p_next;	
} I_link;

	if(Detect_Box_Collision(&TEST_V->box, TEST_B) == 'T' && Detect_Box_Slope_Collision(&TEST_V->box, TEST_SLO) == 'T')
	{
	printf("Colliding! \n");	
	
		if(Collision_Type_Box_Sloped_Box(TEST_V, TEST_B, TEST_SLO) != 'S' || Collision_Type_Slope_y(TEST_SLO, TEST_V->x_vel, TEST_V->y_vel, TEST_V->box.x - TEST_V->x_vel, TEST_V->box.y - TEST_V->y_vel, TEST_SLO->y, TEST_SLO->y + 30, TEST_V->box.y ) != 'T')
		{
		General_Rect_Resolver(TEST_V, TEST_B, Collision_Type_Rect(TEST_V, TEST_B));
		}	
		else
		{
		TEST_V->box.y = Resolve_Box_Slope_Vertical(&TEST_V->box, TEST_SLO);	
		}	
	}	


void Move_All_VBoxes(DPS* o_Arr)
{
	if(o_Arr == NULL)
	{
	printf("Error. Null object array. No vboxes to move. \n");
	return;	
	}
	
	unsigned register int A = 0;
	unsigned register int B = 0;
	unsigned register int C = 0;
	unsigned register int D = 0;
	
	while( A < o_Arr->size && B < o_Arr->non_null)
	{
		if(o_Arr->Array[A] != NULL && ((Object*)o_Arr->Array[A])->dps_ins != NULL)
		{
			while(C < ((Object*)o_Arr->Array[A])->dps_ins->size && D < ((Object*)o_Arr->Array[A])->dps_ins->non_null)
			{
				if(((idat*)((Object*)o_Arr->Array[A])->dps_ins->Array[C]) != NULL)
				{
					for(unsigned register int Z = 0; Z < ((Object*)o_Arr->Array[A])->size[0]; Z++)
					{
					Move_VBox(&((idat*)((Object*)o_Arr->Array[A])->dps_ins->Array[C])->hc[Z].vb);	
					}
				D++;		
				}
			C++;
			}
		B++;	
		}	
	A++;
	}	
}
void Set_VBox_Velocity(VBox* p_self, int x_v, int y_v)
{
p_self->x_vel = x_v;
p_self->y_vel = y_v;	
}

i_data* Prepare_Instance_Data(unsigned int input_sizes[4])
{
	i_data* p_new = malloc(sizeof(i_data));
	
	p_new->sizes[0] = input_sizes[0];
	p_new->Flags = malloc(input_sizes[0]*sizeof(GBFS));
	p_new->sizes[1] = input_sizes[1];
	p_new->iValues = malloc(input_sizes[1]*sizeof(int));
	p_new->sizes[2] = input_sizes[2];
	p_new->vBoxes = malloc(input_sizes[2]*sizeof(VBox));
	p_new->sizes[3] = input_sizes[3];
	p_new->rects = malloc(input_sizes[3]*sizeof(SDL_Rect));
	
	return p_new;
}
//////////////////////////////////////////////////////Player.c/////////////////////////////////////////////////////////////////////////////

		if(F_player_crouching == 'F' && F_KeyState[2] == 1)
		{
		F_player_crouching = 'T';
		}	

		else if(F_player_crouching == 'T')
		{

			if(F_KeyState[1] == 1)
			{
			F_player_crouching = 'F';
			//printf("standing left\n");	
			}
		}
	
	
////////////////////////////////////C_and_R.c/////////////////////////////////////////////////////////////////////////////	

GBFS Check_Possible_Tile_Collisions(VBox* self)
{
/*
This will be the returned GBFS that will tell the details of the collision resolution for the frame.

Flag one = Right resolution.
Flag two = Left resolution.
Flag three = Top resolution.
Flag four = Bottom resolution.
Flag five = Corner resolution.
Flag six = one resolution.
Flag seven = two resolutions.
Flag eight = three resolutions.

Flags one/two, three/four, and six/seven/eight are mutually exclusive with only one returning 1(true).
Corner resolution flag is only true when corner is specifically the only resolution for frame and would make flag six the only other true flag.
*/

GBFS F_return;

//These coordinates are normalized to the (0,0) of tile that they are inside.
int normalized_x = self->box.x / BASE_TILE_SIZE;
int normalized_y = self->box.y / BASE_TILE_SIZE;
int normalized_x_p_w = (self->box.x + self->box.w) / BASE_TILE_SIZE;
int normalized_y_p_h = (self->box.y + self->box.h) / BASE_TILE_SIZE;

//These variables tell us the width and height of the tile rectangle we will be checking.
int normalized_width = normalized_x_p_w - normalized_x + 1;
int normalized_height = normalized_y_p_h - normalized_y + 1;	

/*
A flag that checks if two collision resolutions have happened.
Due to the nature of the grid system, only two resolutions, vertical and horizontal, should be 
neccessary to resolve any collision, unless the previous state of the box was already colliding.
Additionally, there are special cases where a third resoution may be needed, such as a corner case
where the VO differences are the same.
*/
int F_two_resolution_check = 0;

//The box structure that will change its position each iteration of the nested loop to mimic the tile's position and dimensions.
//It is assumed at this point in time that the width and height will not change under any circumstances.
Box TileBox = {0,0,BASE_TILE_SIZE,BASE_TILE_SIZE};
Slope TileSlope = {1,'F',0,0,0};

//Variable that will be used to access specific tile array index.
int index = 0;

//Flag for if a corner resolution case is found.
char F_corner_check = 'F';
Box corner_box = {0,0,0,0};

//This is the main double loop that will search each tile the vbox is overlapping and check for collisions.
//Reset index to first value after collision resolution.
for (register int Counter_One = 0; Counter_One < normalized_height; Counter_One++)
{
	
	for (register int Counter_Two = 0; Counter_Two < normalized_width; Counter_Two++)
	{
		//If there were two collision resolutions, exit the function.
		if (F_two_resolution_check == 2)
		{
		goto EXIT;
		}
	
		//Sets the tile index to match the tile being checked. Exits function if unviable.
		index = normalized_x + ((normalized_y + Counter_One)*HORIZONTAL_LEVEL_TILES) + Counter_Two;
		
		if (index < 0 || index > (TOTAL_TILES-1))
		{
		printf("ERROR. Attempted to use out of bound tile index! \n");
		goto EXIT;
		}	
		
		//Checks collision with tilebox if it has the right tilecode.
		if (p_PRIMARY_TILE_ARRAY[index].CollisionCode != 0)
		{
		TileBox.x = (normalized_x + Counter_Two) * BASE_TILE_SIZE;
		TileBox.y = (normalized_y + Counter_One) * BASE_TILE_SIZE;
		
			if (Detect_Box_Collision(&self->box, &TileBox).a == 1)
			{
				switch (Collision_Code_Master_Switch(p_PRIMARY_TILE_ARRAY[index].CollisionCode, self, TileBox))
				{
				case 'L':
				F_two_resolution_check++;
				normalized_x += 1;
				Counter_One = 0;
				Counter_Two = 0;
				printf("R %d\n", index);
				F_return.a = 1;
				break;
				case 'R':
				F_two_resolution_check++;
				normalized_x -= 1;
				Counter_One = 0;
				Counter_Two = 0;
				printf("L %d\n", index);
				F_return.b = 1;
				break;			
				case 'D':
				F_two_resolution_check++;
				normalized_y -= 1;
				Counter_One = 0;
				Counter_Two = 0;
				printf("T %d\n", index);
				F_return.c = 1;
				break;
				case 'U':
				F_two_resolution_check++;
				normalized_y += 1;
				Counter_One = 0;
				Counter_Two = 0;
				printf("B %d\n", index);
				F_return.d = 1;
				break;
				case 'C':
				F_corner_check = 'T';
				corner_box = TileBox;
				printf("C %d\n", index);
				break;			
				}
			}		
		}

	
	
	}	
}

//Extra code that deals with a single collision with a corner. Can deal with corner case that resolves into another collision.
if (F_corner_check == 'T' && F_two_resolution_check == 0)
{
	F_return.e = 1;
	
	//These values will be used to hold the original self box's position.
	int original_x = self->box.x;
	int original_y = self->box.y;
	

	
	if(self->y_vel > 0)
	{
	Resolve_Top(&self->box ,&TileBox);
	normalized_y -= 1;
	
	
		for (register int y = 0; y < normalized_height; y++)
		{
			if (self->x_vel > 0)
			{
			index = normalized_x + ((normalized_y + y)*HORIZONTAL_LEVEL_TILES + normalized_width);
			}
			else
			{
			index = normalized_x + ((normalized_y + y)*HORIZONTAL_LEVEL_TILES);	
			}
			
			if(p_PRIMARY_TILE_ARRAY[index].TileCode_G == 1) 
			{
				TileBox.x = (normalized_x) * BASE_TILE_SIZE;
				TileBox.y = (normalized_y + y) * BASE_TILE_SIZE;
				
				if (Detect_Box_Collision(&self->box, &TileBox).a == 1)
				{
				self->box.y = original_y;
				normalized_y += 1;	
					printf("check 1\n");
					
					if (self->x_vel > 0)
					{
					Resolve_Left(&self->box ,&corner_box);
					normalized_x -= 1;
					printf("check 1.1\n");					
						for( register int x = 0; x < normalized_width; x++)
						{
						index = normalized_x + (normalized_y*HORIZONTAL_LEVEL_TILES) + x;	
							
							if(p_PRIMARY_TILE_ARRAY[index].TileCode_G == 1) 
							{
							TileBox.x = (normalized_x + x) * BASE_TILE_SIZE;
							TileBox.y = normalized_y*BASE_TILE_SIZE;
							
								if (Detect_Box_Collision(&self->box, &TileBox).a == 1)
								{
									Resolve_Top(&self->box ,&corner_box);
									printf("bottom right\n");
									break;
								}
							}
						}
					}
					
					else
					{
					Resolve_Right(&self->box ,&corner_box);
					normalized_x += 1;
					printf("check 1.2\n");
						for( register int x = 0; x < normalized_width; x++)
						{
						index = normalized_x + (normalized_y*HORIZONTAL_LEVEL_TILES) + x;	
							
							if(p_PRIMARY_TILE_ARRAY[index].TileCode_G == 1) 
							{
							TileBox.x = (normalized_x + x) * BASE_TILE_SIZE;
							TileBox.y = normalized_y*BASE_TILE_SIZE;
							
								if (Detect_Box_Collision(&self->box, &TileBox).a == 1)
								{
									Resolve_Top(&self->box ,&corner_box);
									printf("bottom left\n");
									break;
								}
							}
						}
					}	
				}
			}
		}
	}
	
	else
	{
	Resolve_Bottom(&self->box ,&TileBox);	
	normalized_y +=1;
	
		for (register int y = 0; y < normalized_height; y++)
		{
			if (self->x_vel > 0)
			{
			index = normalized_x + ((normalized_y + y)*HORIZONTAL_LEVEL_TILES + normalized_width);
			}
			else
			{
			index = normalized_x + ((normalized_y + y)*HORIZONTAL_LEVEL_TILES);	
			}
		
			if(p_PRIMARY_TILE_ARRAY[index].TileCode_G == 1) 
			{
				TileBox.x = (normalized_x) * BASE_TILE_SIZE;
				TileBox.y = (normalized_y + y) * BASE_TILE_SIZE;
				
				if (Detect_Box_Collision(&self->box, &TileBox).a == 1)
				{
				self->box.y = original_y;
				normalized_y -= 1;
				printf("check 2\n");				
					
					if (self->x_vel > 0)
					{
					Resolve_Left(&self->box ,&corner_box);
					normalized_x -= 1;
					printf("check 2.1\n");
						for( register int x = 0; x < normalized_width; x++)
						{
						index = normalized_x + (normalized_y*HORIZONTAL_LEVEL_TILES) + x;	
							
							if(p_PRIMARY_TILE_ARRAY[index].TileCode_G == 1) 
							{
							TileBox.x = (normalized_x + x) * BASE_TILE_SIZE;
							TileBox.y = normalized_y*BASE_TILE_SIZE;
							
								if (Detect_Box_Collision(&self->box, &TileBox).a == 1)
								{
									Resolve_Bottom(&self->box ,&corner_box);
									printf("top right\n");
									break;
								}
							}
						}
					}
					
					else
					{
					Resolve_Right(&self->box ,&corner_box);
					normalized_x += 1;
					printf("check 2.2\n");
						for( register int x = 0; x < normalized_width; x++)
						{
						index = normalized_x + (normalized_y*HORIZONTAL_LEVEL_TILES) + x;	
							
							if(p_PRIMARY_TILE_ARRAY[index].TileCode_G == 1) 
							{
							TileBox.x = (normalized_x + x) * BASE_TILE_SIZE;
							TileBox.y = normalized_y*BASE_TILE_SIZE;
							
								if (Detect_Box_Collision(&self->box, &TileBox).a == 1)
								{
									Resolve_Bottom(&self->box ,&corner_box);
									printf("top left\n");
									break;
								}
							}
						}
					}	
				}
			}
		}
	
	
	
	}	



}	



EXIT:
return F_return;
}

char Col_Sloped_Box(VBox* V, Box* B, Slope* C)
{
	//This function assumes that we already know that the sloped box has been collided with.
//corner can take four values. Bottom right(00), bottom left(01), top right(02), top left(03).	
//Whether vertical velocity will contribute to horizontal displacement along the slope is still being considered.	
//Should ramps be sticky?

//First we need to determine if the point of interest can collide with the slope.
int F_can_collide;

int x_vel_term;
int y_vel_term;

//If slope greater than 1, multiply x term. if less than one, multiply y term

	//Checks if current VBox velocity allows for slope collision.
	switch (C->F_corner)
	{
	case 00:
	x_vel_term = -V->x_vel;
	y_vel_term = V->y_vel;
	break;
	
	case 01:
	x_vel_term = -V->x_vel;
	y_vel_term = V->y_vel;
	break;
	
	case 02:
	x_vel_term = V->x_vel;
	y_vel_term = -V->y_vel;
	break;
	
	case 03:
	x_vel_term = V->x_vel;
	y_vel_term = -V->y_vel;
	break;
	}



if (C->F_div == 'T')
{
F_can_collide = (C->m*y_vel_term) + x_vel_term;	
}
else
{
F_can_collide = (C->m*x_vel_term) + y_vel_term;		
}	
	

	
if (F_can_collide <= 0 )	
{
return Collision_Type_Rect(V, B);
}	
else	
{
	
	
	

	
return 'S';	
}	
	
}

char Col_Sloped_Box(VBox* V, Box* B, Slope* C)
{
	//This function assumes that we already know that the sloped box has been collided with.
//corner can take four values. Bottom right(00), bottom left(01), top right(02), top left(03).	
//Whether vertical velocity will contribute to horizontal displacement along the slope is still being considered.	
//Should ramps be sticky?

//First we need to determine if the point of interest can collide with the slope.
int F_can_collide;

int x_vel_term;
int y_vel_term;

//If slope greater than 1, multiply x term. if less than one, multiply y term

	//Checks if current VBox velocity allows for slope collision.
	switch (C->F_corner)
	{
	case 00:
	x_vel_term = -V->x_vel;
	y_vel_term = V->y_vel;
	break;
	
	case 01:
	x_vel_term = -V->x_vel;
	y_vel_term = V->y_vel;
	break;
	
	case 02:
	x_vel_term = V->x_vel;
	y_vel_term = -V->y_vel;
	break;
	
	case 03:
	x_vel_term = V->x_vel;
	y_vel_term = -V->y_vel;
	break;
	}



if (C->F_div == 'T')
{
F_can_collide = (C->m*y_vel_term) + x_vel_term;	
}
else
{
F_can_collide = (C->m*x_vel_term) + y_vel_term;		
}	
	

	
if (F_can_collide <= 0 )	
{
return Collision_Type_Rect(V, B);
}	
else	
{
	
	
	

	
return 'S';	
}	
	
}

//This function is really bad. Needs heavy revision.
char Col_Slope_y(Slope* S, int x_vel, int y_vel, int current_x, int current_y, int min, int max, int t_y)
{
//slope = y_vel/x_vel	
int slope_difference;	
int constant_difference;	
	
int velocity_slope;	
	
constant_difference = t_y - S->y; //Difference between the slope's y position and the VBox's.

//Calculate VBox's travel slope.	
if (x_vel != 0)
{
velocity_slope = y_vel/x_vel;	
}
else
{
velocity_slope = y_vel*100;	
}	

	
if (S->F_div == 'T')
{
slope_difference = S->m - ((S->m*S->m)*(y_vel/x_vel));		
}	
else 
{
slope_difference = S->m - velocity_slope;
}


if (slope_difference == 0)
{
printf("Error! Attempt to divide by 0 when finding x collision! \n");
return 'F';	
}

int x_collision = constant_difference/slope_difference;


int y_collision = velocity_slope*x_collision + (current_y + y_vel);



if (y_collision >= min && y_collision <= max )
{
printf("end T \n");
return 'T';	
}
else
{
printf("end F \n");
printf("y coll %d | %d | %d \n", min, y_collision, max);
return 'F';	
}	

}	

//Give general slope resolution methods later.
int Res_Box_Slope_Vertical(Box* A, Slope* B)
{
//The slope resolution revolves around resolving a single point, in practice on a box.
//This point can be a corner or a midpoint on the top or bottom edge.
//y_res is the resolved y height

//min and max y resolves are the limits to how far from the slope y position the point can be resolved.

int x_cons;
//Extra vertical displacement due to box's height.
int y_disp;

int y_ret;


switch(B->F_corner)
{
	case 00:
	x_cons = A->x;
	y_disp = A->h;
	break;	
	case 01:
	x_cons = A->x + A->w;
	y_disp = A->h;
	break;	
	case 02:
	x_cons = A->x;
	y_disp = 0;
	break;		
	case 03:
	x_cons = A->x + A->w;
	y_disp = 0;
	break;		
}

//This simple if/else statement sets the return y value based on whether the slope is fractional or whole.
if (B->F_div == 'F')
{
y_ret = B->y + (x_cons - B->x )*B->m - y_disp;
}
else
{
y_ret = B->y - (B->x - x_cons)/B->m - y_disp;
}	




return y_ret;	
}

int Res_Box_Slope_Horizontal(Box* A, Slope* B)
{
//The slope resolution revolves around resolving a single point, in practice on a box.
//This point can be a corner or a midpoint on the top or bottom edge.
//y_res is the resolved y height


int y_cons;
//Extra horizontal displacement due to box's height.
int x_disp;

int x_ret;



switch(B->F_corner)
{
	case 00:
	y_cons = A->y + A->h;
	x_disp = 0;
	break;	
	case 01:
	y_cons = A->y + A->h;
	x_disp = A->w;
	break;	
	case 02:
	y_cons = A->y;
	x_disp = 0;
	break;		
	case 03:
	y_cons = A->y;
	x_disp = A->w;
	break;		
}
//y = m(x+c) + b
//x = (y - b)/m - c
if (B->F_div == 'F')
{
	
x_ret = B->x + (y_cons - B->y)/B->m - x_disp;


}
else
{
x_ret = B->x - (B->y - y_cons)*B->m - x_disp;



}	


return x_ret;	
}
	
	