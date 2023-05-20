//standard headers
#include<stdio.h>//for standard IO
#include<stdlib.h>//for exit()
#include<memory.h>//for memset()
//#include<string.h>
#include"vmath.h"
using namespace vmath;

//Macros
#define WIN_WIDTH 800
#define WIN_HEIGHT 600

//x11 headers(x server's 11th version )
#include<X11/Xlib.h>//XClient APIs ()
#include<X11/Xutil.h>//XVisualInfo+
#include<X11/XKBlib.h>

//OpenGL header files
#include<GL/glew.h> //for opengl extensions
#include<GL/gl.h>//for opengl functionality
#include<GL/glx.h>//for brigding apis

//global variables
Display *gpDisplay=NULL;
XVisualInfo *visualInfo=NULL;

Colormap colormap;
int styleMask;
Window window;
Atom wm_init_window_atom;
Bool gbFullscreen=False;
FILE* gpFile = NULL;
//opengl related variables
typedef GLXContext (*glXCreateContextAttribsARBProc)(Display*, GLXFBConfig, GLXContext, Bool, const int*);
glXCreateContextAttribsARBProc glXCreateContextAttribsARB = NULL;
GLXFBConfig glxFBConfig;
// GLXFBConfig glxFBConfigs[];
GLXContext glxContex;
Bool bActiveWindow = False;
//programmable pipeline related global variables
GLuint shaderProgramObject;
enum
{
	AMC_ATRIBUTE_POSITION = 0,
	AMC_ATRIBUTE_COLOR,
	AMC_ATRIBUTE_NORMAL,
	AMC_ATRIBUTE_TEXTURE0

};

GLuint VAO;
GLuint VBO;
GLuint VBO_position;
GLuint VBO_element;
GLuint mvpMatrixUniform;

mat4 persepectiveProjectionMAtrix;

#define SUCCESS 1
#define VECTOR_EMPTY 2

typedef struct Vector_Int
{
	int* pInt;
	int size;
}VECTOR_INT;

typedef struct Vector_Float
{
	float* pFloat;
	int size;
}VECTOR_FLOAT;

#define BUFFER_SIZE 1024
char buffer[BUFFER_SIZE];

FILE* gp_MeshFile = NULL;
VECTOR_FLOAT* gpVertex, * gpTexture, * gpNormal;
VECTOR_INT* gpVertexIndices, * gpTextureIndices, * gpNormalIndices;
VECTOR_FLOAT* gpVertexSorted, * gpTextureSorted, * gpNormalSorted;
int nrPosCoords = 0, nrTexCoords = 0, nrNormalCoords = 0, nrFaces = 0;



//entry point function
int main(void)
{
	//local function declarations
	//void uninitialize(void);
	void ToggleFullscreen(void);
	int initialize(void);
	void resize(int,int);
	void display(void);
	void uninitialize(void);
	
	//local variables
	int defaultScreen;
	int defaultDepth;
	GLXFBConfig* glxFBConfigs = NULL;
	GLXFBConfig bestGLXFBConfig;
	XVisualInfo* tempXVisualInfo = NULL;
	int numFBConfigs;
	XSetWindowAttributes windowAttributes;
	XEvent event;
	KeySym keySym;
	int screenWidth;
	int screenHeight;
	char keys[26];
	int iRetval = 0;

	
	static int frameBufferAttributes[]=
	{
		GLX_X_RENDERABLE,True,//aadva render kar
		GLX_DRAWABLE_TYPE,GLX_WINDOW_BIT,//draw kashavar karaychay ->window
		GLX_RENDER_TYPE,GLX_RGBA_BIT,
		GLX_X_VISUAL_TYPE,GLX_TRUE_COLOR,
		GLX_RED_SIZE,8,
		GLX_GREEN_SIZE,8,
		GLX_BLUE_SIZE,8,
		GLX_ALPHA_SIZE,8,
		GLX_STENCIL_SIZE,8,
		GLX_DEPTH_SIZE,24,
		GLX_DOUBLEBUFFER,True,
		None//0
		
	};
	
	Bool bDone = False;
	
	static GC gc;//GraphicContext (= HDC in Windows)
	XGCValues gcValues;
	static XFontStruct *fontStruct=NULL;
	XColor fontColor;
	char str[]="Hello World !!!!";
	int strLength;
	int strWidth;
	int fontHeight;
	static int winWidth;
	static int winHeight;
		
	
	//code
	if((gpFile = fopen("Log.txt","w")) == NULL)
	{
		printf("Error:File I/O Error : fopen() failed\n");
		exit(0);
	}
	else
	{
		fprintf(gpFile,"Log file successfully created.\n");
	}
	
	//XSrever la  cha interface  available karun denyasathi request karto XOpenDisplay vaprun 
	//Step_01 : Open the display
	gpDisplay=XOpenDisplay(NULL);//NULL cha jagi cmdLine cha parameter jato jo network la lagto mainly (aplyala nahi lagate)
	
	if(gpDisplay == NULL)
	{
		fprintf(gpFile,"Error:XOpenDisplay() failed\n");
		uninitialize();
		exit(1);
	}	
	
	//step 02:Get default screen using display
	//default screen = hMonitor /primaryMonitor yala graphic card cha adaptor lavlay tya related default screen de 
	//yala graphic card cha adaptor lavlay tya mhanje graphic card milala mhanje access milala actually VRAM cha access milu shakto.
	//(//altimately backtrac karat VRAM paryant jata yeta hence VRAM madhla FrameBuffer//)-->RTR chi bhasha 
	defaultScreen=XDefaultScreen(gpDisplay);
	
	//step_03:Get default depth using display screeen.
	//VARAM access milala thus aplyala frame buffer milala tyatch depth buffer ahe hence aplyala default depth milali
	//he display mala dilelya default screen chi jo ki primary monitor ahe tyachi default depth de.
	defaultDepth=XDefaultDepth(gpDisplay,defaultScreen);
	//----------------------------------------------------------------
	//step_04: Get XVisualInfo using XMatchVisualInfo() and do error checking 
	//XServer la openGL mahiti nahia ajun bridging API picture madhe aanle nahiet 
	//Xserver aplya property la framebuffer na mhanta Visual mhanto 
	//frameBuffer mhanje graphic card cha related properties mhanje XServer cha bhashet Visual 
	//Visual is analogous to pixelFormatDescriptor
	
	//Other options are:
	//1.DirectColor
	//2.StaticColor
	//----------------------------------------------------------------
	
	glxFBConfigs = glXChooseFBConfig(
	gpDisplay,
	defaultScreen,
	frameBufferAttributes,
	&numFBConfigs
	);

	printf("Breakpoint_0\n");

	if (glxFBConfigs == NULL)
	{
		fprintf(gpFile, "glXChooseFBConfig() failed.\n");
		uninitialize();
		exit(1);
	}
	fprintf(gpFile, "found number of buffer config is %d.\n",numFBConfigs);


	//find best frame buffer config
	int bestFrameBufferConfig = -1;
	int worstFrameBufferConfig = -1;
	int bestNumberOfSamples = -1;
	int worstNumberOfSamples = 999;

	for (int i = 0; i < numFBConfigs; i++)
	{
		tempXVisualInfo = glXGetVisualFromFBConfig(
			gpDisplay,
			glxFBConfigs[i]
		);
 
		if (tempXVisualInfo != NULL)
		{
			int samples, sampleBuffers;
			
			glXGetFBConfigAttrib(
			gpDisplay,
			glxFBConfigs[i],
			GLX_SAMPLE_BUFFERS,
			&sampleBuffers
			);

			glXGetFBConfigAttrib(
				gpDisplay,
				glxFBConfigs[i],
				GLX_SAMPLES,
				&samples
			);

			fprintf(gpFile, "visualInfo = 0x%lu found sampleBuffers = %d and samples = %d\n",tempXVisualInfo->visualid,sampleBuffers,samples);

			if (bestFrameBufferConfig < 0 || sampleBuffers && samples > bestNumberOfSamples)
			{
				bestFrameBufferConfig = i;
				bestNumberOfSamples = samples;

			}

			if (worstFrameBufferConfig < 0 || !sampleBuffers || samples < worstNumberOfSamples)
			{
				worstFrameBufferConfig = i;
				worstNumberOfSamples = samples;

			}
		}
		XFree(tempXVisualInfo);
		tempXVisualInfo = NULL;
	}

	printf("Breakpoint_1\n");

	bestGLXFBConfig = glxFBConfigs[bestFrameBufferConfig];
	printf("Breakpoint_2\n");
	printf("bestFrameBufferConfig: %d\n", bestFrameBufferConfig);
	glxFBConfig = bestGLXFBConfig;
	printf("Breakpoint_3\n");
	XFree(glxFBConfigs);
	printf("Breakpoint_4\n");
	glxFBConfigs = NULL;
	printf("Breakpoint_5\n");

	visualInfo = glXGetVisualFromFBConfig(
		gpDisplay,
		bestGLXFBConfig
	);
	printf("Breakpoint_6\n");
	fprintf(gpFile, "visual id of best visual info is 0x%lu",visualInfo->visualid);
	//step 05:Fill/initialize struct XSetWindowAtributes (=WNDCLASSEX in Windowa) and along with that also state 	colormap and event mask.
	
	//we will fill only few properties hence need to zero out the entire structure first.
	memset(&windowAttributes,0,sizeof(XSetWindowAttributes));
	windowAttributes.border_pixel=0;
	windowAttributes.background_pixel=XBlackPixel(gpDisplay,
	defaultScreen);//background pixel=hbrBackground in Windows
	windowAttributes.background_pixmap=0;
	windowAttributes.colormap=XCreateColormap(
	gpDisplay,
	RootWindow(gpDisplay,visualInfo->screen),//Default screen pan deu shaklo asto parantu ticha colormap ha TrueColor cha colormap aselach as nahi mhanun tayar kelelya visual info chi screen dili.
	visualInfo->visual,
	AllocNone
	
	);//mala mazya Truecolor visual la map honara colormap de,
	windowAttributes.event_mask=ExposureMask| KeyPressMask|StructureNotifyMask|FocusChangeMask;//this member decides which event messages to be responded to and which should be masked.
	//ExposureMask (= wM_PAINT in Windows )	
	
	//step 06: Initialize global Colormap using using colormap from window attributes.
	colormap=windowAttributes.colormap;
	
	//step 07:Initialize window style using StyleMask.
	styleMask=CWBorderPixel|CWBackPixel|CWColormap|CWEventMask;
	//step_08: Create the window using XCreateWindow()
	//(=CreateWindow() in Windows )and do error checking.
	
	window=XCreateWindow(
		gpDisplay,
		RootWindow(gpDisplay,visualInfo->screen),
		0,
		0,
		WIN_WIDTH,
		WIN_HEIGHT,
		0,
		visualInfo->depth,
		InputOutput,
		visualInfo->visual,
		styleMask,
		&windowAttributes
		);	
		
		if(!window)
		{
			printf("Error:XCreateWindow() failed\n");
			uninitialize();
			exit(1);
		
		}
		//step 08:Give name to the window in its title / caption bar.
		
		XStoreName(gpDisplay,window,"AJG: Window");
		
		//step 09:prepare our window to respond to :
		//A. Closing by clicking on close (X) button 
		//B, Closing by clicking on 'Close' option in system menu by creating and setting WindowManagerProtocolAtom.
//Why? ->karan close button ani system cha menu cha control XWindows kade nasto to window manager kade asto .Tyamule aplyala window manager la rule sangave lagtat i.e. protocols dyave lagtat ki kase operations kar window che ,
	wm_init_window_atom =XInternAtom(	
		gpDisplay,
		"WM_DELETE_WINDOW",
		True//ha atom tayar ahe ki nahi te paha.Tayar asel tar raha tayar nasel tar tayar kar.
		
		);//ha atom jasa kahi internship karayla jatoy i.e  swatachi  layki milvayla gelay mhanun function cha nav as ahe .
		XSetWMProtocols(
		gpDisplay,
		window,
		&wm_init_window_atom,
		1//Number of elements are there in the array specified in 3rd argument.
		
		);
		
		
		//sstep 10:show the window using  XMapWindow().
		XMapWindow(gpDisplay,window);
		
		
		//centering window
		screenWidth=XWidthOfScreen(XScreenOfDisplay(gpDisplay,defaultScreen));
		screenHeight=XHeightOfScreen(XScreenOfDisplay(gpDisplay,defaultScreen));
		XMoveWindow(gpDisplay,window,(screenWidth-WIN_WIDTH)/2,(screenHeight-WIN_HEIGHT)/2);
		
		iRetval = initialize();
		
		if (iRetval == -1)
		{
			fprintf(gpFile, "failed to get address of glXCreateContextAttribsARB().\n");
			uninitialize();
			exit(1);
		}
		else if(iRetval == -2)
		{
			fprintf(gpFile, "failed to create using  glXCreateContextAttribsARB().\n");
			uninitialize();
			exit(1);
		}
		else
		{
			fprintf(gpFile, "Initialization successful.\n");
		}
		
		//step 11:create the message loop by:
		//A.Getting next event using XNextEvent().
		//B.Handling key press of ESC key.
		//C.Handling message '33'.
		while(bDone==False)
		{	//peek window
			while(XPending(gpDisplay))
			{
			//grt message
			XNextEvent(gpDisplay,&event);
			
			
			switch(event.type)
			{
				//WM_CREATE in windows
				case MapNotify://(=WM_CREATE in windows) je je pudhe lagnar ahe te ethe banvun ghyava
				//fontStruct=XLoadQueryFont(gpDisplay,"fixed");//fixed font is default font on XWindows (= system font on windows)
				break;
				
				case FocusIn:
				bActiveWindow=True;
				break;
				
				case FocusOut:
				bActiveWindow=False;
				break;
				
				case KeyPress:
				keySym=XkbKeycodeToKeysym(
				
					gpDisplay,
					event.xkey.keycode,
					0,
					0
				);
						
				switch(keySym)
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
					if(gbFullscreen==False)
					{
						ToggleFullscreen();
						gbFullscreen=True;
					
					}
					else
					{
					
						ToggleFullscreen();
						gbFullscreen=False;
					}
					break;
				}
				break;
				
				case ConfigureNotify:
				winWidth=event.xconfigure.width;
				winHeight=event.xconfigure.height;
				resize(winWidth,winHeight);
				break;
				
				case Expose:
				break;
				
				case 33: //apan atom madhe WM_DELETE_WINDOW register kel ahe tyamule close operation chya velela 33 ha event.type yeto.
				bDone=True;
				break;
				
				default:
				break;
				
			}		
		}
		if(bActiveWindow==True)
		{
			//update();
			
			display();	
		}
	}	
		
		//step_12: After closing message loop call uninitialize() and return
		uninitialize();
		
	return(0);	

}

void ToggleFullscreen(void)
{

	//local variable functions
	Atom wm_current_state_atom;
	Atom wm_fullscreen_state_atom;
	XEvent event;
	
	//code
	wm_current_state_atom=
	XInternAtom(gpDisplay,"_NET_WM_STATE",False);
	wm_fullscreen_state_atom=
	XInternAtom(gpDisplay,"_NET_WM_STATE_FULLSCREEN",False);
	
	memset(&event,0,sizeof(XEvent));
	event.type=ClientMessage;
	event.xclient.window=window;
	event.xclient.message_type=wm_current_state_atom;
	event.xclient.format=32;
	event.xclient.data.l[0]=gbFullscreen?0:1;
	event.xclient.data.l[1]=wm_fullscreen_state_atom;
	
	XSendEvent(
	gpDisplay,
	RootWindow(gpDisplay,visualInfo->screen),
	False,
	SubstructureNotifyMask,
	&event
	);
}


int initialize(void)
{
	//local function declaration
	void resize(int, int);
	void printGLInfo(void);
	void uninitialize(void);
	void loadMesh(void);

	//code

	/*glxContex=glXCreateContext(gpDisplay,
	visualInfo,
	NULL,
	True
	);*/

	//glXCreateContext() does not have the capacity to create pp comp

	glXCreateContextAttribsARB = (glXCreateContextAttribsARBProc)glXGetProcAddressARB((GLubyte*)"glXCreateContextAttribsARB");
	printf("breakpoint_0");
	if (glXCreateContextAttribsARB == NULL)
	{
		printf("glXGetProcAddressARB() failed.\n");
		return(-1);
		
	}

	printf("breakpoint_1");
	GLint contextAttributes[] = {
		GLX_CONTEXT_MAJOR_VERSION_ARB, 4,
		GLX_CONTEXT_MINOR_VERSION_ARB, 5,
		GLX_CONTEXT_PROFILE_MASK_ARB, GLX_CONTEXT_CORE_PROFILE_BIT_ARB,
		None

	};

	glxContex = glXCreateContextAttribsARB(
		gpDisplay,
		glxFBConfig,
		NULL,//shared context
		True,
		contextAttributes
	);

	if (!glxContex)
	{
		GLint contextAttributes[] = {
		   GLX_CONTEXT_MAJOR_VERSION_ARB, 1,
		   GLX_CONTEXT_MINOR_VERSION_ARB, 0,
		   None

		};

		glxContex = glXCreateContextAttribsARB(
			gpDisplay,
			glxFBConfig,
			NULL,//shared context
			True,
			contextAttributes
		);
			
		
		if (glxContex)
		{
			fprintf(gpFile, "cannot support version 4.5 hence falling back to default version.\n");
		}
		else
		{
			fprintf(gpFile, "cannot support any version.\n");
			return (-2);
		}
	}
	else
	{
		fprintf(gpFile, " support for 4.5 version.\n");
	}
	
	if (!glXIsDirect(gpDisplay, glxContex))//checking h/w or s/w rendering is supported
	{
		fprintf(gpFile, "direct rendering i.e h/w rendering is not supported.\n");
	}
	else
	{
		fprintf(gpFile, "direct rendering i.e h/w rendering is  supported.\n");

	}

	glXMakeCurrent(
		gpDisplay,
		window,
		glxContex
	);

	// Make the rendering context as the current context
	// if (wglMakeCurrent(ghdc, ghrc) == FALSE)
	// {
	// 	return(-4);
	// }

	
	//glew initialiation
	if (glewInit() != GLEW_OK)
	{
		return(-5);
	}
	printGLInfo();

		//vertex shader
	const GLchar* vertexShaderSourcecode =
		"#version 460 core" \
		"\n" \
		"in vec4 a_position;" \
		"uniform mat4 u_mvpMatrix;" \
		"void main(void)" \
		"{" \
		"gl_Position = u_mvpMatrix * a_position;" \
		"}";

	//create the shader
	GLuint vertexShaderObject = glCreateShader(GL_VERTEX_SHADER);
	
	//give shader code to shader object
	glShaderSource(vertexShaderObject, 1, (const GLchar**)&vertexShaderSourcecode, NULL);

	//compile the code(programatically compilation by OpenGL engine (machine code for GPU) (inline compiler)
	glCompileShader(vertexShaderObject);

	//Error checking of shader compilation
	GLint status;
	GLint infoLogLength;
	char* log = NULL;

	//a.getting compilation status
	glGetShaderiv(vertexShaderObject, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE)
	{
		//b.getting length of log (message) compilation
		glGetShaderiv(vertexShaderObject, GL_INFO_LOG_LENGTH, &infoLogLength);
		if (infoLogLength > 0)
		{
			//c.allocate enough memory
			log = (char*)malloc(infoLogLength);
			if (log != NULL)
			{
				//d.get the compilation log into this buffer
				GLsizei written;
				glGetShaderInfoLog(vertexShaderObject, infoLogLength, &written, log);
				//e.display the contents of buffer
				fprintf(gpFile, "vertex shader compilation log: %s\n",log);
				//f.free log
				free(log);
				//g.exit the application
				uninitialize();
			}
		}
	}

	//fragment shader
	const GLchar* fragmentShaderSourcecode =
		"#version 460 core" \
		"\n" \
		"out vec4 FragColor;" \
		"void main(void)" \
		"{" \
		"FragColor = vec4(1.0,1.0,1.0,1.0);" \
		"}";

	//create the shader
	GLuint fragmentShaderObject = glCreateShader(GL_FRAGMENT_SHADER);
	
	//give shader code to shader object
	glShaderSource(fragmentShaderObject, 1, (const GLchar**)&fragmentShaderSourcecode, NULL);

	//compile the code(programatically compilation by OpenGL engine (machine code for GPU) (inline compiler)
	glCompileShader(fragmentShaderObject);
	
	//reinitialization
	 status=0;
	 infoLogLength=0;
	 log = NULL;

	 //a.getting compilation status
	 glGetShaderiv(fragmentShaderObject, GL_COMPILE_STATUS, &status);
	 
	 if (status == GL_FALSE)
	 {
		 //b.getting length of log (message) compilation
		 glGetShaderiv(fragmentShaderObject, GL_INFO_LOG_LENGTH, &infoLogLength);
		 if (infoLogLength > 0)
		 {
			 //c.allocate enough memory
			 log = (char*)malloc(infoLogLength);
			 if (log != NULL)
			 {
				 //d.get the compilation log into this buffer
				 GLsizei written;
				 glGetShaderInfoLog(fragmentShaderObject, infoLogLength, &written, log);
				 //e.display the contents of buffer
				 fprintf(gpFile, "fragment shader compilation log: %s\n", log);
				 //f.free log
				 free(log);
				 //g.exit the application
				 uninitialize();
			 }
		 }
	 }
	
	 //d.1.shader program object
	 shaderProgramObject = glCreateProgram();
	 
	 //d.2.attach desire shaders to this shader obj
	 glAttachShader(shaderProgramObject, vertexShaderObject);
	 glAttachShader(shaderProgramObject, fragmentShaderObject);

	 //a1.prelink binding of shader program with vertex attributes
	 glBindAttribLocation(shaderProgramObject,AMC_ATRIBUTE_POSITION,"a_position");



	 //d.3.link shader obj
	 glLinkProgram(shaderProgramObject);

	 //d.4.linking status
	 
	 status = 0;
	 infoLogLength = 0;
	 log = NULL;

	 glGetProgramiv(shaderProgramObject, GL_LINK_STATUS, &status);

	 if (status == GL_FALSE)
	 {
		 glGetProgramiv(shaderProgramObject, GL_INFO_LOG_LENGTH, &infoLogLength);
		 if (infoLogLength > 0)
		 {
			 log = (char*)malloc(infoLogLength);
			 if (log != NULL)
			 {
				 GLsizei written;
				 glGetProgramInfoLog(shaderProgramObject, infoLogLength, &written, log);
				 fprintf(gpFile, "shader pprogram link log:%s\n", log);
				 free(log);

				 uninitialize();
			 }
		 }
	 }

	 mvpMatrixUniform = glGetUniformLocation(shaderProgramObject, "u_mvpMatrix");

	 loadMesh();
	//VAO and VBO related code
	 glGenVertexArrays(1, &VAO);//1 VAO
	 /*1 address dila jato bind n unbind madhe ja lines lihilya jatat tya saglya record hotat*/
	 glBindVertexArray(VAO);

	 glGenBuffers(1, &VBO_position);
	 glBindBuffer(GL_ARRAY_BUFFER, VBO_position);
	 //array hold karnara buffer. gpu ch aat memory milali tyat target kele

	 //data bharaychay target cha side ni liha GL_STATIC_DRAW->attach bhar data
	 glBufferData(GL_ARRAY_BUFFER,
		gpVertexSorted->size *  sizeof(GLfloat), gpVertexSorted->pFloat, GL_STATIC_DRAW);

	 //AMC_ATRIBUTE_POSITION=0
	 /*shader madhe me 0th pos la ha data map karat ahe (targetaed data from gpu memory),
		yache tu baghtana 3-3 cha set kar,
		tyala float data type laav(gpu float vegla asto ),not sure data normalize(1,-1) asel he sangto,kiticha stride(dhanga) taku,dhanga takaycha astil tar to data kuthay*/
	 glVertexAttribPointer(AMC_ATRIBUTE_POSITION,3,GL_FLOAT,GL_FALSE,0,NULL );

	 glEnableVertexAttribArray(AMC_ATRIBUTE_POSITION);
	 //VBO ha opec pointer ahe
	 glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	 //elements vbo
	 glGenBuffers(1, &VBO_element);
	 glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBO_element);
	 glBufferData(GL_ELEMENT_ARRAY_BUFFER,
		 gpVertexIndices->size * sizeof(int), gpVertexIndices->pInt, GL_STATIC_DRAW);
	 glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	 glBindVertexArray(0);


	// Here starts OpenGL code
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	//depth related changes
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	persepectiveProjectionMAtrix = mat4::identity();

	resize(WIN_WIDTH, WIN_HEIGHT);
	return(0);
	
}
void printGLInfo(void)
{
	//local vatiavable declaration
	GLint numExtensions = 0;

	//code
	fprintf(gpFile, "OpenGL vendor: %s \n", glGetString(GL_VENDOR));
	fprintf(gpFile, "OpenGL renderer: %s \n", glGetString(GL_RENDERER));
	fprintf(gpFile, "OpenGL version: %s \n", glGetString(GL_VERSION));
	fprintf(gpFile, "GLSL version: %s \n", glGetString(GL_SHADING_LANGUAGE_VERSION));//GLSL--?graphic library shading language


	glGetIntegerv(GL_NUM_EXTENSIONS, &numExtensions);
	fprintf(gpFile, "number of supported extensions : %d\n", numExtensions);
	//fprintf(gpFile,"OpenGL version: %s \n",glGetString(GL_VERSION));
	for (int i = 0; i < numExtensions; i++)
	{
		fprintf(gpFile, "GLSL Version :%s\n", glGetStringi(GL_EXTENSIONS, i));
	}
}

void resize(int width, int height)
{
	// Code
	if (height == 0) // To avoid divide by zero instruction (illegal instruction) in future
		height = 1;

	glViewport(0, 0, (GLsizei)width, (GLsizei)height);

	persepectiveProjectionMAtrix = vmath::perspective(45.0f,
		((GLfloat)width / (GLfloat)height),
		0.1f,
		100.0f);
}

void display(void)
{
	// Code
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	//je kahi draw karnare shader madhe means code te ethe use karnar
	//e.1.use shader program object
	glUseProgram(shaderProgramObject);

	//transformations
	mat4 translationMatrix = mat4::identity();
	translationMatrix = vmath::translate(0.0f, 0.0f, -4.0f);
	mat4 modelViewMatrix = mat4::identity();
	//mat4 modelViewMatrix = mat4::identity();
	mat4 modelViewProjectionMatrix = mat4::identity();

	//translationMatrix = vmath::translate(0.0f, 0.0f, -4.0f); 
	modelViewMatrix = translationMatrix;
	
	modelViewProjectionMatrix = persepectiveProjectionMAtrix * modelViewMatrix;

	glUniformMatrix4fv(
		mvpMatrixUniform,//initialize madhe jo uniform ghetlay to
		1,
		GL_FALSE,
		modelViewProjectionMatrix
	);

	glPointSize(5.0f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBO_element);
	glDrawElements(GL_TRIANGLES,gpVertexIndices->size,
		GL_UNSIGNED_INT,NULL);

	glBindVertexArray(0);

	//e.2.draw desire graphics here
	
	//e.3.unuse shader program object
	glUseProgram(0);
	glXSwapBuffers(gpDisplay, window);
}

void update(void)
{
	//code
}

	//step 12 : after closing message loop call uniToggleFullscreennitialize() and return.
void uninitialize(void)
{
	// Local function declarations
	void ToggleFullscreen(void);
	GLXContext currentContext;

	// Code
	if (gbFullscreen)
	{
		ToggleFullscreen();
	}

	//delesion of vbo
	if (VBO_element)
	{
		glDeleteBuffers(1, &VBO_element);
		VBO_element = 0;
	}

	if (VBO_position)
	{
		glDeleteBuffers(1, &VBO_position);
		VBO_position = 0;
	}

	//delesion of VAO
	if (VAO)
	{
		glDeleteVertexArrays(1, &VAO);
			VAO = 0;
	}
	//shader uninitialization
	if (shaderProgramObject)
	{

		//f.0.again use shader program object
		glUseProgram(shaderProgramObject);
		GLsizei numAttachedShaders;
		//f.1.get no of attach shaders
		glGetProgramiv(shaderProgramObject,GL_ATTACHED_SHADERS,&numAttachedShaders);
		//f.2.create empty buffer to hold array of shader objects
		GLuint* shaderObjects = NULL;
		//f.3.
		shaderObjects = (GLuint*)malloc(numAttachedShaders*sizeof(GLuint));
		//f.4 fill it
		glGetAttachedShaders(shaderProgramObject,numAttachedShaders,&numAttachedShaders,shaderObjects);
		//f.5.
		for (GLsizei i = 0; i < numAttachedShaders; i++)
		{
			glDetachShader(shaderProgramObject, shaderObjects[i]);
			glDeleteShader(shaderObjects[i]);

			shaderObjects[i] = 0;
		}

		//f.6 free memory allocated for buffer
		free(shaderObjects);
		shaderObjects = NULL;

		//f.7.unuse shader program object
		glDeleteProgram(shaderProgramObject);
		shaderProgramObject = 0;


	}

	currentContext=glXGetCurrentContext();
	if(currentContext && currentContext == glxContex)
	{
		glXMakeCurrent(gpDisplay,0,0);
	}

	if(glxContex)
	{
		glXDestroyContext(gpDisplay,glxContex);
		glxContex = NULL;
	}

	if(visualInfo)
	{
		free(visualInfo);
		visualInfo = NULL;
	}	

	if(window)
	{
		XDestroyWindow(gpDisplay,window);
	}

	if(colormap)
	{
		XFreeColormap(gpDisplay,colormap);
	}

	if(gpDisplay)
	{
		XCloseDisplay(gpDisplay);
		gpDisplay=NULL;

	}

	if(gpFile)
	{
		fprintf(gpFile,"closing log file.\n");
		fclose(gpFile);
		gpFile=NULL;
	}
}


//mesh loading functions
void loadMesh(void)
{
	//code
	VECTOR_INT* createVectorInteger(void);
	VECTOR_FLOAT* createVectorFloat(void);
	void pushBackVectorInteger(VECTOR_INT*, int);
	void pushBackVectorFloat(VECTOR_FLOAT*, float);
	void destroyVectorInteger(VECTOR_INT*);
	void destroyVectorFloat(VECTOR_FLOAT*);
	void uninitialize(void);

	const char* space = " ";
	const char* slash = "/";
	const char* firstToken = NULL;
	const char* token;

	char* fEntries[3] = { NULL , NULL , NULL };

	int i, vi;

	gp_MeshFile = fopen("MonkeyHead.OBJ", "r");
	if (gp_MeshFile == NULL)
	{
		fprintf(gpFile, "Error in opening mesh file.\n");
		uninitialize();
		exit(EXIT_FAILURE);
	}

	gpVertex = createVectorFloat();
	gpTexture = createVectorFloat();
	gpNormal = createVectorFloat();

	gpVertexIndices = createVectorInteger();
	gpTextureIndices = createVectorInteger();
	gpNormalIndices = createVectorInteger();

	while (fgets(buffer, BUFFER_SIZE, gp_MeshFile) != NULL)
	{
		firstToken = strtok(buffer, space);

		if (strcmp(firstToken, "v") == 0)
		{
			nrPosCoords++;
			while ((token = strtok(NULL, space)) != NULL)
				pushBackVectorFloat(gpVertex, atof(token));
		}
		else if (strcmp(firstToken, "vt") == 0)
		{
			nrTexCoords++;
			while ((token = strtok(NULL, space)) != NULL)
				pushBackVectorFloat(gpTexture, atof(token));
		}
		else if (strcmp(firstToken, "vn") == 0)
		{
			nrTexCoords++;
			while ((token = strtok(NULL, space)) != NULL)
				pushBackVectorFloat(gpNormal, atof(token));
		}
		else if (strcmp(firstToken, "f") == 0)
		{
			nrTexCoords++;
			for (i = 0; i < 3; i++)
			{
				fEntries[i] = strtok(NULL, space);
			}

			for (i = 0; i < 3; i++)
			{
				token = strtok(fEntries[i], slash);
				pushBackVectorInteger(gpVertexIndices, atoi(token) - 1);

				token = strtok(NULL, slash);
				pushBackVectorInteger(gpTextureIndices, atoi(token) - 1);

				token = strtok(NULL, slash);
				pushBackVectorInteger(gpNormalIndices, atoi(token) - 1);
			}
		}

	}

	//sorting arrays
	gpVertexSorted = createVectorFloat();
	for (int i = 0; i < gpVertexIndices->size; i++)
	{
		pushBackVectorFloat(gpVertexSorted, gpVertex->pFloat[i]);
	}
	gpTextureSorted = createVectorFloat();
	for (int i = 0; i < gpTextureIndices->size; i++)
	{
		pushBackVectorFloat(gpTextureSorted, gpTexture->pFloat[i]);
	}
	gpNormalSorted = createVectorFloat();
	for (int i = 0; i < gpNormalSorted->size; i++)
	{
		pushBackVectorFloat(gpNormalSorted, gpNormal->pFloat[i]);
	}

	fclose(gp_MeshFile);
	gp_MeshFile = NULL;

}

//Integer vector
VECTOR_INT* createVectorInteger(void)
{
		void uninitialize(void);
		VECTOR_INT* pVecInt = (VECTOR_INT*)calloc(1, sizeof(VECTOR_INT));

		if (pVecInt == NULL)
		{
			fprintf(gpFile, "Error creating vector:calloc() failed to allocate memory to pVecInt.\n");
			uninitialize();
			exit(EXIT_FAILURE);
		}

		pVecInt->pInt = NULL;
		pVecInt->size = 0;

		return(pVecInt);

}

void destroyVectorInteger(VECTOR_INT* pVecInt)
{
	if (pVecInt->pInt != NULL)
	{
		free(pVecInt->pInt);
		pVecInt = NULL;
	}

	if (pVecInt != NULL)
	{
		free(pVecInt);
		pVecInt = NULL;
	}

}


void pushBackVectorInteger(VECTOR_INT* pVecInt, int newElement)
{
	void uninitialize(void);

	pVecInt->size += 1;

	pVecInt->pInt = (int*)realloc(pVecInt->pInt, (pVecInt->size) * sizeof(int));
	if (pVecInt->pInt == NULL)
	{
		fprintf(gpFile, "ERROR push back: realloc() failed to allocate memory to pVecInt->pi.\n");
		uninitialize();
		exit(EXIT_FAILURE);
	}
	pVecInt->pInt[pVecInt->size - 1] = newElement;
}

int popBackVectorInteger(VECTOR_INT* pVecInt, int* poppedValue)
{
	if (pVecInt->size == 0)
	{
		return(VECTOR_EMPTY);
	}
	
	*poppedValue = pVecInt->pInt[(pVecInt->size) - 1];

	pVecInt->size -= 1;
	if (pVecInt->size > 1)
	{
		pVecInt->pInt = (int*)realloc(pVecInt->pInt, (pVecInt->size) * sizeof(int));
	}

	if (pVecInt->pInt == NULL)
	{
		fprintf(gpFile, "Error pop back:realloc() failed to allocate memory to pVecInt->pi.\n");
		uninitialize();
		exit(EXIT_FAILURE);
	}

	return(SUCCESS);

}

//float vector

VECTOR_FLOAT* createVectorFloat(void)
{
	void uninitialize(void);
	VECTOR_FLOAT* pVecFloat = (VECTOR_FLOAT*)calloc(1, sizeof(VECTOR_FLOAT));

	if (pVecFloat == NULL)
	{
		fprintf(gpFile, "Error creating vector:calloc() failed to allocate memory to pVecFloat.\n");
		uninitialize();
		exit(EXIT_FAILURE);
	}

	pVecFloat->pFloat = NULL;
	pVecFloat->size = 0;

	return(pVecFloat);
}


void destroyVectorFloat(VECTOR_FLOAT* pVecFloat)
{
	if (pVecFloat->pFloat != NULL)
	{
		free(pVecFloat->pFloat);
		pVecFloat = NULL;
	}

	if (pVecFloat != NULL)
	{
		free(pVecFloat);
		pVecFloat = NULL;
	}

}


void pushBackVectorFloat(VECTOR_FLOAT* pVecFloat, float newElement)
{
	void uninitialize(void);

	pVecFloat->size += 1;

	pVecFloat->pFloat = (float*)realloc(pVecFloat->pFloat, (pVecFloat->size) * sizeof(float));
	if (pVecFloat->pFloat == NULL)
	{
		fprintf(gpFile, "ERROR push back: realloc() failed to allocate memory to pVecFloat->pf.\n");
		uninitialize();
		exit(EXIT_FAILURE);
	}
	pVecFloat->pFloat[pVecFloat->size - 1] = newElement;

}

float popBackVectorFloat(VECTOR_FLOAT* pVecFloat, float* poppedValue)
{
	if (pVecFloat->size == 0)
	{
		return(VECTOR_EMPTY);
	}

	*poppedValue = pVecFloat->pFloat[(pVecFloat->size) - 1];

	pVecFloat->size -= 1;
	if (pVecFloat->size > 1)
	{
		pVecFloat->pFloat = (float*)realloc(pVecFloat->pFloat, (pVecFloat->size) * sizeof(float));
	}

	if (pVecFloat->pFloat == NULL)
	{
		fprintf(gpFile, "Error pop back:realloc() failed to allocate memory to pVecInt->pi.\n");
		uninitialize();
		exit(EXIT_FAILURE);
	}

	return(SUCCESS);

}



