//headers
#include <windows.h>               //standard windows header
#include <stdio.h>                 //standard C io header 
#include <math.h>                  //standard C math header 
#include <d3d11.h>                 //Direct3D 11 header
#include <d3dcompiler.h>           //Direct3D shader compilation
#include "D3D.h"             //Resources header
#include "Sphere.h"                //Sphere header

#pragma warning(disable: 4838)     //disable header warnings
#include "xnamath.h"       //Direct3D math header

//import libraries
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "kernel32.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "Sphere.lib")

//symbolic constants
#define WIN_WIDTH  800             //initial width of window  
#define WIN_HEIGHT 600             //initial height of window

#define VK_F       0x46            //virtual key code of F key
#define VK_f       0x60            //virtual key code of f key

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
ID3D11HullShader* gpID3D11HullShader = NULL;                    //tessellation control shader
ID3D11DomainShader* gpID3D11DomainShader = NULL;                //tessellation evaluation shader
ID3D11PixelShader* gpID3D11PixelShader = NULL;
ID3D11InputLayout* gpID3D11InputLayout = NULL;

ID3D11Buffer* gpID3D11Buffer_ConstantBuffer_HullShader = NULL;
ID3D11Buffer* gpID3D11Buffer_ConstantBuffer_DomainShader = NULL;
ID3D11Buffer* gpID3D11Buffer_ConstantBuffer_PixelShader = NULL;

ID3D11Buffer* gpID3D11Buffer_VertexBuffer = NULL;

//constant buffer in DirectX must have size >= 16 byte or multiples of 16
struct CBUFFER_HULL_SHADER
{
    XMVECTOR HullConstantFunctionParam;
};

struct CBUFFER_PIXEL_SHADER
{
    XMVECTOR LineColor;
};

struct CBUFFER_DOMAIN_SHADER
{
    XMMATRIX WorldViewProjectionMatrix;
};

XMMATRIX perspectiveProjectionMatrix;
unsigned int gNumberOfLineSegements = 1;

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
        TEXT("D3D11"),    //window caption
        WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN |       //window style
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

        case VK_UP:
            gNumberOfLineSegements++;
            if (gNumberOfLineSegements >= 30)
                gNumberOfLineSegements = 30;
            break;

        case VK_DOWN:
            gNumberOfLineSegements--;
            if (gNumberOfLineSegements <= 0)
                gNumberOfLineSegements = 1;
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

    //vertex shader
    const char* vertexShaderSourceCode =
        "struct vertex_output"                                          \
        "{"                                                             \
        "   float4 position : POSITION0;"                               \
        "};"                                                            \

        "vertex_output main(float2 pos : POSITION)"                     \
        "{"                                                             \
        "   vertex_output output;"                                      \
        "   output.position = float4(pos, 0.0f, 1.0f);"                 \
        "   return (output);"                                           \
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
        fprintf(gpFile, "--------------------------------------------------------------------------\n");
        fclose(gpFile);
        return (hr);
    }

    //set vertex shader into pipeline
    gpID3D11DeviceContext->VSSetShader(gpID3D11VertexShader, NULL, 0);

    //hull shader 
    const char* hullShaderSourceCode =
        "cbuffer ConstantBuffer"                                                                            \
        "{"                                                                                                 \
        "   float4 hull_constant_function_param;"                                                           \
        "}"                                                                                                 \

        "struct vertex_output"                                                                              \
        "{"                                                                                                 \
        "   float4 position : POSITION0;"                                                                   \
        "};"                                                                                                \

        "struct hull_constant_output"                                                                       \
        "{"                                                                                                 \
        "   float edges[2] : SV_TessFactor;"                                                                \
        "};"                                                                                                \

        "hull_constant_output hull_constant_function(void)"                                                 \
        "{"                                                                                                 \
        "   hull_constant_output output;"                                                                   \
        "   float numberOfLinesStrips = hull_constant_function_param[0];"                                   \
        "   float numberOfLinesSegments = hull_constant_function_param[1];"                                 \

        "   output.edges[0] = numberOfLinesStrips;"                                                         \
        "   output.edges[1] = numberOfLinesSegments;"                                                       \

        "   return (output);"                                                                               \
        "}"                                                                                                 \

        "struct hull_output"                                                                                \
        "{"                                                                                                 \
        "   float4 position : POSITION1;"                                                                   \
        "};"                                                                                                \

        "[domain (\"isoline\")]"                                                                            \
        "[partitioning (\"integer\")]"                                                                      \
        "[outputtopology (\"line\")]"                                                                       \
        "[outputcontrolpoints (4)]"                                                                         \
        "[patchconstantfunc (\"hull_constant_function\")]"                                                  \

        "hull_output main(InputPatch<vertex_output, 4> input_patch, uint i:SV_OutputControlPointID)"        \
        "{"                                                                                                 \
        "   hull_output output;"                                                                            \
        "   output.position = input_patch[i].position;"                                                     \
        "   return (output);"                                                                               \
        "}";

    //buffers for object code and error 
    ID3DBlob* pID3DBlob_hullShaderCode = NULL;
    pID3DBlob_Error = NULL;

    hr = D3DCompile(
        hullShaderSourceCode,
        lstrlenA(hullShaderSourceCode) + 1,
        "HS",
        NULL,
        D3D_COMPILE_STANDARD_FILE_INCLUDE,
        "main",
        "hs_5_0",
        0,
        0,
        &pID3DBlob_hullShaderCode,
        &pID3DBlob_Error
    );
    if (FAILED(hr))
    {
        if (pID3DBlob_Error != NULL)
        {
            fopen_s(&gpFile, gLogFileName, "a+");
            fprintf(gpFile, "Error : hull shader compilation failed : %s", (char*)pID3DBlob_Error->GetBufferPointer());
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
        fprintf(gpFile, "-> hull shader compiled successfully.\n");
        fprintf(gpFile, "--------------------------------------------------------------------------\n");
        fclose(gpFile);
    }

    //create hull shader object
    hr = gpID3D11Device->CreateHullShader(
        pID3DBlob_hullShaderCode->GetBufferPointer(),
        pID3DBlob_hullShaderCode->GetBufferSize(),
        NULL,
        &gpID3D11HullShader
    );
    if (FAILED(hr))
    {
        fopen_s(&gpFile, gLogFileName, "a+");
        fprintf(gpFile, "Error : ID3D11Device::CreateHullShader() failed.\n");
        fprintf(gpFile, "--------------------------------------------------------------------------\n");
        fclose(gpFile);
        return (hr);
    }

    //set hull shader into pipeline
    gpID3D11DeviceContext->HSSetShader(gpID3D11HullShader, NULL, 0);

    //release hull shader buffer
    pID3DBlob_hullShaderCode->Release();
    pID3DBlob_hullShaderCode = NULL;

    //domain shader 
    const char* domainShaderSourceCode =
        "cbuffer ConstantBuffer"                                                                                                                \
        "{"                                                                                                                                     \
        "   float4x4 worldViewProjectionMatrix;"                                                                                                \
        "}"                                                                                                                                     \

        "struct hull_output"                                                                                                                    \
        "{"                                                                                                                                     \
        "   float4 position : POSITION1;"                                                                                                       \
        "};"                                                                                                                                    \

        "struct hull_constant_output"                                                                                                           \
        "{"                                                                                                                                     \
        "   float edges[2] : SV_TessFactor;"                                                                                                    \
        "};"                                                                                                                                    \

        "struct domain_output"                                                                                                                  \
        "{"                                                                                                                                     \
        "   float4 position : SV_POSITION;"                                                                                                     \
        "};"                                                                                                                                    \

        "[domain(\"isoline\")]"                                                                                                                 \

        "domain_output main(hull_constant_output input, OutputPatch<hull_output, 4> output_patch, float2 tess_coord:SV_DomainLocation)"         \
        "{"                                                                                                                                     \
        "   domain_output output;"                                                                                                              \
        "   float3 p0 = output_patch[0].position.xyz;"                                                                                          \
        "   float3 p1 = output_patch[1].position.xyz;"                                                                                          \
        "   float3 p2 = output_patch[2].position.xyz;"                                                                                          \
        "   float3 p3 = output_patch[3].position.xyz;"                                                                                          \

        "   float3 p = p0 * (1.0f - tess_coord.x) * (1.0f - tess_coord.x) * (1.0f - tess_coord.x) +"                                            \
        "              p1 * 3.0f * tess_coord.x * (1.0f - tess_coord.x) * (1.0f - tess_coord.x) +"                                              \
        "              p2 * 3.0f * tess_coord.x * tess_coord.x * (1.0f - tess_coord.x) +"                                                       \
        "              p3 * tess_coord.x * tess_coord.x * tess_coord.x;"                                                                        \

        "   float4 position = float4(p, 1.0f);"                                                                                                 \
        "   output.position = mul(worldViewProjectionMatrix, position);"                                                                        \
        "   return (output);"                                                                                                                   \
        "}";

    //buffers for object code and error 
    ID3DBlob* pID3DBlob_domainShaderCode = NULL;
    pID3DBlob_Error = NULL;

    hr = D3DCompile(
        domainShaderSourceCode,
        lstrlenA(domainShaderSourceCode) + 1,
        "DS",
        NULL,
        D3D_COMPILE_STANDARD_FILE_INCLUDE,
        "main",
        "ds_5_0",
        0,
        0,
        &pID3DBlob_domainShaderCode,
        &pID3DBlob_Error
    );
    if (FAILED(hr))
    {
        if (pID3DBlob_Error != NULL)
        {
            fopen_s(&gpFile, gLogFileName, "a+");
            fprintf(gpFile, "Error : domain shader compilation failed : %s", (char*)pID3DBlob_Error->GetBufferPointer());
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
        fprintf(gpFile, "-> domain shader compiled successfully.\n");
        fprintf(gpFile, "--------------------------------------------------------------------------\n");
        fclose(gpFile);
    }

    //create domain shader object
    hr = gpID3D11Device->CreateDomainShader(
        pID3DBlob_domainShaderCode->GetBufferPointer(),
        pID3DBlob_domainShaderCode->GetBufferSize(),
        NULL,
        &gpID3D11DomainShader
    );
    if (FAILED(hr))
    {
        fopen_s(&gpFile, gLogFileName, "a+");
        fprintf(gpFile, "Error : ID3D11Device::CreateDomainShader() failed.\n");
        fprintf(gpFile, "--------------------------------------------------------------------------\n");
        fclose(gpFile);
        return (hr);
    }

    //set domain shader into pipeline
    gpID3D11DeviceContext->DSSetShader(gpID3D11DomainShader, NULL, 0);

    //release domain shader buffer
    pID3DBlob_domainShaderCode->Release();
    pID3DBlob_domainShaderCode = NULL;

    //pixel shader 
    const char* pixelShaderSourceCode =
        "cbuffer ConstantBuffer"                        \
        "{"                                             \
        "   float4 lineColor;"                          \
        "}"                                             \

        "float4 main(void) : SV_TARGET"                 \
        "{"                                             \
        "   float4 pixel_color = lineColor;"            \
        "   return (pixel_color);"                      \
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
    D3D11_INPUT_ELEMENT_DESC d3d11InputElementDesc;
    ZeroMemory((void*)&d3d11InputElementDesc, sizeof(D3D11_INPUT_ELEMENT_DESC));
    d3d11InputElementDesc.SemanticName = "POSITION";
    d3d11InputElementDesc.SemanticIndex = 0;
    d3d11InputElementDesc.Format = DXGI_FORMAT_R32G32_FLOAT;
    d3d11InputElementDesc.AlignedByteOffset = 0;
    d3d11InputElementDesc.InputSlot = 0;
    d3d11InputElementDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
    d3d11InputElementDesc.InstanceDataStepRate = 0;

    hr = gpID3D11Device->CreateInputLayout(
        &d3d11InputElementDesc,
        1,
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

    //setup vertices, normals and texcoords
    const float vertices[] =
    {
        -1.0f, -1.0f,
        -0.5f, 1.0f,
        0.5f, -1.0f,
        1.0f, 1.0f
    };

    //create vertex buffer
    D3D11_BUFFER_DESC d3d11BufferDesc;
    ZeroMemory((void*)&d3d11BufferDesc, sizeof(D3D11_BUFFER_DESC));
    d3d11BufferDesc.ByteWidth = sizeof(float) * _ARRAYSIZE(vertices);
    d3d11BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    d3d11BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    d3d11BufferDesc.Usage = D3D11_USAGE_DYNAMIC;

    hr = gpID3D11Device->CreateBuffer(&d3d11BufferDesc, NULL, &gpID3D11Buffer_VertexBuffer);
    if (FAILED(hr))
    {
        fopen_s(&gpFile, gLogFileName, "a+");
        fprintf(gpFile, "Error : ID3D11Device::CreateBuffer() failed for vertex buffer.\n");
        fclose(gpFile);
        return (hr);
    }

    //copy vertices into buffer
    D3D11_MAPPED_SUBRESOURCE d3d11MappedSubresource;
    ZeroMemory((void*)&d3d11MappedSubresource, sizeof(D3D11_MAPPED_SUBRESOURCE));
    gpID3D11DeviceContext->Map(gpID3D11Buffer_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &d3d11MappedSubresource);
    memcpy(d3d11MappedSubresource.pData, vertices, sizeof(vertices));
    gpID3D11DeviceContext->Unmap(gpID3D11Buffer_VertexBuffer, 0);

    //create constant buffer for hull shader
    ZeroMemory((void*)&d3d11BufferDesc, sizeof(D3D11_BUFFER_DESC));
    d3d11BufferDesc.ByteWidth = sizeof(CBUFFER_HULL_SHADER);
    d3d11BufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    d3d11BufferDesc.Usage = D3D11_USAGE_DEFAULT;
    hr = gpID3D11Device->CreateBuffer(&d3d11BufferDesc, NULL, &gpID3D11Buffer_ConstantBuffer_HullShader);
    if (FAILED(hr))
    {
        fopen_s(&gpFile, gLogFileName, "a+");
        fprintf(gpFile, "Error : ID3D11Device::CreateBuffer() failed for constant buffer of hull shader.\n");
        fclose(gpFile);
        return (hr);
    }

    //set constant buffer into pipeline
    gpID3D11DeviceContext->HSSetConstantBuffers(0, 1, &gpID3D11Buffer_ConstantBuffer_HullShader);

    //create constant buffer for domain shader
    ZeroMemory((void*)&d3d11BufferDesc, sizeof(D3D11_BUFFER_DESC));
    d3d11BufferDesc.ByteWidth = sizeof(CBUFFER_DOMAIN_SHADER);
    d3d11BufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    d3d11BufferDesc.Usage = D3D11_USAGE_DEFAULT;
    hr = gpID3D11Device->CreateBuffer(&d3d11BufferDesc, NULL, &gpID3D11Buffer_ConstantBuffer_DomainShader);
    if (FAILED(hr))
    {
        fopen_s(&gpFile, gLogFileName, "a+");
        fprintf(gpFile, "Error : ID3D11Device::CreateBuffer() failed for constant buffer of domain shader.\n");
        fclose(gpFile);
        return (hr);
    }

    //set constant buffer into pipeline
    gpID3D11DeviceContext->DSSetConstantBuffers(0, 1, &gpID3D11Buffer_ConstantBuffer_DomainShader);

    //create constant buffer for pixel shader
    ZeroMemory((void*)&d3d11BufferDesc, sizeof(D3D11_BUFFER_DESC));
    d3d11BufferDesc.ByteWidth = sizeof(CBUFFER_PIXEL_SHADER);
    d3d11BufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    d3d11BufferDesc.Usage = D3D11_USAGE_DEFAULT;
    hr = gpID3D11Device->CreateBuffer(&d3d11BufferDesc, NULL, &gpID3D11Buffer_ConstantBuffer_PixelShader);
    if (FAILED(hr))
    {
        fopen_s(&gpFile, gLogFileName, "a+");
        fprintf(gpFile, "Error : ID3D11Device::CreateBuffer() failed for constant buffer of pixel shader.\n");
        fclose(gpFile);
        return (hr);
    }

    //set constant buffer into pipeline
    gpID3D11DeviceContext->PSSetConstantBuffers(0, 1, &gpID3D11Buffer_ConstantBuffer_PixelShader);

    //create rasterizer state
    D3D11_RASTERIZER_DESC d3d11RasterizerDesc;
    ZeroMemory((void*)&d3d11RasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));
    d3d11RasterizerDesc.AntialiasedLineEnable = FALSE;
    d3d11RasterizerDesc.CullMode = D3D11_CULL_NONE;
    d3d11RasterizerDesc.DepthBias = 0;
    d3d11RasterizerDesc.DepthBiasClamp = 0.0f;
    d3d11RasterizerDesc.DepthClipEnable = TRUE;
    d3d11RasterizerDesc.FillMode = D3D11_FILL_SOLID;
    d3d11RasterizerDesc.FrontCounterClockwise = FALSE;
    d3d11RasterizerDesc.MultisampleEnable = FALSE;
    d3d11RasterizerDesc.ScissorEnable = FALSE;
    d3d11RasterizerDesc.SlopeScaledDepthBias = 0.0f;

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
    d3d11Texture2DDesc.SampleDesc.Count = 1;                                //1 to 4
    d3d11Texture2DDesc.SampleDesc.Quality = 0;                              //default
    d3d11Texture2DDesc.ArraySize = 1;                                       //default
    d3d11Texture2DDesc.MipLevels = 1;                                       //default
    d3d11Texture2DDesc.CPUAccessFlags = 0;                                  //default
    d3d11Texture2DDesc.MiscFlags = 0;                                       //default

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
    CBUFFER_HULL_SHADER constantBuffer_HullShader;
    CBUFFER_DOMAIN_SHADER constantBuffer_DomainShader;
    CBUFFER_PIXEL_SHADER constantBuffer_PixelShader;

    XMMATRIX worldMatrix;
    XMMATRIX viewMatrix;
    XMMATRIX wvpMatrix;

    UINT stride;
    UINT offset;

    //code
    //clear buffers
    gpID3D11DeviceContext->ClearRenderTargetView(gpID3D11RenderTargetView, gClearColor);
    gpID3D11DeviceContext->ClearDepthStencilView(gpID3D11DepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

    //set matrices 
    worldMatrix = XMMatrixTranslation(0.0f, 0.0f, 4.0f);
    viewMatrix = XMMatrixIdentity();
    wvpMatrix = worldMatrix * viewMatrix * perspectiveProjectionMatrix;

    ZeroMemory((void*)&constantBuffer_DomainShader, sizeof(CBUFFER_DOMAIN_SHADER));
    constantBuffer_DomainShader.WorldViewProjectionMatrix = wvpMatrix;
    gpID3D11DeviceContext->UpdateSubresource(gpID3D11Buffer_ConstantBuffer_DomainShader, 0, NULL, &constantBuffer_DomainShader, 0, 0);

    ZeroMemory((void*)&constantBuffer_HullShader, sizeof(CBUFFER_HULL_SHADER));
    constantBuffer_HullShader.HullConstantFunctionParam = XMVectorSet(1.0f, (FLOAT)gNumberOfLineSegements, 0.0f, 0.0f);
    gpID3D11DeviceContext->UpdateSubresource(gpID3D11Buffer_ConstantBuffer_HullShader, 0, NULL, &constantBuffer_HullShader, 0, 0);

    ZeroMemory((void*)&constantBuffer_PixelShader, sizeof(CBUFFER_PIXEL_SHADER));
    constantBuffer_PixelShader.LineColor = XMVectorSet(1.0f, 1.0f, 0.0f, 1.0f);
    gpID3D11DeviceContext->UpdateSubresource(gpID3D11Buffer_ConstantBuffer_PixelShader, 0, NULL, &constantBuffer_PixelShader, 0, 0);

    stride = sizeof(float) * 2;
    offset = 0;
    gpID3D11DeviceContext->IASetVertexBuffers(0, 1, &gpID3D11Buffer_VertexBuffer, &stride, &offset);

    gpID3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_4_CONTROL_POINT_PATCHLIST);
    gpID3D11DeviceContext->Draw(4, 0);

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

    if (gpID3D11Buffer_ConstantBuffer_DomainShader)
    {
        gpID3D11Buffer_ConstantBuffer_DomainShader->Release();
        gpID3D11Buffer_ConstantBuffer_DomainShader = NULL;
    }

    if (gpID3D11Buffer_ConstantBuffer_HullShader)
    {
        gpID3D11Buffer_ConstantBuffer_HullShader->Release();
        gpID3D11Buffer_ConstantBuffer_HullShader = NULL;
    }

    if (gpID3D11Buffer_ConstantBuffer_PixelShader)
    {
        gpID3D11Buffer_ConstantBuffer_PixelShader->Release();
        gpID3D11Buffer_ConstantBuffer_PixelShader = NULL;
    }

    if (gpID3D11Buffer_VertexBuffer)
    {
        gpID3D11Buffer_VertexBuffer->Release();
        gpID3D11Buffer_VertexBuffer = NULL;
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

    if (gpID3D11DomainShader)
    {
        gpID3D11DomainShader->Release();
        gpID3D11DomainShader = NULL;
    }

    if (gpID3D11HullShader)
    {
        gpID3D11HullShader->Release();
        gpID3D11HullShader = NULL;
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