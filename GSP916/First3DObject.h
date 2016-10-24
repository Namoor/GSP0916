#pragma once
#include "d3dinclude.h"

#include "Camera.h"
#include "Transform.h"
#include "Texture.h"
#include "Mesh.h"

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

	ID3D11Buffer* m_pVertexBuffer;
	ID3D11Buffer* m_pIndexBuffer;
	ID3D11VertexShader* m_pVertexShader;
	ID3D11PixelShader* m_pPixelShader;
	ID3D11InputLayout* m_pInputLayout;
	ID3D11Buffer* m_pMatrixBuffer;

	Texture* m_pMainTexture;

	Transform* m_pTransform;

	float CurrentAngle;

	Mesh* m_pMesh;
};