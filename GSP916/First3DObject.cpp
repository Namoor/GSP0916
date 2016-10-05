#include "First3DObject.h"


struct First3DVertex
{
	XMFLOAT3 Position;
	XMFLOAT4 Color;
};

First3DObject::First3DObject()
{
	m_pDevCon = nullptr;
	m_pDevice = nullptr;
	m_pIndexBuffer = nullptr;
	m_pVertexBuffer = nullptr;
	m_pInputLayout = nullptr;
	m_pVertexShader = nullptr;
	m_pPixelShader = nullptr;
}

void First3DObject::Init(ID3D11Device* p_pDevice, ID3D11DeviceContext* p_pDevCon)
{
	m_pDevCon = p_pDevCon;
	m_pDevice = p_pDevice;

	// VertexBuffer
	D3D11_BUFFER_DESC _VBDesc;
	ZeroMemory(&_VBDesc, sizeof(_VBDesc));

	_VBDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER;
	_VBDesc.ByteWidth = 6 * sizeof(First3DVertex);
	_VBDesc.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
	_VBDesc.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;

	m_pDevice->CreateBuffer(&_VBDesc, nullptr, &m_pVertexBuffer);

	First3DVertex _Vertices[6];
	_Vertices[0].Position = XMFLOAT3(-0.5f, -0.5f, 0);
	_Vertices[0].Color = XMFLOAT4(0, 1, 0, 1);
	_Vertices[1].Position = XMFLOAT3(-0.5f, 0.5f, 0);
	_Vertices[1].Color = XMFLOAT4(0, 1, 0, 1);

	_Vertices[2].Position = XMFLOAT3(0.5f, -0.5f, 0);
	_Vertices[2].Color = XMFLOAT4(1, 1, 1, 1);
	_Vertices[3].Position = XMFLOAT3(0.5f, 0.5f, 0);
	_Vertices[3].Color = XMFLOAT4(1, 1, 1, 1);

	_Vertices[4].Position = XMFLOAT3(1, -0.5f, 1);
	_Vertices[4].Color = XMFLOAT4(0, 0, 1, 1);
	_Vertices[5].Position = XMFLOAT3(1, 0.5f, 1);
	_Vertices[5].Color = XMFLOAT4(0, 0, 1, 1);

	D3D11_MAPPED_SUBRESOURCE _VMSR;

	m_pDevCon->Map(m_pVertexBuffer, 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &_VMSR);
	memcpy(_VMSR.pData, _Vertices, 6 * sizeof(First3DVertex));
	m_pDevCon->Unmap(m_pVertexBuffer, 0);

	// IndexBuffer
	D3D11_BUFFER_DESC _IBDesc;
	ZeroMemory(&_IBDesc, sizeof(_IBDesc));

	_IBDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_INDEX_BUFFER;
	_IBDesc.ByteWidth = 4 * 3 * sizeof(unsigned int);
	_IBDesc.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
	_IBDesc.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;

	m_pDevice->CreateBuffer(&_IBDesc, nullptr, &m_pIndexBuffer);

	unsigned int _Indices[12];

	_Indices[0] = 0; // Triangle 0
	_Indices[1] = 1;
	_Indices[2] = 2;

	_Indices[3] = 2; // Triangle 1
	_Indices[4] = 1;
	_Indices[5] = 3;

	_Indices[6] = 2; // Triangle 2
	_Indices[7] = 3;
	_Indices[8] = 4;

	_Indices[9] = 4; // Triangle 3
	_Indices[10] = 3;
	_Indices[11] = 5;

	D3D11_MAPPED_SUBRESOURCE _IMSR;

	m_pDevCon->Map(m_pIndexBuffer, 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &_IMSR);
	memcpy(_IMSR.pData, _Indices, 4 * 3 * sizeof(unsigned int));
	m_pDevCon->Unmap(m_pIndexBuffer, 0);

	// VertexShader
	ID3DBlob* _pVShaderCode;
	ID3DBlob* _pPShaderCode;
	ID3DBlob* _pError;

	if (D3DCompileFromFile(L"First3DShader.hlsl", nullptr, nullptr, "VShader", "vs_5_0", 0, 0, &_pVShaderCode, &_pError) != S_OK)
	{
		MessageBox(0, (char*)_pError->GetBufferPointer(), "Fehler im VertexShader", MB_OK);

		PostQuitMessage(0);
		return;
	}

	m_pDevice->CreateVertexShader(_pVShaderCode->GetBufferPointer(), _pVShaderCode->GetBufferSize(), nullptr, &m_pVertexShader);


	// PixelShader
	if (D3DCompileFromFile(L"First3DShader.hlsl", nullptr, nullptr, "PShader", "ps_5_0", 0, 0, &_pPShaderCode, &_pError) != S_OK)
	{
		MessageBox(0, (char*)_pError->GetBufferPointer(), "Fehler im PixelShader", MB_OK);

		PostQuitMessage(0);
		return;
	}

	m_pDevice->CreatePixelShader(_pPShaderCode->GetBufferPointer(), _pPShaderCode->GetBufferSize(), nullptr, &m_pPixelShader);



	// InputLayout
	D3D11_INPUT_ELEMENT_DESC _IED[2];

	// Position
	_IED[0].InputSlot = 0;
	_IED[0].InstanceDataStepRate = 0;
	_IED[0].InputSlotClass = D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA;

	_IED[0].AlignedByteOffset = 0;
	_IED[0].Format = DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT;

	_IED[0].SemanticName = "POSITION";
	_IED[0].SemanticIndex = 0;

	// Color
	_IED[1].InputSlot = 0;
	_IED[1].InstanceDataStepRate = 0;
	_IED[1].InputSlotClass = D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA;

	_IED[1].AlignedByteOffset = 12;
	_IED[1].Format = DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT;

	_IED[1].SemanticName = "COLOR";
	_IED[1].SemanticIndex = 0;

	m_pDevice->CreateInputLayout(_IED, 2, _pVShaderCode->GetBufferPointer(), _pVShaderCode->GetBufferSize(), &m_pInputLayout);
}

void First3DObject::Render()
{
	unsigned int _Stride = sizeof(First3DVertex);
	unsigned int _Offset = 0;

	m_pDevCon->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &_Stride, &_Offset);
	m_pDevCon->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT::DXGI_FORMAT_R32_UINT, 0);

	m_pDevCon->IASetInputLayout(m_pInputLayout);
	m_pDevCon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	m_pDevCon->VSSetShader(m_pVertexShader, nullptr, 0);
	m_pDevCon->PSSetShader(m_pPixelShader, nullptr, 0);

	m_pDevCon->DrawIndexed(12, 0, 0);
}