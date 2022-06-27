#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <emscripten/html5.h>
#include <SDL.h>
#ifndef GL_GLEXT_PROTOTYPES
#define GL_GLEXT_PROTOTYPES
#endif
#include <SDL_opengles2.h>
#else

#include <SDL2/SDL.h>
#ifndef GL_GLEXT_PROTOTYPES
#define GL_GLEXT_PROTOTYPES
#endif
#include <SDL2/SDL_opengles2.h>
#include <SDL2/SDL_opengles2_gl2ext.h>
#endif

#include "structs.h"

#ifndef _H_INIT_
#define _H_INIT_


void initSDL(App*);
void initGL(ObjectData*);

void destroyGL(ObjectData*);

GLuint loadAndCompileShader(GLenum shaderType, const char *sourceCode);
GLuint linkShader(GLuint vertShaderID, GLuint fragShaderID);

#endif
