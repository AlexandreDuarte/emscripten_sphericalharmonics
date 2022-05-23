#include "draw.h"
#include <SDL2/SDL_opengles2.h>

void draw(void) {
    glClear(GL_COLOR_BUFFER_BIT);

    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
}