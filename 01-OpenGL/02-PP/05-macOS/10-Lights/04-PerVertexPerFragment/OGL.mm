// sh build.sh
// Header files
#import <foundation/Foundation.h>
#import <cocoa/cocoa.h>
// OpenGL
#import <QuartzCore/CVDisplayLink.h>
#import <OpenGL/gl3.h>
#import <OpenGL/gl3ext.h>
#import "vmath.h"
#import "Sphere.h"

using namespace vmath;





// Global Function Declaration
CVReturn MyDisplayLinkCallback(CVDisplayLinkRef, const CVTimeStamp*, const CVTimeStamp*,
                               CVOptionFlags, CVOptionFlags*, void*);

// Global varaible declarations
FILE *gpFile = NULL;
int bLight;
GLfloat lightAmbient[] = { 0.0f,0.0f,0.0f,1.0f };
GLfloat lightDiffuse[] = { 1.0f,1.0f,1.0f,1.0f };
GLfloat lightSpecular[] = { 1.0f,1.0f,1.0f,1.0f };
GLfloat lightPosition[] = { 100.0f,100.0f,100.0f,1.0f };

GLfloat materialAmbient[] = { 0.0f,0.0f,0.0f,1.0f };
GLfloat materialDiffuse[] = { 1.0f,1.0f,1.0f,1.0f };
GLfloat materialSpecular[] = { 1.0f,1.0f,1.0f,1.0f };
GLfloat materialShininess = 50.0f;
GLchar choosenShader = 'v';
// Interface/class declarations
@interface AppDelegate : NSObject < NSApplicationDelegate, NSWindowDelegate >
  
@end

@interface GLView : NSOpenGLView 

@end


// EntryPoint Function

int main(int argc, char* argv[])
{
    // code
    NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init]; // Memroy management 

    NSApp = [NSApplication sharedApplication];

    AppDelegate* appDelegate = [[AppDelegate alloc] init];

    [NSApp setDelegate: appDelegate];

    // GameLoop
    [NSApp run];
     
    // release 
    [pool release];

    return 0;
}

// implementation of AppDelegate
@implementation AppDelegate
{
    @private
    NSWindow *window;
    GLView *view;
}

- (void) applicationDidFinishLaunching: (NSNotification*) notification
{
    // code
   // NSLog(@"Application Started Successfully !!\n");
    NSBundle *appBundle = [NSBundle mainBundle];
    NSString *appDirPath = [appBundle bundlePath];
    NSString *parentDirPath = [appDirPath stringByDeletingLastPathComponent];
    NSString *logFileNameWithPath = [NSString stringWithFormat:@"%@/log.txt", parentDirPath];
    const char* pszLogFileNameWithPath = [logFileNameWithPath cStringUsingEncoding:NSASCIIStringEncoding];
   // const char* pszLogFileNameWithPath = [logFileNameWithPath UTF8String];

    gpFile = fopen(pszLogFileNameWithPath, "w");
    if(gpFile == NULL)
    {
        [self release];
        [NSApp terminate:self];
    }

    fprintf(gpFile, "program is started Successfully\n");

    NSRect rect = NSMakeRect(0.0, 0.0, 800.0, 600.0);
    window = [[NSWindow alloc] initWithContentRect:rect
                                styleMask:NSWindowStyleMaskTitled | NSWindowStyleMaskClosable | NSWindowStyleMaskMiniaturizable | NSWindowStyleMaskResizable
                                backing:NSBackingStoreBuffered
                                defer:NO];

    [window setTitle:@"macOS Sanket Pawar"];
    NSColor *backgroundColor = [NSColor blackColor];
    [window setBackgroundColor: backgroundColor];

    // create view
    view = [[GLView alloc] initWithFrame:rect];
    //set view
    [window setContentView:view];

    [window center];
    [window setDelegate:self];
    [window makeKeyAndOrderFront:self];
   



}

- (void) applicationWillTerminate: (NSNotification*) notification
{
    // code
    // NSLog(@"Application Terminate Successfully !!\n");
    if(gpFile)
    {
        fprintf(gpFile, "program is Terminate Successfully\n");
        fclose(gpFile);
        gpFile = NULL;
    }
    
}

- (void) windowWillClose: (NSNotification*) notification
{
    // code
    [NSApp terminate:self];
}

-(void) dealloc
{
    // code
    if(view)
    {
        [view release];
        view = nil;
    }

    if(window)
    {
        [window release];
        window = nil;
    }
    
    [super dealloc];
}
@end 



@implementation GLView
{
    @private
    CVDisplayLinkRef displayLink;
    // Programable pipeline related global variables
    GLuint shaderProgramObject;

    enum
    {
        AMC_ATTRIBUTE_POSITION = 0,
        AMC_ATTRIBUTE_COLOR,
        AMC_ATTRIBUTE_NORMAL,
        AMC_ATTRIBUTE_TEXTURE0
    };

    GLuint VOA;
    GLuint VBO;
    GLuint projectionMatrixUniform;
	GLuint modelMatrixUniform;
	GLuint viewMatrixUniform;
	GLfloat angleSphere;
	GLuint vao_sphere;
	GLuint vbo_sphere_position;
	GLuint vbo_sphere_normal;
	GLuint vbo_sphere_indices;
	int numVertices;
	int numElements;
    /* LIGHTS */
	GLuint laUniform;
	GLuint ldUniform;
	GLuint lsUniform;
	GLuint kaUniform;
	GLuint kdUniform;
	GLuint ksUniform;
	GLuint materialShininessUniform;
	GLuint lightPositionUniform;
	GLuint LightingEnabledUniform;
	mat4 perceptivegraphicsProjectionMatrix;

	// Programable pipeline related global variables
	GLuint shaderProgramObject_PerVertex;
	GLuint vertexShaderObject_PerVertex;
	GLuint fragmentShaderObject_PerVertex;
	GLuint shaderProgramObject_PerFragment;
	GLuint vertexShaderObject_PerFragment;
	GLuint fragmentShaderObject_PerFragment;
}

-(id) initWithFrame:(NSRect)frame
{
    // code
    self = [super initWithFrame:frame];

    if(self)
    {
        // 1. Init Array of OpenGL Pixel format attribute 
        NSOpenGLPixelFormatAttribute openGLattributes[] =
        {
            NSOpenGLPFAOpenGLProfile, NSOpenGLProfileVersion4_1Core,
            NSOpenGLPFAScreenMask, CGDisplayIDToOpenGLDisplayMask(kCGDirectMainDisplay),
            NSOpenGLPFAColorSize, 24,
            NSOpenGLPFADepthSize, 24,
            NSOpenGLPFAAlphaSize, 8,
            NSOpenGLPFANoRecovery,
            NSOpenGLPFAAccelerated,
            NSOpenGLPFADoubleBuffer,
            0
        };

        //2. Create OprnGL Pixel Format using attribute
        NSOpenGLPixelFormat *gLPixelFormat = [[[NSOpenGLPixelFormat alloc] initWithAttributes:openGLattributes] autorelease];
        if(gLPixelFormat == nil)
        {
            fprintf(gpFile, "Error : Failed to get OpenGL Pixel Format.\n");
            [self uninitialize];
            [self release];
            [NSApp terminate:self];
        }
        
        //3. Create OpenGL context in above format
        NSOpenGLContext *gLContext = [[[NSOpenGLContext alloc] initWithFormat:gLPixelFormat shareContext:nil] autorelease];
        if(gLContext == nil)
        {
            fprintf(gpFile, "Error : Failed to get OpenGL Context.\n");
            [self uninitialize];
            [self release];
            [NSApp terminate:self];
        }

        //4.  Set this view's Pixel Format using above Pixel Format 
        [self setPixelFormat:gLPixelFormat];
        
        //5. Set View's by using Above OpenGL Context
        [self setOpenGLContext:gLContext];
    }

    return (self);
}

// Define getFrameForTime() with predefine param
-(CVReturn)getFrameForTime:(const CVTimeStamp*)outputTime
{
    //code
    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
    [self drawView];
    [pool release];
    
    return (kCVReturnSuccess);
}

// overrideable
-(void)prepareOpenGL
{   
    // code
    [super prepareOpenGL];
    [[self openGLContext] makeCurrentContext];


    // set boudble buffer swaping interval into one
    GLint swapInterval = 1;

    [[self openGLContext] setValues:&swapInterval forParameter:NSOpenGLCPSwapInterval];

    // OpenGL Log
    fprintf(gpFile, "Renderer : %s\n", glGetString(GL_RENDERER));
    fprintf(gpFile, "Version : %s\n", glGetString(GL_VERSION));
    fprintf(gpFile, "GLSL Version : %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

    [self initialize];

    // cretae, config , start displaylink

    // create displaylink
    CVDisplayLinkCreateWithActiveCGDisplays(&displayLink);

    // set callback in displaylink
    CVDisplayLinkSetOutputCallback(displayLink, &MyDisplayLinkCallback, self);

    // convert nsopengl pixel foramt to cgl pixel format
    CGLPixelFormatObj cglPixelformat = (CGLPixelFormatObj)[[self pixelFormat] CGLPixelFormatObj];

    //convert cgl context
    CGLContextObj cglContext = (CGLContextObj)[[self openGLContext] CGLContextObj];

    // using above info set current CG display to CGL pixel format
    CVDisplayLinkSetCurrentCGDisplayFromOpenGLContext(displayLink, cglContext, cglPixelformat);

    // start displaylink
    CVDisplayLinkStart(displayLink);


}

-(void)drawRect:(NSRect)dirtyRect
{
    //code
    [self drawView];
}

-(void)drawView
{
    //code
    [[self openGLContext] makeCurrentContext];

    CGLLockContext((CGLContextObj)[[self openGLContext] CGLContextObj]);

    // call ur display here
    [self display];

    [self myUpdate];

    // do double buffering
    CGLFlushDrawable((CGLContextObj)[[self openGLContext] CGLContextObj]);

    CGLUnlockContext((CGLContextObj)[[self openGLContext] CGLContextObj]);
}

-(int)initialize
{
	[self InitVertexShaderPerVertex];
    [self InitFragmentShaderPerVertex];
	[self InitShaderProgramPerVertex];

	[self InitVertexShaderPerFragment];
	[self InitFragmentShaderPerFragment];
	[self InitShaderProgramPerFragment];

	[self InitSphere];
	

	// VOA AND VBA Array related lines
	
	 	

	// Here Starts OpenGL Code
	// Clear the Screen using Blue Color
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	// Depth Related Changes
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);


	perceptivegraphicsProjectionMatrix = mat4::identity();
	
    return 0;
}
-(void)InitShaderProgramPerVertex
{
	// 1. Create shader program object
	shaderProgramObject_PerVertex = glCreateProgram();

	// 2. attach desire shaders to this shader program object
	glAttachShader(shaderProgramObject_PerVertex, vertexShaderObject_PerVertex);
	glAttachShader(shaderProgramObject_PerVertex, fragmentShaderObject_PerVertex);

	//  Pre-linking binding of shader program object with vertex attributes 
	glBindAttribLocation(shaderProgramObject_PerVertex, AMC_ATTRIBUTE_POSITION, "a_position");

	// Bind the normal attribute location before linking.
	glBindAttribLocation(shaderProgramObject_PerVertex, AMC_ATTRIBUTE_NORMAL, "a_normal");


	// 3. link shader program object
	glLinkProgram(shaderProgramObject_PerVertex);

	// retriving/getting uniformed location from shader program object
	modelMatrixUniform = glGetUniformLocation(shaderProgramObject_PerVertex,
		"u_modelMatrix");

	viewMatrixUniform = glGetUniformLocation(shaderProgramObject_PerVertex,
		"u_viewMatrix");

	projectionMatrixUniform = glGetUniformLocation(shaderProgramObject_PerVertex,
		"u_projectionMatrix");

	laUniform = glGetUniformLocation(shaderProgramObject_PerVertex, "u_la");
	ldUniform = glGetUniformLocation(shaderProgramObject_PerVertex, "u_ld");
	lsUniform = glGetUniformLocation(shaderProgramObject_PerVertex, "u_ls");
	kaUniform = glGetUniformLocation(shaderProgramObject_PerVertex, "u_ka");
	kdUniform = glGetUniformLocation(shaderProgramObject_PerVertex, "u_kd");
	ksUniform = glGetUniformLocation(shaderProgramObject_PerVertex, "u_ks");
	lightPositionUniform = glGetUniformLocation(shaderProgramObject_PerVertex, "u_lightPosition");
	materialShininessUniform = glGetUniformLocation(shaderProgramObject_PerVertex, "u_materialShininess");

	LightingEnabledUniform = glGetUniformLocation(shaderProgramObject_PerVertex, "LightingEnabled");

	// 4. do link error checking with similar a to g steps like above
	GLint status = 0;
	GLint infoLogLen = 0;
	char* log = NULL;

	// a. Getting link status
	glGetProgramiv(shaderProgramObject_PerVertex,
		GL_LINK_STATUS,
		&status);

	if (status == GL_FALSE)
	{
		// Getting length of log of LINK status
		glGetProgramiv(shaderProgramObject_PerVertex,
			GL_INFO_LOG_LENGTH,
			&infoLogLen);

		if (infoLogLen > 0)
		{
			// // Allocate enough memory to buffer to hold the log 
			log = (char*)malloc(infoLogLen);
			if (log != NULL)
			{
				// Get the log into this allocated buffer.
				GLsizei written;
				glGetProgramInfoLog(shaderProgramObject_PerVertex,
					infoLogLen,
					&written,
					log);

				// display the contents of buffer
				fprintf(gpFile, "Shader Program shaderProgramObject_PerVertex Link Log : % s\n", log);

				// Free the allocated the buffer.
				free(log);

				// exit the application due to error
				[self uninitialize];
            	[self release];
            	[NSApp terminate:self];

			}

		}
	}
}

-(void)InitFragmentShaderPerVertex
{
	const GLchar* fragmentShaderSourcecode =
		"#version 410 core" \
		"\n" \
		"in vec3 phongAdsColor;" \
		"\n" \
		"out vec4 fragmentColor;" \
		"\n" \
		"void main(void)" \
		"{" \
		"   fragmentColor = vec4(phongAdsColor, 1.0);" \
		"}";

	// 2. Creating shader object
	fragmentShaderObject_PerVertex = glCreateShader(GL_FRAGMENT_SHADER);

	// 3. Giving shader code to shader object
	glShaderSource(fragmentShaderObject_PerVertex,
		1,
		(const GLchar**)&fragmentShaderSourcecode,
		NULL);

	// 4. Compile the shader
	glCompileShader(fragmentShaderObject_PerVertex);

	// 5. Error checking of shader compilation
	GLint status = 0;
	GLint infoLogLen = 0;
	char* log = NULL;

	// a. Getting compilation status
	glGetShaderiv(fragmentShaderObject_PerVertex,
		GL_COMPILE_STATUS,
		&status);

	if (status == GL_FALSE)
	{
		// Getting length of log of compilation status
		glGetShaderiv(fragmentShaderObject_PerVertex,
			GL_INFO_LOG_LENGTH,
			&infoLogLen);

		if (infoLogLen > 0)
		{
			// Allocate enough memory to buffer to hold the compilation log 
			log = (char*)malloc(infoLogLen);
			if (log != NULL)
			{
				// Get the compilation log into this allocated buffer.
				GLsizei written;
				glGetShaderInfoLog(fragmentShaderObject_PerVertex,
					infoLogLen,
					&written,
					log);

				// display the contents of buffer
				fprintf(gpFile, "Fragment Shader fragmentShaderObject_PerVertex Compilation Log: %s\n", log);

				// Free the allocated the buffer.
				free(log);

				// exit the application due to error
				[self uninitialize];
            	[self release];
            	[NSApp terminate:self];
			}
		}
	}
}


-(void)InitVertexShaderPerVertex
{
	const GLchar* vertexShaderSourcecode =
		"#version 410 core" \
		"\n" \
		"in vec4 a_position;" \
		"in vec3 a_normal;" \
		"\n" \
		"out vec3 phongAdsColor;" \
		"\n" \
		"uniform mat4 u_modelMatrix;" \
		"uniform mat4 u_viewMatrix;" \
		"uniform mat4 u_projectionMatrix;" \
		"uniform int LightingEnabled;" \
		"uniform vec3 u_la;" \
		"uniform vec3 u_ld;" \
		"uniform vec3 u_ls;" \
		"uniform vec3 u_ka;" \
		"uniform vec3 u_kd;" \
		"uniform vec3 u_ks;" \
		"uniform vec4 u_lightPosition;" \
		"uniform float u_materialShininess;" \
		"\n" \
		"void main(void)" \
		"{" \
		"   if(LightingEnabled == 1)" \
		"   {" \
		"       vec4 eyeCoordinates = u_viewMatrix * u_modelMatrix * a_position;" \
		"       vec3 transformedNormal = normalize(mat3(u_viewMatrix * u_modelMatrix) * a_normal);" \
		"       vec3 lightDirection = normalize(vec3(u_lightPosition) - eyeCoordinates.xyz);" \
		"       float tNormalDotLightDirection = max(dot(transformedNormal, lightDirection), 0.0);" \
		"       vec3 ambient = u_la * u_ka;" \
		"       vec3 diffuse = u_ld * u_kd * tNormalDotLightDirection;" \
		"       vec3 reflectionVector = reflect(-lightDirection, transformedNormal);" \
		"       vec3 viewVector = normalize(-eyeCoordinates.xyz);" \
		"       vec3 specular = u_ls * u_ks * pow(max(dot(reflectionVector, viewVector), 0.0), u_materialShininess);" \
		"       phongAdsColor = ambient + diffuse + specular;"
		"   }" \
		"   else" \
		"   {" \
		"       phongAdsColor = vec3(1.0, 1.0, 1.0);" \
		"   }" \
		"\n" \
		"   gl_Position = u_projectionMatrix * u_viewMatrix * u_modelMatrix * a_position;" \
		"}";

	// Creating shader object
	vertexShaderObject_PerVertex = glCreateShader(GL_VERTEX_SHADER);

	// Giving shader code to shader object
	glShaderSource(vertexShaderObject_PerVertex,
		1,
		(const GLchar**)&vertexShaderSourcecode,
		NULL);

	// Compile the shader
	glCompileShader(vertexShaderObject_PerVertex);

	// Error Checking
	GLint status = 0;
	GLint infoLogLen = 0;
	char* log = NULL;

	// a. Getting compilation status
	glGetShaderiv(vertexShaderObject_PerVertex,
		GL_COMPILE_STATUS,
		&status);

	if (status == GL_FALSE)
	{
		// Getting length of log of compilation status
		glGetShaderiv(vertexShaderObject_PerVertex,
			GL_INFO_LOG_LENGTH,
			&infoLogLen);
		if (infoLogLen > 0)
		{
			// Allocate enough memory to buffer to hold the compilation log 
			log = (char*)malloc(infoLogLen);
			if (log != NULL)
			{
				// Get the compilation log into this allocated buffer.
				GLsizei written;
				glGetShaderInfoLog(vertexShaderObject_PerVertex,
					infoLogLen,
					&written,
					log);

				// display the contents of buffer
				fprintf(gpFile, "Vertex Shader vertexShaderObject_PerVertex Compilation Log: %s\n", log);

				// Free the allocated the buffer.
				free(log);

				// exit the application due to error
				[self uninitialize];
            	[self release];
            	[NSApp terminate:self];
			}
		}
	}
}

-(void)InitShaderProgramPerFragment
{
	GLint status = 0;
	GLint infoLogLen = 0;
	char* log = NULL;

	// 1. Create shader program object
	shaderProgramObject_PerFragment = glCreateProgram();

	// 2. attach desire shaders to this shader program object
	glAttachShader(shaderProgramObject_PerFragment, vertexShaderObject_PerFragment);
	glAttachShader(shaderProgramObject_PerFragment, fragmentShaderObject_PerFragment);

	//  Pre-linking binding of shader program object with vertex attributes 
	glBindAttribLocation(shaderProgramObject_PerFragment, AMC_ATTRIBUTE_POSITION, "a_position");

	// Bind the normal attribute location before linking.
	glBindAttribLocation(shaderProgramObject_PerFragment, AMC_ATTRIBUTE_NORMAL, "a_normal");


	// 3. link shader program object
	glLinkProgram(shaderProgramObject_PerFragment);

	// retriving/getting uniformed location from shader program object
	modelMatrixUniform = glGetUniformLocation(shaderProgramObject_PerFragment,
		"u_modelMatrix");

	viewMatrixUniform = glGetUniformLocation(shaderProgramObject_PerFragment,
		"u_viewMatrix");

	projectionMatrixUniform = glGetUniformLocation(shaderProgramObject_PerFragment,
		"u_projectionMatrix");

	laUniform = glGetUniformLocation(shaderProgramObject_PerFragment, "u_la");
	ldUniform = glGetUniformLocation(shaderProgramObject_PerFragment, "u_ld");
	lsUniform = glGetUniformLocation(shaderProgramObject_PerFragment, "u_ls");
	kaUniform = glGetUniformLocation(shaderProgramObject_PerFragment, "u_ka");
	kdUniform = glGetUniformLocation(shaderProgramObject_PerFragment, "u_kd");
	ksUniform = glGetUniformLocation(shaderProgramObject_PerFragment, "u_ks");
	lightPositionUniform = glGetUniformLocation(shaderProgramObject_PerFragment, "u_lightPosition");
	materialShininessUniform = glGetUniformLocation(shaderProgramObject_PerFragment, "u_materialShininess");

	LightingEnabledUniform = glGetUniformLocation(shaderProgramObject_PerFragment, "LightingEnabled");

	// 4. do link error checking with similar a to g steps like above
	status = 0;
	infoLogLen = 0;
	log = NULL;

	// a. Getting link status
	glGetProgramiv(shaderProgramObject_PerFragment,
		GL_LINK_STATUS,
		&status);

	if (status == GL_FALSE)
	{
		// Getting length of log of LINK status
		glGetProgramiv(shaderProgramObject_PerFragment,
			GL_INFO_LOG_LENGTH,
			&infoLogLen);

		if (infoLogLen > 0)
		{
			// // Allocate enough memory to buffer to hold the log 
			log = (char*)malloc(infoLogLen);
			if (log != NULL)
			{
				// Get the log into this allocated buffer.
				GLsizei written;
				glGetProgramInfoLog(shaderProgramObject_PerFragment,
					infoLogLen,
					&written,
					log);

		
				// display the contents of buffer
				fprintf(gpFile, "Shader Program shaderProgramObject_PerFragment Link Log : % s\n", log);

				// Free the allocated the buffer.
				free(log);

				// exit the application due to error
				[self uninitialize];
            	[self release];
            	[NSApp terminate:self];

			}

		}
	}
}

-(void) InitVertexShaderPerFragment
{
	const GLchar* vertexShaderSourcecode =
		"#version 410 core" \
		"\n" \

		"in vec4 a_position;" \
		"in vec3 a_normal;" \

		"uniform mat4 u_modelMatrix;" \
		"uniform mat4 u_viewMatrix;" \
		"uniform mat4 u_projectionMatrix;" \
		"uniform int LightingEnabled;" \
		"uniform vec4 u_lightPosition;" \

		"out vec3 transformedNormal;" \
		"out vec3 lightDirection;" \
		"out vec3 viewVector;" \

		"\n" \
		"void main(void)" \
		"{" \
		"   if(LightingEnabled == 1)" \
		"   {" \
		"       vec4 eyeCoordinates = u_viewMatrix * u_modelMatrix * a_position;" \
		"		mat3 normalMatrix = mat3(u_viewMatrix * u_modelMatrix);" \
		"       transformedNormal = mat3(u_viewMatrix * u_modelMatrix) * a_normal;" \
		"       lightDirection = vec3(u_lightPosition) - eyeCoordinates.xyz;" \
		"       viewVector = -eyeCoordinates.xyz;" \
		"   }" \
		"\n" \
		"   gl_Position = u_projectionMatrix * u_viewMatrix * u_modelMatrix * a_position;" \
		"}";

	// Creating shader object
	 vertexShaderObject_PerFragment = glCreateShader(GL_VERTEX_SHADER);

	// Giving shader code to shader object
	glShaderSource(vertexShaderObject_PerFragment,
		1,
		(const GLchar**)&vertexShaderSourcecode,
		NULL);

	// Compile the shader
	glCompileShader(vertexShaderObject_PerFragment);

	// Error Checking
	GLint status;
	GLint infoLogLen;
	char* log = NULL;

	// a. Getting compilation status
	glGetShaderiv(vertexShaderObject_PerFragment,
		GL_COMPILE_STATUS,
		&status);

	if (status == GL_FALSE)
	{
		// Getting length of log of compilation status
		glGetShaderiv(vertexShaderObject_PerFragment,
			GL_INFO_LOG_LENGTH,
			&infoLogLen);
		if (infoLogLen > 0)
		{
			// Allocate enough memory to buffer to hold the compilation log 
			log = (char*)malloc(infoLogLen);
			if (log != NULL)
			{
				// Get the compilation log into this allocated buffer.
				GLsizei written;
				glGetShaderInfoLog(vertexShaderObject_PerFragment,
					infoLogLen,
					&written,
					log);

				// display the contents of buffer
				fprintf(gpFile, "Vertex Shader vertexShaderObject_PerFragment Compilation Log: %s\n", log);

				// Free the allocated the buffer.
				free(log);

				// exit the application due to error
				[self uninitialize];
            	[self release];
            	[NSApp terminate:self];
			}
		}
	}
}

-(void)InitFragmentShaderPerFragment
{
	GLint status = 0;
	GLint infoLogLen = 0;
	char* log = NULL;

	// Fragment Shader

	// 1. Writing shader code
	const GLchar* fragmentShaderSourcecode =
		"#version 410 core" \
		"\n"\

		"in vec3 transformedNormal;" \
		"in vec3 lightDirection;" \
		"in vec3 viewVector;" \

		"uniform vec3 u_la;" \
		"uniform vec3 u_ld;" \
		"uniform vec3 u_ls;" \
		"uniform vec3 u_ka;" \
		"uniform vec3 u_kd;" \
		"uniform vec3 u_ks;" \
		"uniform float u_materialShininess;" \
		"uniform int LightingEnabled;" \
		"out vec4 fragColor;" \

		"\n" \
		"void main(void)" \
		"{" \
		"vec3 phong_ads_color;" \
		"if(LightingEnabled == 1)" \
		"{" \
		" vec3 ambient = u_la * u_ka;" \
		" vec3 normalized_transformed_normal = normalize(transformedNormal);" \
		"vec3 normized_light_direction = normalize(lightDirection);" \
		"vec3 diffuse = u_ld * u_kd *  max(dot(normized_light_direction, normalized_transformed_normal), 0.0);" \
		"vec3 reflectionVector = reflect(-normized_light_direction, normalized_transformed_normal);" \
		"vec3 normized_viewVector = normalize(viewVector);" \
		"vec3 specular = u_ls * u_ks * pow(max(dot(reflectionVector, normized_viewVector), 0.0), u_materialShininess);" \
		"phong_ads_color = ambient + diffuse + specular;" \

		"}"	\
		"else" \
		"{" \
		"phong_ads_color = vec3(1.0, 1.0, 1.0);" \
		"}" \
		"   fragColor = vec4(phong_ads_color, 1.0);" \
		"}";

	// 2. Creating shader object
	 fragmentShaderObject_PerFragment = glCreateShader(GL_FRAGMENT_SHADER);

	// 3. Giving shader code to shader object
	glShaderSource(fragmentShaderObject_PerFragment,
		1,
		(const GLchar**)&fragmentShaderSourcecode,
		NULL);

	// 4. Compile the shader
	glCompileShader(fragmentShaderObject_PerFragment);

	// 5. Error checking of shader compilation
	status = 0;
	infoLogLen = 0;
	log = NULL;

	// a. Getting compilation status
	glGetShaderiv(fragmentShaderObject_PerFragment,
		GL_COMPILE_STATUS,
		&status);

	if (status == GL_FALSE)
	{
		// Getting length of log of compilation status
		glGetShaderiv(fragmentShaderObject_PerFragment,
			GL_INFO_LOG_LENGTH,
			&infoLogLen);

		if (infoLogLen > 0)
		{
			// Allocate enough memory to buffer to hold the compilation log 
			log = (char*)malloc(infoLogLen);
			if (log != NULL)
			{
				// Get the compilation log into this allocated buffer.
				GLsizei written;
				glGetShaderInfoLog(fragmentShaderObject_PerFragment,
					infoLogLen,
					&written,
					log);

				// display the contents of buffer
				fprintf(gpFile, "Fragment Shader fragmentShaderObject_PerFragment Compilation Log: %s\n", log);

				// Free the allocated the buffer.
				free(log);

				// exit the application due to error
				[self uninitialize];
            	[self release];
            	[NSApp terminate:self];
			}
		}
	}
}

-(void)InitSphere
{
	float sphere_vertices[1146];
    float sphere_normals[1146];
    float sphere_textures[764];
    short sphere_elements[2280];

    Sphere *sphere = [[Sphere alloc] init];
    [sphere getSphereVertexData:sphere_vertices :sphere_normals :sphere_textures :sphere_elements];
    numVertices = [sphere getNumberOfSphereVertices];
    numElements = [sphere getNumberOfSphereElements];
        
        //set up vao and vbo
    glGenVertexArrays(1, &vao_sphere);
    glBindVertexArray(vao_sphere);
    glGenBuffers(1, &vbo_sphere_position);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_sphere_position);

    glBufferData(GL_ARRAY_BUFFER, sizeof(sphere_vertices), sphere_vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(AMC_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(AMC_ATTRIBUTE_POSITION);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glGenBuffers(1, &vbo_sphere_normal);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_sphere_normal);
    glBufferData(GL_ARRAY_BUFFER, sizeof(sphere_normals), sphere_normals, GL_STATIC_DRAW);
    glVertexAttribPointer(AMC_ATTRIBUTE_NORMAL, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(AMC_ATTRIBUTE_NORMAL);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glGenBuffers(1, &vbo_sphere_indices);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_sphere_indices);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(sphere_elements), sphere_elements, GL_STATIC_DRAW);
    glBindVertexArray(0);
        
    [sphere release];	
}

// override 
-(void)reshape
{
    //code
    [super reshape];
    CGLLockContext((CGLContextObj)[[self openGLContext] CGLContextObj]);

    NSRect rect = [self bounds];
    int width = rect.size.width;
    int height = rect.size.height;

    // call our resize
    [self resize:width : height];

    CGLUnlockContext((CGLContextObj)[[self openGLContext] CGLContextObj]);

}

-(void)resize: (int)width : (int)height
{
    // code
    if(height < 0)
    {
        height = 1;
    }
        
    glViewport(0, 0, (GLsizei)width, (GLsizei)height);
    
	perceptivegraphicsProjectionMatrix = vmath::perspective(45.0f, (GLfloat)width / (GLfloat)height, 1.0f, 100.0f);
}

-(void)display
{
    // code 
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (choosenShader == 'v')
	{
		glUseProgram(shaderProgramObject_PerVertex);
	}
	else if(choosenShader == 'f')
	{
		glUseProgram(shaderProgramObject_PerFragment);
	}


	mat4 translationMatrix = mat4::identity();
	mat4 modelMatrix = mat4::identity();
	mat4 viewMatrix = mat4::identity();

	mat4 modelViewProjectionMatrix = mat4::identity();
	mat4 scaleMatrix = mat4::identity();
	mat4 rotationMatrix = mat4::identity();
	mat4 rotationMatrix_x = mat4::identity();
	mat4 rotationMatrix_y = mat4::identity();
	mat4 rotationMatrix_z = mat4::identity();

	translationMatrix = vmath::translate(0.0f, 0.0f, -3.0f);
	//scaleMatrix = vmath::scale(0.75f, 0.75f, 0.75f);
	//rotationMatrix_x = vmath::rotate(angleCube, 1.0f, 0.0f, 0.0f);
	rotationMatrix_y = vmath::rotate(angleSphere, 0.0f, 1.0f, 0.0f);
	//rotationMatrix_z = vmath::rotate(angleCube, 0.0f, 0.0f, 1.0f);
	rotationMatrix = rotationMatrix_x * rotationMatrix_y * rotationMatrix_z;
	modelMatrix = translationMatrix * scaleMatrix * rotationMatrix;	// Order is very important
	modelViewProjectionMatrix = perceptivegraphicsProjectionMatrix * modelMatrix;

	// send above transformation matrixes to shader in respective matrix uniform
	glUniformMatrix4fv(modelMatrixUniform,
		1,
		GL_FALSE,
		modelMatrix);

	glUniformMatrix4fv(viewMatrixUniform,
		1,
		GL_FALSE,
		viewMatrix);

	glUniformMatrix4fv(projectionMatrixUniform,
		1,
		GL_FALSE,
		perceptivegraphicsProjectionMatrix);


	/* Sending LIGHT realed Unifrom */
	if (bLight == true)
	{
		glUniform1i(LightingEnabledUniform, 1);
		glUniform3fv(laUniform, 1, lightAmbient);
		glUniform3fv(ldUniform, 1, lightDiffuse);
		glUniform3fv(lsUniform, 1, lightSpecular);
		glUniform4fv(lightPositionUniform, 1, lightPosition);

		glUniform3fv(kaUniform, 1, materialAmbient);
		glUniform3fv(kdUniform, 1, materialDiffuse);
		glUniform3fv(ksUniform, 1, materialSpecular);
		glUniform1f(materialShininessUniform, materialShininess);

	}
	else
	{
		glUniform1i(LightingEnabledUniform, 0);
	}
	// bind with vertex array object
// *** bind vao ***
	glBindVertexArray(vao_sphere);

	// *** draw, either by glDrawTriangles() or glDrawArrays() or glDrawElements()
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Vbo_sphere_element);
	glDrawElements(GL_TRIANGLES, numElements, GL_UNSIGNED_SHORT, 0);

	// *** unbind vao ***
	glBindVertexArray(0);




	glUseProgram(0);
} 

-(void)myUpdate
{
    // code 
    angleSphere += 0.02f;
	if (angleSphere >= 360.f)
	{
		angleSphere = angleSphere - 360.0f;
	}
}

-(void)uninitialize
{
    // code 
     if(vbo_sphere_indices)
    {
       glDeleteBuffers(1, &vbo_sphere_indices); 
	    vbo_sphere_indices = 0;
    }


    if(vbo_sphere_normal)
    {
    	glDeleteBuffers(1, &vbo_sphere_normal);
        vbo_sphere_normal = 0;
    }

    if(vbo_sphere_position)
    {
        glDeleteBuffers(1, &vbo_sphere_position);
        vbo_sphere_position = 0;
    }
    
    if(vao_sphere)
    {
        glDeleteVertexArrays(1, &vao_sphere);
        vao_sphere = 0;
    }
	// shader uninitialise
	if (shaderProgramObject)
	{
		// 0. again used sharderprogramobject
		glUseProgram(shaderProgramObject);

		// 1. Get Number of attach shaders
		GLsizei numAttchedShaders;
		glGetProgramiv(shaderProgramObject,
			GL_ATTACHED_SHADERS,
			&numAttchedShaders);

		// 2. Create empty buffer to hold array of attach shader objects
		GLuint* shaderObjects = NULL;
		// 3. allocate enough memroy to this according to number of attach shaders and fill it with attachedshaderobject
		shaderObjects = (GLuint*)malloc(numAttchedShaders*sizeof(GLuint));

		glGetAttachedShaders(shaderProgramObject,
			numAttchedShaders,
		(GLsizei*)	&shaderProgramObject,
			shaderObjects);

		// 4. As number of attach shaders more than 1 start a loop and inside loop deattach, delete shader one by one
		for (GLsizei i = 0; i < numAttchedShaders; i++)
		{
			glDetachShader(shaderProgramObject, shaderObjects[i]);
			glDeleteShader(shaderObjects[i]);
			shaderObjects[i] = 0;
		}

		// 5. free memory allocated for buffer
		free(shaderObjects);
		shaderObjects = NULL;

		// 6. unused sharderprogramobject
		glUseProgram(0);

		// 7. delete sharderprogramobject
		glDeleteProgram(shaderProgramObject);
		shaderProgramObject = 0;
	
	}

}

-(BOOL)acceptsFirstResponder
{
    //code
    [[self window] makeFirstResponder:self];
        
    return (YES);
}


-(void)keyDown:(NSEvent*)event
{
    //code
    int key = [[event characters] characterAtIndex:0];
    switch(key)
    {	
		// escape
        case 27:
			[[self window] toggleFullScreen:self];
            break;
        
		case 'Q':
        case 'q':
			[self uninitialize];
            [self release];
            [NSApp terminate:self];
            break;

        case 'F':
        case 'f':
			choosenShader = 'f';
            break;
		case 'V':
        case 'v':
			choosenShader = 'v';
            break;

        case 'L':
		case 'l':
			if (bLight == false)
			{
				bLight = true;
			}
			else
			{
				bLight = false;
			}
			break;
        default:
        break;
    }
}

-(void)mouseDown:(NSEvent*)event
{
    // code

}

-(void)dealloc
{
    [super dealloc];
    
    if(displayLink)
    {
        CVDisplayLinkStop(displayLink);
        CVDisplayLinkRelease(displayLink);
        displayLink = nil;
    }
}

@end

// Implement MyDisplayLinkCallback Callback function
CVReturn MyDisplayLinkCallback(CVDisplayLinkRef displayLink, const CVTimeStamp *currentTime, const CVTimeStamp *outputTime,
                               CVOptionFlags flagsIn, CVOptionFlags *flagsOut, void *view)
{
    //code
    CVReturn result = [(GLView*)view getFrameForTime:outputTime];
    
    return (result);
}
