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

	
	int lightCount=0;
	private int shaderProgramObject_pf;
	private int shaderProgramObject_pv;

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

	float materialAmbiant[]={0.0f,0.0f,0.0f,1.0f};
	float materialDiffuse[]={1.0f,1.0f,1.0f,1.0f};
	float materialSpecular[]={1.0f,1.0f,1.0f,1.0f};

	float materialShininess=50.0f ;








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
		
		lightCount++;
		System.out.println("AMC:"+"lightCount: "+lightCount);
		if(lightCount>=3)
			lightCount=0;
	
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

	final String vertexShaderSourceCode_pf=String.format(
	"#version 320 es"+
	"\n"+
	"in vec4 a_position;"+
		"in vec3  a_normal;"+
		"uniform mat4 u_modelMatrix;" +
		"uniform mat4 u_viewMatrix; "+
		"uniform mat4 u_projectionMatrix; "+
		"uniform vec4 u_lightPosition;"+
		"uniform mediump  int u_lightningEnable;"+
		"out vec3 transformNormal;"+
		"out vec3 lightDirection ;"+
		"out vec3 viewerVector;"+
		"void main(void)" +
		"{" +
		"if(u_lightningEnable==1)"+
		"{"+
		"vec4 eyeCoordinate=u_viewMatrix*u_modelMatrix*a_position;"+
		"mat3 normalMatrix=mat3(u_viewMatrix*u_modelMatrix);"+
		"transformNormal=normalMatrix*a_normal;"+
		"lightDirection=vec3(u_lightPosition)-eyeCoordinate.xyz;"+
		"viewerVector=-eyeCoordinate.xyz;"+
		"}"+
		"gl_Position=u_projectionMatrix *u_viewMatrix* u_modelMatrix* a_position ;" +
		"}");

	int vertexShaderObject_pf=GLES32.glCreateShader(GLES32.GL_VERTEX_SHADER);
	GLES32.glShaderSource(vertexShaderObject_pf,vertexShaderSourceCode_pf);
	GLES32.glCompileShader(vertexShaderObject_pf);

	int status[]=new int[1];
	int infoLogLength[]=new int[1];
	String log=null;

	GLES32.glGetShaderiv(vertexShaderObject_pf,GLES32.GL_COMPILE_STATUS,status,0);
	if(status[0]==GLES32.GL_FALSE){
		GLES32.glGetShaderiv(vertexShaderObject_pf,GLES32.GL_INFO_LOG_LENGTH,infoLogLength,0);
		if(infoLogLength[0]>0){
			log=GLES32.glGetShaderInfoLog(vertexShaderObject_pf);
			System.out.println("AMC: "+"Vertex Shader  per fragment Compilation Log "+log );
			uninitalize();
			System.exit(0);	
		}
	}
	//fragment Shader
	final String fragmentShaderSourceCode_pf=String.format(
		"#version 320 es" +
		"\n" +
		"precision highp float;"+
		"in vec3 transformNormal;"+
		"in vec3 lightDirection ;"+
		"in vec3 viewerVector;" +
		"uniform vec3 u_la;"+
		"uniform vec3 u_ld;"+
		"uniform vec3 u_ls;"+
		"uniform vec3 u_ka;"+
		"uniform vec3 u_kd;"+
		"uniform vec3 u_ks;"+
		"uniform float u_materialShininess;"+
		"uniform mediump int u_lightningEnable;"+
		"out vec4 FragColor;" +
		"void main(void)" +
		"{" +
		"vec3 pong_ads_light;"+
		"if(u_lightningEnable==1)"+
		"{"+
		"vec3 ambiant=u_la*u_ka;"+
		"vec3 normalize_transform_normal=normalize(transformNormal);"+
		"vec3 normalize_lightDirection=normalize(lightDirection);"+
		"vec3 diffuse=u_ld*u_kd*max(dot(normalize_lightDirection,normalize_transform_normal),0.0);"+
		"vec3 reflectionVector=reflect(-normalize_lightDirection,normalize_transform_normal);"+
		"vec3 normalize_viewerVector=normalize(viewerVector);"+
		"vec3 specular=u_ls*u_ks*pow(max(dot(reflectionVector,normalize_viewerVector),0.0),u_materialShininess);"+
		"pong_ads_light=ambiant +diffuse +specular;"+
		"}"+
		"else"+
		"{"+
		"pong_ads_light = vec3(1.0f,1.0f,1.0f);"+
		"}"+
		"FragColor=vec4(pong_ads_light,1.0f);" +
		"}"); 

		int fragmentShaderObject_pf=GLES32.glCreateShader(GLES32.GL_FRAGMENT_SHADER);
		GLES32.glShaderSource(fragmentShaderObject_pf,fragmentShaderSourceCode_pf);
		GLES32.glCompileShader(fragmentShaderObject_pf);

		status[0]=0;
		infoLogLength[0]=0;
		log=null;
		GLES32.glGetShaderiv(fragmentShaderObject_pf,GLES32.GL_COMPILE_STATUS,status,0);
		if(status[0]==GLES32.GL_FALSE){
			GLES32.glGetShaderiv(fragmentShaderObject_pf,GLES32.GL_INFO_LOG_LENGTH,infoLogLength,0);
			if(infoLogLength[0]>0){
				log=GLES32.glGetShaderInfoLog(fragmentShaderObject_pf);
				System.out.println("AMC:"+"fragment Shader per fragment Compilation log : "+log);
				uninitalize();
				System.exit(0);
			}
		}

		//shaderProgram
		 shaderProgramObject_pf= GLES32.glCreateProgram();
		 GLES32.glAttachShader(shaderProgramObject_pf,vertexShaderObject_pf);
		 GLES32.glAttachShader(shaderProgramObject_pf,fragmentShaderObject_pf);

		
		 GLES32.glBindAttribLocation(shaderProgramObject_pf,MyGLESMacro.AMC_ATTRIBUTE_POSITION,"a_position");
		 GLES32.glBindAttribLocation(shaderProgramObject_pf,MyGLESMacro.AMC_ATTRIBUTE_NORMAL,"a_normal");
		 GLES32.glLinkProgram(shaderProgramObject_pf);
		  infoLogLength[0]=0;
		 status[0]=0;
		 log=null;
		 GLES32.glGetShaderiv(shaderProgramObject_pf,GLES32.GL_LINK_STATUS,status,0);
		 if(status[0]==GLES32.GL_FALSE){
				GLES32.glGetShaderiv(shaderProgramObject_pf,GLES32.GL_INFO_LOG_LENGTH,infoLogLength,0);
				if(infoLogLength[0]>0){
					log=GLES32.glGetShaderInfoLog(shaderProgramObject_pf);
					System.out.println("AMC:"+"Shader program link log : "+log);
					uninitalize();
					System.exit(0);
					
					}
		 }



		//per Vertex
		
	final String vertexShaderSourceCode_pv=String.format(
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
	"uniform mediump int u_lightningEnable;"+
	"out vec3 pong_ads_light;"+
	"void main()"+
	"{"+
	"if(u_lightningEnable==1)"+
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

	int vertexShaderObject_pv=GLES32.glCreateShader(GLES32.GL_VERTEX_SHADER);
	GLES32.glShaderSource(vertexShaderObject_pv,vertexShaderSourceCode_pv);
	GLES32.glCompileShader(vertexShaderObject_pv);

	 status[0]=0;
	 infoLogLength[0]=0;
	 log=null;

	GLES32.glGetShaderiv(vertexShaderObject_pv,GLES32.GL_COMPILE_STATUS,status,0);
	if(status[0]==GLES32.GL_FALSE){
		GLES32.glGetShaderiv(vertexShaderObject_pv,GLES32.GL_INFO_LOG_LENGTH,infoLogLength,0);
		if(infoLogLength[0]>0){
			log=GLES32.glGetShaderInfoLog(vertexShaderObject_pv);
			System.out.println("AMC: "+"Vertex Shader per vertex Compilation Log "+log );
			uninitalize();
			System.exit(0);	
		}
	}
	//fragment Shader
	final String fragmentShaderSourceCode_pv=String.format(
		"#version 320 es" +
		"\n" +
		"precision highp float;"+
		"in vec3 pong_ads_light;"+
		
		"out vec4 FragColor;"+
		"void main(void)"+
		"{"+
		"FragColor=vec4(pong_ads_light,1.0f);"+
		"}"); 

		int fragmentShaderObject_pv=GLES32.glCreateShader(GLES32.GL_FRAGMENT_SHADER);
		GLES32.glShaderSource(fragmentShaderObject_pv,fragmentShaderSourceCode_pv);
		GLES32.glCompileShader(fragmentShaderObject_pv);

		status[0]=0;
		infoLogLength[0]=0;
		log=null;
		GLES32.glGetShaderiv(fragmentShaderObject_pv,GLES32.GL_COMPILE_STATUS,status,0);
		if(status[0]==GLES32.GL_FALSE){
			GLES32.glGetShaderiv(fragmentShaderObject_pv,GLES32.GL_INFO_LOG_LENGTH,infoLogLength,0);
			if(infoLogLength[0]>0){
				log=GLES32.glGetShaderInfoLog(fragmentShaderObject_pv);
				System.out.println("AMC:"+"fragment Shader per vertex Compilation log : "+log);
				uninitalize();
				System.exit(0);
			}
		}

		//shaderProgram
		 shaderProgramObject_pv= GLES32.glCreateProgram();
		 GLES32.glAttachShader(shaderProgramObject_pv,vertexShaderObject_pv);
		 GLES32.glAttachShader(shaderProgramObject_pv,fragmentShaderObject_pv);

		
		 GLES32.glBindAttribLocation(shaderProgramObject_pv,MyGLESMacro.AMC_ATTRIBUTE_POSITION,"a_position");
		 GLES32.glBindAttribLocation(shaderProgramObject_pv,MyGLESMacro.AMC_ATTRIBUTE_NORMAL,"a_normal");
		 GLES32.glLinkProgram(shaderProgramObject_pv);
		  infoLogLength[0]=0;
		 status[0]=0;
		 log=null;
		 GLES32.glGetShaderiv(shaderProgramObject_pv,GLES32.GL_LINK_STATUS,status,0);
		 if(status[0]==GLES32.GL_FALSE){
				GLES32.glGetShaderiv(shaderProgramObject_pv,GLES32.GL_INFO_LOG_LENGTH,infoLogLength,0);
				if(infoLogLength[0]>0){
					log=GLES32.glGetShaderInfoLog(shaderProgramObject_pv);
					System.out.println("AMC:"+"Shader program link log : "+log);
					uninitalize();
					System.exit(0);
					
					}
		 }

		



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
		
	
		if(lightCount==2){

		GLES32.glUseProgram(shaderProgramObject_pf);
				 //post linking 
		modelMatrixUniform= GLES32.glGetUniformLocation(shaderProgramObject_pf,"u_modelMatrix");
		viewMatrixUniform=GLES32.glGetUniformLocation(shaderProgramObject_pf,"u_viewMatrix");
		projectionMatrixUniform=GLES32.glGetUniformLocation(shaderProgramObject_pf,"u_projectionMatrix");
		
		
		laUniform=GLES32.glGetUniformLocation(shaderProgramObject_pf,"u_la");
		ldUniform=GLES32.glGetUniformLocation(shaderProgramObject_pf,"u_ld");
		lsUniform=GLES32.glGetUniformLocation(shaderProgramObject_pf,"u_ls");
		lightPositionUniform=GLES32.glGetUniformLocation(shaderProgramObject_pf,"u_lightPosition");
	
		kaUniform=GLES32.glGetUniformLocation(shaderProgramObject_pf,"u_ka");
		kdUniform=GLES32.glGetUniformLocation(shaderProgramObject_pf,"u_kd");
		ksUniform=GLES32.glGetUniformLocation(shaderProgramObject_pf,"u_ks");

		materialShininessUniform=GLES32.glGetUniformLocation(shaderProgramObject_pf,"u_materialShininess");
		lightEnableUniform=GLES32.glGetUniformLocation(shaderProgramObject_pf,"u_lightningEnable");

		}
		else{
		 //post linking 
		modelMatrixUniform= GLES32.glGetUniformLocation(shaderProgramObject_pv,"u_modelMatrix");
		viewMatrixUniform=GLES32.glGetUniformLocation(shaderProgramObject_pv,"u_viewMatrix");
		projectionMatrixUniform=GLES32.glGetUniformLocation(shaderProgramObject_pv,"u_projectionMatrix");
		
		
		laUniform=GLES32.glGetUniformLocation(shaderProgramObject_pv,"u_la");
		ldUniform=GLES32.glGetUniformLocation(shaderProgramObject_pv,"u_ld");
		lsUniform=GLES32.glGetUniformLocation(shaderProgramObject_pv,"u_ls");
		lightPositionUniform=GLES32.glGetUniformLocation(shaderProgramObject_pv,"u_lightPosition");
	
		kaUniform=GLES32.glGetUniformLocation(shaderProgramObject_pv,"u_ka");
		kdUniform=GLES32.glGetUniformLocation(shaderProgramObject_pv,"u_kd");
		ksUniform=GLES32.glGetUniformLocation(shaderProgramObject_pv,"u_ks");

		materialShininessUniform=GLES32.glGetUniformLocation(shaderProgramObject_pv,"u_materialShininess");
		lightEnableUniform=GLES32.glGetUniformLocation(shaderProgramObject_pv,"u_lightningEnable");
		GLES32.glUseProgram(shaderProgramObject_pv);

		}

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

			GLES32.glUniform3fv(laUniform,1,lightAmbiant,0);
			GLES32.glUniform3fv(ldUniform,1,lightDiffuse,0);
			GLES32.glUniform3fv(lsUniform,1,lightSpecular,0);
			GLES32.glUniform4fv(lightPositionUniform,1,lightPosition,0);

			GLES32.glUniform3fv(kaUniform,1,materialAmbiant,0);
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

		GLES32.glUseProgram(0);
				

		requestRender();
	}
	private void uninitalize(){
			//code
		int retVal[]=new int[1];
		if(shaderProgramObject_pf>0){
			GLES32.glUseProgram(shaderProgramObject_pf);
				GLES32.glGetProgramiv(shaderProgramObject_pf,GLES32.GL_ATTACHED_SHADERS,retVal,0);
				if(retVal[0]>0){
					int numAttachedShaders=retVal[0];
				
					int shaderObject[]=new int[numAttachedShaders];
					GLES32.glGetAttachedShaders(shaderProgramObject_pf,numAttachedShaders,retVal,0,shaderObject,0);
					for(int i=0;i<numAttachedShaders;i++){
						GLES32.glDetachShader(shaderProgramObject_pf,shaderObject[i]);
						GLES32.glDeleteShader(shaderObject[i]);
						shaderObject[i]=0;
					}
				
				}
				GLES32.glUseProgram(0);
				GLES32.glDeleteProgram(shaderProgramObject_pf);
				shaderProgramObject_pf=0;
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
 