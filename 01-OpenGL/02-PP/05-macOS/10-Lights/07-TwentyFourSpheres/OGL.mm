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

GLfloat materialAmbient[4];
GLfloat materialDiffuse[4];
GLfloat materialSpecular[4];
GLfloat materialShininess;
int bXAxis = true;
int bYAxis = false;
int bZAxis = false;

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
	int numVertices;
	int numElements;

	float sphere_vertices[1146];
	float sphere_normals[1146];
	float sphere_textures[764];
	unsigned short sphere_elements[2280];
	unsigned int gNumVertices;
	unsigned int gNumElements;
	GLuint gVao_sphere;
	GLuint gVbo_sphere_position;
	GLuint gVbo_sphere_normal;
	GLuint gVbo_sphere_element;

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
    // code// Vertex Shader code
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

	laUniform = glGetUniformLocation(shaderProgramObject, "u_la");
	ldUniform = glGetUniformLocation(shaderProgramObject, "u_ld");
	lsUniform = glGetUniformLocation(shaderProgramObject, "u_ls");
	kaUniform = glGetUniformLocation(shaderProgramObject, "u_ka");
	kdUniform = glGetUniformLocation(shaderProgramObject, "u_kd");
	ksUniform = glGetUniformLocation(shaderProgramObject, "u_ks");
	lightPositionUniform = glGetUniformLocation(shaderProgramObject, "u_lightPosition");
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
	
	 float sphere_vertices[1146];
    float sphere_normals[1146];
        float sphere_textures[764];
        short sphere_elements[2280];

        Sphere *sphere = [[Sphere alloc] init];
        [sphere getSphereVertexData:sphere_vertices :sphere_normals :sphere_textures :sphere_elements];
        numVertices = [sphere getNumberOfSphereVertices];
        numElements = [sphere getNumberOfSphereElements];
        
        //set up vao and vbo
        glGenVertexArrays(1, &gVao_sphere);
        glBindVertexArray(gVao_sphere);
        glGenBuffers(1, &gVbo_sphere_position);
        glBindBuffer(GL_ARRAY_BUFFER, gVbo_sphere_position);

        glBufferData(GL_ARRAY_BUFFER, sizeof(sphere_vertices), sphere_vertices, GL_STATIC_DRAW);
        glVertexAttribPointer(AMC_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray(AMC_ATTRIBUTE_POSITION);
    	glBindBuffer(GL_ARRAY_BUFFER, 0);

        glGenBuffers(1, &gVbo_sphere_normal);
        glBindBuffer(GL_ARRAY_BUFFER, gVbo_sphere_normal);
        glBufferData(GL_ARRAY_BUFFER, sizeof(sphere_normals), sphere_normals, GL_STATIC_DRAW);
        glVertexAttribPointer(AMC_ATTRIBUTE_NORMAL, 3, GL_FLOAT, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray(AMC_ATTRIBUTE_NORMAL);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glGenBuffers(1, &gVbo_sphere_element);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_sphere_element);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(sphere_elements), sphere_elements, GL_STATIC_DRAW);
        glBindVertexArray(0);
        
        [sphere release];

	// Here Starts OpenGL Code
	// Clear the Screen using Blue Color
	glClearColor(0.0f, 0.0f, 1.0f, 1.0f);

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

	}
	else
	{
		glUniform1i(LightingEnabledUniform, 0);
	}
	// bind with vertex array object
	[self draw24Sphere];





	glUseProgram(0);
} 

-(void)draw24Sphere
{
	/* Variable declarations*/
	GLfloat xPos = -2.3f;
	GLfloat yPos = 0.3f;
	GLfloat zPos = -5.0f;

	/* CODE */

	// ***** 1st sphere on 1st column, emerald *****
	// ambient material
	materialAmbient[0] = 0.0215; // r
	materialAmbient[1] = 0.1745; // g
	materialAmbient[2] = 0.0215; // b
	materialAmbient[3] = 1.0f;   // a
	
	// diffuse material
	materialDiffuse[0] = 0.07568; // r
	materialDiffuse[1] = 0.61424; // g
	materialDiffuse[2] = 0.07568; // b
	materialDiffuse[3] = 1.0f;    // a


	// specular material
	materialSpecular[0] = 0.633;    // r
	materialSpecular[1] = 0.727811; // g
	materialSpecular[2] = 0.633;    // b
	materialSpecular[3] = 1.0f;     // a


	// shininess
	materialShininess = 0.6 * 128;

	glUniform1f(materialShininessUniform, materialShininess);
	glUniform3fv(kaUniform, 1, materialAmbient);
	glUniform3fv(kdUniform, 1, materialDiffuse);
	glUniform3fv(ksUniform, 1, materialSpecular);

	mat4 translationMatrix = mat4::identity();
	mat4 modelMatrix = mat4::identity();
	mat4 viewMatrix = mat4::identity();

	mat4 modelViewProjectionMatrix = mat4::identity();
	mat4 scaleMatrix = mat4::identity();
	scaleMatrix = vmath::scale(0.35f, 0.35f, 0.35f);
	translationMatrix = vmath::translate(xPos, yPos + 1.3f, zPos);
	modelMatrix = translationMatrix * scaleMatrix;	// Order is very important
	modelViewProjectionMatrix = perceptivegraphicsProjectionMatrix * modelMatrix;
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

	
	glBindVertexArray(gVao_sphere);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_sphere_element);
	glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

	// *** unbind vao ***
	glBindVertexArray(0);
#if 1
	// *******************************************************

	// ***** 2nd sphere on 1st column, jade *****
	// ambient material
	materialAmbient[0] = 0.135;  // r
	materialAmbient[1] = 0.2225; // g
	materialAmbient[2] = 0.1575; // b
	materialAmbient[3] = 1.0f;   // a

	// diffuse material
	materialDiffuse[0] = 0.54; // r
	materialDiffuse[1] = 0.89; // g
	materialDiffuse[2] = 0.63; // b
	materialDiffuse[3] = 1.0f; // a

	// specular material
	materialSpecular[0] = 0.316228; // r
	materialSpecular[1] = 0.316228; // g
	materialSpecular[2] = 0.316228; // b
	materialSpecular[3] = 1.0f;     // a

	// shininess
	materialShininess = 0.1 * 128;
	glUniform1f(materialShininessUniform, materialShininess);
	glUniform3fv(kaUniform, 1, materialAmbient);
	glUniform3fv(kdUniform, 1, materialDiffuse);
	glUniform3fv(ksUniform, 1, materialSpecular);

	translationMatrix = mat4::identity();
	modelMatrix = mat4::identity();
	viewMatrix = mat4::identity();

	modelViewProjectionMatrix = mat4::identity();
	scaleMatrix = mat4::identity();
	scaleMatrix = vmath::scale(0.35f, 0.35f, 0.35f);
	translationMatrix = vmath::translate(xPos, yPos + 0.7f, zPos);
	modelMatrix = translationMatrix * scaleMatrix;	// Order is very important
	modelViewProjectionMatrix = perceptivegraphicsProjectionMatrix * modelMatrix;
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



	glBindVertexArray(gVao_sphere);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_sphere_element);
	glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

	// *** unbind vao ***
	glBindVertexArray(0);
	// *******************************************************

	// ***** 3rd sphere on 1st column, obsidian *****
	// ambient material
	materialAmbient[0] = 0.05375; // r
	materialAmbient[1] = 0.05;    // g
	materialAmbient[2] = 0.06625; // b
	materialAmbient[3] = 1.0f;    // a


	// diffuse material
	materialDiffuse[0] = 0.18275; // r
	materialDiffuse[1] = 0.17;    // g
	materialDiffuse[2] = 0.22525; // b
	materialDiffuse[3] = 1.0f;    // a


	// specular material
	materialSpecular[0] = 0.332741; // r
	materialSpecular[1] = 0.328634; // g
	materialSpecular[2] = 0.346435; // b
	materialSpecular[3] = 1.0f;     // a


	// shininess
	materialShininess = 0.3 * 128;


	glUniform1f(materialShininessUniform, materialShininess);
	glUniform3fv(kaUniform, 1, materialAmbient);
	glUniform3fv(kdUniform, 1, materialDiffuse);
	glUniform3fv(ksUniform, 1, materialSpecular);

	translationMatrix = mat4::identity();
	modelMatrix = mat4::identity();
	viewMatrix = mat4::identity();

	modelViewProjectionMatrix = mat4::identity();
	scaleMatrix = mat4::identity();
	scaleMatrix = vmath::scale(0.35f, 0.35f, 0.35f);
	translationMatrix = vmath::translate(xPos, yPos + 0.1f, zPos);
	modelMatrix = translationMatrix * scaleMatrix;	// Order is very important
	modelViewProjectionMatrix = perceptivegraphicsProjectionMatrix * modelMatrix;
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



	glBindVertexArray(gVao_sphere);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_sphere_element);
	glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

	// *** unbind vao ***
	glBindVertexArray(0);
	// *******************************************************

	// ***** 4th sphere on 1st column, pearl *****
	// ambient material
	materialAmbient[0] = 0.25;    // r
	materialAmbient[1] = 0.20725; // g
	materialAmbient[2] = 0.20725; // b
	materialAmbient[3] = 1.0f;    // a


	// diffuse material
	materialDiffuse[0] = 1.0;   // r
	materialDiffuse[1] = 0.829; // g
	materialDiffuse[2] = 0.829; // b
	materialDiffuse[3] = 1.0f;  // a


	// specular material
	materialSpecular[0] = 0.296648; // r
	materialSpecular[1] = 0.296648; // g
	materialSpecular[2] = 0.296648; // b
	materialSpecular[3] = 1.0f;     // a


	// shininess
	materialShininess = 0.088 * 128;


	// geometry
	glUniform1f(materialShininessUniform, materialShininess);
	glUniform3fv(kaUniform, 1, materialAmbient);
	glUniform3fv(kdUniform, 1, materialDiffuse);
	glUniform3fv(ksUniform, 1, materialSpecular);

	translationMatrix = mat4::identity();
	modelMatrix = mat4::identity();
	viewMatrix = mat4::identity();

	modelViewProjectionMatrix = mat4::identity();
	scaleMatrix = mat4::identity();
	scaleMatrix = vmath::scale(0.35f, 0.35f, 0.35f);
	translationMatrix = vmath::translate(xPos, yPos - 0.5f, zPos);
	modelMatrix = translationMatrix * scaleMatrix;	// Order is very important
	modelViewProjectionMatrix = perceptivegraphicsProjectionMatrix * modelMatrix;
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



	glBindVertexArray(gVao_sphere);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_sphere_element);
	glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

	// *** unbind vao ***
	glBindVertexArray(0);
	// *******************************************************

	// ***** 5th sphere on 1st column, ruby *****
	// ambient material
	materialAmbient[0] = 0.1745;  // r
	materialAmbient[1] = 0.01175; // g
	materialAmbient[2] = 0.01175; // b
	materialAmbient[3] = 1.0f;    // a


	// diffuse material
	materialDiffuse[0] = 0.61424; // r
	materialDiffuse[1] = 0.04136; // g
	materialDiffuse[2] = 0.04136; // b
	materialDiffuse[3] = 1.0f;    // a


	// specular material
	materialSpecular[0] = 0.727811; // r
	materialSpecular[1] = 0.626959; // g
	materialSpecular[2] = 0.626959; // b
	materialSpecular[3] = 1.0f;     // a


	// shininess
	materialShininess = 0.6 * 128;


	// geometry
	glUniform1f(materialShininessUniform, materialShininess);
	glUniform3fv(kaUniform, 1, materialAmbient);
	glUniform3fv(kdUniform, 1, materialDiffuse);
	glUniform3fv(ksUniform, 1, materialSpecular);

	translationMatrix = mat4::identity();
	modelMatrix = mat4::identity();
	viewMatrix = mat4::identity();

	modelViewProjectionMatrix = mat4::identity();
	scaleMatrix = mat4::identity();
	scaleMatrix = vmath::scale(0.35f, 0.35f, 0.35f);
	translationMatrix = vmath::translate(xPos, yPos - 1.1f, zPos);
	modelMatrix = translationMatrix * scaleMatrix;	// Order is very important
	modelViewProjectionMatrix = perceptivegraphicsProjectionMatrix * modelMatrix;
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


	glBindVertexArray(gVao_sphere);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_sphere_element);
	glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

	// *** unbind vao ***
	glBindVertexArray(0);
	// *******************************************************

	// ***** 6th sphere on 1st column, turquoise *****
	// ambient material
	materialAmbient[0] = 0.1;     // r
	materialAmbient[1] = 0.18725; // g
	materialAmbient[2] = 0.1745;  // b
	materialAmbient[3] = 1.0f;    // a


	// diffuse material
	materialDiffuse[0] = 0.396;   // r
	materialDiffuse[1] = 0.74151; // g
	materialDiffuse[2] = 0.69102; // b
	materialDiffuse[3] = 1.0f;    // a


	// specular material
	materialSpecular[0] = 0.297254; // r
	materialSpecular[1] = 0.30829;  // g
	materialSpecular[2] = 0.306678; // b
	materialSpecular[3] = 1.0f;     // a


	// shininess
	materialShininess = 0.1 * 128;


	// geometry
	glUniform1f(materialShininessUniform, materialShininess);
	glUniform3fv(kaUniform, 1, materialAmbient);
	glUniform3fv(kdUniform, 1, materialDiffuse);
	glUniform3fv(ksUniform, 1, materialSpecular);

	translationMatrix = mat4::identity();
	modelMatrix = mat4::identity();
	viewMatrix = mat4::identity();

	modelViewProjectionMatrix = mat4::identity();
	scaleMatrix = mat4::identity();
	scaleMatrix = vmath::scale(0.35f, 0.35f, 0.35f);
	translationMatrix = vmath::translate(xPos, yPos - 1.7f, zPos);
	modelMatrix = translationMatrix * scaleMatrix;	// Order is very important
	modelViewProjectionMatrix = perceptivegraphicsProjectionMatrix * modelMatrix;
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



	glBindVertexArray(gVao_sphere);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_sphere_element);
	glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

	// *** unbind vao ***
	glBindVertexArray(0);
	// *******************************************************
	// *******************************************************
	// *******************************************************



	// ***** 1st sphere on 2nd column, brass *****
	// ambient material
	materialAmbient[0] = 0.329412; // r
	materialAmbient[1] = 0.223529; // g
	materialAmbient[2] = 0.027451; // b
	materialAmbient[3] = 1.0f;     // a


	// diffuse material
	materialDiffuse[0] = 0.780392; // r
	materialDiffuse[1] = 0.568627; // g
	materialDiffuse[2] = 0.113725; // b
	materialDiffuse[3] = 1.0f;     // a


	// specular material
	materialSpecular[0] = 0.992157; // r
	materialSpecular[1] = 0.941176; // g
	materialSpecular[2] = 0.807843; // b
	materialSpecular[3] = 1.0f;     // a


	// shininess
	materialShininess = 0.21794872 * 128;

	glUniform1f(materialShininessUniform, materialShininess);
	glUniform3fv(kaUniform, 1, materialAmbient);
	glUniform3fv(kdUniform, 1, materialDiffuse);
	glUniform3fv(ksUniform, 1, materialSpecular);

	translationMatrix = mat4::identity();
	modelMatrix = mat4::identity();
	viewMatrix = mat4::identity();

	modelViewProjectionMatrix = mat4::identity();
	scaleMatrix = mat4::identity();
	scaleMatrix = vmath::scale(0.35f, 0.35f, 0.35f);
	translationMatrix = vmath::translate(xPos + 1.5f, yPos + 1.3f, zPos);
	modelMatrix = translationMatrix * scaleMatrix;	// Order is very important
	modelViewProjectionMatrix = perceptivegraphicsProjectionMatrix * modelMatrix;
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



	glBindVertexArray(gVao_sphere);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_sphere_element);
	glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

	// *** unbind vao ***
	glBindVertexArray(0);
	// geometry

	// *******************************************************

	// ***** 2nd sphere on 2nd column, bronze *****
	// ambient material
	materialAmbient[0] = 0.2125; // r
	materialAmbient[1] = 0.1275; // g
	materialAmbient[2] = 0.054;  // b
	materialAmbient[3] = 1.0f;   // a


	// diffuse material
	materialDiffuse[0] = 0.714;   // r
	materialDiffuse[1] = 0.4284;  // g
	materialDiffuse[2] = 0.18144; // b
	materialDiffuse[3] = 1.0f;    // a


	// specular material
	materialSpecular[0] = 0.393548; // r
	materialSpecular[1] = 0.271906; // g
	materialSpecular[2] = 0.166721; // b
	materialSpecular[3] = 1.0f;     // a


	// shininess
	materialShininess = 0.2 * 128;

	// geometry
	glUniform1f(materialShininessUniform, materialShininess);
	glUniform3fv(kaUniform, 1, materialAmbient);
	glUniform3fv(kdUniform, 1, materialDiffuse);
	glUniform3fv(ksUniform, 1, materialSpecular);

	translationMatrix = mat4::identity();
	modelMatrix = mat4::identity();
	viewMatrix = mat4::identity();

	modelViewProjectionMatrix = mat4::identity();
	scaleMatrix = mat4::identity();
	scaleMatrix = vmath::scale(0.35f, 0.35f, 0.35f);
	translationMatrix = vmath::translate(xPos + 1.5f, yPos + 0.7f, zPos);
	modelMatrix = translationMatrix * scaleMatrix;	// Order is very important
	modelViewProjectionMatrix = perceptivegraphicsProjectionMatrix * modelMatrix;
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


	glBindVertexArray(gVao_sphere);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_sphere_element);
	glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

	// *** unbind vao ***
	glBindVertexArray(0);
	// *******************************************************

	// ***** 3rd sphere on 2nd column, chrome *****
	// ambient material
	materialAmbient[0] = 0.25; // r
	materialAmbient[1] = 0.25; // g
	materialAmbient[2] = 0.25; // b
	materialAmbient[3] = 1.0f; // a


	// diffuse material
	materialDiffuse[0] = 0.4;  // r
	materialDiffuse[1] = 0.4;  // g
	materialDiffuse[2] = 0.4;  // b
	materialDiffuse[3] = 1.0f; // a
	// specular material
	materialSpecular[0] = 0.774597; // r
	materialSpecular[1] = 0.774597; // g
	materialSpecular[2] = 0.774597; // b
	materialSpecular[3] = 1.0f;     // a


	// shininess
	materialShininess = 0.6 * 128;


	// geometry
	glUniform1f(materialShininessUniform, materialShininess);
	glUniform3fv(kaUniform, 1, materialAmbient);
	glUniform3fv(kdUniform, 1, materialDiffuse);
	glUniform3fv(ksUniform, 1, materialSpecular);

	translationMatrix = mat4::identity();
	modelMatrix = mat4::identity();
	viewMatrix = mat4::identity();

	modelViewProjectionMatrix = mat4::identity();
	scaleMatrix = mat4::identity();
	scaleMatrix = vmath::scale(0.35f, 0.35f, 0.35f);
	translationMatrix = vmath::translate(xPos + 1.5f, yPos + 0.1f, zPos);
	modelMatrix = translationMatrix * scaleMatrix;	// Order is very important
	modelViewProjectionMatrix = perceptivegraphicsProjectionMatrix * modelMatrix;
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

	

	glBindVertexArray(gVao_sphere);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_sphere_element);
	glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

	// *** unbind vao ***
	glBindVertexArray(0);
	// *******************************************************

	// ***** 4th sphere on 2nd column, copper *****
	// ambient material
	materialAmbient[0] = 0.19125; // r
	materialAmbient[1] = 0.0735;  // g
	materialAmbient[2] = 0.0225;  // b
	materialAmbient[3] = 1.0f;    // a


	// diffuse material
	materialDiffuse[0] = 0.7038;  // r
	materialDiffuse[1] = 0.27048; // g
	materialDiffuse[2] = 0.0828;  // b
	materialDiffuse[3] = 1.0f;    // a


	// specular material
	materialSpecular[0] = 0.256777; // r
	materialSpecular[1] = 0.137622; // g
	materialSpecular[2] = 0.086014; // b
	materialSpecular[3] = 1.0f;     // a


	// shininess
	materialShininess = 0.1 * 128;


	// geometry
	glUniform1f(materialShininessUniform, materialShininess);
	glUniform3fv(kaUniform, 1, materialAmbient);
	glUniform3fv(kdUniform, 1, materialDiffuse);
	glUniform3fv(ksUniform, 1, materialSpecular);

	translationMatrix = mat4::identity();
	modelMatrix = mat4::identity();
	viewMatrix = mat4::identity();

	modelViewProjectionMatrix = mat4::identity();
	scaleMatrix = mat4::identity();
	scaleMatrix = vmath::scale(0.35f, 0.35f, 0.35f);
	translationMatrix = vmath::translate(xPos + 1.5f, yPos - 0.5f, zPos);
	modelMatrix = translationMatrix * scaleMatrix;	// Order is very important
	modelViewProjectionMatrix = perceptivegraphicsProjectionMatrix * modelMatrix;
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



	glBindVertexArray(gVao_sphere);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_sphere_element);
	glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

	// *** unbind vao ***
	glBindVertexArray(0);
	// *******************************************************

	// ***** 5th sphere on 2nd column, gold *****
	// ambient material
	materialAmbient[0] = 0.24725; // r
	materialAmbient[1] = 0.1995;  // g
	materialAmbient[2] = 0.0745;  // b
	materialAmbient[3] = 1.0f;    // a


	// diffuse material
	materialDiffuse[0] = 0.75164; // r
	materialDiffuse[1] = 0.60648; // g
	materialDiffuse[2] = 0.22648; // b
	materialDiffuse[3] = 1.0f;    // a


	// specular material
	materialSpecular[0] = 0.628281; // r
	materialSpecular[1] = 0.555802; // g
	materialSpecular[2] = 0.366065; // b
	materialSpecular[3] = 1.0f;     // a


	// shininess
	materialShininess = 0.4 * 128;


	// geometry
	glUniform1f(materialShininessUniform, materialShininess);
	glUniform3fv(kaUniform, 1, materialAmbient);
	glUniform3fv(kdUniform, 1, materialDiffuse);
	glUniform3fv(ksUniform, 1, materialSpecular);

	translationMatrix = mat4::identity();
	modelMatrix = mat4::identity();
	viewMatrix = mat4::identity();

	modelViewProjectionMatrix = mat4::identity();
	scaleMatrix = mat4::identity();
	scaleMatrix = vmath::scale(0.35f, 0.35f, 0.35f);
	translationMatrix = vmath::translate(xPos + 1.5f, yPos - 1.1f, zPos);
	modelMatrix = translationMatrix * scaleMatrix;	// Order is very important
	modelViewProjectionMatrix = perceptivegraphicsProjectionMatrix * modelMatrix;
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



	glBindVertexArray(gVao_sphere);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_sphere_element);
	glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

	// *** unbind vao ***
	glBindVertexArray(0);
	// *******************************************************

	// ***** 6th sphere on 2nd column, silver *****
	// ambient material
	materialAmbient[0] = 0.19225; // r
	materialAmbient[1] = 0.19225; // g
	materialAmbient[2] = 0.19225; // b
	materialAmbient[3] = 1.0f;    // a


	// diffuse material
	materialDiffuse[0] = 0.50754; // r
	materialDiffuse[1] = 0.50754; // g
	materialDiffuse[2] = 0.50754; // b
	materialDiffuse[3] = 1.0f;    // a


	// specular material
	materialSpecular[0] = 0.508273; // r
	materialSpecular[1] = 0.508273; // g
	materialSpecular[2] = 0.508273; // b
	materialSpecular[3] = 1.0f;     // a


	// shininess
	materialShininess = 0.4 * 128;


	// geometry
	glUniform1f(materialShininessUniform, materialShininess);
	glUniform3fv(kaUniform, 1, materialAmbient);
	glUniform3fv(kdUniform, 1, materialDiffuse);
	glUniform3fv(ksUniform, 1, materialSpecular);

	translationMatrix = mat4::identity();
	modelMatrix = mat4::identity();
	viewMatrix = mat4::identity();

	modelViewProjectionMatrix = mat4::identity();
	scaleMatrix = mat4::identity();
	scaleMatrix = vmath::scale(0.35f, 0.35f, 0.35f);
	translationMatrix = vmath::translate(xPos + 1.5f, yPos - 1.7f, zPos);
	modelMatrix = translationMatrix * scaleMatrix;	// Order is very important
	modelViewProjectionMatrix = perceptivegraphicsProjectionMatrix * modelMatrix;
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


	glBindVertexArray(gVao_sphere);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_sphere_element);
	glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

	// *** unbind vao ***
	glBindVertexArray(0);
	// *******************************************************
	// *******************************************************
	// *******************************************************

	// ***** 1st sphere on 3rd column, black *****
	// ambient material
	materialAmbient[0] = 0.0;  // r
	materialAmbient[1] = 0.0;  // g
	materialAmbient[2] = 0.0;  // b
	materialAmbient[3] = 1.0f; // a


	// diffuse material
	materialDiffuse[0] = 0.01; // r
	materialDiffuse[1] = 0.01; // g
	materialDiffuse[2] = 0.01; // b
	materialDiffuse[3] = 1.0f; // a


	// specular material
	materialSpecular[0] = 0.50; // r
	materialSpecular[1] = 0.50; // g
	materialSpecular[2] = 0.50; // b
	materialSpecular[3] = 1.0f; // a


	// shininess
	materialShininess = 0.25 * 128;


	// geometry
	glUniform1f(materialShininessUniform, materialShininess);
	glUniform3fv(kaUniform, 1, materialAmbient);
	glUniform3fv(kdUniform, 1, materialDiffuse);
	glUniform3fv(ksUniform, 1, materialSpecular);

	translationMatrix = mat4::identity();
	modelMatrix = mat4::identity();
	viewMatrix = mat4::identity();

	modelViewProjectionMatrix = mat4::identity();
	scaleMatrix = mat4::identity();
	scaleMatrix = vmath::scale(0.35f, 0.35f, 0.35f);
	translationMatrix = vmath::translate(xPos + 3.0f, yPos + 1.3f, zPos);
	modelMatrix = translationMatrix * scaleMatrix;	// Order is very important
	modelViewProjectionMatrix = perceptivegraphicsProjectionMatrix * modelMatrix;
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


	glBindVertexArray(gVao_sphere);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_sphere_element);
	glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

	// *** unbind vao ***
	glBindVertexArray(0);
	// *******************************************************

	// ***** 2nd sphere on 3rd column, cyan *****
	// ambient material
	materialAmbient[0] = 0.0;  // r
	materialAmbient[1] = 0.1;  // g
	materialAmbient[2] = 0.06; // b
	materialAmbient[3] = 1.0f; // a
	

	// diffuse material
	materialDiffuse[0] = 0.0;        // r
	materialDiffuse[1] = 0.50980392; // g
	materialDiffuse[2] = 0.50980392; // b
	materialDiffuse[3] = 1.0f;       // a
	

	// specular material
	materialSpecular[0] = 0.50196078; // r
	materialSpecular[1] = 0.50196078; // g
	materialSpecular[2] = 0.50196078; // b
	materialSpecular[3] = 1.0f;       // a
	

	// shininess
	materialShininess = 0.25 * 128;
	

	// geometry
	glUniform1f(materialShininessUniform, materialShininess);
	glUniform3fv(kaUniform, 1, materialAmbient);
	glUniform3fv(kdUniform, 1, materialDiffuse);
	glUniform3fv(ksUniform, 1, materialSpecular);

	translationMatrix = mat4::identity();
	modelMatrix = mat4::identity();
	viewMatrix = mat4::identity();

	modelViewProjectionMatrix = mat4::identity();
	scaleMatrix = mat4::identity();
	scaleMatrix = vmath::scale(0.35f, 0.35f, 0.35f);
	translationMatrix = vmath::translate(xPos + 3.0f, yPos + 0.7f, zPos);
	modelMatrix = translationMatrix * scaleMatrix;	// Order is very important
	modelViewProjectionMatrix = perceptivegraphicsProjectionMatrix * modelMatrix;
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


	glBindVertexArray(gVao_sphere);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_sphere_element);
	glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

	// *** unbind vao ***
	glBindVertexArray(0);
	// *******************************************************

	// ***** 3rd sphere on 2nd column, green *****
	// ambient material
	materialAmbient[0] = 0.0;  // r
	materialAmbient[1] = 0.0;  // g
	materialAmbient[2] = 0.0;  // b
	materialAmbient[3] = 1.0f; // a


	// diffuse material
	materialDiffuse[0] = 0.1;  // r
	materialDiffuse[1] = 0.35; // g
	materialDiffuse[2] = 0.1;  // b
	materialDiffuse[3] = 1.0f; // a


	// specular material
	materialSpecular[0] = 0.45; // r
	materialSpecular[1] = 0.55; // g
	materialSpecular[2] = 0.45; // b
	materialSpecular[3] = 1.0f; // a


	// shininess
	materialShininess = 0.25 * 128;


	// geometry
	glUniform1f(materialShininessUniform, materialShininess);
	glUniform3fv(kaUniform, 1, materialAmbient);
	glUniform3fv(kdUniform, 1, materialDiffuse);
	glUniform3fv(ksUniform, 1, materialSpecular);

	translationMatrix = mat4::identity();
	modelMatrix = mat4::identity();
	viewMatrix = mat4::identity();

	modelViewProjectionMatrix = mat4::identity();
	scaleMatrix = mat4::identity();
	scaleMatrix = vmath::scale(0.35f, 0.35f, 0.35f);
	translationMatrix = vmath::translate(xPos + 3.0f, yPos + 0.1f, zPos);
	modelMatrix = translationMatrix * scaleMatrix;	// Order is very important
	modelViewProjectionMatrix = perceptivegraphicsProjectionMatrix * modelMatrix;
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


	glBindVertexArray(gVao_sphere);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_sphere_element);
	glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

	// *** unbind vao ***
	glBindVertexArray(0);
	// *******************************************************

	// ***** 4th sphere on 3rd column, red *****
	// ambient material
	materialAmbient[0] = 0.0;  // r
	materialAmbient[1] = 0.0;  // g
	materialAmbient[2] = 0.0;  // b
	materialAmbient[3] = 1.0f; // a
	

	// diffuse material
	materialDiffuse[0] = 0.5;  // r
	materialDiffuse[1] = 0.0;  // g
	materialDiffuse[2] = 0.0;  // b
	materialDiffuse[3] = 1.0f; // a
	

	// specular material
	materialSpecular[0] = 0.7;  // r
	materialSpecular[1] = 0.6;  // g
	materialSpecular[2] = 0.6;  // b
	materialSpecular[3] = 1.0f; // a
	

	// shininess
	materialShininess = 0.25 * 128;
	

	// geometry
	glUniform1f(materialShininessUniform, materialShininess);
	glUniform3fv(kaUniform, 1, materialAmbient);
	glUniform3fv(kdUniform, 1, materialDiffuse);
	glUniform3fv(ksUniform, 1, materialSpecular);

	translationMatrix = mat4::identity();
	modelMatrix = mat4::identity();
	viewMatrix = mat4::identity();

	modelViewProjectionMatrix = mat4::identity();
	scaleMatrix = mat4::identity();
	scaleMatrix = vmath::scale(0.35f, 0.35f, 0.35f);
	translationMatrix = vmath::translate(xPos + 3.0f, yPos - 0.5f, zPos);
	modelMatrix = translationMatrix * scaleMatrix;	// Order is very important
	modelViewProjectionMatrix = perceptivegraphicsProjectionMatrix * modelMatrix;
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


	glBindVertexArray(gVao_sphere);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_sphere_element);
	glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

	// *** unbind vao ***
	glBindVertexArray(0);
	// *******************************************************

	// ***** 5th sphere on 3rd column, white *****
	// ambient material
	materialAmbient[0] = 0.0;  // r
	materialAmbient[1] = 0.0;  // g
	materialAmbient[2] = 0.0;  // b
	materialAmbient[3] = 1.0f; // a
	

	// diffuse material
	materialDiffuse[0] = 0.55; // r
	materialDiffuse[1] = 0.55; // g
	materialDiffuse[2] = 0.55; // b
	materialDiffuse[3] = 1.0f; // a


	// specular material
	materialSpecular[0] = 0.70; // r
	materialSpecular[1] = 0.70; // g
	materialSpecular[2] = 0.70; // b
	materialSpecular[3] = 1.0f; // a


	// shininess
	materialShininess = 0.25 * 128;


	// geometry
	glUniform1f(materialShininessUniform, materialShininess);
	glUniform3fv(kaUniform, 1, materialAmbient);
	glUniform3fv(kdUniform, 1, materialDiffuse);
	glUniform3fv(ksUniform, 1, materialSpecular);

	translationMatrix = mat4::identity();
	modelMatrix = mat4::identity();
	viewMatrix = mat4::identity();

	modelViewProjectionMatrix = mat4::identity();
	scaleMatrix = mat4::identity();
	scaleMatrix = vmath::scale(0.35f, 0.35f, 0.35f);
	translationMatrix = vmath::translate(xPos + 3.0f, yPos - 1.1f, zPos);
	modelMatrix = translationMatrix * scaleMatrix;	// Order is very important
	modelViewProjectionMatrix = perceptivegraphicsProjectionMatrix * modelMatrix;
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


	glBindVertexArray(gVao_sphere);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_sphere_element);
	glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

	// *** unbind vao ***
	glBindVertexArray(0);
	// *******************************************************

	// ***** 6th sphere on 3rd column, yellow plastic *****
	// ambient material
	materialAmbient[0] = 0.0;  // r
	materialAmbient[1] = 0.0;  // g
	materialAmbient[2] = 0.0;  // b
	materialAmbient[3] = 1.0f; // a


	// diffuse material
	materialDiffuse[0] = 0.5;  // r
	materialDiffuse[1] = 0.5;  // g
	materialDiffuse[2] = 0.0;  // b
	materialDiffuse[3] = 1.0f; // a


	// specular material
	materialSpecular[0] = 0.60; // r
	materialSpecular[1] = 0.60; // g
	materialSpecular[2] = 0.50; // b
	materialSpecular[3] = 1.0f; // a


	// shininess
	materialShininess = 0.25 * 128;


	// geometry
	glUniform1f(materialShininessUniform, materialShininess);
	glUniform3fv(kaUniform, 1, materialAmbient);
	glUniform3fv(kdUniform, 1, materialDiffuse);
	glUniform3fv(ksUniform, 1, materialSpecular);

	translationMatrix = mat4::identity();
	modelMatrix = mat4::identity();
	viewMatrix = mat4::identity();

	modelViewProjectionMatrix = mat4::identity();
	scaleMatrix = mat4::identity();
	scaleMatrix = vmath::scale(0.35f, 0.35f, 0.35f);
	translationMatrix = vmath::translate(xPos + 3.0f, yPos - 1.7f, zPos);
	modelMatrix = translationMatrix * scaleMatrix;	// Order is very important
	modelViewProjectionMatrix = perceptivegraphicsProjectionMatrix * modelMatrix;
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


	glBindVertexArray(gVao_sphere);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_sphere_element);
	glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

	// *** unbind vao ***
	glBindVertexArray(0);
	// *******************************************************
	// *******************************************************
	// *******************************************************

	// ***** 1st sphere on 4th column, black *****
	// ambient material
	materialAmbient[0] = 0.02; // r
	materialAmbient[1] = 0.02; // g
	materialAmbient[2] = 0.02; // b
	materialAmbient[3] = 1.0f; // a
	

	// diffuse material
	materialDiffuse[0] = 0.01; // r
	materialDiffuse[1] = 0.01; // g
	materialDiffuse[2] = 0.01; // b
	materialDiffuse[3] = 1.0f; // a
	

	// specular material
	materialSpecular[0] = 0.4;  // r
	materialSpecular[1] = 0.4;  // g
	materialSpecular[2] = 0.4;  // b
	materialSpecular[3] = 1.0f; // a


	// shininess
	materialShininess = 0.078125 * 128;
	

	// geometry
	glUniform1f(materialShininessUniform, materialShininess);
	glUniform3fv(kaUniform, 1, materialAmbient);
	glUniform3fv(kdUniform, 1, materialDiffuse);
	glUniform3fv(ksUniform, 1, materialSpecular);

	translationMatrix = mat4::identity();
	modelMatrix = mat4::identity();
	viewMatrix = mat4::identity();

	modelViewProjectionMatrix = mat4::identity();
	scaleMatrix = mat4::identity();
	scaleMatrix = vmath::scale(0.35f, 0.35f, 0.35f);
	translationMatrix = vmath::translate(xPos + 4.5f, yPos + 1.3f, zPos);
	modelMatrix = translationMatrix * scaleMatrix;	// Order is very important
	modelViewProjectionMatrix = perceptivegraphicsProjectionMatrix * modelMatrix;
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


	glBindVertexArray(gVao_sphere);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_sphere_element);
	glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

	// *** unbind vao ***
	glBindVertexArray(0);
	// *******************************************************

	// ***** 2nd sphere on 4th column, cyan *****
	// ambient material
	materialAmbient[0] = 0.0;  // r
	materialAmbient[1] = 0.05; // g
	materialAmbient[2] = 0.05; // b
	materialAmbient[3] = 1.0f; // a


	// diffuse material
	materialDiffuse[0] = 0.4;  // r
	materialDiffuse[1] = 0.5;  // g
	materialDiffuse[2] = 0.5;  // b
	materialDiffuse[3] = 1.0f; // a


	// specular material
	materialSpecular[0] = 0.04; // r
	materialSpecular[1] = 0.7;  // g
	materialSpecular[2] = 0.7;  // b
	materialSpecular[3] = 1.0f; // a


	// shininess
	materialShininess = 0.078125 * 128;


	// geometry
	glUniform1f(materialShininessUniform, materialShininess);
	glUniform3fv(kaUniform, 1, materialAmbient);
	glUniform3fv(kdUniform, 1, materialDiffuse);
	glUniform3fv(ksUniform, 1, materialSpecular);

	translationMatrix = mat4::identity();
	modelMatrix = mat4::identity();
	viewMatrix = mat4::identity();

	modelViewProjectionMatrix = mat4::identity();
	scaleMatrix = mat4::identity();
	scaleMatrix = vmath::scale(0.35f, 0.35f, 0.35f);
	translationMatrix = vmath::translate(xPos + 4.5f, yPos + 0.7f, zPos);
	modelMatrix = translationMatrix * scaleMatrix;	// Order is very important
	modelViewProjectionMatrix = perceptivegraphicsProjectionMatrix * modelMatrix;
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


	glBindVertexArray(gVao_sphere);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_sphere_element);
	glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

	// *** unbind vao ***
	glBindVertexArray(0);
	// *******************************************************

	// ***** 3rd sphere on 4th column, green *****
	// ambient material
	materialAmbient[0] = 0.0;  // r
	materialAmbient[1] = 0.05; // g
	materialAmbient[2] = 0.0;  // b
	materialAmbient[3] = 1.0f; // a


	// diffuse material
	materialDiffuse[0] = 0.4;  // r
	materialDiffuse[1] = 0.5;  // g
	materialDiffuse[2] = 0.4;  // b
	materialDiffuse[3] = 1.0f; // a


	// specular material
	materialSpecular[0] = 0.04; // r
	materialSpecular[1] = 0.7;  // g
	materialSpecular[2] = 0.04; // b
	materialSpecular[3] = 1.0f; // a


	// shininess
	materialShininess = 0.078125 * 128;


	// geometry
	glUniform1f(materialShininessUniform, materialShininess);
	glUniform3fv(kaUniform, 1, materialAmbient);
	glUniform3fv(kdUniform, 1, materialDiffuse);
	glUniform3fv(ksUniform, 1, materialSpecular);

	translationMatrix = mat4::identity();
	modelMatrix = mat4::identity();
	viewMatrix = mat4::identity();

	modelViewProjectionMatrix = mat4::identity();
	scaleMatrix = mat4::identity();
	scaleMatrix = vmath::scale(0.35f, 0.35f, 0.35f);
	translationMatrix = vmath::translate(xPos + 4.5f, yPos + 0.1f, zPos);
	modelMatrix = translationMatrix * scaleMatrix;	// Order is very important
	modelViewProjectionMatrix = perceptivegraphicsProjectionMatrix * modelMatrix;
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


	glBindVertexArray(gVao_sphere);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_sphere_element);
	glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

	// *** unbind vao ***
	glBindVertexArray(0);
	// *******************************************************

	// ***** 4th sphere on 4th column, red *****
	// ambient material
	materialAmbient[0] = 0.05; // r
	materialAmbient[1] = 0.0;  // g
	materialAmbient[2] = 0.0;  // b
	materialAmbient[3] = 1.0f; // a


	// diffuse material
	materialDiffuse[0] = 0.5;  // r
	materialDiffuse[1] = 0.4;  // g
	materialDiffuse[2] = 0.4;  // b
	materialDiffuse[3] = 1.0f; // a


	// specular material
	materialSpecular[0] = 0.7;  // r
	materialSpecular[1] = 0.04; // g
	materialSpecular[2] = 0.04; // b
	materialSpecular[3] = 1.0f; // a


	// shininess
	materialShininess = 0.078125 * 128;


	// geometry
	glUniform1f(materialShininessUniform, materialShininess);
	glUniform3fv(kaUniform, 1, materialAmbient);
	glUniform3fv(kdUniform, 1, materialDiffuse);
	glUniform3fv(ksUniform, 1, materialSpecular);

	translationMatrix = mat4::identity();
	modelMatrix = mat4::identity();
	viewMatrix = mat4::identity();

	modelViewProjectionMatrix = mat4::identity();
	scaleMatrix = mat4::identity();
	scaleMatrix = vmath::scale(0.35f, 0.35f, 0.35f);
	translationMatrix = vmath::translate(xPos + 4.5f, yPos - 0.5f, zPos);
	modelMatrix = translationMatrix * scaleMatrix;	// Order is very important
	modelViewProjectionMatrix = perceptivegraphicsProjectionMatrix * modelMatrix;
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


	glBindVertexArray(gVao_sphere);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_sphere_element);
	glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

	// *** unbind vao ***
	glBindVertexArray(0);
	// *******************************************************

	// ***** 5th sphere on 4th column, white *****
	// ambient material
	materialAmbient[0] = 0.05; // r
	materialAmbient[1] = 0.05; // g
	materialAmbient[2] = 0.05; // b
	materialAmbient[3] = 1.0f; // a


	// diffuse material
	materialDiffuse[0] = 0.5;  // r
	materialDiffuse[1] = 0.5;  // g
	materialDiffuse[2] = 0.5;  // b
	materialDiffuse[3] = 1.0f; // a
	

	// specular material
	materialSpecular[0] = 0.7;  // r
	materialSpecular[1] = 0.7;  // g
	materialSpecular[2] = 0.7;  // b
	materialSpecular[3] = 1.0f; // a


	// shininess
	materialShininess = 0.078125 * 128;


	// geometry
	glUniform1f(materialShininessUniform, materialShininess);
	glUniform3fv(kaUniform, 1, materialAmbient);
	glUniform3fv(kdUniform, 1, materialDiffuse);
	glUniform3fv(ksUniform, 1, materialSpecular);

	translationMatrix = mat4::identity();
	modelMatrix = mat4::identity();
	viewMatrix = mat4::identity();

	modelViewProjectionMatrix = mat4::identity();
	scaleMatrix = mat4::identity();
	scaleMatrix = vmath::scale(0.35f, 0.35f, 0.35f);
	translationMatrix = vmath::translate(xPos + 4.5f, yPos - 1.1f, zPos);
	modelMatrix = translationMatrix * scaleMatrix;	// Order is very important
	modelViewProjectionMatrix = perceptivegraphicsProjectionMatrix * modelMatrix;
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


	glBindVertexArray(gVao_sphere);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_sphere_element);
	glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

	// *** unbind vao ***
	glBindVertexArray(0);
	// *******************************************************

	// ***** 6th sphere on 4th column, yellow rubber *****
	// ambient material
	materialAmbient[0] = 0.05; // r
	materialAmbient[1] = 0.05; // g
	materialAmbient[2] = 0.0;  // b
	materialAmbient[3] = 1.0f; // a


	// diffuse material
	materialDiffuse[0] = 0.5;  // r
	materialDiffuse[1] = 0.5;  // g
	materialDiffuse[2] = 0.4;  // b
	materialDiffuse[3] = 1.0f; // a


	// specular material
	materialSpecular[0] = 0.7;  // r
	materialSpecular[1] = 0.7;  // g
	materialSpecular[2] = 0.04; // b
	materialSpecular[3] = 1.0f; // a


	// shininess
	materialShininess = 0.078125 * 128;


	// geometry
	glUniform1f(materialShininessUniform, materialShininess);
	glUniform3fv(kaUniform, 1, materialAmbient);
	glUniform3fv(kdUniform, 1, materialDiffuse);
	glUniform3fv(ksUniform, 1, materialSpecular);

	translationMatrix = mat4::identity();
	modelMatrix = mat4::identity();
	viewMatrix = mat4::identity();

	modelViewProjectionMatrix = mat4::identity();
	scaleMatrix = mat4::identity();
	scaleMatrix = vmath::scale(0.35f, 0.35f, 0.35f);
	translationMatrix = vmath::translate(xPos + 4.5f, yPos - 1.7f, zPos);
	modelMatrix = translationMatrix * scaleMatrix;	// Order is very important
	modelViewProjectionMatrix = perceptivegraphicsProjectionMatrix * modelMatrix;
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

	

	glBindVertexArray(gVao_sphere);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_sphere_element);
	glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

	// *** unbind vao ***
	glBindVertexArray(0);
	// *******************************************************
	// *******************************************************
	// *******************************************************
#endif
}
-(void)myUpdate
{
    // code 
	float radius = 8.0f;
    angleSphere += 0.02f;
	if (angleSphere >= 360.f)
	{
		angleSphere = angleSphere - 360.0f;
	}
	if (bXAxis)
	{
		lightPosition = vmath::vec4(0.0f, radius * sinf(angleSphere * M_PI / 180.0f), radius * cosf(angleSphere * M_PI / 180.0f), 1.0f);
	}
	if (bYAxis)
	{
		lightPosition = vmath::vec4(radius * sinf(angleSphere * M_PI / 180.0f), 0.0f, radius * cosf(angleSphere * M_PI / 180.0f), 1.0f);

	}
	if (bZAxis)
	{
		lightPosition = vmath::vec4(radius * sinf(angleSphere * M_PI / 180.0f), radius * cosf(angleSphere * M_PI / 180.0f), 0.0f, 1.0f);
	}
}

-(void)uninitialize
{
    // code 
     if(gVbo_sphere_element)
    {
       glDeleteBuffers(1, &gVbo_sphere_element); 
	    gVbo_sphere_element = 0;
    }


    if(gVbo_sphere_normal)
    {
    	glDeleteBuffers(1, &gVbo_sphere_normal);
        gVbo_sphere_normal = 0;
    }

    if(gVbo_sphere_position)
    {
        glDeleteBuffers(1, &gVbo_sphere_position);
        gVbo_sphere_position = 0;
    }
    
    if(gVao_sphere)
    {
        glDeleteVertexArrays(1, &gVao_sphere);
        gVao_sphere = 0;
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
		case 'x':
		case 'X':
			bXAxis = true;
			bYAxis = false;
			bZAxis = false;
			break;

		case 'y':
		case 'Y':
			bXAxis = false;
			bYAxis = true;
			bZAxis = false;
			break;

		case 'z':
		case 'Z':
			bXAxis = false;
			bYAxis = false;
			bZAxis = true;
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
