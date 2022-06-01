#include "draw.h"
#include <SDL2/SDL_opengles2.h>

void draw(int indices_size) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glDrawElements(GL_TRIANGLE_STRIP, indices_size, GL_UNSIGNED_INT, 0);
}