// headers
#include <windows.h>
#include <stdio.h>
#include <gl/GL.h>

#include "OGL.h"

#pragma comment(lib, "opengl32.lib")

// macros
#define WIN_WIDTH  800
#define WIN_HEIGHT 600

// global function declarations
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

// global variables
HDC   ghdc = NULL;
HGLRC ghrc = NULL;

BOOL gbFullscreen = FALSE;
BOOL gbActiveWindow = FALSE;

HWND  ghwnd = NULL;
FILE* gpFile = NULL;

DWORD dwStyle;
WINDOWPLACEMENT wpPrev = { sizeof(WINDOWPLACEMENT) };


// WinMain()
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow)
{
	// function declarations
	void initialize(void);
	void display(void);

	// variable declarations
	BOOL bDone = FALSE;
	WNDCLASSEX wndclass;
	HWND hwnd;
	MSG msg;
	TCHAR szAppName[] = TEXT("MyApp");

	// code
	// open file for logging
	if (fopen_s(&gpFile, "Log.txt", "w") != 0)
	{
		MessageBox(NULL, TEXT("Cannot open RMCLog.txt file.."), TEXT("Error"), MB_OK | MB_ICONERROR);
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
	hwnd = CreateWindowEx(WS_EX_APPWINDOW,
		szAppName,
		TEXT("OGL"),
		WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_VISIBLE,
		(width / 2) - 400,
		(height / 2) - 300,
		WIN_WIDTH,
		WIN_HEIGHT,
		NULL,
		NULL,
		hInstance,
		NULL);

	ghwnd = hwnd;

	initialize();

	ShowWindow(hwnd, iCmdShow);
	SetForegroundWindow(hwnd);
	SetFocus(hwnd);

	// Game Loop!
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
				// call update() here for OpenGL rendering

				// call display() here for OpenGL rendering
				display();
			}
		}
	}

	return((int)msg.wParam);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	// function declaration
	void display(void);
	void resize(int, int);
	void uninitialize();
	void ToggleFullscreen(void);

	// code
	switch (iMsg)
	{

	case WM_SETFOCUS:
		gbActiveWindow = TRUE;
		break;

	case WM_KILLFOCUS:
		gbActiveWindow = FALSE;
		break;

	case WM_SIZE:
		resize(LOWORD(lParam), HIWORD(lParam));
		break;

	case WM_CHAR:
		switch (wParam)
		{
		case 27:
			DestroyWindow(hwnd);
			break;

		case 'f':
		case 'F':
			ToggleFullscreen();
			break;

		default:
			break;
		}
		break;

	case WM_ERASEBKGND:
		return(0);

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

	return(DefWindowProc(hwnd, iMsg, wParam, lParam));
}

void ToggleFullscreen(void)
{
	// local variables
	MONITORINFO mi = { sizeof(MONITORINFO) };

	// code
	if (gbFullscreen == FALSE)
	{
		dwStyle = GetWindowLong(ghwnd, GWL_STYLE);
		if (dwStyle & WS_OVERLAPPEDWINDOW)
		{
			if (GetWindowPlacement(ghwnd, &wpPrev) &&
				GetMonitorInfo(MonitorFromWindow(ghwnd, MONITORINFOF_PRIMARY), &mi))
			{
				SetWindowLong(ghwnd, GWL_STYLE, dwStyle & ~WS_OVERLAPPEDWINDOW);
				SetWindowPos(ghwnd, HWND_TOP,
					mi.rcMonitor.left,
					mi.rcMonitor.top,
					mi.rcMonitor.right - mi.rcMonitor.left,
					mi.rcMonitor.bottom - mi.rcMonitor.top,
					SWP_NOZORDER | SWP_FRAMECHANGED);
			}
		}
		ShowCursor(FALSE);
		gbFullscreen = TRUE;
	}
	else
	{
		SetWindowLong(ghwnd, GWL_STYLE, dwStyle | WS_OVERLAPPEDWINDOW);
		SetWindowPlacement(ghwnd, &wpPrev);
		SetWindowPos(ghwnd, HWND_TOP,
			0, 0, 0, 0,
			SWP_NOZORDER | SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE | SWP_NOOWNERZORDER);
		ShowCursor(TRUE);
		gbFullscreen = FALSE;
	}
}


void initialize(void)
{
	// function declarations
	void resize(int, int);

	// variable declarations
	PIXELFORMATDESCRIPTOR pfd;
	int iPixelFormatIndex;

	// code
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

	iPixelFormatIndex = ChoosePixelFormat(ghdc, &pfd);
	if (iPixelFormatIndex == 0)
	{
		fprintf(gpFile, "ChoosePixelFormat() failed..\n");
		DestroyWindow(ghwnd);
	}

	if (SetPixelFormat(ghdc, iPixelFormatIndex, &pfd) == FALSE)
	{
		fprintf(gpFile, "SetPixelFormat() failed..\n");
		DestroyWindow(ghwnd);
	}

	ghrc = wglCreateContext(ghdc);
	if (ghrc == NULL)
	{
		fprintf(gpFile, "wglCreateContext() failed..\n");
		DestroyWindow(ghwnd);
	}

	if (wglMakeCurrent(ghdc, ghrc) == FALSE)
	{
		fprintf(gpFile, "wglMakeCurrent() failed..\n");
		DestroyWindow(ghwnd);
	}

	// set clear color
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	// warm-up resize call
	resize(WIN_WIDTH, WIN_HEIGHT);
}

void resize(int width, int height)
{
	// code
	if (height == 0)
		height = 1;

	glViewport(0, 0, (GLsizei)width, (GLsizei)height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	if (width <= height)
	{
		glOrtho(-100.0f,
			100.0f,
			-100.0f * ((GLfloat)height / (GLfloat)width),
			100.0f * ((GLfloat)height / (GLfloat)width),
			-100.0f,
			100.0f);
	}
	else
	{
		glOrtho(-100.0f * ((GLfloat)width / (GLfloat)height),
			100.0f * ((GLfloat)width / (GLfloat)height),
			-100.0f,
			100.0f,
			-100.0f,
			100.0f);
	}
}

void display(void)
{
	// code
	glClear(GL_COLOR_BUFFER_BIT);

	// Pattern 1
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(-120.0f, 20.0f, 1.0f);

	glPointSize(4.0f);
	glBegin(GL_POINTS);

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			glVertex2f(10 * i, 10 * j);
		}
	}

	glEnd();

	// Pattern 2
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(-20.0f, 20.0f, 1.0f);

	glLineWidth(2.0);
	glBegin(GL_LINES);

	for (int i = 0; i < 3; i++)
	{
		glVertex2f(0, 10 * (i + 1));
		glVertex2f(10 * 3, 10 * (i + 1));

		glVertex2f(10 * i, 0);
		glVertex2f(10 * i, 10 * 3);
	}

	glVertex2f(0, 0);
	glVertex2f(10 * 3, 10 * 3);

	for (int i = 1; i < 3; i++)
	{
		glVertex2f(0, 10 * i);
		glVertex2f(10 * (3 - i), 10 * 3);

		glVertex2f(10 * i, 0);
		glVertex2f(10 * 3, 10 * (3 - i));

	}

	glEnd();

	// Pattern 3
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(80.0f, 20.0f, 1.0f);

	glBegin(GL_LINES);

	for (int i = 0; i < 4; i++)
	{
		glVertex2f(0, 10 * i);
		glVertex2f(10 * 3, 10 * i);

		glVertex2f(10 * i, 0);
		glVertex2f(10 * i, 10 * 3);
	}
	glEnd();

	// Pattern 4
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(-120.0f, -60.0f, 1.0f);

	glBegin(GL_LINES);

	for (int i = 0; i < 4; i++)
	{
		glVertex2f(0, 10 * i);
		glVertex2f(10 * 3, 10 * i);

		glVertex2f(10 * i, 0);
		glVertex2f(10 * i, 10 * 3);
	}

	glVertex2f(0, 0);
	glVertex2f(10 * 3, 10 * 3);

	for (int i = 1; i < 3; i++)
	{
		glVertex2f(0, 10 * i);
		glVertex2f(10 * (3 - i), 10 * 3);

		glVertex2f(10 * i, 0);
		glVertex2f(10 * 3, 10 * (3 - i));

	}

	glEnd();

	// Pattern 5
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(-20.0f, -60.0f, 1.0f);

	glBegin(GL_LINES);

	for (int i = 0; i < 4; i++)
	{
		glVertex2f(0, 10 * 3);
		glVertex2f(10 * 3, 10 * i);

		glVertex2f(0, 10 * 3);
		glVertex2f(10 * i, 0);
	}

	glVertex2f(0, 0);
	glVertex2f(10 * 3, 0);

	glVertex2f(10 * 3, 0);
	glVertex2f(10 * 3, 10 * 3);


	glEnd();

	// Pattern 6
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(80.0f, -60.0f, 1.0f);

	glPointSize(2.0);
	glBegin(GL_QUADS);

	for (int i = 0; i < 3; i++)
	{
		switch (i) {
		case 0:
			glColor3f(1.0f, 0.0f, 0.0f);
			break;

		case 1:
			glColor3f(0.0f, 1.0f, 0.0f);
			break;

		case 2:
			glColor3f(0.0f, 0.0f, 1.0f);
			break;
		}

		glVertex2f(10 * (i + 1), 10 * 3);
		glVertex2f(10 * i, 10 * 3);
		glVertex2f(10 * i, 0);
		glVertex2f(10 * (i + 1), 0);

	}
	glEnd();

	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_LINES);
	for (int i = 0; i < 4; i++)
	{
		glVertex2f(0, 10 * i);
		glVertex2f(10 * 3, 10 * i);

		glVertex2f(10 * i, 0);
		glVertex2f(10 * i, 10 * 3);
	}
	glEnd();

	SwapBuffers(ghdc);
}

void uninitialize(void)
{
	// code
	if (gbFullscreen == TRUE)
	{
		dwStyle = GetWindowLong(ghwnd, GWL_STYLE);
		SetWindowLong(ghwnd, GWL_STYLE, dwStyle | WS_OVERLAPPEDWINDOW);
		SetWindowPlacement(ghwnd, &wpPrev);
		SetWindowPos(ghwnd,
			HWND_TOP,
			0,
			0,
			0,
			0,
			SWP_NOZORDER | SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE | SWP_NOOWNERZORDER);

		ShowCursor(TRUE);
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

	if (gpFile)
	{
		fprintf(gpFile, "==== Application Terminated ====\n");
		fclose(gpFile);
		gpFile = NULL;
	}
}

