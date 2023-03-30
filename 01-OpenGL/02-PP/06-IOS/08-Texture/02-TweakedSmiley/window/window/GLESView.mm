

#import "GLESView.h"

#import <OpenGLES/ES3/gl.h>
#import <OpenGLES/ES3/glext.h>
#import "vmath.h"

using  namespace vmath;

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
        GLuint VBO_position;
        GLuint VBO_texcoords;
        GLuint texture_smiley;
        GLuint textureSamplerUniform;
        GLuint mvpMatrixUniform;
        mat4 perceptivegraphicsProjectionMatrix;
        GLuint keypressedUniform;
        int keypress;
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
    keypress = -1;
    const GLchar* vertexShaderSourcecode =
            "#version 300 es" \
            "\n" \
            "in vec4 a_position;" \
            "in vec2 a_texcoord;" \
            "uniform mat4 u_mvpMatrix;" \
            "out vec2 a_texcoord_out;" \
            "void main(void)" \
            "{" \
            "    gl_Position = u_mvpMatrix * a_position;" \
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
            "in vec2 a_texcoord_out;" \
            "uniform sampler2D u_textureSampler;" \
            "uniform int u_keypress;" \
            "out vec4 FragColor;" \
            "void main(void)" \
            "{" \
                "FragColor=texture(u_textureSampler, a_texcoord_out);" \
            "if(u_keypress == 1)" \
            "{" \
            "FragColor=texture(u_textureSampler, a_texcoord_out);" \
            "}" \
            "else" \
            "{" \
            "FragColor=vec4(1.0f, 1.0f, 1.0f, 1.0f);" \
            "}" \
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
            AMC_ATTRIBUTE_TEXTURE0,
            "a_texcoord");
    
        // 3. link shader program object
        glLinkProgram(shaderProgramObject);

        // retriving/getting uniformed location from shader program object
        mvpMatrixUniform = glGetUniformLocation(shaderProgramObject,
            "u_mvpMatrix");

        textureSamplerUniform = glGetUniformLocation(shaderProgramObject,
            "u_textureSampler");
        
        keypressedUniform = glGetUniformLocation(shaderProgramObject,
            "u_keypress");
    
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
    const GLfloat position[] = {
                        1.0f, 1.0f, 0.0f,
                        -1.0f, 1.0f, 0.0f,
                        -1.0f, -1.0f,0.0f,
                        1.0f, -1.0f, 0.0f
        };

    
         /* VOA Reactangle */
        glGenVertexArrays(1,
            &VOA);

        // create vertex array object
        glBindVertexArray(VOA);
        // create vertex data buffer - VBO for position
        glGenBuffers(1, &VBO_position);

        // bind with vertex data buffer
        glBindBuffer(GL_ARRAY_BUFFER, VBO_position);

        // create storage of buffer data for perticular target
        glBufferData(GL_ARRAY_BUFFER,
            sizeof(position),
            position,
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


        // VBO for texcoord
        glGenBuffers(1, &VBO_texcoords);

        glBindBuffer(GL_ARRAY_BUFFER, VBO_texcoords);

        glBufferData(GL_ARRAY_BUFFER,
            4*2*sizeof(GL_FLOAT), // 4 rows of 2 cols
            NULL,
            GL_DYNAMIC_DRAW);

        glVertexAttribPointer(AMC_ATTRIBUTE_TEXTURE0,
            2,
            GL_FLOAT,
            GL_FALSE,
            0,
            NULL);

        glEnableVertexAttribArray(AMC_ATTRIBUTE_TEXTURE0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glBindVertexArray(0);


    
    
    glClearDepthf(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    
    texture_smiley = [self LoadGlTexture : @"Smiley" : @"bmp"];
    if(!texture_smiley)
    {
        printf("Error : failed to load smiley.bmp texture.\n");
        [self uninitialize];
        [self release];
    }
    
    glEnable(GL_TEXTURE_2D);
    
    glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
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

        GLfloat texcoord[8];
        mat4 translationMatrix = mat4::identity();
        mat4 modelViewMatrix = mat4::identity();
        mat4 modelViewProjectionMatrix = mat4::identity();

        translationMatrix = vmath::translate(0.0f, 0.0f, -6.0f);
        
        modelViewMatrix = translationMatrix;    // Order is very important
        modelViewProjectionMatrix = perceptivegraphicsProjectionMatrix * modelViewMatrix;

        // send above transformation matrixes to shader in respective matrix uniform
        glUniformMatrix4fv(mvpMatrixUniform,
            1,
            GL_FALSE,
            modelViewProjectionMatrix);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture_smiley);
        glUniform1i(textureSamplerUniform, 0);

        // bind with vertex array object
        glBindVertexArray(VOA);
        if (keypress == 1)
        {
            texcoord[0] = 0.5f;
            texcoord[1] = 0.5f;

            texcoord[2] = 0.0f;
            texcoord[3] = 0.5f;

            texcoord[4] = 0.0f;
            texcoord[5] = 0.0f;

            texcoord[6] = 0.5f;
            texcoord[7] = 0.0f;
            glUniform1i(keypressedUniform, 1);
        }
        else if (keypress == 2)
        {
            texcoord[0] = 1.0f;
            texcoord[1] = 1.0f;

            texcoord[2] = 0.0f;
            texcoord[3] = 1.0f;

            texcoord[4] = 0.0f;
            texcoord[5] = 0.0f;

            texcoord[6] = 1.0f;
            texcoord[7] = 0.0f;
            glUniform1i(keypressedUniform, 1);
        }
        else if (keypress == 3)
        {
            texcoord[0] = 2.0f;
            texcoord[1] = 2.0f;

            texcoord[2] = 0.0f;
            texcoord[3] = 2.0f;

            texcoord[4] = 0.0f;
            texcoord[5] = 0.0f;

            texcoord[6] = 2.0f;
            texcoord[7] = 0.0f;
            glUniform1i(keypressedUniform, 1);
        }
        else if (keypress == 4)
        {
            texcoord[0] = 0.5f;
            texcoord[1] = 0.5f;

            texcoord[2] = 0.5f;
            texcoord[3] = 0.5f;

            texcoord[4] = 0.5f;
            texcoord[5] = 0.5f;

            texcoord[6] = 0.5f;
            texcoord[7] = 0.5f;
            glUniform1i(keypressedUniform, 1);
        }
        else
        {
            glUniform1i(keypressedUniform, 0);
        }
        glBindBuffer(GL_ARRAY_BUFFER, VBO_texcoords);
        glBufferData(GL_ARRAY_BUFFER,
            sizeof(texcoord),
            texcoord,
            GL_DYNAMIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        
        // draw
        glDrawArrays(GL_TRIANGLE_FAN,
            0,
            4);

        glBindVertexArray(0);
        
    glUseProgram(0);

}

- (void)update
{
    //code
    
}

- (void)uninitialize
{
    //code
        if(VBO_texcoords)
        {
            glDeleteBuffers(1, &VBO_texcoords);
            VBO_texcoords = 0;
        }
        if (VBO_position)
        {
            glDeleteBuffers(1, &VBO_position);
            VBO_position = 0;
        }

        if (VOA)
        {
            glDeleteVertexArrays(1, &VOA);
            VOA = 0;
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
    keypress = 1;
    
}

- (void) onDoubleTap:(UITapGestureRecognizer*)gr
{
    //code
    keypress = 2;
    
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
    keypress = 3;
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
