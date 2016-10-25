#pragma once

#include "Material.h"
#include "Mesh.h"
#include "Transform.h"
#include "Camera.h"

class MeshRenderer
{
public:
	void Init(Material* p_pMaterial, Mesh* p_pMesh, ID3D11Device* p_pDevice, ID3D11DeviceContext* p_pDevCon);
	void SetRenderTransform(Transform* p_pTransform);


	void Render(Camera* p_pCamera);
private:
	Mesh* m_pMesh;
	Material* m_pMaterial;

	Transform* m_pTransform;
	ID3D11Buffer* m_pMatrixConstantBuffer;

	ID3D11Device* m_pDevice;
	ID3D11DeviceContext* m_pDevCon;
};