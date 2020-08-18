#include <SDL.h>

SDL_Event InputEvent;

enum KEY_STATES
{
STANDBY,
PRESSED,
HELD,
RELEASED
};

enum KEYS
{
UNKNOWN,
UP,
DOWN,
LEFT,
RIGHT,
A,
B,
C,
D,
E,
F,
G,
H,
I,
J,
K,
L,
M,
N,
O,
P,
Q,
R,
S,
T,
U,
V,
W,
X,
Y,
Z,
TOTAL_KEYS
};

extern char F_KeyState[TOTAL_KEYS];

void TakeInputs();