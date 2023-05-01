//headers
#include <windows.h>               //standard windows header
#include <stdio.h>                 //standard C io header 
#include <math.h>                  //standard C math header 
#include <d3d11.h>                 //Direct3D 11 header
#include <d3dcompiler.h>           //Direct3D shader compilation
#include "D3D.h"             //Resources header

#pragma warning(disable: 4838)     //disable header warnings
#include "xnamath.h"       //Direct3D math header
#include "WICTextureLoader.h"      //Direct3D texture loader header

//import libraries
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "kernel32.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "DirectXTK.lib")

//symbolic constants
#define WIN_WIDTH  800             //initial width of window  
#define WIN_HEIGHT 600             //initial height of window

#define VK_F       0x46            //virtual key code of F key
#define VK_f       0x60            //virtual key code of f key

//type declarations
struct Light
{
    float ambient[4];
    float diffuse[4];
    float specular[4];
    float position[4];
};

struct Material
{
    float ambient[4];
    float diffuse[4];
    float specular[4];
    float shininess;
};

//callback procedure declaration
LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);

//global variables
HWND   ghwnd = NULL;              //handle to a window
DWORD dwStyle = NULL;              //window style
WINDOWPLACEMENT wpPrev;            //structure for holding previous window position

bool gbActiveWindow = false;       //flag indicating whether window is active or not
bool gbFullscreen = false;         //flag indicating whether window is fullscreen or not

FILE* gpFile = NULL;              //log file handle
char gLogFileName[] = "log.txt";   //log file name

float gClearColor[4];
IDXGISwapChain* gpIDXGISwapChain = NULL;
ID3D11Device* gpID3D11Device = NULL;
ID3D11DeviceContext* gpID3D11DeviceContext = NULL;
ID3D11RenderTargetView* gpID3D11RenderTargetView = NULL;
ID3D11RasterizerState* gpID3D11RasterizerState = NULL;
ID3D11DepthStencilView* gpID3D11DepthStencilView = NULL;

ID3D11VertexShader* gpID3D11VertexShader = NULL;
ID3D11PixelShader* gpID3D11PixelShader = NULL;
ID3D11InputLayout* gpID3D11InputLayout = NULL;
ID3D11Buffer* gpID3D11Buffer_ConstantBuffer = NULL;

ID3D11Buffer* gpID3D11Buffer_VertexBuffer_position = NULL;
ID3D11Buffer* gpID3D11Buffer_VertexBuffer_normal = NULL;

struct CBUFFER
{
    XMMATRIX WorldMatrix;
    XMMATRIX ViewMatrix;
    XMMATRIX ProjectionMatrix;

    XMVECTOR La[2];
    XMVECTOR Ld[2];
    XMVECTOR Ls[2];
    XMVECTOR LightPosition[2];

    XMVECTOR Ka;
    XMVECTOR Kd;
    XMVECTOR Ks;
    float MaterialShininess;

    unsigned int KeyPressed;
};

XMMATRIX perspectiveProjectionMatrix;
struct Light light[2];
struct Material material;

bool gbLight = false;
bool bAnimate = false;

//windows entry point function
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow)
{
    //function declarations
    HRESULT Initialize(void);                              //initialize OpenGL state machine
    void Display(void);                                    //render scene

    //variable declarations
    WNDCLASSEX wndclass;                                   //structure holding window class attributes
    MSG msg;                                               //structure holding message attributes
    HWND hwnd;                                             //handle to a window
    TCHAR szAppName[] = TEXT("Direct3D11");                //name of window class
    HRESULT hr = S_OK;

    int cxScreen, cyScreen;                                //screen width and height for centering window
    int init_x, init_y;                                    //top-left coordinates of centered window
    bool bDone = false;                                    //flag indicating whether or not to exit from game loop

    //code
    //create/open  'log.txt' file
    if (fopen_s(&gpFile, gLogFileName, "w") != 0)
    {
        TCHAR str[255];
        wsprintf(str, TEXT("Failed to open %s file"), gLogFileName);
        MessageBox(NULL, str, TEXT("Error"), MB_OK | MB_ICONERROR);
        exit(EXIT_FAILURE);
    }
    else
    {
        fprintf(gpFile, "--------------------------------------------------------------------------\n");
        fprintf(gpFile, "-> Program started successfully\n");
        fclose(gpFile);
    }

    //initialization of WNDCLASSEX
    wndclass.cbSize = sizeof(WNDCLASSEX);                            //size of structure
    wndclass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;            //window style
    wndclass.lpfnWndProc = WndProc;                                       //address of callback procedure
    wndclass.cbClsExtra = 0;                                             //extra class bytes
    wndclass.cbWndExtra = 0;                                             //extra window bytes
    wndclass.hInstance = hInstance;                                     //handle to a program
    wndclass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(MYICON));  //handle to an icon
    wndclass.hCursor = LoadCursor((HINSTANCE)NULL, IDC_ARROW);        //handle to a cursor
    wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);           //handle to a background brush
    wndclass.lpszClassName = szAppName;                                     //name of a custom class
    wndclass.lpszMenuName = NULL;                                          //name of a custom menu
    wndclass.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(MYICON));  //handle to a small icon

    //register above class
    RegisterClassEx(&wndclass);

    //get screen width and height
    cxScreen = GetSystemMetrics(SM_CXSCREEN);
    cyScreen = GetSystemMetrics(SM_CYSCREEN);

    //calculate top-left coordinates for a centered window
    init_x = (cxScreen / 2) - (WIN_WIDTH / 2);
    init_y = (cyScreen / 2) - (WIN_HEIGHT / 2);

    //create window
    hwnd = CreateWindowEx(WS_EX_APPWINDOW,                //extended window style          
        szAppName,                                    //class name
        TEXT("D3D11"),   //window caption
        WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN |       //window stle
        WS_CLIPSIBLINGS | WS_VISIBLE,
        init_x,                                       //X-coordinate of top left corner of window 
        init_y,                                       //Y-coordinate of top left corner of window
        WIN_WIDTH,                                    //initial window width                 
        WIN_HEIGHT,                                   //initial window height
        (HWND)NULL,                                   //handle to a parent window  : NULL desktop
        (HMENU)NULL,                                  //handle to a menu : NULL no menu
        hInstance,                                    //handle to a program instance
        (LPVOID)NULL);                                //data to be sent to window callback : NULL no data to send      

    //store handle to a window in global handle
    ghwnd = hwnd;

    //initialize OpenGL rendering context
    hr = Initialize();
    if (FAILED(hr))
    {
        fopen_s(&gpFile, gLogFileName, "a+");
        fprintf(gpFile, "Error : Initialize() failed.\n");
        fclose(gpFile);
        DestroyWindow(ghwnd);
    }

    ShowWindow(hwnd, iCmdShow);                 //set specified window's show state
    SetForegroundWindow(hwnd);                  //brings the thread that created the specified window to foreground
    SetFocus(hwnd);                             //set the keyboard focus to specified window 

    //game loop
    while (bDone == false)
    {
        //1 : pointer to structure for window message
        //2 : handle to window : NULL do not process child window's messages 
        //3 : message filter min range : 0 no range filtering
        //4 : message filter max range : 0 no range filtering
        //5 : remove message from queue after processing from PeekMessage
        if (PeekMessage(&msg, (HWND)NULL, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)           //if current message is WM_QUIT then exit from game loop
            {
                bDone = true;
            }
            else
            {
                TranslateMessage(&msg);          //translate virtual-key message into character message
                DispatchMessage(&msg);           //dispatch message  to window procedure
            }
        }
        else
        {
            if (gbActiveWindow == true)           //if window has keyboard focus 
            {
                Display();                       //render the scene
            }
        }
    }

    return ((int)msg.wParam);                    //exit code given by PostQuitMessage 
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
    //function declarations
    void ToggleFullscreen(void);                //toggle window between fullscreen and previous position 
    HRESULT Resize(int, int);                   //handle window resize event
    void Cleanup(void);                         //release resources  

    //variable declarations
    HRESULT hr = S_OK;

    //code
    switch (iMsg)
    {
    case WM_SETFOCUS:                        //event : window has keyboard focus
        gbActiveWindow = true;
        break;

    case WM_KILLFOCUS:                       //event : window dosen't have keyboard focus
        gbActiveWindow = false;
        break;

    case WM_ERASEBKGND:                      //event : window background must be erased 
        return (0);                          //dont let DefWindowProc handle this event

    case WM_SIZE:                            //event : window is resized
        if (gpID3D11DeviceContext)
        {
            hr = Resize(LOWORD(lParam), HIWORD(lParam));
            if (FAILED(hr))
            {
                fopen_s(&gpFile, gLogFileName, "a+");
                fprintf(gpFile, "Error : Resize() failed.\n");
                fclose(gpFile);
                DestroyWindow(ghwnd);
            }
        }
        break;

    case WM_KEYDOWN:                         //event : a key has been pressed
        switch (wParam)
        {
        case VK_ESCAPE:
            DestroyWindow(hwnd);
            break;

        case VK_F:
        case VK_f:
            ToggleFullscreen();
            break;

        default:
            break;
        }
        break;

    case WM_CHAR:
        switch (wParam)
        {
        case 'L':
        case 'l':
            if (gbLight == true)
            {
                gbLight = false;
            }
            else
            {
                gbLight = true;
            }
            break;

        case 'A':
        case 'a':
            if (bAnimate == true)
            {
                bAnimate = false;
            }
            else
            {
                bAnimate = true;
            }
            break;

        default:
            break;
        }
        break;

    case WM_CLOSE:                           //event : window is closed from sysmenu or close button
        DestroyWindow(hwnd);
        break;

    case WM_DESTROY:
        Cleanup();
        PostQuitMessage(0);
        break;

    default:
        break;
    }

    //call default window procedure for unhandled messages
    return (DefWindowProc(hwnd, iMsg, wParam, lParam));
}

void ToggleFullscreen(void)
{
    //variable declarations
    MONITORINFO mi = { sizeof(MONITORINFO) };            //structure holding monitor information

    //code
    if (gbFullscreen == false)                            //if screen is not in fulscreen mode 
    {
        dwStyle = GetWindowLong(ghwnd, GWL_STYLE);       //get window style
        if (dwStyle & WS_OVERLAPPEDWINDOW)                //if current window style has WS_OVERLAPPEDWINDOW
        {
            if (GetWindowPlacement(ghwnd, &wpPrev) && GetMonitorInfo(MonitorFromWindow(ghwnd, MONITORINFOF_PRIMARY), &mi))
            {
                // if wpPrev is successfully filled with current window placement
                // and mi is successfully filled with primary monitor info then
                // 1 -> Remove WS_OVERLAPPEDWINDOW style
                // 2 -> Set window position by aligning left-top corner of window 
                //     to left-top corner of monitor and setting width and height 
                //     to monitor's width and height (effectively making window 
                //     fullscreen)
                // SWP_NOZORDER : Don't change Z-order
                // SWP_FRAMECHANGED: Forces recalculation of New Client area (WM_NCCALCSIZE)
                SetWindowLong(ghwnd, GWL_STYLE, (dwStyle & ~WS_OVERLAPPEDWINDOW));
                SetWindowPos(ghwnd,                                     //     top 
                    HWND_TOP,                                           //left +--------------+ right
                    mi.rcMonitor.left,                                  //     |              |
                    mi.rcMonitor.top,                                   //     |              |
                    mi.rcMonitor.right - mi.rcMonitor.left,             //     |              |
                    mi.rcMonitor.bottom - mi.rcMonitor.top,             //     |              |
                    SWP_NOZORDER | SWP_FRAMECHANGED);                   //     +--------------+
            }                                                           //     bottom
        }

        ShowCursor(false);                                 //hide the cursor
        gbFullscreen = true;
    }
    else                                                   //if screen is in fullscreen mode
    {
        // Toggle the window to previously saved dimension
        // 1 -> Add WS_OVERLAPPEDWINDOW to window style 
        // 2 -> Set window placement to stored previous placement
        // 3 -> Force the effects of SetWindowPlacement by call to 
        //      SetWindowPos with
        // SWP_NOMOVE : Don't change left top position of window 
        //              i.e ignore third and forth parameters
        // SWP_NOSIZE : Don't change dimensions of window
        //              i.e ignore fifth and sixth parameters
        // SWP_NOZORDER : Don't change Z-order of the window and
        //              its child windows
        // SWP_NOOWNERZORDER : Don't change Z-order of owner of the 
        //              window (reffered by ghwnd)
        // SWP_FRAMECHANGED : Forces recalculation of New Client area (WM_NCCALCSIZE)
        SetWindowLong(ghwnd, GWL_STYLE, (dwStyle | WS_OVERLAPPEDWINDOW));
        SetWindowPlacement(ghwnd, &wpPrev);
        SetWindowPos(ghwnd,
            HWND_TOP,
            0,
            0,
            0,
            0,
            SWP_NOOWNERZORDER | SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER | SWP_FRAMECHANGED);

        ShowCursor(true);            //show cursor
        gbFullscreen = false;
    }
}

HRESULT Initialize(void)
{
    //function declarations
    void Cleanup(void);
    HRESULT printD3DInfo(void);
    HRESULT Resize(int width, int height);

    //variable declarations
    DXGI_SWAP_CHAIN_DESC dxgiSwapChainDesc;

    D3D_FEATURE_LEVEL d3dFeatureLevel_required = D3D_FEATURE_LEVEL_11_0;
    D3D_FEATURE_LEVEL d3dFeatureLevel_acquired = D3D_FEATURE_LEVEL_10_0;
    D3D_DRIVER_TYPE d3dDriverTypes[] = { D3D_DRIVER_TYPE_HARDWARE, D3D_DRIVER_TYPE_WARP, D3D_DRIVER_TYPE_REFERENCE };
    D3D_DRIVER_TYPE d3dDriverType;

    UINT numFeatureLevels = 1;
    UINT numDriverTypes = 0;
    UINT createDeviceFlags = 0;

    HRESULT hr = S_OK;

    //code
    hr = printD3DInfo();
    if (FAILED(hr))
    {
        fopen_s(&gpFile, gLogFileName, "a+");
        fprintf(gpFile, "Error : printD3DInfo() failed.\n");
        fclose(gpFile);
        return (hr);
    }

    //initialize dxgiSwapChainDesc structure
    ZeroMemory((void*)&dxgiSwapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));
    dxgiSwapChainDesc.BufferCount = 1;
    dxgiSwapChainDesc.BufferDesc.Width = WIN_WIDTH;
    dxgiSwapChainDesc.BufferDesc.Height = WIN_HEIGHT;
    dxgiSwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    dxgiSwapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
    dxgiSwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
    dxgiSwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    dxgiSwapChainDesc.OutputWindow = ghwnd;
    dxgiSwapChainDesc.SampleDesc.Count = 1;
    dxgiSwapChainDesc.SampleDesc.Quality = 0;
    dxgiSwapChainDesc.Windowed = TRUE;

    //get total number of types of drivers
    numDriverTypes = sizeof(d3dDriverTypes) / sizeof(d3dDriverTypes[0]);

    //enumerate driver types to choose the best
    for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
    {
        d3dDriverType = d3dDriverTypes[driverTypeIndex];
        hr = D3D11CreateDeviceAndSwapChain(
            NULL,
            d3dDriverType,
            NULL,
            createDeviceFlags,
            &d3dFeatureLevel_required,
            numFeatureLevels,
            D3D11_SDK_VERSION,
            &dxgiSwapChainDesc,
            &gpIDXGISwapChain,
            &gpID3D11Device,
            &d3dFeatureLevel_acquired,
            &gpID3D11DeviceContext
        );

        if (SUCCEEDED(hr))
            break;
    }

    if (FAILED(hr))
    {
        fopen_s(&gpFile, gLogFileName, "a+");
        fprintf(gpFile, "Error : D3D11CreateDeviceAndSwapChain() failed.\n");
        fclose(gpFile);
        return (hr);
    }
    else
    {
        fopen_s(&gpFile, gLogFileName, "a+");
        fprintf(gpFile, "-> Driver Info\n");
        fprintf(gpFile, "Driver Type    : ");
        if (d3dDriverType == D3D_DRIVER_TYPE_HARDWARE)
        {
            fprintf(gpFile, "Hardware Type.\n");
        }
        else if (d3dDriverType == D3D_DRIVER_TYPE_WARP)
        {
            fprintf(gpFile, "Warp Type.\n");
        }
        else if (d3dDriverType == D3D_DRIVER_TYPE_REFERENCE)
        {
            fprintf(gpFile, "Reference Type.\n");
        }
        else
        {
            fprintf(gpFile, "Unknown Type.\n");
        }

        fprintf(gpFile, "Feature Level  : ");
        if (d3dFeatureLevel_acquired == D3D_FEATURE_LEVEL_11_0)
        {
            fprintf(gpFile, "11.0\n");
        }
        else if (d3dFeatureLevel_acquired == D3D_FEATURE_LEVEL_10_1)
        {
            fprintf(gpFile, "10.1\n");
        }
        else if (d3dFeatureLevel_acquired == D3D_FEATURE_LEVEL_10_0)
        {
            fprintf(gpFile, "10.0\n");
        }
        else
        {
            fprintf(gpFile, "Unknown.\n");
        }
        fprintf(gpFile, "--------------------------------------------------------------------------\n");
        fclose(gpFile);
    }

    //per pixel lighting shader program

    //vertex shader
    const char* vertexShaderSourceCode =
        "cbuffer ConstantBuffer"                                                                                        \
        "{"                                                                                                             \
        "   float4x4 worldMatrix;"                                                                                      \
        "   float4x4 viewMatrix;"                                                                                       \
        "   float4x4 projectionMatrix;"                                                                                 \

        "   float4 la[2];"                                                                                              \
        "   float4 ld[2];"                                                                                              \
        "   float4 ls[2];"                                                                                              \
        "   float4 lightPosition[2];"                                                                                   \

        "   float4 ka;"                                                                                                 \
        "   float4 kd;"                                                                                                 \
        "   float4 ks;"                                                                                                 \
        "   float materialShininess;"                                                                                   \

        "   uint keyPressed;"                                                                                           \
        "}"                                                                                                             \

        "struct vertex_output"                                                                                          \
        "{"                                                                                                             \
        "   float4 position           : SV_POSITION;"                                                                               \
        "   float3 transformed_normal : NORMAL0;"                                                                         \
        "   float3 light_direction[2] : light_direction;"                                                                         \
        "   float3 viewer_vector      : viewer_vector;"                                                                              \
        "};"                                                                                                            \

        "vertex_output main(float4 pos:POSITION, float4 normal:NORMAL)"                                                 \
        "{"                                                                                                             \
        "   vertex_output output;"                                                                                      \
        "   if(keyPressed == 1)"                                                                                        \
        "   {"                                                                                                          \
        "       float4 eye_position = mul(worldMatrix, pos);"                                                           \
        "       eye_position = mul(viewMatrix, eye_position);"                                                          \

        "       for(int i = 0; i < 2; i++)"                                                                             \
        "       {"                                                                                                      \
        "           output.light_direction[i] = (float3)lightPosition[i] - eye_position.xyz;"                           \
        "       }"                                                                                                      \

        "       output.transformed_normal = (float3)mul((float3x3)worldMatrix, (float3)normal);"                        \
        "       output.viewer_vector = -eye_position.xyz;"                                                              \
        "   }"                                                                                                          \

        "   float4 position = mul(worldMatrix, pos);"                                                                   \
        "   position = mul(viewMatrix, position);"                                                                      \
        "   position = mul(projectionMatrix, position);"                                                                \
        "   output.position = position;"                                                                                \
        "   return (output);"                                                                                           \
        "}";

    //buffers for object code and error 
    ID3DBlob* pID3DBlob_VertexShaderCode = NULL;
    ID3DBlob* pID3DBlob_Error = NULL;

    //compile vertex shader
    hr = D3DCompile(
        vertexShaderSourceCode,
        lstrlenA(vertexShaderSourceCode) + 1,
        "VS",
        NULL,
        D3D_COMPILE_STANDARD_FILE_INCLUDE,
        "main",
        "vs_5_0",
        0,
        0,
        &pID3DBlob_VertexShaderCode,
        &pID3DBlob_Error
    );
    if (FAILED(hr))
    {
        if (pID3DBlob_Error != NULL)
        {
            fopen_s(&gpFile, gLogFileName, "a+");
            fprintf(gpFile, "Error : vertex shader compilation failed : %s.", (char*)pID3DBlob_Error->GetBufferPointer());
            fprintf(gpFile, "--------------------------------------------------------------------------\n");
            fclose(gpFile);
            pID3DBlob_Error->Release();
            pID3DBlob_Error = NULL;
            return (hr);
        }
        else
        {
            fopen_s(&gpFile, gLogFileName, "a+");
            fprintf(gpFile, "Error : D3DCompile() failed (COM error).\n");
            fclose(gpFile);
            return (hr);
        }
    }
    else
    {
        fopen_s(&gpFile, gLogFileName, "a+");
        fprintf(gpFile, "-> vertex shader compiled successfully.\n");
        fprintf(gpFile, "--------------------------------------------------------------------------\n");
        fclose(gpFile);
    }

    //create vertex shader object 
    hr = gpID3D11Device->CreateVertexShader(
        pID3DBlob_VertexShaderCode->GetBufferPointer(),
        pID3DBlob_VertexShaderCode->GetBufferSize(),
        NULL,
        &gpID3D11VertexShader
    );
    if (FAILED(hr))
    {
        fopen_s(&gpFile, gLogFileName, "a+");
        fprintf(gpFile, "Error : ID3D11Device::CreateVertexShader() failed.\n");
        fclose(gpFile);
        return (hr);
    }

    //set vertex shader into pipeline
    gpID3D11DeviceContext->VSSetShader(gpID3D11VertexShader, NULL, 0);

    //pixel shader 
    const char* pixelShaderSourceCode =
        "cbuffer ConstantBuffer"                                                                                                            \
        "{"                                                                                                                                 \
        "   float4x4 worldMatrix;"                                                                                                          \
        "   float4x4 viewMatrix;"                                                                                                           \
        "   float4x4 projectionMatrix;"                                                                                                     \

        "   float4 la[2];"                                                                                                                  \
        "   float4 ld[2];"                                                                                                                  \
        "   float4 ls[2];"                                                                                                                  \
        "   float4 lightPosition[2];"                                                                                                       \

        "   float4 ka;"                                                                                                                     \
        "   float4 kd;"                                                                                                                     \
        "   float4 ks;"                                                                                                                     \
        "   float materialShininess;"                                                                                                       \

        "   uint keyPressed;"                                                                                                               \
        "}"                                                                                                                                 \

        "struct vertex_output"                                                                                                              \
        "{"                                                                                                                                 \
        "   float4 position           : SV_POSITION;"                                                                                                   \
        "   float3 transformed_normal : NORMAL0;"                                                                                             \
        "   float3 light_direction[2] : light_direction;"                                                                                             \
        "   float3 viewer_vector      : viewer_vector;"                                                                                                  \
        "};"                                                                                                                                \

        "float4 main(vertex_output input) : SV_TARGET"                                                                                      \
        "{"                                                                                                                                 \
        "   float3 phong_ads_light = float3(0.0f, 0.0f, 0.0f);"                                                                             \
        "   if(keyPressed == 1)"                                                                                                            \
        "   {"                                                                                                                              \
        "       float3 normalized_transformed_normal = normalize(input.transformed_normal);"                                                \
        "       float3 normalized_viewer_vector = normalize(input.viewer_vector);"                                                          \

        "       for(int i = 0; i < 2; i++)"                                                                                                 \
        "       {"                                                                                                                          \
        "           float3 normalized_light_direction = normalize(input.light_direction[i]);"                                               \
        "           float3 reflection_vector = reflect(-normalized_light_direction, normalized_transformed_normal);"                        \

        "           float3 ambient = la[i] * ka;"                                                                                           \
        "           float3 diffuse = ld[i] * kd * max(dot(normalized_light_direction, normalized_transformed_normal), 0.0f);"               \
        "           float3 specular = ls[i] * ks * pow(max(dot(reflection_vector, normalized_viewer_vector), 0.0f), materialShininess);"    \

        "           phong_ads_light = phong_ads_light + ambient + diffuse + specular;"                                                      \
        "       }"
        "   }"                                                                                                                              \
        "   else"                                                                                                                           \
        "   {"                                                                                                                              \
        "       phong_ads_light = float3(1.0f, 1.0f, 1.0f);"                                                                                \
        "   }"                                                                                                                              \

        "   float4 color = float4(phong_ads_light, 1.0f);"                                                                                  \
        "   return (color);"                                                                                                                \
        "}";

    //buffers for object code and error 
    ID3DBlob* pID3DBlob_PixelShaderCode = NULL;
    pID3DBlob_Error = NULL;

    hr = D3DCompile(
        pixelShaderSourceCode,
        lstrlenA(pixelShaderSourceCode) + 1,
        "PS",
        NULL,
        D3D_COMPILE_STANDARD_FILE_INCLUDE,
        "main",
        "ps_5_0",
        0,
        0,
        &pID3DBlob_PixelShaderCode,
        &pID3DBlob_Error
    );
    if (FAILED(hr))
    {
        if (pID3DBlob_Error != NULL)
        {
            fopen_s(&gpFile, gLogFileName, "a+");
            fprintf(gpFile, "Error : pixel shader compilation failed : %s", (char*)pID3DBlob_Error->GetBufferPointer());
            fclose(gpFile);
            pID3DBlob_Error->Release();
            pID3DBlob_Error = NULL;
            return (hr);
        }
        else
        {
            fopen_s(&gpFile, gLogFileName, "a+");
            fprintf(gpFile, "Error : D3DCompile() failed (COM error).\n");
            fclose(gpFile);
            return (hr);
        }
    }
    else
    {
        fopen_s(&gpFile, gLogFileName, "a+");
        fprintf(gpFile, "-> pixel shader compiled successfully.\n");
        fclose(gpFile);
    }

    //create pixel shader object
    hr = gpID3D11Device->CreatePixelShader(
        pID3DBlob_PixelShaderCode->GetBufferPointer(),
        pID3DBlob_PixelShaderCode->GetBufferSize(),
        NULL,
        &gpID3D11PixelShader
    );
    if (FAILED(hr))
    {
        fopen_s(&gpFile, gLogFileName, "a+");
        fprintf(gpFile, "Error : ID3D11Device::CreatePixelShader() failed.\n");
        fclose(gpFile);
        return (hr);
    }

    //set pixel shader into pipeline
    gpID3D11DeviceContext->PSSetShader(gpID3D11PixelShader, NULL, 0);

    //release pixel shader buffer
    pID3DBlob_PixelShaderCode->Release();
    pID3DBlob_PixelShaderCode = NULL;

    //initialize input layout 
    D3D11_INPUT_ELEMENT_DESC d3d11InputElementDesc[2];

    d3d11InputElementDesc[0].SemanticName = "POSITION";
    d3d11InputElementDesc[0].SemanticIndex = 0;
    d3d11InputElementDesc[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
    d3d11InputElementDesc[0].AlignedByteOffset = 0;
    d3d11InputElementDesc[0].InputSlot = 0;
    d3d11InputElementDesc[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
    d3d11InputElementDesc[0].InstanceDataStepRate = 0;

    d3d11InputElementDesc[1].SemanticName = "NORMAL";
    d3d11InputElementDesc[1].SemanticIndex = 0;
    d3d11InputElementDesc[1].Format = DXGI_FORMAT_R32G32B32_FLOAT;
    d3d11InputElementDesc[1].AlignedByteOffset = 0;
    d3d11InputElementDesc[1].InputSlot = 1;
    d3d11InputElementDesc[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
    d3d11InputElementDesc[1].InstanceDataStepRate = 0;

    hr = gpID3D11Device->CreateInputLayout(
        d3d11InputElementDesc,
        _ARRAYSIZE(d3d11InputElementDesc),
        pID3DBlob_VertexShaderCode->GetBufferPointer(),
        pID3DBlob_VertexShaderCode->GetBufferSize(),
        &gpID3D11InputLayout
    );
    if (FAILED(hr))
    {
        fopen_s(&gpFile, gLogFileName, "a+");
        fprintf(gpFile, "Error : ID3D11Device::CreateInputLayout() failed.\n");
        fclose(gpFile);
        pID3DBlob_VertexShaderCode->Release();
        pID3DBlob_VertexShaderCode = NULL;
        return (hr);
    }

    //set Input Layout in pipeline
    gpID3D11DeviceContext->IASetInputLayout(gpID3D11InputLayout);

    //release vertex shader buffer
    pID3DBlob_VertexShaderCode->Release();
    pID3DBlob_VertexShaderCode = NULL;

    //create constant buffer 
    D3D11_BUFFER_DESC d3d11BufferDesc;
    ZeroMemory((void*)&d3d11BufferDesc, sizeof(D3D11_BUFFER_DESC));
    d3d11BufferDesc.ByteWidth = sizeof(CBUFFER);
    d3d11BufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    d3d11BufferDesc.Usage = D3D11_USAGE_DEFAULT;
    hr = gpID3D11Device->CreateBuffer(&d3d11BufferDesc, NULL, &gpID3D11Buffer_ConstantBuffer);
    if (FAILED(hr))
    {
        fopen_s(&gpFile, gLogFileName, "a+");
        fprintf(gpFile, "Error : ID3D11Device::CreateBuffer() failed for constant buffer.\n");
        fclose(gpFile);
        return (hr);
    }

    //set constant buffer into pipeline
    gpID3D11DeviceContext->VSSetConstantBuffers(0, 1, &gpID3D11Buffer_ConstantBuffer);
    gpID3D11DeviceContext->PSSetConstantBuffers(0, 1, &gpID3D11Buffer_ConstantBuffer);

    //setup vertices, normals and texcoords
    const float pyramid_vertices[] =
    {
        //front
        +0.0f, +1.0f, +0.0f,
        +1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,

        //right
        +0.0f, +1.0f, +0.0f,
        +1.0f, -1.0f, +1.0f,
        +1.0f, -1.0f, -1.0f,

        //back
        +0.0f, +1.0f, +0.0f,
        -1.0f, -1.0f, +1.0f,
        +1.0f, -1.0f, +1.0f,

        //left
        +0.0f, +1.0f, +0.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, +1.0f,
    };

    const float pyramid_normals[] =
    {
        //front
        +0.0f, +0.447214f, -0.894427f,
        +0.0f, +0.447214f, -0.894427f,
        +0.0f, +0.447214f, -0.894427f,

        //right
        +0.894427f, +0.447214f, +0.0f,
        +0.894427f, +0.447214f, +0.0f,
        +0.894427f, +0.447214f, +0.0f,

        //back
        +0.0f, +0.447214f, +0.894427f,
        +0.0f, +0.447214f, +0.894427f,
        +0.0f, +0.447214f, +0.894427f,

        //left
        -0.894427f, +0.447214f, +0.0f,
        -0.894427f, +0.447214f, +0.0f,
        -0.894427f, +0.447214f, +0.0f,
    };

    //create vertex buffer for position
    ZeroMemory((void*)&d3d11BufferDesc, sizeof(D3D11_BUFFER_DESC));
    d3d11BufferDesc.ByteWidth = sizeof(float) * _ARRAYSIZE(pyramid_vertices);
    d3d11BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    d3d11BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    d3d11BufferDesc.Usage = D3D11_USAGE_DYNAMIC;

    hr = gpID3D11Device->CreateBuffer(&d3d11BufferDesc, NULL, &gpID3D11Buffer_VertexBuffer_position);
    if (FAILED(hr))
    {
        fopen_s(&gpFile, gLogFileName, "a+");
        fprintf(gpFile, "Error : ID3D11Device::CreateBuffer() failed for position buffer.\n");
        fclose(gpFile);
        return (hr);
    }

    D3D11_MAPPED_SUBRESOURCE d3d11MappedSubresource;
    ZeroMemory((void*)&d3d11MappedSubresource, sizeof(D3D11_MAPPED_SUBRESOURCE));
    gpID3D11DeviceContext->Map(gpID3D11Buffer_VertexBuffer_position, 0, D3D11_MAP_WRITE_DISCARD, 0, &d3d11MappedSubresource);
    memcpy(d3d11MappedSubresource.pData, pyramid_vertices, sizeof(pyramid_vertices));
    gpID3D11DeviceContext->Unmap(gpID3D11Buffer_VertexBuffer_position, 0);

    //create vertex buffer for normals
    ZeroMemory((void*)&d3d11BufferDesc, sizeof(D3D11_BUFFER_DESC));
    d3d11BufferDesc.ByteWidth = sizeof(float) * _ARRAYSIZE(pyramid_normals);
    d3d11BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    d3d11BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    d3d11BufferDesc.Usage = D3D11_USAGE_DYNAMIC;

    hr = gpID3D11Device->CreateBuffer(&d3d11BufferDesc, NULL, &gpID3D11Buffer_VertexBuffer_normal);
    if (FAILED(hr))
    {
        fopen_s(&gpFile, gLogFileName, "a+");
        fprintf(gpFile, "Error : ID3D11Device::CreateBuffer() failed for color buffer.\n");
        fclose(gpFile);
        return (hr);
    }

    ZeroMemory((void*)&d3d11MappedSubresource, sizeof(D3D11_MAPPED_SUBRESOURCE));
    gpID3D11DeviceContext->Map(gpID3D11Buffer_VertexBuffer_normal, 0, D3D11_MAP_WRITE_DISCARD, 0, &d3d11MappedSubresource);
    memcpy(d3d11MappedSubresource.pData, pyramid_normals, sizeof(pyramid_normals));
    gpID3D11DeviceContext->Unmap(gpID3D11Buffer_VertexBuffer_normal, 0);

    //create rasterizer state
    D3D11_RASTERIZER_DESC d3d11RasterizerDesc;
    ZeroMemory((void*)&d3d11RasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));
    d3d11RasterizerDesc.AntialiasedLineEnable = FALSE;
    d3d11RasterizerDesc.CullMode = D3D11_CULL_NONE;
    d3d11RasterizerDesc.DepthBias = 0;
    d3d11RasterizerDesc.DepthBiasClamp = 0.0f;
    d3d11RasterizerDesc.DepthClipEnable = FALSE;
    d3d11RasterizerDesc.FillMode = D3D11_FILL_SOLID;
    d3d11RasterizerDesc.FrontCounterClockwise = FALSE;
    d3d11RasterizerDesc.MultisampleEnable = FALSE;
    d3d11RasterizerDesc.ScissorEnable = FALSE;
    d3d11RasterizerDesc.SlopeScaledDepthBias = FALSE;

    hr = gpID3D11Device->CreateRasterizerState(&d3d11RasterizerDesc, &gpID3D11RasterizerState);
    if (FAILED(hr))
    {
        fopen_s(&gpFile, gLogFileName, "a+");
        fprintf(gpFile, "Error : ID3D11Device::CreateRasterizerState() failed to create rasterizer state.\n");
        fclose(gpFile);
        return (hr);
    }

    //set rasterizer state
    gpID3D11DeviceContext->RSSetState(gpID3D11RasterizerState);

    //set clear color
    gClearColor[0] = 0.0f;
    gClearColor[1] = 0.0f;
    gClearColor[2] = 0.0f;
    gClearColor[3] = 1.0f;

    //set projection matrix to identity
    perspectiveProjectionMatrix = XMMatrixIdentity();

    //set up light 0 
    light[0].ambient[0] = 0.0f;
    light[0].ambient[1] = 0.0f;
    light[0].ambient[2] = 0.0f;
    light[0].ambient[3] = 1.0f;

    light[0].diffuse[0] = 1.0f;
    light[0].diffuse[1] = 0.0f;
    light[0].diffuse[2] = 0.0f;
    light[0].diffuse[3] = 1.0f;

    light[0].specular[0] = 1.0f;
    light[0].specular[1] = 0.0f;
    light[0].specular[2] = 0.0f;
    light[0].specular[3] = 1.0f;

    light[0].position[0] = 5.0f;
    light[0].position[1] = 0.0f;
    light[0].position[2] = 0.0f;
    light[0].position[3] = 1.0f;

    //set up light 1
    light[1].ambient[0] = 0.0f;
    light[1].ambient[1] = 0.0f;
    light[1].ambient[2] = 0.0f;
    light[1].ambient[3] = 1.0f;

    light[1].diffuse[0] = 0.0f;
    light[1].diffuse[1] = 0.0f;
    light[1].diffuse[2] = 1.0f;
    light[1].diffuse[3] = 1.0f;

    light[1].specular[0] = 0.0f;
    light[1].specular[1] = 0.0f;
    light[1].specular[2] = 1.0f;
    light[1].specular[3] = 1.0f;

    light[1].position[0] = -5.0f;
    light[1].position[1] = 0.0f;
    light[1].position[2] = 0.0f;
    light[1].position[3] = 1.0f;

    //set up material 
    material.ambient[0] = 0.0f;
    material.ambient[1] = 0.0f;
    material.ambient[2] = 0.0f;
    material.ambient[3] = 1.0f;

    material.diffuse[0] = 1.0f;
    material.diffuse[1] = 1.0f;
    material.diffuse[2] = 1.0f;
    material.diffuse[3] = 1.0f;

    material.specular[0] = 1.0f;
    material.specular[1] = 1.0f;
    material.specular[2] = 1.0f;
    material.specular[3] = 1.0f;

    material.shininess = 50.0f;

    //warm-up call
    hr = Resize(WIN_WIDTH, WIN_HEIGHT);
    if (FAILED(hr))
    {
        fopen_s(&gpFile, gLogFileName, "a+");
        fprintf(gpFile, "Error : Resize() failed.\n");
        fclose(gpFile);
        return (hr);
    }

    return (hr);
}

HRESULT printD3DInfo(void)
{
    //variable declarations
    IDXGIFactory* pIDXGIFactory = NULL;
    IDXGIAdapter* pIDXGIAdapter = NULL;
    DXGI_ADAPTER_DESC dxgiAdapterDesc;
    HRESULT hr;
    char str[255];

    //code 
    //initialize IDXGIFactory
    hr = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&pIDXGIFactory);
    if (FAILED(hr))
    {
        fopen_s(&gpFile, gLogFileName, "a+");
        fprintf(gpFile, "Error : CreateDXGIFactory() failed.\n");
        fclose(gpFile);
    }
    else
    {
        //Get IDXGIAdapter (Device Interface) from IDXGIFactory
        if (pIDXGIFactory->EnumAdapters(0, &pIDXGIAdapter) == DXGI_ERROR_NOT_FOUND)
        {
            fopen_s(&gpFile, gLogFileName, "a+");
            fprintf(gpFile, "Error : IDXGIFactory::EnumAdapters() failed.\n");
            fclose(gpFile);
        }
        else
        {
            //zero out DXGI_ADAPTER_DESC structure
            ZeroMemory((void*)&dxgiAdapterDesc, sizeof(DXGI_ADAPTER_DESC));

            //get DXGI_ADAPTER_DESC from IDXGIAdapter (Device Interface)
            hr = pIDXGIAdapter->GetDesc(&dxgiAdapterDesc);
            if (FAILED(hr))
            {
                fopen_s(&gpFile, gLogFileName, "a+");
                fprintf(gpFile, "Error : IDXGIAdapter::GetDesc() failed.\n");
                fclose(gpFile);
            }
            else
            {
                //log the device info from DXGI_ADAPTER_DESC
                WideCharToMultiByte(CP_ACP, 0, dxgiAdapterDesc.Description, 255, str, 255, NULL, NULL);

                fopen_s(&gpFile, gLogFileName, "a+");
                fprintf(gpFile, "--------------------------------------------------------------------------\n");
                fprintf(gpFile, "-> Device Info\n");
                fprintf(gpFile, "Name           : %s\n", str);
                fprintf(gpFile, "VRAM           : %d MB\n", (int)ceil(dxgiAdapterDesc.DedicatedVideoMemory / 1024.0 / 1024.0));
                fprintf(gpFile, "--------------------------------------------------------------------------\n");
                fclose(gpFile);
            }
        }
    }

    //cleanup
    if (pIDXGIAdapter)
    {
        pIDXGIAdapter->Release();
        pIDXGIAdapter = NULL;
    }

    if (pIDXGIFactory)
    {
        pIDXGIFactory->Release();
        pIDXGIFactory = NULL;
    }

    return (hr);
}

HRESULT Resize(int width, int height)
{
    //variable declarations
    ID3D11Texture2D* pID3D11Texture2D_BackBuffer = NULL;
    ID3D11Texture2D* pID3D11Texture2D_DepthBuffer = NULL;
    D3D11_VIEWPORT d3dViewport;
    HRESULT hr = S_OK;

    //code
    if (height < 0)
        height = 0;

    //free size dependant resources
    if (gpID3D11RenderTargetView)
    {
        gpID3D11RenderTargetView->Release();
        gpID3D11RenderTargetView = NULL;
    }

    if (gpID3D11DepthStencilView)
    {
        gpID3D11DepthStencilView->Release();
        gpID3D11DepthStencilView = NULL;
    }

    //resize swap chain buffers
    hr = gpIDXGISwapChain->ResizeBuffers(1, width, height, DXGI_FORMAT_R8G8B8A8_UNORM, 0);
    if (FAILED(hr))
    {
        fopen_s(&gpFile, gLogFileName, "a+");
        fprintf(gpFile, "Error : IDXGISwapChain::ResizeBuffers() failed.\n");
        fclose(gpFile);
        return (hr);
    }

    //get back buffer from swap chain
    hr = gpIDXGISwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pID3D11Texture2D_BackBuffer);
    if (FAILED(hr))
    {
        fopen_s(&gpFile, gLogFileName, "a+");
        fprintf(gpFile, "Error : IDXGISwapChain::GetBuffer() failed.\n");
        fclose(gpFile);
        return (hr);
    }

    //create render target view for resized buffer
    hr = gpID3D11Device->CreateRenderTargetView(pID3D11Texture2D_BackBuffer, NULL, &gpID3D11RenderTargetView);
    if (FAILED(hr))
    {
        fopen_s(&gpFile, gLogFileName, "a+");
        fprintf(gpFile, "Error : ID3D11Device::CreateRenderTargetView() failed.\n");
        fclose(gpFile);
        return (hr);
    }

    pID3D11Texture2D_BackBuffer->Release();
    pID3D11Texture2D_BackBuffer = NULL;

    //setup depth and stencil buffers
    D3D11_TEXTURE2D_DESC d3d11Texture2DDesc;
    ZeroMemory((void*)&d3d11Texture2DDesc, sizeof(D3D11_TEXTURE2D_DESC));
    d3d11Texture2DDesc.Width = (UINT)width;
    d3d11Texture2DDesc.Height = (UINT)height;
    d3d11Texture2DDesc.Format = DXGI_FORMAT_D32_FLOAT;
    d3d11Texture2DDesc.Usage = D3D11_USAGE_DEFAULT;
    d3d11Texture2DDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    d3d11Texture2DDesc.SampleDesc.Count = 1;
    d3d11Texture2DDesc.SampleDesc.Quality = 0;
    d3d11Texture2DDesc.ArraySize = 1;
    d3d11Texture2DDesc.MipLevels = 1;
    d3d11Texture2DDesc.CPUAccessFlags = 0;
    d3d11Texture2DDesc.MiscFlags = 0;

    hr = gpID3D11Device->CreateTexture2D(&d3d11Texture2DDesc, NULL, &pID3D11Texture2D_DepthBuffer);
    if (FAILED(hr))
    {
        fopen_s(&gpFile, gLogFileName, "a+");
        fprintf(gpFile, "Error : ID3D11Device::CreateTexture2D() failed for depth buffer.\n");
        fclose(gpFile);
        return (hr);
    }

    D3D11_DEPTH_STENCIL_VIEW_DESC d3d11DepthStencilViewDesc;
    ZeroMemory((void*)&d3d11DepthStencilViewDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
    d3d11DepthStencilViewDesc.Format = DXGI_FORMAT_D32_FLOAT;
    d3d11DepthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;                  //multi sample

    hr = gpID3D11Device->CreateDepthStencilView(pID3D11Texture2D_DepthBuffer, &d3d11DepthStencilViewDesc, &gpID3D11DepthStencilView);
    if (FAILED(hr))
    {
        fopen_s(&gpFile, gLogFileName, "a+");
        fprintf(gpFile, "Error : ID3D11Device::CreateDepthStencilView() failed.\n");
        fclose(gpFile);
        return (hr);
    }

    pID3D11Texture2D_DepthBuffer->Release();
    pID3D11Texture2D_DepthBuffer = NULL;

    //set render target view as output merger render target
    gpID3D11DeviceContext->OMSetRenderTargets(1, &gpID3D11RenderTargetView, gpID3D11DepthStencilView);

    //set viewport
    d3dViewport.TopLeftX = 0.0f;
    d3dViewport.TopLeftY = 0.0f;
    d3dViewport.Width = (float)width;
    d3dViewport.Height = (float)height;
    d3dViewport.MinDepth = 0.0f;
    d3dViewport.MaxDepth = 1.0f;

    //set viewport for rasterization stage  
    gpID3D11DeviceContext->RSSetViewports(1, &d3dViewport);

    //set projection matrix
    perspectiveProjectionMatrix = XMMatrixPerspectiveFovLH(XMConvertToRadians(45.0f), (float)width / (float)height, 0.1f, 100.0f);

    return (hr);
}

void Display(void)
{
    //variable declarations
    CBUFFER constantBuffer;
    XMMATRIX worldMatrix;
    XMMATRIX viewMatrix;
    UINT stride;
    UINT offset;

    static float pyramid_rotation_angle = 0.0f;

    //code
    gpID3D11DeviceContext->ClearRenderTargetView(gpID3D11RenderTargetView, gClearColor);
    gpID3D11DeviceContext->ClearDepthStencilView(gpID3D11DepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

    viewMatrix = XMMatrixIdentity();
    worldMatrix = XMMatrixRotationY(-pyramid_rotation_angle);
    worldMatrix = worldMatrix * XMMatrixTranslation(0.0f, 0.0f, 5.0f);

    ZeroMemory((void*)&constantBuffer, sizeof(CBUFFER));
    constantBuffer.WorldMatrix = worldMatrix;
    constantBuffer.ViewMatrix = viewMatrix;
    constantBuffer.ProjectionMatrix = perspectiveProjectionMatrix;

    if (gbLight == true)
    {
        constantBuffer.KeyPressed = 1;

        for (int i = 0; i < 2; i++)
        {
            constantBuffer.La[i] = XMVectorSet(light[i].ambient[0], light[i].ambient[1], light[i].ambient[2], light[i].ambient[3]);
            constantBuffer.Ld[i] = XMVectorSet(light[i].diffuse[0], light[i].diffuse[1], light[i].diffuse[2], light[i].diffuse[3]);
            constantBuffer.Ls[i] = XMVectorSet(light[i].specular[0], light[i].specular[1], light[i].specular[2], light[i].specular[3]);
            constantBuffer.LightPosition[i] = XMVectorSet(light[i].position[0], light[i].position[1], light[i].position[2], light[i].position[3]);
        }

        constantBuffer.Ka = XMVectorSet(material.ambient[0], material.ambient[1], material.ambient[2], material.ambient[3]);
        constantBuffer.Kd = XMVectorSet(material.diffuse[0], material.diffuse[1], material.diffuse[2], material.diffuse[3]);
        constantBuffer.Ks = XMVectorSet(material.specular[0], material.specular[1], material.specular[2], material.specular[3]);
        constantBuffer.MaterialShininess = material.shininess;
    }
    else
    {
        constantBuffer.KeyPressed = 0;
    }

    gpID3D11DeviceContext->UpdateSubresource(gpID3D11Buffer_ConstantBuffer, 0, NULL, &constantBuffer, 0, 0);

    stride = sizeof(float) * 3;
    offset = 0;
    gpID3D11DeviceContext->IASetVertexBuffers(0, 1, &gpID3D11Buffer_VertexBuffer_position, &stride, &offset);
    gpID3D11DeviceContext->IASetVertexBuffers(1, 1, &gpID3D11Buffer_VertexBuffer_normal, &stride, &offset);

    gpID3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    gpID3D11DeviceContext->Draw(12, 0);

    //update 
    if (bAnimate)
    {
        pyramid_rotation_angle += 0.0001f;
        if (pyramid_rotation_angle >= 360.0f)
            pyramid_rotation_angle = 0.0f;
    }

    gpIDXGISwapChain->Present(0, 0);
}

void Cleanup(void)
{
    //code
    //if window is in fullscreen mode toggle
    if (gbFullscreen == true)
    {
        ToggleFullscreen();
    }

    //release D3D11 and DXGI resources
    if (gpID3D11RasterizerState)
    {
        gpID3D11RasterizerState->Release();
        gpID3D11RasterizerState = NULL;
    }

    if (gpID3D11Buffer_ConstantBuffer)
    {
        gpID3D11Buffer_ConstantBuffer->Release();
        gpID3D11Buffer_ConstantBuffer = NULL;
    }

    if (gpID3D11Buffer_VertexBuffer_normal)
    {
        gpID3D11Buffer_VertexBuffer_normal->Release();
        gpID3D11Buffer_VertexBuffer_normal = NULL;
    }

    if (gpID3D11Buffer_VertexBuffer_position)
    {
        gpID3D11Buffer_VertexBuffer_position->Release();
        gpID3D11Buffer_VertexBuffer_position = NULL;
    }

    if (gpID3D11InputLayout)
    {
        gpID3D11InputLayout->Release();
        gpID3D11InputLayout = NULL;
    }

    if (gpID3D11PixelShader)
    {
        gpID3D11PixelShader->Release();
        gpID3D11PixelShader = NULL;
    }

    if (gpID3D11VertexShader)
    {
        gpID3D11VertexShader->Release();
        gpID3D11VertexShader = NULL;
    }

    if (gpID3D11DepthStencilView)
    {
        gpID3D11DepthStencilView->Release();
        gpID3D11DepthStencilView = NULL;
    }

    if (gpID3D11RenderTargetView)
    {
        gpID3D11RenderTargetView->Release();
        gpID3D11RenderTargetView = NULL;
    }

    if (gpIDXGISwapChain)
    {
        gpIDXGISwapChain->Release();
        gpIDXGISwapChain = NULL;
    }

    if (gpID3D11DeviceContext)
    {
        gpID3D11DeviceContext->Release();
        gpID3D11DeviceContext = NULL;
    }

    if (gpID3D11Device)
    {
        gpID3D11Device->Release();
        gpID3D11Device = NULL;
    }

    //close the log file
    if (gpFile)
    {
        fopen_s(&gpFile, gLogFileName, "a+");
        fprintf(gpFile, "--------------------------------------------------------------------------\n");
        fprintf(gpFile, "-> Program completed successfully.\n");
        fprintf(gpFile, "--------------------------------------------------------------------------\n");
        fclose(gpFile);
        gpFile = NULL;
    }
}