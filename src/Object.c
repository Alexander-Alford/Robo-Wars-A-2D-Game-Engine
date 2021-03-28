/*

*/

#include "global.h"
#include "cnr.h"
#include "debug.h"






//Core loop master function.
void CoreLogic()
{
	switch(GAME_STATE)
	{
	case 0:
	break;
	case 1:
	break;
	case 2:
	DebugVBoxMover(&Bijec.vb, 3);
	vboxMove(&Bijec.vb);
	TileCollisionSolver(&Bijec.vb);
	break;
	case 3:
	break;
	}
}



