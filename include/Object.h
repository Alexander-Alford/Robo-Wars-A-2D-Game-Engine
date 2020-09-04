
#include <Global.h>

#ifndef OBJECT_H
#define OBJECT_H


typedef struct
{
GBFS flg; //Check coll? 	
unsigned int ret_val;
Box bo;	
} HB_con;

typedef struct
{ 
GBFS flg; 
GBFS col; 
VBox vb;
} CB_con;

typedef struct
{
GBFS flg; 
SDL_Rect dest; 
SDL_Rect src;	
int flip; 
} TB_con;

typedef struct
{
GBFS* fl;
HB_con* hb;	
CB_con* cb;
TB_con* tb;
int* iv;
long int special[10];	
} idat;


typedef struct
{
	unsigned long int ID; 
	unsigned long int index; 

	unsigned int size[5]; 
	
	void* p_S;
	void* p_T; 

	idat refer;	
	void (*func)(idat*); 

	DPS* dps_ins; 
	
} Object;

extern unsigned long int OBJECT_COUNT;
extern unsigned long int OBJECT_ARRAY_SIZE;

extern DPS* OBJECT_P_ARRAY;



//Object functions.
Object* Load_Object(const char* PATH, DPS* o_Arr, unsigned int ID);
void Create_Object_Instance(Object* parent, int x_y[2], long int special_buff[10]);
Object* Create_Object(DPS* o_Dps, unsigned int ID, unsigned int inst_size[5], const char* T_PATH, unsigned int sbank_id, idat* ref_data, unsigned int run_code, unsigned int start_inst_num);
void Destroy_Object_Array(DPS* p_target);

void Object_Update(DPS* dps_o, void (*func)(Object*, idat*));

//Master function. Do not touch.
void UpdateObjects();

#endif






