// headers
#include <stdio.h>
#include <windows.h>

#include <gl/glew.h>

#include <cuda_gl_interop.h>
#include <cuda_runtime.h>

#include <iostream>

#include "OGL.h"
#include "vmath.h"

#include "sinewave.h"

#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "cudart.lib")

// macros
#define WIN_WIDTH 800
#define WIN_HEIGHT 600

using namespace vmath;

enum {
    AMC_ATTRIBUTE_POSITION = 0,
    AMC_ATTRIBUTE_COLOR,
    AMC_ATTRIBUTE_NORMAL,
    AMC_ATTRIBUTE_TEXTURE0,
};

// global function declarations
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void gerstnerWave(float time);
// global variables
HDC ghdc = NULL;
HGLRC ghrc = NULL;

bool gbFullscreen = false;
bool gbActiveWindow = false;

HWND ghwnd = NULL;
FILE* gpFile = NULL;

DWORD dwStyle;
WINDOWPLACEMENT wpPrev = { sizeof(WINDOWPLACEMENT) };

const int meshWidth = 256;
const int meshHeight = 256  ;

#define MY_ARRAY_SIZE (meshWidth * meshHeight * 4)
float pos[meshWidth][meshHeight][4];

GLuint gShaderProgramObject;

GLuint vao;
GLuint vbo;
GLuint vbo_gpu;
GLuint mvpMatrixUniform;
GLuint guiTexture_Water;
GLuint guiVBOQuad_Texcoord = 0;


mat4 perspectiveProjectionMatrix;
float animationTime = 0.0f;

struct cudaGraphicsResource* graphicsResource = NULL;
cudaError_t error;
bool bOnGPU = false;

// WinMain()
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    LPSTR lpszCmdLine, int iCmdShow) {
    // function declarations
    void initialize(void);
    void display(void);
    void update(void);

    // variable declarations
    bool bDone = false;
    WNDCLASSEX wndclass;
    HWND hwnd;
    MSG msg;
    TCHAR szAppName[] = TEXT("MyApp");

    // code
    // open file for logging
    if (fopen_s(&gpFile, "AMCLog.txt", "w") != 0) {
        MessageBox(NULL, TEXT("Cannot open AMCLog.txt file.."), TEXT("Error"),
            MB_OK | MB_ICONERROR);
        exit(0);
    }
    fprintf(gpFile, "==== Application Started ====\n");

    // initialization of WNDCLASSEX
    wndclass.cbSize = sizeof(WNDCLASSEX);
    wndclass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wndclass.cbClsExtra = 0;
    wndclass.cbWndExtra = 0;
    wndclass.lpfnWndProc = WndProc;
    wndclass.hInstance = hInstance;
    wndclass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(MYICON));
    wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wndclass.lpszClassName = szAppName;
    wndclass.lpszMenuName = NULL;
    wndclass.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(MYICON));

    // register above class
    RegisterClassEx(&wndclass);

    // get the screen size
    int width = GetSystemMetrics(SM_CXSCREEN);
    int height = GetSystemMetrics(SM_CYSCREEN);

    // create window
    hwnd = CreateWindowEx(
        WS_EX_APPWINDOW, szAppName, TEXT("CUDA + OpenGL | Sinewave"),
        WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_VISIBLE,
        (width / 2) - 400, (height / 2) - 300, WIN_WIDTH, WIN_HEIGHT, NULL, NULL,
        hInstance, NULL);

    ghwnd = hwnd;

    initialize();

    ShowWindow(hwnd, iCmdShow);
    SetForegroundWindow(hwnd);
    SetFocus(hwnd);

    // Game Loop!
    while (bDone == false) {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT)
                bDone = true;
            else {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
        else {
            if (gbActiveWindow == true) {
                // call update() here for OpenGL rendering
                update();
                // call display() here for OpenGL rendering
                display();
            }
        }
    }

    return ((int)msg.wParam);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam) {
    // function declaration
    void display(void);
    void resize(int, int);
    void uninitialize();
    void ToggleFullscreen(void);

    // code
    switch (iMsg) {

    case WM_SETFOCUS:
        gbActiveWindow = true;
        break;

    case WM_KILLFOCUS:
        gbActiveWindow = false;
        break;

    case WM_SIZE:
        resize(LOWORD(lParam), HIWORD(lParam));
        break;

    case WM_KEYDOWN:
        switch (wParam) {
        case VK_ESCAPE:
            DestroyWindow(hwnd);
            break;

        case 0x46:
        case 0x66:
            ToggleFullscreen();
            break;

        default:
            break;
        }
        break;

    case WM_CHAR:
        switch (wParam) {
        case 'c':
        case 'C':
            bOnGPU = false;
            break;

        case 'g':
        case 'G':
            bOnGPU = true;
            break;
        }
        break;

    case WM_ERASEBKGND:
        return (0);

    case WM_CLOSE:
        DestroyWindow(hwnd);
        break;

    case WM_DESTROY:
        uninitialize();
        PostQuitMessage(0);
        break;

    default:
        break;
    }

    return (DefWindowProc(hwnd, iMsg, wParam, lParam));
}

void ToggleFullscreen(void) {
    // local variables
    MONITORINFO mi = { sizeof(MONITORINFO) };

    // code
    if (gbFullscreen == false) {
        dwStyle = GetWindowLong(ghwnd, GWL_STYLE);
        if (dwStyle & WS_OVERLAPPEDWINDOW) {
            if (GetWindowPlacement(ghwnd, &wpPrev) &&
                GetMonitorInfo(MonitorFromWindow(ghwnd, MONITORINFOF_PRIMARY), &mi)) {
                SetWindowLong(ghwnd, GWL_STYLE, dwStyle & ~WS_OVERLAPPEDWINDOW);
                SetWindowPos(ghwnd, HWND_TOP, mi.rcMonitor.left, mi.rcMonitor.top,
                    mi.rcMonitor.right - mi.rcMonitor.left,
                    mi.rcMonitor.bottom - mi.rcMonitor.top,
                    SWP_NOZORDER | SWP_FRAMECHANGED);
            }
        }
        ShowCursor(FALSE);
        gbFullscreen = true;
    }
    else {
        SetWindowLong(ghwnd, GWL_STYLE, dwStyle | WS_OVERLAPPEDWINDOW);
        SetWindowPlacement(ghwnd, &wpPrev);
        SetWindowPos(ghwnd, HWND_TOP, 0, 0, 0, 0,
            SWP_NOZORDER | SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE |
            SWP_NOOWNERZORDER);
        ShowCursor(TRUE);
        gbFullscreen = false;
    }
}

bool LoadGLTexture(GLuint* puiTexture, TCHAR imageResourceID[])
{
    // variable declarations
    BITMAP bmp = { 0 };
    bool bResult = false;
    HBITMAP hbitmap = NULL;

    // code
    hbitmap = (HBITMAP)LoadImage(GetModuleHandle(NULL), imageResourceID, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION);
    if (hbitmap)
    {
        bResult = TRUE;
        GetObject(hbitmap, sizeof(BITMAP), &bmp);

        glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // for better performance
        glGenTextures(1, puiTexture);
        glBindTexture(GL_TEXTURE_2D, *puiTexture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

        // create texture
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, bmp.bmWidth, bmp.bmHeight, 0, GL_BGR, GL_UNSIGNED_BYTE, bmp.bmBits);
        //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, bmp.bmWidth, bmp.bmHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, bmp.bmBits);
        glGenerateMipmap(GL_TEXTURE_2D);

        glBindTexture(GL_TEXTURE_2D, 0);
        DeleteObject(hbitmap);
    }

    return bResult;
}

void initialize(void) {
    // function declarations
    void resize(int, int);
    void uninitialize(void);

    // variable declarations
    PIXELFORMATDESCRIPTOR pfd;
    int iPixelFormatIndex;

    // code

    //// C U D A /////////////////////////////////////////////////////////

    // cuda initialization
    int devCount;
    error = cudaGetDeviceCount(&devCount);
    if (error != cudaSuccess) {
        fprintf(gpFile, "cudaGetDeviceCount failed..\n");
        uninitialize();
        DestroyWindow(ghwnd);
    }
    else if (devCount == 0) {
        fprintf(gpFile, "No CUDA device detected..\n");
        uninitialize();
        DestroyWindow(ghwnd);
    }
    else {
        error = cudaSetDevice(0);
        if (error != cudaSuccess) {
            fprintf(gpFile, "cudaSetDevice failed..\n");
            uninitialize();
            DestroyWindow(ghwnd);
        }
    }

    //// Programable Pipeline ////////////////////////////////////////////

    ghdc = GetDC(ghwnd);

    ZeroMemory((void*)&pfd, sizeof(PIXELFORMATDESCRIPTOR));
    pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 32;
    pfd.cRedBits = 8;
    pfd.cGreenBits = 8;
    pfd.cBlueBits = 8;
    pfd.cAlphaBits = 8;
    pfd.cDepthBits = 32;

    iPixelFormatIndex = ChoosePixelFormat(ghdc, &pfd);
    if (iPixelFormatIndex == 0) {
        fprintf(gpFile, "ChoosePixelFormat() failed..\n");
        DestroyWindow(ghwnd);
    }

    if (SetPixelFormat(ghdc, iPixelFormatIndex, &pfd) == FALSE) {
        fprintf(gpFile, "SetPixelFormat() failed..\n");
        DestroyWindow(ghwnd);
    }

    ghrc = wglCreateContext(ghdc);
    if (ghrc == NULL) {
        fprintf(gpFile, "wglCreateContext() failed..\n");
        DestroyWindow(ghwnd);
    }

    if (wglMakeCurrent(ghdc, ghrc) == FALSE) {
        fprintf(gpFile, "wglMakeCurrent() failed..\n");
        DestroyWindow(ghwnd);
    }

    // glew initialization for programmable pipeline
    GLenum glew_error = glewInit();
    if (glew_error != GLEW_OK) {
        fprintf(gpFile, "glewInit() failed..\n");
        DestroyWindow(ghwnd);
    }

    // fetch OpenGL related details
    fprintf(gpFile, "OpenGL Vendor:   %s\n", glGetString(GL_VENDOR));
    fprintf(gpFile, "OpenGL Renderer: %s\n", glGetString(GL_RENDERER));
    fprintf(gpFile, "OpenGL Version:  %s\n", glGetString(GL_VERSION));
    fprintf(gpFile, "GLSL Version:    %s\n",
        glGetString(GL_SHADING_LANGUAGE_VERSION));

    // fetch OpenGL enabled extensions
    GLint numExtensions;
    glGetIntegerv(GL_NUM_EXTENSIONS, &numExtensions);

    fprintf(gpFile, "==== OpenGL Extensions ====\n");
    for (int i = 0; i < numExtensions; i++) {
        fprintf(gpFile, "  %s\n", glGetStringi(GL_EXTENSIONS, i));
    }
    fprintf(gpFile, "===========================\n\n");

    //// vertex shader
    // create shader
    GLuint gVertexShaderObject = glCreateShader(GL_VERTEX_SHADER);

    // provide source code to shader
    const GLchar* vertexShaderSourceCode =
        "#version 460 core"
        "\n"
        "in vec4 a_position;"
        "in vec2 a_texcoord;"
        "uniform mat4 u_mvpMatrix;"
        "out vec2 a_texcoord_out;"
        "void main(void)"
        "{"
        "gl_Position = u_mvpMatrix * a_position;"
        "a_texcoord_out = a_texcoord;"
        "}";

    glShaderSource(gVertexShaderObject, 1,
        (const GLchar**)&vertexShaderSourceCode, NULL);

    // compile shader
    glCompileShader(gVertexShaderObject);

    // compilation errors
    GLint iShaderCompileStatus = 0;
    GLint iInfoLogLength = 0;
    GLchar* szInfoLog = NULL;

    glGetShaderiv(gVertexShaderObject, GL_COMPILE_STATUS, &iShaderCompileStatus);
    if (iShaderCompileStatus == GL_FALSE) {
        glGetShaderiv(gVertexShaderObject, GL_INFO_LOG_LENGTH, &iInfoLogLength);
        if (iInfoLogLength > 0) {
            szInfoLog = (GLchar*)malloc(iInfoLogLength);
            if (szInfoLog != NULL) {
                GLsizei written;
                glGetShaderInfoLog(gVertexShaderObject, GL_INFO_LOG_LENGTH, &written,
                    szInfoLog);

                fprintf(gpFile, "Vertex Shader Compiler Info Log: \n%s\n", szInfoLog);
                free(szInfoLog);
                DestroyWindow(ghwnd);
            }
        }
    }

    //// fragment shader
    // create shader
    GLuint gFragmentShaderObject = glCreateShader(GL_FRAGMENT_SHADER);

    // provide source code to shader
    const GLchar* fragmentShaderSourceCode =
        fragmentShaderSourceCode = "#version 460 core"
        "\n"
        "in vec2 a_texcoord_out;"
        "uniform sampler2D u_textureSampler;"
        "out vec4 FragColor;"
        "void main(void)"
        "{"
        "FragColor = vec4(1.0f,1.0f,0.0f,0.3f);"
        "}";

    glShaderSource(gFragmentShaderObject, 1,
        (const GLchar**)&fragmentShaderSourceCode, NULL);

    // compile shader
    glCompileShader(gFragmentShaderObject);

    // compile errors
    iShaderCompileStatus = 0;
    iInfoLogLength = 0;
    szInfoLog = NULL;

    glGetShaderiv(gFragmentShaderObject, GL_COMPILE_STATUS,
        &iShaderCompileStatus);
    if (iShaderCompileStatus == GL_FALSE) {
        glGetShaderiv(gFragmentShaderObject, GL_INFO_LOG_LENGTH, &iInfoLogLength);
        if (iInfoLogLength > 0) {
            szInfoLog = (GLchar*)malloc(iInfoLogLength);
            if (szInfoLog != NULL) {
                GLsizei written;
                glGetShaderInfoLog(gFragmentShaderObject, GL_INFO_LOG_LENGTH, &written,
                    szInfoLog);

                fprintf(gpFile, "Fragment Shader Compiler Info Log: \n%s\n", szInfoLog);
                free(szInfoLog);
                DestroyWindow(ghwnd);
            }
        }
    }

    //// shader program
    // create
    gShaderProgramObject = glCreateProgram();

    // attach shaders
    glAttachShader(gShaderProgramObject, gVertexShaderObject);
    glAttachShader(gShaderProgramObject, gFragmentShaderObject);

    // pre-linking binding to vertex attribute
    glBindAttribLocation(gShaderProgramObject, AMC_ATTRIBUTE_POSITION,
        "a_position");
    glBindAttribLocation(gShaderProgramObject, AMC_ATTRIBUTE_POSITION,
        "a_texcoord");

    // link shader
    glLinkProgram(gShaderProgramObject);

    // linking errors
    GLint iProgramLinkStatus = 0;
    iInfoLogLength = 0;
    szInfoLog = NULL;

    glGetProgramiv(gShaderProgramObject, GL_LINK_STATUS, &iProgramLinkStatus);
    if (iProgramLinkStatus == GL_FALSE) {
        glGetProgramiv(gShaderProgramObject, GL_INFO_LOG_LENGTH, &iInfoLogLength);
        if (iInfoLogLength > 0) {
            szInfoLog = (GLchar*)malloc(iInfoLogLength);
            if (szInfoLog != NULL) {
                GLsizei written;
                glGetProgramInfoLog(gShaderProgramObject, GL_INFO_LOG_LENGTH, &written,
                    szInfoLog);

                fprintf(gpFile, ("Shader Program Linking Info Log: \n%s\n"), szInfoLog);
                free(szInfoLog);
                DestroyWindow(ghwnd);
            }
        }
    }

    // post-linking retrieving uniform locations
    mvpMatrixUniform = glGetUniformLocation(gShaderProgramObject, "u_mvpMatrix");

    const GLfloat quadTexCoords[] =
    {
        1.0f, 1.0f,
        0.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 0.0f
    };

    // create vao
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, MY_ARRAY_SIZE * sizeof(float), NULL,
        GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // vbo for cuda + opengl
    glGenBuffers(1, &vbo_gpu);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_gpu);
    glBufferData(GL_ARRAY_BUFFER, MY_ARRAY_SIZE * sizeof(float), NULL,
        GL_DYNAMIC_DRAW);

    //vbo texture
    glGenBuffers(1, &guiVBOQuad_Texcoord);
    glBindBuffer(GL_ARRAY_BUFFER, guiVBOQuad_Texcoord);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadTexCoords), quadTexCoords, GL_STATIC_DRAW);

    glVertexAttribPointer(AMC_ATTRIBUTE_TEXTURE0, 2, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(AMC_ATTRIBUTE_TEXTURE0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    LoadGLTexture(&guiTexture_Water, MAKEINTRESOURCE(IDBITMAP_WATER));

    // register our vbo with cuda graphics resource
    error = cudaGraphicsGLRegisterBuffer(&graphicsResource, vbo_gpu,
        cudaGraphicsMapFlagsWriteDiscard);
    if (error != cudaSuccess) {
        fprintf(gpFile, "cudaGraphicsGLRegisterBuffer failed..\n");
        uninitialize();
        DestroyWindow(ghwnd);
    }

    glBindVertexArray(0);

    for (int i = 0; i < meshWidth; i++) {
        for (int j = 0; j < meshHeight; j++) {
            for (int k = 0; k < 4; k++) {
                pos[i][j][k] = 0.0f;
            }
        }
    }

    // set clear color
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // set clear depth
    glClearDepth(1.0f);

    //// depth test
    //glEnable(GL_DEPTH_TEST);
    //glDepthFunc(GL_LEQUAL);

    // blend
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    perspectiveProjectionMatrix = mat4::identity();

    // warm-up resize call
    resize(WIN_WIDTH, WIN_HEIGHT);

    ToggleFullscreen();
}

void resize(int width, int height) {
    // code
    if (height == 0)
        height = 1;

    glViewport(0, 0, (GLsizei)width, (GLsizei)height);

    perspectiveProjectionMatrix =
        vmath::perspective(45.0f, (float)width / (float)height, 0.1f, 100.0f);
}

void display(void) {
    // function declarations
    void launchCPUKernel(float);
    void uninitialize(void);

    // code
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // start using OpenGL program object
    glUseProgram(gShaderProgramObject);

    // declaration of matrices
    mat4 translateMatrix;
    mat4 scaleMatrix;
    mat4 modelViewMatrix;
    mat4 modelViewProjectionMatrix;

    // intialize above matrices to identity
    translateMatrix = mat4::identity();
    modelViewMatrix = mat4::identity();
    scaleMatrix = mat4::identity();
    modelViewProjectionMatrix = mat4::identity();

    // transformations
    translateMatrix = translate(0.0f, -3.0f, -20.0f);
    scaleMatrix = scale(12.0f, 1.0f, 8.0f);
    modelViewMatrix = translateMatrix * scaleMatrix;    

    // do necessary matrix multiplication
    modelViewProjectionMatrix = perspectiveProjectionMatrix * modelViewMatrix;

    // send necessary matrices to shader in respective uniforms
    glUniformMatrix4fv(mvpMatrixUniform, 1, GL_FALSE, modelViewProjectionMatrix);

    // bind with vao (this will avoid many binding to vbo_vertex)
    glBindVertexArray(vao);

    if (bOnGPU) {
        // 1. map with the resource
        error = cudaGraphicsMapResources(1, &graphicsResource, 0);
        if (error != cudaSuccess) {
            fprintf(gpFile, "cudaGraphicsMapResource failed..\n");
            uninitialize();
            DestroyWindow(ghwnd);
        }

        // 2. get pointer to mapped resource
        float4* ppos = NULL;
        size_t byteCount;
        error = cudaGraphicsResourceGetMappedPointer((void**)&ppos, &byteCount,
            graphicsResource);
        if (error != cudaSuccess) {
            fprintf(gpFile, "cudaGraphicsResourceGetMappedPointer failed..\n");
            uninitialize();
            DestroyWindow(ghwnd);
        }

        // 3. launch the CUDA kernel
        launchCUDAKernel(ppos, meshWidth, meshHeight, animationTime);

        // 4. unmap the resource
        error = cudaGraphicsUnmapResources(1, &graphicsResource, 0);
        if (error != cudaSuccess) {
            fprintf(gpFile, "cudaGraphicsUnmapResources failed..\n");
            uninitialize();
            DestroyWindow(ghwnd);
        }

        glBindBuffer(GL_ARRAY_BUFFER, vbo_gpu);

    }
    else {

       //launchCPUKernel(animationTime);
        gerstnerWave(animationTime);

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, MY_ARRAY_SIZE * sizeof(float), pos,
            GL_DYNAMIC_DRAW);
    }

    glVertexAttribPointer(AMC_ATTRIBUTE_POSITION, 4, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(AMC_ATTRIBUTE_POSITION);

    // draw necessary scene
    //glDrawArrays(GL_POINTS, 0, meshWidth * meshHeight);
    glDrawArrays(GL_POINTS, 0, meshWidth * meshHeight);
    //glDrawArrays(GL_TRIANGLE_FAN, 4, meshWidth * meshHeight);
    //glDrawArrays(GL_TRIANGLE_FAN, 8, meshWidth * meshHeight);
    //glDrawArrays(GL_TRIANGLE_FAN, 12, meshWidth * meshHeight);
    //glDrawArrays(GL_TRIANGLE_FAN, 16, meshWidth * meshHeight);
    //glDrawArrays(GL_TRIANGLE_FAN, 20, meshWidth * meshHeight);


    // unbind vao
    glBindVertexArray(0);

    // stop using OpenGL program object
    glUseProgram(0);

    SwapBuffers(ghdc);
    animationTime += 0.1f;
}

void update(void) {
    // code
}

void uninitialize(void) {
    // code
    if (gbFullscreen == true) {
        dwStyle = GetWindowLong(ghwnd, GWL_STYLE);
        SetWindowLong(ghwnd, GWL_STYLE, dwStyle | WS_OVERLAPPEDWINDOW);
        SetWindowPlacement(ghwnd, &wpPrev);
        SetWindowPos(ghwnd, HWND_TOP, 0, 0, 0, 0,
            SWP_NOZORDER | SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE |
            SWP_NOOWNERZORDER);

        ShowCursor(TRUE);
    }

    if (vao) {
        glDeleteVertexArrays(1, &vao);
        vao = 0;
    }

    if (vbo) {
        glDeleteBuffers(1, &vbo);
        vbo = 0;
    }

    // destroy shader programs
    if (gShaderProgramObject) {
        GLsizei shaderCount;
        GLsizei i;

        glUseProgram(gShaderProgramObject);
        glGetProgramiv(gShaderProgramObject, GL_ATTACHED_SHADERS, &shaderCount);

        GLuint* pShaders = (GLuint*)malloc(shaderCount * sizeof(GLuint));
        if (pShaders) {
            glGetAttachedShaders(gShaderProgramObject, shaderCount, &shaderCount,
                pShaders);

            for (i = 0; i < shaderCount; i++) {
                // detach shader
                glDetachShader(gShaderProgramObject, pShaders[i]);

                // delete shader
                glDeleteShader(pShaders[i]);
                pShaders[i] = 0;
            }

            free(pShaders);
        }

        glDeleteProgram(gShaderProgramObject);
        gShaderProgramObject = 0;
        glUseProgram(0);
    }

    if (wglGetCurrentContext() == ghrc) {
        wglMakeCurrent(NULL, NULL);
    }

    if (ghrc) {
        wglDeleteContext(ghrc);
        ghrc = NULL;
    }

    if (ghdc) {
        ReleaseDC(ghwnd, ghdc);
        ghdc = NULL;
    }

    if (gpFile) {
        fprintf(gpFile, "==== Application Terminated ====\n");
        fclose(gpFile);
        gpFile = NULL;
    }
}

#if 0
void launchCPUKernel(float time) {
    for (int i = 0; i < meshWidth; i++) {
        for (int j = 0; j < meshHeight; j++) {
            for (int k = 0; k < 4; k++) {
                float u = i / (float)meshWidth;
                float v = j / (float)meshHeight;

                u = (u * 2.0) - 1.0;
                v = (v * 2.0) - 1.0;

                float freq = 10.0f;
                float w = sinf(freq * u + time) * cosf(freq * v + time) * 0.02f;

                if (k == 0)
                    pos[i][j][k] = u;
                if (k == 1)
                    pos[i][j][k] = w;
                if (k == 2)
                    pos[i][j][k] = v;
                if (k == 3)
                    pos[i][j][k] = 1.0f;
            }
        }
    }
}
#endif

void gerstnerWave(float time)
{
    for (int i = 0; i < meshWidth; i++)
    {
        for (int j = 0; j < meshHeight; j++)
        {
            for (int k = 0; k < 4; k++)
            {
                float u = i / (float)meshWidth;
                float v = j / (float)meshHeight;

                u = (u * 2.0)-1.0f;
                v = (v * 2.0)-1.0f;

                float freq = 10.0f;
                float w = 1 - abs(sinf(freq * v + time)) * abs(cosf(freq * u + time) * 0.1f);
               
                //float w = sinf(freq * u + time) * cosf(freq * v + time) * 0.2f;
                if (k == 0)
                    pos[i][j][k] = u;
                if (k == 1)
                    pos[i][j][k] = w;
                if (k == 2)
                    pos[i][j][k] = v;
                if (k == 3)
                    pos[i][j][k] = 1.0f;
            }
        }
    }
}

