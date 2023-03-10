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


//Global variables
FILE* gpFile = NULL;
Display *display = NULL;
XVisualInfo *visualInfo = NULL;
Colormap colormap;
Window window;
Bool fullscreen = False;
Bool bActiveWindow = False;

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
}

void resize(int width, int height)
{
	//code
	if(height == 0)
		height =1;
	glViewport(0,0,(GLsizei)width,(GLsizei)height);
	
}

void draw(void)
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


