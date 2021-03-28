#ifndef CNR_H
#define CNR_H

#include "global.h"






void vboxMove(Vbox* self);
static inline void vboxAddVel(Vbox* self, int16_t x_a, int16_t y_a);

static inline void boxResLeft(Box* A, Box* B);
static inline void boxResRight(Box* A, Box* B);
static inline void boxResUp(Box* A, Box* B);
static inline void boxResDown(Box* A, Box* B);

uint8_t vboxSolveBox(Vbox* V, Box* B);

uint8_t TileCollisionSolver(Vbox* V);

#endif 