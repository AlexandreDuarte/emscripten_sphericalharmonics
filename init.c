#include "io.h"
#include "defs.h"
#include "init.h"
#include "spherical_harmonics.h"
#include "matrix.h"

#ifdef __EMSCRIPTEN__
//TODO implement resizing and dynamic size (dependant on website)
EM_JS(int, getWidth, (), {
    return 500;
  //return window.innerWidth*7/10;
});

EM_JS(int, getHeight, (), {
  return 500;
  //return window.innerHeight*7/10;
});
EM_BOOL resize_callback(int eventType, const EmscriptenUiEvent *e, void *rawState);
#endif

void initSDL(App *app)
{
    printf("start init call\n");
    int rendererFlags, windowFlags;

    rendererFlags = SDL_RENDERER_ACCELERATED;
    windowFlags = SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL;

    SDL_SetMainReady();

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("Unable to initialize SDL: %s\n", SDL_GetError());
        exit(1);
    }

#ifdef __EMSCRIPTEN__
    emscripten_set_resize_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, NULL, 1, resize_callback);
    app->width = getHeight();
    app->height = getHeight();
    //emscripten_set_canvas_element_size("canvas", app->width, app->height);
#else
    app->width = SCREEN_WIDTH;
    app->height = SCREEN_HEIGHT;
#endif

    app->window = SDL_CreateWindow("GL2", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, app->width, app->height, windowFlags);

    if (!app->window)
    {
        printf("Failed to initialize %d x %d window: %s\n", app->width, app->height, SDL_GetError());
        exit(1);
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetSwapInterval(0);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16 );

    app->glContext = SDL_GL_CreateContext(app->window);

    printf("Context created: %s\n", glGetString(GL_VERSION));

    if (!app->glContext)
    {
        printf("OpenGLES context could not be created! SDL Error: %s\n", SDL_GetError());
        exit(1);
    }
    printf("end init call\n");

}

void initGL(ObjectData* data)
{

    printf("initGL call\n");

    glEnable(GL_DEPTH_TEST);
   /*glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);*/
    glEnable(GL_CULL_FACE);

    char* vs = loadFile("assets/shaders/shader.vert");
    char* fs = loadFile("assets/shaders/shader.frag");

    printf("Successful file loading\n");

    GLuint vshader = loadAndCompileShader(GL_VERTEX_SHADER, vs);
    GLuint fshader = loadAndCompileShader(GL_FRAGMENT_SHADER, fs);

    free(vs);
    free(fs);
    printf("Successful shader loading\n");

    computeSphericalHarmonic(data, 4, 1);

    if (!data->indices_data) {
        printf("id failed\n");
    }

    if (!data->vertex_data) {
        printf("vd failed\n");
    }

    GLuint program = linkShader(vshader, fshader);
    glUseProgram(program);

    GLuint VAO, VBO, EBO;

#ifdef __EMSCRIPTEN__
    glGenVertexArraysOES(1, &VAO);
#else
    glGenVertexArrays(1, &VAO);
#endif
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    
#ifdef __EMSCRIPTEN__
    glGenVertexArraysOES(1, &VAO);
#else
    glBindVertexArray(VAO);
#endif

    data->VAO = VAO;
    data->VBO = VBO;
    data->EBO = EBO;

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*data->vertex_size, data->vertex_data, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*data->indices_size, data->indices_data, GL_STATIC_DRAW);

    GLint pos1Attrib = glGetAttribLocation(program, "pos");
    glVertexAttribPointer(pos1Attrib, 3, GL_FLOAT, GL_FALSE, 4*sizeof(float), 0);
    glEnableVertexAttribArray(pos1Attrib);

    GLint colorSwitchAttrib = glGetAttribLocation(program, "sC");
    glVertexAttribPointer(colorSwitchAttrib, 1, GL_FLOAT, GL_FALSE, 4*sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(colorSwitchAttrib);

    Matrix4 matrix, matrix1, matrix2;
    createRotationMatrix4X(&matrix1, -1.8f);
    createRotationMatrix4Z(&matrix2, M_PI - 0.5f);
    createMatrix4(&matrix);
    multMatrices(&matrix, 2, &matrix1, &matrix2);

    glUniformMatrix4fv(glGetUniformLocation(program, "mvp"), 1, GL_FALSE, matrix.elem);
    glUniform4f(glGetUniformLocation(program, "color1"), 0.9f, 0.9f, 0.9f, 1.0f);
    glUniform4f(glGetUniformLocation(program, "color2"), 0.2f, 0.2f, 0.2f, 1.0f);
    
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    printf("Completed gl allocation\n");
}

void destroyGL(ObjectData* data) {
    glDeleteBuffers(1, &data->VBO);
    glDeleteBuffers(1, &data->VAO);
    glDeleteBuffers(1, &data->EBO);
}

GLuint loadAndCompileShader(GLenum shaderType, const char *sourceCode)
{
    GLuint shader = glCreateShader(shaderType);
    if (shader)
    {
        glShaderSource(shader, 1, &sourceCode, NULL);
        glCompileShader(shader);

        GLint compiled = 0;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
        if (!compiled)
        {
            GLint infoLen = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
            if (infoLen)
            {
                char *buf = (char *)malloc(infoLen);
                if (buf)
                {
                    glGetShaderInfoLog(shader, infoLen, NULL, buf);
                    printf("Could not compile shader %s:", buf);
                    free(buf);
                }
                glDeleteShader(shader);
                shader = 0;
            }
        }
    }
    return shader;
}

GLuint linkShader(GLuint vertShaderID, GLuint fragShaderID)
{
    if (!vertShaderID || !fragShaderID)
    {
        return 0;
    }
    GLuint program = glCreateProgram();

    if (program)
    {
        glAttachShader(program, vertShaderID);
        glAttachShader(program, fragShaderID);

        glLinkProgram(program);
        GLint linkStatus = GL_FALSE;
        glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);

        if (linkStatus != GL_TRUE)
        {
            GLint bufLength = 0;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH,
                           &bufLength);
            if (bufLength)
            {
                char *buf = (char *)malloc(bufLength);
                if (buf)
                {
                    glGetProgramInfoLog(program, bufLength, NULL, buf);
                    printf("Could not link program:n%sn", buf);
                    free(buf);
                }
            }
            glDeleteProgram(program);
            program = 0;
        }
    }
    return program;
}

#ifdef __EMSCRIPTEN__
EM_BOOL resize_callback(int eventType, const EmscriptenUiEvent *e, void *rawState) {
	//app.width = (int) ((float)SCREEN_WIDTH/SCREEN_HEIGHT * e->windowInnerHeight);
    


    printf("Resize Callback %d, %d\n", getHeight(), getHeight());

    glViewport(0, 0, getWidth(), getHeight());
    

    return 0;
}
#endif