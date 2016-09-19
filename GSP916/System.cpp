#include "System.h"

#include <Windows.h>

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

	m_pDeviceContext->OMSetRenderTargets(1, &m_pScreen, nullptr);

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
}


int System::Run(IScene* p_pScene)
{
	p_pScene->Init(m_pGraphicsDevice, m_pDeviceContext);

	int QuitCode = 0;
	bool Running = true;
	MSG _msg;
	int Frames = 0;

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
			}
		}



		// Scene Updaten
		p_pScene->Update();

		Frames++;

		float _Color[4];
		_Color[0] = 1; // (rand() % 256) / 256.0f;
		_Color[1] = 0; // (rand() % 256) / 256.0f;
		_Color[2] = 0; // (rand() % 256) / 256.0f;
		_Color[3] = 1;

		m_pDeviceContext->ClearRenderTargetView(m_pScreen, _Color);

		// Scene Rendern
		p_pScene->Render(m_pGraphicsDevice, m_pDeviceContext);

		m_pSwapChain->Present(0, 0);

	}

	// Resourcen wieder freigeben

	return QuitCode;
}