package com.ssp.window;

import android.opengl.GLSurfaceView;
//for Context
import android.content.Context;



//event relatwed backage
import android.view.GestureDetector;
import android.view.GestureDetector.OnDoubleTapListener;

import android.view.GestureDetector.OnGestureListener;
import android.view.MotionEvent;

//package related to graphic
import javax.microedition.khronos.opengles.GL10;
import javax.microedition.khronos.egl.EGLConfig;
import android.opengl.GLES32;


//buffer related package
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.FloatBuffer;

//matrix pacage
import android.opengl.Matrix;

public class GLESView extends GLSurfaceView implements OnDoubleTapListener,OnGestureListener,GLSurfaceView.Renderer{
	private GestureDetector gestureDetector;


		private int VAO[]=new int[1];
	private  int  VBO[]=new int[1];
	private int VBON[]=new int[1];
	private int modelMatrixUniform;
	private int viewMatrixUniform;
	private int projectionMatrixUniform;


	private int shaderProgramObject;
	private float prespectiveProjectionMatrix[]=new float[16];


	float rotationAngle=0.0f;

	//light varaible
	private int kdUniform;
	private int ldUniform;
	private int lightPositionUniform;
	private int lightEnableUniform;
	boolean bLight=false;

	float lightDiffuse[]={1.0f,1.0f,1.0f,1.0f};
	float materialDiffuse[]={0.5f,0.5f,0.5f,1.0f};
	float lightPosition[]={0.0f,0.0f,2.0f,1.0f};

	private Context context;
	GLESView(Context _context){   //this from MainActivuty come here as Context
		super(_context);
		context=_context;
		
		setEGLContextClientVersion(3);   //egl :embeded graphoc library  //here 3 is openGL ES 3.2
		setRenderer(this);
		setRenderMode(GLSurfaceView.RENDERMODE_WHEN_DIRTY);  //render or repaint rhe screen i.e when screen is dirty then render it again
		//gesture related Code
		gestureDetector=new GestureDetector(context,this,null,false);
		//1 para: context which is activity  
		//2 para :this is self as MyView
		//3 Para: is tehre another class which is handling me and my event
		//4 para :reserved it is there beacuse it user in furture .
		gestureDetector.setOnDoubleTapListener(this);
	}

	//3 method of GLSurfaceView.Renderer
	@Override
	public void onSurfaceCreated(GL10 gl,EGLConfig config){   //same as initalize
			String glEsVersion=gl.glGetString(GL10.GL_VERSION);
			System.out.println("AMC: "+"Version is  "+glEsVersion);

			String glEsRenderer=gl.glGetString(GL10.GL_RENDERER);
			System.out.println("AMC: "+"Renderer is  "+glEsRenderer);

			String glSlVersion=gl.glGetString(GLES32.GL_SHADING_LANGUAGE_VERSION);
			System.out.println("AMC: "+"glSL is  "+glSlVersion);

			initalize();
		//	System.out.println("ll\n");
			
			}
		
	@Override
	public void onSurfaceChanged(GL10 unUsed,int width,int height){ //same as resize
		resize(width,height);	
	}

	@Override
	public void onDrawFrame(GL10 unUsed){  //display
		//on drawFrame consider as game loop
		display();
		update();
	}
	@Override
	public boolean onTouchEvent(MotionEvent e){
			if(!gestureDetector.onTouchEvent(e)){
				super.onTouchEvent(e);	
			}
			return true;
	}

	//3 method of doubletaplistner interface
	@Override
	public boolean onDoubleTap(MotionEvent e){
		if(bLight==false){
			bLight=true;
					System.out.println("AMC:"+"Tr");

			}
		else {
			bLight=false;
					System.out.println("AMC:"+"Fa");
			}
	
		return true;
	}
	@Override
	public boolean onDoubleTapEvent(MotionEvent e){
		return true;
	}
	@Override
	public boolean onSingleTapConfirmed(MotionEvent e){
		
			return true;
	}
	
	//6 method of on Gesture listerner

	   @Override
	public boolean onDown(MotionEvent e){
		return true;
	}
	   @Override
	public boolean onFling(MotionEvent e1,MotionEvent e2,float velocityX,float velocityY){
		return true;
	}
	   @Override
	public void onLongPress(MotionEvent e){
		
	}
	@Override
	public boolean onScroll(MotionEvent e1,MotionEvent e2,float distanceX,float distanceY){
		uninitalize();
		System.exit(0);
		return true;
		}
	@Override
	public void	onShowPress(MotionEvent e){
	
	}
	@Override
	public boolean onSingleTapUp(MotionEvent e){	
		return true;
	}
	// custom private function
	private void initalize(){
	//////
	////////////////////////////
	final String vertexShaderSourceCode=String.format(
	"#version 320 es"+
	"\n"+
	"in vec4 a_position;"+
	"in vec3 a_normal;"+
	"uniform mat4 u_modelMatrix;"+
	"uniform mat4 u_viewMatrix;"+
	"uniform mat4 u_projectionMatrix;"+
	"uniform vec3 u_kd;"+
	"uniform vec3 u_ld;"+
	"uniform vec4 u_lightPosition;"+
	"uniform  mediump int u_lightningEnable;"+
	"out vec3 diffuse_light_color;"+

	"void main()"+
	"{"+
	"if(u_lightningEnable==1)"+
	"{"+
	"vec4 eyeCoordinate=u_viewMatrix*u_modelMatrix*a_position;"+
	"mat3 normalMatrix=mat3(transpose(inverse(u_viewMatrix*u_modelMatrix)));"+
	"vec3 transformedNormals =normalize(normalMatrix*a_normal);"+
	"vec3 lightDirection=normalize(vec3(u_lightPosition-eyeCoordinate));"+
	"diffuse_light_color=u_ld*u_kd*max(dot(lightDirection,transformedNormals ),0.0);"+
	"}"+
	"gl_Position=u_projectionMatrix*u_viewMatrix*u_modelMatrix*a_position;"+
	"}");

	int vertexShaderObject=GLES32.glCreateShader(GLES32.GL_VERTEX_SHADER);
	GLES32.glShaderSource(vertexShaderObject,vertexShaderSourceCode);
	GLES32.glCompileShader(vertexShaderObject);

	int status[]=new int[1];
	int infoLogLength[]=new int[1];
	String log=null;

	GLES32.glGetShaderiv(vertexShaderObject,GLES32.GL_COMPILE_STATUS,status,0);
	if(status[0]==GLES32.GL_FALSE){
		GLES32.glGetShaderiv(vertexShaderObject,GLES32.GL_INFO_LOG_LENGTH,infoLogLength,0);
		if(infoLogLength[0]>0){
			log=GLES32.glGetShaderInfoLog(vertexShaderObject);
				System.out.println("AMC:"+"Vertex Shader Compilation log : "+log);
				uninitalize();
				System.exit(0);
		}
	}

	final String fragmentShaderSourceCode=String.format(
	"#version 320 es"+
	"\n"+
	"precision highp float;"+
	"in vec3 diffuse_light_color;"+
	"uniform mediump int u_lightningEnable;"+
	"out vec4 fragColor;"+
	"void main()"+
	"{"+
	"if(u_lightningEnable==1)"+
	"{"+
	"fragColor=vec4(diffuse_light_color,1.0f);"+
	"}"+
	"else"+
	"{"+
	"fragColor=vec4(1.0f,1.0f,1.0f,1.0f);"+
	"}"+
	"}");
	
	int fragmentShaderObject=GLES32.glCreateShader(GLES32.GL_FRAGMENT_SHADER); 
	GLES32.glShaderSource(fragmentShaderObject,fragmentShaderSourceCode);
	GLES32.glCompileShader(fragmentShaderObject);

	infoLogLength[0]=0;
	status[0]=0;
	log=null;

	GLES32.glGetShaderiv(fragmentShaderObject,GLES32.GL_COMPILE_STATUS,status,0);
	if(status[0]==GLES32.GL_FALSE){
		GLES32.glGetShaderiv(fragmentShaderObject,GLES32.GL_INFO_LOG_LENGTH,infoLogLength,0);
		if(infoLogLength[0]>0){
			log=GLES32.glGetShaderInfoLog(fragmentShaderObject);
			System.out.println("AMC:"+"fragment Shader Compilation log : "+log);
				uninitalize();
				System.exit(0);
		}

	}

	//shader program

	shaderProgramObject=GLES32.glCreateProgram();
	GLES32.glAttachShader(shaderProgramObject,vertexShaderObject);
	GLES32.glAttachShader(shaderProgramObject,fragmentShaderObject);

	//pre linking
	GLES32.glBindAttribLocation(shaderProgramObject,MyGLESMacro.AMC_ATTRIBUTE_POSITION,"a_position");
	GLES32.glBindAttribLocation(shaderProgramObject,MyGLESMacro.AMC_ATTRIBUTE_NORMAL,"a_normal");

	GLES32.glLinkProgram(shaderProgramObject);

	status[0]=0;
	infoLogLength[0]=0;
	log=null;

	GLES32.glGetProgramiv(shaderProgramObject,GLES32.GL_LINK_STATUS,status,0);
	if(status[0]==GLES32.GL_FALSE){
		GLES32.glGetProgramiv(shaderProgramObject,GLES32.GL_INFO_LOG_LENGTH,infoLogLength,0);
		if(infoLogLength[0]>0){
			log=GLES32.glGetProgramInfoLog(shaderProgramObject);
				System.out.println("AMC:"+"Shader program link log : "+log);
				uninitalize();
				System.exit(0);
		}
	}

	//post linking
	modelMatrixUniform=GLES32.glGetUniformLocation(shaderProgramObject,"u_modelMatrix");
	viewMatrixUniform=GLES32.glGetUniformLocation(shaderProgramObject,"u_viewMatrix");
	projectionMatrixUniform=GLES32.glGetUniformLocation(shaderProgramObject,"u_projectionMatrix");

	
	ldUniform = GLES32.glGetUniformLocation(shaderProgramObject, "u_ld");
	kdUniform =GLES32.glGetUniformLocation(shaderProgramObject, "u_kd");
	lightPositionUniform = GLES32.glGetUniformLocation(shaderProgramObject, "u_lightPosition");
	lightEnableUniform = GLES32.glGetUniformLocation(shaderProgramObject, "u_lightningEnable");

	final float pyramidPosition[] =new float[]{
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
		-1.0f, -1.0f, 1.0f
	};

	final float pyramidNormals[]=new float[]{

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
		-0.894427f, 0.447214f, 0.0f // left-right
	};


	//AMC and VBO
	
	GLES32.glGenVertexArrays(1,VAO,0);
	GLES32.glBindVertexArray(VAO[0]);

	GLES32.glGenBuffers(1,VBO,0);
	GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER,VBO[0]);

	ByteBuffer byteBuffer=ByteBuffer.allocateDirect(pyramidPosition.length*4);
	byteBuffer.order(ByteOrder.nativeOrder());

	FloatBuffer positionBuffer=byteBuffer.asFloatBuffer();

	positionBuffer.put(pyramidPosition);;
	positionBuffer.position(0);

	GLES32.glBufferData(GLES32.GL_ARRAY_BUFFER,pyramidPosition.length*4,positionBuffer,GLES32.GL_STATIC_DRAW);
	GLES32.glVertexAttribPointer(MyGLESMacro.AMC_ATTRIBUTE_POSITION,3,GLES32.GL_FLOAT,false,0,0);

	GLES32.glEnableVertexAttribArray(MyGLESMacro.AMC_ATTRIBUTE_POSITION);
	GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER,0);

	//Normal
	GLES32.glGenBuffers(1,VBON,0);
	GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER,VBON[0]);

	ByteBuffer byteBufferNormal=ByteBuffer.allocateDirect(pyramidNormals.length*4);
	byteBufferNormal.order(ByteOrder.nativeOrder());

	FloatBuffer normalBuffer=byteBufferNormal.asFloatBuffer();

	normalBuffer.put(pyramidNormals);;
	normalBuffer.position(0);

	GLES32.glBufferData(GLES32.GL_ARRAY_BUFFER,pyramidNormals.length*4,normalBuffer,GLES32.GL_STATIC_DRAW);
	GLES32.glVertexAttribPointer(MyGLESMacro.AMC_ATTRIBUTE_NORMAL,3,GLES32.GL_FLOAT,false,0,0);

	GLES32.glEnableVertexAttribArray(MyGLESMacro.AMC_ATTRIBUTE_NORMAL);
	GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER,0);

	GLES32.glBindVertexArray(0);

	GLES32.glClearDepthf(1.0f);
	GLES32.glEnable(GLES32.GL_DEPTH_TEST);
	GLES32.glDepthFunc(GLES32.GL_LEQUAL);

	GLES32.glClearColor(0.0f,0.0f,0.0f,1.0f);

	Matrix.setIdentityM(prespectiveProjectionMatrix,0);
	}

	private void resize(int width,int height){
	if(height==0)
		height=1;
	GLES32.glViewport(0,0,width,height);
	Matrix.perspectiveM(prespectiveProjectionMatrix,0,45.0f,(float)width/(float)height,0.1f,100.0f);
	}


	private void update(){
		rotationAngle=rotationAngle+0.5f;
		if(rotationAngle>360.0f)
			rotationAngle=0.0f;
	}
	private void display(){

			
	GLES32.glClear(GLES32.GL_COLOR_BUFFER_BIT|GLES32.GL_DEPTH_BUFFER_BIT);

	GLES32.glUseProgram(shaderProgramObject);
	float translationMatrix[]=new float[16];
	Matrix.setIdentityM(translationMatrix,0);

	float rotationMatrix[]=new float[16];
	Matrix.setIdentityM(rotationMatrix,0);

	float modelMatrix[]=new float[16];
	Matrix.setIdentityM(modelMatrix,0);

	float viewMatrix[]=new float[16];
	Matrix.setIdentityM(viewMatrix,0);

	Matrix.translateM(translationMatrix,0,0.0f,0.0f,-4.0f);
	Matrix.setRotateM(rotationMatrix,0,rotationAngle,0.0f,1.0f,0.0f);

	Matrix.multiplyMM(modelMatrix,0,translationMatrix,0,rotationMatrix,0);

	GLES32.glUniformMatrix4fv(modelMatrixUniform,1,false,modelMatrix,0);
	GLES32.glUniformMatrix4fv(viewMatrixUniform,1,false,viewMatrix,0);
	GLES32.glUniformMatrix4fv(projectionMatrixUniform,1,false,prespectiveProjectionMatrix,0);

	if (bLight == true) {
		GLES32.glUniform1i(lightEnableUniform,1);
		GLES32.glUniform3fv(ldUniform,1,lightDiffuse,0);
		GLES32.glUniform3fv(kdUniform,1,materialDiffuse,0);
		GLES32.glUniform4fv(lightPositionUniform,1,lightPosition,0);
	}
	else
		GLES32.glUniform1i(lightEnableUniform, 0);


	GLES32.glBindVertexArray(VAO[0]);
	GLES32.glDrawArrays(GLES32.GL_TRIANGLES,0,12);

	GLES32.glBindVertexArray(0);
	
	GLES32.glUseProgram(0);


		requestRender();  //same as swapBuffer
	}
	private void uninitalize(){
		//code
		int retVal[]=new int[1];
		if(shaderProgramObject>0){
			GLES32.glUseProgram(shaderProgramObject);
				GLES32.glGetProgramiv(shaderProgramObject,GLES32.GL_ATTACHED_SHADERS,retVal,0);
				if(retVal[0]>0){
					int numAttachedShaders=retVal[0];
				
					int shaderObject[]=new int[numAttachedShaders];
					GLES32.glGetAttachedShaders(shaderProgramObject,numAttachedShaders,retVal,0,shaderObject,0);
					for(int i=0;i<numAttachedShaders;i++){
						GLES32.glDetachShader(shaderProgramObject,shaderObject[i]);
						GLES32.glDeleteShader(shaderObject[i]);
						shaderObject[i]=0;
					}
				
				}
				GLES32.glUseProgram(0);
				GLES32.glDeleteProgram(shaderProgramObject);
				shaderProgramObject=0;
			}
	
	}//end of uninitalize
}
 