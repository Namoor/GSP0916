#pragma once

#include "d3dinclude.h"
#include "Texture.h"


class PostProcessingRenderTarget
{
public:
	void Init(ID3D11Device* p_pDevice, ID3D11DeviceContext* p_pDevCon, int p_SizeX, int p_SizeY);
	void Bind();
	Texture* GetTexture();


private:
	ID3D11DepthStencilView* m_pDSV;
	ID3D11RenderTargetView* m_pRTV;

	Texture* m_pTexture;

	D3D11_VIEWPORT m_ViewPort;

	ID3D11Device* m_pDevice;
	ID3D11DeviceContext* m_pDevCon;
};
