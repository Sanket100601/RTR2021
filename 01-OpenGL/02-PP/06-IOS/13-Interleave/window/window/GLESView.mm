

#import "GLESView.h"

#import <OpenGLES/ES3/gl.h>
#import <OpenGLES/ES3/glext.h>
#import "vmath.h"

using  namespace vmath;
GLfloat lightAmbient[] = { 0.0f,0.0f,0.0f,1.0f };
GLfloat lightDiffuse[] = { 1.0f,1.0f,1.0f,1.0f };
GLfloat lightSpecular[] = { 1.0f,1.0f,1.0f,1.0f };
GLfloat lightPosition[] = { 100.0f,100.0f,100.0f,1.0f };

GLfloat materialAmbient[] = { 0.0f,0.0f,0.0f,1.0f };
GLfloat materialDiffuse[] = { 1.0f,1.0f,1.0f,1.0f };
GLfloat materialSpecular[] = { 1.0f,1.0f,1.0f,1.0f };
GLfloat materialShininess = 50.0f;
BOOL bLight = false;
@implementation GLESView

{
    @private
    EAGLContext *eaglContext;
    
    GLuint defaultFramebuffer;
    GLuint colorRenderBuffer;
    GLuint depthRenderBuffer;
    
    CADisplayLink *displayLink;
    GLint fps;
    BOOL isAnimating;
    
    
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

- (id) initWithFrame:(CGRect)frame
{
    //code
    self = [super initWithFrame:frame];
    
    if(self)
    {
        //Create drawable surface (Layer)
        CAEAGLLayer *eaglLayer = (CAEAGLLayer *)[super layer];
        
        //Set EAGL Layer properties
        [eaglLayer setOpaque:YES];
        
        NSNumber *boolNumber = [NSNumber numberWithBool:FALSE];
        
        NSDictionary *dictionary = [NSDictionary dictionaryWithObjectsAndKeys:boolNumber, kEAGLDrawablePropertyRetainedBacking, kEAGLColorFormatRGBA8, kEAGLDrawablePropertyColorFormat,nil];
        
        [eaglLayer setDrawableProperties:dictionary];
        
        //create EAGL Context
        eaglContext = [[EAGLContext alloc]initWithAPI:kEAGLRenderingAPIOpenGLES3];
        
        if(eaglContext == nil){
            printf("Falied to create eaglContext\n");
            [self uninitialize];
            [self release];
            exit(0);
        }
        
        //set this context as current
        [EAGLContext setCurrentContext:eaglContext];
        
        //print opengl info
        printf("Renderer : %s\n", glGetString(GL_RENDERER));
        printf("GLVersion : %s\n", glGetString(GL_VERSION));
        printf("GLESVersion : %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

        //create framebuffer
        glGenFramebuffers(1, &defaultFramebuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, defaultFramebuffer);
        
        glGenRenderbuffers(1, &colorRenderBuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, colorRenderBuffer);
        
        //provide storage to render buffer
        [eaglContext renderbufferStorage:GL_RENDERBUFFER fromDrawable:eaglLayer];
        
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, colorRenderBuffer);
        
        //Backing width and height
        GLint backingWidth;
        GLint backingHeight;
        
        //Get color render buffer into backing width and height
        glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_WIDTH, &backingWidth);
        glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_HEIGHT, &backingHeight);
        printf("Width : %d\n", backingWidth);
        printf("Height : %d\n", backingHeight);
        //Create Depth render buffer
        glGenRenderbuffers(1, &depthRenderBuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, depthRenderBuffer);
        
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, backingWidth, backingHeight);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRenderBuffer);
        
        //Check Status of framebuffer
        if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        {
            printf("Failed to create framebuffer\n");
            [self uninitialize];
            [self release];
            exit(0);
        }
        
        [self initialize];
        
        fps = 60;   //iOS 8.2
        isAnimating = NO;
        
        //Single tap
        UITapGestureRecognizer *singleTapGestureRecognizer = [[UITapGestureRecognizer alloc]initWithTarget:self action:@selector(onSingleTap:)];
        
        [singleTapGestureRecognizer setNumberOfTapsRequired:1];
        
        [singleTapGestureRecognizer setNumberOfTouchesRequired:1];
        
        [singleTapGestureRecognizer setDelegate:self];
        
        [self addGestureRecognizer:singleTapGestureRecognizer];
        
        //Double Tap
        UITapGestureRecognizer *doubleTapGestureRecognizer = [[UITapGestureRecognizer alloc]initWithTarget:self action:@selector(onDoubleTap:)];
        
        [doubleTapGestureRecognizer setNumberOfTapsRequired:2];
        
        [doubleTapGestureRecognizer setNumberOfTouchesRequired:1];
        
        [doubleTapGestureRecognizer setDelegate:self];
        
        [self addGestureRecognizer:doubleTapGestureRecognizer];
        
        //
        [singleTapGestureRecognizer requireGestureRecognizerToFail:doubleTapGestureRecognizer];
        
        //Swipe
        UISwipeGestureRecognizer *swipeGestureRecognizer = [[UISwipeGestureRecognizer alloc]initWithTarget:self action:@selector(onSwipe:)];
        [self addGestureRecognizer:swipeGestureRecognizer];
        
        //Long Press
        UILongPressGestureRecognizer *longPressGestureRecognizer = [[UILongPressGestureRecognizer alloc]initWithTarget:self action:@selector(onLongPress:)];
        [self addGestureRecognizer:longPressGestureRecognizer];
        
    }
    
    return self;
}

+ (Class)layerClass
{
    //code
    return([CAEAGLLayer class]);
}




- (void)layoutSubviews
{
    //code
    
    glBindRenderbuffer(GL_RENDERBUFFER, colorRenderBuffer);

    [eaglContext renderbufferStorage:GL_RENDERBUFFER fromDrawable:(CAEAGLLayer*)[self layer]];
    
    GLint backingWidth;
    GLint backingHeight;
    
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_WIDTH, &backingWidth);
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_HEIGHT, &backingHeight);
    
    glGenRenderbuffers(1, &depthRenderBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, depthRenderBuffer);
    
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, backingWidth, backingHeight);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRenderBuffer);
    
    //call resize here
    [self resize:backingWidth :backingHeight];

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        printf("Failed to create framebuffer\n");
        [self uninitialize];
        [self release];
        exit(0);
    }
    
    //call drawView
    [self drawView:self];
}

//- (void)drawRect:(CGRect)dirtyRect
//{
    //code
    
//}

- (void)drawView:(id)sender
{
    //code
    [EAGLContext setCurrentContext:eaglContext];
    
    glBindFramebuffer(GL_FRAMEBUFFER, defaultFramebuffer);
    
    //call display and update here
    [self display];
    [self update];
    
    glBindRenderbuffer(GL_RENDERBUFFER, colorRenderBuffer);
    
    [eaglContext presentRenderbuffer:GL_RENDERBUFFER];
    
}

- (void)startAnimation
{
    //code
    if(isAnimating == NO)
    {
        //create display link
        displayLink = [CADisplayLink displayLinkWithTarget:self selector:@selector(drawView:)];
        
        //set fps
        [displayLink setPreferredFramesPerSecond:fps];
        
        //Game Loop
        [displayLink addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
        
        isAnimating = YES;
    }
}

- (void)stopAnimation
{
    //code
    if(isAnimating == YES)
    {
        [displayLink invalidate];
        displayLink = nil;
        
        isAnimating = NO;
    }
}

- (int)initialize
{
    //code
    const GLchar* vertexShaderSourcecode =
            "#version 300 es" \
            "\n" \
            "uniform mediump int LightingEnabled;" \
            "in vec4 a_position;" \
            "in vec3 a_normal;" \
            "in vec2 a_texcoord;" \
            "in vec4 a_color;" \
            "uniform mat4 u_modelMatrix;" \
            "uniform mat4 u_viewMatrix;" \
            "uniform mat4 u_projectionMatrix;" \
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
            "        mat3 normalMatrix = mat3(u_viewMatrix * u_modelMatrix);" \
            "       transformedNormal = mat3(u_viewMatrix * u_modelMatrix) * a_normal;" \
            "       lightDirection = vec3(u_lightPosition) - eyeCoordinates.xyz;" \
            "       viewVector = -eyeCoordinates.xyz;" \
            "   }" \
            "   gl_Position = u_projectionMatrix * u_viewMatrix * u_modelMatrix * a_position;" \
            "    a_color_out = a_color;" \
            "    a_texcoord_out = a_texcoord;" \
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
                    printf( "Vertex Shader Compilation Log: %s\n", log);

                    // Free the allocated the buffer.
                    free(log);

                    // exit the application due to error
                    [self uninitialize];
                    [self release];
                    
                }
            }
        }
        

                                                

        // Fragment Shader
        
        // 1. Writing shader code
        const GLchar* fragmentShaderSourcecode =
            "#version 300 es" \
            "\n" \
            "precision highp float;" \
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
            "uniform mediump int LightingEnabled;" \
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
            
            "}"    \
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
                    printf("Fragment Shader Compilation Log: %s\n", log);

                    // Free the allocated the buffer.
                    free(log);

                    // exit the application due to error
                    [self uninitialize];
                    [self release];
                    
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
                    printf("Shader Program Link Log : %s\n", log);

                    // Free the allocated the buffer.
                    free(log);

                    // exit the application due to error
                    [self uninitialize];
                    [self release];
                   

                }

            }
        }


        // VOA AND VBA Array related lines
        
        // declarations of vertex data arrays
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

    
    glClearDepthf(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    
    texture_marble = [self LoadGlTexture : @"marble" : @"bmp"];
    if(!texture_marble)
    {
        printf("Error : failed to load marble.bmp texture.\n");
        [self uninitialize];
        [self release];
    }
    
    glEnable(GL_TEXTURE_2D);
    
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    perceptivegraphicsProjectionMatrix = mat4::identity();
    return 0;
}

-(GLuint)LoadGlTexture:(NSString *) textureFilename: (NSString *)extenstion
{
    // code
    NSBundle *appBundle = [NSBundle mainBundle];
    NSString * textureFileNameWithExtention = [appBundle pathForResource: textureFilename ofType: extenstion];
    
    UIImage *uiImage = [[UIImage alloc] initWithContentsOfFile:textureFileNameWithExtention];
    if(!uiImage)
    {
        printf("initWithContentsOfFile failed\n");
        return (0);
    }
            
    CGImageRef cgImageRef = [uiImage CGImage];


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



- (void)resize:(int)width :(int)height
{
    //code
    if(height < 0)
    {
        height = 1;
    }
    
    glViewport(0, 0, (GLsizei)width, (GLsizei)height);
    
    perceptivegraphicsProjectionMatrix = vmath::perspective(45.0f, (GLfloat)width / (GLfloat)height, 1.0f, 100.0f);

}

- (void)display
{
    //code
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
    modelMatrix = translationMatrix * scaleMatrix * rotationMatrix;    // Order is very important
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

    glUseProgram(0);

}

- (void)update
{
    //code
    angleCube += 0.5f;
    if (angleCube >= 360.f)
    {
        angleCube = angleCube - 360.0f;
    }
}

- (void)uninitialize
{
    //code
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
            (GLsizei*)    &shaderProgramObject,
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

    if(depthRenderBuffer)
    {
        glDeleteRenderbuffers(1, &depthRenderBuffer);
        
        depthRenderBuffer = 0;
    }
    
    if(colorRenderBuffer)
    {
        glDeleteRenderbuffers(1, &colorRenderBuffer);
        
        colorRenderBuffer = 0;
    }
    
    if(defaultFramebuffer)
    {
        glDeleteFramebuffers(1, &defaultFramebuffer);
        
        defaultFramebuffer = 0;
    }
    
    if([EAGLContext currentContext] == eaglContext)
    {
        [EAGLContext setCurrentContext:nil];
    }
    
    [eaglContext release];
    eaglContext = nil;
    
    [super dealloc];
}

- (BOOL)acceptsFirstResponder
{
    //code
    return YES;
}

- (void)touchesBegan:(NSSet*)touches withEvent:(UIEvent *)event
{
    //code
    
}

- (void) onSingleTap:(UITapGestureRecognizer*)gr
{
    //code
    if (bLight == false)
    {
        bLight = true;
    }
    else
    {
        bLight = false;
    }
        
}

- (void) onDoubleTap:(UITapGestureRecognizer*)gr
{
    //code
    
}

- (void) onSwipe:(UISwipeGestureRecognizer*)gr
{
    
    //code
    [self uninitialize];
    
    [self release];
    exit(0);
}

- (void) onLongPress:(UILongPressGestureRecognizer*)gr
{
    //code
    
}

- (void) dealloc
{
    //code
    [self uninitialize];
    
    if(displayLink)
    {
        [displayLink release];
        
        displayLink = nil;
    }
    
    [super dealloc];
}

@end
