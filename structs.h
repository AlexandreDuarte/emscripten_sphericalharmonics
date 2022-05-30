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


typedef struct {
    int vertex_size;
    int indices_size;
    float* vertex_data;
    unsigned int* indices_data;
} ObjectData;

#endif