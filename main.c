//If a variable starts with a capital F it is a flag.
//General rule: Don't include header files in other header files. Even with ifndef.
//Boolean variables will be represented as char's that can be assigned either 'F' for False or 'T' for True.
//PATH always referes to a filepath and should never be a global term.

#include <stdio.h>
#include <Input.h>
#include <Object.h>
#include <Startup.h>
#include <Graphics.h>
#include <Sound.h>


//T = continue loop, F = exit loop.
char F_Core_Loop = 'T';

int GAME_SPEED = 1;


int main(int argc, char* argv[]) 
{
printf( "PROGRAM START\n" );
	
Start_Window();			
/*
This is the primary game loop that will only end when the core loop flag is False.
TakeInputs: Handles all external input.
UpdateObjects: Handles all game logic, AI, and objects.
ManageGraphics: Handles updating graphics, applying effects, and manipulating images.
PlaySounds: Handles all sound effects and music.
FrameDelay: Forces every game loop to take atleast the given time.
*/	
while(F_Core_Loop != 'F')
{
TakeInputs();

UpdateObjects();

ManageGraphics();
	
ControlSounds();	

//SDL_Delay(14);
FrameControl();	
}
	
Terminate();

printf( "PROGRAM END\n" );	
return 0;
}