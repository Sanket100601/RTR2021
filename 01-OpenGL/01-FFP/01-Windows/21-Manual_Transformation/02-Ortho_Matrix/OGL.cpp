// Header Files
#include<windows.h>
#include "OGL.h"
#include<stdio.h>	// For FILE_IO()
#include<stdlib.h>	//For Exit()
#define _USE_MATH_DEFINES 1
#include<math.h>	//For Pi,trignometric functions

// Opengl Header files 
#include <Gl/gL.h>
#define WIN_WIDTH 800
#define WIN_HEIGHT 600

//OpenGl Liabraries
#pragma comment(lib,"OpenGl32.lib")

//Global functions declarations
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

// Global variable
HWND ghwnd = FALSE;
BOOL gbFullScreen = FALSE;
FILE* gpFile = NULL;
BOOL gbActiveWindow = FALSE;
HDC ghdc = NULL;
HGLRC ghrc = NULL;
GLfloat anglePyramid = 0.0f;
GLfloat angleCube = 0.0f;
GLfloat speed = 0.1f;
GLfloat identityMatrix[16];
GLfloat translationMatrix[16];
GLfloat scaleMatrix[16];
GLfloat rotationMatrix_X[16];
GLfloat rotationMatrix_Y[16];
GLfloat rotationMatrix_Z[16];
GLfloat ortho_matrix[16];

// Entry Point Function
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow)
{
	// Function Declarion
	int initialize(void);
	void display(void);
	void update(void);
	void uninitialize(void);

	//variable declaration 
	WNDCLASSEX wndclass;
	HWND hwnd;
	MSG msg;
	TCHAR szAppName[] = TEXT("MyWindow");
	BOOL bDone = FALSE;
	int iRetval = 0;

	// code
	if (fopen_s(&gpFile, "Log.txt", "w") != 0)
	{
		MessageBox(NULL, TEXT("Creation Of Log File Failed. EXITTING"), TEXT("File I/O Error"), MB_OK);
		exit(0);
	}
	else
	{
		fprintf(gpFile, "Log File Is Susccessfully Created\n");
	}
	// initialization of class WNDCLASSEX struction
	wndclass.cbSize = sizeof(WNDCLASSEX);
	wndclass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.lpfnWndProc = WndProc;
	wndclass.hInstance = hInstance;
	wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndclass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(MYICON));
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.lpszClassName = szAppName;
	wndclass.lpszMenuName = NULL;
	wndclass.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(MYICON));

	// Registering WNDCLASSEX
	RegisterClassEx(&wndclass);



	// Create The Window
	hwnd = CreateWindowEx(WS_EX_APPWINDOW, szAppName,
		TEXT("OpenGl Window"),
		WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_VISIBLE,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		WIN_WIDTH,
		WIN_HEIGHT,
		NULL,
		NULL,
		hInstance,
		NULL);

	ghwnd = hwnd;

	// initialize();
	iRetval = initialize();
	if (iRetval == -1)
	{
		fprintf(gpFile, "CHOOSEPIXELFORMAT FAILED !!!\n");
		uninitialize();
	}
	if (iRetval == -2)
	{
		fprintf(gpFile, "SETPIXELFORMATFAILED !!!\n");
		uninitialize();
	}
	if (iRetval == -3)
	{
		fprintf(gpFile, "CREATEOPENGLCONTEXT FAILED !!!\n");
		uninitialize();
	}
	if (iRetval == -4)
	{
		fprintf(gpFile, "MAKINOPENGLCONTEXT AS CURRENT CONTEXT FAILED !!!\n");
		uninitialize();
	}
	// Show Window
	ShowWindow(hwnd, iCmdShow);

	// Foregrounding and Focusing The Window
	SetForegroundWindow(hwnd);
	SetFocus(hwnd);

	// Game Loop
	while (bDone == FALSE)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				bDone = TRUE;
			}
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
				// Render The Scene
				display();

				// Update The Scene
				update();
			}
		}
	}
	return (int)msg.wParam;
}

// CallBack Function
LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	static RECT rc;

	//function declaration
	void ToggleFullScreen();
	void resize(int, int);
	void uninitialize(void);

	switch (iMsg)
	{
	case WM_CREATE:
	{
		int sspWidth, sspHeight;
		RECT rect;

		// get screen size
		sspWidth = GetSystemMetrics(SM_CXSCREEN);
		sspHeight = GetSystemMetrics(SM_CYSCREEN);

		// get window
		GetWindowRect(hwnd, &rect);

		//Reset the value in rect
		rect.left = (sspWidth - rect.right) / 2;
		rect.top = (sspHeight - rect.bottom) / 2;

		// move the window to the specified position
		SetWindowPos(hwnd, HWND_TOP, rect.left, rect.top, rect.right, rect.bottom, SWP_SHOWWINDOW);
	}
	break;

	case WM_SIZE:
		resize(LOWORD(lParam), HIWORD(lParam));
		GetClientRect(hwnd, &rc);
		break;

	case WM_CHAR:
		switch (wParam)
		{
		case 'F':
		case 'f':
			ToggleFullScreen();
			break;
		case 27:
			if (gpFile)
			{
				fprintf(gpFile, "Log File SuccessFully CLosed\n");
				fclose(gpFile);
				gpFile = NULL;
			}
			uninitialize();
			PostQuitMessage(0);
		default:
			break;
		}
		break;

	case WM_SETFOCUS:
		gbActiveWindow = TRUE;
		break;

	case WM_KILLFOCUS:
		gbActiveWindow = FALSE;
		break;

	case WM_ERASEBKGND:
		break;

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

	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}

void ToggleFullScreen()
{
	// variable Declaration
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
		SetWindowPos(ghwnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_FRAMECHANGED);

		ShowCursor(TRUE);
		gbFullScreen = FALSE;
	}

}

int initialize(void)
{
	// Function Declarations
	void resize(int, int);	

	// Variable Declarations
	PIXELFORMATDESCRIPTOR pfd;
	int iPixelFormatIndex = 0;
	// Code
	ZeroMemory(&pfd, sizeof(PIXELFORMATDESCRIPTOR));
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cRedBits = 8;
	pfd.cGreenBits = 8;
	pfd.cBlueBits = 8;
	pfd.cAlphaBits = 8;

	// Get DC
	ghdc = GetDC(ghwnd);

	// choose pixel Format
	iPixelFormatIndex = ChoosePixelFormat(ghdc, &pfd);

	if (iPixelFormatIndex == 0)
	{
		return -1;
	}

	if (SetPixelFormat(ghdc, iPixelFormatIndex, &pfd) == FALSE)
	{
		return -2;
	}

	// Ceate opengl rendering context
	ghrc = wglCreateContext(ghdc);
	if (ghrc == NULL)
	{
		return -3;
	}

	if (wglMakeCurrent(ghdc, ghrc) == FALSE)
	{
		return -4;
	}
	// here stars opengl code
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	// Depth Related Functions
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glShadeModel(GL_SMOOTH);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	// Initialization of Matrx Arrays
	// Identity Matrix
	identityMatrix[0] = 1.0f; identityMatrix[4] = 0.0f; identityMatrix[8] = 0.0f; identityMatrix[12] = 0.0f;
	identityMatrix[1] = 0.0f; identityMatrix[5] = 1.0f; identityMatrix[9] = 0.0f; identityMatrix[13] = 0.0f;
	identityMatrix[2] = 0.0f; identityMatrix[6] = 0.0f; identityMatrix[10] = 1.0f; identityMatrix[14] = 0.0f;
	identityMatrix[3] = 0.0f; identityMatrix[7] = 0.0f; identityMatrix[11] = 0.0f; identityMatrix[15] = 1.0f;


	// Translation Matrix
	translationMatrix[0] = 1.0f; translationMatrix[4] = 0.0f; translationMatrix[8] = 0.0f; translationMatrix[12] = 0.0f;
	translationMatrix[1] = 0.0f; translationMatrix[5] = 1.0f; translationMatrix[9] = 0.0f; translationMatrix[13] = 0.0f;
	translationMatrix[2] = 0.0f; translationMatrix[6] = 0.0f; translationMatrix[10] = 1.0f; translationMatrix[14] = -6.0f;
	translationMatrix[3] = 0.0f; translationMatrix[7] = 0.0f; translationMatrix[11] = 0.0f; translationMatrix[15] = 1.0f;


	//Scale Matrix
	scaleMatrix[0] = 0.75f; scaleMatrix[4] = 0.0f; scaleMatrix[8] = 0.0f; scaleMatrix[12] = 0.0f;
	scaleMatrix[1] = 0.0f; scaleMatrix[5] = 0.75f; scaleMatrix[9] = 0.0f; scaleMatrix[13] = 0.0f;
	scaleMatrix[2] = 0.0f; scaleMatrix[6] = 0.0f; scaleMatrix[10] = 0.75f; scaleMatrix[14] = 0.0f;
	scaleMatrix[3] = 0.0f; scaleMatrix[7] = 0.0f; scaleMatrix[11] = 0.0f; scaleMatrix[15] = 1.0f;

	resize(WIN_WIDTH, WIN_HEIGHT);		// WarmUp Resize Call
	return 0;
}

void resize(int width, int height)
{
	// Code
	if (height == 0)
		height = 1;

	glViewport(0,0, (GLsizei)width, (GLsizei)height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	GLfloat l, r, t, b, n, f;
	if (width <= height)
	{
		l = -100.0f;
		r = 100.0f;
		b = -100.0f * ((GLfloat)height / (GLfloat)width);
		t = 100.0f * ((GLfloat)height / (GLfloat)width);
		n = -100.0f;
		f = 100.0f;
		
	}
	else
	{
		l = -100.0f * ((GLfloat)width / (GLfloat)height);
		r = 100.0f * ((GLfloat)width / (GLfloat)height);
		b = -100.0f;
		t = 100.0f;
		n = -100.0f;
		f = 100.0f;
		
	}
	ortho_matrix[0] = 2 / (r - l);
	ortho_matrix[1] = 0;
	ortho_matrix[2] = 0;
	ortho_matrix[3] = 0;
	ortho_matrix[4] = 0;
	ortho_matrix[5] = 2 / (t - b);
	ortho_matrix[6] = 0;
	ortho_matrix[7] = 0;
	ortho_matrix[8] = 0;
	ortho_matrix[9] = 0;
	ortho_matrix[10] = -2 / (f - n);
	ortho_matrix[11] = 0;
	ortho_matrix[12] = (r + l) / (r - l);
	ortho_matrix[13] = (t + b) / (t - b);
	ortho_matrix[14] = (f + n) / (f - n);
	ortho_matrix[15] = 1;

	glMultMatrixf(ortho_matrix);
}

void drawTriangle(void)
{

	glBegin(GL_TRIANGLES);
	glColor3f(1.0f, 0.0f, 0.0f); glVertex3f(0.0f, 50.0f, 50.0f);
	glColor3f(0.0f, 1.0f, 0.0f); glVertex3f(-50.0f, -50.0f, 50.0f);
	glColor3f(0.0f, 0.0f, 1.0f); glVertex3f(50.0f, -50.0f, 50.0f);

	glEnd();
}

void display(void)
{
	// Code
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	//glLoadIdentity();
	glLoadMatrixf(identityMatrix);
	//glTranslatef(0.0f, 0.0f, -4.0f);
	glMultMatrixf(translationMatrix);
	glMultMatrixf(scaleMatrix);

	/*glRotatef(angleCube, 1.0f, 0.0f, 0.0f);
	glRotatef(angleCube, 0.0f, 1.0f, 0.0f);
	glRotatef(angleCube, 0.0f, 0.0f, 1.0f);*/

	float angle = angleCube * M_PI / 180.0f;
	// X Rotation Matrix
	rotationMatrix_X[0] = 1.0f; rotationMatrix_X[4] = 0.0f;		 rotationMatrix_X[8] = 0.0f;			rotationMatrix_X[12] = 0.0f;
	rotationMatrix_X[1] = 0.0f; rotationMatrix_X[5] = cos(angle);	 rotationMatrix_X[9] = -sin(angle);	rotationMatrix_X[13] = 0.0f;
	rotationMatrix_X[2] = 0.0f; rotationMatrix_X[6] = sin(angle);	 rotationMatrix_X[10] = cos(angle);	rotationMatrix_X[14] = 0.0f;
	rotationMatrix_X[3] = 0.0f; rotationMatrix_X[7] = 0.0f;		 rotationMatrix_X[11] = 0.0f;		rotationMatrix_X[15] = 1.0f;

	//Rotation marix Y
	rotationMatrix_Y[0] = cos(angle);	rotationMatrix_Y[4] = 0.0f;	 rotationMatrix_Y[8] = sin(angle);	rotationMatrix_Y[12] = 0.0f;
	rotationMatrix_Y[1] = 0.0f;			rotationMatrix_Y[5] = 1.0f;	 rotationMatrix_Y[9] = 0.0f;			rotationMatrix_Y[13] = 0.0f;
	rotationMatrix_Y[2] = -sin(angle);	rotationMatrix_Y[6] = 0.0f;	 rotationMatrix_Y[10] = cos(angle);	rotationMatrix_Y[14] = 0.0f;
	rotationMatrix_Y[3] = 0.0f;			rotationMatrix_Y[7] = 0.0f;	 rotationMatrix_Y[11] = 0.0f;		rotationMatrix_Y[15] = 1.0f;
	//Rotation MAtrix Z
	rotationMatrix_Z[0] = cos(angle); rotationMatrix_Z[4] = -sin(angle);	rotationMatrix_Z[8] = 0.0f;	rotationMatrix_Z[12] = 0.0f;
	rotationMatrix_Z[1] = sin(angle); rotationMatrix_Z[5] = cos(angle);		rotationMatrix_Z[9] = 0.0f;	rotationMatrix_Z[13] = 0.0f;
	rotationMatrix_Z[2] = 0.0f;		 rotationMatrix_Z[6] = 0.0f;			rotationMatrix_Z[10] = 1.0f; rotationMatrix_Z[14] = 0.0f;
	rotationMatrix_Z[3] = 0.0f;		 rotationMatrix_Z[7] = 0.0f;			rotationMatrix_Z[11] = 0.0f; rotationMatrix_Z[15] = 1.0f;


	/*glMultMatrixf(rotationMatrix_X);
	glMultMatrixf(rotationMatrix_Y);
	glMultMatrixf(rotationMatrix_Z);*/
	drawTriangle();
	SwapBuffers(ghdc);
}

void update(void)
{
	angleCube += speed;
	anglePyramid += speed;

	if (angleCube <= -360.0f)
	{
		angleCube = 0.0f;
	}

	if (anglePyramid >= 360.0f)
	{
		anglePyramid = 0.0f;
	}
}

void uninitialize(void)
{
	// Function Declaration
	void ToggleFullScreen(void);

	// Code
	if (gbFullScreen)
	{
		ToggleFullScreen();
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
		fprintf(gpFile, "Log File SuccessFully CLosed\n");
		fclose(gpFile);
		gpFile = NULL;
	}
}