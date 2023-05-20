// Header Files
#include <windows.h>
#include <stdio.h>  // for FileIO functions
#include <stdlib.h> // for exit()
#include "OGL.h"

// OPENGL HEADER FILES
#include <GL/glew.h> // THIS IS MUST BE ABOVE GL.h
#include <GL/gl.h>

#include "vmath.h"

/* OpenCL Related */
#include <CL/opencl.h>

using namespace vmath;

// macros
#define WIN_WIDTH 800
#define WIN_HEIGHT 600

// OpenGL Libraries 
#pragma comment(lib, "glew32.lib")
#pragma comment(lib,"OpenGL32.lib")
#pragma comment(lib, "OpenCL.lib")

// global variable declarations
HWND ghwnd = NULL;
HDC ghdc = NULL;
HGLRC ghrc = NULL;
BOOL gbFullScreen = FALSE;
BOOL gbActiveWindow = FALSE;
FILE* gpFile = NULL;


// Programable pipeline related global variables
GLuint shaderProgramObject;

enum
{
	AMC_ATTRIBUTE_POSITION = 0,
	AMC_ATTRIBUTE_COLOR,
	AMC_ATTRIBUTE_NORMAL,
	AMC_ATTRIBUTE_TEXTURE0
};

GLuint VOA;
GLuint VBO;
GLuint mvpMatrixUniform;

mat4 perceptivegraphicsProjectionMatrix;


/* Sine Wave */
unsigned int meshWidth = 1024;
unsigned int meshHeight = 1024;
#define MYARRAYSIZE meshWidth*meshHeight*4
float pos[1024][1024][4];

float animationTime = 0.0f;

/* OpenCL Related varaibles */
cl_int oclResult;
cl_mem graphicsResource = NULL;
cl_context oclContext;
cl_command_queue oclCommandQueue;
cl_program oclProgram;
cl_kernel oclKernel;
cl_platform_id oclPlatformID;
cl_device_id oclDeviceID;

cl_device_id* oclDeviceIDs = NULL;

GLuint VBO_GPU;
bool onGPU = false;

// global function declarations
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

// Entry point function
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow)
{
	// function delcarations
	int initialise(void);
	void display(void);
	void update(void);
	void uninitialise(void);
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
	wndclass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(MYICON));
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.lpszClassName = szAppName;
	wndclass.lpszMenuName = NULL;
	wndclass.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(MYICON));

	// Resgistry above class
	RegisterClassEx(&wndclass);

	// create the window
	hwnd = CreateWindowEx(WS_EX_APPWINDOW,
		szAppName,
		TEXT("OGL"),
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
		uninitialise();
	}
	else if (iRetVal == -2)
	{
		fprintf(gpFile, "Set Pixel Format Failed !\n");
		uninitialise();
	}
	else if (iRetVal == -3)
	{
		fprintf(gpFile, "Create OpenGL Context Failed !\n");
		uninitialise();
	}
	else if (iRetVal == -4)
	{
		fprintf(gpFile, "Making OpenGL Context as Current Context Failed !\n");
		uninitialise();
	}
	else
	{
		fprintf(gpFile, "initialise function Sucessful\n");
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
	uninitialise();
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

		break;
	case WM_CHAR:
		switch (wParam)
		{
		case 'C':
		case 'c':
			onGPU = false;
			break;

		case 'g':
		case 'G':
			onGPU = true;
			break;

		case 'F':
		case 'f':
			ToggleFullScreen();
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
	void PrintGLInfo(void);
	void uninitialise(void);
	void resize(int width, int height);

	// variable declarations
	PIXELFORMATDESCRIPTOR pfd;
	int iPixelFormatIndex = 0;
	unsigned int dev_count;

	// code
	// init of PIXELFORMATDESCRIPTOR structure
	ZeroMemory(&pfd, sizeof(PIXELFORMATDESCRIPTOR)); // memset((void*)&pfd, NULL, sizeof(PIXELFORMATDESCRIPTOR));
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


	// GLEW init
	if (glewInit() != GLEW_OK)
	{
		return -5;
	}

	/* OpenCL Init */
	fprintf(gpFile, "Init OpenCL\n");
	/* Step1 : Get Platform ID */
	oclResult = clGetPlatformIDs(1, &oclPlatformID, NULL);
	if (oclResult != CL_SUCCESS)
	{
		fprintf(gpFile, "clGetPlatformIDs() is Failed : %d\n", oclResult);
		uninitialise();
		exit(EXIT_FAILURE);
	}

	/* Get GPU Device ID */
		/* Step1: Get Total GPU Device count oclPlatfromID*/
	oclResult = clGetDeviceIDs(oclPlatformID, CL_DEVICE_TYPE_GPU, 0, NULL, &dev_count);
	if (oclResult != CL_SUCCESS)
	{
		fprintf(gpFile, "clGetDeviceIDs() Failed to get Device count : %d\n", oclResult);
		uninitialise();
		exit(EXIT_FAILURE);
	}
	else if (dev_count == 0)
	{
		fprintf(gpFile, "GPU Device count is 0\n");
		uninitialise();
		exit(EXIT_FAILURE);
	}
	fprintf(gpFile, "GPU Device count is %ld\n", dev_count);
	//fprintf(gpFile, "GPU Device count is %lf\n", dev_count);
	
	/* Step2: Create memroy for array of device IDs */
	oclDeviceIDs = (cl_device_id*)malloc(sizeof(cl_device_id) * dev_count);
	/* Here should be error checking for malloc */

		/* Step3: Fill the array */
	oclResult = clGetDeviceIDs(oclPlatformID, CL_DEVICE_TYPE_GPU, dev_count, oclDeviceIDs, NULL);
	if (oclResult != CL_SUCCESS)
	{
		fprintf(gpFile, "clGetDeviceIDs() Failed : %d\n", oclResult);
		uninitialise();
		exit(EXIT_FAILURE);
	}
	/* Step4: Take 0th from array as selected device */
	oclDeviceID = oclDeviceIDs[0];
	free(oclDeviceIDs);

	/* Step3: Create openCL context for selected OpenCL Device */
	/* create Context Properties Array() */
	cl_context_properties oclContextProperties[] =
	{
		CL_GL_CONTEXT_KHR, (cl_context_properties)wglGetCurrentContext(),
		CL_WGL_HDC_KHR, (cl_context_properties)wglGetCurrentDC(),
		CL_CONTEXT_PLATFORM, (cl_context_properties)oclPlatformID,
		0


	};
	/* Create actual openCL context */
	oclContext = clCreateContext(oclContextProperties, 1, &oclDeviceID, NULL, NULL, &oclResult);
	if (oclResult != CL_SUCCESS)
	{
		fprintf(gpFile, "clCreateContext() Failed : %d\n", oclResult);
		uninitialise();
		exit(EXIT_FAILURE);
	}
	/* Create Command Queue */
	oclCommandQueue = clCreateCommandQueue(oclContext, oclDeviceID, 0, &oclResult);
	if (oclResult != CL_SUCCESS)
	{
		fprintf(gpFile, "clCreateCommandQueueWithProperties() Failed : %d\n", oclResult);
		uninitialise();
		exit(EXIT_FAILURE);
	}

	/* create OpenCL program from OpenCL source code */
/* Write OpenCL kernel source code */
	const char* oclSourceCode =

		"__kernel void sineWaveKernel(__global float4 * position, unsigned int width, unsigned int height, float time)" \
		"{" \
		// code
		"unsigned int i =  get_global_id(0);" \
		"unsigned int j =  get_global_id(1);" \

		"float u = (float)i / (float)width;" \
		"float v = (float)j / (float)height;" \

		"u = u * 2.0f - 1.0f;" \
		"v = v * 2.0f - 1.0f;" \

		"float freq = 4.0f;" \
		"float w = sin(u * freq + time) * cos(v * freq + time) * 0.5f;" \
		"position[j * width + i] = (float4)(u, w, v, 1.0f); " \

		"}";

	/* Create Actual Program from above source code */
	oclProgram = clCreateProgramWithSource(oclContext, 1, (const char**)&oclSourceCode, NULL, &oclResult);
	if (oclResult != CL_SUCCESS)
	{
		fprintf(gpFile, "clCreateProgramWithSource() Failed : %d\n", oclResult);
		uninitialise();
		exit(EXIT_FAILURE);
	}

	/* build OpenCL program */
	oclResult = clBuildProgram(oclProgram, 0, NULL, "-cl-fast-relaxed-math", NULL, NULL);
	if (oclResult != CL_SUCCESS)
	{
		size_t len;
		fprintf(gpFile, "clBuildProgram() Failed : %d\n", oclResult);
		char buffer[2048];
		clGetProgramBuildInfo(oclProgram, oclDeviceID, CL_PROGRAM_BUILD_LOG, sizeof(buffer), buffer, &len);
		fprintf(gpFile, "Program Build Log : %s\n", buffer);
		uninitialise();
		exit(EXIT_FAILURE);
	}

	// create OpenCL kernel by passing kernel function name 
	oclKernel = clCreateKernel(oclProgram, "sineWaveKernel", &oclResult);
	if (oclResult != CL_SUCCESS)
	{
		fprintf(gpFile, "clCreateKernel() is Failed : %d\n", oclResult);
		uninitialise();
		exit(EXIT_FAILURE);
	}


	
	// Vertex Shader code
	const GLchar* vertexShaderSourcecode =
		"#version 460 core" \
		"\n" \
		"in vec4 a_position;" \
		"uniform mat4 u_mvpMatrix;" \
		"void main(void)" \
		"{" \
		"	gl_Position = u_mvpMatrix * a_position;" \
		"}";

	// Creating shader object
	GLuint vertexShaderObject = glCreateShader(GL_VERTEX_SHADER);

	// Giving shader code to shader object
	glShaderSource(vertexShaderObject,
		1,
		(const GLchar**)&vertexShaderSourcecode,
		NULL);

	// Compile the shader
	glCompileShader(vertexShaderObject);

	// Error Checking
	GLint status;
	GLint infoLogLen;
	char* log = NULL;

	// a. Getting compilation status
	glGetShaderiv(vertexShaderObject,
		GL_COMPILE_STATUS,
		&status);

	if (status == GL_FALSE)
	{
		// Getting length of log of compilation status
		glGetShaderiv(vertexShaderObject,
			GL_INFO_LOG_LENGTH,
			&infoLogLen);
		if (infoLogLen > 0)
		{
			// Allocate enough memory to buffer to hold the compilation log 
			log = (char*)malloc(infoLogLen);
			if (log != NULL)
			{
				// Get the compilation log into this allocated buffer.
				GLsizei written;
				glGetShaderInfoLog(vertexShaderObject,
					infoLogLen,
					&written,
					log);

				// display the contents of buffer
				fprintf(gpFile, "Vertex Shader Compilation Log: %s\n", log);

				// Free the allocated the buffer.
				free(log);

				// exit the application due to error
				uninitialise();
			}
		}
	}




	// Fragment Shader

	// 1. Writing shader code
	const GLchar* fragmentShaderSourcecode =
		"#version 460 core" \
		"\n" \
		"out vec4 FragColor;" \
		"void main(void)" \
		"{" \
		"FragColor=vec4(1.0, 0.5, 0.0, 1.0);" \
		"}";

	// 2. Creating shader object
	GLuint fragmentShaderObject = glCreateShader(GL_FRAGMENT_SHADER);

	// 3. Giving shader code to shader object
	glShaderSource(fragmentShaderObject,
		1,
		(const GLchar**)&fragmentShaderSourcecode,
		NULL);

	// 4. Compile the shader
	glCompileShader(fragmentShaderObject);

	// 5. Error checking of shader compilation
	status = 0;
	infoLogLen = 0;
	log = NULL;

	// a. Getting compilation status
	glGetShaderiv(fragmentShaderObject,
		GL_COMPILE_STATUS,
		&status);

	if (status == GL_FALSE)
	{
		// Getting length of log of compilation status
		glGetShaderiv(fragmentShaderObject,
			GL_INFO_LOG_LENGTH,
			&infoLogLen);

		if (infoLogLen > 0)
		{
			// Allocate enough memory to buffer to hold the compilation log 
			log = (char*)malloc(infoLogLen);
			if (log != NULL)
			{
				// Get the compilation log into this allocated buffer.
				GLsizei written;
				glGetShaderInfoLog(fragmentShaderObject,
					infoLogLen,
					&written,
					log);

				// display the contents of buffer
				fprintf(gpFile, "Fragment Shader Compilation Log: %s\n", log);

				// Free the allocated the buffer.
				free(log);

				// exit the application due to error
				uninitialise();
			}
		}
	}

	// Creating, Linking, error checking of shader program
	// 1. Create shader program object
	shaderProgramObject = glCreateProgram();

	// 2. attach desire shaders to this shader program object
	glAttachShader(shaderProgramObject, vertexShaderObject);
	glAttachShader(shaderProgramObject, fragmentShaderObject);

	//  Pre-linking binding of shader program object with vertex attributes 
	glBindAttribLocation(shaderProgramObject,
		AMC_ATTRIBUTE_POSITION,
		"a_position");


	// 3. link shader program object
	glLinkProgram(shaderProgramObject);

	// retriving/getting uniformed location from shader program object
	mvpMatrixUniform = glGetUniformLocation(shaderProgramObject,
		"u_mvpMatrix");



	// 4. do link error checking with similar a to g steps like above
	status = 0;
	infoLogLen = 0;
	log = NULL;

	// a. Getting link status
	glGetProgramiv(shaderProgramObject,
		GL_LINK_STATUS,
		&status);

	if (status == GL_FALSE)
	{
		// Getting length of log of LINK status
		glGetProgramiv(shaderProgramObject,
			GL_INFO_LOG_LENGTH,
			&infoLogLen);

		if (infoLogLen > 0)
		{
			// // Allocate enough memory to buffer to hold the log 
			log = (char*)malloc(infoLogLen);
			if (log != NULL)
			{
				// Get the log into this allocated buffer.
				GLsizei written;
				glGetProgramInfoLog(shaderProgramObject,
					infoLogLen,
					&written,
					log);

				// display the contents of buffer
				fprintf(gpFile, "Shader Program Link Log : % s\n", log);

				// Free the allocated the buffer.
				free(log);

				// exit the application due to error
				uninitialise();

			}

		}
	}


	// VOA AND VBA Array related lines

	// declarations of vertex data arrays
	for (unsigned int i = 0; i < meshWidth; i++)
	{
		for (unsigned int j = 0; j < meshHeight; j++)
		{
			for (unsigned int k = 0; k < 4; k++)
			{
				pos[i][j][k] = 0.0f;
			}
		}
	}

	glGenVertexArrays(1,
		&VOA);

	// create vertex array object
	glBindVertexArray(VOA);
	// create vertex data buffer
	glGenBuffers(1, &VBO);

	// bind with vertex data buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// create storage of buffer data for perticular target
	glBufferData(GL_ARRAY_BUFFER,
		(MYARRAYSIZE * sizeof(float)),
		NULL,
		GL_DYNAMIC_DRAW);

	// f. unbind with respective vertex data buffer
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	/* OPENCL GPU */
	glGenBuffers(1, &VBO_GPU);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_GPU);

	glBufferData(GL_ARRAY_BUFFER,
		(MYARRAYSIZE * sizeof(float)),
		NULL,
		GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// Here Starts OpenGL Code
	// Clear the Screen using Blue Color
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
#if 1
	/* Create OPENCL-OPENGL Interop Resource */
	graphicsResource = clCreateFromGLBuffer(oclContext, CL_MEM_WRITE_ONLY, VBO_GPU, &oclResult);
	if (oclResult != CL_SUCCESS)
	{
		fprintf(gpFile, "clCreateFromGLBuffer() is Failed : %d\n", oclResult);
		uninitialise();
		exit(EXIT_FAILURE);
	}
#endif
	// Depth Related Changes
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);


	perceptivegraphicsProjectionMatrix = mat4::identity();
	resize(WIN_WIDTH, WIN_HEIGHT);
	return 0;
}

void resize(int width, int height)
{
	// code
	if (height == 0)
		height = 1; // to avoid divide 0 illegal instruction for future code


	glViewport(0, 0, GLsizei(width), GLsizei(height));

	perceptivegraphicsProjectionMatrix = vmath::perspective(45.0f, (GLfloat)width / (GLfloat)height, 1.0f, 100.0f);

}

void display(void)
{
	// Function Declarations
	void sineWave(unsigned int, unsigned int, float);
	void uninitialise(void);

	// varaible declarations
	size_t globalWorkSize[2];

	// code
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// 1. Use ShaderProgranObject
	glUseProgram(shaderProgramObject);


	// 2. Draw desire graphics
	// Transformation related steps
	mat4 translationMatrix = mat4::identity();
	mat4 modelViewMatrix = mat4::identity();
	mat4 modelViewProjectionMatrix = mat4::identity();


	translationMatrix = vmath::translate(0.0f, 0.0f, -2.0f);
	modelViewMatrix = translationMatrix;
	modelViewProjectionMatrix = perceptivegraphicsProjectionMatrix * modelViewMatrix;

	// send above transformation matrixes to shader in respective matrix uniform
	glUniformMatrix4fv(mvpMatrixUniform,
		1,
		GL_FALSE,
		modelViewProjectionMatrix);


	// bind with vertex array object
	glBindVertexArray(VOA);

	if (onGPU)
	{
		// OpenCL Related Code
		/* step1 set openc kernel param */

		/*Passing 0th param */
		oclResult = clSetKernelArg(oclKernel, 0, sizeof(cl_mem), (void*)&graphicsResource);
		if (oclResult != CL_SUCCESS)
		{
			printf("clSetKernelArg() is Failed  For 0th Argument : %d\n", oclResult);
			uninitialise();
			exit(EXIT_FAILURE);
		}

		/*Passing 1st param */
		oclResult = clSetKernelArg(oclKernel, 1, sizeof(unsigned int), &meshWidth);
		if (oclResult != CL_SUCCESS)
		{
			printf("clSetKernelArg() is Failed  For 1st Argument : %d\n", oclResult);
			uninitialise();
			exit(EXIT_FAILURE);
		}

		/* Passing 2nd param */
		oclResult = clSetKernelArg(oclKernel, 2, sizeof(unsigned int), &meshHeight);
		if (oclResult != CL_SUCCESS)
		{
			printf("clSetKernelArg() is Failed  For 2nd Argument : %d\n", oclResult);
			uninitialise();
			exit(EXIT_FAILURE);
		}

		/* Passing 3rd param */
		oclResult = clSetKernelArg(oclKernel, 3, sizeof(float), &animationTime);
		if (oclResult != CL_SUCCESS)
		{
			printf("clSetKernelArg() is Failed  For 3rd Argument : %d\n", oclResult);
			uninitialise();
			exit(EXIT_FAILURE);
		}

		/* Enqueu Graphics Resource in command queue */
		oclResult = clEnqueueAcquireGLObjects(oclCommandQueue, 1, &graphicsResource, 0, NULL, NULL);
		if (oclResult != CL_SUCCESS)
		{
			printf("clEnqueueAcquireGLObjects() is Failed  : %d\n", oclResult);
			uninitialise();
			exit(EXIT_FAILURE);
		}

		/* Run OpenCL Kernel */
		globalWorkSize[0] = meshWidth;
		globalWorkSize[1] = meshHeight;

		oclResult = clEnqueueNDRangeKernel(oclCommandQueue, oclKernel, 2, NULL, globalWorkSize, NULL, 0, NULL, NULL);
		if (oclResult != CL_SUCCESS)
		{
			printf("clEnqueueNDRangeKernel() is Failed  : %d\n", oclResult);
			uninitialise();
			exit(EXIT_FAILURE);
		}

		oclResult = clEnqueueReleaseGLObjects(oclCommandQueue, 1, &graphicsResource, 0, NULL, NULL);
		if (oclResult != CL_SUCCESS)
		{
			printf("clEnqueueReleaseGLObjects() is Failed  : %d\n", oclResult);
			uninitialise();
			exit(EXIT_FAILURE);
		}

		/* Finish command queue */
		clFinish(oclCommandQueue);

		/* Step4: GPU VBO Related Code */
		glBindBuffer(GL_ARRAY_BUFFER, VBO_GPU);

		/* As CUDA Done all work so no need of glBufferData, glVertexAttribPointer, glEnableVertexAttribArray Internally */
	}
	else
	{
		// CPU Related Code
		sineWave(meshWidth, meshHeight, animationTime);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);

		glBufferData(GL_ARRAY_BUFFER,
			MYARRAYSIZE * sizeof(float),
			pos,
			GL_DYNAMIC_DRAW);


	}

	glVertexAttribPointer(AMC_ATTRIBUTE_POSITION,
		4,
		GL_FLOAT,
		GL_FALSE,
		0,
		NULL);

	glEnableVertexAttribArray(AMC_ATTRIBUTE_POSITION);

	// draw
	glDrawArrays(GL_POINTS,
		0,
		meshWidth * meshHeight);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	animationTime = animationTime + 0.1f;





	// 3. Unused ShaderProgranObject
	glUseProgram(0);

	SwapBuffers(ghdc);
}

void sineWave(unsigned int width, unsigned int height, float time)
{
	// code
	for (unsigned int i = 0; i < width; i++)
	{
		for (unsigned int j = 0; j < height; j++)
		{
			for (unsigned int k = 0; k < 4; k++)
			{
				float u = (float)i / (float)width;
				float v = (float)j / (float)height;

				u = u * 2.0f - 1.0f;
				v = v * 2.0f - 1.0f;

				float freq = 4.0f;
				float w = sinf(u * freq + time) * cosf(v * freq + time) * 0.5f;

				if (k == 0)
				{
					pos[i][j][k] = u;
				}

				if (k == 1)
				{
					pos[i][j][k] = w;
				}

				if (k == 2)
				{
					pos[i][j][k] = v;
				}
				
				if (k == 3)
				{
					pos[i][j][k] = 1.0f;
				}
			}
		}
	}

}

void update(void)
{
	// code

}

void uninitialise(void)
{
	// function declarations
	void ToggleFullScreen(void);

	// code
	if (gbFullScreen)
	{
		ToggleFullScreen();
	}

	if (VBO)
	{
		glDeleteBuffers(1, &VBO);
		VBO = 0;
	}

	if (VBO_GPU)
	{
		if (graphicsResource)
		{
			clReleaseMemObject(graphicsResource);
			graphicsResource = NULL;
		}
		glDeleteBuffers(1, &VBO_GPU);
		VBO_GPU = 0;
	}

	if (oclKernel)
	{
		clReleaseKernel(oclKernel);
		oclKernel = NULL;
	}

	if (oclProgram)
	{
		clReleaseProgram(oclProgram);
		oclProgram = NULL;
	}

	if (oclCommandQueue)
	{
		clReleaseCommandQueue(oclCommandQueue);
		oclCommandQueue = NULL;
	}

	if (oclContext)
	{
		clReleaseContext(oclContext);
		oclContext = NULL;
	}




	if (VOA)
	{
		glDeleteVertexArrays(1, &VOA);
		VOA = 0;
	}
	// shader uninitialise
	if (shaderProgramObject)
	{
		// 0. again used sharderprogramobject
		glUseProgram(shaderProgramObject);

		// 1. Get Number of attach shaders
		GLsizei numAttchedShaders;
		glGetProgramiv(shaderProgramObject,
			GL_ATTACHED_SHADERS,
			&numAttchedShaders);

		// 2. Create empty buffer to hold array of attach shader objects
		GLuint* shaderObjects = NULL;
		// 3. allocate enough memroy to this according to number of attach shaders and fill it with attachedshaderobject
		shaderObjects = (GLuint*)malloc(numAttchedShaders * sizeof(GLuint));

		glGetAttachedShaders(shaderProgramObject,
			numAttchedShaders,
			(GLsizei*)&shaderProgramObject,
			shaderObjects);

		// 4. As number of attach shaders more than 1 start a loop and inside loop deattach, delete shader one by one
		for (GLsizei i = 0; i < numAttchedShaders; i++)
		{
			glDetachShader(shaderProgramObject, shaderObjects[i]);
			glDeleteShader(shaderObjects[i]);
			shaderObjects[i] = NULL;
		}

		// 5. free memory allocated for buffer
		free(shaderObjects);
		shaderObjects = NULL;

		// 6. unused sharderprogramobject
		glUseProgram(0);

		// 7. delete sharderprogramobject
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

}


void PrintGLInfo(void)
{
	// local varaible declarations
	GLint numExtenstions;

	// code
	fprintf(gpFile, "OpenGL Vendor: %s\n", glGetString(GL_VENDOR));
	fprintf(gpFile, "OpenGL Renderer: %s\n", glGetString(GL_RENDERER));
	fprintf(gpFile, "OpenGL Version: %s\n", glGetString(GL_VERSION));
	fprintf(gpFile, "GLSL Version: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

	glGetIntegerv(GL_NUM_EXTENSIONS, &numExtenstions);


	fprintf(gpFile, "Number of Supported Extentions : %d\n", numExtenstions);

	for (int i = 0; i < numExtenstions; i++)
	{
		fprintf(gpFile, "%s\n", glGetStringi(GL_EXTENSIONS, i));
	}

}

