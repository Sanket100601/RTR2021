#include<windows.h>
#include<stdio.h>    //for FILE 
#include<stdlib.h>        //for exit();
#include"OGL.h"

#include<gl/glew.h>   //this line must be before the gl.h
#include<GL/gl.h>


//blue screen
#define WIN_WIDTH 800
#define WIN_HEIGHT 600

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

//OpenGL Library
#pragma comment(lib,"glew32.lib")
#pragma comment(lib,"OpenGL32.lib")
 
//Global  FILE Variable
FILE* gFile = NULL;

//Global Hwnd 
HWND gHwnd = NULL;
HDC gHdc = NULL;
HGLRC ghrc = NULL;  //handel Graphic library renderic contexs	

//Global active Window
BOOL bgActiveWindow = FALSE;
BOOL bFullScreen = FALSE;



int  WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR szCmdLine, int iCmdShow) {

	//function declaration
	int initialize(void);
	void display(void);
	void update(void);

	void uninitialize(void);
	HWND hwnd;
	MSG msg;
	TCHAR szAppName[] = TEXT("MyWindow");
	WNDCLASSEX wndclass;;

	BOOL bDone = FALSE;
	int iRetVal = 0;
	if (fopen_s(&gFile, "Log.txt", "w") != 0) {
		MessageBox(NULL, TEXT("Log File Creation Failed"), TEXT("File I/O Error"), MB_OK);
		exit(0);
	}
	else {
		fprintf(gFile, "Log file is open Successfully...\n");
	}


	wndclass.cbSize = sizeof(wndclass);
	wndclass.style = CS_VREDRAW | CS_HREDRAW | CS_OWNDC;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.lpszClassName = szAppName;
	wndclass.lpfnWndProc = WndProc;
	wndclass.lpszMenuName = NULL;
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(MYICON));
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hInstance = hInstance;
	wndclass.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(MYICON));

	RegisterClassEx(&wndclass);

	//for getting the width and height of window
	int x = GetSystemMetrics(SM_CXSCREEN);
	int y = GetSystemMetrics(SM_CYSCREEN);

	hwnd = CreateWindowEx(WS_EX_APPWINDOW, szAppName,
		TEXT("Blue Color Screen"),
		WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_VISIBLE,
		/*CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,*/
		x / 2 - WIN_WIDTH / 2,
		y / 2 - WIN_HEIGHT / 2,
		WIN_WIDTH,
		WIN_HEIGHT,
		NULL,
		NULL,
		hInstance,
		NULL);

	gHwnd = hwnd;
	//
	iRetVal = initialize();
	//uninitialize();
	if (iRetVal == -1) {
		fprintf(gFile, "Choose Pixel Formate Failed\n");
		uninitialize();
	}
	else if (iRetVal == -2) {
		fprintf(gFile, "Set Pixel Formate Failed\n");
		uninitialize();
	}
	else if (iRetVal == -3) {
		fprintf(gFile, "Create OpenGL Contex Failed \n");
		uninitialize();;
	}
	else if (iRetVal == -4) {
		fprintf(gFile, "Making OpenGl Context as current contex failed\n");
		uninitialize();
	}
	else if (iRetVal == -6) {
		fprintf(gFile,"glew init failed\n");
		uninitialize();
	}
	else {
		fprintf(gFile, "Initialization successfully\n");
	}
	ShowWindow(hwnd, iCmdShow);

	//forGrounding and focusing the window
	SetForegroundWindow(hwnd);
	SetFocus(hwnd);

	//Game Loop

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
			if (bgActiveWindow == TRUE) {
				//game loop
				display();

				update();
			}
		}

	}
	uninitialize();
	return (msg.wParam);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT imsg, WPARAM wParam, LPARAM lParam) {
	void TogglefullScreen();
	void resize(int, int);
	

	switch (imsg) {
	case WM_SETFOCUS:
		bgActiveWindow = TRUE;
		break;
	case WM_KILLFOCUS:
		bgActiveWindow = FALSE;
		break;
	case WM_ERASEBKGND:
		return 0;
	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;
	case WM_SIZE:
		resize(LOWORD(lParam), HIWORD(lParam));
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_CHAR:
		switch (wParam) {
		case 'F':
		case 'f':
			TogglefullScreen();
			break;
		case 27:
			PostQuitMessage(0);
			break;
		}
		break;
	default:
		break;
	}
	return(DefWindowProc(hwnd, imsg, wParam, lParam));


}
void TogglefullScreen() {

	MONITORINFO mi;
	static WINDOWPLACEMENT wp;
	static DWORD dwStyle;



	wp.length = sizeof(WINDOWPLACEMENT);
	if (bFullScreen == FALSE) {
		dwStyle = GetWindowLong(gHwnd, GWL_STYLE);
		if (dwStyle & WS_OVERLAPPEDWINDOW) {
			mi.cbSize = sizeof(MONITORINFO);

			if (GetWindowPlacement(gHwnd, &wp) && GetMonitorInfo(MonitorFromWindow(gHwnd, MONITORINFOF_PRIMARY), &mi)) {
				SetWindowLong(gHwnd, GWL_STYLE, dwStyle & ~WS_OVERLAPPEDWINDOW);
				SetWindowPos(gHwnd, HWND_TOP, mi.rcMonitor.left, mi.rcMonitor.top, mi.rcMonitor.right - mi.rcMonitor.left, mi.rcMonitor.bottom - mi.rcMonitor.top, SWP_NOZORDER | SWP_FRAMECHANGED);
			}
		}
		ShowCursor(FALSE);
		bFullScreen = TRUE;
	}
	else {
		SetWindowLong(gHwnd, GWL_STYLE, dwStyle | WS_OVERLAPPEDWINDOW);
		SetWindowPlacement(gHwnd, &wp);
		SetWindowPos(gHwnd, HWND_TOP, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_FRAMECHANGED | SWP_NOZORDER | SWP_NOOWNERZORDER);
		ShowCursor(TRUE);
		bFullScreen = FALSE;
	}


}

int initialize(void) {
	//function declaration
	void resize(int, int);
	void printGlInfo(void);
	void uninitialize(void);


	GLint status;
	GLint infoLogLength;
	char* log = NULL;

	

	//variable declaration
	PIXELFORMATDESCRIPTOR pfd;    
	int iPixelFormatIndex = 0;


	//code
	ZeroMemory(&pfd, sizeof(PIXELFORMATDESCRIPTOR));  //  or   memset((void*)&pfd,NULL,sizeof(PIXELFORMATEDESCRIPTOR));

	//initializing the pfd structure

	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cRedBits = 8;
	pfd.cGreenBits = 8;
	pfd.cBlueBits = 8;
	pfd.cAlphaBits = 8;
	pfd.cDepthBits = 32;  //24 alos can be done

	gHdc = GetDC(gHwnd);
	//ChoosePixelformate
	iPixelFormatIndex = ChoosePixelFormat(gHdc, &pfd);
	if (iPixelFormatIndex == 0) {
		return -1;
	}
	//SetChoosenPixelFormat
	if (SetPixelFormat(gHdc, iPixelFormatIndex, &pfd) == FALSE) {
		return -2;
	}

	//CreatOpenGlContex
	ghrc = wglCreateContext(gHdc);  
	if (ghrc == NULL) {
		return -3;
	}

	//Make the rendering contex as current contex

	if (wglMakeCurrent(gHdc, ghrc) == FALSE) {
		return -4;  
	}


	//GLEW initalization
	if (glewInit()!=GLEW_OK) {
		return -5;
	}

	//print openGl Info
	printGlInfo();


	//here start the OpenGl Code
	glClearColor(0.0f, 0.0f, 1.0f, 1.0f);  //here only we say that clear with mention color but coloring is not done here
	
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	//this two line not use in pp as its depricated
	//glShadeModel(GL_SMOOTH);
	//glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);


	resize(WIN_WIDTH, WIN_HEIGHT);
	return 0;


}
void printGlInfo(void) {

	//local variable declartion

	GLint numExtensions=0;


	//code
	fprintf(gFile, "OpenGLVendor :    %s\n", glGetString(GL_VENDOR));
	fprintf(gFile, "OpenGLRenderer :  %s\n", glGetString(GL_RENDERER));
	fprintf(gFile, "OpenGLVersion :   %s\n", glGetString(GL_VERSION));
	fprintf(gFile, "OpenGLSLVersion : %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
	//fprintf(gFile, "OpenGLVersion :   %s\n", glGetString(GL_VERSION));

	glGetIntegerv(GL_NUM_EXTENSIONS,&numExtensions);

	fprintf(gFile,"Number of Suppported Extension %d\n",numExtensions);
	for (int i = 0; i < numExtensions; i++) {
		fprintf(gFile, "%s\n",glGetStringi(GL_EXTENSIONS,i));
	}
}
void resize(int width, int height) {
	if (height == 0)
		height = 1;

	glViewport(0, 0,GLsizei(width), GLsizei(height));
}
void display(void) {

	//code
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



	SwapBuffers(gHdc);
}
void update(void) {

	//code

}

void uninitialize(void) {
	void TogglefullScreen();
	GLsizei numAttachShader;
	//to Avoid  exit window in full screen mode
	if (bFullScreen) {
		TogglefullScreen();
	}

	if (wglGetCurrentContext() == ghrc) {
		wglMakeCurrent(NULL, NULL);
	}
	if (ghrc) {
		wglDeleteContext(ghrc);
		ghrc = NULL;
	}
	if (gHdc) {
		ReleaseDC(gHwnd, gHdc);
		gHdc = NULL;
	}
	if (gHwnd) {
		DestroyWindow(gHwnd);
		gHwnd = NULL;
	}
	if (gFile) {
		fprintf(gFile, "Log F	ile is Close Successfully...\n");
		fclose(gFile);
		gFile = NULL;
	}
}

