// headers
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/XKBlib.h>
#include <X11/keysym.h>

#include <GL/gl.h>   // for OpenGL APIs
#include <GL/glu.h>  // for OpenGL utility functions
#include <GL/glx.h>  // for GLX APIs (bridging API)

// namespace
using namespace std;

// global variable declarations
Display* gpDisplay = NULL;
Window gWindow;
XVisualInfo* gpXVisualInfo = NULL;
Colormap gColormap;

GLXContext gGLXContext;

bool bFullscreen = false;
int giWindowWidth = 800;
int giWindowHeight = 600;

bool bLight = false;

GLfloat lightAmbient[] = { 0.1f, 0.1f, 0.1f, 1.0f };
GLfloat lightDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat lightPosition[] = { 0.0f, 0.0f, 0.0f, 1.0f };
GLfloat lightSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };

GLfloat materialAmbient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
GLfloat materialDiffuse[] = { 0.5f, 0.2f, 0.7f, 1.0f };
GLfloat materialSpecular[] = { 0.7f, 0.7f, 0.7f, 1.0f };
GLfloat materialShininess = 128.0f;

GLfloat light_model_ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
GLfloat light_model_local_viewer[] = { 0.0f };

GLUquadric* quadric[24];
GLfloat angleOfXRotation = 0.0f;
GLfloat angleOfYRotation = 0.0f;
GLfloat angleOfZRotation = 0.0f;

GLuint keyPressed = 0;

// entry-point function
int main(void)
{
	// function prototypes
	void CreateWindow(void);
	void ToggleFullscreen(void);

	void initialize(void);
	void display(void);
	void update(void);
	void resize(int, int);
	void uninitialize(void);

	// variable declarations
	int winWidth = giWindowWidth;
	int winHeight = giWindowHeight;

	bool bDone = false;

	// code
	CreateWindow();
	initialize();

	// game loop
	XEvent event;
	KeySym keysym;

	while (bDone == false)
	{
		while (XPending(gpDisplay))
		{
			XNextEvent(gpDisplay, &event);
			switch (event.type)
			{
			case MapNotify:
				break;

			case KeyPress:
				keysym = XkbKeycodeToKeysym(gpDisplay, event.xkey.keycode, 0, 0);
				switch (keysym)
				{
				case XK_Escape:
					bDone = true;
					break;

				case XK_F:
				case XK_f:
					if (bFullscreen == false)
					{
						ToggleFullscreen();
						bFullscreen = true;
					}
					else
					{
						ToggleFullscreen();
						bFullscreen = false;
					}
					break;

				case XK_L:
				case XK_l:
					bLight = !bLight;
					if (bLight)
					{
						glEnable(GL_LIGHTING);
					}
					else
					{
						glDisable(GL_LIGHTING);
					}
					break;

				case XK_X:
				case XK_x:
					keyPressed = 1;
					angleOfXRotation = 0.0f;
					break;

				case XK_Y:
				case XK_y:
					keyPressed = 2;
					angleOfYRotation = 0.0f;
					break;

				case XK_Z:
				case XK_z:
					keyPressed = 3;
					angleOfZRotation = 0.0f;
					break;

				default:
					break;
				}
				break;

			case ButtonPress:
				switch (event.xbutton.button)
				{
				case 1:
					break;

				case 2:
					break;

				case 3:
					break;

				default:
					break;
				}
				break;

			case MotionNotify:
				break;

			case ConfigureNotify:
				winWidth = event.xconfigure.width;
				winHeight = event.xconfigure.height;
				resize(winWidth, winHeight);
				break;

			case Expose:
				break;

			case DestroyNotify:
				break;

			case 33:
				bDone = true;
				break;

			default:
				break;
			}
		}

		update();
		display();
	}

	uninitialize();
	return(0);
}

void CreateWindow(void)
{
	// function prototypes
	void uninitialize(void);

	// variable declarations
	XSetWindowAttributes winAttribs;
	int defaultScreen;
	int styleMask;

	// ~ pixel format descriptor
	static int frameBufferAttributes[] = {
		GLX_DOUBLEBUFFER, True,
		GLX_RGBA,
		GLX_RED_SIZE, 8,
		GLX_GREEN_SIZE, 8,
		GLX_BLUE_SIZE, 8,
		GLX_ALPHA_SIZE, 8,
		GLX_DEPTH_SIZE, 24,
		None // here '0' is also possible
	};

	// code
	gpDisplay = XOpenDisplay(NULL);
	if (gpDisplay == NULL)
	{
		printf("ERROR: Unable to open XDisplay.\nExiting..\n");
		uninitialize();
		exit(1);
	}

	defaultScreen = XDefaultScreen(gpDisplay);

	gpXVisualInfo = glXChooseVisual(gpDisplay, defaultScreen, frameBufferAttributes);
	if (gpXVisualInfo == NULL)
	{
		printf("ERROR: Unable to get a Visual.\nExiting..\n");
		uninitialize();
		exit(1);
	}

	winAttribs.border_pixel = 0;
	winAttribs.background_pixmap = 0;
	winAttribs.colormap = XCreateColormap(gpDisplay,
		RootWindow(gpDisplay, gpXVisualInfo->screen),
		gpXVisualInfo->visual,
		AllocNone);
	gColormap = winAttribs.colormap;
	winAttribs.background_pixel = BlackPixel(gpDisplay, gpXVisualInfo->depth);

	winAttribs.event_mask = ExposureMask | VisibilityChangeMask | ButtonPressMask | KeyPressMask | PointerMotionMask | StructureNotifyMask;
	styleMask = CWBorderPixel | CWBackPixel | CWEventMask | CWColormap;

	gWindow = XCreateWindow(gpDisplay,
		RootWindow(gpDisplay, gpXVisualInfo->screen),
		0,
		0,
		giWindowWidth,
		giWindowHeight,
		0,
		gpXVisualInfo->depth,
		InputOutput,
		gpXVisualInfo->visual,
		styleMask,
		&winAttribs);
	if (!gWindow)
	{
		printf("ERROR: Failed to create Main Window.\nExiting..\n");
		uninitialize();
		exit(1);
	}

	XStoreName(gpDisplay, gWindow, "xWindows");

	Atom windowManagerDelete = XInternAtom(gpDisplay, "WM_DELETE_WINDOW", True);
	XSetWMProtocols(gpDisplay, gWindow, &windowManagerDelete, 1);

	XMapWindow(gpDisplay, gWindow);
}

void ToggleFullscreen(void)
{
	// variable declarations
	Atom wm_state;
	Atom fullscreen;
	XEvent xev = { 0 };

	// code
	wm_state = XInternAtom(gpDisplay, "_NET_WM_STATE", False);
	memset(&xev, 0, sizeof(XEvent));

	xev.type = ClientMessage;
	xev.xclient.window = gWindow;
	xev.xclient.message_type = wm_state;
	xev.xclient.format = 32;
	xev.xclient.data.l[0] = bFullscreen ? 0 : 1;

	fullscreen = XInternAtom(gpDisplay, "_NET_WM_STATE_FULLSCREEN", False);
	xev.xclient.data.l[1] = fullscreen;

	XSendEvent(gpDisplay,
		RootWindow(gpDisplay, gpXVisualInfo->screen),
		False,
		StructureNotifyMask,
		&xev);
}

void initialize(void)
{
	// function declarations
	void resize(int, int);
	void uninitialize(void);

	// code
	gGLXContext = glXCreateContext(gpDisplay, gpXVisualInfo, NULL, GL_TRUE);
	glXMakeCurrent(gpDisplay, gWindow, gGLXContext);

	// opengl
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

	// depth
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glShadeModel(GL_SMOOTH);

	// light
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
	glEnable(GL_LIGHT0);

	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);
	glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

	for (int i = 0; i < 24; i++)
	{
		quadric[i] = gluNewQuadric();
	}

	// warmup resize
	resize(giWindowWidth, giWindowHeight);
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

GLfloat materialAmbient[4][6][4] = {
	{   // Column 1
		{0.0215f, 0.1745f, 0.0215f, 1.0f },
		{0.135f, 0.2225f, 0.1575f, 1.0f },
		{0.05375f, 0.05f, 0.06625f, 1.0f },
		{0.25f, 0.20725f, 0.20725f, 1.0f },
		{0.1745f, 0.01175f, 0.01175f, 1.0f },
		{0.1f, 0.18725f, 0.1745f, 1.0f }
	},
	{   // Column 2
		{0.329412f, 0.223529f, 0.027451f, 1.0f },
		{0.2125f, 0.1275f, 0.054f, 1.0f },
		{0.25f, 0.25f, 0.25f, 1.0f },
		{0.19125f, 0.0735f, 0.0225f, 1.0f },
		{0.24725f, 0.1995f, 0.0745f, 1.0f },
		{0.19225f, 0.19225f, 0.19225f, 1.0f }
	},
	{   // Column 3
		{0.0f, 0.0f, 0.0f, 1.0f },
		{0.0f, 0.1f, 0.06f, 1.0f },
		{0.0f, 0.0f, 0.0f, 1.0f },
		{0.0f, 0.0f, 0.0f, 1.0f },
		{0.0f, 0.0f, 0.0f, 1.0f },
		{0.0f, 0.0f, 0.0f, 1.0f }
	},
	{   // Column 4
		{0.02f, 0.02f, 0.02f, 1.0f },
		{0.0f, 0.05f, 0.05f, 1.0f },
		{0.0f, 0.05f, 0.0f, 1.0f },
		{0.05f, 0.0f, 0.0f, 1.0f },
		{0.05f, 0.05f, 0.05f, 1.0f },
		{0.05f, 0.05f, 0.0f, 1.0f }
	}
};

GLfloat materialDiffuse[4][6][4] = {
	{   // Column 1
		{0.07568f, 0.61424f, 0.07568f, 1.0f},
		{0.54f, 0.89f, 0.63f, 1.0f},
		{0.18275f, 0.17f, 0.22525f, 1.0f},
		{1.0f, 0.829f, 0.829f, 1.0f},
		{0.61424f, 0.04136f, 0.04136f, 1.0f},
		{0.396f, 0.74151f, 0.69102f, 1.0f},
	},
	{   // Column 2
		{0.780392f, 0.568627f, 0.113725f, 1.0f},
		{0.714f, 0.4284f, 0.18144f, 1.0f},
		{0.4f, 0.4f, 0.4f, 1.0f},
		{0.7038f, 0.27048f, 0.0828f, 1.0f},
		{0.75164f, 0.60648f, 0.22648f, 1.0f},
		{0.50754f, 0.50754f, 0.50754f, 1.0f},
	},
	{   // Column 3
		{0.01f, 0.01f, 0.01f, 1.0f},
		{0.0f, 0.50980392f, 0.50980392f, 1.0f},
		{0.1f, 0.35f, 0.1f, 1.0f},
		{0.5f, 0.0f, 0.0f, 1.0f},
		{0.55f, 0.55f, 0.55f, 1.0f},
		{0.5f, 0.5f, 0.0f, 1.0f},
	},
	{   // Column 4
		{0.01f, 0.01f, 0.01f, 1.0f},
		{0.4f, 0.5f, 0.5f, 1.0f},
		{0.4f, 0.5f, 0.4f, 1.0f},
		{0.5f, 0.4f, 0.4f, 1.0f},
		{0.5f, 0.5f, 0.5f, 1.0f},
		{0.5f, 0.5f, 0.4f, 1.0f},
	},
};

GLfloat materialSpecular[4][6][4] = {
	{   // Column 1
		{0.633f, 0.727811f, 0.633f, 1.0f},
		{0.316228f, 0.316228f, 0.316228f, 1.0f},
		{0.332741f, 0.328634f, 0.346435f, 1.0f},
		{0.296648f, 0.296648f, 0.296648f, 1.0f},
		{0.727811f, 0.626959f, 0.626959f, 1.0f},
		{0.297254f, 0.30829f, 0.306678f, 1.0f},
	},
	{   // Column 2
		{0.992157f, 0.941176f, 0.807843f, 1.0f},
		{0.393548f, 0.271906f, 0.166721f, 1.0f},
		{0.774597f, 0.774597f, 0.774597f, 1.0f},
		{0.256777f, 0.137622f, 0.086014f, 1.0f},
		{0.628281f, 0.555802f, 0.366065f, 1.0f},
		{0.508273f, 0.508273f, 0.508273f, 1.0f},
	},
	{   // Column 3
		{0.50f, 0.50f, 0.50f, 1.0f},
		{0.50196078f, 0.50196078f, 0.50196078f, 1.0f},
		{0.45f, 0.55f, 0.45f, 1.0f},
		{0.7f, 0.6f, 0.6f, 1.0f},
		{0.70f, 0.70f, 0.70f, 1.0f},
		{0.60f, 0.60f, 0.50f, 1.0f},
	},
	{   // Column 4
		{0.4f, 0.4f, 0.4f, 1.0f},
		{0.04f, 0.7f, 0.7f, 1.0f},
		{0.04f, 0.7f, 0.04f, 1.0f},
		{0.7f, 0.04f, 0.04f, 1.0f},
		{0.7f, 0.7f, 0.7f, 1.0f},
		{0.7f, 0.7f, 0.04f, 1.0f},
	}
};

GLfloat matrialShininess[4][6][4] = {
	{   // Column 1
		{0.6f * 128.0f, 0.6f * 128.0f, 0.6f * 128.0f, 0.6f * 128.0f},
		{0.1f * 128.0f, 0.1f * 128.0f, 0.1f * 128.0f, 0.1f * 128.0f},
		{0.3f * 128.0f, 0.3f * 128.0f, 0.3f * 128.0f, 0.3f * 128.0f},
		{0.088f * 128.0f, 0.088f * 128.0f, 0.088f * 128.0f, 0.088f * 128.0f},
		{0.6f * 128.0f, 0.6f * 128.0f, 0.6f * 128.0f, 0.6f * 128.0f},
		{0.1f * 128.0f, 0.1f * 128.0f, 0.1f * 128.0f, 0.1f * 128.0f},
	},
	{   // Column 2
		{0.21794872f * 128.0f, 0.21794872f * 128.0f, 0.21794872f * 128.0f, 0.21794872f * 128.0f},
		{0.2f * 128.0f, 0.2f * 128.0f, 0.2f * 128.0f, 0.2f * 128.0f},
		{0.6f * 128.0f, 0.6f * 128.0f, 0.6f * 128.0f, 0.6f * 128.0f},
		{0.1f * 128.0f, 0.1f * 128.0f, 0.1f * 128.0f, 0.1f * 128.0f},
		{0.4f * 128.0f, 0.4f * 128.0f, 0.4f * 128.0f, 0.4f * 128.0f},
		{0.4f * 128.0f, 0.4f * 128.0f, 0.4f * 128.0f, 0.4f * 128.0f},
	},
	{   // Column 3
		{0.25f * 128.0f, 0.25f * 128.0f, 0.25f * 128.0f, 0.25f * 128.0f},
		{0.25f * 128.0f, 0.25f * 128.0f, 0.25f * 128.0f, 0.25f * 128.0f},
		{0.25f * 128.0f, 0.25f * 128.0f, 0.25f * 128.0f, 0.25f * 128.0f},
		{0.25f * 128.0f, 0.25f * 128.0f, 0.25f * 128.0f, 0.25f * 128.0f},
		{0.25f * 128.0f, 0.25f * 128.0f, 0.25f * 128.0f, 0.25f * 128.0f},
		{0.25f * 128.0f, 0.25f * 128.0f, 0.25f * 128.0f, 0.25f * 128.0f},
	},
	{   // Column 4
		{0.078125f * 128.0f, 0.078125f * 128.0f, 0.078125f * 128.0f, 0.078125f * 128.0f},
		{0.078125f * 128.0f, 0.078125f * 128.0f, 0.078125f * 128.0f, 0.078125f * 128.0f},
		{0.078125f * 128.0f, 0.078125f * 128.0f, 0.078125f * 128.0f, 0.078125f * 128.0f},
		{0.078125f * 128.0f, 0.078125f * 128.0f, 0.078125f * 128.0f, 0.078125f * 128.0f},
		{0.078125f * 128.0f, 0.078125f * 128.0f, 0.078125f * 128.0f, 0.078125f * 128.0f},
		{0.078125f * 128.0f, 0.078125f * 128.0f, 0.078125f * 128.0f, 0.078125f * 128.0f},
	}
};

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
	guadric = gluNewQuadric();

	// 3rd parameter is for slices (like longitudes)
	// 4th parameter is for stacks (like latitudes)
	// Higher the value of 3rd and 4th parameters, i.e. more the sub-divisions,
	// more circular the sphere will look.
	gluSphere(guadric, 0.75, 30, 30);

	// Pop back to initial state.
	glPopMatrix();
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);

	// Column 1
	glLoadIdentity();
	glTranslatef(-6.0f, -5.0f, -15.0f);
	rotateLight();
	loadMaterial(0, 0);
	drawSphere();

	glLoadIdentity();
	glTranslatef(-6.0f, -3.0f, -15.0f);
	rotateLight();
	loadMaterial(0, 1);
	drawSphere();

	glLoadIdentity();
	glTranslatef(-6.0f, -1.0f, -15.0f);
	rotateLight();
	loadMaterial(0, 2);
	drawSphere();

	glLoadIdentity();
	glTranslatef(-6.0f, 1.0f, -15.0f);
	rotateLight();
	loadMaterial(0, 3);
	drawSphere();

	glLoadIdentity();
	glTranslatef(-6.0f, 3.0f, -15.0f);
	rotateLight();
	loadMaterial(0, 4);
	drawSphere();

	glLoadIdentity();
	glTranslatef(-6.0f, 5.0f, -15.0f);
	rotateLight();
	loadMaterial(0, 5);
	drawSphere();

	// Column 2
	glLoadIdentity();
	glTranslatef(-2.0f, -5.0f, -15.0f);
	rotateLight();
	loadMaterial(1, 0);
	drawSphere();

	glLoadIdentity();
	glTranslatef(-2.0f, -3.0f, -15.0f);
	rotateLight();
	loadMaterial(1, 1);
	drawSphere();

	glLoadIdentity();
	glTranslatef(-2.0f, -1.0f, -15.0f);
	rotateLight();
	loadMaterial(1, 2);
	drawSphere();

	glLoadIdentity();
	glTranslatef(-2.0f, 1.0f, -15.0f);
	rotateLight();
	loadMaterial(1, 3);
	drawSphere();

	glLoadIdentity();
	glTranslatef(-2.0f, 3.0f, -15.0f);
	rotateLight();
	loadMaterial(1, 4);
	drawSphere();

	glLoadIdentity();
	glTranslatef(-2.0f, 5.0f, -15.0f);
	rotateLight();
	loadMaterial(1, 5);
	drawSphere();

	// Column 3
	glLoadIdentity();
	glTranslatef(2.0f, -5.0f, -15.0f);
	rotateLight();
	loadMaterial(2, 0);
	drawSphere();

	glLoadIdentity();
	glTranslatef(2.0f, -3.0f, -15.0f);
	rotateLight();
	loadMaterial(2, 1);
	drawSphere();

	glLoadIdentity();
	glTranslatef(2.0f, -1.0f, -15.0f);
	rotateLight();
	loadMaterial(2, 2);
	drawSphere();

	glLoadIdentity();
	glTranslatef(2.0f, 1.0f, -15.0f);
	rotateLight();
	loadMaterial(2, 3);
	drawSphere();

	glLoadIdentity();
	glTranslatef(2.0f, 3.0f, -15.0f);
	rotateLight();
	loadMaterial(2, 4);
	drawSphere();

	glLoadIdentity();
	glTranslatef(2.0f, 5.0f, -15.0f);
	rotateLight();
	loadMaterial(2, 5);
	drawSphere();

	// Column 4
	glLoadIdentity();
	glTranslatef(6.0f, -5.0f, -15.0f);
	rotateLight();
	loadMaterial(3, 0);
	drawSphere();

	glLoadIdentity();
	glTranslatef(6.0f, -3.0f, -15.0f);
	rotateLight();
	loadMaterial(3, 1);
	drawSphere();

	glLoadIdentity();
	glTranslatef(6.0f, -1.0f, -15.0f);
	rotateLight();
	loadMaterial(3, 2);
	drawSphere();

	glLoadIdentity();
	glTranslatef(6.0f, 1.0f, -15.0f);
	rotateLight();
	loadMaterial(3, 3);
	drawSphere();

	glLoadIdentity();
	glTranslatef(6.0f, 3.0f, -15.0f);
	rotateLight();
	loadMaterial(3, 4);
	drawSphere();

	glLoadIdentity();
	glTranslatef(6.0f, 5.0f, -15.0f);
	rotateLight();
	loadMaterial(3, 5);
	drawSphere();

	SwapBuffers(hdc);
}

void update(void)
{
	// code
	if (angleOfXRotation < 360)
	{
		angleOfXRotation += 0.75;
	}
	else
	{
		angleOfXRotation = 0.0;
	}

	if (angleOfYRotation < 360)
	{
		angleOfYRotation += 0.75;
	}
	else
	{
		angleOfYRotation = 0.0;
	}

	if (angleOfZRotation < 360)
	{
		angleOfZRotation += 0.75;
	}
	else
	{
		angleOfZRotation = 0.0;
	}
}

void uninitialize(void)
{
	if (quadric)
	{
		for (int i = 0; i < 24; i++)
		{
			gluDeleteQuadric(quadric[i]);
		}
	}

	GLXContext currentGLXContext = glXGetCurrentContext();
	if (currentGLXContext != NULL && currentGLXContext == gGLXContext)
	{
		glXMakeCurrent(gpDisplay, 0, 0);
	}

	if (gGLXContext)
	{
		glXDestroyContext(gpDisplay, gGLXContext);
	}

	if (gWindow)
	{
		XDestroyWindow(gpDisplay, gWindow);
	}

	if (gColormap)
	{
		XFreeColormap(gpDisplay, gColormap);
	}

	if (gpXVisualInfo)
	{
		free(gpXVisualInfo);
		gpXVisualInfo = NULL;
	}

	if (gpDisplay)
	{
		XCloseDisplay(gpDisplay);
		gpDisplay = NULL;
	}
}

