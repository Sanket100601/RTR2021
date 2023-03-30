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

    

	GLuint VOA_pyramid;
	GLuint VBO_pyramid_position;
    GLuint VBO_normal;


	GLfloat anglePyramid;
   	GLuint projectionMatrixUniform;
	GLuint modelMatrixUniform;
	GLuint viewMatrixUniform;

	/* LIGHTS */
	GLuint laUniform[2];
	GLuint ldUniform[2];
	GLuint lsUniform[2];
	GLuint kaUniform;
	GLuint kdUniform;
	GLuint ksUniform;
	GLuint materialShininessUniform;
	GLuint lightPositionUniform[2];
	GLuint LightingEnabledUniform;
	struct Light
	{
		vec4 lightAmbient;
		vec4 lightDiffuse;
		vec4 lightSpecular;
		vec4 lightPosition;
	};
	Light lights[2];


	mat4 perceptivegraphicsProjectionMatrix;
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
		"\n" \
		"out vec3 phongAdsColor;" \
		"\n" \
		"uniform mat4 u_modelMatrix;" \
		"uniform mat4 u_viewMatrix;" \
		"uniform mat4 u_projectionMatrix;" \
		"uniform int LightingEnabled;" \
		"uniform vec3 u_la[2];" \
		"uniform vec3 u_ld[2];" \
		"uniform vec3 u_ls[2];" \
		"uniform vec3 u_ka;" \
		"uniform vec3 u_kd;" \
		"uniform vec3 u_ks;" \
		"uniform vec4 u_lightPosition[2];" \
		"uniform float u_materialShininess;" \
		"\n" \
		"void main(void)" \
		"{" \
		"   if(LightingEnabled == 1)" \
		"   {" \
		"       vec4 iCoordinates = u_viewMatrix * u_modelMatrix * a_position;" \
		"       vec3 transformedNormal = normalize(mat3(u_viewMatrix * u_modelMatrix) * a_normal);" \
		"       vec3 viewVector = normalize(-iCoordinates.xyz);" \

		"		vec3 ambient[2];" \
		"		vec3 lightDirection[2];" \
		"		vec3 diffuse[2];" \
		"		vec3 reflectionVector[2];"
		"		vec3 specular[2];" \

		"		for(int i=0; i<2; i++)" \
		"		{" \

		"			ambient[i] = u_la[i] * u_ka;" \
		"			lightDirection[i] = normalize(vec3(u_lightPosition[i]) - iCoordinates.xyz); " \
		"			diffuse[i] = u_ld[i] * u_kd * max(dot(transformedNormal, lightDirection[i]), 0.0);" \
		"			reflectionVector[i] = reflect(-lightDirection[i], transformedNormal);" \
		"			specular[i] = u_ls[i] * u_ks * pow(max(dot(reflectionVector[i], viewVector), 0.0), u_materialShininess);" \
		"			phongAdsColor = phongAdsColor + ambient[i] + diffuse[i] + specular[i];" \
		
		"		}" \
		"   }" \
		"   else" \
		"   {" \
		"       phongAdsColor = vec3(1.0, 1.0, 1.0);" \
		"   }" \
		"\n" \
		"   gl_Position = u_projectionMatrix * u_viewMatrix * u_modelMatrix * a_position;" \
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
		"in vec3 phongAdsColor;" \
		"\n" \
		"out vec4 fragmentColor;" \
		"\n" \
		"void main(void)" \
		"{" \
		"   fragmentColor = vec4(phongAdsColor, 1.0);" \
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

	// 3. link shader program object
	glLinkProgram(shaderProgramObject);

	// retriving/getting uniformed location from shader program object
	modelMatrixUniform = glGetUniformLocation(shaderProgramObject,
		"u_modelMatrix");

	viewMatrixUniform = glGetUniformLocation(shaderProgramObject,
		"u_viewMatrix");

	projectionMatrixUniform = glGetUniformLocation(shaderProgramObject,
		"u_projectionMatrix");

	laUniform[0] = glGetUniformLocation(shaderProgramObject, "u_la[0]");
	ldUniform[0] = glGetUniformLocation(shaderProgramObject, "u_ld[0]");
	lsUniform[0] = glGetUniformLocation(shaderProgramObject, "u_ls[0]");
	lightPositionUniform[0] = glGetUniformLocation(shaderProgramObject, "u_lightPosition[0]");

	laUniform[1] = glGetUniformLocation(shaderProgramObject, "u_la[1]");
	ldUniform[1] = glGetUniformLocation(shaderProgramObject, "u_ld[1]");
	lsUniform[1] = glGetUniformLocation(shaderProgramObject, "u_ls[1]");
	lightPositionUniform[1] = glGetUniformLocation(shaderProgramObject, "u_lightPosition[1]");

	kaUniform = glGetUniformLocation(shaderProgramObject, "u_ka");
	kdUniform = glGetUniformLocation(shaderProgramObject, "u_kd");
	ksUniform = glGetUniformLocation(shaderProgramObject, "u_ks");
	
	materialShininessUniform = glGetUniformLocation(shaderProgramObject, "u_materialShininess");

	LightingEnabledUniform = glGetUniformLocation(shaderProgramObject, "LightingEnabled");

	

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
	
	
	
	const GLfloat pyramidPosition[] = {
				// front
				0.0f, 1.0f, 0.0f, 
				-1.0f, -1.0f, 1.0f,
				1.0f, -1.0f, 1.0f,

				// right
				0.0f, 1.0f, 0.0f, 
				1.0f, -1.0f, 1.0f,
				1.0f, -1.0f, -1.0f,

				// back
				0.0f, 1.0f, 0.0f,
				1.0f, -1.0f, -1.0f, 
				-1.0f, -1.0f, -1.0f,
				
				// left
				0.0f, 1.0f, 0.0f, 
				-1.0f, -1.0f, -1.0f,
				-1.0f, -1.0f, 1.0f,
	};

	



	 /* PYRAMID */
	glGenVertexArrays(1,
		&VOA_pyramid);

	// create vertex array object
	glBindVertexArray(VOA_pyramid);
	// create vertex data buffer - VBO for position
	glGenBuffers(1, &VBO_pyramid_position);

	// bind with vertex data buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO_pyramid_position);

	// create storage of buffer data for perticular target
	glBufferData(GL_ARRAY_BUFFER,
		sizeof(pyramidPosition),
		pyramidPosition,
		GL_STATIC_DRAW);

	// specifiy where, how much, which buffer data to be consider as vertex data arrays
	glVertexAttribPointer(AMC_ATTRIBUTE_POSITION,
		3,
		GL_FLOAT,
		GL_FALSE,
		0,
		NULL);

	// e. enable respetive buffer binding vertex array
	glEnableVertexAttribArray(AMC_ATTRIBUTE_POSITION);

	// f. unbind with respective vertex data buffer
	glBindBuffer(GL_ARRAY_BUFFER, 0);


	const float pyramidNormals[] = {
		0.0f, 0.447214f, 0.894427f,// front-top
0.0f, 0.447214f, 0.894427f,// front-left
0.0f, 0.447214f, 0.894427f,// front-right
0.894427f, 0.447214f, 0.0f, // right-top
0.894427f, 0.447214f, 0.0f, // right-left
0.894427f, 0.447214f, 0.0f, // right-right
0.0f, 0.447214f, -0.894427f, // back-top
0.0f, 0.447214f, -0.894427f, // back-left
0.0f, 0.447214f, -0.894427f, // back-right 
-0.894427f, 0.447214f, 0.0f, // left-top
-0.894427f, 0.447214f, 0.0f, // left-left 
-0.894427f, 0.447214f, 0.0f // left-right *************************************************************************************************************

	};
	// create vertex data buffer - VBO for position
	glGenBuffers(1, &VBO_normal);

	// bind with vertex data buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO_normal);

	// create storage of buffer data for perticular target
	glBufferData(GL_ARRAY_BUFFER,
		sizeof(pyramidNormals),
		pyramidNormals,
		GL_STATIC_DRAW);

	// specifiy where, how much, which buffer data to be consider as vertex data arrays
	glVertexAttribPointer(AMC_ATTRIBUTE_NORMAL,
		3,
		GL_FLOAT,
		GL_FALSE,
		0,
		NULL);

	// e. enable respetive buffer binding vertex array
	glEnableVertexAttribArray(AMC_ATTRIBUTE_NORMAL);

	// f. unbind with respective vertex data buffer
	glBindBuffer(GL_ARRAY_BUFFER, 0);



	glBindVertexArray(0);

	
	lights[0].lightAmbient = vmath::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	lights[0].lightDiffuse = vmath::vec4(1.0f, 0.0f, 0.0f, 1.0f);	// RED
	lights[0].lightSpecular = vmath::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	lights[0].lightPosition = vmath::vec4(-2.0f, 0.0f, 0.0f, 1.0f);

	lights[1].lightAmbient = vmath::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	lights[1].lightDiffuse = vmath::vec4(0.0f, 0.0f, 1.0f, 1.0f);	// Blue
	lights[1].lightSpecular = vmath::vec4(0.0f, 0.0f, 1.0f, 1.0f);
	lights[1].lightPosition = vmath::vec4(2.0f, 0.0f, 0.0f, 1.0f);
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
	mat4 rotationMatrix = mat4::identity();
	mat4 modelMatrix = mat4::identity();
	mat4 viewMatrix = mat4::identity();
	mat4 modelViewProjectionMatrix = mat4::identity();


	translationMatrix = vmath::translate(0.0f, 0.0f, -6.0f);
	rotationMatrix = vmath::rotate(anglePyramid, 0.0f, 1.0f, 0.0f);

	modelMatrix = translationMatrix * rotationMatrix;	// Order is very important 
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


	// bind with vertex array object
	if (bLight == true)
	{
		for (int i = 0; i < 2; i++)
		{
			glUniform1i(LightingEnabledUniform, 1);
			glUniform3fv(laUniform[i], 1, lights[i].lightAmbient);
			glUniform3fv(ldUniform[i], 1, lights[i].lightDiffuse);
			glUniform3fv(lsUniform[i], 1, lights[i].lightSpecular);
			glUniform4fv(lightPositionUniform[i], 1, lights[i].lightPosition);


		}
		glUniform3fv(kaUniform, 1, materialAmbient);
		glUniform3fv(kdUniform, 1, materialDiffuse);
		glUniform3fv(ksUniform, 1, materialSpecular);
		glUniform1f(materialShininessUniform, materialShininess);

	}
	else
	{
		glUniform1i(LightingEnabledUniform, 0);
	}

	glBindVertexArray(VOA_pyramid);
	
	// draw
	glDrawArrays(GL_TRIANGLES,
		0,
		12);

	glBindVertexArray(0);

	// 3. Unused ShaderProgranObject
	glUseProgram(0);

	[self myUpdate];
} 

-(void)myUpdate
{
    // code 
	anglePyramid += 0.5f;
	if (anglePyramid >= 360.f)
	{
		anglePyramid = anglePyramid - 360.0f;
	}
}

-(void)uninitialize
{
    // code 
   

	if (VBO_pyramid_position)
	{
		glDeleteBuffers(1, &VBO_pyramid_position);
		VBO_pyramid_position = 0;
	}



	if (VOA_pyramid)
	{
		glDeleteVertexArrays(1, &VOA_pyramid);
		VOA_pyramid = 0;
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