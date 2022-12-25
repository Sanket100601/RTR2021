#include<windows.h>
#include<stdio.h>    //for FILE 
#include<stdlib.h>        //for exit();
#include"OGL.h"
#include "vmath.h" 
#include<gl/glew.h>   //this line must be before the gl.h
#include<GL/gl.h>

using namespace vmath;  //for this line we declare our file name as .cpp rather tha .c
//beacuse namespace is work only in .cpp file

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

//PP related global variable
GLuint shaderProgramObject;
enum
{

	VBA_ATTRIBUTE_POSITION = 0,
	VBA_ATTRIBUTE_COLOR,
	VBA_ATTRIBUTE_NORMAL,
	VBA_ATTRIBUTE_TEXTURE0,
};

GLuint VAO;
GLuint VBO_Position;
GLuint VBO_TexCoord;


int keyPress=-1;
GLuint texture_smiley;

GLuint mvpMatrixUniform;
GLuint textureSamplarUniform;
GLuint keypressUniform;

mat4 prespectiveProjectionMatrix; 

int  WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR szCmdLine, int iCmdShow) {

	//function declaration
	int initialize(void);
	void display(void);


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
		TEXT("OGL"),
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

	/*while(GetMessage(&msg,NULL,0,0)){
		TranslateMessage(&msg);
		DispatchMessage(&msg);

	}*/

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
		case 49:
			keyPress = 1;
			
			break;
		case 50:
			keyPress = 2;
			
			break;
		case 51:
			keyPress = 3;
		
			break;
		case 52:
			keyPress = 4;
			
			break;
		default:
	
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
	BOOL LoadGlTexure(GLuint*, TCHAR[]);

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

	
	//Vertex Shader
	const GLchar* vertexShaderSourceCode =
		"#version 400 core" \
		"\n" \
		"in vec4 a_position;" \
		"in vec2 a_texcoord;"\
		"uniform mat4 u_mvpMatrix;" \
		"out vec2 a_texcoord_out;"\
		"void main(void)" \
		"{" \
		"gl_Position=u_mvpMatrix * a_position ;" \
		"a_texcoord_out =a_texcoord; "\
		"}";

	//gl_position he internally fragement la passs kela jata 
	// 	pan color cha tas nai we have to pass this color to fragement as input
	//creating the shader object

	GLuint vertexShaderObject = glCreateShader(GL_VERTEX_SHADER);
	//give the code to shader object
	glShaderSource(vertexShaderObject, 1, (const GLchar**)&vertexShaderSourceCode, NULL);
	//2 :how many string in the shader
	//3 : is shader  array bi=ut we use onlt 1 as 2nd parameter so  give address of that array 
	//4  :if 3 th paramter is array of string then 4th parameter is aary of length of each string of 4th parameter 
	//otherwise put NULL
	
	//compling the shader which is created above
	//it compile the code from human understandable to GPu understandable not as CPU understandable
	glCompileShader(vertexShaderObject);

	glGetShaderiv(vertexShaderObject, GL_COMPILE_STATUS, &status);

	//if error occure then only it go to if statement
	if (status == GL_FALSE) {
		glGetShaderiv(vertexShaderObject, GL_INFO_LOG_LENGTH, &infoLogLength);
		if (infoLogLength > 0) {
			log = (char*)malloc(sizeof(infoLogLength));
			if (log != NULL) {
				GLsizei written;
				glGetShaderInfoLog(vertexShaderObject, infoLogLength, &written, log);
	  			fprintf(gFile, "Vertex Shader Compilation Log :  %s\n", log);
				free(log);
				uninitialize();
			}
		}

	}

	//fragment shader
	const GLchar* fragmentShaderSourceCode =
		"#version 400 core" \
		"\n" \
		"in vec2 a_texcoord_out;" \
		"uniform sampler2D u_texturesampler;"\
		"uniform  int u_keyPress; "\
		"out vec4 FragColor; " \
		"void main(void)" \
		"{" \
	      "if (u_keyPress == 1)"\
			"{"\
				"FragColor = texture(u_texturesampler, a_texcoord_out); " \
			"}"\
			"else"\
			"{"\
				"FragColor = vec4(1.0f,1.0f,1.0f,1.0f);"\

			"}"\
		"}";
	//texture is funcrion
	GLuint fragmentShaderObject = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShaderObject, 1, (const GLchar**)&fragmentShaderSourceCode, NULL);
	glCompileShader(fragmentShaderObject);
	status = 0;
	log = NULL;
	infoLogLength = 0;

	glGetShaderiv(fragmentShaderObject, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE) {
		glGetShaderiv(fragmentShaderObject, GL_INFO_LOG_LENGTH, &infoLogLength);
		if (infoLogLength > 0) {
			log = (char*)malloc(sizeof(infoLogLength));
			if (log != NULL) {
				GLsizei written;
				glGetShaderInfoLog(fragmentShaderObject, infoLogLength, &written, log);
				fprintf(gFile, "Fragemnt Shader Compilation Log :  %s\n", log);
				free(log);
				uninitialize();
			}
		}
	}

	//shader program object
	shaderProgramObject = glCreateProgram();

	glAttachShader(shaderProgramObject, vertexShaderObject);
	glAttachShader(shaderProgramObject, fragmentShaderObject);

	glBindAttribLocation(shaderProgramObject,VBA_ATTRIBUTE_POSITION,"a_position");
	glBindAttribLocation(shaderProgramObject,VBA_ATTRIBUTE_TEXTURE0,"a_texcoord");


	glLinkProgram(shaderProgramObject);
	status = 0;
	infoLogLength = 0;
	log = NULL;

	glGetProgramiv(shaderProgramObject, GL_LINK_STATUS, &status);
	if (status == GL_FALSE) {
		glGetProgramiv(shaderProgramObject, GL_INFO_LOG_LENGTH, &infoLogLength);
		if (infoLogLength > 0) {
			log = (char*)malloc(sizeof(infoLogLength));
			if (log != NULL) {
				GLsizei written;
				glGetProgramInfoLog(shaderProgramObject, infoLogLength, &written, log);

				fprintf(gFile, "Shader Program Link  Log : %s\n", log);
				free(log);
				uninitialize();
			}
		}
	}

	//post-linking
	mvpMatrixUniform = glGetUniformLocation(shaderProgramObject, "u_mvpMatrix");
	textureSamplarUniform = glGetUniformLocation(shaderProgramObject, "u_texturesamplaer");
	keypressUniform = glGetUniformLocation(shaderProgramObject,"u_keyPress");
	//declaration of vertex data array

	const GLfloat position[] =
	{
		1.0f,1.0f,0.0f,
		-1.0f,1.0f,0.0f,
		-1.0f,-1.0f,0.0f,
		1.0f,-1.0f,0.0f};

	

	//VAO and VBO related code
	


	glGenVertexArrays(1,&VAO);
	glBindVertexArray(VAO);

	//VBO for Position
	glGenBuffers(1,&VBO_Position);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_Position);

	glBufferData(GL_ARRAY_BUFFER,sizeof(position), position,GL_STATIC_DRAW );

	glVertexAttribPointer(VBA_ATTRIBUTE_POSITION,3,GL_FLOAT,GL_FALSE,0,NULL);
	//2n d:paramern :no of row

	glEnableVertexAttribArray(VBA_ATTRIBUTE_POSITION);
	
	glBindBuffer(GL_ARRAY_BUFFER,0);

	//VBO for Color
	glGenBuffers(1,&VBO_TexCoord);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_TexCoord);
	glBufferData(GL_ARRAY_BUFFER,4*2*sizeof(GLfloat), NULL,GL_DYNAMIC_DRAW);
	glVertexAttribPointer(VBA_ATTRIBUTE_TEXTURE0,2,GL_FLOAT,GL_FALSE,0,NULL);
	glEnableVertexAttribArray(VBA_ATTRIBUTE_TEXTURE0);
	glBindBuffer(GL_ARRAY_BUFFER,0);

	glBindVertexArray(0);




	//deph and color related code
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);  //here only we say that clear with mention color but coloring is not done here
	
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	//this two line not use in pp as its depricated
	//glShadeModel(GL_SMOOTH);
	//glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);


	if (LoadGlTexure(&texture_smiley, MAKEINTRESOURCEA(ID_BITMAP_SMILEY)) == FALSE) {
		fprintf(gFile,"failed\n");
		return -5;

	}
	
	glEnable(GL_TEXTURE_2D);

	prespectiveProjectionMatrix = mat4::identity();

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

	prespectiveProjectionMatrix = vmath::perspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,100.0f);


}
void display(void) {

	//code
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//use the shader program object
	glUseProgram(shaderProgramObject);

	//Transformation
	//Pyramid
	mat4 translationMatrix = mat4::identity();

	mat4 modelViewMatrix = mat4::identity();
	mat4 modelViewProjectMatrix = mat4::identity();

	
	
	translationMatrix = vmath::translate(0.0f,0.0f,-3.0f); //glTranslatef is repleace by this line

	
	modelViewMatrix = translationMatrix;
	//here * is overloaded at vmath file 
	//order is important  here
	modelViewProjectMatrix = prespectiveProjectionMatrix *modelViewMatrix;

	glUniformMatrix4fv(mvpMatrixUniform,1,GL_FALSE,modelViewProjectMatrix);


	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D,texture_smiley);
	glUniform1i(textureSamplarUniform,0);


	glBindVertexArray(VAO);

	GLfloat texcoord[8];
	if (keyPress == 1) {
		texcoord[0] = 0.5f;
		texcoord[1] = 0.5f;

		texcoord[2] = 0.0f;
		texcoord[3] = 0.5f;

		texcoord[4] = 0.0f;
		texcoord[5] = 0.0f;

		texcoord[6] = 0.5f;
		texcoord[7] = 0.0f;

		glUniform1i(keypressUniform, 1);

	}
	else if (keyPress == 2) {

		texcoord[0] = 1.0f;
		texcoord[1] = 1.0f;

		texcoord[2] = 0.0f;
		texcoord[3] = 1.0f;

		texcoord[4] = 0.0f;
		texcoord[5] = 0.0f;

		texcoord[6] = 1.0f;
		texcoord[7] = 0.0f;
		glUniform1i(keypressUniform, 1);
	}
	else if (keyPress == 3) {
		texcoord[0] = 2.0f;
		texcoord[1] = 2.0f;

		texcoord[2] = 0.0f;
		texcoord[3] = 2.0f;

		texcoord[4] = 0.0f;
		texcoord[5] = 0.0f;

		texcoord[6] = 2.0f;
		texcoord[7] = 0.0f;
		glUniform1i(keypressUniform, 1);

	}
	else if (keyPress == 4) {

		texcoord[0] = 0.5f;
		texcoord[1] = 0.5f;

		texcoord[2] = 0.5f;
		texcoord[3] = 0.5f;

		texcoord[4] = 0.5f;
		texcoord[5] = 0.5f;

		texcoord[6] = 0.5f;
		texcoord[7] = 0.5f;

		glUniform1i(keypressUniform, 1);

	}
	else {

		glUniform1i(keypressUniform, 0);
	}


	glBindBuffer(GL_ARRAY_BUFFER, VBO_TexCoord);
	glBufferData(GL_ARRAY_BUFFER, sizeof(texcoord), texcoord, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);


	//here the drawing start
	glDrawArrays(GL_TRIANGLE_FAN,0,4);
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	//
	
	//unuse the shader progarm object

	glUseProgram(0);


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
	if (VBO_TexCoord) {
		glDeleteBuffers(1, &VBO_TexCoord);
		VBO_TexCoord = 0;
	}

	// VBO uninitaliation and deletation
	
	if (texture_smiley) {
		glDeleteTextures(1,&texture_smiley);
		texture_smiley = 0;
	}
	

	
	// VBO uninitaliation and deletation
	if (VBO_Position) {
		glDeleteBuffers(1,&VBO_Position);
		VBO_Position = 0;
	}


	//deletetion and unintailiation of VOA
	if (VAO) {
		glDeleteVertexArrays(1,&VAO);
		VAO = 0;
	}



	//shader uninialization

	if (shaderProgramObject) {
		glUseProgram(shaderProgramObject);
		glGetProgramiv(shaderProgramObject,GL_ATTACHED_SHADERS,&numAttachShader);
		GLuint* shaderObject=NULL;
		shaderObject = (GLuint*)malloc(numAttachShader*sizeof(GLuint));
		
		glGetAttachedShaders(shaderProgramObject,numAttachShader,&numAttachShader,shaderObject);

		for (GLsizei i = 0; i  < numAttachShader; i++) {
				
			glDetachShader(shaderProgramObject, shaderObject[i]);
			glDeleteShader(shaderObject[i]);
			shaderObject[i] = 0;
			
		}
		free(shaderObject);
		shaderObject = NULL;
		glUseProgram(0);
		glDeleteProgram(shaderProgramObject);
		shaderProgramObject = 0;
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

BOOL LoadGlTexure(GLuint* texture, TCHAR imageResourceId[]) {

	//variable declaration
	HBITMAP hBitMap = NULL;
	BITMAP bmp;
	BOOL bResult = FALSE;


	hBitMap = (HBITMAP)LoadImage(GetModuleHandle(NULL), imageResourceId, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION);

	//1 . GetModuleHandle() :get the currrent hInstance;
	//NULL ->get the current 
	//2 .id
	//3 :type of image to be loaded ex, bitmap,icon,cursor
	//4 ,5 height and width for the bitmap it bydefault zero it use only for icon and cursor


	if (hBitMap) {
		bResult = TRUE;
		GetObject(hBitMap, sizeof(bmp), &bmp);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		/*
			GL_UNPACK_ALIGAMENT : unpack kar pixel la and the allig kar i.e neat lav 4 chay group ne
			  4 beacuse of 4 color,R,G,B,A
		*/
		glGenTextures(1, texture);
		/*Genrate the texture name
			1parameter  -> kiti name genrate karayche aahe
			2 parameter ->A pointer to the first element of an array in
			which the generated texture names are stored.
			*/

		glBindTexture(GL_TEXTURE_2D, *texture);

		/*	1->param->The target to which the texture is bound.Must have the value GL_TEXTURE_1D or GL_TEXTURE_2D.

				2 ->paramThe name of a texture; the texture name cannot currently be in use.
			*/

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);


		//createthetexture
	//	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, bmp.bmWidth, bmp.bmHeight, GL_BGR_EXT, GL_UNSIGNED_BYTE, bmp.bmBits);

		glTexImage2D(GL_TEXTURE_2D,0,3,bmp.bmWidth,bmp.bmHeight,0, GL_BGR, GL_UNSIGNED_BYTE, bmp.bmBits);
		glGenerateMipmap(GL_TEXTURE_2D);
		/// 
		//1->texture cha type
		// 2->textue che color
		//3->texture chi width kiti theu
		//4->texture chi height kiti theu

		glBindTexture(GL_TEXTURE_2D, 0);

		DeleteObject(hBitMap);

	}
	return bResult;


}