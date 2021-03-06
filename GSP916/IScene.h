#pragma once
#include "d3dinclude.h"

class IScene
{
public:
	virtual void Init(ID3D11Device* p_pDevice, ID3D11DeviceContext* p_pDevCon, ID3D11RenderTargetView* m_pScreen, ID3D11DepthStencilView* m_pDepthStencil) = 0;
	
	virtual void Update(float p_DeltaTime) = 0;
	virtual void Render(ID3D11Device* p_pDevice, ID3D11DeviceContext* p_pDevCon) = 0;
};