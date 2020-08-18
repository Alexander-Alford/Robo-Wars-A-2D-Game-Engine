#include <Global.h>


typedef struct
{
	int m;
	char F_div;
	char F_corner;
	int x;
	int y;
} Slope;






void Move_VBox(VBox* p_self);
void Debug_VBox_Mover(VBox* p_self);
void Debug_VBox_Mover_two(VBox* p_self, unsigned int vel);
void Apply_Acceleration(VBox* p_self, int x_a, int y_a);

GBFS Det_Box_Collision(Box* A, Box* B);




char Col_Rect(VBox* V, Box* B);

GBFS Solver_Rect(VBox* V, Box* B, GBFS C) ;

GBFS Tile_Collision_Physical(VBox* self);
