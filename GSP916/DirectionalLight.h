#pragma once

#include "d3dinclude.h"
#include "ShadowMap.h"

class DirectionalLight
{
public:
	DirectionalLight();
	void Init(ID3D11Device* p_pDevice, ID3D11DeviceContext* p_pDevCon);

	void SetDirection(XMFLOAT3 p_Direction);

	XMFLOAT3 GetDirection();
	ShadowMap* GetShadowMap();

	XMMATRIX GetViewProjectionMatrix();

private:

	ShadowMap* m_pShadowMap;
	XMFLOAT3 m_Direction;
};