var canvas = null;
var gl = null;
var bFullscreen = false;
var canvas_original_width;
var canvas_original_height;

const webGLMacros =
{
    AMC_ATTRIBUTE_POSITION: 0,
    AMC_ATTRIBUTE_COLOR: 1,
    AMC_ATTRIBUTE_NORMAL: 2,
    AMC_ATTRIBUTE_TEXTUREZERO: 3,
};

// FBO related
let FBO_WIDTH = 512;
let FBO_HEIGHT = 512;
var winWidth_fbo;
var winHeight_fbo;

var shaderProgramObject;

var vao_cube;
var vbo_cube_position;
var vbo_cube_texcoord;

var mvpMatrixUniform;
var textureSamplerUniform;

var angleCube = 0.0;
var perspectiveProjectionMatrix;

var fbo;
var rbo;
var fbo_texture;
var bFBOResult = false

// flags
var bLight = 0;
var choosenShader = 0;
var angleLight_sphere = 0.0;

// FBO RELATED VARIABLES **************

var shaderProgramObject_pf;
var shaderProgramObject_pv;

var modelMatrixUniform_pf;
var viewMatrixUniform_pf;
var projectionMatrixUniform_pf;

var modelMatrixUniform_pv;
var viewMatrixUniform_pv;
var projectionMatrixUniform_pv;
var perspectiveProjectionMatrixSphere;


//-----------------light per vertex
var laUniform_pv = new Array(3);
var ldUniform_pv = new Array(3);
var lsUniform_pv = new Array(3);
var lightPositionUniform_pv = new Array(3);

var kaUniform_pv;
var kdUniform_pv;
var ksUniform_pv;
var materialShininessUniform_pv;

var lightingEnabledUniform_pv;
//------------- light per fragment
var laUniform_pf = new Array(3);
var ldUniform_pf = new Array(3);
var lsUniform_pf = new Array(3);
var lightPositionUniform_pf = new Array(3);

var kaUniform_pf;
var kdUniform_pf;
var ksUniform_pf;
var materialShininessUniform_pf;
var lightingEnabledUniform_pf;

// ----------------------
var ambientSphere = new Array(3);
var diffuseSphere = new Array(3);
var specularSphere = new Array(3);
var lightPositionSphere = new Array(3);

var materialAmbientSphere = [0.0, 0.0, 0.0];
var materialDiffuseSphere = [1.0, 1.0, 1.0];  // whiter material
var materialSpecularSphere = [1.0, 1.0, 1.0]; // whiter specular
var materialShininessSphere = 128.0;

// flags
var bLightSphere = 0;
var choosenShaderSphere = 0;

var angleSphere = 0.0;
// sphere related 
var sphere = null;



var requestAnimationFrame = window.requestAnimationFrame ||
    window.mozRequestAnimationFrame ||
    window.webkitRequestAnimationFrame ||
    window.oRequestAnimationFrame || // opera specific
    window.msRequestAnimationFrame;   // this is swapbuffers i.e double buffering function

function main() {
    //code
    //1) get canvas
    canvas = document.getElementById("SSP");
    if (!canvas) {
        console.log("Id not found,canvas failed!!\n");
    }
    else {
        console.log("canvas id successfully accessed!!\n");
    }

    // back up canvas dimensions --- to be used in resize
    canvas_original_width = canvas.width;
    canvas_original_height = canvas.height;

    // initialize
    initialize();


    // warm - up resize
    resize();

    // display 
    display();

    // add keyboard and mouse event listeners
    window.addEventListener("keydown", keyDown, false);
    window.addEventListener("click", mouseDown, false);
    window.addEventListener("resize", resize, false); // do bubbling hence false last param


}


function toggleFullscreen() {
    // code
    var fullscreen_element = document.fullscreenElement ||
        document.mozFullScreenelement ||
        document.webkitFullscreenelement ||
        document.msFullscreenelement ||
        null;
    if (fullscreen_element == null) // if not fullscreent
    {
        if (canvas.requestFullscreen) // for chrome and opera function pointer --- if function exist call that function
        {
            canvas.requestFullscreen();
        } else if (canvas.mozRequestFullScreen) {
            canvas.mozRequestFullScreen();
        } else if (canvas.webkitRequestFullscreen) {
            canvas.webkitRequestFullscreen();
        } else if (canvas.msRequestFullscreen) {
            canvas.msRequestFullscreen();
        }

        bFullscreen = true
    } else {
        if (document.exitFullscreen) // function pointer --- if function exist call that function
        {
            document.exitFullscreen();
        } else if (document.mozExitFullScreen) {
            document.mozExitFullScreen();
        } else if (document.webkitExitFullscreen) {
            document.webkitExitFullscreen();
        } else if (document.msExitFullscreen) {
            document.msExitFullscreen();
        }
        bFullscreen = false;
    }
}

function initialize() {
    //code
    //2) get webgl 2 context
    gl = canvas.getContext("webgl2");
    if (!gl) {
        console.log("Obtaining WebGL 2.0 context failed!!\n");
    }
    else {
        console.log("Obtaining WebGL 2.0 context successfully done!!\n");
    }

    // set viewport width and height of context
    gl.viewportWidth = canvas.width;
    gl.viewportHeight = canvas.height;

    // openGL code starts
    // vertex shader
    var vertexShaderSourceCode =
        "#version 300 es" +   // webgl 2.0 is subset of OpenGL ES 3.0 not 3.20
        "\n" +
        "in vec4 a_position;" +
        "in vec2 a_texcoord;" +
        "uniform mat4 u_mvpMatrix;" +
        "out vec2 a_texcoord_out;" +
        "void main(void)" +
        "{" +
        "gl_Position = u_mvpMatrix * a_position;" +
        "a_texcoord_out = a_texcoord;" +
        "}";

    var vertexShaderObject = gl.createShader(gl.VERTEX_SHADER);

    gl.shaderSource(vertexShaderObject, vertexShaderSourceCode);

    gl.compileShader(vertexShaderObject);

    if (gl.getShaderParameter(vertexShaderObject, gl.COMPILE_STATUS) == false) {
        var error = gl.getShaderInfoLog(vertexShaderObject);
        if (error.length > 0) {
            alert("Vertex Shader Compilation Error :" + error);
            unintialize();
        }
    }

    // fragment shader
    var fragmentShaderSourceCode =
        "#version 300 es" +
        "\n" +
        "precision highp float;" +
        "in vec2 a_texcoord_out;" +
        "uniform sampler2D u_textureSampler;" +
        "out vec4 FragColor;" +
        "void main(void)" +
        "{" +
        "FragColor = texture(u_textureSampler,a_texcoord_out);" +
        "}";

    var fragmentShaderObject = gl.createShader(gl.FRAGMENT_SHADER);

    gl.shaderSource(fragmentShaderObject, fragmentShaderSourceCode);

    gl.compileShader(fragmentShaderObject);

    if (gl.getShaderParameter(fragmentShaderObject, gl.COMPILE_STATUS) == false) {
        var error = gl.getShaderInfoLog(fragmentShaderObject);
        if (error.length > 0) {
            alert("Fragment Shader Compilation Error :" + error);
            unintialize();
        }
    }
    // shader program
    shaderProgramObject = gl.createProgram();
    gl.attachShader(shaderProgramObject, vertexShaderObject);
    gl.attachShader(shaderProgramObject, fragmentShaderObject);

    // prilinking shaders' attribute binding
    gl.bindAttribLocation(shaderProgramObject, webGLMacros.AMC_ATTRIBUTE_POSITION, "a_position");
    gl.bindAttribLocation(shaderProgramObject, webGLMacros.AMC_ATTRIBUTE_TEXTUREZERO, "a_texcoord");



    // shader program linking
    gl.linkProgram(shaderProgramObject);

    if (gl.getProgramParameter(shaderProgramObject, gl.LINK_STATUS) == false) {
        var error = gl.getProgramInfoLog(shaderProgramObject);
        if (error.length > 0) {
            alert("Shader Program Error :" + error);
            unintialize();
        }
    }

    // post linking
    mvpMatrixUniform = gl.getUniformLocation(shaderProgramObject, "u_mvpMatrix");
    textureSamplerUniform = gl.getUniformLocation(shaderProgramObject, "u_textureSampler");
    // declaration of vertex data array

    var cubeVertices = new Float32Array(
        [
            // top
            1.0, 1.0, -1.0,
            -1.0, 1.0, -1.0,
            -1.0, 1.0, 1.0,
            1.0, 1.0, 1.0,

            // bottom
            1.0, -1.0, -1.0,
            -1.0, -1.0, -1.0,
            -1.0, -1.0, 1.0,
            1.0, -1.0, 1.0,

            // front
            1.0, 1.0, 1.0,
            -1.0, 1.0, 1.0,
            -1.0, -1.0, 1.0,
            1.0, -1.0, 1.0,

            // back
            1.0, 1.0, -1.0,
            -1.0, 1.0, -1.0,
            -1.0, -1.0, -1.0,
            1.0, -1.0, -1.0,

            // right
            1.0, 1.0, -1.0,
            1.0, 1.0, 1.0,
            1.0, -1.0, 1.0,
            1.0, -1.0, -1.0,

            // left
            -1.0, 1.0, 1.0,
            -1.0, 1.0, -1.0,
            -1.0, -1.0, -1.0,
            -1.0, -1.0, 1.0
        ]
    );

    var cubeTexcoord = new Float32Array(
        [
            1.0, 1.0,
            0.0, 1.0, // top
            0.0, 0.0,
            1.0, 0.0,

            0.0, 1.0,
            1.0, 1.0,
            1.0, 0.0, // bottom
            0.0, 0.0,

            1.0, 1.0, // front
            0.0, 1.0,
            0.0, 0.0,
            1.0, 0.0,

            0.0, 0.0,
            0.0, 1.0,
            1.0, 1.0,
            1.0, 0.0,

            0.0, 1.0, // right
            0.0, 0.0,
            1.0, 0.0,
            1.0, 1.0,

            1.0, 1.0,
            0.0, 1.0,
            0.0, 0.0,
            1.0, 0.0

        ]
    );
    // vao and vbo steps

    vao_cube = gl.createVertexArray();
    gl.bindVertexArray(vao_cube);

    vbo_cube_position = gl.createBuffer();
    gl.bindBuffer(gl.ARRAY_BUFFER, vbo_cube_position);
    gl.bufferData(gl.ARRAY_BUFFER, cubeVertices, gl.STATIC_DRAW);
    gl.vertexAttribPointer(webGLMacros.AMC_ATTRIBUTE_POSITION, 3, gl.FLOAT, false, 0, 0);
    gl.enableVertexAttribArray(webGLMacros.AMC_ATTRIBUTE_POSITION);

    gl.bindBuffer(gl.ARRAY_BUFFER, null);
    vbo_cube_texcoord = gl.createBuffer();
    gl.bindBuffer(gl.ARRAY_BUFFER, vbo_cube_texcoord);
    gl.bufferData(gl.ARRAY_BUFFER, cubeTexcoord, gl.STATIC_DRAW);
    gl.vertexAttribPointer(webGLMacros.AMC_ATTRIBUTE_TEXTUREZERO, 2, gl.FLOAT, false, 0, 0);
    gl.enableVertexAttribArray(webGLMacros.AMC_ATTRIBUTE_TEXTUREZERO);

    gl.bindBuffer(gl.ARRAY_BUFFER, null);
    gl.bindVertexArray(null);

    // depth and clear color related code 
    gl.clearDepth(1.0);
    gl.enable(gl.DEPTH_TEST);
    gl.depthFunc(gl.LEQUAL);

    // clear the screen by color blue
    gl.clearColor(0.0, 0.0, 0.0, 1.0); // by default float taken

    perspectiveProjectionMatrix = mat4.create();
    resize();
    bFBOResult = createFBO(FBO_WIDTH, FBO_HEIGHT);
    if (bFBOResult == true) {
        initialize_sphere(FBO_WIDTH, FBO_HEIGHT);
    }
}

function createFBO(textureWidth, textureHeight){
    //var maxRenderBufferSize =  gl.getIntegerv(gl.MAX_RENDERBUFFER_SIZE);

    // create framebuffer object
    fbo = gl.createFramebuffer();
    gl.bindFramebuffer(gl.FRAMEBUFFER, fbo);
    // create renderbuffer object

    rbo = gl.createRenderbuffer();
    gl.bindRenderbuffer(gl.RENDERBUFFER, rbo);

    // storage and format of the render buffer
    gl.renderbufferStorage(gl.RENDERBUFFER, gl.DEPTH_COMPONENT16, textureWidth, textureHeight);

    fbo_texture = gl.createTexture();
    gl.bindTexture(gl.TEXTURE_2D, fbo_texture);
    gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_WRAP_S, gl.CLAMP_TO_EDGE);
    gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_WRAP_T, gl.CLAMP_TO_EDGE);
    gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MAG_FILTER, gl.LINEAR);
    gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MIN_FILTER, gl.LINEAR);
    gl.texImage2D(gl.TEXTURE_2D, 0, gl.RGB, textureWidth, textureHeight, 0, gl.RGB, gl.UNSIGNED_SHORT_5_6_5, null);

    gl.framebufferTexture2D(gl.FRAMEBUFFER, gl.COLOR_ATTACHMENT0, gl.TEXTURE_2D, fbo_texture, 0);
    gl.framebufferRenderbuffer(gl.FRAMEBUFFER, gl.DEPTH_ATTACHMENT, gl.RENDERBUFFER, rbo);

    var result = gl.checkFramebufferStatus(gl.FRAMEBUFFER);
    if (result != gl.FRAMEBUFFER_COMPLETE) {
        console.log("frame buffer is not complete!!");
        return false;
    }
    gl.bindFramebuffer(gl.FRAMEBUFFER, null);
    return true;
}

function initialize_sphere() {
    
    // openGL code starts
    // vertex shader
    var vertexShaderSourceCode_pv =
        "#version 300 es" +   // webgl 2.0 is subset of OpenGL ES 3.0 not 3.20
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
        "}";

    var vertexShaderObject_pv = gl.createShader(gl.VERTEX_SHADER);

    gl.shaderSource(vertexShaderObject_pv, vertexShaderSourceCode_pv);

    gl.compileShader(vertexShaderObject_pv);

    if (gl.getShaderParameter(vertexShaderObject_pv, gl.COMPILE_STATUS) == false) {
        var error = gl.getShaderInfoLog(vertexShaderObject_pv);
        if (error.length > 0) {
            alert("Vertex Shader Compilation Error :" + error);
            unintialize();
        }
    }

    // fragment shader
    var fragmentShaderSourceCode_pv =
        "#version 300 es" +
        "\n" +
        "precision highp float;" +
        "in vec3 phong_ads_light;" +
        "out vec4 FragColor;" +
        "void main(void)" +
        "{" +
        "FragColor = vec4(phong_ads_light,1.0);" +
        "}";

    var fragmentShaderObject_pv = gl.createShader(gl.FRAGMENT_SHADER);

    gl.shaderSource(fragmentShaderObject_pv, fragmentShaderSourceCode_pv);

    gl.compileShader(fragmentShaderObject_pv);

    if (gl.getShaderParameter(fragmentShaderObject_pv, gl.COMPILE_STATUS) == false) {
        var error = gl.getShaderInfoLog(fragmentShaderObject_pv);
        if (error.length > 0) {
            alert("Fragment Shader Compilation Error :" + error);
            unintialize();
        }
    }
    // shader program
    shaderProgramObject_pv = gl.createProgram();
    gl.attachShader(shaderProgramObject_pv, vertexShaderObject_pv);
    gl.attachShader(shaderProgramObject_pv, fragmentShaderObject_pv);

    // prilinking shaders' attribute binding
    gl.bindAttribLocation(shaderProgramObject_pv, webGLMacros.AMC_ATTRIBUTE_POSITION, "a_position");
    gl.bindAttribLocation(shaderProgramObject_pv, webGLMacros.AMC_ATTRIBUTE_NORMAL, "a_normal");

    // shader program linking
    gl.linkProgram(shaderProgramObject_pv);

    if (gl.getProgramParameter(shaderProgramObject_pv, gl.LINK_STATUS) == false) {
        var error = gl.getProgramInfoLog(shaderProgramObject_pv);
        if (error.length > 0) {
            alert("Shader Program Error :" + error);
            unintialize();
        }
    }

    // post linking
    modelMatrixUniform_pv = gl.getUniformLocation(shaderProgramObject_pv, "u_modelMatrix");
    viewMatrixUniform_pv = gl.getUniformLocation(shaderProgramObject_pv, "u_viewMatrix");
    projectionMatrixUniform_pv = gl.getUniformLocation(shaderProgramObject_pv, "u_projectionMatrix");


    laUniform_pv[0] = gl.getUniformLocation(shaderProgramObject_pv, "u_la[0]");
    ldUniform_pv[0] = gl.getUniformLocation(shaderProgramObject_pv, "u_ld[0]");
    lsUniform_pv[0] = gl.getUniformLocation(shaderProgramObject_pv, "u_ls[0]");
    lightPositionUniform_pv[0] = gl.getUniformLocation(shaderProgramObject_pv, "u_lightPosition[0]");

    laUniform_pv[1] = gl.getUniformLocation(shaderProgramObject_pv, "u_la[1]");
    ldUniform_pv[1] = gl.getUniformLocation(shaderProgramObject_pv, "u_ld[1]");
    lsUniform_pv[1] = gl.getUniformLocation(shaderProgramObject_pv, "u_ls[1]");
    lightPositionUniform_pv[1] = gl.getUniformLocation(shaderProgramObject_pv, "u_lightPosition[1]");

    laUniform_pv[2] = gl.getUniformLocation(shaderProgramObject_pv, "u_la[2]");
    ldUniform_pv[2] = gl.getUniformLocation(shaderProgramObject_pv, "u_ld[2]");
    lsUniform_pv[2] = gl.getUniformLocation(shaderProgramObject_pv, "u_ls[2]");
    lightPositionUniform_pv[2] = gl.getUniformLocation(shaderProgramObject_pv, "u_lightPosition[2]");


    kaUniform_pv = gl.getUniformLocation(shaderProgramObject_pv, "u_ka");
    kdUniform_pv = gl.getUniformLocation(shaderProgramObject_pv, "u_kd");
    ksUniform_pv = gl.getUniformLocation(shaderProgramObject_pv, "u_ks");

    lightingEnabledUniform_pv = gl.getUniformLocation(shaderProgramObject_pv, "u_lightingEnabled");
    materialShininessUniform_pv = gl.getUniformLocation(shaderProgramObject_pv, "u_materialShininess");

    // openGL code starts
    // vertex shader
    var vertexShaderSourceCode_pf =
        "#version 300 es" +   // webgl 2.0 is subset of OpenGL ES 3.0 not 3.20
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
        "}";

    var vertexShaderObject_pf = gl.createShader(gl.VERTEX_SHADER);

    gl.shaderSource(vertexShaderObject_pf, vertexShaderSourceCode_pf);

    gl.compileShader(vertexShaderObject_pf);

    if (gl.getShaderParameter(vertexShaderObject_pf, gl.COMPILE_STATUS) == false) {
        var error = gl.getShaderInfoLog(vertexShaderObject_pf);
        if (error.length > 0) {
            alert("Vertex Shader Compilation Error :" + error);
            unintialize();
        }
    }

    // fragment shader
    var fragmentShaderSourceCode_pf =
        "#version 300 es" +
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
        "}";

    var fragmentShaderObject_pf = gl.createShader(gl.FRAGMENT_SHADER);

    gl.shaderSource(fragmentShaderObject_pf, fragmentShaderSourceCode_pf);

    gl.compileShader(fragmentShaderObject_pf);

    if (gl.getShaderParameter(fragmentShaderObject_pf, gl.COMPILE_STATUS) == false) {
        var error = gl.getShaderInfoLog(fragmentShaderObject_pf);
        if (error.length > 0) {
            alert("Fragment Shader Compilation Error :" + error);
            unintialize();
        }
    }
    // shader program
    shaderProgramObject_pf = gl.createProgram();
    gl.attachShader(shaderProgramObject_pf, vertexShaderObject_pf);
    gl.attachShader(shaderProgramObject_pf, fragmentShaderObject_pf);

    // prilinking shaders' attribute binding
    gl.bindAttribLocation(shaderProgramObject_pf, webGLMacros.AMC_ATTRIBUTE_POSITION, "a_position");
    gl.bindAttribLocation(shaderProgramObject_pf, webGLMacros.AMC_ATTRIBUTE_NORMAL, "a_normal");

    // shader program linking
    gl.linkProgram(shaderProgramObject_pf);

    if (gl.getProgramParameter(shaderProgramObject_pf, gl.LINK_STATUS) == false) {
        var error = gl.getProgramInfoLog(shaderProgramObject_pf);
        if (error.length > 0) {
            alert("Shader Program Error :" + error);
            unintialize();
        }
    }

    // post linking
    modelMatrixUniform_pf = gl.getUniformLocation(shaderProgramObject_pf, "u_modelMatrix");
    viewMatrixUniform_pf = gl.getUniformLocation(shaderProgramObject_pf, "u_viewMatrix");
    projectionMatrixUniform_pf = gl.getUniformLocation(shaderProgramObject_pf, "u_projectionMatrix");


    laUniform_pf[0] = gl.getUniformLocation(shaderProgramObject_pf, "u_la[0]");
    ldUniform_pf[0] = gl.getUniformLocation(shaderProgramObject_pf, "u_ld[0]");
    lsUniform_pf[0] = gl.getUniformLocation(shaderProgramObject_pf, "u_ls[0]");
    lightPositionUniform_pf[0] = gl.getUniformLocation(shaderProgramObject_pf, "u_lightPosition[0]");

    laUniform_pf[1] = gl.getUniformLocation(shaderProgramObject_pf, "u_la[1]");
    ldUniform_pf[1] = gl.getUniformLocation(shaderProgramObject_pf, "u_ld[1]");
    lsUniform_pf[1] = gl.getUniformLocation(shaderProgramObject_pf, "u_ls[1]");
    lightPositionUniform_pf[1] = gl.getUniformLocation(shaderProgramObject_pf, "u_lightPosition[1]");

    laUniform_pf[2] = gl.getUniformLocation(shaderProgramObject_pf, "u_la[2]");
    ldUniform_pf[2] = gl.getUniformLocation(shaderProgramObject_pf, "u_ld[2]");
    lsUniform_pf[2] = gl.getUniformLocation(shaderProgramObject_pf, "u_ls[2]");
    lightPositionUniform_pf[2] = gl.getUniformLocation(shaderProgramObject_pf, "u_lightPosition[2]");


    kaUniform_pf = gl.getUniformLocation(shaderProgramObject_pf, "u_ka");
    kdUniform_pf = gl.getUniformLocation(shaderProgramObject_pf, "u_kd");
    ksUniform_pf = gl.getUniformLocation(shaderProgramObject_pf, "u_ks");

    lightingEnabledUniform_pf = gl.getUniformLocation(shaderProgramObject_pf, "u_lightingEnabled");
    materialShininessUniform_pf = gl.getUniformLocation(shaderProgramObject_pf, "u_materialShininess");


    sphere = new Mesh();
    makeSphere(sphere, 2.0, 30, 30);
    // depth and clear color related code 
    gl.clearDepth(1.0);
    gl.enable(gl.DEPTH_TEST);
    gl.depthFunc(gl.LEQUAL);

    // clear the screen by color blue
    gl.clearColor(0.0, 0.0, 0.0, 1.0); // by default float taken

    ambientSphere[0] = [0.0, 0.0, 0.0];
    diffuseSphere[0] = [1.0, 0.0, 0.0];
    specularSphere[0] = [1.0, 0.0, 0.0];
    lightPositionSphere[0] = [0.0, 0.0, 0.0, 1.0];

    ambientSphere[1] = [0.0, 0.0, 0.0];
    diffuseSphere[1] = [0.0, 1.0, 0.0];
    specularSphere[1] = [0.0, 1.0, 0.0];
    lightPositionSphere[1] = [0.0, 0.0, 0.0, 1.0];

    ambientSphere[2] = [0.0, 0.0, 0.0];
    diffuseSphere[2] = [0.0, 0.0, 1.0];
    specularSphere[2] = [0.0, 0.0, 1.0];
    lightPositionSphere[2] = [0.0, 0.0, 0.0, 1.0];

    perspectiveProjectionMatrixSphere = mat4.create();
    resize_sphere();
}

function resize() {
    //code
    if (bFullscreen == true) {
        canvas.width = window.innerWidth;
        canvas.height = window.innerHeight;
    }
    else {
        canvas.width = canvas_original_width;
        canvas.height = canvas_original_height;
    }

    winWidth_fbo = canvas.width;
    winHeight_fbo = canvas.height;
    
    if (canvas.height == 0) {
        canvas.height = 1;
    }
    gl.viewport(0, 0, canvas.width, canvas.height);
    mat4.perspective(perspectiveProjectionMatrix, 45.0, parseFloat(canvas.width) / parseFloat(canvas.height), 0.1, 100.0);
}

function resize_sphere(width, height) {
    if (height == 0) {
        height = 1;
    }

    gl.viewport(0, 0, width, height);
    mat4.perspective(perspectiveProjectionMatrix, 45.0, parseFloat(width) / parseFloat(height), 0.1, 100.0);

}

function display() {
    //code
    if (bFBOResult == true) {
        display_sphere(FBO_WIDTH, FBO_HEIGHT);
        update_sphere();
    }
    gl.clearColor(1.0, 1.0, 1.0, 1.0);
    resize();
    gl.clear(gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT);

    gl.useProgram(shaderProgramObject);
    var translationMatrix = mat4.create();
    var rotationMatrix = mat4.create();
    var scaleMatrix = mat4.create();
    var modelViewMatrix = mat4.create();
    var modelViewProjectionMatrix = mat4.create();


    mat4.translate(translationMatrix, translationMatrix, [0.0, 0.0, -5.0]);
    mat4.rotateX(rotationMatrix, rotationMatrix, degToRad(angleCube));
    mat4.rotateY(rotationMatrix, rotationMatrix, degToRad(angleCube));
    mat4.rotateZ(rotationMatrix, rotationMatrix, degToRad(angleCube));
    mat4.scale(scaleMatrix, scaleMatrix, [0.75, 0.75, 0.75]);
    mat4.multiply(modelViewMatrix, translationMatrix, rotationMatrix);
    mat4.multiply(modelViewMatrix, modelViewMatrix, scaleMatrix);
    mat4.multiply(modelViewProjectionMatrix, perspectiveProjectionMatrix, modelViewMatrix);

    gl.uniformMatrix4fv(mvpMatrixUniform, false, modelViewProjectionMatrix);
    gl.activeTexture(gl.TEXTURE0);
    gl.bindTexture(gl.TEXTURE_2D, fbo_texture);
    gl.uniform1i(textureSamplerUniform, 0);
    gl.bindVertexArray(vao_cube);
    gl.drawArrays(gl.TRIANGLE_FAN, 0, 4);
    gl.drawArrays(gl.TRIANGLE_FAN, 4, 4);
    gl.drawArrays(gl.TRIANGLE_FAN, 8, 4);
    gl.drawArrays(gl.TRIANGLE_FAN, 12, 4);
    gl.drawArrays(gl.TRIANGLE_FAN, 16, 4);
    gl.drawArrays(gl.TRIANGLE_FAN, 20, 4);
    gl.bindTexture(gl.TEXTURE_2D, null);

    gl.bindVertexArray(null);

    gl.useProgram(null);

    update();

    // double buffering emulation (khot)
    requestAnimationFrame(display, canvas);  // like glswapbuffser
}

function display_sphere(textureWidth, textureHeight) {
    // code
    gl.bindFramebuffer(gl.FRAMEBUFFER, fbo);
    gl.clearColor(0.0, 0.0, 0.0, 1.0);
    resize_sphere(textureWidth, textureHeight);
    gl.clear(gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT);

    if (choosenShader == 0) {
        gl.useProgram(shaderProgramObject_pv);

    }
    else if (choosenShader == 1) {
        gl.useProgram(shaderProgramObject_pf);
    }
    var translationMatrix = mat4.create();
    var modelMatrix = mat4.create();
    var viewMatrix = mat4.create();
    mat4.translate(modelMatrix, translationMatrix, [0.0, 0.0, -5.0]);

    if (choosenShader == 0) {
        gl.uniformMatrix4fv(modelMatrixUniform_pv, false, modelMatrix);
        gl.uniformMatrix4fv(viewMatrixUniform_pv, false, viewMatrix);
        gl.uniformMatrix4fv(projectionMatrixUniform_pv, false, perspectiveProjectionMatrix);
    }
    else {
        gl.uniformMatrix4fv(modelMatrixUniform_pf, false, modelMatrix);
        gl.uniformMatrix4fv(viewMatrixUniform_pf, false, viewMatrix);
        gl.uniformMatrix4fv(projectionMatrixUniform_pf, false, perspectiveProjectionMatrix);

    }
    sinValue = Math.sin(degToRad(angleSphere - 0.25)) * 6.0
    cosValue = Math.cos(degToRad(angleSphere - 0.5)) * 6.0
    console.log(sinValue + "----------------" + cosValue)

    lightPositionSphere[0] = [cosValue, sinValue, 0.0, 1.0];
    lightPositionSphere[1] = [sinValue, 0.0, cosValue, 1.0];
    lightPositionSphere[2] = [0.0, sinValue, cosValue + 2.0, 1.0];

    if (choosenShader == 0) {
        if (bLight == 1) {
            gl.uniform1i(lightingEnabledUniform_pv, 1); // keypress sent
            for (var i = 0; i < 3; i++) {
                gl.uniform3fv(laUniform_pv[i], ambientSphere[i]);
                gl.uniform3fv(ldUniform_pv[i], diffuseSphere[i]);
                gl.uniform3fv(lsUniform_pv[i], specularSphere[i]);
                gl.uniform4fv(lightPositionUniform_pv[i], lightPositionSphere[i]);

            }
            gl.uniform3fv(kaUniform_pv, materialAmbientSphere);
            gl.uniform3fv(kdUniform_pv, materialDiffuseSphere);
            gl.uniform3fv(ksUniform_pv, materialSpecularSphere);
            gl.uniform1f(materialShininessUniform_pv, materialShininessSphere);
        }
        else {
            gl.uniform1i(lightingEnabledUniform_pv, 0);
        }

    }
    else if (choosenShader == 1) {
        if (bLight == 1) {
            gl.uniform1i(lightingEnabledUniform_pf, 1); // keypress sent
            for (var i = 0; i < 3; i++) {
                gl.uniform3fv(laUniform_pf[i], ambientSphere[i]);
                gl.uniform3fv(ldUniform_pf[i], diffuseSphere[i]);
                gl.uniform3fv(lsUniform_pf[i], specularSphere[i]);
                gl.uniform4fv(lightPositionUniform_pf[i], lightPositionSphere[i]);

            }
            gl.uniform3fv(kaUniform_pf, materialAmbientSphere);
            gl.uniform3fv(kdUniform_pf, materialDiffuseSphere);
            gl.uniform3fv(ksUniform_pf, materialSpecularSphere);
            gl.uniform1f(materialShininessUniform_pf, materialShininessSphere);
        }
        else {
            gl.uniform1i(lightingEnabledUniform_pf, 0);
        }
    }

    sphere.draw();
    gl.useProgram(null);


    gl.bindFramebuffer(gl.FRAMEBUFFER, null);
}

function update() {
    //code

    angleCube = angleCube + 0.5;
    if (angleCube > 360.0) {
        angleCube = 0.0;
    }

}

function update_sphere() {

    angleSphere = angleSphere + 1.5;
    if (angleSphere > 360.0) {
        angleSphere = 0.0;
    }
}

function degToRad(degrees) {
    return (degrees * Math.PI / 180.0);
}

function unintialize() {
    //code

    uninitialize_sphere();
    if (vbo_cube_position) {
        gl.deleteBuffer(vbo_cube_position);
        vbo_cube_position = null;
    }

    if (vbo_cube_texcoord) {
        gl.deleteBuffer(vbo_cube_texcoord);
        vbo_cube_texcoord = null;
    }

    if (vao_cube) {
        gl.deleteVertexArray(vao_cube);
        vao_cube = null;
    }

    if (shaderProgramObject) {
        gl.useProgram(shaderProgramObject);
        var shaderObjects = gl.getAttachedShaders(shaderProgramObject);
        for (let i = 0; i < shaderObjects.length; i++) {
            gl.detachShader(shaderProgramObject, shaderObjects[i]);
            gl.deleteShader(shaderObjects[i]);
            shaderObjects[i] = null;
        }
        shaderObjects = null;
        gl.useProgram(null);
        gl.deleteProgram(shaderProgramObject);
        shaderProgramObject = null;
    }

}

function uninitialize_sphere() {
    if (fbo_texture) {
        gl.deleteTexture(fbo_texture);
        fbo_texture = null;
    }
    if (rbo) {
        gl.deleteRenderbuffer(rbo);
        rbo = null;
    }
    if (fbo) {
        gl.deleteFramebuffer(fbo);
        fbo = null;
    }

    if (sphere) {
        sphere.deallocate();
        sphere = null;
    }

    var shaderObj = null;
    if (choosenShader = 0) {
        shaderObj = shaderProgramObject_pv;
    }
    if (choosenShader = 1) {
        shaderObj = shaderProgramObject_pf;
    }
    if (shaderObj) {
        gl.useProgram(shaderObj);
        var shaderObjects = gl.getAttachedShaders(shaderObj);
        for (let i = 0; i < shaderObjects.length; i++) {
            gl.detachShader(shaderObj, shaderObjects[i]);
            gl.deleteShader(shaderObjects[i]);
            shaderObjects[i] = null;
        }
        shaderObjects = null;
        gl.useProgram(null);
        gl.deleteProgram(shaderObj);
        shaderObj = null;
    }

}

// keyboard event listener
function keyDown(event) {
    // code
    switch (event.keyCode) {
        case 69: // e
            toggleFullscreen();
            break
        case 81: // q
            unintialize();
            window.close(); // not all browser will follow this
            break
        case 70: // f
            // per fragment flag
            choosenShader = 1;
            break;
        case 86:
            // per vertex flag
            choosenShader = 0;
            break;

        case 76: // L
            if (!bLight) {
                bLight = 1;
            }
            else {
                bLight = 0;
            }
            break;

        default:
            break;
    }

}
// mouse event listener
function mouseDown() {
    // button

}
