// Header Files
#include<windows.h>
#include "OGL.h"
#include<stdio.h>	// For FILE_IO()
#include<stdlib.h>	//For Exit()
#include<cmath>
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
GLUquadric* quadric = NULL;
GLfloat speed = 0.1f;

GLfloat lightZeroAmbient[] = { 0.0f, 0.0f, 0.0f, 0.0f };
GLfloat lightZeroDiffuse[] = { 1.0f, 0.0f, 0.0f, 1.0f };
GLfloat lightZeroSpecular[] = { 1.0f, 0.0f, 0.0f, 1.0f };
GLfloat lightZeroPosition[] = { 0.0f, 0.0f, 0.0f, 1.0f };

GLfloat lightOneAmbient[] = { 0.0f, 0.0f, 0.0f, 0.0f };
GLfloat lightOneDiffuse[] = { 0.0f, 1.0f, 0.0f, 1.0f };
GLfloat lightOneSpecular[] = { 0.0f, 1.0f, 0.0f, 1.0f };
GLfloat lightOnePosition[] = { 0.0f, 0.0f, 0.0f, 1.0f };

GLfloat lightTwoAmbient[] = { 0.0f, 0.0f, 0.0f, 0.0f };
GLfloat lightTwoDiffuse[] = { 0.0f, 0.0f, 1.0f, 1.0f };
GLfloat lightTwoSpecular[] = { 0.0f, 0.0f, 1.0f, 1.0f };
GLfloat lightTwoPosition[] = { 0.0f, 0.0f, 0.0f, 1.0f };

GLfloat materialAmbient[] = { 0.0f, 0.0f, 0.0f, 0.0f };
GLfloat materialDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat materialSpecular[] = { 1.0f, 1.0f, 1.0f, 0.0f };
GLfloat matrialShininess[] = { 50.0f, 50.0f, 50.0f, 50.0f };

GLfloat lightAngleZero = 0.0f;
GLfloat lightAngleOne = 0.0f;
GLfloat lightAngleTwo = 0.0f;

BOOL gbLight = FALSE;

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
		case 'L':
		case 'l':
			if (gbLight == FALSE)
			{
				glEnable(GL_LIGHTING);
				gbLight = TRUE;
			}
			else
			{
				glDisable(GL_LIGHTING);
				gbLight = FALSE;
			}
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
	pfd.cDepthBits = 32;

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

	quadric = gluNewQuadric();

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// Depth Related Functions
	glClearDepth(1.0f);

	glLightfv(GL_LIGHT0, GL_AMBIENT, lightZeroAmbient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightZeroDiffuse);
	glLightfv(GL_LIGHT0, GL_POSITION, lightZeroPosition);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightZeroSpecular);

	glLightfv(GL_LIGHT1, GL_AMBIENT, lightOneAmbient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, lightOneDiffuse);
	glLightfv(GL_LIGHT1, GL_POSITION, lightOnePosition);
	glLightfv(GL_LIGHT1, GL_SPECULAR, lightOneSpecular);

	glLightfv(GL_LIGHT2, GL_AMBIENT, lightTwoAmbient);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, lightTwoDiffuse);
	glLightfv(GL_LIGHT2, GL_POSITION, lightTwoPosition);
	glLightfv(GL_LIGHT2, GL_SPECULAR, lightTwoSpecular);

	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);
	glMaterialfv(GL_FRONT, GL_SHININESS, matrialShininess);

	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHT2);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glShadeModel(GL_SMOOTH);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHT2);

	resize(WIN_WIDTH, WIN_HEIGHT);		// WarmUp Resize Call
	return 0;
}

void resize(int width, int height)
{
	// Code
	if (height == 0)
		height = 1;

	glViewport(0, 0, (GLsizei)width, (GLsizei)height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);
}

void drawSphere(void)
{
	// Push initial matrix.
	glPushMatrix();

	// Rotate the sphere by 90 degree on x-axis
	// because sphere created by GLUT has it rotated in x-axis
	// The north-south poles are on x-axis instead of y-axis.
	glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// Draw the sun

	// 3rd parameter is for slices (like longitudes)
	// 4th parameter is for stacks (like latitudes)
	// Higher the value of 3rd and 4th parameters, i.e. more the sub-divisions,
	// more circular the sphere will look.
	gluSphere(quadric, 1, 50, 50);

	// Pop back to initial state.
	glPopMatrix();
}

void display(void)
{
	// Code
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	/*glTranslatef(0.0f, 0.0f, 0.55f);
	glRotatef(lightAngleZero, 1.0f, 0.0f, 0.0f);
	lightZeroPosition[2] = lightAngleZero;*/
	float angle = lightAngleZero * (3.14159265358979323846 / 180);
	float x = 5.0f * cos(angle);
	float y = 5.0f * sin(angle);
	lightZeroPosition[1] = x;
	lightZeroPosition[2] = y;

	glLightfv(GL_LIGHT0, GL_POSITION, lightZeroPosition);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	angle = lightAngleZero * (3.14159265358979323846 / 180);
	x = 5.0f * cos(angle);
	y = 5.0f * sin(angle);
	lightOnePosition[0] = x;
	lightOnePosition[2] = y;
	glLightfv(GL_LIGHT1, GL_POSITION, lightOnePosition);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	angle = lightAngleZero * (3.14159265358979323846 / 180);
	x = 5.0f * cos(angle);
	y = 5.0f * sin(angle);
	lightTwoPosition[0] = x;
	lightTwoPosition[1] = y;
	glLightfv(GL_LIGHT2, GL_POSITION, lightTwoPosition);

	glTranslatef(0.0f, 0.0f, -4.0f);
	drawSphere();
	SwapBuffers(ghdc);
}

void update(void)
{
	// code
	lightAngleZero += speed;
	lightAngleOne += speed;
	lightAngleTwo += speed;

	if (lightAngleZero <= -360.0f)
	{
		lightAngleZero = 0.0f;
	}

	if (lightAngleOne >= 360.0f)
	{
		lightAngleOne = 0.0f;
	}

	if (lightAngleTwo >= 360.0f)
	{
		lightAngleTwo = 0.0f;
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