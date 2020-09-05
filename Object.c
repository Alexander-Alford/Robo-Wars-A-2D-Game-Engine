/*
To handle objects, a strong ditinction must be made between types of objects and instances of object. A given type of an object will
share texture, sounds, run code, and other non-changing aspects of the object. Individual instances of an object must be able to maintain their
own personal data independent of all other instances. These instances will be linked to eachother in a chain, tethered to the object itself.

Type Properties:
-ID
-Source texture
-Sound chunks
-Constant data values
-run code

Instance Properties:
-Position/velocity
-unshared graphical data/flags
-unshared sound data
-personal data



-----------------Reference Object Data--------------------------------
#1 					//object id
1 					//run code
0 1 				//sound and texture flags
Assets/Player.png 	//Texture resource
1 1 1 1 6 			//GBFS, hb containers, cb containers, tb containers, and integer members. Should be atleast 1 collision box.

1 1 1 1 1 0 0 0 	//Default GBFS

0 0 0 0 0 0 0 0		//Default hb containers					
1					//Return value of hitbox
0 0 16 24			//hitbox data, to be used as offsets from primary cb box

1 1 0 0 0 0 0 0 	//Default cb containers	
0 0 16 24 0 0		//Collision flags default to 0

1 0 0 0 0 0 0 0 	//Default tb containers
0 0 16 24
0 0 20 40
0					//no flip, v_flip, h_flip, both

0 0 3 5 5 0 		//integer members

-----------------Reference Map Instance Declaration----------------------------

1				//# of object types in map

0001			//first listed object id
1 				//# of instances of object

120 100 	-1	//x_y position of instance no change, skip
-------------------------------------------------------------------------------
*/

#include <Global.h>
#include <Input.h>
#include <Experiment.h>
#include <Graphics.h>
#include <Player.h>
#include <C_and_R.h>
#include <Sound.h>
#include <Enemy.h>



typedef struct //Hit box container.
{
GBFS flg; //Check coll? Flip other boxes' collision checking?	
unsigned int ret_val;
Box bo;	//Hit box coordinates are relative to CB[0].
} HB_con;

typedef struct //Collision box container.
{ 
GBFS flg; //Move when called? collides with tiles? collides with other objects? d,e,f,g,h = extra
GBFS col; //Collision flags for tile collision resolution. If vb doesn't collide with them can be used for something else.
VBox vb;
} CB_con;

typedef struct //Texture bind container.
{
GBFS flg; //Render? null source? null dest? defgh = to be decided
SDL_Rect dest; //should be controlled by object function
SDL_Rect src;	//^^^^^^^^
int flip; //^^^^^^^^^^^^^^
} TB_con;

typedef struct //Instance data.
{
GBFS* fl;
HB_con* hb;	
CB_con* cb;
TB_con* tb;
int* iv;
long int special[10];	//Solution to creation values problem. Defaults to 0 for every value.	
void* parent_o;
} idat;

//Add render instructions.(render/no render, simple/ex, use vbox as dest/use sdl rect, no source rect, if ex rotate?, if ex flip?)
typedef struct
{
//Object meta-data. Data about the object structure itself.
	unsigned long int ID; 
	unsigned long int index; //Index in object array.

//Shared Resources. Pointers may be shared with several other objects. If so, do not deallocate resources until no object is using them.
	unsigned int size[5]; //The number of GBFS, hb containers, cb containers, tb containers, and integer members in an instance.
	
	void* p_S; //Sound resource, S_bind. 
	void* p_T; //Texture resource, T_bind. 
	

	idat refer;	//Reference instance.
	void (*func)(idat*); //Pointer to a function pointer that takes input from p_data_c and p_data_p.

//Dynamic instance array.
	DPS* dps_ins; 
	
} Object;

unsigned long int OBJECT_COUNT = 0; //Total number of loaded/active objects.
unsigned long int OBJECT_ARRAY_SIZE = 0; //Total number of pointers in object array. 

DPS* OBJECT_P_ARRAY = NULL;

//These need to be fixed at some point.
void Decide_Player_State(idat* self);
void Bouncer_Run(idat* self);
void Resolve_Tile_Collisions(Object* obj, idat* ida);



//Deallocate and create object instance. 
void Destroy_Instance(idat* target)
{
free(target->fl);
free(target->hb);	
free(target->cb);
free(target->tb);
free(target->iv);
printf("	Instance deallocated.\n");	
}


void Create_Object_Instance(Object* parent, int x_y[2], long int special_buff[10])
{
	if(parent == NULL)
	{
	printf("Warning! Could not find object to create instance of.\n");	
	return;
	}
		
	if(parent->dps_ins == NULL)
	{
	printf("Warning! Could not create instance for object ID: %d\nNo instance array present.\n", parent->ID);	
	return;
	}
	

	idat* p_new = malloc(sizeof(idat));
	
		p_new->fl = malloc(parent->size[0]*sizeof(GBFS));
		p_new->hb = malloc(parent->size[1]*sizeof(HB_con));
		p_new->cb = malloc(parent->size[2]*sizeof(CB_con));
		p_new->tb = malloc(parent->size[3]*sizeof(TB_con));
		p_new->iv = malloc(parent->size[4]*sizeof(int));
	
	for(unsigned register int I = 0; I < parent->size[0]; I++)
	{
	p_new->fl[I] = parent->refer.fl[I];		
	}
	for(unsigned register int I = 0; I < parent->size[1]; I++)
	{
	p_new->hb[I] = parent->refer.hb[I];		
	}
	for(unsigned register int I = 0; I < parent->size[2]; I++)
	{
	p_new->cb[I] = parent->refer.cb[I];		
	}	
	for(unsigned register int I = 0; I < parent->size[3]; I++)
	{
	p_new->tb[I] = parent->refer.tb[I];	
	}
	for(unsigned register int I = 0; I < parent->size[4]; I++)
	{
	p_new->iv[I] = parent->refer.iv[I];	
	}
	
		if(parent->size[2] > 0)
		{
		p_new->cb[0].vb.box.x = x_y[0];
		p_new->cb[0].vb.box.y = x_y[1];
		}
		else
		{	
		printf("Error! No assignable x/y values to object!/n");	
		}
	
	for(unsigned register int S = 0; S < 10; S++)
	{
	p_new->special[S] = special_buff[S];
	}
	
	p_new->parent_o = parent;
	
	printf("	Instance of Object %d created: x=%d, y=%d, with special |%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|.\n", parent->ID, p_new->cb[0].vb.box.x, p_new->cb[0].vb.box.y, p_new->special[0], p_new->special[1], p_new->special[2], p_new->special[3], p_new->special[4], p_new->special[5], p_new->special[6], p_new->special[7], p_new->special[8], p_new->special[9]);

DPS_Add_Member(parent->dps_ins, p_new, 1);
}




//Grab_Run_Code takes an integer ID argument and returns a pointer to a void returning function with two void pointer parameters. 
void (*Load_F_Pointers(unsigned int f_Code))(idat*)
{
	void (*p_to_func)(idat*) = NULL;
	
	switch(f_Code)
	{
	case 1:	
	p_to_func = &Decide_Player_State;
	break;
	case 2:
	p_to_func = &Bouncer_Run;
	break;
	default:
	printf("Error! Could not grab object run code. \n");
	break;	
	}
	
return p_to_func;
}

//Utility function. Might not be needed.
Object* Return_p_Object_With_ID(const int ID, DPS* o_array)
{
	
	for(register int I = 0; I < o_array->size; I++)
	{
		
		if(ID == ((Object*)o_array->Array[I])->ID)
		{
		return o_array->Array[I];	
		}
	}	

printf("Could not find %d object ID in array. \n", ID);	
return NULL;	
}

void Prepare_Object_S_Bind(unsigned int target_ID, Object* obj, const char* PATH, DPS* obj_arr)
{
	
	
	register int I = 0;
	register int N = 0;
	
	while(I < obj_arr->size && N <= obj_arr->non_null) //Loop checks to see if texture already exists.
	{
		
		if(obj_arr->Array[I] != NULL)
		{
		
			if( ((Object*)obj_arr->Array[I])->p_S != NULL && ((S_Bind*)((Object*)obj_arr->Array[I])->p_S)->ID == target_ID)
			{
			obj->p_S = Assign_Sound_Bank(((Object*)obj_arr->Array[I])->p_S, "Assets/s_bank", target_ID);
			return;	
			}
		
		N++;	
		}	
		
	I++;
	}	
	
printf("	A new sound bank was allocated...\n");	
obj->p_S = Assign_Sound_Bank(NULL, PATH, target_ID);

return;			
}

//Prepares an objects T_Bind.
void Prepare_Object_T_Bind(char* PATH, Object* obj, DPS* obj_arr)
{
	
	if(PATH == NULL)
	{
	printf("Error? Attempt to prepare T_bind without a path.\n");	
	return;	
	}
	
	register int I = 0;
	register int N = 0;
	
	while(I < obj_arr->size && N <= obj_arr->non_null) //Loop checks to see if texture already exists.
	{
		
		if(obj_arr->Array[I] != NULL)
		{
		
			if( ((Object*)obj_arr->Array[I])->p_T != NULL && ((T_Bind*)((Object*)obj_arr->Array[I])->p_T)->Name == PATH)
			{
			obj->p_T = Assign_Texture(((Object*)obj_arr->Array[I])->p_T, NULL);
			return;	
			}
		
		N++;	
		}	
		
	I++;
	}	
	
printf("	A new texture was allocated...\n");	
obj->p_T = Assign_Texture(NULL, PATH);	

return;		
}

//Initiates object structure members to 0 and null. Returns object pointer.
Object* Create_Object(DPS* o_Dps, unsigned int ID, unsigned int inst_size[5], char* T_PATH, unsigned int sbank_id, idat* ref_data, unsigned int run_code, unsigned int start_inst_num)
{
	
	Object* p_new = malloc(sizeof(Object));

	if(p_new == NULL)
	{
	printf("Error. Failure to allocate memory for Object. \n");	
	}	
	
	else //Sets object meta-data.
	{
	p_new->ID = ID;	
	p_new->index = DPS_Add_Member(o_Dps, p_new, 10);

	p_new->p_S = NULL;
	p_new->p_T = NULL;
	p_new->func = NULL;
	p_new->dps_ins = NULL;

	
	p_new->size[0] = inst_size[0];
	p_new->size[1] = inst_size[1];
	p_new->size[2] = inst_size[2];
	p_new->size[3] = inst_size[3];
	p_new->size[4] = inst_size[4];	

		if(p_new->index < 0)
		{
		printf("Error. Could not create new object. Object array is full. \n");
		free(p_new);
		return NULL;
		}
		
		else //Sets object shared resources.
		{
		Prepare_Object_S_Bind(sbank_id, p_new, "Assets/s_bank", o_Dps); 
		Prepare_Object_T_Bind(T_PATH, p_new, o_Dps);

		p_new->refer.fl = ref_data->fl;
		p_new->refer.hb = ref_data->hb;
		p_new->refer.cb = ref_data->cb;
		p_new->refer.tb = ref_data->tb;
		p_new->refer.iv = ref_data->iv;
		
		for(register char i = 0; i < 10; i++)
		{			
			p_new->refer.special[i] = 0;
		}

		p_new->func = Load_F_Pointers(run_code);
		
		p_new->dps_ins = Create_DPS(start_inst_num);
		}

	OBJECT_COUNT++;	
	printf("	Object #%d created. Sizes: |%d|%d|%d|%d|%d|\n", ID, p_new->size[0], p_new->size[1], p_new->size[2], p_new->size[3], p_new->size[4]);
	return p_new;
	}	
}




//Loads the given number and kind of objects based on data at the given path to the given object array.
Object* Load_Object(const char* PATH, DPS* o_Arr, unsigned int ID)
{
		
	FILE* p_O_data = NULL;
	p_O_data = fopen(PATH, "r");
				
	if (!p_O_data)
	{
	printf("Failed to open object file. \n");
	return NULL;
	}	
	
		
	char pound_check = 'A';
	unsigned int ID_buff = 0;
	
	unsigned int start_inst = 0;
	unsigned int int_buffer[5] = {0}; 
	unsigned int flag_buffer[8] = {0};
	unsigned int s_bank_ID = 0;
	char T_PATH[100] = {0};
	idat ref_copy = {NULL,NULL,NULL,NULL,NULL,{0}};
	unsigned int RC_ID = 0; //run code
			
		while(pound_check != '#' && fscanf(p_O_data, "%c", &pound_check) != EOF)
		{
			if(pound_check == '#')
			{
				
				
				fscanf(p_O_data, "%u", &ID_buff);
					
					//Once the correct ID is found, all of the resources are loaded into buffers that are used to create the object.
					if(ID_buff == ID)
					{
					fscanf(p_O_data, "%u", &RC_ID);
					fscanf(p_O_data, "%u %u", &int_buffer[0], &int_buffer[1]);
							
						if(int_buffer[0] > 0)
						{
						fscanf(p_O_data, "%u", &s_bank_ID);	
						}
							
						if(int_buffer[1] > 0)
						{
						fscanf(p_O_data, "%100s", &T_PATH);	
						}
						
					fscanf(p_O_data, "%u", &start_inst);											
					fscanf(p_O_data, "%u %u %u %u %u", &int_buffer[0], &int_buffer[1], &int_buffer[2], &int_buffer[3], &int_buffer[4]);
	
					
					ref_copy.fl = malloc(int_buffer[0]*sizeof(GBFS));
					ref_copy.hb = malloc(int_buffer[1]*sizeof(HB_con));					
					ref_copy.cb = malloc(int_buffer[2]*sizeof(CB_con));
					ref_copy.tb = malloc(int_buffer[3]*sizeof(TB_con));
					ref_copy.iv = malloc(int_buffer[4]*sizeof(int));
	
					for(unsigned register int I = 0; I < int_buffer[0]; I++) //Reference instance flags.
						{
							for(register int C = 0; C < 8; C++)
							{
							fscanf(p_O_data, "%u", &flag_buffer[C]);						
							}

							ref_copy.fl[I] = Assign_GBFS(flag_buffer);
						}
						
					for(unsigned register int I = 0; I < int_buffer[1]; I++) //Reference instance hitboxes.
						{						
							for(register int C = 0; C < 8; C++)
							{
							fscanf(p_O_data, "%u", &flag_buffer[C]);
							}						
						
							ref_copy.hb[I].flg = Assign_GBFS(flag_buffer);
							
							fscanf(p_O_data, "%u", &ref_copy.hb[I].ret_val);
							
							fscanf(p_O_data, "%u", &ref_copy.hb[I].bo.x);
							fscanf(p_O_data, "%u", &ref_copy.hb[I].bo.y);
							fscanf(p_O_data, "%u", &ref_copy.hb[I].bo.w);
							fscanf(p_O_data, "%u", &ref_copy.hb[I].bo.h);	
						}
						
					for(unsigned register int I = 0; I < int_buffer[2]; I++) //Referemce instance collision boxes.
						{
							for(register int C = 0; C < 8; C++)
							{
							fscanf(p_O_data, "%u", &flag_buffer[C]);
							}
							
							unsigned int col_null[8] = {0};
							
							ref_copy.cb[I].flg = Assign_GBFS(flag_buffer);	
							ref_copy.cb[I].col = Assign_GBFS(col_null);
							
							fscanf(p_O_data, "%u", &ref_copy.cb[I].vb.box.x);
							fscanf(p_O_data, "%u", &ref_copy.cb[I].vb.box.y);
							fscanf(p_O_data, "%u", &ref_copy.cb[I].vb.box.w);
							fscanf(p_O_data, "%u", &ref_copy.cb[I].vb.box.h);
							fscanf(p_O_data, "%u", &ref_copy.cb[I].vb.x_vel);
							fscanf(p_O_data, "%u", &ref_copy.cb[I].vb.y_vel);					

						}
								
					for(unsigned register int I = 0; I < int_buffer[3]; I++) //Reference instance texture binds.
						{
							for(register int C = 0; C < 8; C++)
							{
							fscanf(p_O_data, "%u", &flag_buffer[C]);	
							}
							ref_copy.tb[I].flg = Assign_GBFS(flag_buffer);

	
							fscanf(p_O_data, "%u", &ref_copy.tb[I].dest.x);
							fscanf(p_O_data, "%u", &ref_copy.tb[I].dest.y);
							fscanf(p_O_data, "%u", &ref_copy.tb[I].dest.w);
							fscanf(p_O_data, "%u", &ref_copy.tb[I].dest.h);

							fscanf(p_O_data, "%u", &ref_copy.tb[I].src.x);
							fscanf(p_O_data, "%u", &ref_copy.tb[I].src.y);
							fscanf(p_O_data, "%u", &ref_copy.tb[I].src.w);
							fscanf(p_O_data, "%u", &ref_copy.tb[I].src.h);
								
							fscanf(p_O_data, "%u", &ref_copy.tb[I].flip);
						}							
							
					for(unsigned register int I = 0; I < int_buffer[4]; I++) //Reference instance integer values. 
						{
							fscanf(p_O_data, "%d", &ref_copy.iv[I]);
						}	
					
					
						fclose(p_O_data);

						return Create_Object(OBJECT_P_ARRAY, ID, int_buffer, T_PATH, s_bank_ID, &ref_copy, RC_ID, start_inst);
					}
					else
					{
					pound_check = 'A';	
					}
			}
		}
	
	printf("Error! could not find object. \n", ID);	
	fclose(p_O_data);
	return NULL;
		
}




//Frees all of an object's allocated resources and checks intra-object resources.
void Destroy_Object(DPS* holder, Object* p_obj)
{
	if(p_obj != NULL)
	{
		printf("Destroying object #%d... \n", p_obj->ID);

		if(p_obj->p_T != NULL)
		{
		Destroy_Check_Texture(p_obj->p_T);	
		}
		
		if(p_obj->p_S != NULL)
		{
		Check_Destroy_Sbank(p_obj->p_S);
		}

			free(p_obj->refer.fl);
			free(p_obj->refer.hb);
			free(p_obj->refer.cb);		
			free(p_obj->refer.tb);
			free(p_obj->refer.iv);

		p_obj->func = NULL;
		
		for(unsigned register int I = 0; I < p_obj->dps_ins->size; I++)
		{
			if(p_obj->dps_ins->Array[I] != NULL)
			{
			Destroy_Instance(((idat*)p_obj->dps_ins->Array[I]));
			DPS_Remove_Member(p_obj->dps_ins, I);
			}	
		}
		
		Destroy_DPS(p_obj->dps_ins);

		OBJECT_COUNT--;
		DPS_Remove_Member(holder, p_obj->index);
		printf("	Object destroyed.\n", p_obj->ID, p_obj->ID);
	}
	else
	{
	printf("Error! Attempt to destroy null object! \n");	
	}
}


void Destroy_Object_Array(DPS* p_target)
{
	if(p_target == NULL)
	{
	printf("Error! Attempt to destroy null object array. \n");	
	}
	
	else
	{
	printf("	Beginning object array destruction... \n");	
	unsigned register int A = 0;
	unsigned register int I = 0;
	
		while (A <= p_target->non_null && I < p_target->size)
		{
		printf("|");	
			if(p_target->Array[I] != NULL)
			{
			printf("%lu|	", ((Object*)p_target->Array[I])->ID);
			Destroy_Object(p_target ,p_target->Array[I]);	
	
			A++;		
			}
			else
			{
			printf("NULL|	");				
			}
		I++;
		}

	printf("Complete. Total objects destroyed = %d\n", A);
	
	Destroy_DPS(p_target);
	
	}
}




void Move_All_VBoxes(Object* obj, idat* ida)
{
	for(unsigned register int I = 0; I < obj->size[2]; I++)
		{
			if(ida->cb[I].flg.a == 1) //Move is move flag set.
			{
			Move_VBox(&ida->cb[I].vb);		
			}
		}
}

void Run_Object_Function(Object* obj, idat* ida)
{
	obj->func(ida);
}

void Object_Update(DPS* dps_o, void (*func)(Object*, idat*))
{
	
	if(dps_o != NULL)
	{
		for(unsigned register int A = 0, B = 0; A < dps_o->size && B <= dps_o->non_null; A++)
		{
			if(dps_o->Array[A] != NULL && ((Object*)dps_o->Array[A])->dps_ins != NULL)
			{
				for(unsigned register int C = 0, D = 0; C < ((Object*)dps_o->Array[A])->dps_ins->size && D <= ((Object*)dps_o->Array[A])->dps_ins->non_null; C++)
				{
					if(((Object*)dps_o->Array[A])->dps_ins->Array[C] != NULL)
					{
					func((Object*)dps_o->Array[A], ((idat*)((Object*)dps_o->Array[A])->dps_ins->Array[C]));
					D++;					
					}
				}
			B++;
			}	
		}
	}
	else
	{
	printf("Error. object array is null \n");		
	}
}





//The primary function that updates the status of every object every frame.
void UpdateObjects()
{
	
	//create/destroy objects.	

	Object_Update(OBJECT_P_ARRAY, &Move_All_VBoxes);
	Object_Update(OBJECT_P_ARRAY, &Resolve_Tile_Collisions);
	//Check collisions with projectiles/enemies/others.	
	Object_Update(OBJECT_P_ARRAY, &Run_Object_Function);
		
				//((Object*)OBJECT_P_ARRAY->Array[0])->func(((idat*)((Object*)OBJECT_P_ARRAY->Array[0])->dps_ins->Array[0]));	Deprecated...
		
		//Layered collision detection?
	//Resolve collisions.
	//Check states/flags.
	//AI.






	if(F_KeyState[P] == PRESSED)
	{
	Dump_Data_To_Console();
	}
}



