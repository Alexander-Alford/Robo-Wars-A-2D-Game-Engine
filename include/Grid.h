#include <Global.h>


SDL_Texture* TILES_TEXTURE;

typedef struct 
{
	char BlockPropety;
	unsigned int TileCode_G;
	unsigned int CollisionCode;
	char tF_flip;	
} Tile;


unsigned int TOTAL_TILE_TEXTURES;
unsigned int DEBUG_TILE_INDEX;

extern Tile* p_PRIMARY_TILE_ARRAY;

void Assign_Tile_Texture_Array(const char* PATH);

void Render_Tiles();

void ReadMapData(const char* PATH, DPS* o_dps, unsigned int level_id);

void Free_Tile_Texture();

void Free_Tile_Array();

