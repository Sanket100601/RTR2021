// Header Files
#include <windows.h>
#include <stdio.h>  // for FileIO functions
#include <stdlib.h> // for exit()
#include "OGL.h"

// OPENGL HEADER FILES
#include <GL/glew.h>	// THIS MUST BE BEFORE <GL/gl.h>
#include <GL/gl.h>
#include "vmath.h"	
#include "Sphere.h"

using namespace vmath;

// OpenGL Libraries 
#pragma comment(lib,"OpenGL32.lib")
#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "Sphere.lib")

// Macros
#define WIN_WIDTH 800
#define WIN_HEIGHT 600

#define FBO_WIDTH 512
#define FBO_HEIGHT 512


// global variable declarations
HWND ghwnd = NULL;
HDC ghdc = NULL;
HGLRC ghrc = NULL;
BOOL gbFullScreen = FALSE;
BOOL gbActiveWindow = FALSE;
FILE* gpFile = NULL;
int winWidth;
int winHeight;
BOOL bFBOResult = false;

// Programmable pipeline related Global variables
GLuint shaderProgramObject;

enum
{
	AMC_ATTRIBUTE_POSITION = 0,
	AMC_ATTRIBUTE_COLOR,
	AMC_ATTRIBUTE_NORMAL,
	AMC_ATTRIBUTE_TEXTURE0
};

GLuint vao_cube;
GLuint vbo_cube_position;
GLuint vbo_cube_texcoord;
GLuint mvpMatrixUniform;

GLuint textureSamplerUniform;

GLfloat angleCube = 0.0f;

mat4 perspectiveProjectionMatrix;
mat4 perspectiveProjectionMatrix_sphere;

// FBO related Global variables
GLuint fbo;	// Frame Buffer Object
GLuint rbo;	// Render Buffer Object
GLuint fbo_texture;
BOOL bLight = FALSE;

// Texture scene global variables
GLuint shaderProgramObject_sphere;

GLuint vao_sphere;				// Vertex Array Object
GLuint vbo_sphere_position;	// Vertex Buffer Object
GLuint vbo_sphere_normal;
GLuint vbo_sphere_element;

// Uniforms
GLuint modelMatrixUniform_sphere;
GLuint viewMatrixUniform_sphere;
GLuint projectionMatrixUniform_sphere;

float sphere_vertices[1146];
float sphere_normals[1146];
float sphere_textures[764];
unsigned short sphere_elements[2280];

unsigned int gNumVertices_sphere;
unsigned int gNumElements_sphere;

GLuint laUniform_sphere[3];
GLuint ldUniform_sphere[3];
GLuint lsUniform_sphere[3];
GLuint lightPositionUniform_sphere[3];

GLuint kaUniform_sphere;
GLuint kdUniform_sphere;
GLuint ksUniform_sphere;
GLuint materialShininessUniform_sphere;

GLuint lightingEnabledUniform_sphere;

GLfloat lightAmbientZero_sphere[] = { 0.0f, 0.0f, 0.0f, 1.0f };
GLfloat lightDiffuseZero_sphere[] = { 1.0f, 0.0f, 0.0f, 0.0f, 1.0f };	// Red
GLfloat specularLightZero_sphere[] = { 1.0f, 0.0f, 0.0f, 1.0f };	// Red
GLfloat lightPositionZero_sphere[] = { 0.0f, 0.0f, 0.0f, 1.0f };

GLfloat lightAmbientOne_sphere[] = { 0.0f, 0.0f, 0.0f, 1.0f };
GLfloat lightDiffuseOne_sphere[] = { 0.0f, 1.0f, 0.0f, 0.0f, 1.0f };	// Green
GLfloat specularLightOne_sphere[] = { 0.0f, 1.0f, 0.0f, 1.0f };	// Green
GLfloat lightPositionOne_sphere[] = { 0.0f, 0.0f, 0.0f, 1.0f };

GLfloat lightAmbientTwo_sphere[] = { 0.0f, 0.0f, 0.0f, 1.0f };
GLfloat lightDiffuseTwo_sphere[] = { 0.0f, 0.0f, 1.0f, 0.0f, 1.0f };	// Blue
GLfloat specularLightTwo_sphere[] = { 0.0f, 0.0f, 1.0f, 1.0f };	// Blue
GLfloat lightPositionTw_sphereo[] = { 0.0f, 0.0f, 0.0f, 1.0f };

GLfloat materialAmbient_sphere[] = { 0.0f, 0.0f, 0.0f, 1.0f };
GLfloat materialDiffuse_sphere[] = { 1.0f, 1.0f, 1.0f , 1.0f };
GLfloat materialSpecular_sphere[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat materialShininess_sphere = 50.0f;

GLfloat lightAngleZero_sphere = 0.0f;
GLfloat lightAngleOne_sphere = 0.0f;
GLfloat lightAngleTwo_sphere = 0.0f;

GLfloat lightRotatingAngle_sphere;

struct Light
{
	vec4 lightAmbient;
	vec4 lightDiffuse;
	vec4 lightSpecular;
	vec4 lightPosition;
};

Light lights_sphere[3];


// global function declarations
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

// Entry point function
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow)
{
	// function delcarations
	int initialise (void);
	void display (void);
	void update (void);
	void uninitialize (void);
	// variable declarations
	WNDCLASSEX wndclass;
	HWND hwnd;
	MSG msg;
	TCHAR szAppName[] = TEXT("My Window");
	BOOL bDone = FALSE;
	int iRetVal = 0;
	int width, height;
	int x, y;

	// code
	if (fopen_s(&gpFile, "Log.txt", "w") != 0)
	{
		MessageBox(NULL, TEXT("Creation Of Log File Failed. Exiting..."), TEXT("File I/O Error"), MB_OK);
		exit(0);
	}
	else
	{
		fprintf(gpFile, "Log File Is Sucessfully Created !!!\n");
	}
	width = GetSystemMetrics(SM_CXSCREEN);
	height = GetSystemMetrics(SM_CYSCREEN);

	x = (width - WIN_WIDTH) / 2;
	y = (height - WIN_HEIGHT) / 2;

	// Initialization of class structure
	wndclass.cbSize = sizeof(WNDCLASSEX);
	wndclass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.lpfnWndProc = WndProc;
	wndclass.hInstance = hInstance;
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(MyIcon));
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.lpszClassName = szAppName;
	wndclass.lpszMenuName = NULL;
	wndclass.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(MyIcon));

	// Resgistry above class
	RegisterClassEx(&wndclass);

	// create the window
	hwnd = CreateWindowEx(WS_EX_APPWINDOW,
		szAppName,
		TEXT(" OGL Window"),
		WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_VISIBLE,
		x,
		y,
		WIN_WIDTH,
		WIN_HEIGHT,
		NULL,
		NULL,
		hInstance,
		NULL);
	ghwnd = hwnd;

	// initialise
	iRetVal = initialise();
	if (iRetVal == -1)
	{
		fprintf(gpFile, "Choose Pixel Format Failed !\n");
		uninitialize();
	}
	else if (iRetVal == -2)
	{
		fprintf(gpFile, "Set Pixel Format Failed !\n");
		uninitialize();
	}
	else if (iRetVal == -3)
	{
		fprintf(gpFile, "Create OpenGL Context Failed !\n");
		uninitialize();
	}
	
	else if (iRetVal == -5)
	{
		fprintf(gpFile, "glewInit() Failed !\n");
		uninitialize();
	}
	else if (iRetVal == -6)
	{
		fprintf(gpFile, "Create FBO Failed !\n");
		uninitialize();
	}

	else
	{
		fprintf(gpFile, "Initialize function Successfull\n");
	}

	// show window
	ShowWindow(hwnd, iCmdShow);

	// Foregrounding and focusing window
	SetForegroundWindow(hwnd); 
	SetFocus(hwnd);

	// Game Loop
	while (bDone == FALSE)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				bDone = TRUE;
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			if (gbActiveWindow == TRUE)
			{
				// render the scene
				display();

				// update the scene
				update();

			}
		}
	}
	uninitialize();
	return ((int)msg.wParam);
}

// CALLBACK FUNCTION
LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	// function declarations
	void ToggleFullScreen(void);
	void resize(int, int);


	// Code
	switch (iMsg)
	{
	
	case WM_SETFOCUS:
		gbActiveWindow = TRUE;
		break;
	case WM_KILLFOCUS:
		gbActiveWindow = FALSE;
		break;
	case WM_ERASEBKGND:

		return 0;
	case WM_CHAR:
		switch (wParam)
		{
		case 'F':
		case 'f':
			ToggleFullScreen();
			break;

		case 'L':
		case 'l':
			if (bLight == FALSE)
			{
				bLight = TRUE;
			}
			else
			{
				bLight = FALSE;
			}
			break;

		default:
			break;
		}
		break;

	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
		{
			if (gpFile)
			{
				fprintf(gpFile, "Log File Is Sucessfully Closed !!!\n");
				fclose(gpFile);
				gpFile = NULL;
			}
			//PostQuitMessage(0);
			DestroyWindow(hwnd);
		}
			
		break;
	case WM_SIZE:
		resize(LOWORD(lParam), HIWORD(lParam));
		break;
	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;
	case WM_DESTROY:
	
		PostQuitMessage(0);
		break;
	default:
		break;
	}

	return (DefWindowProc(hwnd, iMsg, wParam, lParam));
}

void ToggleFullScreen(void)
{
	// varible declarations
	static DWORD dwStyle;
	static WINDOWPLACEMENT wp;
	MONITORINFO mi;


	// code
	wp.length = sizeof(WINDOWPLACEMENT);

	if (gbFullScreen == FALSE)
	{
		dwStyle = GetWindowLong(ghwnd, GWL_STYLE);
		if (dwStyle & WS_OVERLAPPEDWINDOW)
		{
			mi.cbSize = sizeof(MONITORINFO);


			if (GetWindowPlacement(ghwnd, &wp) && GetMonitorInfo(MonitorFromWindow(ghwnd, MONITORINFOF_PRIMARY), &mi))
			{
				SetWindowLong(ghwnd, GWL_STYLE, dwStyle & ~WS_OVERLAPPEDWINDOW);
				SetWindowPos(ghwnd, HWND_TOP, mi.rcMonitor.left, mi.rcMonitor.top, mi.rcMonitor.right - mi.rcMonitor.left, mi.rcMonitor.bottom - mi.rcMonitor.top, SWP_NOZORDER | SWP_FRAMECHANGED);

			}
			ShowCursor(FALSE);
			gbFullScreen = TRUE;
		}

	}
	else
	{
		SetWindowLong(ghwnd, GWL_STYLE, dwStyle | WS_OVERLAPPEDWINDOW);
		SetWindowPlacement(ghwnd, &wp);
		SetWindowPos(ghwnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOOWNERZORDER | SWP_NOZORDER | SWP_FRAMECHANGED);

		ShowCursor(TRUE);
		gbFullScreen = FALSE;
	}
}

int initialise(void)
{
	// function declarations
	void resize(int, int);
	void printGLInfo(void);
	void uninitialize(void);
	BOOL createFBO(GLint, GLint);
	int initialise_sphere(int, int);

	// variable declarations
	PIXELFORMATDESCRIPTOR pfd;
	int iPixelFormatIndex = 0;

	// code
	// init of PIXELFORMATDESCRIPTOR structure
	ZeroMemory(&pfd, sizeof(PIXELFORMATDESCRIPTOR));
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cDepthBits = 32;
	pfd.cColorBits = 32;
	pfd.cRedBits = 8;
	pfd.cGreenBits = 8;
	pfd.cBlueBits = 8;
	pfd.cAlphaBits = 8;
	pfd.cDepthBits = 32;	// 24 also can be done

	// Get DC
	ghdc = GetDC(ghwnd);

	// Choose Pixel Format
	iPixelFormatIndex = ChoosePixelFormat(ghdc, &pfd);
	if (iPixelFormatIndex == 0)
	{
		return -1;
	}
	// set chosen pixel format
	// CODE
	if (SetPixelFormat(ghdc, iPixelFormatIndex, &pfd) == FALSE)
		return -2;
	// Create OpenGL Rendering Context
	ghrc = wglCreateContext(ghdc);
	if (ghrc == NULL)
		return -3;
	// make rendering contex as current contex
	if (wglMakeCurrent(ghdc, ghrc) == FALSE)
		return -4;

	// GLEW initialization
	if(glewInit() != GLEW_OK )
	{
		return(-5);
	}

	// Print OpenGL Info
	printGLInfo();
	
	// Vertex Shader
	const GLchar *vertexShaderSourceCode = 
	"#version 460 core" \
	"\n" \
	"in vec4 a_position;" \
	"in vec2 a_texcoord;" \
	"uniform mat4 u_mvpMatrix;" \
	"out vec2 a_texcoord_out;" \
	"void main(void)" \
	"{" \
		"gl_Position = u_mvpMatrix * a_position;" \
		"a_texcoord_out = a_texcoord;" \
	"}"; 
	
	// Create Shader
	GLuint vertexShaderObject = glCreateShader(GL_VERTEX_SHADER);
	
	// Give the shader source code to this code
	glShaderSource(vertexShaderObject, 1, (const GLchar **)&vertexShaderSourceCode, NULL);
	
	// Compile the shader code to make it GPU understandable from human understandable
	glCompileShader(vertexShaderObject);
	
	// Error checking of shader compilation
	GLint status;
	GLint infoLogLength;
	char *log = NULL;
	
	// Getting compilation status
	glGetShaderiv(vertexShaderObject, GL_COMPILE_STATUS, &status);
	
	if(status == GL_FALSE)	// GL_FALSE means there is error
	{
		// Getting length of log of compilation status
		glGetShaderiv(vertexShaderObject, GL_INFO_LOG_LENGTH, &infoLogLength);
		if(infoLogLength > 0)
		{
			// Allocate enough memory to the buffer to hold the compilation log
			log = (char *)malloc(infoLogLength);
			if(log != NULL)
			{
				GLsizei written;
				
				// Get the compilation log into thtis allocated buffer
				glGetShaderInfoLog(vertexShaderObject, infoLogLength, &written, log);
				
				// Dsiplay the log/contents of buffer
				fprintf(gpFile, "Vertex Shader complitation log: %s\n", log);
				
				// Free the allocated buffer
				free(log);
				
				// Exit the application due to error
				uninitialize();
			}
		}
	}
	
	// Fragment Shader
	const GLchar *fragmentShaderSourceCode = 
	"#version 460 core" \
	"\n" \
	"in vec2 a_texcoord_out;" \
	"uniform sampler2D u_textureSampler;" \
	"out vec4 FragColor;" \
	"void main(void)" \
	"{" \
		"FragColor = texture(u_textureSampler,a_texcoord_out);" \
	"}"; 
	
	// Create Shader
	GLuint fragmentShaderObject = glCreateShader(GL_FRAGMENT_SHADER);
	
	// Give the shader source code to this code
	glShaderSource(fragmentShaderObject, 1, (const GLchar **)&fragmentShaderSourceCode, NULL);
	
	// Compile the shader code to make it GPU understandable from human understandable
	glCompileShader(fragmentShaderObject);
	
	// Error checking of the shader compilation
	status = 0;
	infoLogLength = 0;
	log = NULL;
	
	// Getting compilation status
	glGetShaderiv(fragmentShaderObject, GL_COMPILE_STATUS, &status);
	
	if(status == GL_FALSE)
	{
		glGetShaderiv(fragmentShaderObject, GL_INFO_LOG_LENGTH, &infoLogLength);
		if(infoLogLength > 0 )
		{
			// Allocate enough memory to the buffer to hold the comiplation log
			log = (char *)malloc(infoLogLength);
			if(log != NULL)
			{
				GLsizei written;
				
				// Get the compilation log into this allocated buffer
				glGetShaderInfoLog(fragmentShaderObject, infoLogLength, &written, log);
				
				// Display the log/content of buffer
				fprintf(gpFile, "Fragment shader compilation log: %s", log);
				
				// Free the allocated buffer
				free(log);
				
				// Exit the application due to the error
				uninitialize();
			}
		}
	}
	
	// Create Shader program object
	shaderProgramObject = glCreateProgram();
	
	// Attach desired shaders to this program object
	glAttachShader(shaderProgramObject, vertexShaderObject);
	glAttachShader(shaderProgramObject, fragmentShaderObject);

	// Pre-link binding of shader program object with Vertex attributes
	glBindAttribLocation(shaderProgramObject, AMC_ATTRIBUTE_POSITION, "a_position");
	glBindAttribLocation(shaderProgramObject, AMC_ATTRIBUTE_TEXTURE0, "a_texcoord");

	// Link shader program object
	glLinkProgram(shaderProgramObject);
	
	// Link error checking
	status = 0 ;
	infoLogLength = 0;
	log = NULL;
	
	// Getting link status
	glGetProgramiv(shaderProgramObject, GL_LINK_STATUS, &status);
	
	if(status == GL_FALSE)
	{
		glGetProgramiv(shaderProgramObject, GL_INFO_LOG_LENGTH, &infoLogLength);
		if(infoLogLength > 0)
		{
			// Allocate enough memory to the buffer
			log = (char *)malloc(infoLogLength);
			if(log != NULL)
			{
				GLsizei written;
				
				// Get the link log to the allocated buffer
				glGetProgramInfoLog(shaderProgramObject, infoLogLength, &written, log);
				
				// Display the log/ content of buffer
				fprintf(gpFile, "Shader Program Link log : %s", log);
				
				// Free the buffer
				free(log);
				
				// Exit the application
				uninitialize();
			}
		}
	}

	// Post link retrieving/getting uniform location from shader program object
	glGetUniformLocation(shaderProgramObject, "u_mvpMatrix");
	textureSamplerUniform = glGetUniformLocation(shaderProgramObject, "u_textureSampler");

	// vao and vbo_pyramid_position related steps
	// Declare vertex data arrays
	const GLfloat cubePosition[] =
	{
		// top
		1.0f, 1.0f, -1.0f,
		-1.0f, 1.0f, -1.0f,
		-1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,

		// bottom
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,

		 // front
		 1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f, -1.0f, 1.0f,
		 1.0f, -1.0f, 1.0f,

		 // back
		 1.0f, 1.0f, -1.0f,
		-1.0f, 1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,

		 // right
		 1.0f, 1.0f, -1.0f,
		 1.0f, 1.0f, 1.0f,
		 1.0f, -1.0f, 1.0f,
		 1.0f, -1.0f, -1.0f,

		 // left
		-1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f, 1.0f,

	};

	const GLfloat cubeTexcoord[] =
	{
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,

		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,

		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,

		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,

		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,

		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,
	};

	


	// vao cube
	glGenVertexArrays(1, &vao_cube);

	// Bind with respective array object
	glBindVertexArray(vao_cube);

	// Recording 
	// Create vertex data buffer vbo for position
	glGenBuffers(1, &vbo_cube_position);

	// Bind with vertex data buffer
	glBindBuffer(GL_ARRAY_BUFFER, vbo_cube_position);

	// Create storage for buffer data for a particular target
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubePosition), cubePosition, GL_STATIC_DRAW);

	// Specify where, how much, which buffer data to consider as vertex data arrays
	glVertexAttribPointer(AMC_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	// Enable the respective buffer binding vertex array
	glEnableVertexAttribArray(AMC_ATTRIBUTE_POSITION);

	// Unbind the vao respective vertex data buffer
	glBindBuffer(GL_ARRAY_BUFFER, 0);


	// vbo for color
	glGenBuffers(1, &vbo_cube_texcoord);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_cube_texcoord);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeTexcoord), cubeTexcoord, GL_STATIC_DRAW);
	glVertexAttribPointer(AMC_ATTRIBUTE_TEXTURE0, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(AMC_ATTRIBUTE_TEXTURE0);

	// vbo for color
	glVertexAttrib3f(AMC_ATTRIBUTE_TEXTURE0, 0.0f, 0.0f, 1.0f);

	// Unbind with vbo the respective vertex array object and stop recording
	glBindVertexArray(0);


	// Here Starts OpenGL Code

	// Calling Load GL Texture method
	

	glEnable(GL_TEXTURE_2D);

	// Depth Related Changes. 2 lines are not useful in PP, depricated in PP - >glShadeModel(GL_SMOOTH), glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	// Clear the Screen using Blue Color
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	// Iniitialize needed matrices using vmath
	perspectiveProjectionMatrix = mat4::identity();

	// warm-up
	resize(WIN_WIDTH, WIN_HEIGHT);

	// FBO code
	bFBOResult = createFBO(FBO_WIDTH, FBO_HEIGHT);

	// Here there should be error checking
	int iRetval;
	if (bFBOResult == TRUE)
	{
		iRetval = initialise_sphere(FBO_WIDTH, FBO_HEIGHT);
	}
	else
	{
		fprintf(gpFile, "Create FBO failed...\n");
		return -6;
	}

	return 0;
}

BOOL createFBO(GLint textureWidth, GLint textureHeight)
{
	// Code
	// 1. Check available render buffer size
	int maxRenderbufferSize;
	glGetIntegerv(GL_MAX_RENDERBUFFER_SIZE, &maxRenderbufferSize);
	if (maxRenderbufferSize < textureWidth || maxRenderbufferSize < textureHeight)
	{
		fprintf(gpFile, "Insufficient render buffer size.\n");
		return false;
	}

	// 2. Create Frame Buffer Object
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	// 3. Create Render Buffer Object
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);

	// 4. Where to keep this render buffer and what will be the format of render buffer - Sotrage and format of render buffer. Here - GL_DEPTH_COMPONENT16 - this has nothing to do with depth, it is only used as its value is 16 and 16 is also suitiable for mobile
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, textureWidth, textureHeight);

	// 5. Create empty texture for upcoming target scene
	glGenTextures(1, &fbo_texture);
	glBindTexture(GL_TEXTURE_2D, fbo_texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, NULL);

	// 6. Give above texture to fbo
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fbo_texture, 0);

	// 7. Give render buffer object to fbo
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo);

	// 8. Check whether frame buffer created successfully or not
	GLenum result = glCheckFramebufferStatus(GL_FRAMEBUFFER);

	if (result != GL_FRAMEBUFFER_COMPLETE)
	{
		fprintf(gpFile, "Frame buffer is not complete.\n");
		return false;
	}

	// 9. Unbind frame buffer - doing this will unbind texture and rbo
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return true;
}

int initialise_sphere(int width, int height)
{
	// function declarations
	void resize_sphere(int, int);
	void uninitialize_sphere(void);

	// Vertex Shader
	const GLchar* vertexShaderSourceCode_sphere =
	"#version 460 core"
	"\n"
	"in vec4 a_position;" \
	"in vec3 a_normal;" \
	"uniform mat4 u_modelMatrix;" \
	"uniform mat4 u_viewMatrix;" \
	"uniform mat4 u_projectionMatrix;" \

	"uniform vec3 u_la[3];" \
	"uniform vec3 u_ld[3];" \
	"uniform vec3 u_ls[3];" \
	"uniform vec4 u_lightPosition[3]; " \

	"uniform vec3 u_ka;" \
	"uniform vec3 u_kd;" \
	"uniform vec3 u_ks;" \
	"uniform float u_materialShininess;"
	"uniform int u_lightingEnabled;" \
	"out vec3 phong_ADS_Light;" \
	"void main(void)" \
	"{" \
		"if (u_lightingEnabled == 1)" \
		"{" \
			"phong_ADS_Light = vec3(0.0f,0.0f,0.0f);" \
			"vec4 iCoordinates = u_viewMatrix * u_modelMatrix * a_position;" \
			"mat3 normalMatrix = mat3(u_viewMatrix * u_modelMatrix);" \
			"vec3 transformedNormals = normalize(normalMatrix * a_normal);" \
			"vec3 viewerVector = normalize(-iCoordinates.xyz);" \

			"vec3 ambient[3];" \
			"vec3 lightDirection[3];" \
			"vec3 diffuse[3];" \
			"vec3 reflectionVector[3];" \
			"vec3 specular[3]; " \

			"for(int i = 0; i < 3; i++)" \
			"{" \
				"ambient[i] = u_la[i] * u_ka;" \
				"lightDirection[i] = normalize( vec3(u_lightPosition[i]) - iCoordinates.xyz );" \

				"diffuse[i] = u_ld[i] * u_kd * max( dot( lightDirection[i], transformedNormals), 0.0);" \
				"reflectionVector[i] = reflect(-lightDirection[i], transformedNormals);" \

				"specular[i] = u_ls[i] * u_ks * pow( max( dot( reflectionVector[i], viewerVector ), 0.0 ), u_materialShininess ); " \
				"phong_ADS_Light = phong_ADS_Light + ambient[i] + diffuse[i] + specular[i];" \
			"}" \
		"}" \
		"else" \
		"{" \
			"phong_ADS_Light = vec3(1.0, 1.0, 1.0);" \
		"}" \

		"gl_Position = u_projectionMatrix * u_viewMatrix * u_modelMatrix * a_position;" \
	"}"; \

		// Create Shader
		GLuint vertexShaderObject_sphere = glCreateShader(GL_VERTEX_SHADER);

	// Give the shader source code to this code
	glShaderSource(vertexShaderObject_sphere, 1, (const GLchar**)&vertexShaderSourceCode_sphere, NULL);

	// Compile the shader code to make it GPU understandable from human understandable
	glCompileShader(vertexShaderObject_sphere);

	// Error checking of shader compilation
	GLint status;
	GLint infoLogLength;
	char* log = NULL;

	// Getting compilation status
	glGetShaderiv(vertexShaderObject_sphere, GL_COMPILE_STATUS, &status);

	if (status == GL_FALSE)	// GL_FALSE means there is error
	{
		// Getting length of log of compilation status
		glGetShaderiv(vertexShaderObject_sphere, GL_INFO_LOG_LENGTH, &infoLogLength);
		if (infoLogLength > 0)
		{
			// Allocate enough memory to the buffer to hold the compilation log
			log = (char*)malloc(infoLogLength);
			if (log != NULL)
			{
				GLsizei written;

				// Get the compilation log into thtis allocated buffer
				glGetShaderInfoLog(vertexShaderObject_sphere, infoLogLength, &written, log);

				// Dsiplay the log/contents of buffer
				fprintf(gpFile, "Vertex Shader of sphere complitation log: %s\n", log);

				// Free the allocated buffer
				free(log);

				// Exit the application due to error
				uninitialize_sphere();
			}
		}
	}

	// Fragment Shader
	const GLchar* fragmentShaderSourceCode_sphere =
	"#version 460 core"
	"\n"
	"in vec3 phong_ADS_Light;"
	"out vec4 FragColor;"
	"void main(void)"
	"{"
		"FragColor = vec4(phong_ADS_Light, 1.0);"
	"}";

	// Create Shader
	GLuint fragmentShaderObject_sphere = glCreateShader(GL_FRAGMENT_SHADER);

	// Give the shader source code to this code
	glShaderSource(fragmentShaderObject_sphere, 1, (const GLchar**)&fragmentShaderSourceCode_sphere, NULL);

	// Compile the shader code to make it GPU understandable from human understandable
	glCompileShader(fragmentShaderObject_sphere);

	// Error checking of the shader compilation
	status = 0;
	infoLogLength = 0;
	log = NULL;

	// Getting compilation status
	glGetShaderiv(fragmentShaderObject_sphere, GL_COMPILE_STATUS, &status);

	if (status == GL_FALSE)
	{
		glGetShaderiv(fragmentShaderObject_sphere, GL_INFO_LOG_LENGTH, &infoLogLength);
		if (infoLogLength > 0)
		{
			// Allocate enough memory to the buffer to hold the comiplation log
			log = (char*)malloc(infoLogLength);
			if (log != NULL)
			{
				GLsizei written;

				// Get the compilation log into this allocated buffer
				glGetShaderInfoLog(fragmentShaderObject_sphere, infoLogLength, &written, log);

				// Display the log/content of buffer
				fprintf(gpFile, "Fragment shader of sphere compilation log: %s", log);

				// Free the allocated buffer
				free(log);

				// Exit the application due to the error
				uninitialize_sphere();
			}
		}
	}

	// Create Shader program object
	shaderProgramObject_sphere = glCreateProgram();

	// Attach desired shaders to this program object
	glAttachShader(shaderProgramObject_sphere, vertexShaderObject_sphere);
	glAttachShader(shaderProgramObject_sphere, fragmentShaderObject_sphere);

	// Pre-link binding of shader program object with Vertex attributes
	glBindAttribLocation(shaderProgramObject_sphere, AMC_ATTRIBUTE_POSITION, "a_position");
	glBindAttribLocation(shaderProgramObject_sphere, AMC_ATTRIBUTE_NORMAL, "a_normal");

	// Link shader program object
	glLinkProgram(shaderProgramObject_sphere);

	// Link error checking
	status = 0;
	infoLogLength = 0;
	log = NULL;

	// Getting link status
	glGetProgramiv(shaderProgramObject_sphere, GL_LINK_STATUS, &status);

	if (status == GL_FALSE)
	{
		glGetProgramiv(shaderProgramObject_sphere, GL_INFO_LOG_LENGTH, &infoLogLength);
		if (infoLogLength > 0)
		{
			// Allocate enough memory to the buffer
			log = (char*)malloc(infoLogLength);
			if (log != NULL)
			{
				GLsizei written;

				// Get the link log to the allocated buffer
				glGetProgramInfoLog(shaderProgramObject_sphere, infoLogLength, &written, log);

				// Display the log/ content of buffer
				fprintf(gpFile, "Sphere Shader Program Link log : %s", log);

				// Free the buffer
				free(log);

				// Exit the application
				uninitialize_sphere();
			}
		}
	}

	// Post link retrieving/getting uniform location from shader program object
	modelMatrixUniform_sphere = glGetUniformLocation(shaderProgramObject_sphere, "u_modelMatrix");
	viewMatrixUniform_sphere = glGetUniformLocation(shaderProgramObject_sphere, "u_viewMatrix");
	projectionMatrixUniform_sphere = glGetUniformLocation(shaderProgramObject_sphere, "u_projectionMatrix");

	// Light 0
	laUniform_sphere[0] = glGetUniformLocation(shaderProgramObject_sphere, "u_la[0]");
	ldUniform_sphere[0] = glGetUniformLocation(shaderProgramObject_sphere, "u_ld[0]");
	lsUniform_sphere[0] = glGetUniformLocation(shaderProgramObject_sphere, "u_ls[0]");
	lightPositionUniform_sphere[0] = glGetUniformLocation(shaderProgramObject_sphere, "u_lightPosition[0]");

	// Light 1
	laUniform_sphere[1] = glGetUniformLocation(shaderProgramObject_sphere, "u_la[1]");
	ldUniform_sphere[1] = glGetUniformLocation(shaderProgramObject_sphere, "u_ld[1]");
	lsUniform_sphere[1] = glGetUniformLocation(shaderProgramObject_sphere, "u_ls[1]");
	lightPositionUniform_sphere[1] = glGetUniformLocation(shaderProgramObject_sphere, "u_lightPosition[1]");

	// Light 2
	laUniform_sphere[2] = glGetUniformLocation(shaderProgramObject_sphere, "u_la[2]");
	ldUniform_sphere[2] = glGetUniformLocation(shaderProgramObject_sphere, "u_ld[2]");
	lsUniform_sphere[2] = glGetUniformLocation(shaderProgramObject_sphere, "u_ls[2]");
	lightPositionUniform_sphere[2] = glGetUniformLocation(shaderProgramObject_sphere, "u_lightPosition[2]");

	// Material
	kaUniform_sphere = glGetUniformLocation(shaderProgramObject_sphere, "u_ka");
	kdUniform_sphere = glGetUniformLocation(shaderProgramObject_sphere, "u_kd");
	ksUniform_sphere = glGetUniformLocation(shaderProgramObject_sphere, "u_ks");
	materialShininessUniform_sphere = glGetUniformLocation(shaderProgramObject_sphere, "u_materialShininess");

	lightingEnabledUniform_sphere = glGetUniformLocation(shaderProgramObject_sphere, "u_lightingEnabled");



	// Here Starts OpenGL Code
	// Sphere related functions
	getSphereVertexData(sphere_vertices, sphere_normals, sphere_textures, sphere_elements);
	gNumVertices_sphere = getNumberOfSphereVertices();
	gNumElements_sphere = getNumberOfSphereElements();

	// vao
	glGenVertexArrays(1, &vao_sphere);
	glBindVertexArray(vao_sphere);

	// position vbo
	glGenBuffers(1, &vbo_sphere_position);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_sphere_position);
	glBufferData(GL_ARRAY_BUFFER, sizeof(sphere_vertices), sphere_vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(AMC_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(AMC_ATTRIBUTE_POSITION);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// normal vbo
	glGenBuffers(1, &vbo_sphere_normal);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_sphere_normal);
	glBufferData(GL_ARRAY_BUFFER, sizeof(sphere_normals), sphere_normals, GL_STATIC_DRAW);

	glVertexAttribPointer(AMC_ATTRIBUTE_NORMAL, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	glEnableVertexAttribArray(AMC_ATTRIBUTE_NORMAL);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// element vbo
	glGenBuffers(1, &vbo_sphere_element);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_sphere_element);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(sphere_elements), sphere_elements, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// unbind vao
	glBindVertexArray(0);

	// Depth Related Changes. 2 lines are not useful in PP, depricated in PP - >glShadeModel(GL_SMOOTH), glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	// Clear the Screen using Blue Color
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	// Lights initializations
	lights_sphere[0].lightAmbient = vmath::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	lights_sphere[0].lightDiffuse = vmath::vec4(1.0f, 0.0f, 0.0f, 1.0f);	//Red light
	lights_sphere[0].lightSpecular = vmath::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	lights_sphere[0].lightPosition = vmath::vec4(-2.0f, 0.0f, 0.0f, 1.0f);

	lights_sphere[1].lightAmbient = vmath::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	lights_sphere[1].lightDiffuse = vmath::vec4(0.0f, 1.0f, 0.0f, 1.0f);	//Green light
	lights_sphere[1].lightSpecular = vmath::vec4(0.0f, 1.0f, 0.0f, 1.0f);
	lights_sphere[1].lightPosition = vmath::vec4(0.0f, 0.0f, 0.0f, 1.0f);

	lights_sphere[2].lightAmbient = vmath::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	lights_sphere[2].lightDiffuse = vmath::vec4(0.0f, 0.0f, 1.0f, 1.0f);	//Blue light
	lights_sphere[2].lightSpecular = vmath::vec4(0.0f, 0.0f, 1.0f, 1.0f);
	lights_sphere[2].lightPosition = vmath::vec4(2.0f, 0.0f, 0.0f, 1.0f);



	// Iniitialize needed matrices using vmath
	perspectiveProjectionMatrix_sphere = mat4::identity();

	// warm-up
	resize_sphere(FBO_WIDTH, FBO_HEIGHT);
	return 0;
}

void printGLInfo(void)
{
	// Local variable declarations
	GLint numExtensions = 0;

	// Code
	fprintf(gpFile, "OpenGL Vendor: %s\n", glGetString(GL_VENDOR) );
	fprintf(gpFile, "OpenGL Renderer: %s\n", glGetString(GL_RENDERER) );
	fprintf(gpFile, "OpenGL Version: %s\n", glGetString(GL_VERSION) );
	fprintf(gpFile, "GLSL version: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION) );
	
	glGetIntegerv( GL_NUM_EXTENSIONS, &numExtensions );

	fprintf(gpFile, "Number of supported extensions: %d\n", numExtensions);

	for(int i = 0 ; i < numExtensions ; i ++)
	{
		fprintf(gpFile, "%s\n", glGetStringi(GL_EXTENSIONS, i ) );
	}
}

void resize(int width, int height)
{
	// code
	/*width = WIN_WIDTH;
	height = WIN_HEIGHT;*/

	winWidth = width;
	winHeight = height;

	if (height == 0)
		height = 1; // to avoid divide 0 illegal instruction for future code

	glViewport(0, 0, GLsizei(width), GLsizei(height));

	perspectiveProjectionMatrix = vmath::perspective(45.0f, (GLfloat)width/ (GLfloat)height, 0.1f, 100.0f );

}

void resize_sphere(int width, int height)
{
	// code
	
	if (height == 0)
		height = 1; // to avoid divide 0 illegal instruction for future code

	glViewport(0, 0, GLsizei(width), GLsizei(height));

	perspectiveProjectionMatrix_sphere = vmath::perspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);

}

void display(void)
{
	// Function declaration
	void display_sphere(GLint , GLint );
	void update_sphere(void);

	// Code

	if (bFBOResult == TRUE)
	{
		display_sphere(FBO_WIDTH, FBO_HEIGHT);
		update_sphere();
	}
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	resize(winWidth, winHeight);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	
	// Use the shader program object
	glUseProgram(shaderProgramObject);

	// Transformation
	// Declare and initialize needed matrices
	mat4 translationMatrix = mat4::identity();
	mat4 rotationMatrix = mat4::identity();
	mat4 rotationMatrix_x = mat4::identity();
	mat4 rotationMatrix_y = mat4::identity();
	mat4 rotationMatrix_z = mat4::identity();
	mat4 modelViewMatrix = mat4::identity();
	mat4 modelViewProjectionMatrix = mat4::identity();

	// Cube
	// Transformation
	mat4 scaleMatrix			= mat4::identity();
	
	// glTranslatef  is replaced by this line
	translationMatrix = vmath::translate(0.0f, 0.0f, -4.0f);
	scaleMatrix = vmath::scale(0.75f, 0.75f, 0.75f);
	
	rotationMatrix_x = vmath::rotate(angleCube, 1.0f, 0.0f, 0.0f);		// glRotate3f()
	rotationMatrix_y = vmath::rotate(angleCube, 0.0f, 1.0f, 0.0f);		// glRotate3f()
	rotationMatrix_z = vmath::rotate(angleCube, 0.0f, 0.0f, 1.0f);		// glRotate3f()
	rotationMatrix = rotationMatrix_x * rotationMatrix_y * rotationMatrix_z;		// glRotate3f()
	modelViewMatrix = translationMatrix * rotationMatrix * scaleMatrix;				// Order is very important
	

	// Do translation, rotation, scale -> transformations using vmath

	// Do necessary  matrix multiplications
	modelViewProjectionMatrix = perspectiveProjectionMatrix * modelViewMatrix;

	// Send above transformation matrices to the respective matrix uniforms
	glUniformMatrix4fv(mvpMatrixUniform, 1, GL_FALSE, modelViewProjectionMatrix);

	// Texture related
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, fbo);
	glUniform1i(textureSamplerUniform, 0);

	// Bind with vertex array object
	glBindVertexArray(vao_cube);

	// Here, there should be drawing code, Do the animation /  Draw the animation
	// Cube 6 surfaces
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	glDrawArrays(GL_TRIANGLE_FAN, 4, 4);
	glDrawArrays(GL_TRIANGLE_FAN, 8, 4);
	glDrawArrays(GL_TRIANGLE_FAN, 12, 4);
	glDrawArrays(GL_TRIANGLE_FAN, 16, 4);
	glDrawArrays(GL_TRIANGLE_FAN, 20, 4);

	// Unbind with vertex array object
	glBindVertexArray(0);


	// Unuse the shader program object
	glUseProgram(0);

	SwapBuffers(ghdc);
}

void update(void)
{
	GLfloat increment = 0.1f;

	angleCube = angleCube + increment;
	if (angleCube >= 360)
		angleCube = 0.0f;

}

void display_sphere(GLint textureWidth, GLint textureHeight)
{
	// Code
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	resize_sphere(textureWidth, textureHeight);


	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Pyramid

	// Use the shader program object
	glUseProgram(shaderProgramObject_sphere);

	// Transformation
	// Declare and initialize needed matrices
	mat4 translationMatrix = mat4::identity();
	mat4 rotationMatrix = mat4::identity();
	mat4 rotationMatrix_x = mat4::identity();
	mat4 rotationMatrix_y = mat4::identity();
	mat4 rotationMatrix_z = mat4::identity();

	mat4 modelMatrix = mat4::identity();
	mat4 viewMatrix = mat4::identity();
	//mat4 projectionMatrix = mat4::identity();

	// glTranslatef  is replaced by this line
	translationMatrix = vmath::translate(0.0f, 0.0f, -2.0f);
	//rotationMatrix = vmath::rotate(anglePyramid, 0.0f, 1.0f, 0.0f);		// glRotate3f()
	modelMatrix = translationMatrix;// *rotationMatrix;				// Order is very important


	// Do translation, rotation, scale -> transformations using vmath

	// DO necessary  matrix multiplications
	//projectionMatrix = perspectiveProjectionMatrix * modelMatrix;

	// Send above transformation matrices to the respective matrix uniforms
	glUniformMatrix4fv(modelMatrixUniform_sphere, 1, GL_FALSE, modelMatrix);
	glUniformMatrix4fv(viewMatrixUniform_sphere, 1, GL_FALSE, viewMatrix);
	glUniformMatrix4fv(projectionMatrixUniform_sphere, 1, GL_FALSE, perspectiveProjectionMatrix_sphere);

	if (bLight == TRUE)
	{
		glUniform1i(lightingEnabledUniform_sphere, 1);

		for (int i = 0; i < 3; i++)
		{
			glUniform3fv(laUniform_sphere[i], 1, lights_sphere[i].lightAmbient);
			glUniform3fv(ldUniform_sphere[i], 1, lights_sphere[i].lightDiffuse);
			glUniform3fv(lsUniform_sphere[i], 1, lights_sphere[i].lightSpecular);
		}
		glUniform4fv(lightPositionUniform_sphere[0], 1, vec4(lights_sphere[0].lightPosition[0], sin(lights_sphere[0].lightPosition[1] + lightRotatingAngle_sphere), cos(lights_sphere[0].lightPosition[2] + lightRotatingAngle_sphere), 1.0f));
		glUniform4fv(lightPositionUniform_sphere[1], 1, vec4(sin(lights_sphere[1].lightPosition[0] + lightRotatingAngle_sphere), lights_sphere[1].lightPosition[1], cos(lights_sphere[1].lightPosition[2] + lightRotatingAngle_sphere), 1.0f));
		glUniform4fv(lightPositionUniform_sphere[2], 1, vec4(sin(lights_sphere[2].lightPosition[0] + lightRotatingAngle_sphere), cos(lights_sphere[2].lightPosition[1] + lightRotatingAngle_sphere), lights_sphere[2].lightPosition[2], 1.0f));

		glUniform3fv(kaUniform_sphere, 1, materialAmbient_sphere);
		glUniform3fv(kdUniform_sphere, 1, materialDiffuse_sphere);
		glUniform3fv(ksUniform_sphere, 1, materialSpecular_sphere);

		glUniform1f(materialShininessUniform_sphere, materialShininess_sphere);

	}
	else
	{
		glUniform1i(lightingEnabledUniform_sphere, 0);
	}

	// Bind with vertex array object
	glBindVertexArray(vao_sphere);

	// Here, there should be drawing code, Do the animation /  Draw the animation
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_sphere_element);
	glDrawElements(GL_TRIANGLES, gNumElements_sphere, GL_UNSIGNED_SHORT, 0);

	// Unbind with vertex array object vao unbind
	glBindVertexArray(0);

	// Unuse the shader program object
	glUseProgram(0);
	
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void update_sphere(void)
{
	// code
	GLfloat incrementCounter = 0.004f;
	lightRotatingAngle_sphere = lightRotatingAngle_sphere + incrementCounter;
	if (lightRotatingAngle_sphere >= 360.f)
	{
		lightRotatingAngle_sphere = lightRotatingAngle_sphere - 360.0f;
	}
}

void uninitialize(void)
{
	// function declarations
	void ToggleFullScreen(void);
	void uninitialize_sphere(void);

	// code
	if (gbFullScreen)
	{
		ToggleFullScreen();
	}
	
	if (vbo_cube_texcoord)
	{
		glDeleteBuffers(1, &vbo_cube_texcoord);
		vbo_cube_texcoord = 0;
	}
	// Delete and uninitialization of vbo_pyramid_position
	
	if (vbo_cube_position)
	{
		glDeleteBuffers(1, &vbo_cube_position);
		vbo_cube_position = 0;
	}

	// Delete and uninitialization of VAO
	if (vao_cube)
	{
		glDeleteVertexArrays(1, &vao_cube);
		vao_cube = 0;
	}

	// Shader uninitialization
	if(shaderProgramObject)
	{
		// Use the shader program
		glUseProgram(shaderProgramObject);
		
		GLsizei numAttachedShaders;
		
		// Get the number of attached shaders
		glGetProgramiv(shaderProgramObject, GL_ATTACHED_SHADERS, &numAttachedShaders);
		
		// Allocate enough memory to this empty buffer according to the number of attached shaders and fill it with the attached shader object
		GLuint *shaderObjects = NULL;
		
		shaderObjects = (GLuint *)malloc(numAttachedShaders * sizeof(GLuint));
		
		glGetAttachedShaders(shaderProgramObject, numAttachedShaders, &numAttachedShaders, shaderObjects);
		
		// As shaders attached can be more than 1, loop for dettach and delete shaders
		for(GLsizei i = 0; i < numAttachedShaders ; i++)
		{
			glDetachShader(shaderProgramObject, shaderObjects[i]);
			glDeleteProgram(shaderObjects[i]);
			shaderObjects[i] = 0;
		}
		
		// Free the memory allocated to buffer
		free(shaderObjects);
		shaderObjects = NULL;
		
		// Unuse the shader prog object
		glUseProgram(0);
		
		// Delete the shader prog object
		glDeleteProgram(shaderProgramObject);
		shaderProgramObject = 0;
	}

	if (wglGetCurrentContext() == ghrc)
	{
		wglMakeCurrent(NULL, NULL);
	}

	if (ghrc)
	{
		wglDeleteContext(ghrc);
		ghrc = NULL;
	}

	if (ghdc)
	{
		ReleaseDC(ghwnd, ghdc);
		ghdc = NULL;
	}

	if (ghwnd)
	{
		DestroyWindow(ghwnd);
		ghwnd = NULL;
	}

	if (gpFile)
	{
		fprintf(gpFile, "Log File Is Sucessfully Closed !!!\n");
		fclose(gpFile);
		gpFile = NULL;
	}

	uninitialize_sphere();
}

void uninitialize_sphere(void)
{
	// function declarations
	void ToggleFullScreen(void);

	// code
	
	// deletion of vbo
	if (vbo_sphere_element)
	{
		glDeleteBuffers(1, &vbo_sphere_element);
		vbo_sphere_element = 0;
	}

	if (vbo_sphere_normal)
	{
		glDeleteBuffers(1, &vbo_sphere_normal);
		vbo_sphere_normal = 0;
	}

	if (vbo_sphere_position)
	{
		glDeleteBuffers(1, &vbo_sphere_position);
		vbo_sphere_position = 0;
	}

	// deletion of vao
	if (vao_sphere)
	{
		glDeleteVertexArrays(1, &vao_sphere);
		vao_sphere = 0;
	}


	// Shader uninitialization
	if (shaderProgramObject_sphere)
	{
		// Use the shader program
		glUseProgram(shaderProgramObject_sphere);

		GLsizei numAttachedShaders;

		// Get the number of attached shaders
		glGetProgramiv(shaderProgramObject_sphere, GL_ATTACHED_SHADERS, &numAttachedShaders);

		// Allocate enough memory to this empty buffer according to the number of attached shaders and fill it with the attached shader object
		GLuint* shaderObjects = NULL;

		shaderObjects = (GLuint*)malloc(numAttachedShaders * sizeof(GLuint));

		glGetAttachedShaders(shaderProgramObject_sphere, numAttachedShaders, &numAttachedShaders, shaderObjects);

		// As shaders attached can be more than 1, loop for dettach and delete shaders
		for (GLsizei i = 0; i < numAttachedShaders; i++)
		{
			glDetachShader(shaderProgramObject_sphere, shaderObjects[i]);
			glDeleteProgram(shaderObjects[i]);
			shaderObjects[i] = 0;
		}

		// Free the memory allocated to buffer
		free(shaderObjects);
		shaderObjects = NULL;

		// Unuse the shader prog object
		glUseProgram(0);

		// Delete the shader prog object
		glDeleteProgram(shaderProgramObject_sphere);
		shaderProgramObject_sphere = 0;
	}


}
