#pragma once
#include "d3dinclude.h"

#include "Camera.h"
#include "Transform.h"
#include "Texture.h"
#include "Mesh.h"
#include "Material.h"
#include "MeshRenderer.h"

class First3DObject
{
public:
	First3DObject();

	void Init(ID3D11Device* p_pDevice, ID3D11DeviceContext* p_pDevCon);

	void Update(float p_DeltaTime);

	void Render(Camera* p_pCamera);

private:
	ID3D11Device* m_pDevice;
	ID3D11DeviceContext* m_pDevCon;

	Texture* m_pMainTexture;

	Transform* m_pTransform;

	float CurrentAngle;

	MeshRenderer* m_pMeshRenderer;
};