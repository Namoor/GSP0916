#pragma once

#include <Windows.h>
#include "d3dinclude.h"
#include "IScene.h"

class System
{
public:

	void Init();
	int Run(IScene* p_pScene);

private:
	void InitWindow();
	void InitGraphics();

	// Window
	HWND m_Window;

	// DirectX
	IDXGISwapChain* m_pSwapChain;

	ID3D11Device* m_pGraphicsDevice; // Buffer erstellen/Meshes generieren
	ID3D11DeviceContext* m_pDeviceContext; // Würfel Rendern

	ID3D11RenderTargetView* m_pScreen;
};
