#include "PostProcessingEffect.h"

void PostProcessingEffect::Init(ID3D11Device* p_pDevice, ID3D11DeviceContext* p_pDevCon, wchar_t* ShaderFileName)
{
	m_pDevice = p_pDevice;
	m_pDevCon = p_pDevCon;


	// VertexBuffer
	D3D11_BUFFER_DESC _VBDesc;
	ZeroMemory(&_VBDesc, sizeof(_VBDesc));

	_VBDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER;
	_VBDesc.ByteWidth = 4 * sizeof(XMFLOAT3);
	_VBDesc.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
	_VBDesc.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;
	m_pDevice->CreateBuffer(&_VBDesc, nullptr, &m_pVertexBuffer);

	XMFLOAT3 _Vertices[4];
	_Vertices[0] = XMFLOAT3(-1, 1, 0.1f);
	_Vertices[1] = XMFLOAT3(1, 1, 0.1f);
	_Vertices[2] = XMFLOAT3(1, -1, 0.1f);
	_Vertices[3] = XMFLOAT3(-1, -1, 0.1f);

	D3D11_MAPPED_SUBRESOURCE _VBMSR;
	m_pDevCon->Map(m_pVertexBuffer, 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &_VBMSR);
	memcpy(_VBMSR.pData, _Vertices, 4 * sizeof(XMFLOAT3));
	m_pDevCon->Unmap(m_pVertexBuffer, 0);

	// IndexBuffer
	D3D11_BUFFER_DESC _IBDesc;
	ZeroMemory(&_IBDesc, sizeof(_IBDesc));

	_IBDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_INDEX_BUFFER;
	_IBDesc.ByteWidth = 6 * sizeof(UINT32);
	_IBDesc.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
	_IBDesc.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;

	m_pDevice->CreateBuffer(&_IBDesc, nullptr, &m_pIndexBuffer);

	unsigned int _Indices[6];
	_Indices[0] = 0;
	_Indices[1] = 1;
	_Indices[2] = 2;

	_Indices[3] = 2;
	_Indices[4] = 3;
	_Indices[5] = 0;

	D3D11_MAPPED_SUBRESOURCE _IBMSR;
	m_pDevCon->Map(m_pIndexBuffer, 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &_IBMSR);
	memcpy(_IBMSR.pData, _Indices, 6 * sizeof(unsigned int));
	m_pDevCon->Unmap(m_pIndexBuffer, 0);


	// VertexShader
	ID3DBlob* _pVertexShaderBlob;
	ID3DBlob* _pErrorMessage;

	HRESULT _Result = D3DCompileFromFile(ShaderFileName, nullptr, nullptr, "VShader", "vs_5_0", 0, 0, &_pVertexShaderBlob, &_pErrorMessage);
	if (_Result != S_OK)
	{
		MessageBox(0, (char*)_pErrorMessage->GetBufferPointer(), "Fehler im VertexShader", MB_OK);
		PostQuitMessage(0);
		return;
	}
	m_pDevice->CreateVertexShader(_pVertexShaderBlob->GetBufferPointer(), _pVertexShaderBlob->GetBufferSize(), nullptr, &m_pVertexShader);


	// PixelShader
	ID3DBlob* _pPixelShaderBlob;

	_Result = D3DCompileFromFile(ShaderFileName, nullptr, nullptr, "PShader", "ps_5_0", 0, 0, &_pPixelShaderBlob, &_pErrorMessage);
	if (_Result != S_OK)
	{
		MessageBox(0, (char*)_pErrorMessage->GetBufferPointer(), "Fehler im PixelShader", MB_OK);
		PostQuitMessage(0);
		return;
	}
	m_pDevice->CreatePixelShader(_pPixelShaderBlob->GetBufferPointer(), _pPixelShaderBlob->GetBufferSize(), nullptr, &m_pPixelShader);

	// Inputlayout
	D3D11_INPUT_ELEMENT_DESC _IEDs[1];
	ZeroMemory(_IEDs, sizeof(D3D11_INPUT_ELEMENT_DESC) * 1);

	_IEDs[0].AlignedByteOffset = 0;
	_IEDs[0].Format = DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT;

	_IEDs[0].SemanticName = "POSITION";
	_IEDs[0].SemanticIndex = 0;

	//_IEDs[0].InputSlot = 0;
	//_IEDs[0].InputSlotClass = D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA;
	//_IEDs[0].InstanceDataStepRate = 0;

	m_pDevice->CreateInputLayout(_IEDs, 1, _pVertexShaderBlob->GetBufferPointer(), _pVertexShaderBlob->GetBufferSize(), &m_pInputLayout);

}


void PostProcessingEffect::Blit(Texture* m_pSource)
{
	unsigned int Offset = 0;
	unsigned int Stride = sizeof(XMFLOAT3);

	m_pDevCon->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &Stride, &Offset);
	m_pDevCon->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	m_pDevCon->IASetInputLayout(m_pInputLayout);
	m_pDevCon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	m_pDevCon->VSSetShader(m_pVertexShader, nullptr, 0);

	m_pDevCon->PSSetShader(m_pPixelShader, nullptr, 0);
	ID3D11ShaderResourceView* _pSource = m_pSource->GetSRV();
	m_pDevCon->PSSetShaderResources(0, 1, &_pSource);

	m_pDevCon->DrawIndexed(6, 0, 0);
}