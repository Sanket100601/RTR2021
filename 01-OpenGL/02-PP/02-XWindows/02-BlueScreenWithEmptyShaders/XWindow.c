// standard header files
#include <stdio.h>  // for standard io
#include <stdlib.h> // for exit
#include <memory.h> // for memset

// X11 headers - Xserver 's 11 th version
#include <X11/Xlib.h>   // just like windows.h in win32
#include <X11/Xutil.h>  // for XvisualInfo
#include <X11/XKBlib.h> // for  keyboard

// OpenGL Header files
#include <GL/glew.h>
#include <GL/gl.h>  // for OpenGL functionality
#include <GL/glx.h> // for Bridging APIs

// Macros
#define WINWIDTH 800
#define WINHEIGHT 600

// global variables
Display *display = NULL;
XVisualInfo *visualInfo = NULL;
Colormap colormap;
Window window;
Bool fullscreen = False;
FILE *gpFile = NULL;

// OpenGL related global variable
typedef GLXContext (*glXCreateContextAttribsARBProc)(Display *, GLXFBConfig, GLXContext, Bool, const int *);
glXCreateContextAttribsARBProc glXCreateContextAttribsARB = NULL;
GLXFBConfig glxFBConfig;
GLXContext glxContext;

Bool bActiveWindow = False;
float anglePyramid = 0.0f;

// programmable pipeline related global variables
GLuint shaderProgramObject;

// entry point function
int main(void)
{
    // function declarations
    void uninitialize(void);
    void toggleFullscreen(void);
    // OpenGL
    int initialize(void);
    void resize(int, int);
    void draw(void);
    void update(void);

    // local variables
    int defaulScreen;
    int defaultDepth;
    // pp related
    GLXFBConfig *glxFBConfigs = NULL;
    GLXFBConfig bestglxFBConfig;
    XVisualInfo *tempXvisualInfo = NULL;
    int numFBConfigs;

    XSetWindowAttributes windowAttributes;
    int styleMask;
    Atom wm_delete_window_atom;
    XEvent event;
    KeySym keysym;
    char keys[26];

    int screenWidth;
    int screenHeight;

    static int winWidth;
    static int winHeight;

    static int frameBufferAttributes[] =
        {
            GLX_X_RENDERABLE, True,
            GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT,
            GLX_RENDER_TYPE, GLX_RGBA_BIT,
            GLX_X_VISUAL_TYPE, GLX_TRUE_COLOR,
            GLX_RED_SIZE, 8,
            GLX_GREEN_SIZE, 8,
            GLX_BLUE_SIZE, 8,
            GLX_ALPHA_SIZE, 8,
            GLX_STENCIL_SIZE, 8,
            GLX_DEPTH_SIZE, 24,
            GLX_DOUBLEBUFFER, True,
            None // every inline initialization in xlib terminated with zero i.e None
        };
    Bool bDone = False;

    // code

    // file io
    gpFile = fopen("logs.txt", "w");
    if (gpFile == NULL)
    {
        fprintf(gpFile, "File I/O Error - Creation of log file failed");
        exit(0);
    }
    else
    {
        fprintf(gpFile, "\nFile opened successfully ...\n");
    }

    // Open the display
    display = XOpenDisplay(NULL);
    if (display == NULL)
    {
        printf("ERROR: XOpenDisplay() failed\n");
        uninitialize();
        exit(1); // errorneous return
    }

    // get default screen from display
    defaulScreen = XDefaultScreen(display);
    // get default depth from display and default screen
    defaultDepth = XDefaultDepth(display, defaulScreen);
    // get XVisualInfo by using  XmatchVisualInfo() & do error checking

    /* default screen --- who have machine connected  --primary monitor in windows */

    // Windows choosePixelFormat
    glxFBConfigs = glXChooseFBConfig(display, defaulScreen, frameBufferAttributes, &numFBConfigs);
    if (glxFBConfigs == NULL)
    {
        fprintf(gpFile, "glXChooseFBConfig () failed");
        uninitialize();
        exit(0);
    }
    fprintf(gpFile, "number of framebuffer configs are = %d\n", numFBConfigs);

    // find best fb config among all

    int bestFrameBufferConfig = -1;
    int worstFrameBufferConfig = -1;
    int bestNumberOfSamples = -1;
    int worstNumberOfSamples = 999;
    for (int i; i < numFBConfigs; i++)
    {
        tempXvisualInfo = glXGetVisualFromFBConfig(display, glxFBConfigs[i]);
        if (tempXvisualInfo != NULL)
        {
            int samples, sampleBuffers;
            glXGetFBConfigAttrib(display, glxFBConfigs[i], GLX_SAMPLE_BUFFERS, &sampleBuffers);
            glXGetFBConfigAttrib(display, glxFBConfigs[i], GLX_SAMPLES, &samples);
            fprintf(gpFile, "visual info = 0x%lu found sampleBufffers =  %d , samples = %d \n", tempXvisualInfo->visualid, sampleBuffers, samples);

            if (bestFrameBufferConfig < 0 || sampleBuffers && samples > bestNumberOfSamples)
            {
                bestFrameBufferConfig = i;
                bestNumberOfSamples = samples;
            }

            if (worstFrameBufferConfig < 0 || !sampleBuffers || samples < worstNumberOfSamples)
            {
                worstFrameBufferConfig = i;
                worstNumberOfSamples = samples;
            }
        }
        XFree(tempXvisualInfo);
        tempXvisualInfo = NULL;
    }
    bestglxFBConfig = glxFBConfigs[bestFrameBufferConfig];
    glxFBConfig = bestglxFBConfig;

    // free array of glxFBConfigs
    XFree(glxFBConfigs);
    glxFBConfigs = NULL;

    visualInfo = glXGetVisualFromFBConfig(display, bestglxFBConfig);
    fprintf(gpFile, "\nvisual id of best visual info is 0x%lu  ", visualInfo->visualid);

    // 5) fill/init window attribute structure a;ong with it set colormap and event mask
    memset(&windowAttributes, 0, sizeof(XSetWindowAttributes));
    windowAttributes.border_pixel = 0;                                      // default
    windowAttributes.background_pixel = XBlackPixel(display, defaulScreen); // hbr background in win32
    windowAttributes.background_pixmap = 0;                                 // do you want to use picture NO
    windowAttributes.colormap = XCreateColormap(display,
                                                XRootWindow(display, visualInfo->screen),
                                                visualInfo->visual,
                                                AllocNone);                                            // 2 ^ 32 sized colored map
    windowAttributes.event_mask = ExposureMask | KeyPressMask | StructureNotifyMask | FocusChangeMask; // KeyPressMask == WM_CHAR , WM_KEYDOWN  , Exposure == WM_PAINT

    // 6) initialize global colormap by using colormap from window attribute
    colormap = windowAttributes.colormap;

    // 7) initialize windows style stylemask
    styleMask = CWBorderPixel | CWBackPixel | CWColormap | CWEventMask;

    // 8) Create the window by using XCreateWindow()
    window = XCreateWindow(display,
                           RootWindow(display, visualInfo->screen),
                           0,
                           0,
                           WINWIDTH,
                           WINHEIGHT,
                           0,
                           visualInfo->depth,
                           InputOutput,
                           visualInfo->visual,
                           styleMask,
                           &windowAttributes);

    if (!window)
    {
        printf("ERROR: XCreateWindow() failed\n");
        uninitialize();
        exit(1); // errorneous return
    }

    // 9) Give name to your window in the title bar
    XStoreName(display, window, "xWindows");

    // 10) Prepare our window to respond to
    // a) closing window by clicking on close button
    // b) closing window by clicking on close option in system menu by creating and setting window manager protocol atom
    wm_delete_window_atom = XInternAtom(display, "WM_DELETE_WINDOW", True);
    XSetWMProtocols(display, window, &wm_delete_window_atom, 1);

    // 11) show the window by XMapWindow()
    XMapWindow(display, window);

    // centering of window
    screenWidth = XWidthOfScreen(XScreenOfDisplay(display, defaulScreen));
    screenHeight = XHeightOfScreen(XScreenOfDisplay(display, defaulScreen));
    XMoveWindow(display, window, (screenWidth / 2 - WINWIDTH / 2), (screenHeight / 2 - WINHEIGHT / 2));

    initialize();
    // message loop
    while (bDone == False)
    {
        while (XPending(display)) // XPending : XWindows ::  PeekMessage : Windows
        {
            XNextEvent(display, &event); // XNextEvent is GetMessage in Windows
            switch (event.type)
            {
            case MapNotify: // analogous to WM_CREATE

                break;

            case FocusIn:
                bActiveWindow = True;
                break;

            case FocusOut:
                bActiveWindow = False;
                break;

            case KeyPress:
                keysym = XkbKeycodeToKeysym(display, event.xkey.keycode, 0, 0);
                switch (keysym)
                {
                case XK_Escape:
                    uninitialize();
                    exit(0);
                    break;
                    // default:
                    //     break;
                }

                XLookupString(&event.xkey, keys, sizeof(keys), NULL, NULL);
                switch (keys[0])
                {
                case 'F':
                case 'f':
                    if (fullscreen == False)
                    {
                        toggleFullscreen();
                        fullscreen = True;
                    }
                    else
                    {
                        toggleFullscreen();
                        fullscreen = False;
                    }
                    break;

                default:
                    break;
                }
                break;
            case ConfigureNotify:

                winWidth = event.xconfigure.width;
                winHeight = event.xconfigure.height;
                resize(winWidth, winHeight);
                break;

            case 33: // wm_delete_window_atom
                bDone = True;
                break;

            default:
                break;
            }
        }

        if (bActiveWindow == True)
        {

            // Update - Animation
            update();

            // Display
            draw();
        }
    }

    uninitialize();
    return (0);
}

void toggleFullscreen(void)
{
    // local variables
    Atom wm_current_state_atom;
    Atom wm_fullscreen_state_atom;
    XEvent event;

    // code
    wm_current_state_atom = XInternAtom(display, "_NET_WM_STATE", False);
    wm_fullscreen_state_atom = XInternAtom(display, "_NET_WM_STATE_FULLSCREEN", False);

    memset(&event, 0, sizeof(XEvent));
    event.type = ClientMessage;
    event.xclient.window = window;
    event.xclient.message_type = wm_current_state_atom;
    event.xclient.format = 32;
    event.xclient.data.l[0] = fullscreen ? 0 : 1;
    event.xclient.data.l[1] = wm_fullscreen_state_atom;

    //     extern Status XSendEvent(
    //     Display*		/* display */,
    //     Window		/* w */,
    //     Bool		/* propagate */,
    //     long		/* event_mask */,
    //     XEvent*		/* event_send */
    // );
    XSendEvent(display,
               RootWindow(display, visualInfo->screen),
               False,
               SubstructureNotifyMask,
               &event);
}

int initialize(void)
{
    // function declaration
    void resize(int, int);
    void uninitialize(void);
    void printGLInfo(void);

    // code
    glXCreateContextAttribsARB = (glXCreateContextAttribsARBProc)glXGetProcAddressARB((GLubyte *)"glXCreateContextAttribsARB");
    if (glXCreateContextAttribsARB == NULL)
    {
        fprintf(gpFile, " glXCreateContextAttribsARB() failed");
        uninitialize();
        exit(0);
    }
 // 4.6 madhil major version
    GLint contextAttribute[] =
        {GLX_CONTEXT_MAJOR_VERSION_ARB, 4,
         GLX_CONTEXT_MINOR_VERSION_ARB, 5,
         GLX_CONTEXT_PROFILE_MASK_ARB, GLX_CONTEXT_CORE_PROFILE_BIT_ARB,
         None};
    glxContext = glXCreateContextAttribsARB(display, glxFBConfig, NULL, True, contextAttribute);
    if (!glxContext)
    {
        GLint contextAttribute[] =
            {GLX_CONTEXT_MAJOR_VERSION_ARB, 4, // 4.6 madhil major version
             GLX_CONTEXT_MINOR_VERSION_ARB, 6,
             None};
        glxContext = glXCreateContextAttribsARB(display, glxFBConfig, NULL, True, contextAttribute);
        // we get topmost of fallback version hence not necessary we will get 1.0 if we asked for same
        fprintf(gpFile, "\ncan not support 4.6 hence falling back default version ");
    }
    else
    {
        fprintf(gpFile, "\nfound supported 4.6 version");
    }
    // checking whether hardware or software rendering supported

    if (!glXIsDirect(display, glxContext))
    {
        fprintf(gpFile, "\ndirect rendering i.e Hardware rendering not supported\n");
    }
    else
    {
        fprintf(gpFile, "\ndirect rendering i.e hardware rendering supported");
    }
    glXMakeCurrent(display, window, glxContext); // for which display(1st param ) which window (2nd) and which context(3rd) we have to make current
                                                 // clear the screen

    if (glewInit() != GLEW_OK)
        return -5;

    // here starts OpenGL Code

    // print OpenGL info
    printGLInfo();

    // 1) vertex shader code
    const GLchar *vertexShaderSourceCode =
        "#version 460 core"
        "\n"
        "void main(void)"
        "{"
        "}";

    // 2) create  shader object
    GLuint vertexShaderObject = glCreateShader(GL_VERTEX_SHADER);

    // 3) give shader code to shader object
    //  void PFNGLSHADERSOURCEPROC(GLuint shader, GLsizei count, const GLchar *const *string, const GLint *length)
    /*1) 3rd parameter is array of shaders but here only one shader hence 2nd param 1
      2) to give single member array give address
      3) if 3rd param arraythen 4th param is array [length of each string]
    */
    glShaderSource(vertexShaderObject, 1, (const GLchar **)&vertexShaderSourceCode, NULL);

    // 4) shader is code for GPU , to compile it >> inline compiler
    glCompileShader(vertexShaderObject);

    // 5) error checking if GPU gives error
    GLint status;
    GLint infoLogLength;
    char *log = NULL;

    // 5a - getting compilation status
    glGetShaderiv(vertexShaderObject, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE) // error exist
    {
        // 5b
        glGetShaderiv(vertexShaderObject, GL_INFO_LOG_LENGTH, &infoLogLength);
        if (infoLogLength > 0)
        {
            log = (char *)malloc(infoLogLength);
            if (log != NULL)
            {
                GLsizei written;
                // 5d
                glGetShaderInfoLog(vertexShaderObject, infoLogLength, &written, log);
                // 5e
                fprintf(gpFile, "Vertex Shader Compilation Log : %s\n", log);
                // 5f
                free(log);
                uninitialize();
                exit(0);
            }
        }
    }

    // fragment shader
    const GLchar *fragmentShaderSourceCode =
        "#version 460 core"
        "\n"
        "void main(void)"
        "{"
        "}";

    // create object
    GLuint fragmentShaderObject = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(fragmentShaderObject, 1, (const GLchar **)&fragmentShaderSourceCode, NULL);

    // compile
    glCompileShader(fragmentShaderObject);

    status = 0;
    infoLogLength = 0;
    log = NULL;

    glGetShaderiv(fragmentShaderObject, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE)
    {
        glGetShaderiv(fragmentShaderObject, GL_INFO_LOG_LENGTH, &infoLogLength);
        if (infoLogLength > 0)
        {
            log = (char *)malloc(infoLogLength);
            if (log != NULL)
            {
                GLsizei written;

                glGetShaderInfoLog(fragmentShaderObject, infoLogLength, &written, log);
                fprintf(gpFile, "Fragment Shader Compilation Log : %s\n", log);
                // 5f
                free(log);
                uninitialize();
                exit(0);
            }
        }
    }

    // shader program object
    // d 1
    shaderProgramObject = glCreateProgram();
    // d 2
    glAttachShader(shaderProgramObject, vertexShaderObject);
    glAttachShader(shaderProgramObject, fragmentShaderObject);

    // d 3
    glLinkProgram(shaderProgramObject);

    status = 0;
    infoLogLength = 0;
    log = NULL;

    glGetProgramiv(shaderProgramObject, GL_LINK_STATUS, &status);
    if (status == GL_FALSE)
    {
        glGetProgramiv(shaderProgramObject, GL_INFO_LOG_LENGTH, &infoLogLength);
        if (infoLogLength > 0)
        {
            log = (char *)malloc(infoLogLength);
            if (log != NULL)
            {
                GLsizei written;
                glGetProgramInfoLog(shaderProgramObject, infoLogLength, &written, log);
                fprintf(gpFile, "Shader Program Link Log error : %s\n", log);
                // 5f
                free(log);
                uninitialize();
                exit(0);
            }
        }
    }

    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glClearColor(0.0f, 0.0f, 1.0f, 1.0f); // blue and alpha are 1.0f

    // warm up resize call
    resize(WINWIDTH, WINHEIGHT);
    return (0);
}

void printGLInfo(void)
{
    // local variable declarations
    GLint numExtensions = 0;

    // code
    fprintf(gpFile, "OpenGL vendor : %s \n", glGetString(GL_VENDOR));
    fprintf(gpFile, "OpenGL renderer : %s \n", glGetString(GL_RENDERER));
    fprintf(gpFile, "OpenGL version : %s \n", glGetString(GL_VERSION));
    fprintf(gpFile, "GLSL version : %s \n", glGetString(GL_SHADING_LANGUAGE_VERSION));

    glGetIntegerv(GL_NUM_EXTENSIONS, &numExtensions);
    fprintf(gpFile, "Number of supported extensions : %d\n", numExtensions);
    for (int i = 0; i < numExtensions; i++)
    {
        fprintf(gpFile, " %s \n", glGetStringi(GL_EXTENSIONS, i));
    }
}

void resize(int width, int height)
{
    // code
    if (height == 0)
    {
        height = 1;
    }
    glViewport(0, 0, (GLsizei)width, (GLsizei)height);

    // glMatrixMode(GL_PROJECTION); // for seeing projection
    // glLoadIdentity();

//gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f); // above if is to avoid height 0 condition
    // void __stdcall gluPerspective(GLdouble fovy, GLdouble aspect, GLdouble zNear, GLdouble zFar)
    //  fovy - field of view eye
    //  aspect ratio
}
void draw(void)
{
    // code
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // use the shader program object
    glUseProgram(shaderProgramObject);

    // here write code for graphics

    // unuse the shader program object
    glUseProgram(0);

    glXSwapBuffers(display, window);
}

void uninitialize(void)
{
    // cod
    GLXContext currentContext;

    currentContext = glXGetCurrentContext();
    if (currentContext && currentContext == glxContext)
    {
        glXMakeCurrent(display, 0, 0);
    }

    if (glxContext)
    {
        glXDestroyContext(display, glxContext);
        glxContext = NULL;
    }

    if (visualInfo)
    {
        free(visualInfo);
        visualInfo = NULL;
    }

    if (window)
    {
        XDestroyWindow(display, window);
    }
    if (colormap)
    {
        XFreeColormap(display, colormap);
    }
    if (fullscreen == True)
    {
        toggleFullscreen();
        fullscreen = False;
    }
    if (display)
    {
        XCloseDisplay(display);
        display = NULL;
    }
    if (gpFile)
    {
        fprintf(gpFile, "Log file successfully closed");
        fclose(gpFile);
        gpFile = NULL;
    }
}

void update(void)
{
    // code
    anglePyramid = anglePyramid + 0.05f;
    if (anglePyramid >= 360.0f)
    {
        anglePyramid = anglePyramid - 360.0f;
    }
}
