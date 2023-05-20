package com.tejas.window;


import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.opengl.GLES20;
import android.opengl.GLES32;
import android.opengl.GLSurfaceView;
import android.opengl.GLUtils;
import android.opengl.Matrix;
import android.view.GestureDetector;
import android.view.GestureDetector.OnDoubleTapListener;
import android.view.GestureDetector.OnGestureListener;
import android.view.MotionEvent;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.FloatBuffer;
import java.nio.ShortBuffer;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

public class GLESView extends GLSurfaceView implements OnDoubleTapListener, OnGestureListener, GLSurfaceView.Renderer {

    private GestureDetector gestureDetector;
    private Context context;
    private int shaderProgramObject;

    // rotation
    private float angleCube = 0.0f;

    // fbo related
    public static final int FBO_WIDTH = 512;
    public static final int FBO_HEIGHT = 512;

    private int fbo[] = new int[1];
    private int rbo[] = new int[1];

    private int fbo_texture[] = new int[1];

    private int winWidth_fbo;
    private int winHeight_fbo;

    private boolean bFBOResult;
    // cube related
    private int vao_cube[] = new int[1];  // vao send as address hence array of 1 element
    private int vbo_cube_position[] = new int[1];
    private int vbo_cube_texcoord[] = new int[1];
    private int mvpMatrixUniform;
    private int textureSamplerUniform;

    private float perspectiveProjectionMatrix[] = new float[16];  // mat4 --> 4 X 4 Matrix hence 16 array size



    // ********** FBO TEXTURE VARIABLES ***********
    private int shaderProgramObject_pv;
    private int shaderProgramObject_pf;


    // sphere
    private int vao_sphere[] = new int[1];
    private int vbo_sphere_position[] = new int[1];
    private int vbo_sphere_normal[] = new int[1];
    private int vbo_sphere_element[] = new int[1];

    private int numElements;
    private int numVertices;


    private int modelMatrixUniform_pf;
    private int viewMatrixUniform_pf;
    private int projectionMatrixUniform_pf;

    private int laUniform_pf[] = new int[3]; // light ambient
    private int ldUniform_pf[] = new int[3]; //  light diffuse
    private int lsUniform_pf[] = new int[3]; // light specular
    private int lightPositionUniform_pf[] = new int[3];

    private int kaUniform_pf;
    private int kdUniform_pf;
    private int ksUniform_pf;
    private int materialShininessUniform_pf;

    private int modelMatrixUniform_pv;
    private int viewMatrixUniform_pv;
    private int projectionMatrixUniform_pv;

    private int laUniform_pv[] = new int[3]; // light ambient
    private int ldUniform_pv[] = new int[3]; //  light diffuse
    private int lsUniform_pv[] = new int[3]; // light specular
    private int lightPositionUniform_pv[] = new int[3];

    private int kaUniform_pv;
    private int kdUniform_pv;
    private int ksUniform_pv;
    private int materialShininessUniform_pv;

    private int lightEnabledUniform_pv;
    private int lightEnabledUniform_pf;


    private int doubleTapSphere = 0;  // bLight in windows


    private float materialAmbientSphere[] = new float[]{0.0f, 0.0f, 0.0f, 1.0f};
    private float materialDiffuseSphere[] = new float[]{0.5f, 0.2f, 0.7f, 1.0f};
    private float materialSpecularSphere[] = new float[]{1.0f, 1.0f, 1.0f, 1.0f};
    private float materialShininessSphere = 50.0f;

    // rotation
    private float perspectiveProjectionMatrixSphere[] = new float[16];  // mat4 --> 4 X 4 Matrix hence 16 array size

    double angleLightSphere = 0.0;
    // pv pf related
    private static final int PER_VERTEX = 0;
    private static final int PER_FRAGMENT = 1;

    private int choosenShaderSphere = PER_VERTEX;

    SphereLights lights[] = new SphereLights[3];


    GLESView(Context _context) {
        super(_context);
        context = _context;

        setEGLContextClientVersion(3); // 3.0 version
        setRenderer(this);
        setRenderMode(GLSurfaceView.RENDERMODE_WHEN_DIRTY); // invalidate rect

        // gesture related code
        // GestureDetector(Context context, GestureDetector.OnGestureListener listener, Handler handler, boolean unused)
        gestureDetector = new GestureDetector(context, this, null, false);
        gestureDetector.setOnDoubleTapListener(this);
    }

    // OpenGL related methods (GLSurfaceView.Renderer)

    // initialize() in Win32
    @Override
    public void onSurfaceCreated(GL10 gl, EGLConfig config) {
        String glesVersion = gl.glGetString(GL10.GL_VERSION);
        String renderer = gl.glGetString(GL10.GL_RENDERER);
        String glslVersion = gl.glGetString(GLES32.GL_SHADING_LANGUAGE_VERSION);

        System.out.println("TMShinde:" + glesVersion);
        System.out.println("TMShinde:" + renderer);

        initialize();
    }

    // resize() in Win32
    @Override
    public void onSurfaceChanged(GL10 unused, int width, int height) {
        resize(width, height);
    }

    // considered as gameloop
    @Override
    public void onDrawFrame(GL10 unused) {
        display();
        update();
    }

    @Override
    public boolean onTouchEvent(MotionEvent e) {
        // code
        if (!gestureDetector.onTouchEvent(e)) {
            super.onTouchEvent(e);
        }
        return true;
    }

    // Three Methods of onDoubleTapListener interface

    @Override
    public boolean onDoubleTap(MotionEvent e) {
        doubleTapSphere++;
        if (doubleTapSphere > 1) {
            doubleTapSphere = 0;
        }
        return true;
    }

    @Override
    public boolean onDoubleTapEvent(MotionEvent e)  // not reliable than above but handled
    {
        return true;
    }

    @Override
    public boolean onSingleTapConfirmed(MotionEvent e) {
        if (choosenShaderSphere == PER_VERTEX) {
            choosenShaderSphere = PER_FRAGMENT;
        } else {
            choosenShaderSphere = PER_VERTEX;
        }
        return true;
    }

    // Six Method of OnGestureListener

    @Override
    public boolean onDown(MotionEvent e) {
        return true;
    }

    @Override
    public boolean onFling(MotionEvent e1, MotionEvent e2, float velocityX, float velocityY) {
        return true;
    }

    @Override
    public void onLongPress(MotionEvent e) {
    }

    // swipe == scroll
    @Override
    public boolean onScroll(MotionEvent e1, MotionEvent e2, float distanceX, float distanceY) {
        uninitialize();
        System.exit(0);
        return true;
    }

    @Override
    public void onShowPress(MotionEvent e) {
        // code
    }

    @Override
    public boolean onSingleTapUp(MotionEvent e) {
        return true;
    }

    // custom private functions

    private void initialize() {
        System.out.println("TMShinde: Inside initialize()" );

        // code
        // vertex shader
        final String vertexShaderSourceCode = String.format
                (
                        "#version 320 es" +
                                "\n" +
                                "in vec4 a_position;" +
                                "in vec2 a_texcoord;" +
                                "uniform mat4 u_mvpMatrix;" +
                                "out vec2 a_texcoord_out;" +
                                "void main(void)" +
                                "{" +
                                "gl_Position = u_mvpMatrix * a_position;" +
                                "a_texcoord_out = a_texcoord;" +
                                "}"
                );
        int vertexShaderObject = GLES32.glCreateShader(GLES32.GL_VERTEX_SHADER);


        GLES32.glShaderSource(vertexShaderObject, vertexShaderSourceCode);
        GLES32.glCompileShader(vertexShaderObject);

        int status[] = new int[1];
        int infoLogLength[] = new int[1];
        String log = null;
        GLES32.glGetShaderiv(vertexShaderObject, GLES32.GL_COMPILE_STATUS, status, 0);

        if (status[0] == GLES32.GL_FALSE) {
            GLES32.glGetShaderiv(vertexShaderObject, GLES32.GL_INFO_LOG_LENGTH, infoLogLength, 0);
            if (infoLogLength[0] > 0) {
                log = GLES32.glGetShaderInfoLog(vertexShaderObject);
                System.out.println("TMShinde: VERTEX SHADER COMPILATION LOG : \n" + log);
                uninitialize();
                System.exit(0);
            }
        }

        // Fragment Shader
        final String fragmentShaderSourceCode = String.format
                (
                        "#version 320 es" +
                                "\n" +
                                "precision highp float;" +
                                "in vec2 a_texcoord_out;" +
                                "uniform sampler2D u_textureSampler;" +
                                "out vec4 FragColor;" +
                                "void main(void)" +
                                "{" +
                                "FragColor = texture(u_textureSampler,a_texcoord_out);" +
                                "}"
                );
        int fragmentShaderObject = GLES32.glCreateShader(GLES32.GL_FRAGMENT_SHADER);


        GLES32.glShaderSource(fragmentShaderObject, fragmentShaderSourceCode);
        GLES32.glCompileShader(fragmentShaderObject);

        status[0] = 0;
        infoLogLength[0] = 0;
        log = null;
        GLES32.glGetShaderiv(fragmentShaderObject, GLES32.GL_COMPILE_STATUS, status, 0);

        if (status[0] == GLES32.GL_FALSE) {
            GLES32.glGetShaderiv(fragmentShaderObject, GLES32.GL_INFO_LOG_LENGTH, infoLogLength, 0);
            if (infoLogLength[0] > 0) {
                log = GLES32.glGetShaderInfoLog(fragmentShaderObject);
                System.out.println("TMShinde: FRAGMENT SHADER COMPILATION LOG : \n" + log);
                uninitialize();
                System.exit(0);
            }
        }

        // shader program
        shaderProgramObject = GLES32.glCreateProgram();

        GLES32.glAttachShader(shaderProgramObject, vertexShaderObject);
        GLES32.glAttachShader(shaderProgramObject, fragmentShaderObject);

        // pre link
        GLES32.glBindAttribLocation(shaderProgramObject, MyGLESMacros.AMC_ATTRIBUTE_POSITION, "a_position");
        GLES32.glBindAttribLocation(shaderProgramObject, MyGLESMacros.AMC_ATTRIBUTE_TEXTURE0, "a_texcoord");
        GLES32.glLinkProgram(shaderProgramObject);

        status[0] = 0;
        infoLogLength[0] = 0;
        log = null;

        GLES32.glGetProgramiv(shaderProgramObject, GLES32.GL_LINK_STATUS, status, 0);
        if (status[0] == GLES32.GL_FALSE) {
            GLES32.glGetProgramiv(shaderProgramObject, GLES32.GL_INFO_LOG_LENGTH, infoLogLength, 0);
            if (infoLogLength[0] > 0) {
                log = GLES32.glGetProgramInfoLog(shaderProgramObject);
                System.out.println("TMShinde:  SHADER PROGRAM LINK LOG  : \n" + log);
                uninitialize();
                System.exit(0);
            }
        }

        // post link
        mvpMatrixUniform = GLES32.glGetUniformLocation(shaderProgramObject, "u_mvpMatrix");
        textureSamplerUniform = GLES32.glGetUniformLocation(shaderProgramObject, "u_textureSampler");

        final float cubePosition[] = new float[]
                {
                        // top
                        1.0f, 1.0f, -1.0f,
                        -1.0f, 1.0f, -1.0f,
                        -1.0f, 1.0f, 1.0f,
                        1.0f, 1.0f, 1.0f,

                        // bottom
                        1.0f, -1.0f, -1.0f,
                        -1.0f, -1.0f, -1.0f,
                        -1.0f, -1.0f, 1.0f,
                        1.0f, -1.0f, 1.0f,

                        // front
                        1.0f, 1.0f, 1.0f,
                        -1.0f, 1.0f, 1.0f,
                        -1.0f, -1.0f, 1.0f,
                        1.0f, -1.0f, 1.0f,

                        // back
                        1.0f, 1.0f, -1.0f,
                        -1.0f, 1.0f, -1.0f,
                        -1.0f, -1.0f, -1.0f,
                        1.0f, -1.0f, -1.0f,

                        // right
                        1.0f, 1.0f, -1.0f,
                        1.0f, 1.0f, 1.0f,
                        1.0f, -1.0f, 1.0f,
                        1.0f, -1.0f, -1.0f,

                        // left
                        -1.0f, 1.0f, 1.0f,
                        -1.0f, 1.0f, -1.0f,
                        -1.0f, -1.0f, -1.0f,
                        -1.0f, -1.0f, 1.0f

                };

        final float cubeTexcoord[] = new float[]
                {
                        0.0f, 0.0f,
                        0.0f, 1.0f, // top
                        1.0f, 1.0f,
                        1.0f, 0.0f,

                        1.0f, 1.0f,
                        0.0f, 1.0f,
                        0.0f, 0.0f,
                        1.0f, 0.0f, // bottom

                        1.0f, 0.0f,
                        0.0f, 0.0f,
                        0.0f, 1.0f,
                        1.0f, 1.0f, // front

                        1.0f, 0.0f,
                        0.0f, 0.0f,
                        0.0f, 1.0f,
                        1.0f, 1.0f,

                        1.0f, 1.0f,
                        1.0f, 0.0f,
                        0.0f, 0.0f,
                        0.0f, 1.0f, // right

                        1.0f, 0.0f,
                        0.0f, 0.0f,
                        0.0f, 1.0f,
                        1.0f, 1.0f


                };


        // ************CUBE
        GLES32.glGenVertexArrays(1, vao_cube, 0);
        GLES32.glBindVertexArray(vao_cube[0]);

        GLES32.glGenBuffers(1, vbo_cube_position, 0);
        GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, vbo_cube_position[0]);
        // buffer data 3rd param code
        // 4 is of float size
        ByteBuffer byteBuffer = ByteBuffer.allocateDirect(cubePosition.length * 4);
        byteBuffer.order(ByteOrder.nativeOrder());
        FloatBuffer positionBuffer = byteBuffer.asFloatBuffer();
        positionBuffer.put(cubePosition);
        positionBuffer.position(0); // its array start position do not change to color

        GLES32.glBufferData(GLES32.GL_ARRAY_BUFFER, cubePosition.length * 4, positionBuffer, GLES32.GL_STATIC_DRAW);
        GLES32.glVertexAttribPointer(MyGLESMacros.AMC_ATTRIBUTE_POSITION, 3, GLES32.GL_FLOAT, false, 0, 0);
        GLES32.glEnableVertexAttribArray(MyGLESMacros.AMC_ATTRIBUTE_POSITION);

        GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, 0);

        // ************************** TEXTURE *******************
        GLES32.glGenBuffers(1, vbo_cube_texcoord, 0);
        GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, vbo_cube_texcoord[0]);
        // buffer data 3rd param code
        // 4 is of float size
        byteBuffer = ByteBuffer.allocateDirect(cubeTexcoord.length * 4);
        byteBuffer.order(ByteOrder.nativeOrder());
        FloatBuffer textureBuffer = byteBuffer.asFloatBuffer();
        textureBuffer.put(cubeTexcoord);
        textureBuffer.position(0); // its array start position do not change to color

        GLES32.glBufferData(GLES32.GL_ARRAY_BUFFER, cubeTexcoord.length * 4, textureBuffer, GLES32.GL_STATIC_DRAW);
        GLES32.glVertexAttribPointer(MyGLESMacros.AMC_ATTRIBUTE_TEXTURE0, 2, GLES32.GL_FLOAT, false, 0, 0);
        GLES32.glEnableVertexAttribArray(MyGLESMacros.AMC_ATTRIBUTE_TEXTURE0);

        GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, 0);
        GLES32.glBindVertexArray(0);

        GLES32.glClearDepthf(1.0f);
        GLES32.glEnable(GLES32.GL_DEPTH_TEST);
        GLES32.glDepthFunc(GLES32.GL_LEQUAL);

        // enable culling
        // GLES32.glEnable(GLES32.GL_CULL_FACE);

        GLES32.glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        GLES32.glEnable(GLES32.GL_TEXTURE_2D);

        // initialization of projection matrix
        Matrix.setIdentityM(perspectiveProjectionMatrix, 0);
        bFBOResult = createFBO(FBO_WIDTH,FBO_WIDTH);
        if(bFBOResult == true)
        {
            initializeSphere();
        }
        else{
            System.out.println("TMShinde:  create FBO function failed  ,can not initialize sphere!!\n");
        }
    }

    private boolean createFBO(int textureWidth, int textureHeight) {
        // code
        System.out.println("TMShinde: Inside createFBO()" );
        int maxRenderbufferSize[] = new int[1];
        ;
        GLES32.glGetIntegerv(GLES32.GL_MAX_RENDERBUFFER_SIZE, maxRenderbufferSize, 0);
        if (maxRenderbufferSize[0] < textureWidth || maxRenderbufferSize[0] < textureHeight) {
            System.out.println("TMShinde:  Insufficient render buffersize!!\n");
            return false;
        }

        GLES32.glGenBuffers(1, fbo, 0);
        GLES32.glBindTexture(GLES32.GL_TEXTURE_2D, fbo_texture[0]);
        GLES32.glTexParameteri(GLES32.GL_TEXTURE_2D, GLES32.GL_TEXTURE_WRAP_S, GLES32.GL_CLAMP_TO_EDGE);
        GLES32.glTexParameteri(GLES32.GL_TEXTURE_2D, GLES32.GL_TEXTURE_WRAP_T, GLES32.GL_CLAMP_TO_EDGE);
        GLES32.glTexParameteri(GLES32.GL_TEXTURE_2D, GLES32.GL_TEXTURE_MAG_FILTER, GLES32.GL_LINEAR);
        GLES32.glTexParameteri(GLES32.GL_TEXTURE_2D, GLES32.GL_TEXTURE_MIN_FILTER, GLES32.GL_LINEAR);
        GLES32.glTexImage2D(GLES32.GL_TEXTURE_2D, 0, GLES32.GL_RGB, textureWidth, textureHeight, 0, GLES32.GL_RGB, GLES32.GL_UNSIGNED_SHORT_5_6_5, null);
        GLES32.glFramebufferTexture2D(GLES32.GL_FRAMEBUFFER,GLES32.GL_COLOR_ATTACHMENT0, GLES32.GL_TEXTURE_2D,fbo_texture[0],0);
        GLES32.glFramebufferRenderbuffer(GLES32.GL_FRAMEBUFFER,GLES32.GL_DEPTH_ATTACHMENT,GLES20.GL_RENDERBUFFER,rbo[0]);

        int result =  GLES32.glCheckFramebufferStatus(GLES20.GL_FRAMEBUFFER);
        if(result != GLES32.GL_FRAMEBUFFER_COMPLETE)
        {
            System.out.println("TMShinde:  Framebuffer is not complete !!\n");
            return false;
        }
        GLES32.glBindFramebuffer(GLES32.GL_FRAMEBUFFER,0);
        return true;
    }

    private void initializeSphere() {

        // code
        System.out.println("TMShinde: Inside initializeSphere()" );
        /*
                                           | |
          _ __   ___ _ __  __   _____ _ __| |_ _____  __
         | '_ \ / _ \ '__| \ \ / / _ \ '__| __/ _ \ \/ /
         | |_) |  __/ |     \ V /  __/ |  | ||  __/>  <
         | .__/ \___|_|      \_/ \___|_|   \__\___/_/\_\
         | |
         |_|
        */
        final String vertexShaderSourceCode_pv = String.format
                (
                        "#version 320 es" +
                                "\n" +
                                "in vec4 a_position;" +
                                "in vec3 a_normal;" +
                                "uniform mat4 u_modelMatrix;" +
                                "uniform mat4 u_viewMatrix;" +
                                "uniform mat4 u_projectionMatrix;" +
                                "uniform vec3 u_la[3];" +
                                "uniform vec3 u_ld[3];" +
                                "uniform vec3 u_ls[3];" +
                                "uniform vec4 u_lightPosition[3];" +
                                "uniform vec3 u_ka;" +
                                "uniform vec3 u_kd;" +
                                "uniform vec3 u_ks;" +
                                "uniform float u_materialShininess;" +
                                "uniform mediump int u_lightingEnabled;" +
                                "out vec3 phong_ads_light;" +
                                "void main(void)" +
                                "{" +
                                "if(u_lightingEnabled == 1)" +
                                "{" +
                                "vec4 eyecoordinates = u_viewMatrix * u_modelMatrix * a_position;" +
                                "mat3 normalMatrix = mat3(u_viewMatrix * u_modelMatrix);" +
                                "vec3 transformedNormals = normalize(normalMatrix * a_normal);" +
                                "vec3 viewerVector = normalize(-eyecoordinates.xyz);" +
                                "vec3 ambient[3];" +
                                "vec3 lightDirection[3];" +
                                "vec3 diffuse[3];" +
                                "vec3 reflectionVector[3];" +
                                "vec3 specular[3];" +
                                "for(int i=0;i<3;i++)" +
                                "{" +
                                "ambient[i] = u_la[i] * u_ka;" +
                                "lightDirection[i]= normalize(vec3(u_lightPosition[i]) -  eyecoordinates.xyz);" +
                                "diffuse[i]=  u_ld[i] * u_kd * max(dot(lightDirection[i],transformedNormals),0.0);" +
                                "reflectionVector[i] = reflect(-lightDirection[i],transformedNormals);" +
                                "specular[i] = u_ls[i] * u_ks * pow(max(dot(reflectionVector[i],viewerVector),0.0),u_materialShininess);" +
                                "phong_ads_light = phong_ads_light+ambient[i]+diffuse[i]+specular[i];" +
                                "}" +
                                "}" +
                                "else" +
                                "{" +
                                "phong_ads_light = vec3(1.0,1.0,1.0);" +
                                "}" +
                                "gl_Position = u_projectionMatrix * u_viewMatrix * u_modelMatrix * a_position;" +
                                "}"
                );
        int vertexShaderObject_pv = GLES32.glCreateShader(GLES32.GL_VERTEX_SHADER);


        GLES32.glShaderSource(vertexShaderObject_pv, vertexShaderSourceCode_pv);
        GLES32.glCompileShader(vertexShaderObject_pv);

        int status[] = new int[1];
        int infoLogLength[] = new int[1];
        String log = null;
        GLES32.glGetShaderiv(vertexShaderObject_pv, GLES32.GL_COMPILE_STATUS, status, 0);

        if (status[0] == GLES32.GL_FALSE) {
            GLES32.glGetShaderiv(vertexShaderObject_pv, GLES32.GL_INFO_LOG_LENGTH, infoLogLength, 0);
            if (infoLogLength[0] > 0) {
                log = GLES32.glGetShaderInfoLog(vertexShaderObject_pv);
                System.out.println("TMShinde:PER VERTEX VERTEX SHADER COMPILATION LOG : \n" + log);
                uninitialize();
                System.exit(0);
            }
        }

        // Fragment Shader
        final String fragmentShaderSourceCode_pv = String.format
                (
                        "#version 320 es" +
                                "\n" +
                                "precision highp float;" +
                                "in vec3 phong_ads_light;" +
                                "out vec4 FragColor;" +
                                "void main(void)" +
                                "{" +
                                "FragColor = vec4(phong_ads_light,1.0);" +
                                "}"
                );
        int fragmentShaderObject_pv = GLES32.glCreateShader(GLES32.GL_FRAGMENT_SHADER);


        GLES32.glShaderSource(fragmentShaderObject_pv, fragmentShaderSourceCode_pv);
        GLES32.glCompileShader(fragmentShaderObject_pv);

        status[0] = 0;
        infoLogLength[0] = 0;
        log = null;
        GLES32.glGetShaderiv(fragmentShaderObject_pv, GLES32.GL_COMPILE_STATUS, status, 0);

        if (status[0] == GLES32.GL_FALSE) {
            GLES32.glGetShaderiv(fragmentShaderObject_pv, GLES32.GL_INFO_LOG_LENGTH, infoLogLength, 0);
            if (infoLogLength[0] > 0) {
                log = GLES32.glGetShaderInfoLog(fragmentShaderObject_pv);
                System.out.println("TMShinde: PER VERTEX FRAGMENT SHADER COMPILATION LOG : \n" + log);
                uninitialize();
                System.exit(0);
            }
        }

        // shader program for per vertex operation
        shaderProgramObject_pv = GLES32.glCreateProgram();

        GLES32.glAttachShader(shaderProgramObject_pv, vertexShaderObject_pv);
        GLES32.glAttachShader(shaderProgramObject_pv, fragmentShaderObject_pv);

        // pre link
        GLES32.glBindAttribLocation(shaderProgramObject_pv, MyGLESMacros.AMC_ATTRIBUTE_POSITION, "a_position");
        GLES32.glBindAttribLocation(shaderProgramObject_pv, MyGLESMacros.AMC_ATTRIBUTE_NORMAL, "a_normal");

        GLES32.glLinkProgram(shaderProgramObject_pv);

        status[0] = 0;
        infoLogLength[0] = 0;
        log = null;

        GLES32.glGetProgramiv(shaderProgramObject_pv, GLES32.GL_LINK_STATUS, status, 0);
        if (status[0] == GLES32.GL_FALSE) {
            GLES32.glGetProgramiv(shaderProgramObject_pv, GLES32.GL_INFO_LOG_LENGTH, infoLogLength, 0);
            if (infoLogLength[0] > 0) {
                log = GLES32.glGetProgramInfoLog(shaderProgramObject_pv);
                System.out.println("TMShinde: PER VERTEX SHADER PROGRAM LINK LOG  : \n" + log);
                uninitialize();
                System.exit(0);
            }
        }

        // post link
        modelMatrixUniform_pv = GLES32.glGetUniformLocation(shaderProgramObject_pv, "u_modelMatrix");
        viewMatrixUniform_pv = GLES32.glGetUniformLocation(shaderProgramObject_pv, "u_viewMatrix");
        projectionMatrixUniform_pv = GLES32.glGetUniformLocation(shaderProgramObject_pv, "u_projectionMatrix");


        // lights uniform
        laUniform_pv[0] = GLES32.glGetUniformLocation(shaderProgramObject_pv, "u_la[0]");
        ldUniform_pv[0] = GLES32.glGetUniformLocation(shaderProgramObject_pv, "u_ld[0]");
        lsUniform_pv[0] = GLES32.glGetUniformLocation(shaderProgramObject_pv, "u_ls[0]");
        lightPositionUniform_pv[0] = GLES32.glGetUniformLocation(shaderProgramObject_pv, "u_lightPosition[0]");

        laUniform_pv[1] = GLES32.glGetUniformLocation(shaderProgramObject_pv, "u_la[1]");
        ldUniform_pv[1] = GLES32.glGetUniformLocation(shaderProgramObject_pv, "u_ld[1]");
        lsUniform_pv[1] = GLES32.glGetUniformLocation(shaderProgramObject_pv, "u_ls[1]");
        lightPositionUniform_pv[1] = GLES32.glGetUniformLocation(shaderProgramObject_pv, "u_lightPosition[1]");

        laUniform_pv[2] = GLES32.glGetUniformLocation(shaderProgramObject_pv, "u_la[2]");
        ldUniform_pv[2] = GLES32.glGetUniformLocation(shaderProgramObject_pv, "u_ld[2]");
        lsUniform_pv[2] = GLES32.glGetUniformLocation(shaderProgramObject_pv, "u_ls[2]");
        lightPositionUniform_pv[2] = GLES32.glGetUniformLocation(shaderProgramObject_pv, "u_lightPosition[2]");


        // material uniform
        kaUniform_pv = GLES32.glGetUniformLocation(shaderProgramObject_pv, "u_ka");
        kdUniform_pv = GLES32.glGetUniformLocation(shaderProgramObject_pv, "u_kd");
        ksUniform_pv = GLES32.glGetUniformLocation(shaderProgramObject_pv, "u_ks");
        materialShininessUniform_pv = GLES32.glGetUniformLocation(shaderProgramObject_pv, "u_materialShininess");
        lightEnabledUniform_pv = GLES32.glGetUniformLocation(shaderProgramObject_pv, "u_lightingEnabled");


        /*                     __                                      _
                            / _|                                    | |
          _ __   ___ _ __  | |_ _ __ __ _  __ _ _ __ ___   ___ _ __ | |_
         | '_ \ / _ \ '__| |  _| '__/ _` |/ _` | '_ ` _ \ / _ \ '_ \| __|
         | |_) |  __/ |    | | | | | (_| | (_| | | | | | |  __/ | | | |_
         | .__/ \___|_|    |_| |_|  \__,_|\__, |_| |_| |_|\___|_| |_|\__|
         | |                               __/ |
         |_|                              |___/

         */
        final String vertexShaderSourceCode_pf = String.format
                (
                        "#version 320 es" +
                                "\n" +
                                "in vec4 a_position;" +
                                "in vec3 a_normal;" +
                                "uniform mat4 u_modelMatrix;" +
                                "uniform mat4 u_viewMatrix;" +
                                "uniform mat4 u_projectionMatrix;" +
                                "uniform vec4 u_lightPosition[2];" +
                                "uniform mediump int u_lightingEnabled;" +
                                "out vec3 transformedNormals;" +
                                "out vec3 lightDirection[2];" +
                                "out vec3 viewerVector;" +
                                "void main(void)" +
                                "{" +
                                "if(u_lightingEnabled == 1)" +
                                "{" +
                                "vec4 eyecoordinates = u_viewMatrix * u_modelMatrix * a_position;" +
                                "mat3 normalMatrix = mat3(u_viewMatrix * u_modelMatrix);" +
                                "transformedNormals = normalMatrix * a_normal;" +
                                "viewerVector = -eyecoordinates.xyz;" +
                                "for(int i=0;i<2;i++)" +
                                "{" +
                                "lightDirection[i] = vec3(u_lightPosition[i]) - eyecoordinates.xyz;" +
                                "}" +
                                "}" +
                                "gl_Position = u_projectionMatrix * u_viewMatrix * u_modelMatrix * a_position;" +
                                "}"
                );
        int vertexShaderObject_pf = GLES32.glCreateShader(GLES32.GL_VERTEX_SHADER);


        GLES32.glShaderSource(vertexShaderObject_pf, vertexShaderSourceCode_pf);
        GLES32.glCompileShader(vertexShaderObject_pf);


        log = null;
        GLES32.glGetShaderiv(vertexShaderObject_pf, GLES32.GL_COMPILE_STATUS, status, 0);

        if (status[0] == GLES32.GL_FALSE) {
            GLES32.glGetShaderiv(vertexShaderObject_pf, GLES32.GL_INFO_LOG_LENGTH, infoLogLength, 0);
            if (infoLogLength[0] > 0) {
                log = GLES32.glGetShaderInfoLog(vertexShaderObject_pf);
                System.out.println("TMShinde:PER FRAGMENT VERTEX SHADER COMPILATION LOG : \n" + log);
                uninitialize();
                System.exit(0);
            }
        }

        // Fragment Shader
        final String fragmentShaderSourceCode_pf = String.format
                (
                        "#version 320 es" +
                                "\n" +
                                "precision highp float;" +
                                "in vec3 transformedNormals;" +
                                "in vec3 lightDirection[2];" +
                                "in vec3 viewerVector;" +
                                "uniform vec3 u_la[2];" +
                                "uniform vec3 u_ld[2];" +
                                "uniform vec3 u_ls[2];" +
                                "uniform vec3 u_ka;" +
                                "uniform vec3 u_kd;" +
                                "uniform vec3 u_ks;" +
                                "uniform float u_materialShininess;" +
                                "uniform mediump int u_lightingEnabled;" +
                                "vec3 phong_ads_light;" +
                                "out vec4 FragColor;" +
                                "void main(void)" +
                                "{" +
                                "if(u_lightingEnabled == 1)" +
                                "{" +
                                "vec3 normalized_transformedNormals = normalize(transformedNormals);" +
                                "vec3 normalized_viewerVector = normalize(viewerVector);" +
                                "vec3 ambient[2];" +
                                "vec3 normalized_lightDirection[2];" +
                                "vec3 diffuse[2];" +
                                "vec3 reflectionVector[2];" +
                                "vec3 specular[2];" +
                                "for(int i=0;i<2;i++)" +
                                "{" +
                                "ambient[i] =  u_la[i] * u_ka;" +
                                "normalized_lightDirection[i]= normalize(lightDirection[i]);" +
                                "diffuse[i]=  u_ld[i] * u_kd * max(dot(normalized_lightDirection[i],normalized_transformedNormals),0.0);" +
                                "reflectionVector[i] = reflect(-normalized_lightDirection[i],normalized_transformedNormals);" +
                                "specular[i] = u_ls[i] * u_ks * pow(max(dot(reflectionVector[i],normalized_viewerVector),0.0),u_materialShininess);" +
                                "phong_ads_light = phong_ads_light+ambient[i]+diffuse[i]+specular[i];" +
                                "}" +
                                "}" +
                                "else" +
                                "{" +
                                "phong_ads_light = vec3(1.0,1.0,1.0);" +
                                "}" +
                                "FragColor = vec4(phong_ads_light,1.0);" +
                                "}"
                );
        int fragmentShaderObject_pf = GLES32.glCreateShader(GLES32.GL_FRAGMENT_SHADER);


        GLES32.glShaderSource(fragmentShaderObject_pf, fragmentShaderSourceCode_pf);
        GLES32.glCompileShader(fragmentShaderObject_pf);

        status[0] = 0;
        infoLogLength[0] = 0;
        log = null;
        GLES32.glGetShaderiv(fragmentShaderObject_pf, GLES32.GL_COMPILE_STATUS, status, 0);

        if (status[0] == GLES32.GL_FALSE) {
            GLES32.glGetShaderiv(fragmentShaderObject_pf, GLES32.GL_INFO_LOG_LENGTH, infoLogLength, 0);
            if (infoLogLength[0] > 0) {
                log = GLES32.glGetShaderInfoLog(fragmentShaderObject_pf);
                System.out.println("TMShinde: PER FRAGMENT FRAGMENT SHADER COMPILATION LOG : +n" + log);
                uninitialize();
                System.exit(0);
            }
        }

        // shader program
        shaderProgramObject_pf = GLES32.glCreateProgram();

        GLES32.glAttachShader(shaderProgramObject_pf, vertexShaderObject_pf);
        GLES32.glAttachShader(shaderProgramObject_pf, fragmentShaderObject_pf);

        // pre link
        GLES32.glBindAttribLocation(shaderProgramObject_pf, MyGLESMacros.AMC_ATTRIBUTE_POSITION, "a_position");
        GLES32.glBindAttribLocation(shaderProgramObject_pf, MyGLESMacros.AMC_ATTRIBUTE_NORMAL, "a_normal");

        GLES32.glLinkProgram(shaderProgramObject_pf);

        status[0] = 0;
        infoLogLength[0] = 0;
        log = null;

        GLES32.glGetProgramiv(shaderProgramObject_pf, GLES32.GL_LINK_STATUS, status, 0);
        if (status[0] == GLES32.GL_FALSE) {
            GLES32.glGetProgramiv(shaderProgramObject_pf, GLES32.GL_INFO_LOG_LENGTH, infoLogLength, 0);
            if (infoLogLength[0] > 0) {
                log = GLES32.glGetProgramInfoLog(shaderProgramObject_pf);
                System.out.println("TMShinde:PER FRAGMENT  SHADER PROGRAM LINK LOG  : \n" + log);
                uninitialize();
                System.exit(0);
            }
        }

        // post link
        modelMatrixUniform_pf = GLES32.glGetUniformLocation(shaderProgramObject_pf, "u_modelMatrix");
        viewMatrixUniform_pf = GLES32.glGetUniformLocation(shaderProgramObject_pf, "u_viewMatrix");
        projectionMatrixUniform_pf = GLES32.glGetUniformLocation(shaderProgramObject_pf, "u_projectionMatrix");


        laUniform_pf[0] = GLES32.glGetUniformLocation(shaderProgramObject_pf, "u_la[0]");
        ldUniform_pf[0] = GLES32.glGetUniformLocation(shaderProgramObject_pf, "u_ld[0]");
        lsUniform_pf[0] = GLES32.glGetUniformLocation(shaderProgramObject_pf, "u_ls[0]");
        lightPositionUniform_pf[0] = GLES32.glGetUniformLocation(shaderProgramObject_pf, "u_lightPosition[0]");

        laUniform_pf[1] = GLES32.glGetUniformLocation(shaderProgramObject_pf, "u_la[1]");
        ldUniform_pf[1] = GLES32.glGetUniformLocation(shaderProgramObject_pf, "u_ld[1]");
        lsUniform_pf[1] = GLES32.glGetUniformLocation(shaderProgramObject_pf, "u_ls[1]");
        lightPositionUniform_pf[1] = GLES32.glGetUniformLocation(shaderProgramObject_pf, "u_lightPosition[1]");

        laUniform_pf[2] = GLES32.glGetUniformLocation(shaderProgramObject_pf, "u_la[2]");
        ldUniform_pf[2] = GLES32.glGetUniformLocation(shaderProgramObject_pf, "u_ld[2]");
        lsUniform_pf[2] = GLES32.glGetUniformLocation(shaderProgramObject_pf, "u_ls[2]");
        lightPositionUniform_pf[2] = GLES32.glGetUniformLocation(shaderProgramObject_pf, "u_lightPosition[2]");

        kaUniform_pf = GLES32.glGetUniformLocation(shaderProgramObject_pf, "u_ka");
        kdUniform_pf = GLES32.glGetUniformLocation(shaderProgramObject_pf, "u_kd");
        ksUniform_pf = GLES32.glGetUniformLocation(shaderProgramObject_pf, "u_ks");
        materialShininessUniform_pf = GLES32.glGetUniformLocation(shaderProgramObject_pf, "u_materialShininess");
        lightEnabledUniform_pf = GLES32.glGetUniformLocation(shaderProgramObject_pf, "u_lightingEnabled");

        Sphere sphere = new Sphere();
        float sphere_vertices[] = new float[1146];
        float sphere_normals[] = new float[1146];
        float sphere_textures[] = new float[764];
        short sphere_elements[] = new short[2280];

        sphere.getSphereVertexData(sphere_vertices, sphere_normals, sphere_textures, sphere_elements);
        numVertices = sphere.getNumberOfSphereVertices();
        numElements = sphere.getNumberOfSphereElements();


        // ************SPHERE
        GLES32.glGenVertexArrays(1, vao_sphere, 0);
        GLES32.glBindVertexArray(vao_sphere[0]);

        // *************** POSITION
        GLES32.glGenBuffers(1, vbo_sphere_position, 0);
        GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, vbo_sphere_position[0]);

        // buffer data 3rd param code
        // 4 is of float size
        ByteBuffer byteBuffer = ByteBuffer.allocateDirect(sphere_vertices.length * 4);
        byteBuffer.order(ByteOrder.nativeOrder());
        FloatBuffer verticesBuffer = byteBuffer.asFloatBuffer();
        verticesBuffer.put(sphere_vertices);
        verticesBuffer.position(0); // its array start position do not change to color

        GLES32.glBufferData(GLES32.GL_ARRAY_BUFFER, sphere_vertices.length * 4, verticesBuffer, GLES32.GL_STATIC_DRAW);
        GLES32.glVertexAttribPointer(MyGLESMacros.AMC_ATTRIBUTE_POSITION, 3, GLES32.GL_FLOAT, false, 0, 0);
        GLES32.glEnableVertexAttribArray(MyGLESMacros.AMC_ATTRIBUTE_POSITION);

        GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, 0);

        // *********** normal vbo
        GLES32.glGenBuffers(1, vbo_sphere_normal, 0);
        GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, vbo_sphere_normal[0]);

        byteBuffer = ByteBuffer.allocateDirect(sphere_normals.length * 4);
        byteBuffer.order(ByteOrder.nativeOrder());
        verticesBuffer = byteBuffer.asFloatBuffer();
        verticesBuffer.put(sphere_normals);
        verticesBuffer.position(0);

        GLES32.glBufferData(GLES32.GL_ARRAY_BUFFER,
                sphere_normals.length * 4,
                verticesBuffer,
                GLES32.GL_STATIC_DRAW);

        GLES32.glVertexAttribPointer(MyGLESMacros.AMC_ATTRIBUTE_NORMAL,
                3,
                GLES32.GL_FLOAT,
                false, 0, 0);

        GLES32.glEnableVertexAttribArray(MyGLESMacros.AMC_ATTRIBUTE_NORMAL);

        GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, 0);


        // ************************** ELEMENT VBO *******************
        GLES32.glGenBuffers(1, vbo_sphere_element, 0);
        GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, vbo_sphere_element[0]);

        byteBuffer = ByteBuffer.allocateDirect(sphere_elements.length * 4);
        byteBuffer.order(ByteOrder.nativeOrder());
        ShortBuffer elementBuffer = byteBuffer.asShortBuffer();
        elementBuffer.put(sphere_elements);
        elementBuffer.position(0); // its array start position do not change to color

        GLES32.glBufferData(GLES32.GL_ELEMENT_ARRAY_BUFFER, sphere_elements.length * 4, elementBuffer, GLES32.GL_STATIC_DRAW);

        GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, 0);
        GLES32.glBindVertexArray(0);

        GLES32.glClearDepthf(1.0f);
        GLES32.glEnable(GLES32.GL_DEPTH_TEST);
        GLES32.glDepthFunc(GLES32.GL_LEQUAL);

        GLES32.glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

        float ambient1[] = new float[]{0.0f, 0.0f, 0.0f, 1.0f};
        float diffuse1[] = new float[]{1.0f, 0.0f, 0.0f, 1.0f};
        float specular1[] = new float[]{1.0f, 0.0f, 0.0f, 1.0f};
        float lightPosition1[] = new float[]{0.0f, 0.0f, 0.0f, 1.0f};

        float ambient2[] = new float[]{0.0f, 0.0f, 0.0f, 1.0f};
        float diffuse2[] = new float[]{0.0f, 1.0f, 0.0f, 1.0f};
        float specular2[] = new float[]{0.0f, 1.0f, 0.0f, 1.0f};
        float lightPosition2[] = new float[]{0.0f, 0.0f, 0.0f, 1.0f};

        float ambient3[] = new float[]{0.0f, 0.0f, 0.0f, 1.0f};
        float diffuse3[] = new float[]{0.0f, 0.0f, 1.0f, 1.0f};
        float specular3[] = new float[]{0.0f, 0.0f, 1.0f, 1.0f};
        float lightPosition3[] = new float[]{0.0f, 0.0f, 0.0f, 1.0f};

        lights[0] = new SphereLights(ambient1, diffuse1, specular1, lightPosition1);
        lights[1] = new SphereLights(ambient2, diffuse2, specular2, lightPosition2);
        lights[2] = new SphereLights(ambient3, diffuse3, specular3, lightPosition3);

        // initialization of projection matrix
        Matrix.setIdentityM(perspectiveProjectionMatrixSphere, 0);
        resizeSphere(FBO_WIDTH,FBO_HEIGHT);

    }
    private void resize(int width, int height) {
        System.out.println("TMShinde: Inside resize()" );
        winWidth_fbo = width;
        winHeight_fbo = height;
        if (height <= 0) {
            height = 1;
        }
        GLES32.glViewport(0, 0, winWidth_fbo, winHeight_fbo);
        Matrix.perspectiveM(perspectiveProjectionMatrix, 0,
                45.0f, (float) winWidth_fbo / (float) winHeight_fbo, 0.1f, 100.0f);
    }

    private void resizeSphere(int width ,int height)
    {
        System.out.println("TMShinde: Inside resizeSphere()" );
        if (height <= 0) {
            height = 1;
        }
        GLES32.glViewport(0, 0, width, height);
        Matrix.perspectiveM(perspectiveProjectionMatrixSphere, 0,
                45.0f, (float) width / (float) height, 0.1f, 100.0f);
    }

    private void display() {
        System.out.println("TMShinde: Inside display()" );
        if (bFBOResult == true)
        {
            display_sphere(FBO_WIDTH, FBO_HEIGHT);
            updateSphere();
        }
        GLES32.glClearColor(1.0f,1.0f,1.0f,1.0f);
        resize(winWidth_fbo,winHeight_fbo);
        GLES32.glClear(GLES32.GL_COLOR_BUFFER_BIT | GLES32.GL_DEPTH_BUFFER_BIT);
        GLES32.glUseProgram(shaderProgramObject);
        float modelViewMatrix[] = new float[16];
        float modelViewProjectionMatrix[] = new float[16];
        // square related
        float rotationX[] = new float[16];
        float rotationY[] = new float[16];
        float rotationZ[] = new float[16];
        float rotationMatrix[] = new float[16];
        Matrix.setIdentityM(rotationX, 0);
        Matrix.setIdentityM(rotationY, 0);
        Matrix.setIdentityM(rotationZ, 0);

        Matrix.setRotateM(rotationX, 0, angleCube, 1.0f, 0.0f, 0.0f);
        Matrix.setRotateM(rotationY, 0, angleCube, 0.0f, 1.0f, 0.0f);
        Matrix.setRotateM(rotationZ, 0, angleCube, 0.0f, 0.0f, 1.0f);

        Matrix.multiplyMM(rotationMatrix, 0, rotationX, 0, rotationY, 0);
        Matrix.multiplyMM(rotationMatrix, 0, rotationMatrix, 0, rotationZ, 0);


        Matrix.setIdentityM(modelViewMatrix, 0);
        Matrix.translateM(modelViewMatrix, 0, 1.5f, 0.0f, -6.0f);
        Matrix.multiplyMM(modelViewMatrix, 0, modelViewMatrix, 0, rotationMatrix, 0);

        Matrix.scaleM(modelViewMatrix, 0, 0.75f, 0.75f, 0.75f);
        Matrix.multiplyMM(modelViewProjectionMatrix, 0, perspectiveProjectionMatrix, 0, modelViewMatrix, 0);
        GLES32.glUniformMatrix4fv(mvpMatrixUniform, 1, false, modelViewProjectionMatrix, 0);

        GLES32.glActiveTexture(GLES32.GL_TEXTURE0);
        GLES32.glBindTexture(GLES32.GL_TEXTURE_2D, fbo_texture[0]);
        GLES32.glUniform1i(textureSamplerUniform, 0);
        GLES32.glBindVertexArray(vao_cube[0]);
        GLES32.glDrawArrays(GLES32.GL_TRIANGLE_FAN, 0, 4);
        GLES32.glDrawArrays(GLES32.GL_TRIANGLE_FAN, 4, 4);
        GLES32.glDrawArrays(GLES32.GL_TRIANGLE_FAN, 8, 4);
        GLES32.glDrawArrays(GLES32.GL_TRIANGLE_FAN, 12, 4);
        GLES32.glDrawArrays(GLES32.GL_TRIANGLE_FAN, 16, 4);
        GLES32.glDrawArrays(GLES32.GL_TRIANGLE_FAN, 20, 4);
        GLES32.glBindVertexArray(0);

        GLES32.glUseProgram(0);
        requestRender(); // swap buffers
    }

    private void display_sphere(int textureWidth ,int textureHeight)
    {
//        System.out.println("TMShinde: Inside display_sphere()" );

        GLES32.glBindFramebuffer(GLES32.GL_FRAMEBUFFER,fbo[0]);
        GLES32.glClearColor(0.0f,0.0f,0.0f,1.0f);
        resizeSphere(textureWidth,textureHeight);

        GLES32.glClear(GLES32.GL_COLOR_BUFFER_BIT | GLES32.GL_DEPTH_BUFFER_BIT);
        if (choosenShaderSphere == PER_VERTEX) {
            GLES32.glUseProgram(shaderProgramObject_pv);
        } else if (choosenShaderSphere == PER_FRAGMENT) {
            GLES32.glUseProgram(shaderProgramObject_pf);
        }
        float modelMatrix[] = new float[16];
        float viewMatrix[] = new float[16];

        // do identity matrix
        Matrix.setIdentityM(modelMatrix, 0);
        Matrix.setIdentityM(viewMatrix, 0);


        // translation then rotation
        Matrix.translateM(modelMatrix, 0, 0.0f, 0.0f, -2.0f);


        // draw shape by sending uniform
        if (choosenShaderSphere == PER_VERTEX) {
            GLES32.glUniformMatrix4fv(modelMatrixUniform_pv, 1, false, modelMatrix, 0);
            GLES32.glUniformMatrix4fv(viewMatrixUniform_pv, 1, false, viewMatrix, 0);
            GLES32.glUniformMatrix4fv(projectionMatrixUniform_pv, 1, false, perspectiveProjectionMatrix, 0);
        } else if (choosenShaderSphere == PER_FRAGMENT) {
            GLES32.glUniformMatrix4fv(modelMatrixUniform_pf, 1, false, modelMatrix, 0);
            GLES32.glUniformMatrix4fv(viewMatrixUniform_pf, 1, false, viewMatrix, 0);
            GLES32.glUniformMatrix4fv(projectionMatrixUniform_pf, 1, false, perspectiveProjectionMatrix, 0);
        }

        // toggle lights logic
        if (doubleTapSphere == 1) {
            float lightPositionX[] = new float[]{0.0f, (float) Math.sin(angleLightSphere) * 5.0f, (float) Math.cos(angleLightSphere) * 5.0f - 1.0f, 1.0f};
            float lightPositionY[] = new float[]{(float) Math.sin(angleLightSphere) * 5.0f, 0.0f, (float) Math.cos(angleLightSphere) * 5.0f - 1.0f, 1.0f};
            float lightPositionZ[] = new float[]{(float) Math.sin(angleLightSphere) * 7.0f,(float) Math.cos(angleLightSphere) * 5.0f, 0.0f, 1.0f};
            lights[0].setLightPosition(lightPositionX);
            lights[1].setLightPosition(lightPositionZ);
            lights[2].setLightPosition(lightPositionY);

            if (choosenShaderSphere == PER_VERTEX) {
                GLES32.glUniform1i(lightEnabledUniform_pv, 1);
                for (int i = 0; i < 3; i++) {
                    GLES32.glUniform3fv(laUniform_pv[i], 1, lights[i].lightAmbient, 0);
                    GLES32.glUniform3fv(ldUniform_pv[i], 1, lights[i].lightDiffuse, 0);
                    GLES32.glUniform3fv(lsUniform_pv[i], 1, lights[i].lightSpecular, 0);
                    GLES32.glUniform4fv(lightPositionUniform_pv[i], 1, lights[i].lightPosition, 0);
                }
                GLES32.glUniform3fv(kaUniform_pv, 1, materialAmbientSphere, 0);
                GLES32.glUniform3fv(kdUniform_pv, 1, materialDiffuseSphere, 0);
                GLES32.glUniform3fv(ksUniform_pv, 1, materialSpecularSphere, 0);
                GLES32.glUniform1f(materialShininessUniform_pv, materialShininessSphere);
            } else if (choosenShaderSphere == PER_FRAGMENT) {
                GLES32.glUniform1i(lightEnabledUniform_pf, 1);

                for (int i = 0; i < 3; i++) {
                    GLES32.glUniform3fv(laUniform_pf[i], 1, lights[i].lightAmbient, 0);
                    GLES32.glUniform3fv(ldUniform_pf[i], 1, lights[i].lightDiffuse, 0);
                    GLES32.glUniform3fv(lsUniform_pf[i], 1, lights[i].lightSpecular, 0);
                    GLES32.glUniform4fv(lightPositionUniform_pf[i], 1, lights[i].lightPosition, 0);
                }

                GLES32.glUniform3fv(kaUniform_pf, 1, materialAmbientSphere, 0);
                GLES32.glUniform3fv(kdUniform_pf, 1, materialDiffuseSphere, 0);
                GLES32.glUniform3fv(ksUniform_pf, 1, materialSpecularSphere, 0);
                GLES32.glUniform1f(materialShininessUniform_pf, materialShininessSphere);
            }
        } else {
            if (choosenShaderSphere == PER_VERTEX) {
                GLES32.glUniform1i(lightEnabledUniform_pv, 0);
            } else if (choosenShaderSphere == PER_FRAGMENT) {
                GLES32.glUniform1i(lightEnabledUniform_pf, 0);
            }
        }

        GLES32.glBindVertexArray(vao_sphere[0]);
        GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, vbo_sphere_element[0]);
        GLES32.glDrawElements(GLES32.GL_TRIANGLES, numElements, GLES32.GL_UNSIGNED_SHORT, 0);
        GLES32.glBindVertexArray(0);


        GLES32.glUseProgram(0);
        GLES32.glBindFramebuffer(GLES32.GL_FRAMEBUFFER,0);
    }
    private void update() {
        // code
       // System.out.println("TMShinde: Inside update()" );

        angleCube = angleCube + 1.0f;
        if (angleCube > 360.0f) {
            angleCube = 0.0f;
        }
    }

    private void updateSphere() {
        // code
       // System.out.println("TMShinde: Inside updateSphere()" );

        angleLightSphere = angleLightSphere + 0.05f;
        if (angleLightSphere > 360.0f)
        {
            angleLightSphere = angleLightSphere - 360.0f;
        }
    }

    private void uninitialize() {
        // code
        uninitializeSphere();
        if (vbo_cube_texcoord[0] > 0) {
            GLES32.glDeleteBuffers(1, vbo_cube_texcoord, 0);
            vbo_cube_texcoord[0] = 0;
        }

        if (vbo_cube_position[0] > 0) {
            GLES32.glDeleteBuffers(1, vbo_cube_position, 0);
            vbo_cube_position[0] = 0;
        }

        if (vao_cube[0] > 0) {
            GLES32.glDeleteVertexArrays(1, vao_cube, 0);
            vao_cube[0] = 0;
        }


        if (shaderProgramObject > 0) {
            GLES32.glUseProgram(shaderProgramObject);
            int retVal[] = new int[1];
            GLES32.glGetProgramiv(shaderProgramObject, GLES32.GL_ATTACHED_SHADERS, retVal, 0);

            if (retVal[0] > 0) {
                int numAttachedShader = retVal[0];
                int shaderObjects[] = new int[numAttachedShader];

                GLES32.glGetAttachedShaders(shaderProgramObject, numAttachedShader, retVal, 0, shaderObjects, 0);

                for (int i = 0; i < numAttachedShader; i++) {
                    GLES32.glDetachShader(shaderProgramObject, shaderObjects[i]);
                    GLES32.glDeleteShader(shaderObjects[i]);
                    shaderObjects[i] = 0;
                }
            }
            GLES32.glUseProgram(0);
            GLES32.glDeleteProgram(shaderProgramObject);
            shaderProgramObject = 0;
        }
    }


    private void uninitializeSphere(){
        if (vao_sphere[0] != 0) {
            GLES32.glDeleteVertexArrays(1, vao_sphere, 0);
            vao_sphere[0] = 0;
        }

        // destroy position vbo
        if (vbo_sphere_position[0] != 0) {
            GLES32.glDeleteBuffers(1, vbo_sphere_position, 0);
            vbo_sphere_position[0] = 0;
        }

        // destroy normal vbo
        if (vbo_sphere_normal[0] != 0) {
            GLES32.glDeleteBuffers(1, vbo_sphere_normal, 0);
            vbo_sphere_normal[0] = 0;
        }

        // destroy element vbo
        if (vbo_sphere_element[0] != 0) {
            GLES32.glDeleteBuffers(1, vbo_sphere_element, 0);
            vbo_sphere_element[0] = 0;
        }
        int chShaderObject = 0;
        if (choosenShaderSphere == PER_VERTEX)
            chShaderObject = shaderProgramObject_pv;
        else if (choosenShaderSphere == PER_FRAGMENT)
            chShaderObject = shaderProgramObject_pf;
        if (chShaderObject > 0) {
            GLES32.glUseProgram(chShaderObject);
            int retVal[] = new int[1];
            GLES32.glGetProgramiv(chShaderObject, GLES32.GL_ATTACHED_SHADERS, retVal, 0);

            if (retVal[0] > 0) {
                int numAttachedShader = retVal[0];
                int shaderObjects[] = new int[numAttachedShader];

                GLES32.glGetAttachedShaders(chShaderObject, numAttachedShader, retVal, 0, shaderObjects, 0);

                for (int i = 0; i < numAttachedShader; i++) {
                    GLES32.glDetachShader(chShaderObject, shaderObjects[i]);
                    GLES32.glDeleteShader(shaderObjects[i]);
                    shaderObjects[i] = 0;
                }
            }
            GLES32.glUseProgram(0);
            GLES32.glDeleteProgram(chShaderObject);
            chShaderObject = 0;
        }
    }
}

class SphereLights {
    float[] lightAmbient;
    float[] lightDiffuse;
    float[] lightSpecular;
    float[] lightPosition;

    public SphereLights(float[] lightAmbient, float[] lightDiffuse, float[] lightSpecular, float[] lightPosition) {
        this.lightAmbient = lightAmbient;
        this.lightDiffuse = lightDiffuse;
        this.lightSpecular = lightSpecular;
        this.lightPosition = lightPosition;
    }

    public float[] getLightAmbient() {
        return lightAmbient;
    }

    public void setLightAmbient(float[] lightAmbient) {
        this.lightAmbient = lightAmbient;
    }

    public float[] getLightDiffuse() {
        return lightDiffuse;
    }

    public void setLightDiffuse(float[] lightDiffuse) {
        this.lightDiffuse = lightDiffuse;
    }

    public float[] getLightSpecular() {
        return lightSpecular;
    }

    public void setLightSpecular(float[] lightSpecular) {
        this.lightSpecular = lightSpecular;
    }

    public float[] getLightPosition() {
        return lightPosition;
    }

    public void setLightPosition(float[] lightPosition) {
        this.lightPosition = lightPosition;
    }
}



