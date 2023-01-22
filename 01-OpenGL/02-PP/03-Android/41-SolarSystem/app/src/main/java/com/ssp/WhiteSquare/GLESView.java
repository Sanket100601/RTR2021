package com.ssp.WhiteSquare;


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
import java.util.Stack;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

public class GLESView extends GLSurfaceView implements OnDoubleTapListener, OnGestureListener, GLSurfaceView.Renderer {

    private GestureDetector gestureDetector;
    private Context context;
    private int shaderProgramObject;


    // sphere
    private int vao_sphere[] = new int[1];
    private int vbo_sphere_position[] = new int[1];
    private int vbo_sphere_normal[] = new int[1];
    private int vbo_sphere_element[] = new int[1];

    private int numElements;
    private int numVertices;


    private int mvpMatrixUniform;
    private int textureSamplerUniform;

    private float perspectiveProjectionMatrix[] = new float[16];  // mat4 --> 4 X 4 Matrix hence 16 array size

    // stack related
//    private float pushedMatrix[] = new float[16];
    Stack<float[]> tmsStack = new Stack<>();
    // solar system related
    int day = 0;
    int year = 0;
    int moon = 0;
int changeGola = 1;

    private int texture_Sun[] = new int[1];
    private int texture_Earth[] = new int[1];
    private int texture_Moon[] = new int[1];

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

        System.out.println("AMC:" + glesVersion);
        System.out.println("AMC:" + renderer);

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
        changeGola++;
        if (changeGola > 6) {
            changeGola = 1;
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
                System.out.println("AMC: VERTEX SHADER COMPILATION LOG : \n" + log);
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
                System.out.println("AMC: FRAGMENT SHADER COMPILATION LOG : \n" + log);
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

        GLES32.glLinkProgram(shaderProgramObject);

        status[0] = 0;
        infoLogLength[0] = 0;
        log = null;

        GLES32.glGetProgramiv(shaderProgramObject, GLES32.GL_LINK_STATUS, status, 0);
        if (status[0] == GLES32.GL_FALSE) {
            GLES32.glGetProgramiv(shaderProgramObject, GLES32.GL_INFO_LOG_LENGTH, infoLogLength, 0);
            if (infoLogLength[0] > 0) {
                log = GLES32.glGetProgramInfoLog(shaderProgramObject);
                System.out.println("AMC:  SHADER PROGRAM LINK LOG  : \n" + log);
                uninitialize();
                System.exit(0);
            }
        }

        // post link
        mvpMatrixUniform = GLES32.glGetUniformLocation(shaderProgramObject, "u_mvpMatrix");
        textureSamplerUniform = GLES32.glGetUniformLocation(shaderProgramObject, "u_textureSampler");


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

        texture_Earth[0] = loadGLTexture(R.raw.earth);
        texture_Moon[0] = loadGLTexture(R.raw.moon);
        texture_Sun[0]  = loadGLTexture(R.raw.sun);
        GLES32.glEnable(GLES32.GL_TEXTURE_2D);

        // initialization of projection matrix
        Matrix.setIdentityM(perspectiveProjectionMatrix, 0);
    }

    private int loadGLTexture(int imageResourceId)
    {
        BitmapFactory.Options options =  new BitmapFactory.Options();
        options.inScaled =  false;
        /* get all resources from context from that give imageResourceId and turn off scaling feature */
        Bitmap bitmap =   BitmapFactory.decodeResource(context.getResources(),imageResourceId,options);
        int texture[] = new int[1];
        GLES32.glPixelStorei(GLES32.GL_UNPACK_ALIGNMENT,1);
        GLES32.glGenTextures(1,texture,0);
        GLES32.glBindTexture(GLES32.GL_TEXTURE_2D,texture[0]);
        GLES32.glTexParameteri(GLES32.GL_TEXTURE_2D,GLES32.GL_TEXTURE_MAG_FILTER,GLES32.GL_LINEAR);
        GLES32.glTexParameteri(GLES32.GL_ARRAY_BUFFER,GLES32.GL_TEXTURE_MIN_FILTER,GLES32.GL_LINEAR_MIPMAP_LINEAR);
        GLUtils.texImage2D(GLES32.GL_TEXTURE_2D,0,bitmap,0);
        GLES32.glGenerateMipmap(GLES32.GL_TEXTURE_2D);
        GLES32.glBindTexture(GLES32.GL_TEXTURE_2D,0);


        return texture[0];
    }

    private void resize(int width, int height) {
        if (height <= 0) {
            height = 1;
        }
        GLES32.glViewport(0, 0, width, height);
        Matrix.perspectiveM(perspectiveProjectionMatrix, 0,
                45.0f, (float) width / (float) height, 0.1f, 100.0f);
    }

    private void display() {
        GLES32.glClear(GLES32.GL_COLOR_BUFFER_BIT | GLES32.GL_DEPTH_BUFFER_BIT);
        GLES32.glUseProgram(shaderProgramObject);
        float modelMatrix[] = new float[16];
        float viewMatrix[] = new float[16];
        float modelViewMatrix[] = new float[16];
        // do identity matrix
        Matrix.setIdentityM(modelMatrix, 0);
        Matrix.setIdentityM(viewMatrix, 0);

        Matrix.translateM(modelMatrix, 0, 0.0f, 0.0f, -3.0f);
        Matrix.multiplyMM(modelViewMatrix, 0, viewMatrix, 0, modelMatrix, 0);
        float modelViewProjectionMatrix[] =  new float[16];
        Matrix.multiplyMM(modelViewProjectionMatrix, 0, perspectiveProjectionMatrix, 0, modelViewMatrix, 0);
        tmsStack.push(modelViewProjectionMatrix);


        drawSphere(texture_Sun[0],modelViewProjectionMatrix);


        modelViewProjectionMatrix = tmsStack.pop();

        Matrix.rotateM(modelMatrix,0,(float)year,0.0f,1.0f,0.0f);
        Matrix.translateM(modelMatrix,0,1.95f,0.0f,0.0f);
        Matrix.rotateM(modelMatrix,0,(float)day,0.0f,1.0f,0.0f);
        Matrix.multiplyMM(modelViewProjectionMatrix,0,modelViewProjectionMatrix,0,modelMatrix,0);
        tmsStack.push(modelViewProjectionMatrix);
        Matrix.scaleM(modelViewProjectionMatrix,0,0.75f,0.75f,0.75f);



        drawSphere(texture_Earth[0],modelViewProjectionMatrix);


        modelViewProjectionMatrix = tmsStack.pop();
        Matrix.translateM(modelViewProjectionMatrix,0,1.05f,0.0f,0.0f);
        Matrix.rotateM(modelViewProjectionMatrix,0,(float)moon,0.0f,1.0f,0.0f);
        Matrix.scaleM(modelViewProjectionMatrix,0,0.45f,0.45f,0.45f);

        drawSphere(texture_Moon[0],modelViewProjectionMatrix);
        GLES32.glUseProgram(0);
        requestRender(); // swap buffers
    }

    private void drawSphere(int textureName,float[] uniform)
    {
        GLES32.glUniformMatrix4fv(mvpMatrixUniform, 1, false,uniform,0);

        GLES32.glActiveTexture(GLES20.GL_TEXTURE0);
        GLES32.glBindTexture(GLES20.GL_TEXTURE_2D,textureName);
        GLES32.glUniform1i(textureSamplerUniform,0);
        GLES32.glBindVertexArray(vao_sphere[0]);
        GLES32.glBindBuffer(GLES20.GL_ELEMENT_ARRAY_BUFFER,vbo_sphere_element[0]);
        GLES32.glDrawElements(GLES32.GL_LINE_LOOP ,numElements, GLES32.GL_UNSIGNED_SHORT,0);
        GLES32.glBindVertexArray(0);
        GLES32.glBindTexture(GLES20.GL_TEXTURE_2D,0);
    }

    private void update() {
        if(changeGola == 1)
        {
            day = (day+2)%360;
        }else  if(changeGola == 3)
        {
            year = (year+2)%360;
        }else  if(changeGola == 5)
        {
            moon = (moon+2)%360;
        }

        if(changeGola == 2)
        {
            day = (day-2)%360;
        }else  if(changeGola == 4)
        {
            year = (year-2)%360;
        }else  if(changeGola == 6)
        {
            moon = (moon-2)%360;
        }
    }

    private void uninitialize() {
        // code

        // destroy vao
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
}

