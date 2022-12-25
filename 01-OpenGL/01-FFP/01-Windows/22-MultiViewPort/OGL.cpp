// header files
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include "OGL.h"

// OpenGL header files
#include <GL/gl.h>
#include <GL/glu.h>

#define WIN_WIDTH 800
#define WIN_HEIGHT 600

// OpenGL libraries
#pragma comment(lib,"OpenGL32.lib")
#pragma comment(lib,"Glu32.lib")

// global function declarations
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

// global variable declarations
FILE* gbFile = NULL;
HWND ghwnd = NULL;
HDC ghdc = NULL;
HGLRC ghrc = NULL;						
BOOL gbFullScreen = FALSE;
BOOL gbActiveWindow = FALSE;
int win_width = 0;
int win_height = 0;

// entry point functions
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow) {

	// function declarations
	int initialize(void);
	void display(void);
	void update(void);
	void uninitialize(void);

	// local variable declarations
	WNDCLASSEX wndclass;
	HWND hwnd;
	MSG msg;
	TCHAR szAppName[] = TEXT("MyWindow");
	BOOL bDone = FALSE;
	int iRetVal = 0;

	// code
	if (fopen_s(&gbFile, "Log.txt", "w") != 0) {
		MessageBox(NULL, TEXT("Creation of log file failed !!! Exiting"), TEXT("FILE I/O Error"), MB_OK);
		exit(0);
	}
	else {
		fprintf(gbFile, "Log File Successfully Created");
	}

	// initialization of WNDCLASS structure
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

	// registering above WNDCLASS
	RegisterClassEx(&wndclass);

	// create the window
	hwnd = CreateWindowEx(WS_EX_APPWINDOW,
		szAppName,
		TEXT("OpenGL Window"),
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

	// initialize
	iRetVal = initialize();
	if (iRetVal == -1) {
		fprintf(gbFile, "Chose pixel format failed\n");
		uninitialize();
	}
	else if (iRetVal == -2) {
		fprintf(gbFile, "Set pixel format failed\n");
		uninitialize();
	}
	else if (iRetVal == -3) {
		fprintf(gbFile, "Create OpenGL context failed\n");
		uninitialize();
	}
	else if (iRetVal == -4) {
		fprintf(gbFile, "Making OpenGL context as current context failed\n");
		uninitialize();
	}
	else {
		fprintf(gbFile, "Success");
	}

	// show window
	ShowWindow(hwnd, iCmdShow);

	// foregrounding and focusing the window
	SetForegroundWindow(hwnd);
	SetFocus(hwnd);

	// game loop
	while (bDone == FALSE) {
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			if (msg.message == WM_QUIT) {
				bDone = TRUE;
			}
			else {
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else {
			if (gbActiveWindow == TRUE) {
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

// CALlBACK FUNCTION
LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam) {

	// function declarations
	void ToggleFullScreen(void);
	void resize(int, int);
	void uninitialize(void);

	switch (iMsg) {

	case WM_CREATE:
	{
		int screenWidth, screenHeight;
		RECT rect;

		// get screen size 
		screenWidth = GetSystemMetrics(SM_CXSCREEN);
		screenHeight = GetSystemMetrics(SM_CYSCREEN);

		// get window size
		GetClientRect(hwnd, &rect);

		// reset the value in rect
		int clientWidth = rect.right - rect.left;
		int clientHeight = rect.bottom - rect.top;

		// move window to the specified position
		SetWindowPos(hwnd, NULL, (screenWidth - clientWidth) / 2, (screenHeight - clientHeight) / 2, clientWidth, clientHeight, 0);
	}
	break;

	case WM_KEYDOWN:

		switch (wParam) 
		{

		case '1':
			glViewport(0, 0, win_width / 2, win_height / 2);
			break;

		case '2':
			glViewport(win_width / 2, 0, win_width / 2, win_height / 2);
			break;

		case '3':
			glViewport(win_width / 2, win_height / 2, win_width / 2, win_height / 2);
			break;

		case '4':
			glViewport(0, win_height / 2, win_width / 2, win_height / 2);
			break;

		case '5':
			glViewport(0, 0, win_width, win_height / 2);
			break;

		case '6':
			glViewport(0, win_height / 2, win_width, win_height / 2);
			break;

		case '7':
			glViewport(0, 0, win_width / 2, win_height);
			break;

		case '8':
			glViewport(win_width / 2, 0, win_width / 2, win_height);
			break;

		case '9':
			glViewport(win_width / 4, win_height / 4, win_width / 2, win_height / 2);
			break;

		case 27:
			DestroyWindow(hwnd);
			break;

		default:
			glViewport(0, 0, win_width, win_height);
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
		return 0;

	case WM_SIZE:
		resize(LOWORD(lParam), HIWORD(lParam));
		break;

	case WM_CHAR:

		switch (wParam) {

		case 'F':
		case 'f':
			ToggleFullScreen();
			break;

		default:
			break;

		}
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


void ToggleFullScreen(void) {

	// variable declarations
	static DWORD dwStyle;
	static WINDOWPLACEMENT wp;
	MONITORINFO mi;

	// code
	wp.length = sizeof(WINDOWPLACEMENT);

	if (gbFullScreen == FALSE) {
		dwStyle = GetWindowLong(ghwnd, GWL_STYLE);
		if (dwStyle & WS_OVERLAPPEDWINDOW) {
			mi.cbSize = sizeof(MONITORINFO);

			if (GetWindowPlacement(ghwnd, &wp) && GetMonitorInfo(MonitorFromWindow(ghwnd, MONITORINFOF_PRIMARY), &mi)) {
				SetWindowLong(ghwnd, GWL_STYLE, dwStyle & ~WS_OVERLAPPEDWINDOW);
				SetWindowPos(ghwnd, HWND_TOP, mi.rcMonitor.left, mi.rcMonitor.top, mi.rcMonitor.right - mi.rcMonitor.left, mi.rcMonitor.bottom - mi.rcMonitor.top, SWP_NOZORDER | SWP_FRAMECHANGED);
			}
			ShowCursor(FALSE);
			gbFullScreen = TRUE;
		}
	}
	else {
		SetWindowLong(ghwnd, GWL_STYLE, dwStyle | WS_OVERLAPPEDWINDOW);
		SetWindowPlacement(ghwnd, &wp);
		SetWindowPos(ghwnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOOWNERZORDER | SWP_NOZORDER | SWP_FRAMECHANGED);

		ShowCursor(TRUE);
		gbFullScreen = FALSE;
	}

}

int initialize(void) {

	// function declarations
	void resize(int, int);

	// variable declarations
	PIXELFORMATDESCRIPTOR pfd;
	int iPixelFormatIndex = 0;

	// code
	// initialization of PIXELFORMATDESCRIPTOR structure
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

	// GetDC
	ghdc = GetDC(ghwnd);

	// choose pixel format
	iPixelFormatIndex = ChoosePixelFormat(ghdc, &pfd);
	if (iPixelFormatIndex == 0) {
		return -1;
	}

	// Set the chosen pixel format
	if (SetPixelFormat(ghdc, iPixelFormatIndex, &pfd) == FALSE) {
		return -2;
	}

	// create OpenGL rendering context 
	ghrc = wglCreateContext(ghdc);								// bridging API
	if (ghrc == NULL) {
		return -3;
	}

	// make the rendering context as the current context
	if (wglMakeCurrent(ghdc, ghrc) == FALSE) {
		return -4;
	}

	// here starts OpenGL code
	// clear the screen using Black Colour
	glClearColor(0.0f, 0.0f, 1.0f, 1.0f);

	//warm up resize call
	resize(WIN_WIDTH, WIN_HEIGHT);
	return 0;
}

void resize(int width, int height) {

	// code
	if (height == 0) {
		height = 1;
	}
	win_width = width;
	win_height = height;
	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(45.0f,
		((GLfloat)width / (GLfloat)height),
		0.1f,
		100.0f);
}

void display(void) {

	// code
	glClear(GL_COLOR_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0.0f, 0.0f, -3.0f);

	glBegin(GL_TRIANGLES);
	glVertex3f(0.0f, 1.0f, 0.0f);
	glVertex3f(-1.0f, -1.0f, 0.0f);
	glVertex3f(1.0f, -1.0f, 0.0f);
	glEnd();
	SwapBuffers(ghdc);

}

void update(void) {

	// code

}

void uninitialize(void) {

	// function declarations
	void ToggleFullScreen(void);

	// code
	if (gbFullScreen) {
		ToggleFullScreen();			 
									
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

	if (ghwnd) {
		DestroyWindow(ghwnd);		
		ghwnd = NULL;
	}

	if (gbFile) {
		fprintf(gbFile, "Log File successfully closed");
		fclose(gbFile);
		gbFile = NULL;
	}
}
