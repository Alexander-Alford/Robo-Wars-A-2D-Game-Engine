/*
This file holds the main function and serves to link together the functionality of every other major
source file. The main function was intentionally kept as simple as possible to maintain a coherent
program structure and to offload the majority of the source code into other files.

As a note for the rest of the source code. Any printf() usage is strictly for debugging.
*/




char coreloop_f = 'T';

/*
The main function for the program is a simple fixed function pipeline where all of the details are
left up to the major functions. There is one function executed at startup, a continuous loop that
the entirety of the game inhabits, and a termination program. Each major function can be broken 
down as follows:
    Begin(): Initializes SDL and its subsystems, creates a window.
    TakeInputs: Handles all external input: keyboard, gamepad, etc.
    UpdateObjects: Handles all game states, logic, AI, and objects.
    ManageGraphics: Handles updating graphics, applying effects, and manipulating images/colors.
    ControlSounds: Handles all sound effects and music as well as mixing.
    FrameDelay: Serves to forces every game loop to take atleast the desired amount of miliseconds.
                Because this game is completely tied to framerate, keeping the framerate consistent
                is incredibly inmportant.
    Terminate(): Destroys all remaining allocated resources and terminates all SDL systems.
*/	
int main(int argc, char* argv[]) {
printf( "PROGRAM START\n" );
	
Start_Window();			

while(coreloop_f != 'F'){
    TakeInputs();
    UpdateObjects();
    ManageGraphics();
    ControlSounds();	
    FrameControl();	
}
	
Terminate();

printf( "PROGRAM END\n" );	
return 0;
}