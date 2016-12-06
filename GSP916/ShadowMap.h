#pragma once

#include "d3dinclude.h"

#include "Texture.h"

class ShadowMap
{
public:
	ShadowMap();
	void Init(ID3D11Device* p_pDevice, ID3D11DeviceContext* p_pDevCon, int p_SixeX, int p_SizeY);

	Texture* GetTextureView();
	
	void Bind();

private:
	ID3D11DepthStencilView* m_pDepthStencilView;


	ID3D11Device* m_pDevice;
	ID3D11DeviceContext* m_pDevCon;

	Texture* m_pTextureView;

	D3D11_VIEWPORT m_Viewport;
};