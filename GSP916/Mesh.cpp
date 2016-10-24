#include "Mesh.h"


void Mesh::Init(int p_VertexCount, int p_ByteSizePerVertex, int p_IndexCount, ID3D11Device* p_pDevice, ID3D11DeviceContext* p_pDevCon)
{
	m_IndexCount = p_IndexCount;
	m_VertexCount = p_VertexCount;
	m_ByteSizePerVertex = p_ByteSizePerVertex;

	m_pDevice = p_pDevice;
	m_pDevCon = p_pDevCon;

	// VertexBuffer erstellen
	D3D11_BUFFER_DESC _VBufferDesc;
	ZeroMemory(&_VBufferDesc, sizeof(_VBufferDesc));

	_VBufferDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER;
	_VBufferDesc.ByteWidth = m_VertexCount * m_ByteSizePerVertex;
	_VBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
	_VBufferDesc.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;

	m_pDevice->CreateBuffer(&_VBufferDesc, nullptr, &m_pVertexBuffer);

	// IndexBuffer erstellen
	D3D11_BUFFER_DESC _IBufferDesc;
	ZeroMemory(&_IBufferDesc, sizeof(_IBufferDesc));

	_IBufferDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_INDEX_BUFFER;
	_IBufferDesc.ByteWidth = m_IndexCount * sizeof(unsigned int);
	_IBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
	_IBufferDesc.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;

	m_pDevice->CreateBuffer(&_IBufferDesc, nullptr, &m_pIndexBuffer);
}

void Mesh::SetData(void* VertexData, void* IndexData)
{
	D3D11_MAPPED_SUBRESOURCE _VMSR;

	m_pDevCon->Map(m_pVertexBuffer, 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &_VMSR);
	memcpy(_VMSR.pData, VertexData, m_VertexCount * m_ByteSizePerVertex);
	m_pDevCon->Unmap(m_pVertexBuffer, 0);


	D3D11_MAPPED_SUBRESOURCE _IMSR;

	m_pDevCon->Map(m_pIndexBuffer, 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &_IMSR);
	memcpy(_IMSR.pData, IndexData, m_IndexCount * sizeof(unsigned int));
	m_pDevCon->Unmap(m_pIndexBuffer, 0);

}


void Mesh::Render()
{
	UINT _Stride = m_ByteSizePerVertex;
	UINT _Offset = 0;

	m_pDevCon->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &_Stride, &_Offset);
	m_pDevCon->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT::DXGI_FORMAT_R32_UINT, 0);
	m_pDevCon->DrawIndexed(36, 0, 0);
}

void Mesh::AddInputLayoutEntry(char* p_SemanticName, int p_SemanticIndex, int p_ByteOffset, DXGI_FORMAT p_Format)
{

}

void Mesh::FinalizeInputLayout()
{

}