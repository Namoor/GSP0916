#include "First3DObject.h"



struct First3DVertex
{
	XMFLOAT3 Position;
	XMFLOAT4 Color;
	XMFLOAT2 UV;
	XMFLOAT3 Normal;
};

struct MatrixBuffer
{
	XMMATRIX MVP;
	XMMATRIX M;
};

First3DObject::First3DObject()
{
	m_pDevCon = nullptr;
	m_pDevice = nullptr;
}

void First3DObject::Init(ID3D11Device* p_pDevice, ID3D11DeviceContext* p_pDevCon)
{
 	m_pDevCon = p_pDevCon;
	m_pDevice = p_pDevice;

	m_pTransform = new Transform(XMFLOAT3(2, 0, 0), XMFLOAT3(0, 0, 0), XMFLOAT3(1, 1, 1));
	m_pMainTexture = new Texture( p_pDevice, "HappyFace.bmp" );

	// Mesh
	Mesh* _pMesh = Mesh::CreateCubeMesh(m_pDevice, m_pDevCon);

	// Material
	Material* _pMaterial = new Material();
	_pMaterial->LoadShader(L"First3DShader.hlsl", "VShader", "PShader", m_pDevice, m_pDevCon);

	_pMaterial->AddInputLayoutEntry("POSITION", 0, 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT);
	_pMaterial->AddInputLayoutEntry("COLOR", 0, 12, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT);
	_pMaterial->AddInputLayoutEntry("TEXCOORD", 0, 28, DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT);
	_pMaterial->AddInputLayoutEntry("NORMAL", 0, 36, DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT);

	_pMaterial->FinalizeInputLayout();

	m_pMeshRenderer = new MeshRenderer();
	m_pMeshRenderer->Init(_pMaterial, _pMesh, m_pDevice, m_pDevCon);

	m_pMeshRenderer->SetRenderTransform(m_pTransform);

	CurrentAngle = 0;
}


void First3DObject::Update(float p_DeltaTime)
{
	m_pTransform->RotateAround(XMFLOAT3(0, 1, 0), p_DeltaTime * 20, true);
}

void First3DObject::Render(Camera* p_pCamera)
{

	// Shader Resources

	ID3D11ShaderResourceView* _pSRV = m_pMainTexture->GetSRV();
	m_pDevCon->PSSetShaderResources( 0, 1, &_pSRV );

	m_pMeshRenderer->Render(p_pCamera);
}