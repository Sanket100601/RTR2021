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



#define MAX 10




// Global Function Declaration
CVReturn MyDisplayLinkCallback(CVDisplayLinkRef, const CVTimeStamp*, const CVTimeStamp*,
                               CVOptionFlags, CVOptionFlags*, void*);

bool pushMatrix(mat4 matrix);
bool matrixFull();
bool matrixEmpty();
bool popMatrix();

// Global varaible declarations
FILE *gpFile = NULL;
float angleEarth = 0.0f;
float angleMoon = 0.0f;
int year = 0;
int day = 0;
mat4 matrixStack[MAX];
int top = -1;


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
    struct Stack
    {
        vmath::mat4 matrix;
        struct Stack* next;
    };

    struct Stack* stack;
    enum
    {
        AMC_ATTRIBUTE_POSITION = 0,
        AMC_ATTRIBUTE_COLOR,
        AMC_ATTRIBUTE_NORMAL,
        AMC_ATTRIBUTE_TEXTURE0
    };

    GLuint VOA;
    GLuint VBO;
    GLuint mvpMatrixUniform;
	GLuint vao_sphere;
	GLuint vbo_sphere_position;
	GLuint vbo_sphere_normal;
	GLuint vbo_sphere_indices;
	int numVertices;
	int numElements;

    GLuint projectionMatrixUniform;
    GLuint modelMatrixUniform;
    GLuint viewMatrixUniform;

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
		"in vec4 a_color;" \
		"out vec4 a_color_out;" \
		"uniform mat4 u_modelMatrix;" \
		"uniform mat4 u_viewMatrix;" \
		"uniform mat4 u_projectionMatrix;" \
		"void main(void)" \
		"{" \
		"	gl_Position = u_projectionMatrix * u_viewMatrix  * u_modelMatrix * a_position;" \
		"a_color_out = a_color;" \
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
	    "out vec4 FragColor;" \
		"in vec4 a_color_out;" \
		"void main(void)" \
		"{" \
		"FragColor=a_color_out;" \
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
    lBindAttribLocation(shaderProgramObject,
		AMC_ATTRIBUTE_COLOR,
		"a_color");

	// 3. link shader program object
	glLinkProgram(shaderProgramObject);

	// retriving/getting uniformed location from shader program object
	modelMatrixUniform = glGetUniformLocation(shaderProgramObject,
		"u_modelMatrix");


	viewMatrixUniform = glGetUniformLocation(shaderProgramObject,
		"u_viewMatrix");

	projectionMatrixUniform = glGetUniformLocation(shaderProgramObject,
		"u_projectionMatrix");

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
        stack = create_stack();
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
	mat4 modelMatrix = mat4::identity();
	mat4 viewMatrix = mat4::identity();
	
	mat4 scaleMatrix = mat4::identity();
	mat4 rotationMatrix = mat4::identity();
	mat4 modelViewMatrix = mat4::identity();
	mat4 modelViewProjectionMatrix = mat4::identity();


   /* @@@@@@@@@@@@@@@@@@@@@@@@@@@@ SUN @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
	translationMatrix = vmath::translate(0.0f, 0.0f, -10.0f);
	modelMatrix = modelMatrix * translationMatrix;

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

    glBindVertexArray(vao_sphere);

	glVertexAttrib3f(AMC_ATTRIBUTE_COLOR, 1.0f, 1.0f, 0.0f);
	glDrawElements(GL_TRIANGLES, numElements, GL_UNSIGNED_SHORT, 0);

	// *** unbind vao ***
	glBindVertexArray(0);
    pushMatrix(modelMatrix);

    /* @@@@@@@@@@@@@@@@@@@@@@@@@@@@ EARTH @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
	translationMatrix = vmath::translate(3.0f, 0.0f, 0.0f);
	scaleMatrix = vmath::scale(0.75f, 0.75f, 0.75f);
	rotationMatrix = vmath::rotate(angleEarth, 0.0f, 1.0f, 0.0f);
	modelMatrix = modelMatrix * scaleMatrix * rotationMatrix *  translationMatrix;
	
	glUniformMatrix4fv(modelMatrixUniform,
		1,
		GL_FALSE,
		modelMatrix);

    glBindVertexArray(vao_sphere);

	glVertexAttrib3f(AMC_ATTRIBUTE_COLOR, 1.0f, 1.0f, 0.0f);
	glDrawElements(GL_TRIANGLES, numElements, GL_UNSIGNED_SHORT, 0);

	// *** unbind vao ***
	glBindVertexArray(0);

	pushMatrix(modelMatrix);

    /* @@@@@@@@@@@@@@@@@@@@@@@@@@@@ MOON @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
	translationMatrix = vmath::translate(3.5f, 0.0f, 0.0f);
	scaleMatrix = vmath::scale(0.35f, 0.35f, 0.35f);
	rotationMatrix = vmath::rotate(angleMoon, 0.0f, 1.0f, 0.0f);
	modelMatrix = modelMatrix * scaleMatrix * rotationMatrix * translationMatrix;
	

	glUniformMatrix4fv(modelMatrixUniform,
		1,
		GL_FALSE,
		modelMatrix);

    glBindVertexArray(vao_sphere);

	glVertexAttrib3f(AMC_ATTRIBUTE_COLOR, 1.0f, 1.0f, 0.0f);
	glDrawElements(GL_TRIANGLES, numElements, GL_UNSIGNED_SHORT, 0);

	// *** unbind vao ***
	glBindVertexArray(0);

	glUseProgram(0);
    [self myUpdate];
} 

-(void)myUpdate
{
    // code 
    angleMoon = angleMoon + 0.5;
	if (angleMoon > 360.0f)
	{
		angleMoon = 0.0f;
	}
	angleEarth = angleEarth + 0.5;
	if (angleEarth > 360.0f)
	{
		angleEarth = 0.0f;
	}
}

-(void)uninitialize
{
    // code 
    if(stack)
    {
        delete_stack(stack);
        stack = NULL;
    }
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
        case 27:
            [self uninitialize];
            [self release];
            [NSApp terminate:self];
            break;
        
        case 'F':
        case 'f':
            [[self window] toggleFullScreen:self];
            break;
        
        case 'D':
            day = (day + 6) % 360;
            break;

        case 'd':
            day = (day - 6) % 360;
            break;

        case 'Y':
            year = (year + 3) % 360;
            break;
            
        case 'y':
            year = (year - 3) % 360;
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

bool pushMatrix(mat4 matrix)
{
    if(matrixFull())
    {
        return false;
       
    }
    else
    {
        matrixStack[++top] = matrix;
    }
    
}

bool matrixFull()
{
    if(top == MAX)
    {
        return true;
    }
}

bool matrixEmpty()
{
    if(top == -1)
    {
        return true;
    }
}

mat4 popMatrix()
{
    if(matrixEmpty())
    {
        exit(0);
    }
    else
    {
        return matrixStack[top--];
    }
}
