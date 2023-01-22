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

import android.opengl.Matrix;

import java.nio.ShortBuffer;
public class GLESView extends GLSurfaceView implements OnDoubleTapListener,OnGestureListener,GLSurfaceView.Renderer{
	private GestureDetector gestureDetector;

	private int shaderProgramObject;

	private int[] vao_sphere = new int[1];
    private int[] vbo_sphere_position = new int[1];
    private int[] vbo_sphere_normal = new int[1];
    private int[] vbo_sphere_element = new int[1];

	private int	numVertices =0;
    private int numElements =0;
	private int modelMatrixUniform;
	private int viewMatrixUniform;
	private int projectionMatrixUniform;

	//light Related Variable
	int ldUniform;
	int kdUniform;
	int lightPositionUniform;
	int lightEnableUniform;
	boolean bLight=false;

	float lightDiffuse[]={1.0f,1.0f,1.0f,1.0f};
	float materialDiffuse[]={0.5f,0.5f,0.5f,1.0f};
	float lightPosition[]={0.0f,0.0f,2.0f,1.0f};








	private float prespectiveProjectionMatrix[]=new float[16];


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
	System.out.println("AMC: "+"Version is "+glEsVersion);

	String glEsRenderer=gl.glGetString(GL10.GL_RENDERER);
	System.out.println("AMC: "+"Renderer is "+glEsRenderer);

	
	String glSlVersion=gl.glGetString(GLES32.GL_SHADING_LANGUAGE_VERSION);
	System.out.println("AMC: "+"glSL is  "+glSlVersion);


		initalize();
			
			}
		
	@Override
	public void onSurfaceChanged(GL10 unUsed,int width,int height){ //same as resize
		resize(width,height);	
	}

	@Override
	public void onDrawFrame(GL10 unUsed){  //display
		//on drawFrame consider as game loop
		display();
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
			if(bLight==false)
			{
				 bLight=true;
					System.out.println("AMC:"+"T : ");
				}else{
			bLight=false;
					System.out.println("AMC:"+"F : ");
			
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

	final String vertexShaderSourceCode=String.format(
	"#version 320 es"+
	"\n"+
	"in vec4 a_position;"+
	"in vec3 a_normal;"+
	
	"uniform mat4 u_modelMatrix;"+
	"uniform mat4 u_viewMatrix;"+
	"uniform mat4 u_projectionMatrix;"+

	"uniform vec3 u_ld;"+
	"uniform vec3 u_kd;"+
	"uniform vec4 u_lightPosition;"+
	"uniform  mediump int u_lightningEnable;"+
	"out  vec3 diffuse_ligh_color;"+
	"void main(void)"+
	"{"+
	"if(u_lightningEnable==1)"+
	"{"+
	"vec4 eyeCoordinate=u_viewMatrix*u_modelMatrix*a_position;"+
	"mat3 normalMatrix=mat3(transpose(inverse(u_viewMatrix*u_modelMatrix)));"+
	"vec3 transformNormal=normalize(normalMatrix*a_normal);"+
	"vec3 lightDirection=normalize(vec3(u_lightPosition-eyeCoordinate));"+
	"diffuse_ligh_color=u_ld*u_kd*max(dot(lightDirection,transformNormal),0.0);"+
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
			System.out.println("AMC: "+"Vertex Shader Compilation Log "+log );
			uninitalize();
			System.exit(0);	
		}
	}
	//fragment Shader
	final String fragmentShaderSourceCode=String.format(
		"#version 320 es"+
		"\n"+
		"precision highp float;"+
		"in vec3 diffuse_ligh_color;"+
		"uniform mediump int u_lightningEnable;"+

		"out vec4 fragColor;"+
		"void main()"+
		"{"+
		"if(u_lightningEnable==1)"+
		"{"+
		"fragColor=vec4(diffuse_ligh_color,1.0f);"+
		"}"+
		"else"+
		"{"+
		"fragColor=vec4(1.0f,1.0f,1.0f,1.0f);"+
		"}"+
		"}"); 

		int fragmentShaderObject=GLES32.glCreateShader(GLES32.GL_FRAGMENT_SHADER);
		GLES32.glShaderSource(fragmentShaderObject,fragmentShaderSourceCode);
		GLES32.glCompileShader(fragmentShaderObject);

		status[0]=0;
		infoLogLength[0]=0;
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

		//shaderProgram
		 shaderProgramObject= GLES32.glCreateProgram();
		 GLES32.glAttachShader(shaderProgramObject,vertexShaderObject);
		 GLES32.glAttachShader(shaderProgramObject,fragmentShaderObject);

		
		 GLES32.glBindAttribLocation(shaderProgramObject,MyGLESMacro.AMC_ATTRIBUTE_POSITION,"a_position");
		 GLES32.glBindAttribLocation(shaderProgramObject,MyGLESMacro.AMC_ATTRIBUTE_NORMAL,"a_normal");
		 GLES32.glLinkProgram(shaderProgramObject);
		  infoLogLength[0]=0;
		 status[0]=0;
		 log=null;
		 GLES32.glGetShaderiv(shaderProgramObject,GLES32.GL_LINK_STATUS,status,0);
		 if(status[0]==GLES32.GL_FALSE){
				GLES32.glGetShaderiv(shaderProgramObject,GLES32.GL_INFO_LOG_LENGTH,infoLogLength,0);
				if(infoLogLength[0]>0){
					log=GLES32.glGetShaderInfoLog(shaderProgramObject);
					System.out.println("AMC:"+"Shader program link log : "+log);
					uninitalize();
					System.exit(0);
					
					}
		 }

		 //post linking 
		modelMatrixUniform= GLES32.glGetUniformLocation(shaderProgramObject,"u_modelMatrix");
		viewMatrixUniform=GLES32.glGetUniformLocation(shaderProgramObject,"u_viewMatrix");
		projectionMatrixUniform=GLES32.glGetUniformLocation(shaderProgramObject,"u_projectionMatrix");
		
		ldUniform=GLES32.glGetUniformLocation(shaderProgramObject,"u_ld");
		kdUniform=GLES32.glGetUniformLocation(shaderProgramObject,"u_kd");
		lightEnableUniform=GLES32.glGetUniformLocation(shaderProgramObject,"u_lightningEnable");
		lightPositionUniform=GLES32.glGetUniformLocation(shaderProgramObject,"u_lightPosition");
		
		


        Sphere sphere=new Sphere();
        float sphere_vertices[]=new float[1146];
        float sphere_normals[]=new float[1146];
        float sphere_textures[]=new float[764];
        short sphere_elements[]=new short[2280];

        sphere.getSphereVertexData(sphere_vertices, sphere_normals, sphere_textures, sphere_elements);
         numVertices = sphere.getNumberOfSphereVertices();
         numElements = sphere.getNumberOfSphereElements();

        // vao
        GLES32.glGenVertexArrays(1,vao_sphere,0);
        GLES32.glBindVertexArray(vao_sphere[0]);
        
        // position vbo
        GLES32.glGenBuffers(1,vbo_sphere_position,0);
        GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER,vbo_sphere_position[0]);
        
        ByteBuffer byteBuffer=ByteBuffer.allocateDirect(sphere_vertices.length * 4);
        byteBuffer.order(ByteOrder.nativeOrder());
        FloatBuffer verticesBuffer=byteBuffer.asFloatBuffer();
        verticesBuffer.put(sphere_vertices);
        verticesBuffer.position(0);
        
        GLES32.glBufferData(GLES32.GL_ARRAY_BUFFER,
                            sphere_vertices.length * 4,
                            verticesBuffer,
                            GLES32.GL_STATIC_DRAW);
        
        GLES32.glVertexAttribPointer(MyGLESMacro.AMC_ATTRIBUTE_POSITION,
                                     3,
                                     GLES32.GL_FLOAT,
                                     false,0,0);
        
        GLES32.glEnableVertexAttribArray(MyGLESMacro.AMC_ATTRIBUTE_POSITION);
        
        GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER,0);
        
        // normal vbo
        GLES32.glGenBuffers(1,vbo_sphere_normal,0);
        GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER,vbo_sphere_normal[0]);
        
        byteBuffer=ByteBuffer.allocateDirect(sphere_normals.length * 4);
        byteBuffer.order(ByteOrder.nativeOrder());
        verticesBuffer=byteBuffer.asFloatBuffer();
        verticesBuffer.put(sphere_normals);
        verticesBuffer.position(0);
        
        GLES32.glBufferData(GLES32.GL_ARRAY_BUFFER,
                            sphere_normals.length * 4,
                            verticesBuffer,
                            GLES32.GL_STATIC_DRAW);
        
        GLES32.glVertexAttribPointer(MyGLESMacro.AMC_ATTRIBUTE_NORMAL,
                                     3,
                                     GLES32.GL_FLOAT,
                                     false,0,0);
        
        GLES32.glEnableVertexAttribArray(MyGLESMacro.AMC_ATTRIBUTE_NORMAL);
        
        GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER,0);
        
        // element vbo
        GLES32.glGenBuffers(1,vbo_sphere_element,0);
        GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER,vbo_sphere_element[0]);
        
        byteBuffer=ByteBuffer.allocateDirect(sphere_elements.length * 2);
        byteBuffer.order(ByteOrder.nativeOrder());
        ShortBuffer elementsBuffer=byteBuffer.asShortBuffer();
        elementsBuffer.put(sphere_elements);
        elementsBuffer.position(0);
        
        GLES32.glBufferData(GLES32.GL_ELEMENT_ARRAY_BUFFER,
                            sphere_elements.length * 2,
                            elementsBuffer,
                            GLES32.GL_STATIC_DRAW);
        
        GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER,0);

        GLES32.glBindVertexArray(0);


			GLES32.glClearDepthf(1.0f);
			GLES32.glEnable(GLES32.GL_DEPTH_TEST);
			GLES32.glDepthFunc(GLES32.GL_LEQUAL);

			//GLES32.glEnable(GLES32.GL_CULL_FACE);
			GLES32.glClearColor(0.0f,0.0f,0.0f,1.0f);
			Matrix.setIdentityM(prespectiveProjectionMatrix,0);

	}
	private void resize(int width,int height){
		if(height==0)
			height=1;
		GLES32.glViewport(0,0,width,height);
		Matrix.perspectiveM(prespectiveProjectionMatrix,0,45.0f,(float)width/(float)height,0.1f,100.0f);

	}
	private void display(){
					//System.out.println("AMC:"+"log : ");

		GLES32.glClear(GLES32.GL_COLOR_BUFFER_BIT |GLES32.GL_DEPTH_BUFFER_BIT );
		
		GLES32.glUseProgram(shaderProgramObject);

		float modelMatrix[]=new float[16];
		Matrix.setIdentityM(modelMatrix,0);

		float translationMatrix[]=new float[16];
		Matrix.setIdentityM(translationMatrix,0);

		float viewMatrix[]=new float[16];
		Matrix.setIdentityM(viewMatrix,0);


		Matrix.translateM(translationMatrix,0,0.0f,0.0f,-2.0f);
		modelMatrix=translationMatrix;

		
		GLES32.glUniformMatrix4fv(modelMatrixUniform,1,false,modelMatrix,0);
		GLES32.glUniformMatrix4fv(viewMatrixUniform,1,false,viewMatrix,0);
		GLES32.glUniformMatrix4fv(projectionMatrixUniform,1,false,prespectiveProjectionMatrix,0);


		if(bLight==true){
			GLES32.glUniform1i(lightEnableUniform,1);
			GLES32.glUniform3fv(ldUniform,1,lightDiffuse,0);
			GLES32.glUniform3fv(kdUniform,1,materialDiffuse,0);
			GLES32.glUniform4fv(lightPositionUniform,1,lightPosition,0);
		}
		else
		GLES32.glUniform1i(lightEnableUniform,0);
		// bind vao
        GLES32.glBindVertexArray(vao_sphere[0]);
        	
        // *** draw, either by glDrawTriangles() or glDrawArrays() or glDrawElements()
        GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, vbo_sphere_element[0]);
        GLES32.glDrawElements(GLES32.GL_TRIANGLES, numElements, GLES32.GL_UNSIGNED_SHORT, 0);
        

        // unbind vao
        GLES32.glBindVertexArray(0);

		GLES32.glUseProgram(0);
				//	System.out.println("AMC:"+"ffff : ");

		requestRender();
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
	 // destroy vao
        if(vao_sphere[0] != 0)
        {
            GLES32.glDeleteVertexArrays(1, vao_sphere, 0);
            vao_sphere[0]=0;
        }
        
        // destroy position vbo
        if(vbo_sphere_position[0] != 0)
        {
            GLES32.glDeleteBuffers(1, vbo_sphere_position, 0);
            vbo_sphere_position[0]=0;
        }
        
        // destroy normal vbo
        if(vbo_sphere_normal[0] != 0)
        {
            GLES32.glDeleteBuffers(1, vbo_sphere_normal, 0);
            vbo_sphere_normal[0]=0;
        }
        
        // destroy element vbo
        if(vbo_sphere_element[0] != 0)
        {
            GLES32.glDeleteBuffers(1, vbo_sphere_element, 0);
            vbo_sphere_element[0]=0;
        }
	
	
	}//end of uninitalize
}
 