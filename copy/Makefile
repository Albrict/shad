#OBJS specifies which files to compile as part of the project
OBJS = shad.h error.c screen.c controls.c shad.c

#CC specifies which compiler we're using
CC = gcc

#COMPILER_FLAGS specifies the additional compilation options we're using
# -w suppresses all warnings
COMPILER_FLAGS = -Wall -W -g -O0 

#LINKER_FLAGS specifies the libraries we're linking against
LINKER_FLAGS = -lncurses

#OBJ_NAME specifies the name of our exectuable
OBJ_NAME = shad

#This is the target that compiles our executable
shad: $(OBJS)
	$(CC) $(OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)
