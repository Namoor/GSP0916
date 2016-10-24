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
	m_pIndexBuffer = nullptr;
	m_pVertexBuffer = nullptr;
	m_pInputLayout = nullptr;
	m_pVertexShader = nullptr;
	m_pPixelShader = nullptr;
}

void First3DObject::Init(ID3D11Device* p_pDevice, ID3D11DeviceContext* p_pDevCon)
{
	m_pTransform = new Transform(XMFLOAT3(2, 0, 0), XMFLOAT3(0, 0, 0), XMFLOAT3(1, 1, 1));
	m_pMainTexture = new Texture( p_pDevice, "HappyFace.bmp" );

 	m_pDevCon = p_pDevCon;
	m_pDevice = p_pDevice;

	m_pMesh = new Mesh();
	m_pMesh->Init(24, sizeof(First3DVertex), 36, m_pDevice, m_pDevCon);

	// VertexBuffer
	D3D11_BUFFER_DESC _VBDesc;
	ZeroMemory(&_VBDesc, sizeof(_VBDesc));

	_VBDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER;
	_VBDesc.ByteWidth = 24 * sizeof(First3DVertex);
	_VBDesc.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
	_VBDesc.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;

	m_pDevice->CreateBuffer(&_VBDesc, nullptr, &m_pVertexBuffer);

	First3DVertex _Vertices[24];
	
	// Vorne
	_Vertices[0].Position = XMFLOAT3(-0.5f, -0.5f, -0.5f);
	_Vertices[0].Color = XMFLOAT4(0, 0, 0, 1);
	_Vertices[0].UV = XMFLOAT2( 0, 1 );
	_Vertices[0].Normal = XMFLOAT3(0, 0, -1);
	_Vertices[1].Position = XMFLOAT3(-0.5f, 0.5f, -0.5f);
	_Vertices[1].Color = XMFLOAT4( 0, 1, 0, 1 );
	_Vertices[1].UV = XMFLOAT2( 0, 0 );
	_Vertices[1].Normal = XMFLOAT3( 0, 0, -1 );

	_Vertices[2].Position = XMFLOAT3(0.5f, 0.5f, -0.5f);
	_Vertices[2].Color = XMFLOAT4( 1, 1, 0, 1 );
	_Vertices[2].UV = XMFLOAT2( 1, 0 );
	_Vertices[2].Normal = XMFLOAT3( 0, 0, -1 );
	_Vertices[3].Position = XMFLOAT3(0.5f, -0.5f, -0.5f);
	_Vertices[3].Color = XMFLOAT4( 1, 0, 0, 1 );
	_Vertices[3].UV = XMFLOAT2( 1, 1 );
	_Vertices[3].Normal = XMFLOAT3( 0, 0, -1 );

	// Rechts
	_Vertices[4].Position = XMFLOAT3(0.5f, -0.5f, -0.5f);
	_Vertices[4].Color = XMFLOAT4( 1, 0, 0, 1 );
	_Vertices[4].UV = XMFLOAT2( 0, 1 );
	_Vertices[4].Normal = XMFLOAT3( 1, 0, 0 );
	_Vertices[5].Position = XMFLOAT3(0.5f, 0.5f, -0.5f);
	_Vertices[5].Color = XMFLOAT4( 1, 1, 0, 1 );
	_Vertices[5].UV = XMFLOAT2( 0, 0 );
	_Vertices[5].Normal = XMFLOAT3( 1, 0, 0 );

	_Vertices[6].Position = XMFLOAT3(0.5f, 0.5f, 0.5f);
	_Vertices[6].Color = XMFLOAT4( 1, 1, 1, 1 );
	_Vertices[6].UV = XMFLOAT2( 1, 0 );
	_Vertices[6].Normal = XMFLOAT3( 1, 0, 0 );
	_Vertices[7].Position = XMFLOAT3(0.5f, -0.5f, 0.5f);
	_Vertices[7].Color = XMFLOAT4( 1, 0, 1, 1 );
	_Vertices[7].UV = XMFLOAT2( 1, 1 );
	_Vertices[7].Normal = XMFLOAT3( 1, 0, 0 );

	// Oben
	_Vertices[8].Position = XMFLOAT3( -0.5f, 0.5f, -0.5f );
	_Vertices[8].Color = XMFLOAT4( 0, 1, 0, 1 );
	_Vertices[8].UV = XMFLOAT2( 0, 1 );
	_Vertices[8].Normal = XMFLOAT3( 0, 1, 0 );
	_Vertices[9].Position = XMFLOAT3( -0.5f, 0.5f, 0.5f );
	_Vertices[9].Color = XMFLOAT4( 0, 1, 1, 1 );
	_Vertices[9].UV = XMFLOAT2( 0, 0 );
	_Vertices[9].Normal = XMFLOAT3( 0, 1, 0 );

	_Vertices[10].Position = XMFLOAT3( 0.5f, 0.5f, 0.5f );
	_Vertices[10].Color = XMFLOAT4( 1, 1, 1, 1 );
	_Vertices[10].UV = XMFLOAT2( 1, 0 );
	_Vertices[10].Normal = XMFLOAT3( 0, 1, 0 );
	_Vertices[11].Position = XMFLOAT3( 0.5f, 0.5f, -0.5f );
	_Vertices[11].Color = XMFLOAT4( 1, 1, 0, 1 );
	_Vertices[11].UV = XMFLOAT2( 1, 1 );
	_Vertices[11].Normal = XMFLOAT3( 0, 1, 0 );
	
	// Hinten
	_Vertices[12].Position = XMFLOAT3( -0.5f, -0.5f, 0.5f );
	_Vertices[12].Color = XMFLOAT4( 0, 0, 1, 1 );
	_Vertices[12].UV = XMFLOAT2( 1, 1 );
	_Vertices[12].Normal = XMFLOAT3( 0, 0, 1 );
	_Vertices[13].Position = XMFLOAT3( -0.5f, 0.5f, 0.5f );
	_Vertices[13].Color = XMFLOAT4( 0, 1, 1, 1 );
	_Vertices[13].UV = XMFLOAT2( 1, 0 );
	_Vertices[13].Normal = XMFLOAT3( 0, 0, 1 );

	_Vertices[14].Position = XMFLOAT3( 0.5f, 0.5f, 0.5f );
	_Vertices[14].Color = XMFLOAT4( 1, 1, 1, 1 );
	_Vertices[14].UV = XMFLOAT2( 0, 0 );
	_Vertices[14].Normal = XMFLOAT3( 0, 0, 1 );
	_Vertices[15].Position = XMFLOAT3( 0.5f, -0.5f, 0.5f );
	_Vertices[15].Color = XMFLOAT4( 1, 0, 1, 1 );
	_Vertices[15].UV = XMFLOAT2( 0, 1 );
	_Vertices[15].Normal = XMFLOAT3( 0, 0, 1 );
	
	// Links
	_Vertices[16].Position = XMFLOAT3( -0.5f, -0.5f, -0.5f );
	_Vertices[16].Color = XMFLOAT4( 0, 0, 0, 1 );
	_Vertices[16].UV = XMFLOAT2( 1, 1 );
	_Vertices[16].Normal = XMFLOAT3( -1, 0, 0 );
	_Vertices[17].Position = XMFLOAT3( -0.5f, 0.5f, -0.5f );
	_Vertices[17].Color = XMFLOAT4( 0, 1, 0, 1 );
	_Vertices[17].UV = XMFLOAT2( 1, 0 );
	_Vertices[17].Normal = XMFLOAT3( -1, 0, 0 );

	_Vertices[18].Position = XMFLOAT3( -0.5f, 0.5f, 0.5f );
	_Vertices[18].Color = XMFLOAT4( 0, 1, 1, 1 );
	_Vertices[18].UV = XMFLOAT2( 0, 0 );
	_Vertices[18].Normal = XMFLOAT3( -1, 0, 0 );
	_Vertices[19].Position = XMFLOAT3( -0.5f, -0.5f, 0.5f );
	_Vertices[19].Color = XMFLOAT4( 0, 0, 1, 1 );
	_Vertices[19].UV = XMFLOAT2( 0, 1 );
	_Vertices[19].Normal = XMFLOAT3( -1, 0, 0 );
	// Unten
	_Vertices[20].Position = XMFLOAT3( -0.5f, -0.5f, -0.5f );
	_Vertices[20].Color = XMFLOAT4( 0, 0, 0, 1 );
	_Vertices[20].UV = XMFLOAT2( 0, 0 );
	_Vertices[20].Normal = XMFLOAT3( 0, -1, 0 );
	_Vertices[21].Position = XMFLOAT3( -0.5f, -0.5f, 0.5f );
	_Vertices[21].Color = XMFLOAT4( 0, 0, 1, 1 );
	_Vertices[21].UV = XMFLOAT2( 0, 1 );
	_Vertices[21].Normal = XMFLOAT3( 0, -1, 0 );

	_Vertices[22].Position = XMFLOAT3( 0.5f, -0.5f, 0.5f );
	_Vertices[22].Color = XMFLOAT4( 1, 0, 1, 1 );
	_Vertices[22].UV = XMFLOAT2( 1, 1 );
	_Vertices[22].Normal = XMFLOAT3( 0, -1, 0 );
	_Vertices[23].Position = XMFLOAT3( 0.5f, -0.5f, -0.5f );
	_Vertices[23].Color = XMFLOAT4( 1, 0, 0, 1 );
	_Vertices[23].UV = XMFLOAT2( 1, 0 );
	_Vertices[23].Normal = XMFLOAT3( 0, -1, 0 );

	D3D11_MAPPED_SUBRESOURCE _VMSR;

	m_pDevCon->Map(m_pVertexBuffer, 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &_VMSR);
	memcpy(_VMSR.pData, _Vertices, 24 * sizeof(First3DVertex));
	m_pDevCon->Unmap(m_pVertexBuffer, 0);

	// IndexBuffer
	D3D11_BUFFER_DESC _IBDesc;
	ZeroMemory(&_IBDesc, sizeof(_IBDesc));

	_IBDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_INDEX_BUFFER;
	_IBDesc.ByteWidth = 12 * 3 * sizeof(unsigned int);
	_IBDesc.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
	_IBDesc.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;

	m_pDevice->CreateBuffer(&_IBDesc, nullptr, &m_pIndexBuffer);

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

	m_pMesh->SetData(&_Vertices, &_Indices);

	D3D11_MAPPED_SUBRESOURCE _IMSR;

	m_pDevCon->Map(m_pIndexBuffer, 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &_IMSR);
	memcpy(_IMSR.pData, _Indices, 12 * 3 * sizeof(unsigned int));
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
	D3D11_INPUT_ELEMENT_DESC _IED[4];

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

	// UV
	_IED[2].InputSlot = 0;
	_IED[2].InstanceDataStepRate = 0;
	_IED[2].InputSlotClass = D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA;

	_IED[2].AlignedByteOffset = 28;
	_IED[2].Format = DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT;

	_IED[2].SemanticName = "TEXCOORD";
	_IED[2].SemanticIndex = 0;

	// NORMAL
	_IED[3].InputSlot = 0;
	_IED[3].InstanceDataStepRate = 0;
	_IED[3].InputSlotClass = D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA;

	_IED[3].AlignedByteOffset = 36;
	_IED[3].Format = DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT;

	_IED[3].SemanticName = "NORMAL";
	_IED[3].SemanticIndex = 0;


	m_pDevice->CreateInputLayout(_IED, 4, _pVShaderCode->GetBufferPointer(), _pVShaderCode->GetBufferSize(), &m_pInputLayout);

	D3D11_BUFFER_DESC _MBD;
	ZeroMemory(&_MBD, sizeof(_MBD));

	_MBD.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_CONSTANT_BUFFER;
	_MBD.ByteWidth = sizeof(MatrixBuffer);
	_MBD.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
	_MBD.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;

	m_pDevice->CreateBuffer(&_MBD, nullptr, &m_pMatrixBuffer);

	CurrentAngle = 0;
}


void First3DObject::Update(float p_DeltaTime)
{
	// CurrentAngle += p_DeltaTime * 20;
	//m_pTransform->Move(XMFLOAT3(p_DeltaTime, 0, 0), false);
	m_pTransform->RotateAround(XMFLOAT3(0, 1, 0), p_DeltaTime * 20, true);
}

void First3DObject::Render(Camera* p_pCamera)
{
	MatrixBuffer _MBuffer;
	_MBuffer.MVP = m_pTransform->GetMatrix() * p_pCamera->GetViewMatrix() * p_pCamera->GetProjectionMatrix();
	_MBuffer.M = m_pTransform->GetInvertTranspose();

	D3D11_MAPPED_SUBRESOURCE _MSR;
	m_pDevCon->Map(m_pMatrixBuffer, 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &_MSR);
	memcpy(_MSR.pData, &_MBuffer, sizeof(MatrixBuffer));
	m_pDevCon->Unmap(m_pMatrixBuffer, 0);

	unsigned int _Stride = sizeof(First3DVertex);
	unsigned int _Offset = 0;


	m_pDevCon->IASetInputLayout(m_pInputLayout);
	m_pDevCon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	m_pDevCon->VSSetShader(m_pVertexShader, nullptr, 0);
	m_pDevCon->VSSetConstantBuffers(0, 1, &m_pMatrixBuffer);

	m_pDevCon->PSSetShader(m_pPixelShader, nullptr, 0);
	ID3D11ShaderResourceView* _pSRV = m_pMainTexture->GetSRV();


	m_pDevCon->PSSetShaderResources( 0, 1, &_pSRV );

	//m_pDevCon->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &_Stride, &_Offset);
	//m_pDevCon->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT::DXGI_FORMAT_R32_UINT, 0);
	//m_pDevCon->DrawIndexed(36, 0, 0);
	m_pMesh->Render();
}