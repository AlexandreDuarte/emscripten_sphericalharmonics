#OBJS specifies which files to compile as part of the project
OBJS = main.c init.c input.c draw.c io.c

#CC specifies which compiler we're using
CC = gcc

#COMPILER_FLAGS specifies the additional compilation options we're using
# -w suppresses all warnings
COMPILER_FLAGS = 

#LINKER_FLAGS specifies the libraries we're linking against
LINKER_FLAGS = -lSDL2 -lGLESv2

#OBJ_NAME specifies the name of our exectuable
OBJ_NAME = test1

#This is the target that compiles our executable
default: $(OBJS)
	$(CC) $(OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)

ems: $(OBJS)
	emcc $(OBJS) -s USE_SDL=2 -s USE_WEBGL2=1 -s FULL_ES3=1 -o test.html --preload-file ./assets -Werror
