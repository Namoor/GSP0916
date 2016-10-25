#include "Mesh.h"

struct BaseMeshVertex
{
	XMFLOAT3 Position;
	XMFLOAT4 Color;
	XMFLOAT2 UV;
	XMFLOAT3 Normal;
};

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

	m_pDevCon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	m_pDevCon->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &_Stride, &_Offset);
	m_pDevCon->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT::DXGI_FORMAT_R32_UINT, 0);
	m_pDevCon->DrawIndexed(36, 0, 0);
}

Mesh* Mesh::CreateCubeMesh(ID3D11Device* p_pDevice, ID3D11DeviceContext* p_pDevCon)
{
	Mesh* _Result = new Mesh();
	_Result->Init(24, sizeof(BaseMeshVertex), 36, p_pDevice, p_pDevCon);

#pragma region Vertices

	BaseMeshVertex _Vertices[24];

	// Vorne
	_Vertices[0].Position = XMFLOAT3(-0.5f, -0.5f, -0.5f);
	_Vertices[0].Color = XMFLOAT4(0, 0, 0, 1);
	_Vertices[0].UV = XMFLOAT2(0, 1);
	_Vertices[0].Normal = XMFLOAT3(0, 0, -1);
	_Vertices[1].Position = XMFLOAT3(-0.5f, 0.5f, -0.5f);
	_Vertices[1].Color = XMFLOAT4(0, 1, 0, 1);
	_Vertices[1].UV = XMFLOAT2(0, 0);
	_Vertices[1].Normal = XMFLOAT3(0, 0, -1);

	_Vertices[2].Position = XMFLOAT3(0.5f, 0.5f, -0.5f);
	_Vertices[2].Color = XMFLOAT4(1, 1, 0, 1);
	_Vertices[2].UV = XMFLOAT2(1, 0);
	_Vertices[2].Normal = XMFLOAT3(0, 0, -1);
	_Vertices[3].Position = XMFLOAT3(0.5f, -0.5f, -0.5f);
	_Vertices[3].Color = XMFLOAT4(1, 0, 0, 1);
	_Vertices[3].UV = XMFLOAT2(1, 1);
	_Vertices[3].Normal = XMFLOAT3(0, 0, -1);

	// Rechts
	_Vertices[4].Position = XMFLOAT3(0.5f, -0.5f, -0.5f);
	_Vertices[4].Color = XMFLOAT4(1, 0, 0, 1);
	_Vertices[4].UV = XMFLOAT2(0, 1);
	_Vertices[4].Normal = XMFLOAT3(1, 0, 0);
	_Vertices[5].Position = XMFLOAT3(0.5f, 0.5f, -0.5f);
	_Vertices[5].Color = XMFLOAT4(1, 1, 0, 1);
	_Vertices[5].UV = XMFLOAT2(0, 0);
	_Vertices[5].Normal = XMFLOAT3(1, 0, 0);

	_Vertices[6].Position = XMFLOAT3(0.5f, 0.5f, 0.5f);
	_Vertices[6].Color = XMFLOAT4(1, 1, 1, 1);
	_Vertices[6].UV = XMFLOAT2(1, 0);
	_Vertices[6].Normal = XMFLOAT3(1, 0, 0);
	_Vertices[7].Position = XMFLOAT3(0.5f, -0.5f, 0.5f);
	_Vertices[7].Color = XMFLOAT4(1, 0, 1, 1);
	_Vertices[7].UV = XMFLOAT2(1, 1);
	_Vertices[7].Normal = XMFLOAT3(1, 0, 0);

	// Oben
	_Vertices[8].Position = XMFLOAT3(-0.5f, 0.5f, -0.5f);
	_Vertices[8].Color = XMFLOAT4(0, 1, 0, 1);
	_Vertices[8].UV = XMFLOAT2(0, 1);
	_Vertices[8].Normal = XMFLOAT3(0, 1, 0);
	_Vertices[9].Position = XMFLOAT3(-0.5f, 0.5f, 0.5f);
	_Vertices[9].Color = XMFLOAT4(0, 1, 1, 1);
	_Vertices[9].UV = XMFLOAT2(0, 0);
	_Vertices[9].Normal = XMFLOAT3(0, 1, 0);

	_Vertices[10].Position = XMFLOAT3(0.5f, 0.5f, 0.5f);
	_Vertices[10].Color = XMFLOAT4(1, 1, 1, 1);
	_Vertices[10].UV = XMFLOAT2(1, 0);
	_Vertices[10].Normal = XMFLOAT3(0, 1, 0);
	_Vertices[11].Position = XMFLOAT3(0.5f, 0.5f, -0.5f);
	_Vertices[11].Color = XMFLOAT4(1, 1, 0, 1);
	_Vertices[11].UV = XMFLOAT2(1, 1);
	_Vertices[11].Normal = XMFLOAT3(0, 1, 0);

	// Hinten
	_Vertices[12].Position = XMFLOAT3(-0.5f, -0.5f, 0.5f);
	_Vertices[12].Color = XMFLOAT4(0, 0, 1, 1);
	_Vertices[12].UV = XMFLOAT2(1, 1);
	_Vertices[12].Normal = XMFLOAT3(0, 0, 1);
	_Vertices[13].Position = XMFLOAT3(-0.5f, 0.5f, 0.5f);
	_Vertices[13].Color = XMFLOAT4(0, 1, 1, 1);
	_Vertices[13].UV = XMFLOAT2(1, 0);
	_Vertices[13].Normal = XMFLOAT3(0, 0, 1);

	_Vertices[14].Position = XMFLOAT3(0.5f, 0.5f, 0.5f);
	_Vertices[14].Color = XMFLOAT4(1, 1, 1, 1);
	_Vertices[14].UV = XMFLOAT2(0, 0);
	_Vertices[14].Normal = XMFLOAT3(0, 0, 1);
	_Vertices[15].Position = XMFLOAT3(0.5f, -0.5f, 0.5f);
	_Vertices[15].Color = XMFLOAT4(1, 0, 1, 1);
	_Vertices[15].UV = XMFLOAT2(0, 1);
	_Vertices[15].Normal = XMFLOAT3(0, 0, 1);

	// Links
	_Vertices[16].Position = XMFLOAT3(-0.5f, -0.5f, -0.5f);
	_Vertices[16].Color = XMFLOAT4(0, 0, 0, 1);
	_Vertices[16].UV = XMFLOAT2(1, 1);
	_Vertices[16].Normal = XMFLOAT3(-1, 0, 0);
	_Vertices[17].Position = XMFLOAT3(-0.5f, 0.5f, -0.5f);
	_Vertices[17].Color = XMFLOAT4(0, 1, 0, 1);
	_Vertices[17].UV = XMFLOAT2(1, 0);
	_Vertices[17].Normal = XMFLOAT3(-1, 0, 0);

	_Vertices[18].Position = XMFLOAT3(-0.5f, 0.5f, 0.5f);
	_Vertices[18].Color = XMFLOAT4(0, 1, 1, 1);
	_Vertices[18].UV = XMFLOAT2(0, 0);
	_Vertices[18].Normal = XMFLOAT3(-1, 0, 0);
	_Vertices[19].Position = XMFLOAT3(-0.5f, -0.5f, 0.5f);
	_Vertices[19].Color = XMFLOAT4(0, 0, 1, 1);
	_Vertices[19].UV = XMFLOAT2(0, 1);
	_Vertices[19].Normal = XMFLOAT3(-1, 0, 0);
	// Unten
	_Vertices[20].Position = XMFLOAT3(-0.5f, -0.5f, -0.5f);
	_Vertices[20].Color = XMFLOAT4(0, 0, 0, 1);
	_Vertices[20].UV = XMFLOAT2(0, 0);
	_Vertices[20].Normal = XMFLOAT3(0, -1, 0);
	_Vertices[21].Position = XMFLOAT3(-0.5f, -0.5f, 0.5f);
	_Vertices[21].Color = XMFLOAT4(0, 0, 1, 1);
	_Vertices[21].UV = XMFLOAT2(0, 1);
	_Vertices[21].Normal = XMFLOAT3(0, -1, 0);

	_Vertices[22].Position = XMFLOAT3(0.5f, -0.5f, 0.5f);
	_Vertices[22].Color = XMFLOAT4(1, 0, 1, 1);
	_Vertices[22].UV = XMFLOAT2(1, 1);
	_Vertices[22].Normal = XMFLOAT3(0, -1, 0);
	_Vertices[23].Position = XMFLOAT3(0.5f, -0.5f, -0.5f);
	_Vertices[23].Color = XMFLOAT4(1, 0, 0, 1);
	_Vertices[23].UV = XMFLOAT2(1, 0);
	_Vertices[23].Normal = XMFLOAT3(0, -1, 0);

#pragma endregion

#pragma region Indices
	unsigned int _Indices[36];

	_Indices[0] = 0; // Front
	_Indices[1] = 1;
	_Indices[2] = 3;
	_Indices[3] = 3;
	_Indices[4] = 1;
	_Indices[5] = 2;

	_Indices[6] = 4; // Right
	_Indices[7] = 5;
	_Indices[8] = 7;
	_Indices[9] = 7;
	_Indices[10] = 5;
	_Indices[11] = 6;

	_Indices[12] = 8; // Top
	_Indices[13] = 9;
	_Indices[14] = 11;
	_Indices[15] = 11;
	_Indices[16] = 9;
	_Indices[17] = 10;

	_Indices[18] = 15; // Back
	_Indices[19] = 13;
	_Indices[20] = 12;
	_Indices[21] = 15;
	_Indices[22] = 14;
	_Indices[23] = 13;

	_Indices[24] = 19; // Left
	_Indices[25] = 17;
	_Indices[26] = 16;
	_Indices[27] = 19;
	_Indices[28] = 18;
	_Indices[29] = 17;

	_Indices[30] = 23; // Back
	_Indices[31] = 21;
	_Indices[32] = 20;
	_Indices[33] = 23;
	_Indices[34] = 22;
	_Indices[35] = 21;
	
#pragma endregion

	_Result->SetData(_Vertices, _Indices);

	return _Result;
}