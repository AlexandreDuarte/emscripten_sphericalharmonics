
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#include "defs.h"
#include "structs.h"
#include "init.h"
#include "draw.h"
#include "input.h"

App app;

void mainLoop(ObjectData* data);
void stop(void);

int main(int argc, char const *argv[]) {

    initSDL(&app);

    ObjectData data;
    initGL(&data);

    glViewport(0, 0, app.width, app.height);

#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop_arg((void*)mainLoop, (void*)&data, 0, 1);
#else
    while (!app.quit)
    {
        mainLoop(&data);
    }
#endif

    return 0;
}

void mainLoop(ObjectData* data) {
    
    draw(data->indices_size);

    SDL_GL_SwapWindow(app.window);

    SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			case SDL_QUIT:
                free(data->indices_data);
                free(data->vertex_data);
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
