#include <SDL2/SDL.h>

#ifndef _H_STRUCTS_
#define _H_STRUCTS_

typedef struct {
    SDL_GLContext *glContext;
    int quit;
    SDL_Window *window;
    int width;
    int height;
} App;

#endif