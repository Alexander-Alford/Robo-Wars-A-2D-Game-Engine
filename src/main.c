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
the entirety of the game inhabits, and a termination program. Each major function can be found fully
described in one of the source files.
*/	
int main(int argc, char* argv[]) {

puts("PROGRAM START");
	
Start_Window();			

while(coreloop_f != 'F'){
    CoreInput();
    CoreObject();
    CoreGraphic();
    CoreSound();	
    FrameControl();	
}
	
Terminate();

puts("PROGRAM END");	

return 0;
}