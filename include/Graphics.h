
#include <Global.h>

typedef struct{
	SDL_Texture* p_texture;	
	unsigned int pnts_to_text;	
	char* Name;
} T_Bind;

extern SDL_Rect SCREEN;

//Declaration of pointers to SDL surface, texture, window, and renderer structures.
SDL_Window* WINDOW;
SDL_Renderer* RENDERER;
SDL_Texture* SCREEN_TEXTURE;
T_Bind* Background;
extern long int TEXTURE_COUNT;
extern float TARGET_FPS;
extern float AVERAGE_FPS;
extern unsigned int FRAMES_ELAPSED;

//Declaration of functions.
SDL_Surface* Load_Surface(const char* PATH);
SDL_Texture* Load_Texture(const char* PATH);
void ManageGraphics();
GBFS Is_On_Screen(SDL_Rect test);
T_Bind* Assign_Texture(T_Bind* p_reuse, char* PATH);
void Destroy_Check_Texture(T_Bind* p_check);
void Position_Screen(Box* center);
