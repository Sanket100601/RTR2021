//headers
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/XKBlib.h>
#include <X11/keysym.h>

#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glx.h>

#include "vmath.h"

//namespaces
using namespace std;
using namespace vmath;

//type declarations
enum
{
    AMC_ATTRIBUTE_POSITION = 0,
    AMC_ATTRIBUTE_COLOR,
    AMC_ATTRIBUTE_NORMAL,
    AMC_ATTRIBUTE_TEXCOORD
};

//type declarations
typedef GLXContext(*glXCreateContextAttribsARBProc)(Display*, GLXFBConfig, GLXContext, Bool, const int*);

//global variable declarations
Display* gpDisplay = NULL;
XVisualInfo* gpXVisualInfo = NULL;
Colormap      gColormap;
Window        gWindow;
GLXContext    gGLXContext;
GLXFBConfig   gGLXFBConfig;

glXCreateContextAttribsARBProc glXCreateContextAttribsARB = NULL;

int giWindowHeight = 600;
int giWindowWidth = 800;
bool gbFullscreen = false;

GLuint vertexShaderObject;
GLuint tesellationControlShaderObject;
GLuint tesellationEvaluationShaderObject;
GLuint fragmentShaderObject;
GLuint shaderProgramObject;

GLuint vao;
GLuint vbo_position;
GLuint mvpMatrixUniform;
GLuint numberOfSegmentsUniform;
GLuint numberOfStripsUniform;
GLuint lineColorUniform;

mat4 perspectiveProjectionMatrix;
unsigned int uiNumberOfSegments;

//entry-point function
int main(void)
{
    //function prototypes
    void CreateWindow(void);
    void ToggleFullscreen(void);
    void Initialize(void);
    void Resize(int, int);
    void Render(void);
    void uninitialize(void);

    //variable declarations
    int winWidth = giWindowWidth;
    int winHeight = giWindowHeight;
    bool bDone = false;

    //code
    CreateWindow();

    Initialize();

    //Message Loop
    XEvent event;
    KeySym keysym;

    while (bDone == false)
    {
        while (XPending(gpDisplay))
        {
            XNextEvent(gpDisplay, &event);
            switch (event.type)
            {
            case MapNotify:
                break;

            case KeyPress:
                keysym = XkbKeycodeToKeysym(gpDisplay, event.xkey.keycode, 0, 0);
                switch (keysym)
                {
                case XK_Escape:
                    bDone = true;
                    break;

                case XK_F:
                case XK_f:
                    if (gbFullscreen == false)
                    {
                        ToggleFullscreen();
                        gbFullscreen = true;
                    }
                    else
                    {
                        ToggleFullscreen();
                        gbFullscreen = false;
                    }
                    break;

                case XK_Up:
                    uiNumberOfSegments++;
                    if (uiNumberOfSegments > 30)
                        uiNumberOfSegments = 30;
                    break;

                case XK_Down:
                    uiNumberOfSegments--;
                    if (uiNumberOfSegments <= 0)
                        uiNumberOfSegments = 1;
                    break;

                default:
                    break;
                }
                break;

            case ButtonPress:
                switch (event.xbutton.button)
                {
                case 1:
                    break;

                case 2:
                    break;

                case 3:
                    break;

                default:
                    break;
                }
                break;

            case MotionNotify:
                break;

            case ConfigureNotify:
                winWidth = event.xconfigure.width;
                winHeight = event.xconfigure.height;
                Resize(winWidth, winHeight);
                break;

            case Expose:
                break;

            case DestroyNotify:
                break;

            case 33:
                bDone = true;
                break;

            default:
                break;
            }
        }

        Render();
    }

    uninitialize();

    return (0);
}

void CreateWindow(void)
{
    //function prototypes
    void uninitialize(void);

    //variable declarations
    XSetWindowAttributes winAttribs;
    GLXFBConfig* pGLXFBConfig = NULL;
    XVisualInfo* pTempXVisualInfo = NULL;
    int numFBConfigs = 0;

    int defaultScreen;
    int styleMask;

    static int frameBufferAttributes[] = { GLX_X_RENDERABLE,             True,
                                           GLX_DRAWABLE_TYPE,  GLX_WINDOW_BIT,
                                           GLX_RENDER_TYPE,      GLX_RGBA_BIT,
                                           GLX_X_VISUAL_TYPE,  GLX_TRUE_COLOR,
                                           GLX_STENCIL_SIZE,                8,
                                           GLX_DOUBLEBUFFER,             True,
                                           GLX_RED_SIZE,                    8,
                                           GLX_GREEN_SIZE,                  8,
                                           GLX_BLUE_SIZE,                   8,
                                           GLX_ALPHA_SIZE,                  8,
                                           GLX_DEPTH_SIZE,                 24,
                                           None };

    //code
    gpDisplay = XOpenDisplay(NULL);
    if (gpDisplay == NULL)
    {
        printf("ERROR : Unable To Open X Display.\nExiting Now...\n");
        uninitialize();
        exit(1);
    }

    defaultScreen = XDefaultScreen(gpDisplay);

    gpXVisualInfo = (XVisualInfo*)malloc(sizeof(XVisualInfo));
    if (gpXVisualInfo == NULL)
    {
        printf("ERROR : Unable To Allocate Memory For Visual Info.\nExiting Now...\n");
        uninitialize();
        exit(1);
    }

    pGLXFBConfig = glXChooseFBConfig(gpDisplay, defaultScreen, frameBufferAttributes, &numFBConfigs);
    if (pGLXFBConfig == NULL)
    {
        printf("ERROR : Unable To Find FBConfigs.\nExiting Now...\n");
        uninitialize();
        exit(1);
    }

    printf("Number of framebuffer configurations found = %d\n", numFBConfigs);

    int bestFrameBufferConfig = -1;
    int worstFrameBufferConfig = -1;
    int bestNumberOfSamples = -1;
    int worstNumberOfSamples = 999;
    int i;

    for (i = 0; i < numFBConfigs; i++)
    {
        pTempXVisualInfo = glXGetVisualFromFBConfig(gpDisplay, pGLXFBConfig[i]);
        if (pTempXVisualInfo)
        {
            int sampleBuffers;
            int samples;

            glXGetFBConfigAttrib(gpDisplay, pGLXFBConfig[i], GLX_SAMPLE_BUFFERS, &sampleBuffers);
            glXGetFBConfigAttrib(gpDisplay, pGLXFBConfig[i], GLX_SAMPLES, &samples);

            if ((bestFrameBufferConfig < 0) || sampleBuffers && (samples > bestNumberOfSamples))
            {
                bestFrameBufferConfig = i;
                bestNumberOfSamples = samples;
            }

            if ((worstFrameBufferConfig < 0) || !sampleBuffers || (samples < worstNumberOfSamples))
            {
                worstFrameBufferConfig = i;
                worstNumberOfSamples = samples;
            }

            printf("FBConfig %d SampleBuffers %d Samples %d Visualid = %ld\n", i, sampleBuffers, samples, pTempXVisualInfo->visualid);
        }
        else
        {
            printf("ERROR : Failed TO Get A Visual From FBConfig.\nExiting Now...\n");
            uninitialize();
            exit(1);
        }

        XFree(pTempXVisualInfo);
        pTempXVisualInfo = NULL;
    }

    gGLXFBConfig = pGLXFBConfig[bestFrameBufferConfig];
    XFree(pGLXFBConfig);
    pGLXFBConfig = NULL;

    gpXVisualInfo = glXGetVisualFromFBConfig(gpDisplay, gGLXFBConfig);
    if (gpXVisualInfo == NULL)
    {
        printf("ERROR : Failed To Get A Visual From FBConfig.\n");
        uninitialize();
        exit(1);
    }

    winAttribs.border_pixel = 0;
    winAttribs.background_pixmap = 0;
    winAttribs.colormap = XCreateColormap(gpDisplay,
        RootWindow(gpDisplay, gpXVisualInfo->screen),
        gpXVisualInfo->visual,
        AllocNone);
    winAttribs.background_pixel = BlackPixel(gpDisplay, defaultScreen);
    winAttribs.event_mask = ExposureMask | VisibilityChangeMask | ButtonPressMask |
        KeyPressMask | PointerMotionMask | StructureNotifyMask;

    styleMask = CWBorderPixel | CWBackPixel | CWEventMask | CWColormap;
    gColormap = winAttribs.colormap;

    gWindow = XCreateWindow(gpDisplay,
        RootWindow(gpDisplay, gpXVisualInfo->screen),
        0,
        0,
        giWindowWidth,
        giWindowHeight,
        0,
        gpXVisualInfo->depth,
        InputOutput,
        gpXVisualInfo->visual,
        styleMask,
        &winAttribs);
    if (!gWindow)
    {
        printf("ERROR : Failed To Create Main Window.\nExiting Now...\n");
        uninitialize();
        exit(1);
    }

    XStoreName(gpDisplay, gWindow, "xWindows");

    Atom windowManagerDelete = XInternAtom(gpDisplay, "WM_DELETE_WINDOW", True);
    XSetWMProtocols(gpDisplay, gWindow, &windowManagerDelete, 1);

    XMapWindow(gpDisplay, gWindow);
}

void ToggleFullscreen(void)
{
    //variable declaration
    Atom wm_state;
    Atom fullscreen;
    XEvent xev = { 0 };

    //code
    wm_state = XInternAtom(gpDisplay, "_NET_WM_STATE", False);
    memset(&xev, 0, sizeof(xev));

    xev.type = ClientMessage;
    xev.xclient.window = gWindow;
    xev.xclient.message_type = wm_state;
    xev.xclient.format = 32;
    xev.xclient.data.l[0] = gbFullscreen ? 0 : 1;

    fullscreen = XInternAtom(gpDisplay, "_NET_WM_STATE_FULLSCREEN", False);
    xev.xclient.data.l[1] = fullscreen;

    XSendEvent(gpDisplay,
        RootWindow(gpDisplay, gpXVisualInfo->screen),
        False,
        StructureNotifyMask,
        &xev);
}

void Initialize(void)
{
    //function declarations
    void Resize(int, int);
    void uninitialize(void);

    //code
    glXCreateContextAttribsARB = (glXCreateContextAttribsARBProc)glXGetProcAddressARB((const GLubyte*)"glXCreateContextAttribsARB");
    if (glXCreateContextAttribsARB == NULL)
    {
        printf("ERROR : Failed To Get glXCreateContextAttribsARB Address.\n");
        uninitialize();
        exit(1);
    }

    const int attribsCore[] = { GLX_CONTEXT_MAJOR_VERSION_ARB, 4,
                                GLX_CONTEXT_MINOR_VERSION_ARB, 5,
                                GLX_CONTEXT_PROFILE_MASK_ARB,  GL_CONTEXT_CORE_PROFILE_BIT,
                                None };

    gGLXContext = glXCreateContextAttribsARB(gpDisplay, gGLXFBConfig, 0, True, attribsCore);
    if (gGLXContext == NULL)
    {
        const int attribs[] = { GLX_CONTEXT_MAJOR_VERSION_ARB, 1,
                                GLX_CONTEXT_MINOR_VERSION_ARB, 1,
                                None };

        gGLXContext = glXCreateContextAttribsARB(gpDisplay, gGLXFBConfig, 0, True, attribs);
    }

    bool is_direct_context = glXIsDirect(gpDisplay, gGLXContext);
    if (is_direct_context)
    {
        printf("Rendering Context Is Hardware Rendering Context.\n");
    }
    else
    {
        printf("Rendering Context Is Software Rendering Context.\n");
    }

    glXMakeCurrent(gpDisplay, gWindow, gGLXContext);

    GLenum glew_error = glewInit();
    if (glew_error != GLEW_OK)
    {
        printf("ERROR : glewInit() failed.\n");
        uninitialize();
        exit(1);
    }

    //OpenGL related log
    printf("\nOpenGL Information\n");
    printf("OpenGL Vendor     : %s\n", glGetString(GL_VENDOR));
    printf("OpenGL Renderer   : %s\n", glGetString(GL_RENDERER));
    printf("OpenGL Version    : %s\n", glGetString(GL_VERSION));
    printf("GLSL Version      : %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

    //OpenGL enabled extensions
    GLint numExt;
    glGetIntegerv(GL_NUM_EXTENSIONS, &numExt);

    printf("OpenGL Extensions : \n");
    for (int i = 0; i < numExt; i++)
    {
        printf("%s\n", glGetStringi(GL_EXTENSIONS, i));
    }
    printf("\n\n");

    //setup render scene

    //--- Vertex Shader ---

    //create shader
    vertexShaderObject = glCreateShader(GL_VERTEX_SHADER);

    //shader source code
    const GLchar* vertexShaderSourceCode =
        "#version 450 core"                                         \
        "\n"                                                        \
        "in vec2 vPosition;"                                        \
        "void main(void)"                                           \
        "{"                                                         \
        "   gl_Position = vec4(vPosition, 0.0f, 1.0f);"             \
        "}";

    //provide source code to shader object
    glShaderSource(vertexShaderObject, 1, (const GLchar**)&vertexShaderSourceCode, NULL);

    //compile shader 
    glCompileShader(vertexShaderObject);

    //shader compilation error checking
    GLint infoLogLength = 0;
    GLint shaderCompiledStatus = 0;
    GLchar* szInfoLog = NULL;

    glGetShaderiv(vertexShaderObject, GL_COMPILE_STATUS, &shaderCompiledStatus);
    if (shaderCompiledStatus == GL_FALSE)
    {
        glGetShaderiv(vertexShaderObject, GL_INFO_LOG_LENGTH, &infoLogLength);
        if (infoLogLength > 0)
        {
            szInfoLog = (GLchar*)malloc(sizeof(GLchar) * infoLogLength);
            if (szInfoLog != NULL)
            {
                GLsizei written;
                glGetShaderInfoLog(vertexShaderObject, infoLogLength, &written, szInfoLog);
                printf("Vertex Shader Compilation Log : %s\n", szInfoLog);
                free(szInfoLog);
                uninitialize();
                exit(0);
            }
        }
    }

    printf("\nVertex Shader Compiled Successfully\n");

    //Tesselation Control Shader
    tesellationControlShaderObject = glCreateShader(GL_TESS_CONTROL_SHADER);

    //shader source code
    const GLchar* tesellationControlShaderSourceCode =
        "#version 450 core"                             \
        "\n"                                            \

        "layout(vertices = 4)out;"                      \
        "uniform int u_numberOfSegments;"               \
        "uniform int u_numberOfStrips;"                 \

        "void main(void)"                               \
        "{"                                             \
        "   gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;"      \
        "   gl_TessLevelOuter[0] = float(u_numberOfStrips);"                                \
        "   gl_TessLevelOuter[1] = float(u_numberOfSegments);"                              \
        "}";

    //provide source code to shader object 
    glShaderSource(tesellationControlShaderObject, 1, (const GLchar**)&tesellationControlShaderSourceCode, NULL);

    //compile shader
    glCompileShader(tesellationControlShaderObject);

    //shader compilation error checking
    glGetShaderiv(tesellationControlShaderObject, GL_COMPILE_STATUS, &shaderCompiledStatus);
    if (shaderCompiledStatus == GL_FALSE)
    {
        glGetShaderiv(tesellationControlShaderObject, GL_INFO_LOG_LENGTH, &infoLogLength);
        if (infoLogLength > 0)
        {
            szInfoLog = (GLchar*)malloc(sizeof(GLchar) * infoLogLength);
            if (szInfoLog != NULL)
            {
                GLsizei written;
                glGetShaderInfoLog(tesellationControlShaderObject, infoLogLength, &written, szInfoLog);
                printf("Tesselation Control Shader Compilation Log : %s\n", szInfoLog);
                free(szInfoLog);
                uninitialize();
                exit(0);
            }
        }
    }

    printf("Tesselation Control Shader Compiled Successfully\n");

    //Tesselation Evaluation Shader
    tesellationEvaluationShaderObject = glCreateShader(GL_TESS_EVALUATION_SHADER);

    //shader source code
    const GLchar* tesellationEvaluationShaderSourceCode =
        "#version 450 core"                             \
        "\n"                                            \

        "layout(isolines)in;"                           \
        "uniform mat4 u_mvpMatrix;"                     \

        "void main(void)"                               \
        "{"                                             \
        "   float tessCoord = gl_TessCoord.x;"          \

        "   vec3 p0 = gl_in[0].gl_Position.xyz;"        \
        "   vec3 p1 = gl_in[1].gl_Position.xyz;"        \
        "   vec3 p2 = gl_in[2].gl_Position.xyz;"        \
        "   vec3 p3 = gl_in[3].gl_Position.xyz;"        \

        "   vec3 p = (p0 * (1.0f - tessCoord) * (1.0f - tessCoord) * (1.0f - tessCoord)) + (p1 * 3.0f * tessCoord * (1.0f - tessCoord) * (1.0f - tessCoord)) + (p2 * 3.0f * tessCoord * tessCoord * (1.0f - tessCoord)) + (p3 * tessCoord * tessCoord * tessCoord);"    \
        "   gl_Position = u_mvpMatrix * vec4(p, 1.0);"   \
        "}";

    //provide source code to shader object 
    glShaderSource(tesellationEvaluationShaderObject, 1, (const GLchar**)&tesellationEvaluationShaderSourceCode, NULL);

    //compile shader
    glCompileShader(tesellationEvaluationShaderObject);

    //shader compilation error checking
    glGetShaderiv(tesellationEvaluationShaderObject, GL_COMPILE_STATUS, &shaderCompiledStatus);
    if (shaderCompiledStatus == GL_FALSE)
    {
        glGetShaderiv(tesellationEvaluationShaderObject, GL_INFO_LOG_LENGTH, &infoLogLength);
        if (infoLogLength > 0)
        {
            szInfoLog = (GLchar*)malloc(sizeof(GLchar) * infoLogLength);
            if (szInfoLog != NULL)
            {
                GLsizei written;
                glGetShaderInfoLog(tesellationEvaluationShaderObject, infoLogLength, &written, szInfoLog);
                printf("Tesselation Evaluation Shader Compilation Log : %s\n", szInfoLog);
                free(szInfoLog);
                uninitialize();
                exit(0);
            }
        }
    }

    printf("Tesselation Evaluation Shader Compiled Successfully\n");

    //--- Fragment Shader ---

    //create shader
    fragmentShaderObject = glCreateShader(GL_FRAGMENT_SHADER);

    //shader source code
    const GLchar* fragmentShaderSourceCode =
        "#version 450 core"                             \
        "\n"                                            \

        "out vec4 FragColor;"                           \
        "uniform vec4 u_lineColor;"                     \

        "void main(void)"                               \
        "{"                                             \
        "   FragColor = u_lineColor;"                   \
        "}";

    //provide source code to shader object 
    glShaderSource(fragmentShaderObject, 1, (const GLchar**)&fragmentShaderSourceCode, NULL);

    //compile shader
    glCompileShader(fragmentShaderObject);

    //shader compilation error checking
    glGetShaderiv(fragmentShaderObject, GL_COMPILE_STATUS, &shaderCompiledStatus);
    if (shaderCompiledStatus == GL_FALSE)
    {
        glGetShaderiv(fragmentShaderObject, GL_INFO_LOG_LENGTH, &infoLogLength);
        if (infoLogLength > 0)
        {
            szInfoLog = (GLchar*)malloc(sizeof(GLchar) * infoLogLength);
            if (szInfoLog != NULL)
            {
                GLsizei written;
                glGetShaderInfoLog(fragmentShaderObject, infoLogLength, &written, szInfoLog);
                printf("Fragment Shader Compilation Log : %s\n", szInfoLog);
                free(szInfoLog);
                uninitialize();
                exit(0);
            }
        }
    }

    printf("Fragment Shader Compiled Successfully\n");

    //--- Shader Program ---

    //create shader program
    shaderProgramObject = glCreateProgram();

    //attach vertex shader to shader program
    glAttachShader(shaderProgramObject, vertexShaderObject);

    //attach tesellation control shader to shader program
    glAttachShader(shaderProgramObject, tesellationControlShaderObject);

    //attach tesellation evaluation shader to shader program
    glAttachShader(shaderProgramObject, tesellationEvaluationShaderObject);

    //attach fragment shader to shader program
    glAttachShader(shaderProgramObject, fragmentShaderObject);

    //binding of shader program object with vertex shader position attribute
    glBindAttribLocation(shaderProgramObject, AMC_ATTRIBUTE_POSITION, "vPositon");

    //link shader program 
    glLinkProgram(shaderProgramObject);

    //shader linking error checking
    GLint shaderProgramLinkStatus = 0;
    glGetProgramiv(shaderProgramObject, GL_LINK_STATUS, &shaderProgramLinkStatus);
    if (shaderProgramLinkStatus == GL_FALSE)
    {
        glGetProgramiv(shaderProgramObject, GL_INFO_LOG_LENGTH, &infoLogLength);
        if (infoLogLength > 0)
        {
            szInfoLog = (GLchar*)malloc(sizeof(GLchar) * infoLogLength);
            if (szInfoLog != NULL)
            {
                GLsizei written;
                glGetProgramInfoLog(shaderProgramObject, infoLogLength, &written, szInfoLog);
                printf("Shader Program Link Log : %s\n", szInfoLog);
                uninitialize();
                exit(0);
            }
        }
    }

    printf("Shader Program Linked Successfully\n");

    //get MVP uniform location
    mvpMatrixUniform = glGetUniformLocation(shaderProgramObject, "u_mvpMatrix");
    numberOfSegmentsUniform = glGetUniformLocation(shaderProgramObject, "u_numberOfSegments");
    numberOfStripsUniform = glGetUniformLocation(shaderProgramObject, "u_numberOfStrips");
    lineColorUniform = glGetUniformLocation(shaderProgramObject, "u_lineColor");

    //vertex data
    const GLfloat vertices[] =
    {
        -1.0f, -1.0f,
        -0.5f, 1.0f,
        0.5f, -1.0f,
        1.0f, 1.0f
    };

    //setup vao and vbo
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo_position);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_position);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(AMC_ATTRIBUTE_POSITION, 2, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(AMC_ATTRIBUTE_POSITION);

    //unbind buffers
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glShadeModel(GL_SMOOTH);

    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    glEnable(GL_CULL_FACE);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    perspectiveProjectionMatrix = mat4::identity();
    uiNumberOfSegments = 1;

    Resize(giWindowWidth, giWindowHeight);
}

void Resize(int width, int height)
{
    //code
    if (height == 0)
        height = 1;

    glViewport(0, 0, (GLsizei)width, (GLsizei)height);

    perspectiveProjectionMatrix = vmath::perspective(45.0f, (float)width / (float)height, 0.1f, 100.0f);
}

void Render(void)
{
    //variable declarations
    mat4 modelViewMatrix;
    mat4 modelViewProjectionMatrix;
    mat4 translateMatrix;

    //code
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //start using OpenGL program object
    glUseProgram(shaderProgramObject);

    //OpenGL Drawing
    //set modelview, modelviewprojection & translate matrices to identity
    modelViewMatrix = mat4::identity();
    modelViewProjectionMatrix = mat4::identity();
    translateMatrix = mat4::identity();

    //translate modelview matrix
    translateMatrix = vmath::translate(0.0f, 0.0f, -4.0f);
    modelViewMatrix = translateMatrix;

    //multiply the modelview and perspective projection matrix to get modelviewprojection matrix 
    modelViewProjectionMatrix = perspectiveProjectionMatrix * modelViewMatrix;

    //pass above modelviewprojection matrix to the vertex shader in
    //"u_mvpMatrix" shader variable
    glUniformMatrix4fv(mvpMatrixUniform, 1, GL_FALSE, modelViewProjectionMatrix);

    glUniform1i(numberOfSegmentsUniform, uiNumberOfSegments);
    glUniform1i(numberOfStripsUniform, 1);
    glUniform4fv(lineColorUniform, 1, vmath::vec4(1.0f, 1.0f, 0.0f, 1.0f));

    //bind vao
    glBindVertexArray(vao);

    //draw
    glPatchParameteri(GL_PATCH_VERTICES, 4);
    glDrawArrays(GL_PATCHES, 0, 4);

    //unbind vao
    glBindVertexArray(0);

    //stop using OpenGL program object
    glUseProgram(0);

    glXSwapBuffers(gpDisplay, gWindow);
}

void uninitialize(void)
{
    //variable declarations
    GLXContext currentGLXContext;

    //code
    //release vao 
    if (vao)
    {
        glDeleteVertexArrays(1, &vao);
        vao = 0;
    }

    //release vbo
    if (vbo_position)
    {
        glDeleteBuffers(1, &vbo_position);
        vbo_position = 0;
    }

    //safe shader cleanup
    if (shaderProgramObject)
    {
        GLsizei shader_count;
        GLuint* p_shaders = NULL;

        glUseProgram(shaderProgramObject);
        glGetProgramiv(shaderProgramObject, GL_ATTACHED_SHADERS, &shader_count);

        p_shaders = (GLuint*)malloc(shader_count * sizeof(GLuint));
        memset((void*)p_shaders, 0, shader_count * sizeof(GLuint));

        glGetAttachedShaders(shaderProgramObject, shader_count, &shader_count, p_shaders);

        for (GLsizei i = 0; i < shader_count; i++)
        {
            glDetachShader(shaderProgramObject, p_shaders[i]);
            glDeleteShader(p_shaders[i]);
            p_shaders[i] = 0;
        }

        free(p_shaders);
        p_shaders = NULL;

        glDeleteProgram(shaderProgramObject);
        shaderProgramObject = 0;
        glUseProgram(0);
    }

    currentGLXContext = glXGetCurrentContext();
    if (currentGLXContext == gGLXContext)
    {
        glXMakeCurrent(gpDisplay, 0, 0);
    }

    if (gGLXContext)
    {
        glXDestroyContext(gpDisplay, gGLXContext);
    }

    if (gWindow)
    {
        XDestroyWindow(gpDisplay, gWindow);
    }

    if (gColormap)
    {
        XFreeColormap(gpDisplay, gColormap);
    }

    if (gpXVisualInfo)
    {
        free(gpXVisualInfo);
        gpXVisualInfo = NULL;
    }

    if (gpDisplay)
    {
        XCloseDisplay(gpDisplay);
        gpDisplay = NULL;
    }
}



