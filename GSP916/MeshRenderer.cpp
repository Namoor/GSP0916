#include "MeshRenderer.h"

struct MatrixConstantBuffer
{
	XMMATRIX WorldViewProjectionMatrix;
	XMMATRIX TransInvWorldMatrix;
};

void MeshRenderer::Init(Material* p_pMaterial, Mesh* p_pMesh, ID3D11Device* p_pDevice, ID3D11DeviceContext* p_pDevCon)
{
	m_pDevice = p_pDevice;
	m_pDevCon = p_pDevCon;

	m_pMaterial = p_pMaterial;
	m_pMesh = p_pMesh;

	D3D11_BUFFER_DESC _MCBD;
	ZeroMemory(&_MCBD, sizeof(_MCBD));

	_MCBD.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_CONSTANT_BUFFER;
	_MCBD.ByteWidth = sizeof(MatrixConstantBuffer);
	_MCBD.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
	_MCBD.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;

	m_pDevice->CreateBuffer(&_MCBD, nullptr, &m_pMatrixConstantBuffer);
}

void MeshRenderer::Render(Camera* p_pCamera)
{
	if (m_pTransform != nullptr)
	{
		MatrixConstantBuffer _Data;
		_Data.WorldViewProjectionMatrix = m_pTransform->GetMatrix() * p_pCamera->GetViewMatrix() * p_pCamera->GetProjectionMatrix();
		_Data.TransInvWorldMatrix = m_pTransform->GetInvertTranspose();

		D3D11_MAPPED_SUBRESOURCE _MSR;
		m_pDevCon->Map(m_pMatrixConstantBuffer, 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &_MSR);
		memcpy(_MSR.pData, &_Data, sizeof(MatrixConstantBuffer));
		m_pDevCon->Unmap(m_pMatrixConstantBuffer, 0);
	}

	m_pMaterial->Bind();

	m_pDevCon->VSSetConstantBuffers(0, 1, &m_pMatrixConstantBuffer);

	m_pMesh->Render();
}


void MeshRenderer::SetRenderTransform(Transform* p_pTransform)
{
	m_pTransform = p_pTransform;
}