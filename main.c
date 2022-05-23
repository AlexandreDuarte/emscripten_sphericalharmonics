
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#include "defs.h"
#include "structs.h"
#include "init.h"
#include "draw.h"
#include "input.h"

App app;

void mainLoop(void);
void stop(void);

int main(int argc, char const *argv[]) {

    printf("Main n\n");
    initSDL(&app);
    initGL();

    glViewport(0, 0, app.width, app.height);

#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop(mainLoop, 0, 1);
#else
    while (!app.quit)
    {
        mainLoop();
    }
#endif

    return 0;
}

void mainLoop(void) {
    
    draw();

    SDL_GL_SwapWindow(app.window);

    SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			case SDL_QUIT:
				stop();
				break;
			case SDL_WINDOWEVENT:
            	if (event.window.event == SDL_WINDOWEVENT_RESIZED) 
            	{
                app.width = event.window.data1;
                app.height = event.window.data2;
                glViewport(0, 0, app.width, app.height);
            	}
            	break;
			default:
				break;
		}
	}
}

void stop(void) {
#ifdef __EMSCRIPTEN__
	emscripten_cancel_main_loop();
#endif
    SDL_DestroyWindow(app.window);
    SDL_Quit();
    exit(0);
}
