#include "ShadingDemo.h"

struct ShadingDemo_Vertex
{
	XMFLOAT3 Position;
};

struct ShadingDemo_MatrixConstantBuffer
{
	XMMATRIX ModelViewProjection;
};

ShadingDemo::ShadingDemo()
{
	m_pDevCon = nullptr;
	m_pDevice = nullptr;

	m_pIndexBuffer = nullptr;
	m_pVertexBuffer = nullptr;
	m_pInputLayout = nullptr;

	m_pMatrixConstantBuffer = nullptr;

	m_pPixelShader = nullptr;
	m_pVertexShader = nullptr;
}

void ShadingDemo::Init(ID3D11Device* p_pDevice, ID3D11DeviceContext* p_pDevCon, int p_Subdivisions)
{
	m_pDevCon = p_pDevCon;
	m_pDevice = p_pDevice;

	int _VerticesPerSide = p_Subdivisions + 1;

	// VertexBuffer
	D3D11_BUFFER_DESC _VBDesc;
	ZeroMemory(&_VBDesc, sizeof(_VBDesc));

	_VBDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER;
	_VBDesc.ByteWidth = ((p_Subdivisions + 1) * (p_Subdivisions + 1)) * sizeof(ShadingDemo_Vertex);
	_VBDesc.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
	_VBDesc.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;


	m_pDevice->CreateBuffer(&_VBDesc, nullptr, &m_pVertexBuffer);

	ShadingDemo_Vertex* _pVertices = (ShadingDemo_Vertex*)malloc(((p_Subdivisions + 1) * (p_Subdivisions + 1)) * sizeof(ShadingDemo_Vertex));

	for (int x = 0; x < _VerticesPerSide; x++)
		for (int y = 0; y < _VerticesPerSide; y++)
		{
			_pVertices[y * _VerticesPerSide + x].Position = XMFLOAT3(x, 0, y);
		}


	D3D11_MAPPED_SUBRESOURCE _VBMSR;
	m_pDevCon->Map(m_pVertexBuffer, 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &_VBMSR);
	memcpy(_VBMSR.pData, _pVertices, (_VerticesPerSide * _VerticesPerSide) * sizeof(ShadingDemo_Vertex));
	m_pDevCon->Unmap(m_pVertexBuffer, 0);

	// IndexBuffer
	int _IndexCount = p_Subdivisions * p_Subdivisions * 2 * 3;

	D3D11_BUFFER_DESC _IBDesc;
	ZeroMemory(&_IBDesc, sizeof(_IBDesc));

	_IBDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_INDEX_BUFFER;
	_IBDesc.ByteWidth = _IndexCount * sizeof(unsigned int);
	_IBDesc.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
	_IBDesc.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;

	m_pDevice->CreateBuffer(&_IBDesc, nullptr, &m_pIndexBuffer);

	unsigned int* _pIndices = (unsigned int*) malloc(_IndexCount * sizeof(unsigned int));

	int _CurrentIndex = 0;

	for (int x = 0; x < p_Subdivisions; x++)
	{
		for (int y = 0; y < p_Subdivisions; y++)
		{
			// Ein Quad mit indices füllen

			unsigned int _TL = x + _VerticesPerSide * y;
			unsigned int _TR = _TL + 1;
			unsigned int _BL = _TL + _VerticesPerSide;
			unsigned int _BR = _BL + 1;

			_pIndices[_CurrentIndex++] = _TL;
			_pIndices[_CurrentIndex++] = _TR;
			_pIndices[_CurrentIndex++] = _BL;

			_pIndices[_CurrentIndex++] = _BL;
			_pIndices[_CurrentIndex++] = _TR;
			_pIndices[_CurrentIndex++] = _BR;
		}
	}

	D3D11_MAPPED_SUBRESOURCE _IBMSR;
	

	m_pDevCon->Map(m_pIndexBuffer, 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &_IBMSR);
	memcpy(_IBMSR.pData, _pIndices, _IndexCount * sizeof(unsigned int));
	m_pDevCon->Unmap(m_pIndexBuffer, 0);




	// Matrix Constant Buffer

	D3D11_BUFFER_DESC _CBDesc;
	ZeroMemory(&_CBDesc, sizeof(_CBDesc));

	_CBDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_CONSTANT_BUFFER;
	_CBDesc.ByteWidth = sizeof(ShadingDemo_MatrixConstantBuffer);
	_CBDesc.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
	_CBDesc.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;

	m_pDevice->CreateBuffer(&_CBDesc, nullptr, &m_pMatrixConstantBuffer);

	// VertexShader

	// PixelShader

	// InputLayout

	int asd = 0;
}

void ShadingDemo::Update(float p_DeltaTime)
{

}

void ShadingDemo::Render(Camera* p_pCamera)
{
	ShadingDemo_MatrixConstantBuffer _NewMatrixData;

	// Struct mit daten füllen
	_NewMatrixData.ModelViewProjection = p_pCamera->GetViewMatrix() * p_pCamera->GetProjectionMatrix();


	// Struct auf die graphikkarte übertragen
	D3D11_MAPPED_SUBRESOURCE _CBMSR;
	m_pDevCon->Map(m_pMatrixConstantBuffer, 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &_CBMSR);
	memcpy(_CBMSR.pData, &_NewMatrixData, sizeof(ShadingDemo_MatrixConstantBuffer));
	m_pDevCon->Unmap(m_pMatrixConstantBuffer, 0);


}