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
	int  laUniform;
	int ldUniform;
	int lsUniform;
	int lightPositionUniform;

	int kaUniform;
	int kdUniform;
	int ksUniform;
	int materialShininessUniform;

	int lightEnableUniform;
	
	boolean bLight=false;

	float lightAmbiant[]={0.0f,0.0f,0.0f,1.0f};
	float  lightDiffuse[]={1.0f,1.0f,1.0f,1.0f};
	float lightSpecular[]={1.0f,1.0f,1.0f,1.0f};
	float lightPosition[]={100.0f,100.0f,100.0f,1.0f};



	float xRotation = 0.0f;
float yRotation = 0.0f;
float zRotation = 0.0f;
int keyPress = 0;
float radius = 5.0f;
int screenWidth,screenHeight;




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
		keyPress++;
		if(keyPress>=4)
		keyPress=0;
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

	"uniform mat4 u_projectionMatrix;"+
	"uniform mat4 u_modelMatrix;"+
	"uniform mat4 u_viewMatrix;"+
	
	"uniform vec3 u_la;"+
	"uniform vec3 u_ld;"+
	"uniform vec3 u_ls;"+
	"uniform vec4 u_lightPosition;"+
	
	"uniform vec3 u_ka;"+
	"uniform vec3 u_kd;"+
	"uniform vec3 u_ks;"+
	"uniform float u_materialShininess;"+
	"uniform int u_lightEnable;"+
	"out vec3 pong_ads_light;"+
	"void main()"+
	"{"+
	"if(u_lightEnable==1)"+
	"{"+
	"vec3 ambiant=u_la*u_ka;"+
	"vec4 eyeCoordinate=u_viewMatrix*u_modelMatrix*a_position;"+
	"mat3 normalMatrix=mat3(u_viewMatrix*u_modelMatrix);"+
	"vec3 transformNormal=normalize(normalMatrix*a_normal);"+
	"vec3 lightDirection=normalize(vec3(u_lightPosition)-eyeCoordinate.xyz);"+
	"vec3 diffuse=u_ld*u_kd*max(dot(lightDirection,transformNormal),0.0);"+
	
	"vec3 reflectionVector=reflect(-lightDirection,transformNormal);"+
	"vec3 viewerVector=normalize(-eyeCoordinate.xyz);"+
	"vec3 specular=u_ls*u_ks*pow(max(dot(reflectionVector,viewerVector),0.0f),u_materialShininess);"+
	"pong_ads_light=ambiant+diffuse+specular;"+
	"}"+
	"else"+
	"{"+
	"pong_ads_light=vec3(1.0f,1.0f,1.0f);"+
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
		"#version 320 es" +
		"\n" +
		"precision highp float;"+
		"in vec3 pong_ads_light;"+
		
		"out vec4 FragColor;"+
		"void main(void)"+
		"{"+
		"FragColor=vec4(pong_ads_light,1.0f);"+
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
		
		
		laUniform=GLES32.glGetUniformLocation(shaderProgramObject,"u_la");
		ldUniform=GLES32.glGetUniformLocation(shaderProgramObject,"u_ld");
		lsUniform=GLES32.glGetUniformLocation(shaderProgramObject,"u_ls");
		lightPositionUniform=GLES32.glGetUniformLocation(shaderProgramObject,"u_lightPosition");
	
		kaUniform=GLES32.glGetUniformLocation(shaderProgramObject,"u_ka");
		kdUniform=GLES32.glGetUniformLocation(shaderProgramObject,"u_kd");
		ksUniform=GLES32.glGetUniformLocation(shaderProgramObject,"u_ks");

		materialShininessUniform=GLES32.glGetUniformLocation(shaderProgramObject,"u_materialShininess");
		lightEnableUniform=GLES32.glGetUniformLocation(shaderProgramObject,"u_lightEnable");



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
			GLES32.glClearColor(0.5f,0.5f,0.5f,1.0f);
			Matrix.setIdentityM(prespectiveProjectionMatrix,0);

	}
	private void resize(int width,int height){
		if(height==0)
			height=1;
			screenHeight=height;
			screenWidth=width;
		GLES32.glViewport(0,0,width,height);
		Matrix.perspectiveM(prespectiveProjectionMatrix,0,45.0f,(float)width/(float)height,0.1f,100.0f);

	}
	private void display(){
					//System.out.println("AMC:"+"log : ");
	
		GLES32.glClear(GLES32.GL_COLOR_BUFFER_BIT |GLES32.GL_DEPTH_BUFFER_BIT );
		if (keyPress == 1) {
		lightPosition[0] = 0.0f;
		lightPosition[1] = (float)Math.cos(xRotation) * radius;
		lightPosition[2] = (float)Math.sin(xRotation) * radius;
	}
	else if (keyPress == 2) {
			lightPosition[0] =(float) Math.cos(yRotation) * radius;
			lightPosition[1] =0.0f;
			lightPosition[2] = (float)Math.sin(yRotation) * radius;
	}
	else if (keyPress == 3) {
		lightPosition[0] = (float)Math.cos(zRotation) * radius;
		lightPosition[1] = (float)Math.sin(zRotation) * radius;
		lightPosition[2] = 0.0f;
	}
	draw24Sphere();
				//	System.out.println("AMC:"+"ffff : ");

		requestRender();
	}
	void draw24Sphere(){
	float materialAmbient[]=new float[4];
	float materialDiffuse[]=new float[4];
	float materialSpecular[]=new float[4];
	float materialShininess;




	GLES32.glUseProgram(shaderProgramObject);

	// code
	// ** 1st sphere on 1st column, emerald **
	// ambient material
	materialAmbient[0] = (float)0.0215f; // r
	materialAmbient[1] = (float)0.1745f; // g
	materialAmbient[2] = (float)0.0215f; // b
	materialAmbient[3] = (float)1.0f;   // a

	// diffuse material
	materialDiffuse[0] = 0.07568f; // r
	materialDiffuse[1] = 0.61424f; // g
	materialDiffuse[2] = 0.07568f; // b
	materialDiffuse[3] = 1.0f;    // a

	// specular material
	materialSpecular[0] = 0.633f;    // r
	materialSpecular[1] = 0.727811f; // g
	materialSpecular[2] = 0.633f;    // b
	materialSpecular[3] = 1.0f;     // a

	// shininess
	materialShininess = 0.6f * (float)128;
		float modelMatrix[]=new float[16];
		Matrix.setIdentityM(modelMatrix,0);

		float translationMatrix[]=new float[16];
		Matrix.setIdentityM(translationMatrix,0);

		float viewMatrix[]=new float[16];
		Matrix.setIdentityM(viewMatrix,0);

		
		
		Matrix.translateM(translationMatrix,0,0.0f,0.0f,-2.0f);
	;
modelMatrix=translationMatrix;


		GLES32.glUniformMatrix4fv(modelMatrixUniform,1,false,modelMatrix,0);
		GLES32.glUniformMatrix4fv(viewMatrixUniform,1,false,viewMatrix,0);
		GLES32.glUniformMatrix4fv(projectionMatrixUniform,1,false,prespectiveProjectionMatrix,0);

		GLES32.glViewport(0,(screenHeight/6)*5,(screenWidth/6),(screenHeight/6));
		if(bLight==true){
			GLES32.glUniform1i(lightEnableUniform,1);

			GLES32.glUniform3fv(laUniform,1,lightAmbiant,0);
			GLES32.glUniform3fv(ldUniform,1,lightDiffuse,0);
			GLES32.glUniform3fv(lsUniform,1,lightSpecular,0);
			GLES32.glUniform4fv(lightPositionUniform,1,lightPosition,0);

			GLES32.glUniform3fv(kaUniform,1,materialAmbient,0);
			GLES32.glUniform3fv(kdUniform,1,materialDiffuse,0);
			GLES32.glUniform3fv(ksUniform,1,materialSpecular,0);

			GLES32.glUniform1f(materialShininessUniform,materialShininess);

		
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


		
	// ** 2nd sphere on 1st column, jade **
	// ambient material
	materialAmbient[0] = 0.135f;  // r
	materialAmbient[1] = 0.2225f; // g
	materialAmbient[2] = 0.1575f; // b
	materialAmbient[3] = 1.0f;   // a

	// diffuse material
	materialDiffuse[0] = 0.54f; // r
	materialDiffuse[1] = 0.89f; // g
	materialDiffuse[2] = 0.63f; // b
	materialDiffuse[3] = 1.0f; // a

	// specular material
	materialSpecular[0] = 0.316228f; // r
	materialSpecular[1] = 0.316228f; // g
	materialSpecular[2] = 0.316228f; // b
	materialSpecular[3] = 1.0f;     // a

	// shininess
	materialShininess = 0.1f * 128f;

	
		Matrix.setIdentityM(modelMatrix,0);
		Matrix.setIdentityM(translationMatrix,0);
		Matrix.setIdentityM(viewMatrix,0);

		
		
		Matrix.translateM(translationMatrix,0,0.0f,0.0f,-2.0f);
	;
		modelMatrix=translationMatrix;



		GLES32.glUniformMatrix4fv(modelMatrixUniform,1,false,modelMatrix,0);
		GLES32.glUniformMatrix4fv(viewMatrixUniform,1,false,viewMatrix,0);
		GLES32.glUniformMatrix4fv(projectionMatrixUniform,1,false,prespectiveProjectionMatrix,0);

		GLES32.glViewport(0,(screenHeight/6)*4,(screenWidth/6),(screenHeight/6));
		if(bLight==true){
			GLES32.glUniform1i(lightEnableUniform,1);

			GLES32.glUniform3fv(laUniform,1,lightAmbiant,0);
			GLES32.glUniform3fv(ldUniform,1,lightDiffuse,0);
			GLES32.glUniform3fv(lsUniform,1,lightSpecular,0);
			GLES32.glUniform4fv(lightPositionUniform,1,lightPosition,0);

			GLES32.glUniform3fv(kaUniform,1,materialAmbient,0);
			GLES32.glUniform3fv(kdUniform,1,materialDiffuse,0);
			GLES32.glUniform3fv(ksUniform,1,materialSpecular,0);

			GLES32.glUniform1f(materialShininessUniform,materialShininess);

		
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



	// ** 3rd sphere on 1st column, obsidian **
	// ambient material
	materialAmbient[0] = 0.05375f; // r
	materialAmbient[1] = 0.05f;    // g
	materialAmbient[2] = 0.06625f; // b
	materialAmbient[3] = 1.0f;    // a

	// diffuse material
	materialDiffuse[0] = 0.18275f; // r
	materialDiffuse[1] = 0.17f;    // g
	materialDiffuse[2] = 0.22525f; // b
	materialDiffuse[3] = 1.0f;    // a

	// specular material
	materialSpecular[0] = 0.332741f; // r
	materialSpecular[1] = 0.328634f; // g
	materialSpecular[2] = 0.346435f; // b
	materialSpecular[3] = 1.0f;     // a
	
	// shininess
	materialShininess = 0.3f * 128f;

	Matrix.setIdentityM(modelMatrix,0);
		Matrix.setIdentityM(translationMatrix,0);
		Matrix.setIdentityM(viewMatrix,0);

		
		
		Matrix.translateM(translationMatrix,0,0.0f,0.0f,-2.0f);
	;
		modelMatrix=translationMatrix;



		GLES32.glUniformMatrix4fv(modelMatrixUniform,1,false,modelMatrix,0);
		GLES32.glUniformMatrix4fv(viewMatrixUniform,1,false,viewMatrix,0);
		GLES32.glUniformMatrix4fv(projectionMatrixUniform,1,false,prespectiveProjectionMatrix,0);

		GLES32.glViewport(0,(screenHeight/6)*3,(screenWidth/6),(screenHeight/6));
		if(bLight==true){
			GLES32.glUniform1i(lightEnableUniform,1);

			GLES32.glUniform3fv(laUniform,1,lightAmbiant,0);
			GLES32.glUniform3fv(ldUniform,1,lightDiffuse,0);
			GLES32.glUniform3fv(lsUniform,1,lightSpecular,0);
			GLES32.glUniform4fv(lightPositionUniform,1,lightPosition,0);

			GLES32.glUniform3fv(kaUniform,1,materialAmbient,0);
			GLES32.glUniform3fv(kdUniform,1,materialDiffuse,0);
			GLES32.glUniform3fv(ksUniform,1,materialSpecular,0);

			GLES32.glUniform1f(materialShininessUniform,materialShininess);

		
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




	// *******************

	// ** 4th sphere on 1st column, pearl **
	// ambient material
	materialAmbient[0] = 0.25f;    // r
	materialAmbient[1] = 0.20725f; // g
	materialAmbient[2] = 0.20725f; // b
	materialAmbient[3] = 1.0f;    // a

	// diffuse material
	materialDiffuse[0] = 1.0f;   // r
	materialDiffuse[1] = 0.829f; // g
	materialDiffuse[2] = 0.829f; // b
	materialDiffuse[3] = 1.0f;  // a

	// specular material
	materialSpecular[0] = 0.296648f; // r
	materialSpecular[1] = 0.296648f; // g
	materialSpecular[2] = 0.296648f; // b
	materialSpecular[3] = 1.0f;     // a

	// shininess
	materialShininess = 0.088f * 128f;

		Matrix.setIdentityM(modelMatrix,0);
		Matrix.setIdentityM(translationMatrix,0);
		Matrix.setIdentityM(viewMatrix,0);

		
		
		Matrix.translateM(translationMatrix,0,0.0f,0.0f,-2.0f);
	;
		modelMatrix=translationMatrix;



		GLES32.glUniformMatrix4fv(modelMatrixUniform,1,false,modelMatrix,0);
		GLES32.glUniformMatrix4fv(viewMatrixUniform,1,false,viewMatrix,0);
		GLES32.glUniformMatrix4fv(projectionMatrixUniform,1,false,prespectiveProjectionMatrix,0);

		GLES32.glViewport(0,(screenHeight/6)*2,(screenWidth/6),(screenHeight/6));
		if(bLight==true){
			GLES32.glUniform1i(lightEnableUniform,1);

			GLES32.glUniform3fv(laUniform,1,lightAmbiant,0);
			GLES32.glUniform3fv(ldUniform,1,lightDiffuse,0);
			GLES32.glUniform3fv(lsUniform,1,lightSpecular,0);
			GLES32.glUniform4fv(lightPositionUniform,1,lightPosition,0);

			GLES32.glUniform3fv(kaUniform,1,materialAmbient,0);
			GLES32.glUniform3fv(kdUniform,1,materialDiffuse,0);
			GLES32.glUniform3fv(ksUniform,1,materialSpecular,0);

			GLES32.glUniform1f(materialShininessUniform,materialShininess);

		
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


	// *******************

	// ** 5th sphere on 1st column, ruby **
	// ambient material
	materialAmbient[0] = 0.1745f;  // r
	materialAmbient[1] = 0.01175f; // g
	materialAmbient[2] = 0.01175f; // b
	materialAmbient[3] = 1.0f;    // a
	
	// diffuse material
	materialDiffuse[0] = 0.61424f; // r
	materialDiffuse[1] = 0.04136f; // g
	materialDiffuse[2] = 0.04136f; // b
	materialDiffuse[3] = 1.0f;    // a


	// specular material
	materialSpecular[0] = 0.727811f; // r
	materialSpecular[1] = 0.626959f; // g
	materialSpecular[2] = 0.626959f; // b
	materialSpecular[3] = 1.0f;     // a


	// shininess
	materialShininess = 0.6f * 128f;


	Matrix.setIdentityM(modelMatrix,0);
		Matrix.setIdentityM(translationMatrix,0);
		Matrix.setIdentityM(viewMatrix,0);

		
		
		Matrix.translateM(translationMatrix,0,0.0f,0.0f,-2.0f);
	;
		modelMatrix=translationMatrix;



		GLES32.glUniformMatrix4fv(modelMatrixUniform,1,false,modelMatrix,0);
		GLES32.glUniformMatrix4fv(viewMatrixUniform,1,false,viewMatrix,0);
		GLES32.glUniformMatrix4fv(projectionMatrixUniform,1,false,prespectiveProjectionMatrix,0);

		GLES32.glViewport(0,(screenHeight/6),(screenWidth/6),(screenHeight/6));
		if(bLight==true){
			GLES32.glUniform1i(lightEnableUniform,1);

			GLES32.glUniform3fv(laUniform,1,lightAmbiant,0);
			GLES32.glUniform3fv(ldUniform,1,lightDiffuse,0);
			GLES32.glUniform3fv(lsUniform,1,lightSpecular,0);
			GLES32.glUniform4fv(lightPositionUniform,1,lightPosition,0);

			GLES32.glUniform3fv(kaUniform,1,materialAmbient,0);
			GLES32.glUniform3fv(kdUniform,1,materialDiffuse,0);
			GLES32.glUniform3fv(ksUniform,1,materialSpecular,0);

			GLES32.glUniform1f(materialShininessUniform,materialShininess);

		
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





	// *******************

	// ** 6th sphere on 1st column, turquoise **
	// ambient material
	materialAmbient[0] = 0.1f;     // r
	materialAmbient[1] = 0.18725f; // g
	materialAmbient[2] = 0.1745f;  // b
	materialAmbient[3] = 1.0f;    // a
	

	// diffuse material
	materialDiffuse[0] = 0.396f;   // r
	materialDiffuse[1] = 0.74151f; // g
	materialDiffuse[2] = 0.69102f; // b
	materialDiffuse[3] = 1.0f;    // a


	// specular material
	materialSpecular[0] = 0.297254f; // r
	materialSpecular[1] = 0.30829f;  // g
	materialSpecular[2] = 0.306678f; // b
	materialSpecular[3] = 1.0f;     // a
	

	// shininess
	materialShininess = 0.1f * 128f;


	Matrix.setIdentityM(modelMatrix,0);
		Matrix.setIdentityM(translationMatrix,0);
		Matrix.setIdentityM(viewMatrix,0);

		
		
		Matrix.translateM(translationMatrix,0,0.0f,0.0f,-2.0f);
	;
		modelMatrix=translationMatrix;



		GLES32.glUniformMatrix4fv(modelMatrixUniform,1,false,modelMatrix,0);
		GLES32.glUniformMatrix4fv(viewMatrixUniform,1,false,viewMatrix,0);
		GLES32.glUniformMatrix4fv(projectionMatrixUniform,1,false,prespectiveProjectionMatrix,0);

		GLES32.glViewport(0,0,(screenWidth/6),(screenHeight/6));
		if(bLight==true){
			GLES32.glUniform1i(lightEnableUniform,1);

			GLES32.glUniform3fv(laUniform,1,lightAmbiant,0);
			GLES32.glUniform3fv(ldUniform,1,lightDiffuse,0);
			GLES32.glUniform3fv(lsUniform,1,lightSpecular,0);
			GLES32.glUniform4fv(lightPositionUniform,1,lightPosition,0);

			GLES32.glUniform3fv(kaUniform,1,materialAmbient,0);
			GLES32.glUniform3fv(kdUniform,1,materialDiffuse,0);
			GLES32.glUniform3fv(ksUniform,1,materialSpecular,0);

			GLES32.glUniform1f(materialShininessUniform,materialShininess);

		
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


	

	 // ** 1st sphere on 2nd column, brass **
	 // ambient material
	materialAmbient[0] = 0.329412f; // r
	materialAmbient[1] = 0.223529f; // g
	materialAmbient[2] = 0.027451f; // b
	materialAmbient[3] = 1.0f;     // a

	// diffuse material
	materialDiffuse[0] = 0.780392f; // r
	materialDiffuse[1] = 0.568627f; // g
	materialDiffuse[2] = 0.113725f; // b
	materialDiffuse[3] = 1.0f;     // a

	// specular material
	materialSpecular[0] = 0.992157f; // r
	materialSpecular[1] = 0.941176f; // g
	materialSpecular[2] = 0.807843f; // b
	materialSpecular[3] = 1.0f;     // a

	// shininess
	materialShininess = 0.21794872f * 128f;

	Matrix.setIdentityM(modelMatrix,0);
		Matrix.setIdentityM(translationMatrix,0);
		Matrix.setIdentityM(viewMatrix,0);

		
		
		Matrix.translateM(translationMatrix,0,0.0f,0.0f,-2.0f);
	;
		modelMatrix=translationMatrix;



		GLES32.glUniformMatrix4fv(modelMatrixUniform,1,false,modelMatrix,0);
		GLES32.glUniformMatrix4fv(viewMatrixUniform,1,false,viewMatrix,0);
		GLES32.glUniformMatrix4fv(projectionMatrixUniform,1,false,prespectiveProjectionMatrix,0);

		GLES32.glViewport(screenWidth/4,(screenHeight/6)*5,(screenWidth/6),(screenHeight/6));
		if(bLight==true){
			GLES32.glUniform1i(lightEnableUniform,1);

			GLES32.glUniform3fv(laUniform,1,lightAmbiant,0);
			GLES32.glUniform3fv(ldUniform,1,lightDiffuse,0);
			GLES32.glUniform3fv(lsUniform,1,lightSpecular,0);
			GLES32.glUniform4fv(lightPositionUniform,1,lightPosition,0);

			GLES32.glUniform3fv(kaUniform,1,materialAmbient,0);
			GLES32.glUniform3fv(kdUniform,1,materialDiffuse,0);
			GLES32.glUniform3fv(ksUniform,1,materialSpecular,0);

			GLES32.glUniform1f(materialShininessUniform,materialShininess);

		
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


	
	// *******************



	// ** 2nd sphere on 2nd column, bronze **
	// ambient material
	materialAmbient[0] = 0.2125f; // r
	materialAmbient[1] = 0.1275f; // g
	materialAmbient[2] = 0.054f;  // b
	materialAmbient[3] = 1.0f;   // a

	// diffuse material
	materialDiffuse[0] = 0.714f;   // r
	materialDiffuse[1] = 0.4284f;  // g
	materialDiffuse[2] = 0.18144f; // b
	materialDiffuse[3] = 1.0f;    // a

	// specular material
	materialSpecular[0] = 0.393548f; // r
	materialSpecular[1] = 0.271906f; // g
	materialSpecular[2] = 0.166721f; // b
	materialSpecular[3] = 1.0f;     // a

	// shininess
	materialShininess = 0.2f * 128f;

	// geometry
Matrix.setIdentityM(modelMatrix,0);
		Matrix.setIdentityM(translationMatrix,0);
		Matrix.setIdentityM(viewMatrix,0);

		
		
		Matrix.translateM(translationMatrix,0,0.0f,0.0f,-2.0f);
	;
		modelMatrix=translationMatrix;



		GLES32.glUniformMatrix4fv(modelMatrixUniform,1,false,modelMatrix,0);
		GLES32.glUniformMatrix4fv(viewMatrixUniform,1,false,viewMatrix,0);
		GLES32.glUniformMatrix4fv(projectionMatrixUniform,1,false,prespectiveProjectionMatrix,0);

		GLES32.glViewport(screenWidth/4,(screenHeight/6)*4,(screenWidth/6),(screenHeight/6));
		if(bLight==true){
			GLES32.glUniform1i(lightEnableUniform,1);

			GLES32.glUniform3fv(laUniform,1,lightAmbiant,0);
			GLES32.glUniform3fv(ldUniform,1,lightDiffuse,0);
			GLES32.glUniform3fv(lsUniform,1,lightSpecular,0);
			GLES32.glUniform4fv(lightPositionUniform,1,lightPosition,0);

			GLES32.glUniform3fv(kaUniform,1,materialAmbient,0);
			GLES32.glUniform3fv(kdUniform,1,materialDiffuse,0);
			GLES32.glUniform3fv(ksUniform,1,materialSpecular,0);

			GLES32.glUniform1f(materialShininessUniform,materialShininess);

		
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

	// *******************

	// ** 3rd sphere on 2nd column, chrome **
	// ambient material
	materialAmbient[0] = 0.25f; // r
	materialAmbient[1] = 0.25f; // g
	materialAmbient[2] = 0.25f; // b
	materialAmbient[3] = 1.0f; // a
	

	// diffuse material
	materialDiffuse[0] = 0.4f;  // r
	materialDiffuse[1] = 0.4f;  // g
	materialDiffuse[2] = 0.4f;  // b
	materialDiffuse[3] = 1.0f; // a


	// specular material
	materialSpecular[0] = 0.774597f; // r
	materialSpecular[1] = 0.774597f; // g
	materialSpecular[2] = 0.774597f; // b
	materialSpecular[3] = 1.0f;     // a
	;

	// shininess
	materialShininess = 0.6f * 128f;
	
	// geometry
Matrix.setIdentityM(modelMatrix,0);
		Matrix.setIdentityM(translationMatrix,0);
		Matrix.setIdentityM(viewMatrix,0);

		
		
		Matrix.translateM(translationMatrix,0,0.0f,0.0f,-2.0f);
	;
		modelMatrix=translationMatrix;



		GLES32.glUniformMatrix4fv(modelMatrixUniform,1,false,modelMatrix,0);
		GLES32.glUniformMatrix4fv(viewMatrixUniform,1,false,viewMatrix,0);
		GLES32.glUniformMatrix4fv(projectionMatrixUniform,1,false,prespectiveProjectionMatrix,0);

		GLES32.glViewport(screenWidth/4,(screenHeight/6)*3,(screenWidth/6),(screenHeight/6));
		if(bLight==true){
			GLES32.glUniform1i(lightEnableUniform,1);

			GLES32.glUniform3fv(laUniform,1,lightAmbiant,0);
			GLES32.glUniform3fv(ldUniform,1,lightDiffuse,0);
			GLES32.glUniform3fv(lsUniform,1,lightSpecular,0);
			GLES32.glUniform4fv(lightPositionUniform,1,lightPosition,0);

			GLES32.glUniform3fv(kaUniform,1,materialAmbient,0);
			GLES32.glUniform3fv(kdUniform,1,materialDiffuse,0);
			GLES32.glUniform3fv(ksUniform,1,materialSpecular,0);

			GLES32.glUniform1f(materialShininessUniform,materialShininess);

		
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

	// *******************

	// ** 4th sphere on 2nd column, copper **
	// ambient material
	materialAmbient[0] = 0.19125f; // r
	materialAmbient[1] = 0.0735f;  // g
	materialAmbient[2] = 0.0225f;  // b
	materialAmbient[3] = 1.0f;    // a
	

	// diffuse material
	materialDiffuse[0] = 0.7038f;  // r
	materialDiffuse[1] = 0.27048f; // g
	materialDiffuse[2] = 0.0828f;  // b
	materialDiffuse[3] = 1.0f;    // a


	// specular material
	materialSpecular[0] = 0.256777f; // r
	materialSpecular[1] = 0.137622f; // g
	materialSpecular[2] = 0.086014f; // b
	materialSpecular[3] = 1.0f;     // a
	

	// shininess
	materialShininess = 0.1f * 128f;
	

	// geometry

Matrix.setIdentityM(modelMatrix,0);
		Matrix.setIdentityM(translationMatrix,0);
		Matrix.setIdentityM(viewMatrix,0);

		
		
		Matrix.translateM(translationMatrix,0,0.0f,0.0f,-2.0f);
	;
		modelMatrix=translationMatrix;



		GLES32.glUniformMatrix4fv(modelMatrixUniform,1,false,modelMatrix,0);
		GLES32.glUniformMatrix4fv(viewMatrixUniform,1,false,viewMatrix,0);
		GLES32.glUniformMatrix4fv(projectionMatrixUniform,1,false,prespectiveProjectionMatrix,0);

		GLES32.glViewport(screenWidth/4,(screenHeight/6)*2,(screenWidth/6),(screenHeight/6));
		if(bLight==true){
			GLES32.glUniform1i(lightEnableUniform,1);

			GLES32.glUniform3fv(laUniform,1,lightAmbiant,0);
			GLES32.glUniform3fv(ldUniform,1,lightDiffuse,0);
			GLES32.glUniform3fv(lsUniform,1,lightSpecular,0);
			GLES32.glUniform4fv(lightPositionUniform,1,lightPosition,0);

			GLES32.glUniform3fv(kaUniform,1,materialAmbient,0);
			GLES32.glUniform3fv(kdUniform,1,materialDiffuse,0);
			GLES32.glUniform3fv(ksUniform,1,materialSpecular,0);

			GLES32.glUniform1f(materialShininessUniform,materialShininess);

		
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

	// *******************

	// ** 5th sphere on 2nd column, gold **
	// ambient material
	materialAmbient[0] = 0.24725f; // r
	materialAmbient[1] = 0.1995f;  // g
	materialAmbient[2] = 0.0745f;  // b
	materialAmbient[3] = 1.0f;    // a


	// diffuse material
	materialDiffuse[0] = 0.75164f; // r
	materialDiffuse[1] = 0.60648f; // g
	materialDiffuse[2] = 0.22648f; // b
	materialDiffuse[3] = 1.0f;    // a
	

	// specular material
	materialSpecular[0] = 0.628281f; // r
	materialSpecular[1] = 0.555802f; // g
	materialSpecular[2] = 0.366065f; // b
	materialSpecular[3] = 1.0f;     // a


	// shininess
	materialShininess = 0.4f * 128f;
	

	// geometry

	Matrix.setIdentityM(modelMatrix,0);
		Matrix.setIdentityM(translationMatrix,0);
		Matrix.setIdentityM(viewMatrix,0);

		
		
		Matrix.translateM(translationMatrix,0,0.0f,0.0f,-2.0f);
	;
		modelMatrix=translationMatrix;



		GLES32.glUniformMatrix4fv(modelMatrixUniform,1,false,modelMatrix,0);
		GLES32.glUniformMatrix4fv(viewMatrixUniform,1,false,viewMatrix,0);
		GLES32.glUniformMatrix4fv(projectionMatrixUniform,1,false,prespectiveProjectionMatrix,0);

		GLES32.glViewport(screenWidth/4,(screenHeight/6),(screenWidth/6),(screenHeight/6));
		if(bLight==true){
			GLES32.glUniform1i(lightEnableUniform,1);

			GLES32.glUniform3fv(laUniform,1,lightAmbiant,0);
			GLES32.glUniform3fv(ldUniform,1,lightDiffuse,0);
			GLES32.glUniform3fv(lsUniform,1,lightSpecular,0);
			GLES32.glUniform4fv(lightPositionUniform,1,lightPosition,0);

			GLES32.glUniform3fv(kaUniform,1,materialAmbient,0);
			GLES32.glUniform3fv(kdUniform,1,materialDiffuse,0);
			GLES32.glUniform3fv(ksUniform,1,materialSpecular,0);

			GLES32.glUniform1f(materialShininessUniform,materialShininess);

		
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

	// *******************

	// ** 6th sphere on 2nd column, silver **
	// ambient material
	materialAmbient[0] = 0.19225f; // r
	materialAmbient[1] = 0.19225f; // g
	materialAmbient[2] = 0.19225f; // b
	materialAmbient[3] = 1.0f;    // a
	

	// diffuse material
	materialDiffuse[0] = 0.50754f; // r
	materialDiffuse[1] = 0.50754f; // g
	materialDiffuse[2] = 0.50754f; // b
	materialDiffuse[3] = 1.0f;    // a


	// specular material
	materialSpecular[0] = 0.508273f; // r
	materialSpecular[1] = 0.508273f; // g
	materialSpecular[2] = 0.508273f; // b
	materialSpecular[3] = 1.0f;     // a
	

	// shininess
	materialShininess = 0.4f * 128f;


	// geometry

	Matrix.setIdentityM(modelMatrix,0);
		Matrix.setIdentityM(translationMatrix,0);
		Matrix.setIdentityM(viewMatrix,0);

		
		
		Matrix.translateM(translationMatrix,0,0.0f,0.0f,-2.0f);
	;
		modelMatrix=translationMatrix;



		GLES32.glUniformMatrix4fv(modelMatrixUniform,1,false,modelMatrix,0);
		GLES32.glUniformMatrix4fv(viewMatrixUniform,1,false,viewMatrix,0);
		GLES32.glUniformMatrix4fv(projectionMatrixUniform,1,false,prespectiveProjectionMatrix,0);

		GLES32.glViewport(screenWidth/4,0,(screenWidth/6),(screenHeight/6));
		if(bLight==true){
			GLES32.glUniform1i(lightEnableUniform,1);

			GLES32.glUniform3fv(laUniform,1,lightAmbiant,0);
			GLES32.glUniform3fv(ldUniform,1,lightDiffuse,0);
			GLES32.glUniform3fv(lsUniform,1,lightSpecular,0);
			GLES32.glUniform4fv(lightPositionUniform,1,lightPosition,0);

			GLES32.glUniform3fv(kaUniform,1,materialAmbient,0);
			GLES32.glUniform3fv(kdUniform,1,materialDiffuse,0);
			GLES32.glUniform3fv(ksUniform,1,materialSpecular,0);

			GLES32.glUniform1f(materialShininessUniform,materialShininess);

		
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

	//*******************
	// *******************
	// *******************
	// ** 1st sphere on 3rd column, black **
	// ambient material
	materialAmbient[0] = 0.0f;  // r
	materialAmbient[1] = 0.0f;  // g
	materialAmbient[2] = 0.0f;  // b
	materialAmbient[3] = 1.0f; // a
	

	// diffuse material
	materialDiffuse[0] = 0.01f; // r
	materialDiffuse[1] = 0.01f; // g
	materialDiffuse[2] = 0.01f; // b
	materialDiffuse[3] = 1.0f; // a


	// specular material
	materialSpecular[0] = 0.50f; // r
	materialSpecular[1] = 0.50f; // g
	materialSpecular[2] = 0.50f; // b
	materialSpecular[3] = 1.0f; // a


	// shininess
	materialShininess = 0.25f * 128f;


	// geometry
	Matrix.setIdentityM(modelMatrix,0);
		Matrix.setIdentityM(translationMatrix,0);
		Matrix.setIdentityM(viewMatrix,0);

		
		
		Matrix.translateM(translationMatrix,0,0.0f,0.0f,-2.0f);
	;
		modelMatrix=translationMatrix;



		GLES32.glUniformMatrix4fv(modelMatrixUniform,1,false,modelMatrix,0);
		GLES32.glUniformMatrix4fv(viewMatrixUniform,1,false,viewMatrix,0);
		GLES32.glUniformMatrix4fv(projectionMatrixUniform,1,false,prespectiveProjectionMatrix,0);

		GLES32.glViewport((screenWidth/4)*2,(screenHeight/6)*5,(screenWidth/6),(screenHeight/6));
		if(bLight==true){
			GLES32.glUniform1i(lightEnableUniform,1);

			GLES32.glUniform3fv(laUniform,1,lightAmbiant,0);
			GLES32.glUniform3fv(ldUniform,1,lightDiffuse,0);
			GLES32.glUniform3fv(lsUniform,1,lightSpecular,0);
			GLES32.glUniform4fv(lightPositionUniform,1,lightPosition,0);

			GLES32.glUniform3fv(kaUniform,1,materialAmbient,0);
			GLES32.glUniform3fv(kdUniform,1,materialDiffuse,0);
			GLES32.glUniform3fv(ksUniform,1,materialSpecular,0);

			GLES32.glUniform1f(materialShininessUniform,materialShininess);

		
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
	// *******************


	// ** 2nd sphere on 3rd column, cyan **
	// ambient material
	materialAmbient[0] = 0.0f;  // r
	materialAmbient[1] = 0.1f;  // g
	materialAmbient[2] = 0.06f; // b
	materialAmbient[3] = 1.0f; // a
	

	// diffuse material
	materialDiffuse[0] = 0.0f;        // r
	materialDiffuse[1] = 0.50980392f; // g
	materialDiffuse[2] = 0.50980392f; // b
	materialDiffuse[3] = 1.0f;       // a
	

	// specular material
	materialSpecular[0] = 0.50196078f; // r
	materialSpecular[1] = 0.50196078f; // g
	materialSpecular[2] = 0.50196078f; // b
	materialSpecular[3] = 1.0f;       // a


	// shininess
	materialShininess = 0.25f * 128f;
	

	// geometry
	Matrix.setIdentityM(modelMatrix,0);
		Matrix.setIdentityM(translationMatrix,0);
		Matrix.setIdentityM(viewMatrix,0);

		
		
		Matrix.translateM(translationMatrix,0,0.0f,0.0f,-2.0f);
	;
		modelMatrix=translationMatrix;



		GLES32.glUniformMatrix4fv(modelMatrixUniform,1,false,modelMatrix,0);
		GLES32.glUniformMatrix4fv(viewMatrixUniform,1,false,viewMatrix,0);
		GLES32.glUniformMatrix4fv(projectionMatrixUniform,1,false,prespectiveProjectionMatrix,0);

		GLES32.glViewport((screenWidth/4)*2,(screenHeight/6)*4,(screenWidth/6),(screenHeight/6));
		if(bLight==true){
			GLES32.glUniform1i(lightEnableUniform,1);

			GLES32.glUniform3fv(laUniform,1,lightAmbiant,0);
			GLES32.glUniform3fv(ldUniform,1,lightDiffuse,0);
			GLES32.glUniform3fv(lsUniform,1,lightSpecular,0);
			GLES32.glUniform4fv(lightPositionUniform,1,lightPosition,0);

			GLES32.glUniform3fv(kaUniform,1,materialAmbient,0);
			GLES32.glUniform3fv(kdUniform,1,materialDiffuse,0);
			GLES32.glUniform3fv(ksUniform,1,materialSpecular,0);

			GLES32.glUniform1f(materialShininessUniform,materialShininess);

		
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
	// *******************

	// ** 3rd sphere on 2nd column, green **
	// ambient material
	materialAmbient[0] = 0.0f;  // r
	materialAmbient[1] = 0.0f;  // g
	materialAmbient[2] = 0.0f;  // b
	materialAmbient[3] = 1.0f; // a


	// diffuse material
	materialDiffuse[0] = 0.1f;  // r
	materialDiffuse[1] = 0.35f; // g
	materialDiffuse[2] = 0.1f;  // b
	materialDiffuse[3] = 1.0f; // a


	// specular material
	materialSpecular[0] = 0.45f; // r
	materialSpecular[1] = 0.55f; // g
	materialSpecular[2] = 0.45f; // b
	materialSpecular[3] = 1.0f; // a
	

	// shininess
	materialShininess = 0.25f * 128f;
	

	// geometry
Matrix.setIdentityM(modelMatrix,0);
		Matrix.setIdentityM(translationMatrix,0);
		Matrix.setIdentityM(viewMatrix,0);

		
		
		Matrix.translateM(translationMatrix,0,0.0f,0.0f,-2.0f);
	;
		modelMatrix=translationMatrix;



		GLES32.glUniformMatrix4fv(modelMatrixUniform,1,false,modelMatrix,0);
		GLES32.glUniformMatrix4fv(viewMatrixUniform,1,false,viewMatrix,0);
		GLES32.glUniformMatrix4fv(projectionMatrixUniform,1,false,prespectiveProjectionMatrix,0);

		GLES32.glViewport((screenWidth/4)*2,(screenHeight/6)*3,(screenWidth/6),(screenHeight/6));
		if(bLight==true){
			GLES32.glUniform1i(lightEnableUniform,1);

			GLES32.glUniform3fv(laUniform,1,lightAmbiant,0);
			GLES32.glUniform3fv(ldUniform,1,lightDiffuse,0);
			GLES32.glUniform3fv(lsUniform,1,lightSpecular,0);
			GLES32.glUniform4fv(lightPositionUniform,1,lightPosition,0);

			GLES32.glUniform3fv(kaUniform,1,materialAmbient,0);
			GLES32.glUniform3fv(kdUniform,1,materialDiffuse,0);
			GLES32.glUniform3fv(ksUniform,1,materialSpecular,0);

			GLES32.glUniform1f(materialShininessUniform,materialShininess);

		
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
	// *******************

	// ** 4th sphere on 3rd column, red **
	// ambient material
	materialAmbient[0] = 0.0f;  // r
	materialAmbient[1] = 0.0f;  // g
	materialAmbient[2] = 0.0f;  // b
	materialAmbient[3] = 1.0f; // a


	// diffuse material
	materialDiffuse[0] = 0.5f;  // r
	materialDiffuse[1] = 0.0f;  // g
	materialDiffuse[2] = 0.0f;  // b
	materialDiffuse[3] = 1.0f; // a
;

	// specular material
	materialSpecular[0] = 0.7f;  // r
	materialSpecular[1] = 0.6f;  // g
	materialSpecular[2] = 0.6f;  // b
	materialSpecular[3] = 1.0f; // a
	

	// shininess
	materialShininess = 0.25f * 128f;
	

	// geometry
Matrix.setIdentityM(modelMatrix,0);
		Matrix.setIdentityM(translationMatrix,0);
		Matrix.setIdentityM(viewMatrix,0);

		
		
		Matrix.translateM(translationMatrix,0,0.0f,0.0f,-2.0f);
	;
		modelMatrix=translationMatrix;



		GLES32.glUniformMatrix4fv(modelMatrixUniform,1,false,modelMatrix,0);
		GLES32.glUniformMatrix4fv(viewMatrixUniform,1,false,viewMatrix,0);
		GLES32.glUniformMatrix4fv(projectionMatrixUniform,1,false,prespectiveProjectionMatrix,0);

		GLES32.glViewport((screenWidth/4)*2,(screenHeight/6)*2,(screenWidth/6),(screenHeight/6));
		if(bLight==true){
			GLES32.glUniform1i(lightEnableUniform,1);

			GLES32.glUniform3fv(laUniform,1,lightAmbiant,0);
			GLES32.glUniform3fv(ldUniform,1,lightDiffuse,0);
			GLES32.glUniform3fv(lsUniform,1,lightSpecular,0);
			GLES32.glUniform4fv(lightPositionUniform,1,lightPosition,0);

			GLES32.glUniform3fv(kaUniform,1,materialAmbient,0);
			GLES32.glUniform3fv(kdUniform,1,materialDiffuse,0);
			GLES32.glUniform3fv(ksUniform,1,materialSpecular,0);

			GLES32.glUniform1f(materialShininessUniform,materialShininess);

		
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
	// *******************

	// ** 5th sphere on 3rd column, white **
	// ambient material
	materialAmbient[0] = 0.0f;  // r
	materialAmbient[1] = 0.0f;  // g
	materialAmbient[2] = 0.0f;  // b
	materialAmbient[3] = 1.0f; // a
	

	// diffuse material
	materialDiffuse[0] = 0.55f; // r
	materialDiffuse[1] = 0.55f; // g
	materialDiffuse[2] = 0.55f; // b
	materialDiffuse[3] = 1.0f; // a


	// specular material
	materialSpecular[0] = 0.70f; // r
	materialSpecular[1] = 0.70f; // g
	materialSpecular[2] = 0.70f; // b
	materialSpecular[3] = 1.0f; // a


	// shininess
	materialShininess = 0.25f * 128f;


	// geometry
Matrix.setIdentityM(modelMatrix,0);
		Matrix.setIdentityM(translationMatrix,0);
		Matrix.setIdentityM(viewMatrix,0);

		
		
		Matrix.translateM(translationMatrix,0,0.0f,0.0f,-2.0f);
	;
		modelMatrix=translationMatrix;



		GLES32.glUniformMatrix4fv(modelMatrixUniform,1,false,modelMatrix,0);
		GLES32.glUniformMatrix4fv(viewMatrixUniform,1,false,viewMatrix,0);
		GLES32.glUniformMatrix4fv(projectionMatrixUniform,1,false,prespectiveProjectionMatrix,0);

		GLES32.glViewport((screenWidth/4)*2,(screenHeight/6),(screenWidth/6),(screenHeight/6));
		if(bLight==true){
			GLES32.glUniform1i(lightEnableUniform,1);

			GLES32.glUniform3fv(laUniform,1,lightAmbiant,0);
			GLES32.glUniform3fv(ldUniform,1,lightDiffuse,0);
			GLES32.glUniform3fv(lsUniform,1,lightSpecular,0);
			GLES32.glUniform4fv(lightPositionUniform,1,lightPosition,0);

			GLES32.glUniform3fv(kaUniform,1,materialAmbient,0);
			GLES32.glUniform3fv(kdUniform,1,materialDiffuse,0);
			GLES32.glUniform3fv(ksUniform,1,materialSpecular,0);

			GLES32.glUniform1f(materialShininessUniform,materialShininess);

		
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
	// *******************

	// ** 6th sphere on 3rd column, yellow plastic **
	// ambient material
	materialAmbient[0] = 0.0f;  // r
	materialAmbient[1] = 0.0f;  // g
	materialAmbient[2] = 0.0f;  // b
	materialAmbient[3] = 1.0f; // a


	// diffuse material
	materialDiffuse[0] = 0.5f;  // r
	materialDiffuse[1] = 0.5f;  // g
	materialDiffuse[2] = 0.0f;  // b
	materialDiffuse[3] = 1.0f; // a
	

	// specular material
	materialSpecular[0] = 0.60f; // r
	materialSpecular[1] = 0.60f; // g
	materialSpecular[2] = 0.50f; // b
	

	// shininess
	materialShininess = 0.25f * 128f;


	// geometry
Matrix.setIdentityM(modelMatrix,0);
		Matrix.setIdentityM(translationMatrix,0);
		Matrix.setIdentityM(viewMatrix,0);

		
		
		Matrix.translateM(translationMatrix,0,0.0f,0.0f,-2.0f);
	;
		modelMatrix=translationMatrix;



		GLES32.glUniformMatrix4fv(modelMatrixUniform,1,false,modelMatrix,0);
		GLES32.glUniformMatrix4fv(viewMatrixUniform,1,false,viewMatrix,0);
		GLES32.glUniformMatrix4fv(projectionMatrixUniform,1,false,prespectiveProjectionMatrix,0);

		GLES32.glViewport((screenWidth/4)*2,0,(screenWidth/6),(screenHeight/6));
		if(bLight==true){
			GLES32.glUniform1i(lightEnableUniform,1);

			GLES32.glUniform3fv(laUniform,1,lightAmbiant,0);
			GLES32.glUniform3fv(ldUniform,1,lightDiffuse,0);
			GLES32.glUniform3fv(lsUniform,1,lightSpecular,0);
			GLES32.glUniform4fv(lightPositionUniform,1,lightPosition,0);

			GLES32.glUniform3fv(kaUniform,1,materialAmbient,0);
			GLES32.glUniform3fv(kdUniform,1,materialDiffuse,0);
			GLES32.glUniform3fv(ksUniform,1,materialSpecular,0);

			GLES32.glUniform1f(materialShininessUniform,materialShininess);

		
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
	// *******************
	// *******************
	// *******************


	// ** 1st sphere on 4th column, black **
	// ambient material
	materialAmbient[0] = 0.02f; // r
	materialAmbient[1] = 0.02f; // g
	materialAmbient[2] = 0.02f; // b
	materialAmbient[3] = 1.0f; // a
	

	// diffuse material
	materialDiffuse[0] = 0.01f; // r
	materialDiffuse[1] = 0.01f; // g
	materialDiffuse[2] = 0.01f; // b
	materialDiffuse[3] = 1.0f; // a


	// specular material
	materialSpecular[0] = 0.4f;  // r
	materialSpecular[1] = 0.4f;  // g
	materialSpecular[2] = 0.4f;  // b
	materialSpecular[3] = 1.0f; // a
;

	// shininess
	materialShininess = 0.078125f * 128f;


	// geometry

	Matrix.setIdentityM(modelMatrix,0);
		Matrix.setIdentityM(translationMatrix,0);
		Matrix.setIdentityM(viewMatrix,0);

		
		
		Matrix.translateM(translationMatrix,0,0.0f,0.0f,-2.0f);
	;
		modelMatrix=translationMatrix;



		GLES32.glUniformMatrix4fv(modelMatrixUniform,1,false,modelMatrix,0);
		GLES32.glUniformMatrix4fv(viewMatrixUniform,1,false,viewMatrix,0);
		GLES32.glUniformMatrix4fv(projectionMatrixUniform,1,false,prespectiveProjectionMatrix,0);

		GLES32.glViewport((screenWidth/4)*3,(screenHeight/6)*5,(screenWidth/6),(screenHeight/6));
		if(bLight==true){
			GLES32.glUniform1i(lightEnableUniform,1);

			GLES32.glUniform3fv(laUniform,1,lightAmbiant,0);
			GLES32.glUniform3fv(ldUniform,1,lightDiffuse,0);
			GLES32.glUniform3fv(lsUniform,1,lightSpecular,0);
			GLES32.glUniform4fv(lightPositionUniform,1,lightPosition,0);

			GLES32.glUniform3fv(kaUniform,1,materialAmbient,0);
			GLES32.glUniform3fv(kdUniform,1,materialDiffuse,0);
			GLES32.glUniform3fv(ksUniform,1,materialSpecular,0);

			GLES32.glUniform1f(materialShininessUniform,materialShininess);

		
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
	// *******************

	// ** 2nd sphere on 4th column, cyan **
	// ambient material
	materialAmbient[0] = 0.0f;  // r
	materialAmbient[1] = 0.05f; // g
	materialAmbient[2] = 0.05f; // b
	materialAmbient[3] = 1.0f; // 

	// diffuse material
	materialDiffuse[0] = 0.4f;  // r
	materialDiffuse[1] = 0.5f;  // g
	materialDiffuse[2] = 0.5f;  // b
	materialDiffuse[3] = 1.0f; // 

	// specular material
	materialSpecular[0] = 0.04f; // r
	materialSpecular[1] = 0.7f;  // g
	materialSpecular[2] = 0.7f;  // b
	materialSpecular[3] = 1.0f; // a


	// shininess
	materialShininess = 0.078125f * 128f;
	

	// geometry

	Matrix.setIdentityM(modelMatrix,0);
		Matrix.setIdentityM(translationMatrix,0);
		Matrix.setIdentityM(viewMatrix,0);

		
		
		Matrix.translateM(translationMatrix,0,0.0f,0.0f,-2.0f);
	;
		modelMatrix=translationMatrix;



		GLES32.glUniformMatrix4fv(modelMatrixUniform,1,false,modelMatrix,0);
		GLES32.glUniformMatrix4fv(viewMatrixUniform,1,false,viewMatrix,0);
		GLES32.glUniformMatrix4fv(projectionMatrixUniform,1,false,prespectiveProjectionMatrix,0);

		GLES32.glViewport((screenWidth/4)*3,(screenHeight/6)*4,(screenWidth/6),(screenHeight/6));
		if(bLight==true){
			GLES32.glUniform1i(lightEnableUniform,1);

			GLES32.glUniform3fv(laUniform,1,lightAmbiant,0);
			GLES32.glUniform3fv(ldUniform,1,lightDiffuse,0);
			GLES32.glUniform3fv(lsUniform,1,lightSpecular,0);
			GLES32.glUniform4fv(lightPositionUniform,1,lightPosition,0);

			GLES32.glUniform3fv(kaUniform,1,materialAmbient,0);
			GLES32.glUniform3fv(kdUniform,1,materialDiffuse,0);
			GLES32.glUniform3fv(ksUniform,1,materialSpecular,0);

			GLES32.glUniform1f(materialShininessUniform,materialShininess);

		
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
	// *******************

	// ** 3rd sphere on 4th column, green **
	// ambient material
	materialAmbient[0] = 0.0f;  // r
	materialAmbient[1] = 0.05f; // g
	materialAmbient[2] = 0.0f;  // b
	materialAmbient[3] = 1.0f; // a
	

	// diffuse material
	materialDiffuse[0] = 0.4f;  // r
	materialDiffuse[1] = 0.5f;  // g
	materialDiffuse[2] = 0.4f;  // b
	materialDiffuse[3] = 1.0f; // a


	// specular material
	materialSpecular[0] = 0.04f; // r
	materialSpecular[1] = 0.7f;  // g
	materialSpecular[2] = 0.04f; // b
	materialSpecular[3] = 1.0f; // a


	// shininess
	materialShininess = 0.078125f * 128f;


	// geometry
	Matrix.setIdentityM(modelMatrix,0);
		Matrix.setIdentityM(translationMatrix,0);
		Matrix.setIdentityM(viewMatrix,0);

		
		
		Matrix.translateM(translationMatrix,0,0.0f,0.0f,-2.0f);
	;
		modelMatrix=translationMatrix;



		GLES32.glUniformMatrix4fv(modelMatrixUniform,1,false,modelMatrix,0);
		GLES32.glUniformMatrix4fv(viewMatrixUniform,1,false,viewMatrix,0);
		GLES32.glUniformMatrix4fv(projectionMatrixUniform,1,false,prespectiveProjectionMatrix,0);

		GLES32.glViewport((screenWidth/4)*3,(screenHeight/6)*3,(screenWidth/6),(screenHeight/6));
		if(bLight==true){
			GLES32.glUniform1i(lightEnableUniform,1);

			GLES32.glUniform3fv(laUniform,1,lightAmbiant,0);
			GLES32.glUniform3fv(ldUniform,1,lightDiffuse,0);
			GLES32.glUniform3fv(lsUniform,1,lightSpecular,0);
			GLES32.glUniform4fv(lightPositionUniform,1,lightPosition,0);

			GLES32.glUniform3fv(kaUniform,1,materialAmbient,0);
			GLES32.glUniform3fv(kdUniform,1,materialDiffuse,0);
			GLES32.glUniform3fv(ksUniform,1,materialSpecular,0);

			GLES32.glUniform1f(materialShininessUniform,materialShininess);

		
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
	// *******************

	// ** 4th sphere on 4th column, red **
	// ambient material
	materialAmbient[0] = 0.05f; // r
	materialAmbient[1] = 0.0f;  // g
	materialAmbient[2] = 0.0f;  // b
	materialAmbient[3] = 1.0f; // a

	// diffuse material
	materialDiffuse[0] = 0.5f;  // r
	materialDiffuse[1] = 0.4f;  // g
	materialDiffuse[2] = 0.4f;  // b
	materialDiffuse[3] = 1.0f; // a

	// specular material
	materialSpecular[0] = 0.7f;  // r
	materialSpecular[1] = 0.04f; // g
	materialSpecular[2] = 0.04f; // b
	materialSpecular[3] = 1.0f; // a


	// shininess
	materialShininess = 0.078125f * 128f;
	

	// geometry
Matrix.setIdentityM(modelMatrix,0);
		Matrix.setIdentityM(translationMatrix,0);
		Matrix.setIdentityM(viewMatrix,0);

		
		
		Matrix.translateM(translationMatrix,0,0.0f,0.0f,-2.0f);
	;
		modelMatrix=translationMatrix;



		GLES32.glUniformMatrix4fv(modelMatrixUniform,1,false,modelMatrix,0);
		GLES32.glUniformMatrix4fv(viewMatrixUniform,1,false,viewMatrix,0);
		GLES32.glUniformMatrix4fv(projectionMatrixUniform,1,false,prespectiveProjectionMatrix,0);

		GLES32.glViewport((screenWidth/4)*3,(screenHeight/6)*2,(screenWidth/6),(screenHeight/6));
		if(bLight==true){
			GLES32.glUniform1i(lightEnableUniform,1);

			GLES32.glUniform3fv(laUniform,1,lightAmbiant,0);
			GLES32.glUniform3fv(ldUniform,1,lightDiffuse,0);
			GLES32.glUniform3fv(lsUniform,1,lightSpecular,0);
			GLES32.glUniform4fv(lightPositionUniform,1,lightPosition,0);

			GLES32.glUniform3fv(kaUniform,1,materialAmbient,0);
			GLES32.glUniform3fv(kdUniform,1,materialDiffuse,0);
			GLES32.glUniform3fv(ksUniform,1,materialSpecular,0);

			GLES32.glUniform1f(materialShininessUniform,materialShininess);

		
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
	// *******************

	// ** 5th sphere on 4th column, white **
	// ambient material
	materialAmbient[0] = 0.05f; // r
	materialAmbient[1] = 0.05f; // g
	materialAmbient[2] = 0.05f; // b;
	materialAmbient[3] = 1.0f; // a


	// diffuse material
	materialDiffuse[0] = 0.5f;  // r
	materialDiffuse[1] = 0.5f;  // g
	materialDiffuse[2] = 0.5f;  // b
	materialDiffuse[3] = 1.0f; // a


	// specular material
	materialSpecular[0] = 0.7f;  // r
	materialSpecular[1] = 0.7f;  // g
	materialSpecular[2] = 0.7f;  // b
	materialSpecular[3] = 1.0f; // a


	// shininess
	materialShininess = 0.078125f * 128f;


	// geometry
Matrix.setIdentityM(modelMatrix,0);
		Matrix.setIdentityM(translationMatrix,0);
		Matrix.setIdentityM(viewMatrix,0);

		
		
		Matrix.translateM(translationMatrix,0,0.0f,0.0f,-2.0f);
	;
		modelMatrix=translationMatrix;



		GLES32.glUniformMatrix4fv(modelMatrixUniform,1,false,modelMatrix,0);
		GLES32.glUniformMatrix4fv(viewMatrixUniform,1,false,viewMatrix,0);
		GLES32.glUniformMatrix4fv(projectionMatrixUniform,1,false,prespectiveProjectionMatrix,0);

		GLES32.glViewport((screenWidth/4)*3,(screenHeight/6),(screenWidth/6),(screenHeight/6));
		if(bLight==true){
			GLES32.glUniform1i(lightEnableUniform,1);

			GLES32.glUniform3fv(laUniform,1,lightAmbiant,0);
			GLES32.glUniform3fv(ldUniform,1,lightDiffuse,0);
			GLES32.glUniform3fv(lsUniform,1,lightSpecular,0);
			GLES32.glUniform4fv(lightPositionUniform,1,lightPosition,0);

			GLES32.glUniform3fv(kaUniform,1,materialAmbient,0);
			GLES32.glUniform3fv(kdUniform,1,materialDiffuse,0);
			GLES32.glUniform3fv(ksUniform,1,materialSpecular,0);

			GLES32.glUniform1f(materialShininessUniform,materialShininess);

		
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
	// *******************

	// ** 6th sphere on 4th column, yellow rubber **
	// ambient material
	materialAmbient[0] = 0.05f; // r
	materialAmbient[1] = 0.05f; // g
	materialAmbient[2] = 0.0f;  // b
	materialAmbient[3] = 1.0f; // a
	

	// diffuse material
	materialDiffuse[0] = 0.5f;  // r
	materialDiffuse[1] = 0.5f;  // g
	materialDiffuse[2] = 0.4f;  // b
	materialDiffuse[3] = 1.0f; // a
	

	// specular material
	materialSpecular[0] = 0.7f;  // r
	materialSpecular[1] = 0.7f;  // g
	materialSpecular[2] = 0.04f; // b
	materialSpecular[3] = 1.0f; // a
	

	// shininess
	materialShininess = 0.078125f * 128f;
	

	// geometry
Matrix.setIdentityM(modelMatrix,0);
		Matrix.setIdentityM(translationMatrix,0);
		Matrix.setIdentityM(viewMatrix,0);

		
		
		Matrix.translateM(translationMatrix,0,0.0f,0.0f,-2.0f);
	;
		modelMatrix=translationMatrix;



		GLES32.glUniformMatrix4fv(modelMatrixUniform,1,false,modelMatrix,0);
		GLES32.glUniformMatrix4fv(viewMatrixUniform,1,false,viewMatrix,0);
		GLES32.glUniformMatrix4fv(projectionMatrixUniform,1,false,prespectiveProjectionMatrix,0);

		GLES32.glViewport((screenWidth/4)*3,0,(screenWidth/6),(screenHeight/6));
		if(bLight==true){
			GLES32.glUniform1i(lightEnableUniform,1);

			GLES32.glUniform3fv(laUniform,1,lightAmbiant,0);
			GLES32.glUniform3fv(ldUniform,1,lightDiffuse,0);
			GLES32.glUniform3fv(lsUniform,1,lightSpecular,0);
			GLES32.glUniform4fv(lightPositionUniform,1,lightPosition,0);

			GLES32.glUniform3fv(kaUniform,1,materialAmbient,0);
			GLES32.glUniform3fv(kdUniform,1,materialDiffuse,0);
			GLES32.glUniform3fv(ksUniform,1,materialSpecular,0);

			GLES32.glUniform1f(materialShininessUniform,materialShininess);

		
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
	// *******************
	// *******************
	// *******************

	

		GLES32.glUseProgram(0);

	}

	void update(){
	//code
	xRotation = xRotation + 0.4f;
	if (xRotation > 360.0f)
		xRotation = 0.0f;

	yRotation = yRotation + 0.4f;
	if (yRotation > 360.0f)
		yRotation = 0.0f;

	zRotation = zRotation + 0.4f;
	if (zRotation > 360.0f)
		zRotation = 0.0f;
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
 