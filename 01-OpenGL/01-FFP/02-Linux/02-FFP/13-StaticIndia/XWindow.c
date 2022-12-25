//Standard Headers
#include<stdio.h>//for standard I/O
#include<stdlib.h>//for exit() function
#include<memory.h>//for memset()

//X11 Headers (Xorg 11th version standard
#include<X11/Xlib.h> //Xclient API header file
#include<X11/Xutil.h>//for XVisualInfo
#include<X11/XKBlib.h>// for Keyboard

//OpenGL Header Files
#include<GL/gl.h> // for OpenGL functionality
#include<GL/glx.h> // for Bridging API's 

//Macros
#define WIN_WIDTH 800
#define WIN_HEIGHT 600


void update(void);
void resize(int width,int height);

//Global variables
FILE* gpFile = NULL;
Display *display = NULL;
XVisualInfo *visualInfo = NULL;
Colormap colormap;
Window window;
Bool fullscreen = False;
Bool bActiveWindow = False;

const GLfloat orange[] = { 1.0f, 153.0f / 256.0f, 51.0f / 256.0f };
const GLfloat  green[] = { 19.0f / 256.0f, 136.0f / 256.0f, 8.0f / 256.0f };
const float white[] = {1, 1, 1};


//OpenGL related Global variables
GLXContext glxContext;


//Entry Point Function
int main(void)
{
	//Function declarations
	void initialize(void);
	void resize(int,int);
	void draw(void);
	void uninitialize(void);
	void toggleFullscreen(void);

	//local variables
	int defaultScreen;
	int defaultDepth;
	XSetWindowAttributes windowAttributes;
	int styleMask;
	Atom wm_delete_window_atom;
	XEvent event;
	KeySym keysym;
	int screenWidth;
	int screenHeight;
	char keys[26];
	static int win_width;
	static int win_height;

	
	int frameBufferAttributes[] = 
				     {
				       GLX_DOUBLEBUFFER,True,
				       GLX_RGBA,
			  	       GLX_RED_SIZE,8,
				       GLX_GREEN_SIZE,8,
				       GLX_BLUE_SIZE,8,
				       GLX_ALPHA_SIZE,8,
				      // GLX_DEPTH_SIZE, 24,

				       None };
        Bool bDone = False;

	//code
	gpFile = fopen("Log.txt", "w");
  	if( gpFile == NULL)
	{
		printf("Creation Of Log File Failed. Exitting...\n");
		exit(0);
	}
	else
	{
		fprintf(gpFile, "\t\t\t\t\t\t !!! SHREE GAJANAN PRASANNA !!! \n\nLog file sucessfully open.\n");
	}
	display = XOpenDisplay(NULL);
	if(display == NULL)
	{
		printf("ERROR:XOpenDisplay() Failed\n");
		uninitialize();
		exit(1);
	}
	
	defaultScreen = XDefaultScreen(display);
	defaultDepth = XDefaultDepth(display,defaultScreen);

	visualInfo = glXChooseVisual(display,defaultScreen,frameBufferAttributes);
	if(visualInfo == NULL )
	{
		printf("ERROR:glxChooseVisual() Failed\n");
		uninitialize();
		exit(1);
	}	

	memset(&windowAttributes,0,sizeof(XSetWindowAttributes));
        
	windowAttributes.border_pixel = 0;
	windowAttributes.background_pixel=XBlackPixel(display,defaultScreen);
	windowAttributes.background_pixmap = 0;
    windowAttributes.colormap = XCreateColormap(display,
	       							            XRootWindow(display,visualInfo->screen),
											    visualInfo->visual,
							    				AllocNone);
	windowAttributes.event_mask = ExposureMask | KeyPressMask | StructureNotifyMask | FocusChangeMask;
	
	colormap = windowAttributes.colormap;
	
	styleMask = CWBorderPixel | CWBackPixel | CWColormap | CWEventMask;
	
	window = XCreateWindow(display,
					       RootWindow(display,visualInfo->screen),
		       		       0,
			       		   0,
			     		   WIN_WIDTH,
			      		   WIN_HEIGHT,
			       		   0,
			       		   visualInfo->depth,
			       		   InputOutput,
			       		   visualInfo->visual,
			       		   styleMask,
			       		   &windowAttributes);

	if(!window)
	{
		printf("ERROR:XCreateWindow() Failed\n");
		uninitialize();
		exit(1);
	}

	XStoreName(display,window,"SSP : OGL");

	wm_delete_window_atom = XInternAtom(display,"WM_DELETE_WINDOW",True);
	XSetWMProtocols(display,window,&wm_delete_window_atom,1);

	XMapWindow(display,window);//like ShowWindow() in Win32 API
	//Centering of Window
	screenWidth = XWidthOfScreen(XScreenOfDisplay(display,defaultScreen));
	screenHeight = XHeightOfScreen(XScreenOfDisplay(display,defaultScreen));
	XMoveWindow(display,window,(screenWidth/2 - WIN_WIDTH/2),(screenHeight/2 - WIN_HEIGHT/2));

	initialize();
	//MessageLoop
	while(bDone == False)
	{
		while(XPending(display)) // PeekMessage() in Windows
		{
	          XNextEvent(display,&event);
	          switch(event.type)
	          {
				case MapNotify: // WM_CREATE
								break;
				case FocusIn:
								bActiveWindow = True;
								break;
				case FocusOut: 
								bActiveWindow = False;
								break;
				case KeyPress:
								keysym = XkbKeycodeToKeysym(display,event.xkey.keycode,0,0);
					     	    switch(keysym)
				      		    {
					     			 case XK_Escape:
						  		    				 bDone = True;
						      						 break;
			                	}

					 			XLookupString(&event.xkey,keys,sizeof(keys),NULL,NULL);
				       		    switch(keys[0])
					 			{
						 			case 'F':
					  	 			case 'f':
												if(fullscreen == False)
												{
													toggleFullscreen();
													fullscreen = True;
												}
												
												else
												{
													toggleFullscreen();
													fullscreen = False;
												}
												break;
								}
								break;
				case ConfigureNotify:
										win_width = event.xconfigure.width;
										win_height = event.xconfigure.height;
										resize(win_width,win_height);				
										break;

				case 33: //analogous to wm_delete_window_atom
						bDone = True;
						break; 
				default:
					break;     	       
	   		  }
		 
		}
		if(bActiveWindow ==True)
		{
			//update();
			draw();
		}
	}	
	uninitialize();
	return(0);
}

void toggleFullscreen(void)
{
	//local variable declarations
	Atom wm_current_state_atom;
        Atom wm_fullscreen_state_atom;
	XEvent event;
	
	//code
	wm_current_state_atom = XInternAtom(display,"_NET_WM_STATE",False);
	wm_fullscreen_state_atom = XInternAtom(display,"_NET_WM_STATE_FULLSCREEN",False);

	memset(&event,0,sizeof(XEvent));
	
	event.type = ClientMessage;
	event.xclient.window = window;
	event.xclient.message_type = wm_current_state_atom;
	event.xclient.format = 32;
	event.xclient.data.l[0] = fullscreen ? 0 : 1;
	event.xclient.data.l[1] = wm_fullscreen_state_atom;

	XSendEvent(display,
		   RootWindow(display,visualInfo->screen),
	  	   False,
		   SubstructureNotifyMask,
		   &event);
		
}

void initialize(void)
{
	//code
	glxContext = glXCreateContext(display,visualInfo,NULL,True);
	glXMakeCurrent(display,window,glxContext);


	//Here starts OpenGL functionality
	glClearColor(0.0f,0.0f,0.0f,1.0f);

	// Depth Related Functions
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glShadeModel(GL_SMOOTH);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	
	resize(WIN_WIDTH,WIN_HEIGHT);
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


void draw(void)
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

	glXSwapBuffers(display,window);
}

void uninitialize(void)
{
	//code
	GLXContext currentContext;
	currentContext = glXGetCurrentContext();

	if(currentContext && currentContext == glxContext)
	{
		glXMakeCurrent(display,0,0);
	}
	if(glxContext)
	{
		glXDestroyContext(display,glxContext);
		glxContext =NULL;
	}
	if(visualInfo)
	{
		free(visualInfo);
		visualInfo = NULL;
	}
	if(fullscreen)
	{
		toggleFullscreen();
	}
	if(window)
	{
		XDestroyWindow(display,window);
	}	
	if(colormap)
	{
		XFreeColormap(display,colormap);
	}
	if(display)
	{
		XCloseDisplay(display);
		display =NULL;
	}
	if (gpFile)
	{
		fprintf(gpFile, "Log file sucessfully close\n");
		fclose(gpFile);
		gpFile = NULL;
	}
}


