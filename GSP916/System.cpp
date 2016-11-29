#include "System.h"

#include <Windows.h>
#include <time.h>

LRESULT CALLBACK WndProc(HWND hWnd, UINT p_Type, WPARAM p_WParam, LPARAM p_LParam)
{
	if (p_Type == WM_DESTROY)
	{
		PostQuitMessage(1);
	}

	return DefWindowProc(hWnd, p_Type, p_WParam, p_LParam);
}

void System::InitWindow()
{
	// Erstellen der Windowclass
	// Windowclass ist quasi ein Fensterbauplan
	WNDCLASSEX _Windowclass;
	ZeroMemory(&_Windowclass, sizeof(_Windowclass));

	_Windowclass.cbSize = sizeof(WNDCLASSEX);
	_Windowclass.lpszClassName = "WindowBauplan";

	_Windowclass.style = CS_HREDRAW | CS_VREDRAW;
	_Windowclass.hInstance = GetModuleHandle(0);
	_Windowclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	_Windowclass.hbrBackground = (HBRUSH)COLOR_GRAYTEXT;
	_Windowclass.lpfnWndProc = &WndProc;

	// Registrieren der Windowclass
	RegisterClassEx(&_Windowclass);

	// (optional) Windowrect erzeugen und anpassen
	RECT _WindowRect;
	_WindowRect.left = 100;
	_WindowRect.right = 900;
	_WindowRect.top = 100;
	_WindowRect.bottom = 700;

	AdjustWindowRect(&_WindowRect, WS_OVERLAPPEDWINDOW, false);

	// Fenster erzeugen
	m_Window = CreateWindowEx(0,
		"WindowBauplan",
		"Mein erstes Fenster",
		WS_OVERLAPPEDWINDOW,
		_WindowRect.left,
		_WindowRect.top,
		_WindowRect.right - _WindowRect.left,
		_WindowRect.bottom - _WindowRect.top,
		NULL,
		NULL,
		GetModuleHandle(0),
		NULL);


	// Fenster anzeigen
	ShowWindow(m_Window, SW_SHOWDEFAULT);
}


void System::InitGraphics()
{
	// Swapchain and Device
	D3D_FEATURE_LEVEL _FeatureLevel = D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_11_0;
	D3D_FEATURE_LEVEL _FeatureLevelOut;


	DXGI_SWAP_CHAIN_DESC _SCD;
	ZeroMemory(&_SCD, sizeof(_SCD));

	_SCD.BufferCount = 1;
	_SCD.BufferDesc.Format = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
	_SCD.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	_SCD.OutputWindow = m_Window;
	_SCD.Windowed = true;
	_SCD.SampleDesc.Count = 4;

	HRESULT _Result = D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE::D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		0, //D3D11_CREATE_DEVICE_FLAG::D3D11_CREATE_DEVICE_DEBUG
		&_FeatureLevel, 1,
		D3D11_SDK_VERSION,
		&_SCD,
		&m_pSwapChain,
		&m_pGraphicsDevice,
		&_FeatureLevelOut,
		&m_pDeviceContext);

	ID3D11Texture2D* m_pSwapChainBuffer;
	m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&m_pSwapChainBuffer);

	m_pGraphicsDevice->CreateRenderTargetView(m_pSwapChainBuffer, nullptr, &m_pScreen);

	D3D11_DEPTH_STENCIL_VIEW_DESC _DSVDesc;
	ZeroMemory( &_DSVDesc, sizeof(_DSVDesc) );


	_DSVDesc.Format = DXGI_FORMAT::DXGI_FORMAT_D32_FLOAT;
	_DSVDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;

	ID3D11Texture2D* _pDepthBufferResource = nullptr;

	D3D11_TEXTURE2D_DESC _T2DDesc;
	ZeroMemory( &_T2DDesc, sizeof(_T2DDesc));

	_T2DDesc.ArraySize = 1;
	_T2DDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_DEPTH_STENCIL;
	_T2DDesc.Format = DXGI_FORMAT::DXGI_FORMAT_D32_FLOAT;
	_T2DDesc.Height = 600;
	_T2DDesc.Width = 800;
	_T2DDesc.MipLevels = 1;
	_T2DDesc.SampleDesc.Count = 4;


	m_pGraphicsDevice->CreateTexture2D( &_T2DDesc, nullptr, &_pDepthBufferResource );

	m_pGraphicsDevice->CreateDepthStencilView( _pDepthBufferResource, &_DSVDesc, &m_pDepthStencil );

	m_pDeviceContext->OMSetRenderTargets(1, &m_pScreen, m_pDepthStencil);

	

	D3D11_VIEWPORT _Viewport;
	_Viewport.TopLeftX = 0;
	_Viewport.TopLeftY = 0;
	_Viewport.Height = 600;
	_Viewport.Width = 800;

	_Viewport.MinDepth = 0;
	_Viewport.MaxDepth = 1;

	m_pDeviceContext->RSSetViewports(1, &_Viewport);

}

void System::Init()
{
	InitWindow();
	InitGraphics();

	m_pInput = new Input();
}


int System::Run(IScene* p_pScene)
{
	p_pScene->Init(m_pGraphicsDevice, m_pDeviceContext);

	int QuitCode = 0;
	bool Running = true;
	MSG _msg;
	int Frames = 0;

	clock_t _Time = clock();

	while (Running)
	{
		// Fenster Updaten
		while (PeekMessage(&_msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&_msg);
			DispatchMessage(&_msg);

			if (_msg.message == WM_QUIT)
			{
				QuitCode = _msg.wParam;
				Running = false;
				return QuitCode;
			}
		}

		m_pInput->Update();

		clock_t _NewTime = clock();
		clock_t _Delta = _NewTime - _Time;
		float DeltaTime = (float)_Delta / CLOCKS_PER_SEC;
		_Time = _NewTime;
		// Scene Updaten
		p_pScene->Update(DeltaTime);

		Frames++;

		float _Color[4];
		_Color[0] = 0.3; // (rand() % 256) / 256.0f;
		_Color[1] = 0.3; // (rand() % 256) / 256.0f;
		_Color[2] = 0.3; // (rand() % 256) / 256.0f;
		_Color[3] = 1;

		m_pDeviceContext->ClearRenderTargetView(m_pScreen, _Color);
		m_pDeviceContext->ClearDepthStencilView( m_pDepthStencil, D3D11_CLEAR_FLAG::D3D11_CLEAR_DEPTH, 1.0f, 0 );

		// Scene Rendern
		p_pScene->Render(m_pGraphicsDevice, m_pDeviceContext);

		m_pSwapChain->Present(0, 0);

	}

	// Resourcen wieder freigeben

	return QuitCode;
}