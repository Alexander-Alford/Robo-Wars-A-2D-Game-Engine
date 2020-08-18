#OBJS specifies which files to compile as part of the project
OBJS = main.c startup.c graphics.c Input.c Object.c Grid.c Sound.c Enemy.c Experiment.c Player.c C_and_R.c Global.c

#CC specifies which compiler we're using 
CC = gcc 

#INCLUDE_PATHS specifies the additional include paths we'll need 
INCLUDE_PATHS = -IC:\PROG\include\SDL2 -IC:.\include

#LIBRARY_PATHS specifies the additional library paths we'll need 
LIBRARY_PATHS = -LC:\PROG\lib\SDL2

#COMPILER_FLAGS specifies the additional compilation options we're using 
# -w suppresses all warnings 
# -Wl,-subsystem,windows gets rid of the console window 
COMPILER_FLAGS = 

#LINKER_FLAGS specifies the libraries we're linking against 
LINKER_FLAGS = -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_mixer

#OBJ_NAME specifies the name of our exectuable 
OBJ_NAME = main 

#This is the target that compiles our executable 
all : $(OBJS) 
		$(CC) $(OBJS) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)