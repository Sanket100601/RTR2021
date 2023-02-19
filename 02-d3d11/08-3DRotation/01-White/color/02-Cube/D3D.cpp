// Header Files
#include <windows.h>
#include <stdio.h>  // for FileIO functions
#include <stdlib.h> // for exit()
#include <math.h>
#include "D3D.h"


/* D3D related Header files */
#include <dxgi.h> // directx graphics infrastructure
#include <d3d11.h>
#include <d3dcompiler.h>
#pragma warning(disable:4838)
#include "xnamath.h"


/* D3D related lib */
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

#define WIN_WIDTH 800
#define WIN_HEIGHT 600

// global variable declarations
HWND ghwnd = NULL;
BOOL gbFullScreen = FALSE;
BOOL gbActiveWindow = FALSE;
FILE* gpFile = NULL;


/* D3D11 Related Variables */
IDXGISwapChain *gpIDXGISwapChain = NULL;
ID3D11Device *gpID3D11Device = NULL;
ID3D11DeviceContext *gpID3D11DeviceContext = NULL;
ID3D11RenderTargetView *gpID3D11RenderTargetView = NULL;
float clearColor[4];


ID3D11VertexShader *gpID3D11VertexShader = NULL;
ID3D11PixelShader *gpID3D11PixelShader = NULL;
ID3D11InputLayout* gpID3D11InputLayout = NULL;
ID3D11Buffer *gpID3D11Buffer_Position_CubeBuffer = NULL;
ID3D11Buffer* gpID3D11Buffer_Color_CubeBuffer = NULL;
ID3D11Buffer* gpID3D11Buffer_ConstantBuffer = NULL;
ID3D11RasterizerState* gpID3D11RasterizerState = NULL;
ID3D11DepthStencilView* gpID3D11DepthStencilView = NULL;


struct CBUFFER
{
	XMMATRIX WorldViewProjectionMatrix;
};

XMMATRIX perspectiveProjectionMatrix;

float angleCube = 0.0f;


// global function declarations
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

// Entry point function
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow)
{
	// function delcarations
	HRESULT initialise (void);
	void display (void);
	void update (void);
	void uninitialise(void);
	
	// variable declarations
	WNDCLASSEX wndclass;
	HWND hwnd;
	MSG msg;
	TCHAR szAppName[] = TEXT("My Window");
	BOOL bDone = FALSE;
	HRESULT hr = S_OK; //sucess_ok
	int width, height;
	int x, y;

	// code
	if (fopen_s(&gpFile, "Log.txt", "w") != 0)
	{
		MessageBox(NULL, TEXT("Creation of Log File Failed. Exitting..."), TEXT("File I/O Error"), MB_OK);
		exit(0);
	}
	else
	{
		fprintf(gpFile, "Log File Successfully Created\n");
		fclose(gpFile);
	}
	width = GetSystemMetrics(SM_CXSCREEN);
	height = GetSystemMetrics(SM_CYSCREEN);

	x = (width - WIN_WIDTH) / 2;
	y = (height - WIN_HEIGHT) / 2;

	// Initialization of class structure
	wndclass.cbSize = sizeof(WNDCLASSEX);
	wndclass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.lpfnWndProc = WndProc;
	wndclass.hInstance = hInstance;
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(MYICON));
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.lpszClassName = szAppName;
	wndclass.lpszMenuName = NULL;
	wndclass.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(MYICON));

	// Resgistry above class
	RegisterClassEx(&wndclass);

	// create the window
	hwnd = CreateWindow(
		szAppName,
		TEXT("D3D11"),
		WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_VISIBLE,
		x,
		y,
		WIN_WIDTH,
		WIN_HEIGHT,
		NULL,
		NULL,
		hInstance,
		NULL);
	ghwnd = hwnd;

	// 	initialise
	hr = initialise();
	if(FAILED(hr))
	{
		if (fopen_s(&gpFile, "Log.txt", "a+") != 0)
		{
			fprintf(gpFile, "Init() Failed!!!\n");
			fclose(gpFile);
			DestroyWindow(hwnd);
			hwnd = NULL;
		}
		
	}
	else
	{
		fopen_s(&gpFile, "Log.txt", "a+");
		fprintf(gpFile, "Init() Sucessfully !!!\n");
		fclose(gpFile);
	}
	
	
	// show window
	ShowWindow(hwnd, iCmdShow);

	// Foregrounding and focusing window
	SetForegroundWindow(hwnd); 
	SetFocus(hwnd);

	// Game Loop
	while (bDone == FALSE)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				bDone = TRUE;
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			if (gbActiveWindow == TRUE)
			{
				// render the scene
				display();

				// update the scene
				update();

			}
		}
	}

	uninitialise();
	return ((int)msg.wParam);
}

// CALLBACK FUNCTION
LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	// function declarations
	void ToggleFullScreen(void);
	HRESULT resize(int width, int height);
	void uninitialise(void);

	// VARIABLE DECLARATIONS
	HRESULT hr = S_OK;

	// Code
	switch (iMsg)
	{
	
	case WM_SETFOCUS:
		gbActiveWindow = TRUE;
		break;
	case WM_KILLFOCUS:
		gbActiveWindow = FALSE;
		break;
	case WM_ERASEBKGND:

		break; // as this is retained mode graphics there is WM_PAINT to paint.
	case WM_CHAR:
		switch (wParam)
		{
		case 'F':
		case 'f':
			ToggleFullScreen();
			break;
		default:
			break;
		}
		break;
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
		{
			if (gpFile)
			{
				fprintf(gpFile, "Log File Is Sucessfully Closed !!!\n");
				fclose(gpFile);
				gpFile = NULL;
			}
			//PostQuitMessage(0);
			DestroyWindow(hwnd);
		}
			
		break;
	case WM_SIZE:
		if(gpID3D11DeviceContext)
		{
			hr = resize(LOWORD(lParam), HIWORD(lParam));
		}
		if (FAILED(hr))
		{
			if (fopen_s(&gpFile, "Log.txt", "a+") != 0)
			{
				fprintf(gpFile, "resize() in WndProc Failed!!!\n");
				fclose(gpFile);		
				return hr;
			}
		}
		break;
	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;
	case WM_DESTROY:
		uninitialise();
		PostQuitMessage(0);
		break;
	default:
		break;
	}

	return (DefWindowProc(hwnd, iMsg, wParam, lParam));
}

void ToggleFullScreen(void)
{
	// varible declarations
	static DWORD dwStyle;
	static WINDOWPLACEMENT wp;
	MONITORINFO mi;


	// code
	wp.length = sizeof(WINDOWPLACEMENT);

	if (gbFullScreen == FALSE)
	{
		dwStyle = GetWindowLong(ghwnd, GWL_STYLE);
		if (dwStyle & WS_OVERLAPPEDWINDOW)
		{
			mi.cbSize = sizeof(MONITORINFO);


			if (GetWindowPlacement(ghwnd, &wp) && GetMonitorInfo(MonitorFromWindow(ghwnd, MONITORINFOF_PRIMARY), &mi))
			{
				SetWindowLong(ghwnd, GWL_STYLE, dwStyle & ~WS_OVERLAPPEDWINDOW);
				SetWindowPos(ghwnd, HWND_TOP, mi.rcMonitor.left, mi.rcMonitor.top, mi.rcMonitor.right - mi.rcMonitor.left, mi.rcMonitor.bottom - mi.rcMonitor.top, SWP_NOZORDER | SWP_FRAMECHANGED);

			}
			ShowCursor(FALSE);
			gbFullScreen = TRUE;
		}

	}
	else
	{
		SetWindowLong(ghwnd, GWL_STYLE, dwStyle | WS_OVERLAPPEDWINDOW);
		SetWindowPlacement(ghwnd, &wp);
		SetWindowPos(ghwnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOOWNERZORDER | SWP_NOZORDER | SWP_FRAMECHANGED);

		ShowCursor(TRUE);
		gbFullScreen = FALSE;
	}
}

HRESULT initialise(void)
{
	// function declarations
	HRESULT printD3DInfo(void);
	HRESULT resize(int width, int height);

	// variable declarations
	HRESULT hr = S_OK; //sucess_ok
	D3D_DRIVER_TYPE d3dDriverType;
	D3D_DRIVER_TYPE d3dDriverTypes[] = {D3D_DRIVER_TYPE_HARDWARE , D3D_DRIVER_TYPE_WARP, D3D_DRIVER_TYPE_REFERENCE};
	D3D_FEATURE_LEVEL d3dFeaatureLevel_Required = D3D_FEATURE_LEVEL_11_0;
	D3D_FEATURE_LEVEL d3dFeaatureLevel_Accquired = D3D_FEATURE_LEVEL_10_0;
	UINT numDrivers = 0;
	UINT createDeviceFlags = 0;
	UINT numFeatureLevels = 1;


	// code
	
	/* Init Swapchain Decription sctruture */
	DXGI_SWAP_CHAIN_DESC dxgiSwapChainDesc;
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

	/* Call D3D11 CreateDevice and Swapchain funcs for required Driver */
	numDrivers = sizeof(d3dDriverTypes) / sizeof(d3dDriverTypes[0]);

	for (UINT i = 0; i < numDrivers; i++)
	{
		d3dDriverType = d3dDriverTypes[i];


		hr = D3D11CreateDeviceAndSwapChain(
			NULL,
			d3dDriverType,
			NULL,
			createDeviceFlags,
			&d3dFeaatureLevel_Required,
			numFeatureLevels,
			D3D11_SDK_VERSION,
			&dxgiSwapChainDesc,
			&gpIDXGISwapChain,
			&gpID3D11Device,
			&d3dFeaatureLevel_Accquired,
			&gpID3D11DeviceContext);

		if (SUCCEEDED(hr))
			break;
	}

	if (FAILED(hr))
	{
		fopen_s(&gpFile, "Log.txt", "a+");
		fprintf(gpFile, "Init() in D3D11CreateDeviceAndSwapChain() Failed!!!\n");
		fclose(gpFile);
		return hr;
	}
	else
	{
		fopen_s(&gpFile, "Log.txt", "a+");

		/* Print Obtained Driver Type */
		if (d3dDriverType == D3D_DRIVER_TYPE_HARDWARE)
		{
			fprintf(gpFile, "D3D11 Obtained Hardware Driver !!!\n");
		}
		else if (d3dDriverType == D3D_DRIVER_TYPE_WARP)
		{
			fprintf(gpFile, "D3D11 Obtained WARP Driver !!!\n");
		}
		else if (d3dDriverType == D3D_DRIVER_TYPE_REFERENCE)
		{
			fprintf(gpFile, "D3D11 Obtained REFERENCE Driver !!!\n");
		}
		else
		{
			fprintf(gpFile, "D3D11 Obtained Unknown Driver !!!\n");
		}

		/* Print Obtained Feature Level */
		if (d3dFeaatureLevel_Accquired == D3D_FEATURE_LEVEL_11_0)
		{
			fprintf(gpFile, "D3D11 Obtained 11.0 Feature Level !!!\n");
		}
		else if (d3dFeaatureLevel_Accquired == D3D_FEATURE_LEVEL_10_1)
		{
			fprintf(gpFile, "D3D11 Obtained 10.1 Feature Level !!!\n");
		}
		else if (d3dFeaatureLevel_Accquired == D3D_FEATURE_LEVEL_10_0)
		{
			fprintf(gpFile, "D3D11 Obtained 10.0 Feature Level !!!\n");
		}
		else
		{
			fprintf(gpFile, "D3D11 Obtained Unknown Feature Level !!!\n");
		}

		fclose(gpFile);
	}

	/* Vertex Shader */
	const char* VertexShaderSourceCode =
		"cbuffer ConstantBuffer" \
		"{" \
		"	float4x4 worldViewProjectionMatrix;" \
		"}" \

		"struct vertex" \
		"{" \
		"	float4 position:SV_POSITION;" \
		"	float4 color:COLOR;" \
		"};" \

		"vertex main(float4 position: POSITION, float4 color: COLOR)" \
		"{" \
		"	vertex output;" \
		"	output.position = mul(worldViewProjectionMatrix, position);" \
		"	output.color = color;" \
		"	return output;" \
		"}";

	ID3DBlob* pID3DBlob_vertexShaderCode = NULL;
	ID3DBlob* pID3DBlob_error = NULL;


	/* compile vertex shader */
	hr = D3DCompile(VertexShaderSourceCode,
		lstrlenA(VertexShaderSourceCode) + 1, // +1 for considering  /0 null terminating 
		"VS",
		NULL,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"main",
		"vs_5_0",
		0,
		0,
		&pID3DBlob_vertexShaderCode,
		&pID3DBlob_error
	);

	if (FAILED(hr))
	{
		fopen_s(&gpFile, "Log.txt", "a+");
		if (pID3DBlob_error)
		{
			fprintf(gpFile, "D3DCompile Failed for Vertex Shader : %s !!!\n", (char*)pID3DBlob_error->GetBufferPointer());
			pID3DBlob_error->Release();
			pID3DBlob_error = NULL;
		}
		else
		{
			fprintf(gpFile, "D3DCompile Failed for Vertex Shader Unknown !!!\n");
		}
		fclose(gpFile);
		return hr;
	}
	else
	{
		fopen_s(&gpFile, "Log.txt", "a+");
		fprintf(gpFile, "D3DCompile Successed for Vertex Shader !!!\n");
		fclose(gpFile);
	}

	/* Create Vertex Shader */
	hr = gpID3D11Device->CreateVertexShader(pID3DBlob_vertexShaderCode->GetBufferPointer(),
		pID3DBlob_vertexShaderCode->GetBufferSize(),
		NULL,
		&gpID3D11VertexShader);

	if (FAILED(hr))
	{
		fopen_s(&gpFile, "Log.txt", "a+");
		fprintf(gpFile, "Init() in gpID3D11Device::CreateVertexShader() Vertex Shader Failed!!!\n");
		fclose(gpFile);
		return hr;
	}
	else
	{
		fopen_s(&gpFile, "Log.txt", "a+");
		fprintf(gpFile, "Init() in gpID3D11Device::CreateVertexShader() Vertex Shader Successed!!!\n");
		fclose(gpFile);
	}

	/* Set Vertex Shader in VS Stage of Pipeline */
	gpID3D11DeviceContext->VSSetShader(gpID3D11VertexShader, NULL, 0);



	/* Pixel Shader */
	const char* PixelShaderSourceCode =
		"struct vertex" \
		"{" \
		"	float4 position:SV_POSITION;" \
		"	float4 color:COLOR;" \
		"};" \

		"float4 main(vertex input): SV_TARGET" \
		"{" \
		"	float4 color;" \
		"	color = input.color;" \
		" return color;" \
		"}";


	ID3DBlob* pID3DBlob_pixelShaderCode = NULL;
	pID3DBlob_error = NULL;


	/* compile Pixel shader */
	hr = D3DCompile(PixelShaderSourceCode,
		lstrlenA(PixelShaderSourceCode) + 1, // +1 for considering  /0 null terminating 
		"PS",
		NULL,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"main",
		"ps_5_0",
		0,
		0,
		&pID3DBlob_pixelShaderCode,
		&pID3DBlob_error
	);

	if (FAILED(hr))
	{
		fopen_s(&gpFile, "Log.txt", "a+");
		if (pID3DBlob_error)
		{
			fprintf(gpFile, "D3DCompile Failed for Pixel Shader : %s !!!\n", (char*)pID3DBlob_error->GetBufferPointer());
			pID3DBlob_error->Release();
			pID3DBlob_error = NULL;
		}
		else
		{
			fprintf(gpFile, "D3DCompile Failed for Pixel Shader Unknown !!!\n");
		}
		fclose(gpFile);
		return hr;
	}
	else
	{
		fopen_s(&gpFile, "Log.txt", "a+");
		fprintf(gpFile, "D3DCompile Successed for Pixel Shader !!!\n");
		fclose(gpFile);
	}

	/* Create Vertex Shader */
	hr = gpID3D11Device->CreatePixelShader(pID3DBlob_pixelShaderCode->GetBufferPointer(),
		pID3DBlob_pixelShaderCode->GetBufferSize(),
		NULL,
		&gpID3D11PixelShader);

	if (FAILED(hr))
	{
		fopen_s(&gpFile, "Log.txt", "a+");
		fprintf(gpFile, "Init() in gpID3D11Device::CreateVertexShader() Pixel Shader Failed!!!\n");
		fclose(gpFile);
		return hr;
	}
	else
	{
		fopen_s(&gpFile, "Log.txt", "a+");
		fprintf(gpFile, "Init() in gpID3D11Device::CreateVertexShader() Pixel Shader Successed!!!\n");
		fclose(gpFile);
	}

	/* Set Vertex Shader in VS Stage of Pipeline */
	gpID3D11DeviceContext->PSSetShader(gpID3D11PixelShader, NULL, 0);

	// =======================================================================

	/* ===============  Input Layout ======================= */

	/* 1. Init Input Layout Structure */
	D3D11_INPUT_ELEMENT_DESC d3d11InputElementDescriptor[2];
	ZeroMemory((void*)d3d11InputElementDescriptor, sizeof(D3D11_INPUT_ELEMENT_DESC) * _ARRAYSIZE(d3d11InputElementDescriptor));

	/* Position */
	d3d11InputElementDescriptor[0].SemanticName = "POSITION";
	d3d11InputElementDescriptor[0].SemanticIndex = 0;
	d3d11InputElementDescriptor[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	d3d11InputElementDescriptor[0].AlignedByteOffset = 0;
	d3d11InputElementDescriptor[0].InputSlot = 0;
	d3d11InputElementDescriptor[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	d3d11InputElementDescriptor[0].InstanceDataStepRate = 0;

	/* Color */
	d3d11InputElementDescriptor[1].SemanticName = "COLOR";
	d3d11InputElementDescriptor[1].SemanticIndex = 0;
	d3d11InputElementDescriptor[1].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	d3d11InputElementDescriptor[1].AlignedByteOffset = 0;
	d3d11InputElementDescriptor[1].InputSlot = 1;
	d3d11InputElementDescriptor[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	d3d11InputElementDescriptor[1].InstanceDataStepRate = 0;
	
	/*2. Create Input Layout */
	hr = gpID3D11Device->CreateInputLayout( d3d11InputElementDescriptor,
											_ARRAYSIZE(d3d11InputElementDescriptor),
											pID3DBlob_vertexShaderCode->GetBufferPointer(),
											pID3DBlob_vertexShaderCode->GetBufferSize(),
											&gpID3D11InputLayout);

	if (FAILED(hr))
	{
		fopen_s(&gpFile, "Log.txt", "a+");
		fprintf(gpFile, "Init() in gpID3D11Device::CreateInputLayout()  Failed!!!\n");
		fclose(gpFile);
		return hr;
	}
	else
	{
		fopen_s(&gpFile, "Log.txt", "a+");
		fprintf(gpFile, "Init() in gpID3D11Device::CreateInputLayout() Successed!!!\n");
		fclose(gpFile);
	}

	/*3. Set this Input Layout  in input assembly stage of pipeline */

	gpID3D11DeviceContext->IASetInputLayout(gpID3D11InputLayout);

	/* Release Blob of shaders */
	pID3DBlob_vertexShaderCode->Release();
	pID3DBlob_vertexShaderCode = NULL;
	pID3DBlob_pixelShaderCode->Release();
	pID3DBlob_pixelShaderCode = NULL;


	/*===================== Geometry =========================== */
	const float cubeVertices[] = {
									// position : x,y,z 
									// SIDE 1 ( TOP )
									-1.0f, +1.0f, +1.0f,
									+1.0f, +1.0f, +1.0f,
									-1.0f, +1.0f, -1.0f,
									-1.0f, +1.0f, -1.0f,
									+1.0f, +1.0f, +1.0f,
									+1.0f, +1.0f, -1.0f,

									// SIDE 2 ( BOTTOM )
									+1.0f, -1.0f, -1.0f,
									+1.0f, -1.0f, +1.0f,
									-1.0f, -1.0f, -1.0f,

									-1.0f, -1.0f, -1.0f,
									+1.0f, -1.0f, +1.0f,
									-1.0f, -1.0f, +1.0f,

									// SIDE 3 ( FRONT )
									-1.0f, +1.0f, -1.0f,
									+1.0f, +1.0f, -1.0f,
									-1.0f, -1.0f, -1.0f,

									-1.0f, -1.0f, -1.0f,
									+1.0f, +1.0f, -1.0f,
									+1.0f, -1.0f, -1.0f,

									// SIDE 4 ( BACK )
									+1.0f, -1.0f, +1.0f,
									+1.0f, +1.0f, +1.0f,
									-1.0f, -1.0f, +1.0f,

									-1.0f, -1.0f, +1.0f,
									+1.0f, +1.0f, +1.0f,
									-1.0f, +1.0f, +1.0f,

									// SIDE 5 ( LEFT )
									-1.0f, +1.0f, +1.0f,
									-1.0f, +1.0f, -1.0f,
									-1.0f, -1.0f, +1.0f,

									-1.0f, -1.0f, +1.0f,
									-1.0f, +1.0f, -1.0f,
									-1.0f, -1.0f, -1.0f,

									// SIDE 6 ( RIGHT )
									+1.0f, -1.0f, -1.0f,
									+1.0f, +1.0f, -1.0f,
									+1.0f, -1.0f, +1.0f,

									+1.0f, -1.0f, +1.0f,
									+1.0f, +1.0f, -1.0f,
									+1.0f, +1.0f, +1.0f,
									};

	const float cubeColor[] = {
									//  color : r,g,b
									// SIDE 1 ( TOP )
									+1.0f, +0.0f, +0.0f,
									+1.0f, +0.0f, +0.0f,
									+1.0f, +0.0f, +0.0f,

									+1.0f, +0.0f, +0.0f,
									+1.0f, +0.0f, +0.0f,
									+1.0f, +0.0f, +0.0f,

									// SIDE 2 ( BOTTOM )
									+0.0f, +1.0f, +0.0f,
									+0.0f, +1.0f, +0.0f,
									+0.0f, +1.0f, +0.0f,

									+0.0f, +1.0f, +0.0f,
									+0.0f, +1.0f, +0.0f,
									+0.0f, +1.0f, +0.0f,

									// SIDE 3 ( FRONT )
									+0.0f, +0.0f, +1.0f,
									+0.0f, +0.0f, +1.0f,
									+0.0f, +0.0f, +1.0f,

									+0.0f, +0.0f, +1.0f,
									+0.0f, +0.0f, +1.0f,
									+0.0f, +0.0f, +1.0f,

									// SIDE 4 ( BACK )
									+0.0f, +1.0f, +1.0f,
									+0.0f, +1.0f, +1.0f,
									+0.0f, +1.0f, +1.0f,

									+0.0f, +1.0f, +1.0f,
									+0.0f, +1.0f, +1.0f,
									+0.0f, +1.0f, +1.0f,

									// SIDE 5 ( LEFT )
									+1.0f, +0.0f, +1.0f,
									+1.0f, +0.0f, +1.0f,
									+1.0f, +0.0f, +1.0f,

									+1.0f, +0.0f, +1.0f,
									+1.0f, +0.0f, +1.0f,
									+1.0f, +0.0f, +1.0f,

									// SIDE 6 ( RIGHT )
									+1.0f, +1.0f, +0.0f,
									+1.0f, +1.0f, +0.0f,
									+1.0f, +1.0f, +0.0f,

									+1.0f, +1.0f, +0.0f,
									+1.0f, +1.0f, +0.0f,
									+1.0f, +1.0f, +0.0f,

									};


	/*  Create Vertex buffer for above position vertices */

	/* A. Init BufferDecriptor Structure */ // glGenBuffer

	/*==========             Position      ============= */
	D3D11_BUFFER_DESC d3d11BufferDescriptor;
	ZeroMemory((void*)&d3d11BufferDescriptor, sizeof(D3D11_BUFFER_DESC));

	d3d11BufferDescriptor.Usage = D3D11_USAGE_DEFAULT;
	d3d11BufferDescriptor.ByteWidth = sizeof(float) * _ARRAYSIZE(cubeVertices);
	d3d11BufferDescriptor.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	/* B. Init subresource data structure to put data into buffer */ //glbufferdata
	D3D11_SUBRESOURCE_DATA d3d11SubResourceData;
	ZeroMemory((void*)& d3d11SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));

	d3d11SubResourceData.pSysMem = cubeVertices;


	/* C.Create the Actual Buffer */ // 
	hr = gpID3D11Device->CreateBuffer(&d3d11BufferDescriptor,
										&d3d11SubResourceData,
										&gpID3D11Buffer_Position_CubeBuffer);

	if (FAILED(hr))
	{
		fopen_s(&gpFile, "Log.txt", "a+");
		fprintf(gpFile, "Init() in gpID3D11Device::CreateBuffer() for Position Buffer  Failed!!!\n");
		fclose(gpFile);
		return hr;
	}
	else
	{
		fopen_s(&gpFile, "Log.txt", "a+");
		fprintf(gpFile, "Init() in gpID3D11Device::CreateBuffer() for Position Buffer  Successed!!!\n");
		fclose(gpFile);
	}

	/* Constant Buffer */

	/* A. Init BufferDecriptor Structure */
	ZeroMemory((void*)&d3d11BufferDescriptor, sizeof(D3D11_BUFFER_DESC));

	d3d11BufferDescriptor.Usage = D3D11_USAGE_DEFAULT;
	d3d11BufferDescriptor.ByteWidth = sizeof(CBUFFER);
	d3d11BufferDescriptor.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	
	/* B. Create the Actual Buffer */ 
	hr = gpID3D11Device->CreateBuffer(&d3d11BufferDescriptor,
		NULL,
		&gpID3D11Buffer_ConstantBuffer);

	if (FAILED(hr))
	{
		fopen_s(&gpFile, "Log.txt", "a+");
		fprintf(gpFile, "Init() in gpID3D11Device::CreateBuffer() for Constant Buffer  Failed!!!\n");
		fclose(gpFile);
		return hr;
	}
	else
	{
		fopen_s(&gpFile, "Log.txt", "a+");
		fprintf(gpFile, "Init() in gpID3D11Device::CreateBuffer() for Constant Buffer Successed!!!\n");
		fclose(gpFile);
	}


	/* ============================  Rasterizer State   =============================== */
	/* Enabling Rasterizer State  */

	/* A. Init Rasterizer descriptor */
	D3D11_RASTERIZER_DESC d3d11RasterizerDescriptor;

	ZeroMemory((void*)&d3d11RasterizerDescriptor, sizeof(D3D11_RASTERIZER_DESC));

	d3d11RasterizerDescriptor.CullMode = D3D11_CULL_NONE;
	d3d11RasterizerDescriptor.FillMode = D3D11_FILL_SOLID;
	d3d11RasterizerDescriptor.FrontCounterClockwise = FALSE;
	d3d11RasterizerDescriptor.MultisampleEnable = FALSE;
	d3d11RasterizerDescriptor.ScissorEnable = FALSE;
	d3d11RasterizerDescriptor.DepthClipEnable = TRUE;
	d3d11RasterizerDescriptor.AntialiasedLineEnable = FALSE;
	d3d11RasterizerDescriptor.DepthBias = 0;
	d3d11RasterizerDescriptor.DepthBiasClamp = 0.0;
	d3d11RasterizerDescriptor.SlopeScaledDepthBias = FALSE;


	/* B. Create Rasterizer State*/
	hr = gpID3D11Device->CreateRasterizerState(&d3d11RasterizerDescriptor, &gpID3D11RasterizerState);
	if (FAILED(hr))
	{
		fopen_s(&gpFile, "Log.txt", "a+");
		fprintf(gpFile, "Init() in gpID3D11Device::CreateRasterizerState() Failed!!!\n");
		fclose(gpFile);
		return hr;
	}
	else
	{
		fopen_s(&gpFile, "Log.txt", "a+");
		fprintf(gpFile, "Init() in gpID3D11Device::CreateRasterizerState()  Successed!!!\n");
		fclose(gpFile);
	}

	/* C. Set Rasterizer State in pipeline */
	gpID3D11DeviceContext->RSSetState(gpID3D11RasterizerState);



	/*==========           Color      ============= */
	ZeroMemory((void*)&d3d11BufferDescriptor, sizeof(D3D11_BUFFER_DESC));

	d3d11BufferDescriptor.Usage = D3D11_USAGE_DEFAULT;
	d3d11BufferDescriptor.ByteWidth = sizeof(float) * _ARRAYSIZE(cubeColor);
	d3d11BufferDescriptor.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	/* B. Init subresource data structure to put data into buffer */ //glbufferdata
	ZeroMemory((void*)&d3d11SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));

	d3d11SubResourceData.pSysMem = cubeColor;


	/* C.Create the Actual Buffer */ // 
	hr = gpID3D11Device->CreateBuffer(&d3d11BufferDescriptor,
		&d3d11SubResourceData,
		&gpID3D11Buffer_Color_CubeBuffer);

	if (FAILED(hr))
	{
		fopen_s(&gpFile, "Log.txt", "a+");
		fprintf(gpFile, "Init() in gpID3D11Device::CreateBuffer() for Color Buffer  Failed!!!\n");
		fclose(gpFile);
		return hr;
	}
	else
	{
		fopen_s(&gpFile, "Log.txt", "a+");
		fprintf(gpFile, "Init() in gpID3D11Device::CreateBuffer() for Color Buffer  Successed!!!\n");
		fclose(gpFile);
	}

	/* Constant Buffer */

	/* A. Init BufferDecriptor Structure */
	ZeroMemory((void*)&d3d11BufferDescriptor, sizeof(D3D11_BUFFER_DESC));

	d3d11BufferDescriptor.Usage = D3D11_USAGE_DEFAULT;
	d3d11BufferDescriptor.ByteWidth = sizeof(CBUFFER);
	d3d11BufferDescriptor.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	/* B. Create the Actual Buffer */
	hr = gpID3D11Device->CreateBuffer(&d3d11BufferDescriptor,
		NULL,
		&gpID3D11Buffer_ConstantBuffer);

	if (FAILED(hr))
	{
		fopen_s(&gpFile, "Log.txt", "a+");
		fprintf(gpFile, "Init() in gpID3D11Device::CreateBuffer() for Constant Buffer  Failed!!!\n");
		fclose(gpFile);
		return hr;
	}
	else
	{
		fopen_s(&gpFile, "Log.txt", "a+");
		fprintf(gpFile, "Init() in gpID3D11Device::CreateBuffer() for Constant Buffer Successed!!!\n");
		fclose(gpFile);
	}

	/* C. Set Constant buffer in vertex shader stage of pipeline */
	gpID3D11DeviceContext->VSSetConstantBuffers(0, 1, &gpID3D11Buffer_ConstantBuffer);


	/* Init clearColor Array */
	clearColor[0] = 0.0f;
	clearColor[1] = 0.0f;
	clearColor[2] = 0.0f; 
	clearColor[3] = 1.0f; // alpha

	/*  Set ProjectionMatrix to Identity */
	perspectiveProjectionMatrix = XMMatrixIdentity();

	/* WARM UP RESIZES */
	hr =  resize(WIN_WIDTH, WIN_HEIGHT);

	if (FAILED(hr))
	{
		fopen_s(&gpFile, "Log.txt", "a+");
		fprintf(gpFile, "Init() in resize() Failed!!!\n");
		fclose(gpFile);
		return hr;
	}
	else
	{
		fopen_s(&gpFile, "Log.txt", "a+");
		fprintf(gpFile, "Init() in resize() Sucessful!!!\n");
		fclose(gpFile);
	}
	
	return hr;
}





HRESULT printD3DInfo(void)
{
	// variable declarations
	HRESULT hr = S_OK; //sucess_ok
	IDXGIFactory *pIDXGIFactory = NULL;
	IDXGIAdapter *pIDXGIAdapter = NULL;
	DXGI_ADAPTER_DESC dxgiAdapterDescriptor;
	char str[256];

	// code
	

	// 1. CreateDXGIFactory
	hr = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&pIDXGIFactory);
	if(FAILED(hr))
	{
		fopen_s(&gpFile, "Log.txt", "a+");
		fprintf(gpFile, "CreateDXGIFactory() Failed!!!\n");
		fclose(gpFile);
		return hr;
		
	}
	else
	{
		fopen_s(&gpFile, "Log.txt", "a+");
		fprintf(gpFile, "CreateDXGIFactory() Is Sucessfully called !!!\n");
		fclose(gpFile);
		
	}

	// 2. GetIDXGIAdapter
	if(pIDXGIFactory->EnumAdapters(0, &pIDXGIAdapter) != DXGI_ERROR_NOT_FOUND)
	{
		// 3. Get Descriptionof found adapter
		ZeroMemory((void*)&dxgiAdapterDescriptor, sizeof(DXGI_ADAPTER_DESC));

		pIDXGIAdapter->GetDesc(&dxgiAdapterDescriptor);
		WideCharToMultiByte(CP_ACP, 0, dxgiAdapterDescriptor.Description, 255, str, 255, NULL, NULL);
		fopen_s(&gpFile, "Log.txt", "a+");
		fprintf(gpFile, "Graphics Card Name = %s\n", str);
		fprintf(gpFile, "Graphics Card Memory = %d GB\n", (int)ceil(dxgiAdapterDescriptor.DedicatedVideoMemory/10.24/10.24/10.24));
		fclose(gpFile);
			
		
	}
	else
	{
		fopen_s(&gpFile, "Log.txt", "a+");
		fprintf(gpFile, "GetIDXGIAdapterp::IDXGIFactory->EnumAdapters Is failed !!!\n");
		fclose(gpFile);
		
	}

	if(pIDXGIAdapter)
	{
		pIDXGIAdapter->Release();
		pIDXGIAdapter = NULL;
	}
	if(pIDXGIFactory)
	{
		pIDXGIFactory->Release();
		pIDXGIFactory = NULL;
	}


	return hr;

}

HRESULT resize(int width, int height)
{
	// variable declarations
	HRESULT hr = S_OK; //sucess_ok
	ID3D11Texture2D* pID3D11Texture2D_BackBuffer = NULL;
	D3D11_VIEWPORT d3dViewport;

	// code 

	/* 1. Release exiting RTV */
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

	/* 2. Tell swapchain to resize buffers according to new width and heigh */
	gpIDXGISwapChain->ResizeBuffers(1, width, height, DXGI_FORMAT_R8G8B8A8_UNORM, 0);

	/* 3. Get New rezied buffer from swapchain into a dummy Texture */
	gpIDXGISwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pID3D11Texture2D_BackBuffer);

	/* 4. Now Create New Render Treget View (RTV) using above buffer */
	hr = gpID3D11Device->CreateRenderTargetView(pID3D11Texture2D_BackBuffer, NULL, &gpID3D11RenderTargetView);
	if (FAILED(hr))
	{
		fopen_s(&gpFile, "Log.txt", "a+");
		fprintf(gpFile, "Resize() in CreateRenderTargetView() Failed!!!\n");
		fclose(gpFile);
		return hr;
	}

	// Relese dummy Texture Interface
	pID3D11Texture2D_BackBuffer->Release();
	pID3D11Texture2D_BackBuffer = NULL;

	/* ========== Depth Stensil View ==============*/

	/*1. Init Texture2D Descriptor */
	D3D11_TEXTURE2D_DESC d3d11Texture2dDescriptor;

	ZeroMemory((void*)&d3d11Texture2dDescriptor, sizeof(D3D11_TEXTURE2D_DESC));

	d3d11Texture2dDescriptor.Width = (UINT)width;
	d3d11Texture2dDescriptor.Height = (UINT)height;
	d3d11Texture2dDescriptor.ArraySize = 1;
	d3d11Texture2dDescriptor.MipLevels = 1;
	d3d11Texture2dDescriptor.SampleDesc.Count = 1;
	d3d11Texture2dDescriptor.SampleDesc.Quality = 0;
	d3d11Texture2dDescriptor.Usage = D3D11_USAGE_DEFAULT;
	d3d11Texture2dDescriptor.Format = DXGI_FORMAT_D32_FLOAT;
	d3d11Texture2dDescriptor.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	d3d11Texture2dDescriptor.CPUAccessFlags = 0;
	d3d11Texture2dDescriptor.MiscFlags = 0;

	/*2. Declare 2D Texture which is going be converted in depthbuffer */
	ID3D11Texture2D* pID3D11Texture2D_DepthBuffer = NULL;

	hr = gpID3D11Device->CreateTexture2D(&d3d11Texture2dDescriptor,
										NULL,
										&pID3D11Texture2D_DepthBuffer);
	if (FAILED(hr))
	{
		fopen_s(&gpFile, "Log.txt", "a+");
		fprintf(gpFile, "Resize() in ID3D11Device::CreateTexture2D Failed!!!\n");
		fclose(gpFile);
		return hr;
	}

	/* 3. Init DSV */
	D3D11_DEPTH_STENCIL_VIEW_DESC  d3d11DepthStencilViewDescriptor;

	ZeroMemory((void*)&d3d11DepthStencilViewDescriptor, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));

	d3d11DepthStencilViewDescriptor.Format = DXGI_FORMAT_D32_FLOAT;
	d3d11DepthStencilViewDescriptor.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;

	hr = gpID3D11Device->CreateDepthStencilView(pID3D11Texture2D_DepthBuffer,
												&d3d11DepthStencilViewDescriptor,
												&gpID3D11DepthStencilView);
	if (FAILED(hr))
	{
		fopen_s(&gpFile, "Log.txt", "a+");
		fprintf(gpFile, "Resize() in ID3D11Device::CreateDepthStencilView Failed!!!\n");
		fclose(gpFile);
		pID3D11Texture2D_DepthBuffer->Release();
		pID3D11Texture2D_DepthBuffer = NULL;
		return hr;
	}
	/*4. Release Local Depth Buffer*/
	pID3D11Texture2D_DepthBuffer->Release();
	pID3D11Texture2D_DepthBuffer = NULL;


	/* 5. Set this New RTV in pipeline */
	gpID3D11DeviceContext->OMSetRenderTargets(1, &gpID3D11RenderTargetView, gpID3D11DepthStencilView);

	/* 6. Init Viewport */
	d3dViewport.TopLeftX = 0.0f;
	d3dViewport.TopLeftY = 0.0f;
	d3dViewport.Width = (float)width;
	d3dViewport.Height = (float)height;
	d3dViewport.MinDepth = 0.0f;
	d3dViewport.MaxDepth = 1.0f;


	/* 7. Tell DeviceContext to set this viewport in pipeline */
	gpID3D11DeviceContext->RSSetViewports(1, &d3dViewport);


	/* 8. Set Projection Matrix */
	perspectiveProjectionMatrix = XMMatrixPerspectiveFovLH(
									XMConvertToRadians(45.0f), 
									(float)width / (float)height, 
									0.1f, 
									100.0f);
	if (height == 0)
		height = 1; // to avoid divide by 0 illegal instruction for future code

	return hr;
}

void display(void)
{
	// code
	/* clear Render Traget View (RTV) using clearcolor */
	gpID3D11DeviceContext->ClearRenderTargetView(gpID3D11RenderTargetView, clearColor);
	/* clear Depth Stencil View (DSV) using clearcolor */
	gpID3D11DeviceContext->ClearDepthStencilView(gpID3D11DepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

	/* Set Position Buffer in Input Assembly Stage of pipeline */
	UINT stride = sizeof(float) * 3;
	UINT offeset = 0;

	gpID3D11DeviceContext->IASetVertexBuffers(0,
												1,
												&gpID3D11Buffer_Position_CubeBuffer,
												&stride,
												&offeset);

	/* Set Color Buffer in Input Assembly Stage of pipeline */
	stride = sizeof(float) * 3;
	offeset = 0;

	gpID3D11DeviceContext->IASetVertexBuffers(1, // inputslot matching
		1,
		&gpID3D11Buffer_Color_CubeBuffer,
		&stride,
		&offeset);

	/* Set Primetive topology in Input Assembly Stage */
	gpID3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	/* Transformations */

	/* A. Init Matrices */
	XMMATRIX translationMatrix = XMMatrixTranslation(0.0f, 0.0f, 6.0f);
	XMMATRIX r1 = XMMatrixRotationX(XMConvertToRadians(angleCube));
	XMMATRIX r2 = XMMatrixRotationY(XMConvertToRadians(angleCube));
	XMMATRIX r3 = XMMatrixRotationZ(XMConvertToRadians(angleCube));
	XMMATRIX rotationMatrix = r1 * r2 * r3;
	XMMATRIX scaleMatrix = XMMatrixScaling(0.75f, 0.75f, 0.75f);
	XMMATRIX worldMatrix = scaleMatrix * rotationMatrix * translationMatrix; // order is imp
	XMMATRIX viewMatrix = XMMatrixIdentity();
	XMMATRIX wvpMatrix = worldMatrix * viewMatrix * perspectiveProjectionMatrix;

	/* B. Put them in Constant Buffer */
	CBUFFER constantBuffer;
	ZeroMemory((void*)&constantBuffer, sizeof(CBUFFER));

	constantBuffer.WorldViewProjectionMatrix = wvpMatrix;

	/* C. Push them into the Shader */ // gluniform
	gpID3D11DeviceContext->UpdateSubresource(gpID3D11Buffer_ConstantBuffer,
											0,
											NULL,
											&constantBuffer,
											0,
											0);

	/* D. Draw the Primetiv */
	gpID3D11DeviceContext->Draw(6, 0);
	gpID3D11DeviceContext->Draw(6, 6);
	gpID3D11DeviceContext->Draw(6, 12);
	gpID3D11DeviceContext->Draw(6, 18);
	gpID3D11DeviceContext->Draw(6, 24);
	gpID3D11DeviceContext->Draw(6, 30);


	/* Swapbuffers by presenting them */
	gpIDXGISwapChain->Present(0, 0);
}

void update(void)
{
	// code
	angleCube = angleCube + 2.0f;
	if (angleCube >= 360.0f)
	{
		angleCube = angleCube - 360.0f;
	}
}

void uninitialise(void)
{
	// function declarations
	void ToggleFullScreen(void);

	// code

	if (gpID3D11DepthStencilView)
	{
		gpID3D11DepthStencilView->Release();
		gpID3D11DepthStencilView = NULL;
	}

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

	if (gpID3D11Buffer_Color_CubeBuffer)
	{
		gpID3D11Buffer_Color_CubeBuffer->Release();
		gpID3D11Buffer_Color_CubeBuffer = NULL;
	}

	if (gpID3D11Buffer_Position_CubeBuffer)
	{
		gpID3D11Buffer_Position_CubeBuffer->Release();
		gpID3D11Buffer_Position_CubeBuffer = NULL;
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
	
	



	if (gbFullScreen)
	{
		ToggleFullScreen();
	}
	if (ghwnd)
	{
		DestroyWindow(ghwnd);
		ghwnd = NULL;
	}
	
}




// cl.exe /c /EHsc Window.c
// rc.exe Window.rc
// link.exe Window.res Window.obj user32.lib kernel32.lib gdi32.lib /SUBSYSTEM:WINDOWS