// sh build.sh
// Header files
#import <foundation/Foundation.h>
#import <cocoa/cocoa.h>
// OpenGL
#import <QuartzCore/CVDisplayLink.h>
#import <OpenGL/gl3.h>
#import <OpenGL/gl3ext.h>
#import "vmath.h"

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

    

	GLuint VOA_cube;
	GLuint VBO;
	GLuint VBO_cube_position;
	GLuint VBO_normal;
	GLfloat angleCube;
    GLuint projectionMatrixUniform;
	GLuint modelMatrixUniform;
	GLuint viewMatrixUniform;
	GLuint texture_marble;
	GLuint textureSamplerUniform;


	mat4 perceptivegraphicsProjectionMatrix;
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

    // do double buffering
    CGLFlushDrawable((CGLContextObj)[[self openGLContext] CGLContextObj]);

    CGLUnlockContext((CGLContextObj)[[self openGLContext] CGLContextObj]);
}

-(int)initialize
{
    // code// Vertex Shader code
	const GLchar* vertexShaderSourcecode =
		"#version 410 core" \
		"\n" \
		"in vec4 a_position;" \
		"in vec3 a_normal;" \
		"in vec2 a_texcoord;" \
		"in vec4 a_color;" \
		"uniform mat4 u_modelMatrix;" \
		"uniform mat4 u_viewMatrix;" \
		"uniform mat4 u_projectionMatrix;" \
		"uniform int LightingEnabled;" \
		"uniform vec4 u_lightPosition;" \

		"out vec3 transformedNormal;" \
		"out vec3 lightDirection;" \
		"out vec3 viewVector;" \
		"out vec2 a_texcoord_out;" \
		"out vec4 a_color_out;" \
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
		"   gl_Position = u_projectionMatrix * u_viewMatrix * u_modelMatrix * a_position;" \
		"	a_color_out = a_color;" \
		"	a_texcoord_out = a_texcoord;" \
		"}";


	// Creating shader object
	GLuint vertexShaderObject = glCreateShader(GL_VERTEX_SHADER);

	// Giving shader code to shader object
	glShaderSource(vertexShaderObject,
		1,
		(const GLchar**)&vertexShaderSourcecode,
		NULL);

	// Compile the shader
	glCompileShader(vertexShaderObject);

	// Error Checking
	GLint status;
	GLint infoLogLen;
	char* log = NULL;

	// a. Getting compilation status
	glGetShaderiv(vertexShaderObject,
		GL_COMPILE_STATUS,
		&status);

	if (status == GL_FALSE)
	{
		// Getting length of log of compilation status
		glGetShaderiv(vertexShaderObject,
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
				glGetShaderInfoLog(vertexShaderObject,
					infoLogLen,
					&written,
					log);

				// display the contents of buffer
				fprintf(gpFile, "Vertex Shader Compilation Log: %s\n", log);

				// Free the allocated the buffer.
				free(log);

				// exit the application due to error
				[self uninitialize];
                [self release];
                [NSApp terminate:self];
			}
		}
	}
	

											

	// Fragment Shader
	
	// 1. Writing shader code
	const GLchar* fragmentShaderSourcecode = 
		"#version 410 core" \
		"\n" \
		"in vec4 a_color_out;" \
		"in vec2 a_texcoord_out;" \
		"uniform sampler2D u_textureSampler;" \
		"out vec4 FragColor;" \
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
		"vec3 phong_ads_color;" \
		"out vec4 fragColor;" \
		"void main(void)" \
		"{" \
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
		"   fragColor = vec4(phong_ads_color, 1.0)*texture(u_textureSampler, a_texcoord_out)*a_color_out;" \
		
		"}";

	// 2. Creating shader object
	GLuint fragmentShaderObject = glCreateShader(GL_FRAGMENT_SHADER);

	// 3. Giving shader code to shader object
	glShaderSource(fragmentShaderObject,
		1,
		(const GLchar**)&fragmentShaderSourcecode,
		NULL);

	// 4. Compile the shader
	glCompileShader(fragmentShaderObject);

	// 5. Error checking of shader compilation
	status = 0;
	infoLogLen = 0;
	log = NULL;

	// a. Getting compilation status
	glGetShaderiv(fragmentShaderObject,
		GL_COMPILE_STATUS,
		&status);

	if (status == GL_FALSE)
	{
		// Getting length of log of compilation status
		glGetShaderiv(fragmentShaderObject,
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
				glGetShaderInfoLog(fragmentShaderObject,
					infoLogLen,
					&written,
					log);

				// display the contents of buffer
				fprintf(gpFile, "Fragment Shader Compilation Log: %s\n", log);

				// Free the allocated the buffer.
				free(log);

				// exit the application due to error
				[self uninitialize];
                [self release];
                [NSApp terminate:self];
			}
		}
	}

	// Creating, Linking, error checking of shader program
	// 1. Create shader program object
	shaderProgramObject = glCreateProgram();

	// 2. attach desire shaders to this shader program object
	glAttachShader(shaderProgramObject, vertexShaderObject);
	glAttachShader(shaderProgramObject, fragmentShaderObject);

	//  Pre-linking binding of shader program object with vertex attributes 
	glBindAttribLocation(shaderProgramObject,
		AMC_ATTRIBUTE_POSITION,
		"a_position");

	glBindAttribLocation(shaderProgramObject,
		AMC_ATTRIBUTE_NORMAL,
		"a_normal");
	
	glBindAttribLocation(shaderProgramObject,
		AMC_ATTRIBUTE_COLOR,
		"a_color");

	glBindAttribLocation(shaderProgramObject,
		AMC_ATTRIBUTE_TEXTURE0,
		"a_texcoord");

	// 3. link shader program object
	glLinkProgram(shaderProgramObject);

	// retriving/getting uniformed location from shader program object
	modelMatrixUniform = glGetUniformLocation(shaderProgramObject,
		"u_modelMatrix");

	viewMatrixUniform = glGetUniformLocation(shaderProgramObject,
		"u_viewMatrix");

	projectionMatrixUniform = glGetUniformLocation(shaderProgramObject,
		"u_projectionMatrix");
	laUniform = glGetUniformLocation(shaderProgramObject, "u_la");
	ldUniform = glGetUniformLocation(shaderProgramObject, "u_ld");
	lsUniform = glGetUniformLocation(shaderProgramObject, "u_ls");
	kaUniform = glGetUniformLocation(shaderProgramObject, "u_ka");
	kdUniform = glGetUniformLocation(shaderProgramObject, "u_kd");
	ksUniform = glGetUniformLocation(shaderProgramObject, "u_ks");
	lightPositionUniform = glGetUniformLocation(shaderProgramObject, "u_lightPosition");
	materialShininessUniform = glGetUniformLocation(shaderProgramObject, "u_materialShininess");

	LightingEnabledUniform = glGetUniformLocation(shaderProgramObject, "LightingEnabled");
	textureSamplerUniform = glGetUniformLocation(shaderProgramObject,  "u_textureSampler");

	

	// 4. do link error checking with similar a to g steps like above
	status = 0;
	infoLogLen = 0;
	log = NULL;

	// a. Getting link status
	glGetProgramiv(shaderProgramObject,
		GL_LINK_STATUS,
		&status);

	if (status == GL_FALSE)
	{
		// Getting length of log of LINK status
		glGetProgramiv(shaderProgramObject,
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
				glGetProgramInfoLog(shaderProgramObject,
					infoLogLen,
					&written,
					log);

				// display the contents of buffer
				fprintf(gpFile, "Shader Program Link Log : % s\n", log);

				// Free the allocated the buffer.
				free(log);

				// exit the application due to error
				[self uninitialize];
                [self release];
                [NSApp terminate:self];

			}

		}
	}


	// VOA AND VBA Array related lines
	
	
	
	
		const GLfloat cubepcnt[] =
	{
		//front             //color-red         //normal-front      //texture-front
		1.0f, 1.0f, 1.0f,   1.0f, 0.0f, 0.0f,   0.0f, 0.0f, 1.0f,   1.0f, 0.0f,
		-1.0f, 1.0f, 1.0f,  1.0f, 0.0f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,
		-1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 1.0f,
		1.0f, -1.0f, 1.0f,  1.0f, 0.0f, 0.0f,   0.0f, 0.0f, 1.0f,   1.0f, 1.0f,

		//right             //color-green       //normal-right      //texture-right
		1.0f, 1.0f, -1.0f,  0.0f, 1.0f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 0.0f,
		1.0f, 1.0f, 1.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, 0.0f,   0.0f, 0.0f,
		1.0f, -1.0f, 1.0f,  0.0f, 1.0f, 0.0f,   1.0f, 0.0f, 0.0f,   0.0f, 1.0f,
		1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,

		//back              //color-blue        //normal-back       //texture-back
		-1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f,   0.0f, 0.0f, -1.0f,  1.0f, 0.0f,
		1.0f, 1.0f, -1.0f,  0.0f, 0.0f, 1.0f,   0.0f, 0.0f, -1.0f,  0.0f, 0.0f,
		1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 1.0f,   0.0f, 0.0f, -1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f, -1.0f,0.0f, 0.0f, 1.0f,   0.0f, 0.0f, -1.0f,  1.0f, 1.0f,

		//left              //color-cyan        //normal-left       //texture-back
		-1.0f, 1.0f, 1.0f,  0.0f, 1.0f, 1.0f,   -1.0f, 0.0f, 0.0f,  1.0f, 0.0f,
		-1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 1.0f,   -1.0f, 0.0f, 0.0f,  0.0f, 0.0f,
		-1.0f, -1.0f, -1.0f,0.0f, 1.0f, 1.0f,   -1.0f, 0.0f, 0.0f,  0.0f, 1.0f,
		-1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 1.0f,   -1.0f, 0.0f, 0.0f,  1.0f, 1.0f,

		//top               //color-magenta     //normal-top        //texture-top
		1.0f, 1.0f, -1.0f,  1.0f, 0.0f, 1.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,
		-1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 1.0f,   0.0f, 1.0f, 0.0f,   0.0f, 0.0f,
		-1.0f, 1.0f, 1.0f,  1.0f, 0.0f, 1.0f,   0.0f, 1.0f, 0.0f,   0.0f, 1.0f,
		1.0f, 1.0f, 1.0f,   1.0f, 0.0f, 1.0f,   0.0f, 1.0f, 0.0f,   1.0f, 1.0f,

		//bottom            //color-yellow      //normal-bottom     //texture-bottom
		1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 0.0f,   0.0f, -1.0f, 0.0f,  1.0f, 0.0f,
		-1.0f, -1.0f, -1.0f,1.0f, 1.0f, 0.0f,   0.0f, -1.0f, 0.0f,  0.0f, 0.0f,
		-1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 0.0f,   0.0f, -1.0f, 0.0f,  0.0f, 1.0f,
		1.0f, -1.0f, 1.0f,  1.0f, 1.0f, 0.0f,   0.0f, -1.0f, 0.0f,  1.0f, 1.0f
	};
		

	



	glGenVertexArrays(1,
		&VOA_cube);

	// create vertex array object
	glBindVertexArray(VOA_cube);
	
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER,
		sizeof(cubepcnt),
			cubepcnt,
			GL_STATIC_DRAW);
	/* sizeof cube_pcnt is nothing but 11*24*sizeof(float) */
	

	/* Position */
	glVertexAttribPointer(AMC_ATTRIBUTE_POSITION,
		3,
		GL_FLOAT,
		GL_FALSE,
		11 * sizeof(GL_FLOAT),
		(void*)(0));

	glEnableVertexAttribArray(AMC_ATTRIBUTE_POSITION);


	/* Color */
	glVertexAttribPointer(AMC_ATTRIBUTE_COLOR,
		3,
		GL_FLOAT,
		GL_FALSE,
		11 * sizeof(GL_FLOAT),
		(void*)(3* sizeof(GL_FLOAT)));

	glEnableVertexAttribArray(AMC_ATTRIBUTE_COLOR);

	/* Normal */
	glVertexAttribPointer(AMC_ATTRIBUTE_NORMAL,
		3,
		GL_FLOAT,
		GL_FALSE,
		11 * sizeof(GL_FLOAT),
		(void*)(6 * sizeof(GL_FLOAT)));

	glEnableVertexAttribArray(AMC_ATTRIBUTE_NORMAL);


	/* Texture */
	glVertexAttribPointer(AMC_ATTRIBUTE_TEXTURE0,
		2,
		GL_FLOAT,
		GL_FALSE,
		11 * sizeof(GL_FLOAT),
		(void*)(9 * sizeof(GL_FLOAT)));

	glEnableVertexAttribArray(AMC_ATTRIBUTE_TEXTURE0);




	glBindVertexArray(0);

	texture_marble = [self LoadGlTexture : "marble.bmp"];
	if(!texture_marble)
    {
        fprintf(gpFile, "Error : failed to load marble.bmp texture.\n");
		[self uninitialize];
        [self release];
        [NSApp terminate:self];
    }
	glEnable(GL_TEXTURE_2D);
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


-(GLuint)LoadGlTexture:(const char *)textureFilename
{
	// code
	NSBundle *appBundle = [NSBundle mainBundle];
    NSString *appDirPath = [appBundle bundlePath];
    NSString *parentDirPath = [appDirPath stringByDeletingLastPathComponent];
    NSString *textureFileNameWithPath = [NSString stringWithFormat:@"%@/%s", parentDirPath, textureFilename];
    const char* psztextureFileNameWithPath = [textureFileNameWithPath cStringUsingEncoding:NSASCIIStringEncoding];

	NSImage *nsImage = [[NSImage alloc] initWithContentsOfFile:textureFileNameWithPath];
	if(!nsImage)  
	{
		return (0);
	}  
            
	CGImageRef cgImageRef = [nsImage CGImageForProposedRect:nil context:nil hints:nil]; 


	int width = (int)CGImageGetWidth(cgImageRef);
	int height = (int)CGImageGetHeight(cgImageRef);

	
	CGDataProviderRef cgDataProvider = CGImageGetDataProvider(cgImageRef);

	CFDataRef imageData = CGDataProviderCopyData(cgDataProvider);

	void* pixels = (void*)CFDataGetBytePtr(imageData);

	GLuint texture = 0;
	glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
    glGenerateMipmap(GL_TEXTURE_2D);

    CFRelease(imageData);
    pixels = NULL;

    return (texture);

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

    glUseProgram(shaderProgramObject);

	mat4 translationMatrix = mat4::identity();
	mat4 modelMatrix = mat4::identity();
	mat4 viewMatrix = mat4::identity();

	mat4 modelViewProjectionMatrix = mat4::identity();
	mat4 scaleMatrix = mat4::identity();
	mat4 rotationMatrix = mat4::identity();
	mat4 rotationMatrix_x = mat4::identity();
	mat4 rotationMatrix_y = mat4::identity();
	mat4 rotationMatrix_z = mat4::identity();

	translationMatrix = vmath::translate(0.0f, 0.0f, -6.0f);
	//scaleMatrix = vmath::scale(0.75f, 0.75f, 0.75f);
	rotationMatrix_x = vmath::rotate(angleCube, 1.0f, 0.0f, 0.0f);
	rotationMatrix_y = vmath::rotate(angleCube, 0.0f, 1.0f, 0.0f);
	rotationMatrix_z = vmath::rotate(angleCube, 0.0f, 0.0f, 1.0f);
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
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_marble);
	glUniform1i(textureSamplerUniform, 0);

	// bind with vertex array object
	glBindVertexArray(VOA_cube);

	// draw
	glDrawArrays(GL_TRIANGLE_FAN,
		0,
		4);
	glDrawArrays(GL_TRIANGLE_FAN,
		4,
		4);
	glDrawArrays(GL_TRIANGLE_FAN,
		8,
		4);
	glDrawArrays(GL_TRIANGLE_FAN,
		12,
		4);
	glDrawArrays(GL_TRIANGLE_FAN,
		16,
		4);
	glDrawArrays(GL_TRIANGLE_FAN,
		20,
		4);

	glBindVertexArray(0);


	// 3. Unused ShaderProgranObject
	glUseProgram(0);

	[self myUpdate];
} 

-(void)myUpdate
{
    // code 
	angleCube += 0.5f;
	if (angleCube >= 360.f)
	{
		angleCube = angleCube - 360.0f;
	}
}

-(void)uninitialize
{
    // code 
	if (VBO)
	{
		glDeleteBuffers(1, &VBO);
		VBO = 0;
	}

	if (VBO_cube_position)
	{
		glDeleteBuffers(1, &VBO_cube_position);
		VBO_cube_position = 0;
	}

	

	if (VOA_cube)
	{
		glDeleteVertexArrays(1, &VOA_cube);
		VOA_cube = 0;
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
        case 27:
            [self uninitialize];
            [self release];
            [NSApp terminate:self];
            break;
        
        case 'F':
        case 'f':
            [[self window] toggleFullScreen:self];
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
