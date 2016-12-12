#include "DirectionalLight.h"

DirectionalLight::DirectionalLight()
{
	m_pShadowMap = nullptr;
}

void DirectionalLight::Init(ID3D11Device* p_pDevice, ID3D11DeviceContext* p_pDevCon)
{
	m_pShadowMap = new ShadowMap();
	m_pShadowMap->Init(p_pDevice, p_pDevCon, 800, 800);
}


ShadowMap* DirectionalLight::GetShadowMap()
{
	return m_pShadowMap;
}

void DirectionalLight::SetDirection(XMFLOAT3 p_Direction)
{
	m_Direction = p_Direction;

	float Distance = sqrt(m_Direction.x * m_Direction.x + m_Direction.y * m_Direction.y + m_Direction.z * m_Direction.z);

	m_Direction.x = m_Direction.x / Distance;
	m_Direction.y = m_Direction.y / Distance;
	m_Direction.z = m_Direction.z / Distance;
}

XMFLOAT3 DirectionalLight::GetDirection()
{
	return m_Direction;
}


XMMATRIX DirectionalLight::GetViewProjectionMatrix()
{
	XMMATRIX _Projection = XMMatrixOrthographicLH(6, 6, 0, 10);
	
	XMFLOAT3 LookAt = XMFLOAT3(2, 0, 2);
	XMFLOAT3 LightPos = XMFLOAT3(m_Direction.x * 5 + LookAt.x, m_Direction.y * 5 + LookAt.y, m_Direction.z * 5 + LookAt.z);
	XMFLOAT3 LightUp = XMFLOAT3(0, 1, 0);

	XMVECTOR Direction = XMLoadFloat3(&m_Direction);
	XMVECTOR Up = XMLoadFloat3(&LightUp);
	
	XMMATRIX View = XMMatrixLookAtLH(XMLoadFloat3(&LightPos), XMLoadFloat3(&LookAt), XMLoadFloat3(&LightUp));

	XMMATRIX VP = View * _Projection;

	return VP;
}