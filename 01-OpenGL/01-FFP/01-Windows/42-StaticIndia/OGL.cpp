#define _USE_MATH_DEFINES
// Header Files
#include<windows.h>
#include "OGL.h"
#include<stdio.h>	// For FILE_IO()
#include<stdlib.h>	//For Exit()
#include<math.h>
// Opengl Header files 
#include <Gl/gl.h>
#include<GL/glu.h>
#define WIN_WIDTH 800
#define WIN_HEIGHT 600

//OpenGl Liabraries
#pragma comment(lib,"OpenGl32.lib")
#pragma comment(lib,"glu32.lib")

//Global functions declarations
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

// Global variable
HWND ghwnd = FALSE;
BOOL gbFullScreen = FALSE;
FILE* gpFile = NULL;
BOOL gbActiveWindow = FALSE;
HDC ghdc = NULL;
HGLRC ghrc = NULL;

const GLfloat orange[] = { 1.0f, 153.0f / 256.0f, 51.0f / 256.0f };
const GLfloat  green[] = { 19.0f / 256.0f, 136.0f / 256.0f, 8.0f / 256.0f };
const float white[] = {1, 1, 1};

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
	resize(WIN_WIDTH, WIN_HEIGHT);		// WarmUp Resize Call
	return 0;
}

void resize(int width, int height)
{
	if (height == 0)
	{
		height = 1;
	}

	glViewport(0, 0, (GLsizei)width, (GLsizei)height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 1.0f, 100.0f);
}

void display(void)
{
	// function declarations
	void I1(void);
	void N(void);
	void D(void);
	void I2(void);
	void A(void);
	void Flag(void);

	// code
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);

	// letters
	glLoadIdentity();
	glTranslatef(0, 0, -2.0f);
	I1();

	glLoadIdentity();
	glTranslatef(0, 0, -2.0f);
	N();

	glLoadIdentity();
	glTranslatef(0, 0, -2.0f);
	D();

	glLoadIdentity();
	glTranslatef(0, 0, -2.0f);
	I2();

	glLoadIdentity();
	glTranslatef(0, 0, -2.0f);
	A();

	glLoadIdentity();
	glTranslatef(0, 0, -2.0f);
	Flag();

	SwapBuffers(ghdc);
}


void I1(void)
{
	glBegin(GL_QUADS);
	{
		glColor3fv(orange);
		glVertex3f(-0.65f, 0.4f, 0.0f);
		glVertex3f(-0.7f, 0.4f, 0.0f);

		glColor3fv(white);
		glVertex3f(-0.7f, 0.0f, 0.0f);
		glVertex3f(-0.65f, 0.0f, 0.0f);

		glColor3fv(white);
		glVertex3f(-0.65f, 0.0f, 0.0f);
		glVertex3f(-0.7f, 0.0f, 0.0f);

		glColor3fv(green);
		glVertex3f(-0.7f, -0.4f, 0.0f);
		glVertex3f(-0.65f, -0.4f, 0.0f);

	}
	glEnd();
}

void N(void)
{
	glBegin(GL_QUADS);
	{
		// first vertical
		glColor3fv(orange);
		glVertex3f(-0.5f, 0.4f, 0.0f);
		glVertex3f(-0.55f, 0.4f, 0.0f);

		glColor3fv(white);
		glVertex3f(-0.55f, 0.0f, 0.0f);
		glVertex3f(-0.5f, 0.0f, 0.0f);

		glColor3fv(white);
		glVertex3f(-0.5f, 0.0f, 0.0f);
		glVertex3f(-0.55f, 0.0f, 0.0f);
		
		glColor3fv(green);
		glVertex3f(-0.55f, -0.4f, 0.0f);
		glVertex3f(-0.5f, -0.4f, 0.0f);

		// second vertical
		glColor3fv(orange);
		glVertex3f(-0.5f, 0.4f, 0.0f);
		glVertex3f(-0.55f, 0.4f, 0.0f);

		glColor3fv(white);
		glVertex3f(-0.48f, 0.0f, 0.0f);
		glVertex3f(-0.42f, 0.0f, 0.0f);

		glColor3fv(white);
		glVertex3f(-0.42f, 0.0f, 0.0f);
		glVertex3f(-0.48f, 0.0f, 0.0f);

		glColor3fv(green);
		glVertex3f(-0.4f, -0.4f, 0.0f);
		glVertex3f(-0.35f, -0.4f, 0.0f);

		// third vertical
		glColor3fv(orange);
		glVertex3f(-0.35f, 0.4f, 0.0f);
		glVertex3f(-0.4f, 0.4f, 0.0f);

		glColor3fv(white);
		glVertex3f(-0.4f, 0.0f, 0.0f);
		glVertex3f(-0.35f, 0.0f, 0.0f);

		glColor3fv(white);
		glVertex3f(-0.35f, 0.0f, 0.0f);
		glVertex3f(-0.4f, 0.0f, 0.0f);

		glColor3fv(green);
		glVertex3f(-0.4f, -0.4f, 0.0f);
		glVertex3f(-0.35f, -0.4f, 0.0f);
	}
	glEnd();
}

void D(void)
{
	glBegin(GL_QUADS);
	{
		// first horizontal
		glColor3fv(orange);
		glVertex3f(0.1f, 0.4f, 0.0f);
		glVertex3f(-0.25f, 0.4f, 0.0f);
		glVertex3f(-0.25f, 0.35f, 0.0f);
		glVertex3f(0.1f, 0.35f, 0.0f);

		// Second horizontal
		glColor3fv(green);
		glVertex3f(0.1f, -0.4f, 0.0f);
		glVertex3f(-0.25f, -0.4f, 0.0f);
		glVertex3f(-0.25f, -0.35f, 0.0f);
		glVertex3f(0.1f, -0.35f, 0.0f);

		/* 1st vertical */
		glColor3fv(orange);
		glVertex3f(-0.15f, 0.4f, 0.0f);
		glVertex3f(-0.2f, 0.4f, 0.0f);

		glColor3fv(white);
		glVertex3f(-0.2f, 0.0f, 0.0f);
		glVertex3f(-0.15f, 0.0f, 0.0f);

		glColor3fv(white);
		glVertex3f(-0.15f, 0.0f, 0.0f);
		glVertex3f(-0.2f, 0.0f, 0.0f);
		
		glColor3fv(green);
		glVertex3f(-0.2f, -0.4f, 0.0f);
		glVertex3f(-0.15f, -0.4f, 0.0f);

		//second vertical
		glColor3fv(orange);
		glVertex3f(0.1f, 0.4f, 0.0f);
		glVertex3f(0.05f, 0.4f, 0.0f);

		glColor3fv(white);
		glVertex3f(0.05f, 0.0f, 0.0f);
		glVertex3f(0.1f, 0.0f, 0.0f);

		glColor3fv(white);
		glVertex3f(0.1f, 0.0f, 0.0f);
		glVertex3f(0.05f, 0.0f, 0.0f);

		glColor3fv(green);
		glVertex3f(0.05f, -0.4f, 0.0f);
		glVertex3f(0.1f, -0.4f, 0.0f);

		

	}
	glEnd();
}

void I2(void)
{
	glBegin(GL_QUADS);
	{
		glColor3fv(orange);
		glVertex3f(0.2f, 0.4f, 0.0f);
		glVertex3f(0.25f, 0.4f, 0.0f);

		glColor3fv(white);
		glVertex3f(0.25f, 0.0f, 0.0f);
		glVertex3f(0.2f, 0.0f, 0.0f);

		glColor3fv(white);
		glVertex3f(0.2f, 0.0f, 0.0f);
		glVertex3f(0.25f, 0.0f, 0.0f);
		
		glColor3fv(green);
		glVertex3f(0.25f, -0.4f, 0.0f);
		glVertex3f(0.2f, -0.4f, 0.0f);
	}
	glEnd();
}

void A(void)
{
	glBegin(GL_QUADS);
	{
		//First horizontal
		glColor3fv(orange);
		glVertex3f(0.75, 0.4, 0.0);
		glVertex3f(0.35, 0.4f, 0.0f);
		glVertex3f(0.35, 0.35f, 0.0f);
		glVertex3f(0.75, 0.35, 0.0);

		// First vertical
		glColor3fv(orange);
		glVertex3f(0.4f, 0.4f, 0.0f);
		glVertex3f(0.35f, 0.4f, 0.0f);

		glColor3fv(white);
		glVertex3f(0.35f, 0.0f, 0.0f);
		glVertex3f(0.4f, 0.0f, 0.0f);

		glColor3fv(white);
		glVertex3f(0.4f, 0.0f, 0.0f);
		glVertex3f(0.35f, 0.0f, 0.0f);
		
		glColor3fv(green);
		glVertex3f(0.35f, -0.4f, 0.0f);
		glVertex3f(0.4f, -0.4f, 0.0f);
	
		// Second Vertical
		glColor3fv(orange);
		glVertex3f(0.75f, 0.4f, 0.0f);
		glVertex3f(0.7f, 0.4f, 0.0f);

		glColor3fv(white);
		glVertex3f(0.7f, 0.0f, 0.0f);
		glVertex3f(0.75f, 0.0f, 0.0f);

		glColor3fv(white);
		glVertex3f(0.75f, 0.0f, 0.0f);
		glVertex3f(0.7f, 0.0f, 0.0f);
		
		glColor3fv(green);
		glVertex3f(0.7f, -0.4f, 0.0f);
		glVertex3f(0.75f, -0.4f, 0.0f);
	}
	glEnd();
}

void Flag(void)
{
	glBegin(GL_QUADS);
	{
		glColor3fv(orange);
		glVertex3f(0.7f, 0.1f, 0.0f);
		glVertex3f(0.4f, 0.1f, 0.0f);
		glVertex3f(0.4f, 0.05f, 0.0f);
		glVertex3f(0.7f, 0.05f, 0.0f);

		glColor3fv(white);
		glVertex3f(0.7f, 0.05f, 0.0f);
		glVertex3f(0.4f, 0.05f, 0.0f);
		glVertex3f(0.4f, 0.0f, 0.0f);
		glVertex3f(0.7f, 0.0f, 0.0f);

		glColor3fv(green);
		glVertex3f(0.7f, 0.0f, 0.0f);
		glVertex3f(0.4f, 0.0f, 0.0f);
		glVertex3f(0.4f, -0.05f, 0.0f);
		glVertex3f(0.7f, -0.05f, 0.0f);
	}
	glEnd();
}

void update(void)
{
	// code
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