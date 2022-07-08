OBJS = main.c init.c input.c draw.c io.c math_utils.c spherical_harmonics.c matrix.c

CC = gcc

GCC_COMPILER_FLAGS = 

EMCC_COMPILER_FLAGS = -s USE_SDL=2 -s USE_WEBGL2=1 -s FULL_ES3=1 -Werror

LINKER_FLAGS = -lSDL2 -lGLESv2 -lm

OBJ_NAME = spherical_harmonics_native
EMCC_OBJ_NAME = spherical_harmonics_wasm.html

compile-native: $(OBJS)
	$(CC) $(OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o ./out/$(OBJ_NAME)  -O3

compile-web: $(OBJS)
	emcc $(OBJS) $(EMCC_COMPILER_FLAGS) -o ./out/web/$(EMCC_OBJ_NAME) --preload-file ./assets

run-native:
	./out/spherical_harmonics_native

run-web:
	emrun ./out/web/spherical_harmonics_wasm.html

