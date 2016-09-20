#include "SpriteBatch.h"
#include <Windows.h>

struct SpriteBatchVertex
{
	XMFLOAT3 Position;
	XMFLOAT4 Color;
};

SpriteBatch::SpriteBatch()
{
	m_pVertexBuffer = nullptr;
	m_pIndexBuffer = nullptr;
	m_pVertexShader = nullptr;
	m_pPixelShader = nullptr;

	m_pInputLayout = nullptr;

	m_pDevCon = nullptr;
	m_pDevice = nullptr;
}

void SpriteBatch::Init(ID3D11Device* p_pDevice, ID3D11DeviceContext* p_pDevCon)
{
	m_pDevice = p_pDevice;
	m_pDevCon = p_pDevCon;

	// VertexBuffer erstellen
	D3D11_BUFFER_DESC _VBDesc;
	ZeroMemory(&_VBDesc, sizeof(_VBDesc));

	_VBDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER;
	_VBDesc.ByteWidth = (DRAWCOMMANDBATCHSIZE * 4) * sizeof(SpriteBatchVertex);
	_VBDesc.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
	_VBDesc.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;

	m_pDevice->CreateBuffer(&_VBDesc, nullptr, &m_pVertexBuffer);

	// IndexBuffer erstellen
	D3D11_BUFFER_DESC _IBDesc;
	ZeroMemory(&_IBDesc, sizeof(_IBDesc));

	_IBDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_INDEX_BUFFER;
	_IBDesc.ByteWidth = (DRAWCOMMANDBATCHSIZE * 6) * sizeof(unsigned int);
	_IBDesc.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
	_IBDesc.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;

	m_pDevice->CreateBuffer(&_IBDesc, nullptr, &m_pIndexBuffer);
	// IndexBuffer mit daten befüllen

	unsigned int _Indices[DRAWCOMMANDBATCHSIZE * 6];
	_Indices[0] = 0;
	_Indices[1] = 3;
	_Indices[2] = 1;

	_Indices[3] = 1;
	_Indices[4] = 3;
	_Indices[5] = 2;

	for (int x = 0; x < DRAWCOMMANDBATCHSIZE; x++)
	{
		_Indices[x * 6 + 0] = x * 4 + 0;
		_Indices[x * 6 + 1] = x * 4 + 3;
		_Indices[x * 6 + 2] = x * 4 + 1;

		_Indices[x * 6 + 3] = x * 4 + 1;
		_Indices[x * 6 + 4] = x * 4 + 3;
		_Indices[x * 6 + 5] = x * 4 + 2;
	}
	D3D11_MAPPED_SUBRESOURCE _MSR;

	m_pDevCon->Map(m_pIndexBuffer, 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &_MSR);
	memcpy(_MSR.pData, &_Indices, (DRAWCOMMANDBATCHSIZE * 6) * sizeof(unsigned int));
	m_pDevCon->Unmap(m_pIndexBuffer, 0);

	// VertexShader erstellen
	ID3DBlob* _pVertexByteCode;
	ID3DBlob* _pErrorMessage;

	if (D3DCompileFromFile(L"SpriteBatch.hlsl", nullptr, nullptr, "VShader", "vs_5_0", 0, 0, &_pVertexByteCode, &_pErrorMessage) != S_OK)
	{
		// Fehler ausgeben
		MessageBox(0, (char*)_pErrorMessage->GetBufferPointer(), "Fehler im VertexShader", MB_OK);

		PostQuitMessage(0);
		return;
	}
	m_pDevice->CreateVertexShader(_pVertexByteCode->GetBufferPointer(), _pVertexByteCode->GetBufferSize(), nullptr, &m_pVertexShader);

	// PixelShader erstellen
	ID3DBlob* _pPixelByteCode;
	if (D3DCompileFromFile(L"SpriteBatch.hlsl", nullptr, nullptr, "PShader", "ps_5_0", 0, 0, &_pPixelByteCode, &_pErrorMessage) != S_OK)
	{
		// Fehler ausgeben
		MessageBox(0, (char*)_pErrorMessage->GetBufferPointer(), "Fehler im PixelShader", MB_OK);

		PostQuitMessage(0);
		return;
	}
	m_pDevice->CreatePixelShader(_pPixelByteCode->GetBufferPointer(), _pPixelByteCode->GetBufferSize(), nullptr, &m_pPixelShader);

	// InputLayout erstellen
	D3D11_INPUT_ELEMENT_DESC _IED[2];
	_IED[0].InputSlot = 0;
	_IED[0].InputSlotClass = D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA;
	_IED[0].InstanceDataStepRate = 0;

	_IED[0].SemanticName = "POSITION";
	_IED[0].SemanticIndex = 0;
	_IED[0].AlignedByteOffset = 0;
	_IED[0].Format = DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT;

	_IED[1].InputSlot = 0;
	_IED[1].InputSlotClass = D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA;
	_IED[1].InstanceDataStepRate = 0;

	_IED[1].SemanticName = "COLOR";
	_IED[1].SemanticIndex = 0;
	_IED[1].AlignedByteOffset = 12;
	_IED[1].Format = DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT;

	m_pDevice->CreateInputLayout(_IED, 2, _pVertexByteCode->GetBufferPointer(), _pVertexByteCode->GetBufferSize(), &m_pInputLayout);
}

void SpriteBatch::Begin()
{

}

void SpriteBatch::Draw(Rect p_Destination, XMFLOAT4 p_Color)
{
	SpriteRenderCommand _NewCommand;
	_NewCommand.m_Color = p_Color;
	_NewCommand.m_Destination = p_Destination;

	m_DrawCommands.push_back(_NewCommand);
	m_DrawCommandCount++;


	if (m_DrawCommandCount >= DRAWCOMMANDBATCHSIZE)
	{
		End();
		Begin();
	}
}

void SpriteBatch::End()
{
	// Alle SpriteRenderCommands in einen Buffer packen und rendern
	SpriteBatchVertex* _Vertices = new SpriteBatchVertex[m_DrawCommandCount * 4];

	int width = 800;
	int height = 600;

	int _CurrentVertex = 0;
	for (auto _It = m_DrawCommands.begin(); _It != m_DrawCommands.end(); _It++)
	{

		SpriteRenderCommand _CurrentCommand = *_It;

		int XStart = _CurrentCommand.m_Destination.x;
		int XEnd = _CurrentCommand.m_Destination.x + _CurrentCommand.m_Destination.width;
		int YStart = _CurrentCommand.m_Destination.y;
		int YEnd = _CurrentCommand.m_Destination.y + _CurrentCommand.m_Destination.height;

		XStart /= width;
		XEnd /= width;
		YStart /= height;
		YEnd /= height;

		_Vertices[_CurrentVertex + 0].Position = XMFLOAT3(XStart, YStart, 0);
		_Vertices[_CurrentVertex + 1].Position = XMFLOAT3(XEnd, YStart, 0);
		_Vertices[_CurrentVertex + 2].Position = XMFLOAT3(XEnd, YEnd, 0);
		_Vertices[_CurrentVertex + 3].Position = XMFLOAT3(XStart, YEnd, 0);

		_Vertices[_CurrentVertex + 0].Color = _CurrentCommand.m_Color;
		_Vertices[_CurrentVertex + 1].Color = _CurrentCommand.m_Color;
		_Vertices[_CurrentVertex + 2].Color = _CurrentCommand.m_Color;
		_Vertices[_CurrentVertex + 3].Color = _CurrentCommand.m_Color;


		_CurrentVertex += 4;
	}
	D3D11_MAPPED_SUBRESOURCE _MSR;
	m_pDevCon->Map(m_pVertexBuffer, 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &_MSR);
	memcpy(_MSR.pData, &_Vertices, m_DrawCommandCount * 4 * sizeof(SpriteBatchVertex));
	m_pDevCon->Unmap(m_pVertexBuffer, 0);
	delete[] _Vertices;

	unsigned int stride = sizeof(SpriteBatchVertex);
	unsigned int offset = 0;
	m_pDevCon->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);
	m_pDevCon->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT::DXGI_FORMAT_R32_UINT, 0);
	m_pDevCon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_pDevCon->IASetInputLayout(m_pInputLayout);

	m_pDevCon->VSSetShader(m_pVertexShader, nullptr, 0);
	m_pDevCon->PSSetShader(m_pPixelShader, nullptr, 0);

	m_pDevCon->DrawIndexed(m_DrawCommandCount * 6, 0, 0);


	m_DrawCommands.clear();
	m_DrawCommandCount = 0;
}