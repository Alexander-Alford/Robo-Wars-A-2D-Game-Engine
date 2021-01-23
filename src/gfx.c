/*
*/

#include <Global.h>
#include <Grid.h>
#include <Experiment.h>
#include <Object.h>
#include <C_and_R.h>


uint32_t WINDOW_WIDTH = 768;
uint32_t WINDOW_HEIGHT = 768;
float TARGET_FPS = 60.000;
float AVERAGE_FPS = 0;
uint32_t FRAMES_ELAPSED = 0;


SDL_Rect SCREEN = {0, 0, 256, 256};

long int TEXTURE_COUNT = 0;


//Needs some SERIOUS cleaning up along with startup.c.


typedef struct{
	SDL_Texture* p_texture;	
	unsigned int pnts_to_text;
	char* Name;
} T_Bind;




//Pointers to the window, renderer, and screen texture. Render->Screen_Texture->Window
SDL_Window* WINDOW = NULL;
SDL_Renderer* RENDERER = NULL;
SDL_Texture* SCREEN_TEXTURE = NULL;



//Background T_Bind.
T_Bind* Background = NULL;


GBFS Is_On_Screen(SDL_Rect* test)
{
	return Det_Box_Collision(test, &SCREEN);
}

//Centers screen on box and does not overstep level bounds.
void Position_Screen(Box* center)
{
int box_x_p_half = center->x + (center->w / 2);
int box_y_p_half = center->y + (center->h / 2);

int half_screenw = SCREEN.w / 2;
int half_screenh = SCREEN.h / 2;

SCREEN.x = box_x_p_half - half_screenw;
SCREEN.y = box_y_p_half - half_screenh;

if (SCREEN.x < 0)
{
SCREEN.x = 0;	
}	
	else if ((SCREEN.x + SCREEN.w) > (HORIZONTAL_LEVEL_TILES*BASE_TILE_SIZE))
	{
	SCREEN.x = 	(HORIZONTAL_LEVEL_TILES*BASE_TILE_SIZE) - SCREEN.w;
	}

if (SCREEN.y < 0)
{
SCREEN.y = 0;	
}	
	else if ((SCREEN.y + SCREEN.h) > (VERTICAL_LEVEL_TILES*BASE_TILE_SIZE))
	{
	SCREEN.y = (VERTICAL_LEVEL_TILES*BASE_TILE_SIZE) - SCREEN.h;
	}
	
}


//Returns a loaded surface. Loads a surface, converts it to a texture, then returns it.
SDL_Surface* Load_Surface(const char* PATH)
{
	SDL_Surface* Surface_Loaded = IMG_Load(PATH); //IMG_Load function from SDL_image.h now replaces SDL_LoadBMP.

		if (Surface_Loaded == NULL)	
			{
			printf( "Image %s failed to load! SDL_image Error: %s\n", PATH, IMG_GetError());
			}

return Surface_Loaded; 
}	
SDL_Texture* Load_Texture(const char* PATH)
{
	SDL_Texture* Return_texture = NULL; 
	SDL_Surface* BaseSurface = NULL; 

		BaseSurface = Load_Surface(PATH); 

		Return_texture = SDL_CreateTextureFromSurface(RENDERER, BaseSurface); //Creates a texture from the chosen render and the surface that contains the image.

		
		if (Return_texture == NULL)
			{
			printf("Unable to create texture from %s! SDL Error: %s\n", PATH, SDL_GetError());
			}
		else
			{
			TEXTURE_COUNT++;		
			}	
		
		SDL_FreeSurface(BaseSurface); 
	
return Return_texture; 	
}


//Creates and returns new T_bind or simply returns pointer of already created T_bind.
//Destroys a T_bind without anything pointing to it.
T_Bind* Assign_Texture(T_Bind* p_reuse, char* PATH)
{
	if(p_reuse == NULL)
	{
		T_Bind* p_new = malloc(sizeof(T_Bind));
			
			if(p_new != NULL)
			{	
			p_new->p_texture = Load_Texture(PATH);
				
				if(p_new->p_texture == NULL)
				{
				printf("Error assigning texture! \n");	
				free(p_new);
				return NULL;
				}	

			p_new->pnts_to_text = 1;
			
			unsigned int namebuffer = (strlen(PATH)+1);
			p_new->Name = malloc((namebuffer)*(sizeof(char)));
			strcpy(p_new->Name, PATH);
			p_new->Name[(namebuffer - 1)] = '\0';
			
			printf("Texture \"%s\" assigned. \n", p_new->Name);
			}
	return p_new;
	}
	
	else
	{
	p_reuse->pnts_to_text++;
	return p_reuse;
	}	
}
void Destroy_Check_Texture(T_Bind* p_check)
{
	if(p_check != NULL)
	{
	p_check->pnts_to_text--;
	
		if(p_check->pnts_to_text < 1)
			{
			SDL_DestroyTexture(p_check->p_texture);	
			TEXTURE_COUNT--;
			printf("	%s texture destroyed. \n", p_check->Name);
			free(p_check->Name);
			p_check->Name = NULL;
			free(p_check);	
			p_check = NULL;
			}	
	}
	else
	{
	printf("Warning! Checking NULL texture.\n");	
	}
}


void Render_Background(SDL_Texture* p_bg)
{
	if(p_bg)
	{
		SDL_Rect rr = {0,0,0,0};
		GBFS buffer = {0,0,0,0,0,0,0,0};
	
		SDL_QueryTexture(p_bg, NULL, NULL, &rr.w, &rr.h);

	
		while(rr.y < (SCREEN.y + SCREEN.h))
		{
			while(rr.x < (SCREEN.x + SCREEN.w))
			{
			buffer = Is_On_Screen(&rr);
			
				if(buffer.a == 1)
				{
				rr.x -= SCREEN.x;	
				rr.y -= SCREEN.y;
				SDL_RenderCopy(RENDERER, p_bg, NULL, &rr);	
				rr.y += SCREEN.y;
				rr.x += SCREEN.x;	
			}
		
			rr.x += rr.w;
			}
	
		rr.y += rr.h;
		rr.x = 0;
		}
	
	}
}

//Primary object rendering function.
void Render_temp(Object* obj, idat* target)
{

	if(target != NULL)
	{	
		for(unsigned register int I = 0; I < obj->size[3]; I++)
		{
			if(target->tb[I].flg.a == 1) //Render flag.
			{	
			SDL_Rect D = target->tb[I].dest;
			D.x -= SCREEN.x;
			D.y -= SCREEN.y;
			
				if(Is_On_Screen(&target->tb[I].dest).a == 1)
				{
				SDL_RenderCopyEx(RENDERER, ((T_Bind*)obj->p_T)->p_texture, &target->tb[I].src, &D, 0.0, NULL, target->tb[I].flip);
				}
			}	
		}
	}
	else
	{
	printf("Error! Instance null! \n");	
	}	
}

void ManageGraphics()
{

	//Sets the render target to the texture SCREEN_TEXTURE.
	SDL_SetRenderTarget(RENDERER, SCREEN_TEXTURE);

	SDL_RenderClear(RENDERER);
	
	Render_Background(Background->p_texture);

	Object_Update(OBJECT_P_ARRAY, &Render_temp);

	
	Render_Tiles();	
	SDL_SetRenderTarget(RENDERER, NULL);	
	


		
	SDL_RenderClear(RENDERER);	
	SDL_RenderCopy(RENDERER, SCREEN_TEXTURE, NULL, NULL);	
	//Update screen renderer.
	SDL_RenderPresent(RENDERER);
	
	FRAMES_ELAPSED++;
	AVERAGE_FPS = (FRAMES_ELAPSED / (SDL_GetTicks()/1000.f));
	//printf("%f \n", AVERAGE_FPS);
}