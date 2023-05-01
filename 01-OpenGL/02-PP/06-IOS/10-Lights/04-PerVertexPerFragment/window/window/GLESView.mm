

#import "GLESView.h"
#import "Sphere.h"

#import <OpenGLES/ES3/gl.h>
#import <OpenGLES/ES3/glext.h>
#import "vmath.h"

using  namespace vmath;

int bLight;
GLfloat lightAmbient_PerFrag[] = { 0.0f,0.0f,0.0f,1.0f };
GLfloat lightDiffuse_PerFrag[] = { 1.0f,1.0f,1.0f,1.0f };
GLfloat lightSpecular_PerFrag[] = { 1.0f,1.0f,1.0f,1.0f };
GLfloat lightPosition_PerFrag[] = { 100.0f,100.0f,100.0f,1.0f };

GLfloat materialAmbient_PerFrag[] = { 0.0f,0.0f,0.0f,1.0f };
GLfloat materialDiffuse_PerFrag[] = { 1.0f,1.0f,1.0f,1.0f };
GLfloat materialSpecular_PerFrag[] = { 1.0f,1.0f,1.0f,1.0f };
GLfloat materialShininess_PerFrag = 50.0f;

GLfloat lightAmbient_PerVertex[] = { 0.0f,0.0f,0.0f,1.0f };
GLfloat lightDiffuse_PerVertex[] = { 1.0f,1.0f,1.0f,1.0f };
GLfloat lightSpecular_PerVertex[] = { 1.0f,1.0f,1.0f,1.0f };
GLfloat lightPosition_PerVertex[] = { 100.0f,100.0f,100.0f,1.0f };

GLfloat materialAmbient_PerVertex[] = { 0.0f,0.0f,0.0f,1.0f };
GLfloat materialDiffuse_PerVertex[] = { 1.0f,1.0f,1.0f,1.0f };
GLfloat materialSpecular_PerVertex[] = { 1.0f,1.0f,1.0f,1.0f };
GLfloat materialShininess_PerVertex = 50.0f;

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
        GLuint shaderProgramObject_pf;
        GLuint shaderProgramObject_pv;
        int keypress;
        enum
        {
            AMC_ATTRIBUTE_POSITION = 0,
            AMC_ATTRIBUTE_COLOR,
            AMC_ATTRIBUTE_NORMAL,
            AMC_ATTRIBUTE_TEXTURE0
        };
        
        GLuint VOA;
        GLuint VBO;
        GLfloat angleSphere;
        GLuint vao_sphere;
        GLuint vbo_sphere_position;
        GLuint vbo_sphere_normal;
        GLuint vbo_sphere_indices;
        int numVertices;
        int numElements;

        GLuint projectionMatrixUniform_pf;
        GLuint modelMatrixUniform_pf;
        GLuint viewMatrixUniform_pf;
    
        GLuint projectionMatrixUniform_pv;
        GLuint modelMatrixUniform_pv;
        GLuint viewMatrixUniform_pv;
    
        mat4 perceptivegraphicsProjectionMatrix;
        /* LIGHTS */
        GLuint laUniform_pf;
        GLuint ldUniform_pf;
        GLuint lsUniform_pf;
        GLuint kaUniform_pf;
        GLuint kdUniform_pf;
        GLuint ksUniform_pf;
        GLuint materialShininessUniform_pf;
        GLuint lightPositionUniform_pf;
        GLuint LightingEnabledUniform_pf;
    
        GLuint laUniform_pv;
        GLuint ldUniform_pv;
        GLuint lsUniform_pv;
        GLuint kaUniform_pv;
        GLuint kdUniform_pv;
        GLuint ksUniform_pv;
        GLuint materialShininessUniform_pv;
        GLuint lightPositionUniform_pv;
        GLuint LightingEnabledUniform_pv;
    
        GLint status;
        GLint infoLogLen;
        char* log;
        GLuint vertexShaderObject_pf;
        GLuint fragmentShaderObject_pf;
        GLuint vertexShaderObject_pv;
        GLuint fragmentShaderObject_pv;
    
            
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

-(void) InitPerVertexShader_pf
{
    const GLchar* vertexShaderSourcecode =
            "#version 300 es" \
            "\n" \
            "\n" \
            "in vec4 a_position;" \
            "in vec3 a_normal;" \
            "out vec3 phongAdsColor;" \
            "uniform mat4 u_modelMatrix;" \
            "uniform mat4 u_viewMatrix;" \
            "uniform mat4 u_projectionMatrix;"
            "uniform mediump int LightingEnabled;" \
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
            "        mat3 normalMatrix = mat3(u_viewMatrix * u_modelMatrix);" \
            "       transformedNormal = mat3(u_viewMatrix * u_modelMatrix) * a_normal;" \
            "       lightDirection = vec3(u_lightPosition) - eyeCoordinates.xyz;" \
            "       viewVector = -eyeCoordinates.xyz;" \
            "   }" \
            "\n" \
            "   gl_Position = u_projectionMatrix * u_viewMatrix * u_modelMatrix * a_position;" \
            "}";

        

        

        // Creating shader object
        vertexShaderObject_pf = glCreateShader(GL_VERTEX_SHADER);

        // Giving shader code to shader object
        glShaderSource(vertexShaderObject_pf,
            1,
            (const GLchar**)&vertexShaderSourcecode,
            NULL);

        // Compile the shader
        glCompileShader(vertexShaderObject_pf);

        // Error Checking
        

        // a. Getting compilation status
        glGetShaderiv(vertexShaderObject_pf,
            GL_COMPILE_STATUS,
            &status);

        if (status == GL_FALSE)
        {
            // Getting length of log of compilation status
            glGetShaderiv(vertexShaderObject_pf,
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
                    glGetShaderInfoLog(vertexShaderObject_pf,
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
}


-(void) initPerFragment_pf
{
    const GLchar* fragmentShaderSourcecode =
        "#version 300 es" \
        "\n" \
        "precision highp float;" \
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

            "}"    \
            "else" \
            "{" \
                "phong_ads_color = vec3(1.0, 1.0, 1.0);" \
            "}" \
        "   fragColor = vec4(phong_ads_color, 1.0);" \
        "}";


    // 2. Creating shader object
    fragmentShaderObject_pf = glCreateShader(GL_FRAGMENT_SHADER);

    // 3. Giving shader code to shader object
    glShaderSource(fragmentShaderObject_pf,
        1,
        (const GLchar**)&fragmentShaderSourcecode,
        NULL);

    // 4. Compile the shader
    glCompileShader(fragmentShaderObject_pf);

    // 5. Error checking of shader compilation
    status = 0;
    infoLogLen = 0;
    log = NULL;

    // a. Getting compilation status
    glGetShaderiv(fragmentShaderObject_pf,
        GL_COMPILE_STATUS,
        &status);

    if (status == GL_FALSE)
    {
        // Getting length of log of compilation status
        glGetShaderiv(fragmentShaderObject_pf,
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
                glGetShaderInfoLog(fragmentShaderObject_pf,
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
}

-(void) InitShaderProgram_pf
{
    shaderProgramObject_pf = glCreateProgram();

    // 2. attach desire shaders to this shader program object
    glAttachShader(shaderProgramObject_pf, vertexShaderObject_pf);
    glAttachShader(shaderProgramObject_pf, fragmentShaderObject_pf);

    //  Pre-linking binding of shader program object with vertex attributes
    glBindAttribLocation(shaderProgramObject_pf,
        AMC_ATTRIBUTE_POSITION,
        "a_position");
    
    glBindAttribLocation(shaderProgramObject_pf,
            AMC_ATTRIBUTE_NORMAL,
            "a_normal");

    // 3. link shader program object
    glLinkProgram(shaderProgramObject_pf);

    // retriving/getting uniformed location from shader program object
    modelMatrixUniform_pf = glGetUniformLocation(shaderProgramObject_pf,
        "u_modelMatrix");

    viewMatrixUniform_pf = glGetUniformLocation(shaderProgramObject_pf,
        "u_viewMatrix");

    projectionMatrixUniform_pf = glGetUniformLocation(shaderProgramObject_pf,
        "u_projectionMatrix");
    laUniform_pf = glGetUniformLocation(shaderProgramObject_pf, "u_la");
    ldUniform_pf = glGetUniformLocation(shaderProgramObject_pf, "u_ld");
    lsUniform_pf = glGetUniformLocation(shaderProgramObject_pf, "u_ls");
    kaUniform_pf = glGetUniformLocation(shaderProgramObject_pf, "u_ka");
    kdUniform_pf = glGetUniformLocation(shaderProgramObject_pf, "u_kd");
    ksUniform_pf = glGetUniformLocation(shaderProgramObject_pf, "u_ks");
    lightPositionUniform_pf = glGetUniformLocation(shaderProgramObject_pf, "u_lightPosition");
    materialShininessUniform_pf = glGetUniformLocation(shaderProgramObject_pf, "u_materialShininess");

    LightingEnabledUniform_pf = glGetUniformLocation(shaderProgramObject_pf, "LightingEnabled");


    

    // 4. do link error checking with similar a to g steps like above
    status = 0;
    infoLogLen = 0;
    log = NULL;

    // a. Getting link status
    glGetProgramiv(shaderProgramObject_pf,
        GL_LINK_STATUS,
        &status);

    if (status == GL_FALSE)
    {
        // Getting length of log of LINK status
        glGetProgramiv(shaderProgramObject_pf,
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
                glGetProgramInfoLog(shaderProgramObject_pf,
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
}











-(void) InitPerVertexShader_pv
{
    const GLchar* vertexShaderSourcecode =
            "#version 300 es" \
            "\n" \
            "\n" \
            "in vec4 a_position;" \
            "in vec3 a_normal;" \
            "out vec3 phongAdsColor;" \
            "uniform mat4 u_modelMatrix;" \
            "uniform mat4 u_viewMatrix;" \
            "uniform mat4 u_projectionMatrix;"
            "uniform mediump int LightingEnabled;" \
            "out vec3 diffuse_light_color;" \
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
        vertexShaderObject_pv = glCreateShader(GL_VERTEX_SHADER);

        // Giving shader code to shader object
        glShaderSource(vertexShaderObject_pv,
            1,
            (const GLchar**)&vertexShaderSourcecode,
            NULL);

        // Compile the shader
        glCompileShader(vertexShaderObject_pv);

        // Error Checking
        

        // a. Getting compilation status
        glGetShaderiv(vertexShaderObject_pv,
            GL_COMPILE_STATUS,
            &status);

        if (status == GL_FALSE)
        {
            // Getting length of log of compilation status
            glGetShaderiv(vertexShaderObject_pv,
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
                    glGetShaderInfoLog(vertexShaderObject_pv,
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
}


-(void) initPerFragment_pv
{
    const GLchar* fragmentShaderSourcecode =
        "#version 300 es" \
        "\n" \
        "precision highp float;" \
        "in vec3 phongAdsColor;" \
        "\n" \
        "out vec4 fragmentColor;" \
        "\n" \
        "void main(void)" \
        "{" \
        "   fragmentColor = vec4(phongAdsColor, 1.0);" \
        "}";

    // 2. Creating shader object
    fragmentShaderObject_pv = glCreateShader(GL_FRAGMENT_SHADER);

    // 3. Giving shader code to shader object
    glShaderSource(fragmentShaderObject_pv,
        1,
        (const GLchar**)&fragmentShaderSourcecode,
        NULL);

    // 4. Compile the shader
    glCompileShader(fragmentShaderObject_pv);

    // 5. Error checking of shader compilation
    status = 0;
    infoLogLen = 0;
    log = NULL;

    // a. Getting compilation status
    glGetShaderiv(fragmentShaderObject_pv,
        GL_COMPILE_STATUS,
        &status);

    if (status == GL_FALSE)
    {
        // Getting length of log of compilation status
        glGetShaderiv(fragmentShaderObject_pv,
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
                glGetShaderInfoLog(fragmentShaderObject_pv,
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
}

-(void) InitShaderProgram_pv
{
    shaderProgramObject_pv = glCreateProgram();

    // 2. attach desire shaders to this shader program object
    glAttachShader(shaderProgramObject_pv, vertexShaderObject_pv);
    glAttachShader(shaderProgramObject_pv, fragmentShaderObject_pv);

    //  Pre-linking binding of shader program object with vertex attributes
    glBindAttribLocation(shaderProgramObject_pv,
        AMC_ATTRIBUTE_POSITION,
        "a_position");
    
    glBindAttribLocation(shaderProgramObject_pv,
            AMC_ATTRIBUTE_NORMAL,
            "a_normal");

    // 3. link shader program object
    glLinkProgram(shaderProgramObject_pv);

    // retriving/getting uniformed location from shader program object
    modelMatrixUniform_pv = glGetUniformLocation(shaderProgramObject_pv,
        "u_modelMatrix");

    viewMatrixUniform_pv = glGetUniformLocation(shaderProgramObject_pv,
        "u_viewMatrix");

    projectionMatrixUniform_pv = glGetUniformLocation(shaderProgramObject_pv,
        "u_projectionMatrix");
    laUniform_pv = glGetUniformLocation(shaderProgramObject_pv, "u_la");
    ldUniform_pv = glGetUniformLocation(shaderProgramObject_pv, "u_ld");
    lsUniform_pv = glGetUniformLocation(shaderProgramObject_pv, "u_ls");
    kaUniform_pv = glGetUniformLocation(shaderProgramObject_pv, "u_ka");
    kdUniform_pv = glGetUniformLocation(shaderProgramObject_pv, "u_kd");
    ksUniform_pv = glGetUniformLocation(shaderProgramObject_pv, "u_ks");
    lightPositionUniform_pv = glGetUniformLocation(shaderProgramObject_pv, "u_lightPosition");
    materialShininessUniform_pv = glGetUniformLocation(shaderProgramObject_pv, "u_materialShininess");

    LightingEnabledUniform_pv = glGetUniformLocation(shaderProgramObject_pv, "LightingEnabled");


    

    // 4. do link error checking with similar a to g steps like above
    status = 0;
    infoLogLen = 0;
    log = NULL;

    // a. Getting link status
    glGetProgramiv(shaderProgramObject_pv,
        GL_LINK_STATUS,
        &status);

    if (status == GL_FALSE)
    {
        // Getting length of log of LINK status
        glGetProgramiv(shaderProgramObject_pv,
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
                glGetProgramInfoLog(shaderProgramObject_pv,
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
}




- (int)initialize
{
    //code
    keypress = 2;
    [self InitPerVertexShader_pf];
    [self initPerFragment_pf ];
    [self InitShaderProgram_pf];
    
    [self InitPerVertexShader_pv];
    [self initPerFragment_pv ];
    [self InitShaderProgram_pv];
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

    
    glClearDepthf(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    
    
    printf("In init");
    
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    perceptivegraphicsProjectionMatrix = mat4::identity();
    return 0;
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

-(void)drawPerFragment
{
    glUseProgram(shaderProgramObject_pf);
    printf("display");
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
        rotationMatrix_y = vmath::rotate(angleSphere, 0.0f, 1.0f, 0.0f);
        rotationMatrix =   rotationMatrix_y ;
        modelMatrix = translationMatrix * scaleMatrix * rotationMatrix;    // Order is very important
        modelViewProjectionMatrix = perceptivegraphicsProjectionMatrix * modelMatrix;

        // send above transformation matrixes to shader in respective matrix uniform
        glUniformMatrix4fv(modelMatrixUniform_pf,
            1,
            GL_FALSE,
            modelMatrix);

        glUniformMatrix4fv(viewMatrixUniform_pf,
            1,
            GL_FALSE,
            viewMatrix);

        glUniformMatrix4fv(projectionMatrixUniform_pf,
            1,
            GL_FALSE,
            perceptivegraphicsProjectionMatrix);


        /* Sending LIGHT realed Unifrom */
        if (bLight == true)
        {
                    glUniform1i(LightingEnabledUniform_pf, 1);
                    glUniform3fv(laUniform_pf, 1, lightAmbient_PerFrag);
                    glUniform3fv(ldUniform_pf, 1, lightDiffuse_PerFrag);
                    glUniform3fv(lsUniform_pf, 1, lightSpecular_PerFrag);
                    glUniform4fv(lightPositionUniform_pf, 1, lightPosition_PerFrag);

                    glUniform3fv(kaUniform_pf, 1, materialAmbient_PerFrag);
                    glUniform3fv(kdUniform_pf, 1, materialDiffuse_PerFrag);
                    glUniform3fv(ksUniform_pf, 1, materialSpecular_PerFrag);
                    glUniform1f(materialShininessUniform_pf, materialShininess_PerFrag);

        }
        else
        {
            glUniform1i(LightingEnabledUniform_pf, 0);
        }
        // bind with vertex array object
        glBindVertexArray(vao_sphere);

        // draw
    glDrawElements(GL_TRIANGLES, numElements, GL_UNSIGNED_SHORT, 0);

        

        glBindVertexArray(0);


    glUseProgram(0);

}



-(void)drawPerVertex
{
    glUseProgram(shaderProgramObject_pv);
    printf("display");
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
        rotationMatrix_y = vmath::rotate(angleSphere, 0.0f, 1.0f, 0.0f);
        rotationMatrix =   rotationMatrix_y ;
        modelMatrix = translationMatrix * scaleMatrix * rotationMatrix;    // Order is very important
        modelViewProjectionMatrix = perceptivegraphicsProjectionMatrix * modelMatrix;

        // send above transformation matrixes to shader in respective matrix uniform
        glUniformMatrix4fv(modelMatrixUniform_pv,
            1,
            GL_FALSE,
            modelMatrix);

        glUniformMatrix4fv(viewMatrixUniform_pv,
            1,
            GL_FALSE,
            viewMatrix);

        glUniformMatrix4fv(projectionMatrixUniform_pv,
            1,
            GL_FALSE,
            perceptivegraphicsProjectionMatrix);


        /* Sending LIGHT realed Unifrom */
        if (bLight == true)
        {
                    glUniform1i(LightingEnabledUniform_pv, 1);
                    glUniform3fv(laUniform_pv, 1, lightAmbient_PerVertex);
                    glUniform3fv(ldUniform_pv, 1, lightDiffuse_PerVertex);
                    glUniform3fv(lsUniform_pv, 1, lightSpecular_PerVertex);
                    glUniform4fv(lightPositionUniform_pv, 1, lightPosition_PerVertex);

                    glUniform3fv(kaUniform_pv, 1, materialAmbient_PerVertex);
                    glUniform3fv(kdUniform_pv, 1, materialDiffuse_PerVertex);
                    glUniform3fv(ksUniform_pv, 1, materialSpecular_PerVertex);
                    glUniform1f(materialShininessUniform_pv, materialShininess_PerVertex);

        }
        else
        {
            glUniform1i(LightingEnabledUniform_pv, 0);
        }
        // bind with vertex array object
        glBindVertexArray(vao_sphere);

        // draw
    glDrawElements(GL_TRIANGLES, numElements, GL_UNSIGNED_SHORT, 0);

        

        glBindVertexArray(0);


    glUseProgram(0);

}

- (void)display
{
    //code
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    if(keypress == 2)
    {
        [self drawPerFragment];
    }
    else  if(keypress == 1)
    {
        [self drawPerVertex];
    }
}

- (void)update
{
    //code
    angleSphere += 0.5f;
        if (angleSphere >= 360.f)
        {
            angleSphere = angleSphere - 360.0f;
        }
}

- (void)uninitialize
{
    //code
        if (vao_sphere)
        {
            glDeleteBuffers(1, &vao_sphere);
            vao_sphere = 0;
        }

    if (vbo_sphere_normal)
    {
        glDeleteBuffers(1, &vbo_sphere_normal);
        vbo_sphere_normal = 0;
    }


       
        
        // shader uninitialise
        if (shaderProgramObject_pf)
        {
            // 0. again used sharderprogramobject
            glUseProgram(shaderProgramObject_pf);

            // 1. Get Number of attach shaders
            GLsizei numAttchedShaders;
            glGetProgramiv(shaderProgramObject_pf,
                GL_ATTACHED_SHADERS,
                &numAttchedShaders);

            // 2. Create empty buffer to hold array of attach shader objects
            GLuint* shaderObjects = NULL;
            // 3. allocate enough memroy to this according to number of attach shaders and fill it with attachedshaderobject
            shaderObjects = (GLuint*)malloc(numAttchedShaders*sizeof(GLuint));

            glGetAttachedShaders(shaderProgramObject_pf,
                numAttchedShaders,
            (GLsizei*)    &shaderProgramObject_pf,
                shaderObjects);

            // 4. As number of attach shaders more than 1 start a loop and inside loop deattach, delete shader one by one
            for (GLsizei i = 0; i < numAttchedShaders; i++)
            {
                glDetachShader(shaderProgramObject_pf, shaderObjects[i]);
                glDeleteShader(shaderObjects[i]);
                shaderObjects[i] = 0;
            }

            // 5. free memory allocated for buffer
            free(shaderObjects);
            shaderObjects = NULL;

            // 6. unused sharderprogramobject
            glUseProgram(0);

            // 7. delete sharderprogramobject
            glDeleteProgram(shaderProgramObject_pf);
            shaderProgramObject_pf = 0;
        
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
            //keypress = 1;
}

- (void) onDoubleTap:(UITapGestureRecognizer*)gr
{
    //code
    if (keypress == 1)
    {
        keypress = 2;
    }
    else
    {
        keypress = 1;
    }
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
    //keypress = 2;
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
