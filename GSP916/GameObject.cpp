#include "GameObject.h"


void GameObject::Init(ID3D11Device* p_pDevice, ID3D11DeviceContext* p_pDevCon, Mesh* p_pMesh, Material* p_pMaterial)
{
	m_pTransform = new Transform(XMFLOAT3(0, 0, 0), XMFLOAT3(0, 0, 0), XMFLOAT3(1, 1, 1));

	m_pRenderer = new MeshRenderer();
	m_pRenderer->Init(p_pMaterial, p_pMesh, p_pDevice, p_pDevCon);
	m_pRenderer->SetRenderTransform(m_pTransform);
}

void GameObject::Render(Camera* p_pCamera)
{
	m_pRenderer->Render(p_pCamera);
}