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
	int laUniform[]=new int[3];
	int ldUniform[]=new int[3];
	int lsUniform[]=new int[3];
	int lightPositionUniform[]=new int[3];

	int kaUniform;
	int kdUniform;
	int ksUniform;
	int materialShininessUniform;

	float xRotationAngle=0.0f;
	float yRotationAngle=0.0f;
	float zRotationAngle=0.0f;

	int lightEnableUniform;
	
	class Light{
		float lightAmbiant[]=new float[4];
		float lightdiffuse[]=new float[4];
		float lightSpecular[]=new float[4];
		float lightPosition[]=new float[4];
	};
	
	Light lights[]=new Light[3]; 
	boolean bLight=false;


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
	
		"uniform mediump  int u_lightningEnable;"+
		"out vec3 transformNormal;"+
		"out vec4 eyeCoordinate;"+
		"out vec3 viewerVector;"+
		"void main(void)" +
		"{" +
		"if(u_lightningEnable==1)"+
		"{"+
		"eyeCoordinate=u_viewMatrix*u_modelMatrix*a_position;"+
		"mat3 normalMatrix=mat3(u_viewMatrix*u_modelMatrix);"+
		"transformNormal=normalMatrix*a_normal;"+
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
		"in vec4 eyeCoordinate;"+
		"in vec3 viewerVector;" +

		"uniform vec3 u_la[3];"+
		"uniform vec3 u_ld[3];"+
		"uniform vec3 u_ls[3];"+
		"uniform vec4 u_lightPosition[3];"+
		"uniform vec3 u_ka;"+
		"uniform vec3 u_kd;"+
		"uniform vec3 u_ks;"+
		"uniform float u_materialShininess;"+
		"uniform mediump int u_lightningEnable;"+
		"out vec4 FragColor;" +
		"void main(void)" +
		"{" +
		"vec3 ambiant[3];"+
		"vec3 diffuse[3];"+
		"vec3 normalize_transform_normal;"+
		"vec3 lightDirection[3];"+
		"vec3 reflectionVector[3];"+
		"vec3 specular[3];"+
		"vec3 pong_ads_light;"+
		"if(u_lightningEnable==1)"+
		"{"+
		"vec3 normalize_viewerVector=normalize(viewerVector);"+
		"for(int i=0;i<3;i++)"+
		"{"+
		" ambiant[i]=u_la[i]*u_ka;"+
		"normalize_transform_normal=normalize(transformNormal);"+
		"lightDirection[i]=normalize(vec3(u_lightPosition[i])-eyeCoordinate.xyz);"+
		"diffuse[i]=u_ld[i]*u_kd*max(dot(lightDirection[i],normalize_transform_normal),0.0);"+
		"reflectionVector[i]=reflect(-lightDirection[i],normalize_transform_normal);"+
		"specular[i]=u_ls[i]*u_ks*pow(max(dot(reflectionVector[i],normalize_viewerVector),0.0),u_materialShininess);"+

		"pong_ads_light+=ambiant[i] +diffuse[i] +specular[i];"+
		"}"+
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
	
	"uniform vec3 u_la[3];"+
	"uniform vec3 u_ld[3];"+
	"uniform vec3 u_ls[3];"+
	"uniform vec4 u_lightPosition[3];"+
	
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
	"vec4 eyeCoordinate=u_viewMatrix*u_modelMatrix*a_position;"+
	"mat3 normalMatrix=mat3(u_viewMatrix*u_modelMatrix);"+
	"vec3 transformNormal=normalize(normalMatrix*a_normal);"+	
	"vec3 viewerVector=normalize(-eyeCoordinate.xyz);"+
	
	"vec3 ambiant[3];"+
	"vec3 lightDirection[3];"+
	"vec3 diffuse[3];"+
	"vec3 reflectionVector[3];"+
	"vec3 specular[3];"+
	"for(int i=0;i<3;i++)"+
	"{"+
	"ambiant[i]=u_la[i]*u_ka;"+
	"lightDirection[i]=normalize(vec3(u_lightPosition[i])-eyeCoordinate.xyz);"+
	"diffuse[i]=u_ld[i]*u_kd*max(dot(lightDirection[i],transformNormal),0.0);"+
	"reflectionVector[i]=reflect(-lightDirection[i],transformNormal);"+
	"specular[i]=u_ls[i]*u_ks*pow(max(dot(reflectionVector[i],viewerVector),0.0f),u_materialShininess);"+
	"pong_ads_light+=ambiant[i]+diffuse[i]+specular[i];"+
	"}"+
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

		//light0
		lights[0]=new Light();
		
		lights[0].lightAmbiant[0]=0.0f;
		lights[0].lightAmbiant[1]=0.0f;
		lights[0].lightAmbiant[2]=0.0f;
		lights[0].lightAmbiant[3]=1.0f;

		lights[0].lightdiffuse[0]=1.0f;
		lights[0].lightdiffuse[1]=0.0f;
		lights[0].lightdiffuse[2]=0.0f;
		lights[0].lightdiffuse[3]=1.0f;

		
		lights[0].lightSpecular[0]=1.0f;
		lights[0].lightSpecular[1]=0.0f;
		lights[0].lightSpecular[2]=0.0f;
		lights[0].lightSpecular[3]=1.0f;

		
		lights[0].lightPosition[0]=0.0f;
		lights[0].lightPosition[1]=3.0f*(float)Math.sin(xRotationAngle);
		lights[0].lightPosition[2]=3.0f*(float)Math.cos(xRotationAngle);
		lights[0].lightPosition[3]=1.0f;

		//light1
		lights[1]=new Light();
		
		lights[1].lightAmbiant[0]=0.0f;
		lights[1].lightAmbiant[1]=0.0f;
		lights[1].lightAmbiant[2]=0.0f;
		lights[1].lightAmbiant[3]=1.0f;

		lights[1].lightdiffuse[0]=0.0f;
		lights[1].lightdiffuse[1]=1.0f;
		lights[1].lightdiffuse[2]=0.0f;
		lights[1].lightdiffuse[3]=1.0f;

		
		lights[1].lightSpecular[0]=0.0f;
		lights[1].lightSpecular[1]=1.0f;
		lights[1].lightSpecular[2]=0.0f;
		lights[1].lightSpecular[3]=1.0f;

		
		lights[1].lightPosition[0]=3.0f*(float)Math.sin(yRotationAngle);
		lights[1].lightPosition[1]=0.0f;
		lights[1].lightPosition[2]=3.0f*(float)Math.cos(yRotationAngle);
		lights[1].lightPosition[3]=1.0f;

		//light 3
		lights[2]=new Light();
		
		lights[2].lightAmbiant[0]=0.0f;
		lights[2].lightAmbiant[1]=0.0f;
		lights[2].lightAmbiant[2]=0.0f;
		lights[2].lightAmbiant[3]=1.0f;

		lights[2].lightdiffuse[0]=0.0f;
		lights[2].lightdiffuse[1]=0.0f;
		lights[2].lightdiffuse[2]=1.0f;
		lights[2].lightdiffuse[3]=1.0f;

		
		lights[2].lightSpecular[0]=0.0f;
		lights[2].lightSpecular[1]=0.0f;
		lights[2].lightSpecular[2]=1.0f;
		lights[2].lightSpecular[3]=1.0f;

		
		lights[2].lightPosition[0]=3.0f*(float)Math.sin(zRotationAngle);
		lights[2].lightPosition[1]=3.0f*(float)Math.cos(zRotationAngle);
		lights[2].lightPosition[2]=0.0f;
		lights[2].lightPosition[3]=1.0f;





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
		
		GLES32.glUseProgram(shaderProgramObject_pf);
		if(lightCount==2){

		 modelMatrixUniform= GLES32.glGetUniformLocation(shaderProgramObject_pf,"u_modelMatrix");
		viewMatrixUniform=GLES32.glGetUniformLocation(shaderProgramObject_pf,"u_viewMatrix");
		projectionMatrixUniform=GLES32.glGetUniformLocation(shaderProgramObject_pf,"u_projectionMatrix");
		
		
		laUniform[0]=GLES32.glGetUniformLocation(shaderProgramObject_pf,"u_la[0]");
		ldUniform[0]=GLES32.glGetUniformLocation(shaderProgramObject_pf,"u_ld[0]");
		lsUniform[0]=GLES32.glGetUniformLocation(shaderProgramObject_pf,"u_ls[0]");
		lightPositionUniform[0]=GLES32.glGetUniformLocation(shaderProgramObject_pf,"u_lightPosition[0]");
	

		laUniform[1]=GLES32.glGetUniformLocation(shaderProgramObject_pf,"u_la[1]");
		ldUniform[1]=GLES32.glGetUniformLocation(shaderProgramObject_pf,"u_ld[1]");
		lsUniform[1]=GLES32.glGetUniformLocation(shaderProgramObject_pf,"u_ls[1]");
		lightPositionUniform[1]=GLES32.glGetUniformLocation(shaderProgramObject_pf,"u_lightPosition[1]");

		laUniform[2]=GLES32.glGetUniformLocation(shaderProgramObject_pf,"u_la[2]");
		ldUniform[2]=GLES32.glGetUniformLocation(shaderProgramObject_pf,"u_ld[2]");
		lsUniform[2]=GLES32.glGetUniformLocation(shaderProgramObject_pf,"u_ls[2]");
		lightPositionUniform[2]=GLES32.glGetUniformLocation(shaderProgramObject_pf,"u_lightPosition[2]");


		kaUniform=GLES32.glGetUniformLocation(shaderProgramObject_pf,"u_ka");
		kdUniform=GLES32.glGetUniformLocation(shaderProgramObject_pf,"u_kd");
		ksUniform=GLES32.glGetUniformLocation(shaderProgramObject_pf,"u_ks");

		materialShininessUniform=GLES32.glGetUniformLocation(shaderProgramObject_pf,"u_materialShininess");
		lightEnableUniform=GLES32.glGetUniformLocation(shaderProgramObject_pf,"u_lightningEnable");

		GLES32.glUseProgram(shaderProgramObject_pf);
		
		}
		else{
		 //post linking 
		modelMatrixUniform= GLES32.glGetUniformLocation(shaderProgramObject_pv,"u_modelMatrix");
		viewMatrixUniform=GLES32.glGetUniformLocation(shaderProgramObject_pv,"u_viewMatrix");
		projectionMatrixUniform=GLES32.glGetUniformLocation(shaderProgramObject_pv,"u_projectionMatrix");
		
		
		laUniform[0]=GLES32.glGetUniformLocation(shaderProgramObject_pv,"u_la[0]");
		ldUniform[0]=GLES32.glGetUniformLocation(shaderProgramObject_pv,"u_ld[0]");
		lsUniform[0]=GLES32.glGetUniformLocation(shaderProgramObject_pv,"u_ls[0]");
		lightPositionUniform[0]=GLES32.glGetUniformLocation(shaderProgramObject_pv,"u_lightPosition[0]");
	

		laUniform[1]=GLES32.glGetUniformLocation(shaderProgramObject_pv,"u_la[1]");
		ldUniform[1]=GLES32.glGetUniformLocation(shaderProgramObject_pv,"u_ld[1]");
		lsUniform[1]=GLES32.glGetUniformLocation(shaderProgramObject_pv,"u_ls[1]");
		lightPositionUniform[1]=GLES32.glGetUniformLocation(shaderProgramObject_pv,"u_lightPosition[1]");

		laUniform[2]=GLES32.glGetUniformLocation(shaderProgramObject_pv,"u_la[2]");
		ldUniform[2]=GLES32.glGetUniformLocation(shaderProgramObject_pv,"u_ld[2]");
		lsUniform[2]=GLES32.glGetUniformLocation(shaderProgramObject_pv,"u_ls[2]");
		lightPositionUniform[2]=GLES32.glGetUniformLocation(shaderProgramObject_pv,"u_lightPosition[2]");


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
			GLES32.glUniform1f(materialShininessUniform,materialShininess);

			lights[0].lightPosition[1]=3.0f*(float)Math.sin(xRotationAngle);
			lights[0].lightPosition[2]=3.0f*(float)Math.cos(xRotationAngle);
			lights[1].lightPosition[0]=3.0f*(float)Math.sin(yRotationAngle);
			lights[1].lightPosition[2]=3.0f*(float)Math.cos(yRotationAngle);
			lights[2].lightPosition[0]=3.0f*(float)Math.sin(zRotationAngle);
			lights[2].lightPosition[1]=3.0f*(float)Math.cos(zRotationAngle);

			for(int i=0;i<3;i++){
				GLES32.glUniform3fv(laUniform[i],1,lights[i].lightAmbiant,0);
				GLES32.glUniform3fv(ldUniform[i],1,lights[i].lightdiffuse,0);
				GLES32.glUniform3fv(lsUniform[i],1,lights[i].lightSpecular,0);
				GLES32.glUniform4fv(lightPositionUniform[i],1,lights[i].lightPosition,0);
			}
			GLES32.glUniform3fv(kaUniform,1,materialAmbiant,0);
			GLES32.glUniform3fv(kdUniform,1,materialDiffuse,0);
			GLES32.glUniform3fv(ksUniform,1,materialSpecular,0);


		
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
	private void update(){
		if (bLight == true) {
		if (xRotationAngle < 360.0f) {
			xRotationAngle = xRotationAngle + 0.1f;
		}
		else
			xRotationAngle = 0.0f;

		if (yRotationAngle < 360.0f) {
			yRotationAngle = yRotationAngle + 0.1f;
		}
		else
			yRotationAngle = 0.0f;

		if (zRotationAngle < 360.0f) {
			zRotationAngle = zRotationAngle + 0.1f;
		}
		else
			zRotationAngle = 0.0f;
	}
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
 