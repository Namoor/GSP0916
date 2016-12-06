#include "Scene2D.h"

struct MyVertex
{
	XMFLOAT3 Position;
	XMFLOAT3 Color;
};


void Scene2D::Init(ID3D11Device* p_pDevice, ID3D11DeviceContext* p_pDevCon, ID3D11RenderTargetView* m_pScreen, ID3D11DepthStencilView* m_pDepthStencil)
{
	m_pDevice = p_pDevice;
	m_pDevCon = p_pDevCon;
	m_pVBuffer = nullptr;
	m_pIBuffer = nullptr;
	m_pPixelShader = nullptr;
	m_pVertexShader = nullptr;
	m_pInputLayout = nullptr;

	m_pTexture = new Texture(p_pDevice, L"HappyFace.bmp");

	m_pSpriteBatch = new SpriteBatch();
	m_pSpriteBatch->Init(m_pDevice, m_pDevCon);

	m_pCalibri = new SpriteFont();
	m_pCalibri->Load("Calibri.fnt", m_pDevice);

	m_pFPS = new FPSDisplay();
	
	MyVertex _VertexBuffer[4];
	_VertexBuffer[0].Position = XMFLOAT3(-0.5f, -0.5f, 0);
	_VertexBuffer[0].Color = XMFLOAT3(0, 1, 0);

	_VertexBuffer[1].Position = XMFLOAT3( 0.5f, -0.5f, 0);
	_VertexBuffer[1].Color = XMFLOAT3(1, 1, 0);

	_VertexBuffer[2].Position = XMFLOAT3( 0.5f,  0.5f, 0);
	_VertexBuffer[2].Color = XMFLOAT3(1, 0, 0);

	_VertexBuffer[3].Position = XMFLOAT3(-0.5f,  0.5f, 0);
	_VertexBuffer[3].Color = XMFLOAT3(0, 0, 0);

	unsigned int _IndexBuffer[6];
	//Triangle 1
	_IndexBuffer[0] = 0;
	_IndexBuffer[1] = 3;
	_IndexBuffer[2] = 1;


	// Triangle 2
	_IndexBuffer[3] = 1;
	_IndexBuffer[4] = 3;
	_IndexBuffer[5] = 2;

	// VertexBuffer erstellen
	D3D11_BUFFER_DESC _VBDesc;
	ZeroMemory(&_VBDesc, sizeof(_VBDesc));
	_VBDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER;
	_VBDesc.ByteWidth = 4 * sizeof(MyVertex);
	_VBDesc.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
	_VBDesc.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;

	m_pDevice->CreateBuffer(&_VBDesc, nullptr, &m_pVBuffer);

	D3D11_MAPPED_SUBRESOURCE _MSRVB;

	m_pDevCon->Map(m_pVBuffer, 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &_MSRVB);
	memcpy(_MSRVB.pData, _VertexBuffer, 4 * sizeof(MyVertex));
	m_pDevCon->Unmap(m_pVBuffer, 0);

	

	// IndexBuffer erstellen
	D3D11_BUFFER_DESC _IBDesc;
	ZeroMemory(&_IBDesc, sizeof(_IBDesc));
	_IBDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_INDEX_BUFFER;
	_IBDesc.ByteWidth = 6 * sizeof(unsigned int);
	_IBDesc.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
	_IBDesc.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;

	m_pDevice->CreateBuffer(&_IBDesc, nullptr, &m_pIBuffer);

	D3D11_MAPPED_SUBRESOURCE _MSRIB;

	m_pDevCon->Map(m_pIBuffer, 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &_MSRIB);
	memcpy(_MSRIB.pData, _IndexBuffer, 6 * sizeof(unsigned int));
	m_pDevCon->Unmap(m_pIBuffer, 0);

	// VertexShader erstellen
	ID3DBlob* _pVShaderCode;
	ID3DBlob* _pErrorMessage;

	if (D3DCompileFromFile(L"VShader.hlsl", nullptr, nullptr, "VShader", "vs_5_0", 0, 0, &_pVShaderCode, &_pErrorMessage) != S_OK)
	{
		MessageBox(0, (char*)_pErrorMessage->GetBufferPointer(), "Fehler im VertexShader", MB_OK);

		PostQuitMessage(0);
		return;
	}

	m_pDevice->CreateVertexShader(_pVShaderCode->GetBufferPointer(), _pVShaderCode->GetBufferSize(), nullptr, &m_pVertexShader);
	
	// Pixelshader erstellen

	ID3DBlob* _pPShaderCode;
	if (D3DCompileFromFile(L"PShader.hlsl", nullptr, nullptr, "PShader", "ps_5_0", 0, 0, &_pPShaderCode, &_pErrorMessage) != S_OK)
	{
		MessageBox(0, (char*)_pErrorMessage->GetBufferPointer(), "Fehler im PixelShader", MB_OK);

		PostQuitMessage(0);
		return;
	}

	m_pDevice->CreatePixelShader(_pPShaderCode->GetBufferPointer(), _pPShaderCode->GetBufferSize(), nullptr, &m_pPixelShader);

	// InputLayout erstellen
	D3D11_INPUT_ELEMENT_DESC _IED[2];
	// Position
	_IED[0].SemanticName = "POSITION";
	_IED[0].SemanticIndex = 0;

	_IED[0].AlignedByteOffset = 0;
	_IED[0].Format = DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT;

	_IED[0].InputSlot = 0;
	_IED[0].InputSlotClass = D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA;
	_IED[0].InstanceDataStepRate = 0;

	// Color
	_IED[1].SemanticName = "COLOR";
	_IED[1].SemanticIndex = 0;

	_IED[1].AlignedByteOffset = 12;
	_IED[1].Format = DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT;

	_IED[1].InputSlot = 0;
	_IED[1].InputSlotClass = D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA;
	_IED[1].InstanceDataStepRate = 0;


	m_pDevice->CreateInputLayout(_IED, 2, _pVShaderCode->GetBufferPointer(), _pVShaderCode->GetBufferSize(), &m_pInputLayout);


	D3D11_BUFFER_DESC _CBDesc;
	ZeroMemory(&_CBDesc, sizeof(_CBDesc));

	_CBDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_CONSTANT_BUFFER;
	_CBDesc.ByteWidth = sizeof(XMFLOAT4);
	_CBDesc.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
	_CBDesc.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;

	m_pDevice->CreateBuffer(&_CBDesc, nullptr, &m_pConstantBuffer);


	D3D11_SAMPLER_DESC _SD;
	ZeroMemory(&_SD, sizeof(_SD));

	_SD.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	_SD.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	_SD.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	_SD.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR;
	_SD.MinLOD = -FLT_MAX;
	_SD.MaxLOD = FLT_MAX;
	_SD.MipLODBias = 0.0f;
	_SD.MaxAnisotropy = 1;
	_SD.ComparisonFunc = D3D11_COMPARISON_NEVER;
	_SD.BorderColor[0] = 0;
	_SD.BorderColor[1] = 0;
	_SD.BorderColor[2] = 1;
	_SD.BorderColor[3] = 1;


	m_pDevice->CreateSamplerState(&_SD, &m_pTextureSampleState);

}

void Scene2D::Update(float p_DeltaTime)
{
	XMFLOAT4 _Color;

	_Color.x = 0.9f;
	_Color.y = 0.5f;
	_Color.z = 0.2f;
	_Color.w = 1;

	D3D11_MAPPED_SUBRESOURCE _MSR;
	m_pDevCon->Map(m_pConstantBuffer, 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &_MSR);
	memcpy(_MSR.pData, &_Color, sizeof(XMFLOAT4));
	m_pDevCon->Unmap(m_pConstantBuffer, 0);

	m_pFPS->Update(p_DeltaTime);
}

void Scene2D::Render(ID3D11Device* p_pDevice, ID3D11DeviceContext* p_pDevCon)
{
	//UINT stride = sizeof(MyVertex);
	//UINT offset = 0;
	//
	//
	//
	//m_pDevCon->IASetVertexBuffers(0, 1, &m_pVBuffer, &stride, &offset);
	//m_pDevCon->IASetIndexBuffer(m_pIBuffer, DXGI_FORMAT::DXGI_FORMAT_R32_UINT, 0);
	//
	//m_pDevCon->IASetInputLayout(m_pInputLayout);
	//m_pDevCon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//
	//m_pDevCon->VSSetShader(m_pVertexShader, nullptr, 0);
	//
	//m_pDevCon->PSSetShader(m_pPixelShader, nullptr, 0);
	//m_pDevCon->PSSetConstantBuffers(0, 1, &m_pConstantBuffer);
	//m_pDevCon->PSSetShaderResources(0, 1, &m_pTexture);
	//m_pDevCon->PSSetSamplers(0, 1, &m_pTextureSampleState);
	//
	//m_pDevCon->DrawIndexed(6, 0, 0);



	m_pSpriteBatch->Begin();

	//m_pSpriteBatch->Draw(Rect(4, 4, 800 - 8, 600 - 8), Rect(0,0,0,0), XMFLOAT4(1, 1, 1, 1));
	//m_pSpriteBatch->Draw(m_pTexture, Rect(200, 0, 100, 100), Rect(0, 0, 499, 396), XMFLOAT4(1, 1, 1, 1));
	//m_pSpriteBatch->Draw(m_pTexture, Rect(250, 100, 100, 100), Rect(0, 0, 499, 396), XMFLOAT4(1, 1, 1, 1));
	//m_pSpriteBatch->Draw(m_pTexture, Rect(0, 200, 100, 100), Rect(0, 0, 499, 396), XMFLOAT4(1, 1, 1, 1));
	//m_pSpriteBatch->Draw(m_pTexture, Rect(400, 0, 100, 100), Rect(0, 0, 499, 396), XMFLOAT4(1, 1, 1, 1));
	//m_pSpriteBatch->Draw(m_pTexture, Rect(0, 400, 100, 100), Rect(0, 0, 499, 396), XMFLOAT4(1, 1, 1, 1));

	for (int x = 0; x < 1; x++)
	{
		//m_pSpriteBatch->Draw(m_pTexture, Rect(0, 0, 500, 500), Rect(0, 0, 300, 200), XMFLOAT4(0, 0, 1, 0.5f));

		m_pSpriteBatch->DrawString(m_pCalibri, "the quick brown fox jumps over the lazy dog", 10, 10, 32, XMFLOAT4(1, 1, 1, 1));
		m_pSpriteBatch->DrawString(m_pCalibri, "THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG", 10, 50, 32, XMFLOAT4(1, 1, 1, 1));

		m_pSpriteBatch->DrawString(m_pCalibri, "?!öÖüÜäÄß*+~#'\\/={}[]()&%$§\"-_.:,;", 10, 100, 32, XMFLOAT4(1, 1, 1, 1));
	}
	m_pFPS->Render(m_pSpriteBatch, m_pCalibri, 0, 0);

	m_pSpriteBatch->End();
}
