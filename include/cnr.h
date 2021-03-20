#include "global.h"









void Debug_VBox_Mover(VBox* p_self);
void Debug_VBox_Mover_two(VBox* p_self, unsigned int vel);

inline void vboxMove(Vbox* self);
inline void vboxAddVel(Vbox* self, int16_t x_a, int16_t y_a);



inline void boxResLeft(Box* A, Box* B);
inline void boxResRight(Box* A, Box* B);
inline void boxResUp(Box* A, Box* B);
inline void boxResDown(Box* A, Box* B);



char vboxSolveBox(Vbox* V, Box* B);

uint8_t TileCollisionSolver(Vbox* V);
