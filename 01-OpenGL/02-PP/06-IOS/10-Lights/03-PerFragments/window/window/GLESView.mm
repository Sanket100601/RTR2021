

#import "GLESView.h"
#import "Sphere.h"

#import <OpenGLES/ES3/gl.h>
#import <OpenGLES/ES3/glext.h>
#import "vmath.h"

using  namespace vmath;

int bLight;
GLfloat lightAmbient[] = { 0.0f,0.0f,0.0f,1.0f };
GLfloat lightDiffuse[] = { 1.0f,1.0f,1.0f,1.0f };
GLfloat lightSpecular[] = { 1.0f,1.0f,1.0f,1.0f };
GLfloat lightPosition[] = { 100.0f,100.0f,100.0f,1.0f };

GLfloat materialAmbient[] = { 0.0f,0.0f,0.0f,1.0f };
GLfloat materialDiffuse[] = { 1.0f,1.0f,1.0f,1.0f };
GLfloat materialSpecular[] = { 1.0f,1.0f,1.0f,1.0f };
GLfloat materialShininess = 50.0f;

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
        
        GLuint VOA;
        GLuint VBO;
        GLfloat angleSphere;
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
                    printf("Shader Program Link Log : %s\n", log);

                    // Free the allocated the buffer.
                    free(log);

                    // exit the application due to error
                    [self uninitialize];
                    [self release];
                   

                }

            }
        }
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

        translationMatrix = vmath::translate(0.0f, 0.0f, -3.0f);
        //scaleMatrix = vmath::scale(0.75f, 0.75f, 0.75f);
        rotationMatrix_y = vmath::rotate(angleSphere, 0.0f, 1.0f, 0.0f);
        rotationMatrix =   rotationMatrix_y ;
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
        // bind with vertex array object
        glBindVertexArray(vao_sphere);

        // draw
    glDrawElements(GL_TRIANGLES, numElements, GL_UNSIGNED_SHORT, 0);

        

        glBindVertexArray(0);


    glUseProgram(0);

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