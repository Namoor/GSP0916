#pragma once
#include "Transform.h"
#include "MeshRenderer.h"
#include "Camera.h"

class GameObject
{
public:
	void Init(ID3D11Device* p_pDevice, ID3D11DeviceContext* p_pDevCon, Mesh* p_pMesh, Material* p_pMaterial);

	void Render(Camera* p_pCamera);

public:
	MeshRenderer* m_pRenderer;
	Transform* m_pTransform;

};