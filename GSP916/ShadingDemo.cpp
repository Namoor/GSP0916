#include "ShadingDemo.h"

struct ShadingDemo_Vertex
{
	XMFLOAT3 Position;
	XMFLOAT3 Normal;
	XMFLOAT3 Tangent;
	XMFLOAT3 BiTangent;
	XMFLOAT2 UV;
};

struct ShadingDemo_MatrixConstantBuffer
{
	XMMATRIX ModelViewProjection;
};

struct ShadingDemo_LightConstantBuffer
{
	XMFLOAT4 Cam_CameraPositionInWorldSpace;

	XMFLOAT4 Dir_LightDirectionRGB_SpecularIntensityA;
	XMFLOAT4 Dir_LightColorRGB_SpecularExponentA;

	XMFLOAT4 PL1_LightPositionRGB_RangeA;
	XMFLOAT4 PL1_LightColorRGB_SpecularExponentA;
	XMFLOAT4 PL1_SpecularIntensityA;

	XMFLOAT4 PL2_LightPositionRGB_RangeA;
	XMFLOAT4 PL2_LightColorRGB_SpecularExponentA;
	XMFLOAT4 PL2_SpecularIntensityA;

	XMFLOAT4 Amb_LightColor;
};

XMFLOAT3 Cross(XMFLOAT3 LHS, XMFLOAT3 RHS)
{
	return XMFLOAT3(LHS.y * RHS.z - LHS.z * RHS.y, LHS.z * RHS.x - RHS.z * LHS.x, LHS.x*RHS.y - LHS.y*RHS.x);
}

XMFLOAT3 Normalize(XMFLOAT3 Vector)
{
	float Length = sqrt(Vector.x * Vector.x + Vector.y * Vector.y + Vector.z * Vector.z);

	return XMFLOAT3(Vector.x / Length, Vector.y / Length, Vector.z / Length);
}

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

	m_TimePassed = 0;
}

float GetHeightAt(float x, float y)
{
	//return 0-y;

	return (sin(x*20) + sin(y*20)) * 0.2f;

	float _dx = fabs( 2*(0.5f - x));
	float _dy = fabs(2*(0.5f - y));

	float _DistanceSquared = _dy * _dy + _dx * _dx;

	if (_DistanceSquared > 1)
		return 0;

	float _Distance = sqrt(_DistanceSquared);

	// 1*1 = _DistanceSquared + h*h | - _DistanceSquared
	// 1 - _DistanceSquared = h*h   | sqrt
	// sqrt(1 - _DistanceSquared) = h

	float Height = sqrt(1 - _DistanceSquared);

	return Height;



}

void ShadingDemo::Init(ID3D11Device* p_pDevice, ID3D11DeviceContext* p_pDevCon, int p_Subdivisions)
{
	m_pDevCon = p_pDevCon;
	m_pDevice = p_pDevice;

	m_pDiffuse = new Texture(p_pDevice, "Metal_Diffuse.jpg");
	m_pNormal = new Texture(p_pDevice, "Metal_Normal.jpg");


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
			_pVertices[y * _VerticesPerSide + x].Position = XMFLOAT3(x/(float)p_Subdivisions * 4, GetHeightAt(x / (float)p_Subdivisions, y / (float)p_Subdivisions) * 2, y / (float)p_Subdivisions * 4);
			_pVertices[y * _VerticesPerSide + x].UV = XMFLOAT2(x / (float)_VerticesPerSide, y / (float)_VerticesPerSide);
		}



	

	// IndexBuffer
	m_Indices = p_Subdivisions * p_Subdivisions * 2 * 3;

	D3D11_BUFFER_DESC _IBDesc;
	ZeroMemory(&_IBDesc, sizeof(_IBDesc));

	_IBDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_INDEX_BUFFER;
	_IBDesc.ByteWidth = m_Indices * sizeof(unsigned int);
	_IBDesc.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
	_IBDesc.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;

	m_pDevice->CreateBuffer(&_IBDesc, nullptr, &m_pIndexBuffer);

	unsigned int* _pIndices = (unsigned int*) malloc(m_Indices * sizeof(unsigned int));

	int _CurrentIndex = 0;

	for (int x = 0; x < p_Subdivisions; x++)
	{
		for (int y = 0; y < p_Subdivisions; y++)
		{
			// Ein Quad mit indices füllen

			unsigned int _BL = x + _VerticesPerSide * y;
			unsigned int _BR = _BL + 1;
			unsigned int _TL = _BL + _VerticesPerSide;
			unsigned int _TR = _TL + 1;

			_pIndices[_CurrentIndex++] = _TL;
			_pIndices[_CurrentIndex++] = _TR;
			_pIndices[_CurrentIndex++] = _BL;

			_pIndices[_CurrentIndex++] = _BL;
			_pIndices[_CurrentIndex++] = _TR;
			_pIndices[_CurrentIndex++] = _BR;
		}
	}

	// Normalen Berechnen
	int _VertexCount = ((p_Subdivisions + 1) * (p_Subdivisions + 1));
	for (int x = 0; x < _VertexCount; x++)
	{
		_pVertices[x].Normal = XMFLOAT3(0, 0, 0);
	}

	for (int x = 0; x < m_Indices; x += 3)
	{
		int _Index0 = _pIndices[ x];
		int _Index1 = _pIndices[x + 1];
		int _Index2 = _pIndices[x + 2];

		XMFLOAT3 _Pos0 = _pVertices[_Index0].Position;
		XMFLOAT3 _Pos1 = _pVertices[_Index1].Position;
		XMFLOAT3 _Pos2 = _pVertices[_Index2].Position;

		XMFLOAT3 _01 = XMFLOAT3(_Pos1.x - _Pos0.x, _Pos1.y - _Pos0.y, _Pos1.z - _Pos0.z);
		XMFLOAT3 _02 = XMFLOAT3(_Pos2.x - _Pos0.x, _Pos2.y - _Pos0.y, _Pos2.z - _Pos0.z);

		//  x0   x1
		//  y0   y1
		//  z0   z1

		// y0*z1-y1*z0
		// z0*x1-z1*x0
		// x0*y1-y0*x1

		XMFLOAT3 _Normal = XMFLOAT3(_01.y*_02.z - _02.y*_01.z, _01.z*_02.x - _02.z*_01.x, _01.x*_02.y - _01.y*_02.x);
		float _Length = sqrt(_Normal.x*_Normal.x + _Normal.y*_Normal.y + _Normal.z*_Normal.z);
		
		XMFLOAT3 _Normalized = XMFLOAT3(_Normal.x / _Length, _Normal.y / _Length, _Normal.z / _Length);

		_pVertices[_Index0].Normal = XMFLOAT3(_Normalized.x + _pVertices[_Index0].Normal.x, _Normalized.y + _pVertices[_Index0].Normal.y, _Normalized.z + _pVertices[_Index0].Normal.z);
		_pVertices[_Index1].Normal = XMFLOAT3(_Normalized.x + _pVertices[_Index1].Normal.x, _Normalized.y + _pVertices[_Index1].Normal.y, _Normalized.z + _pVertices[_Index1].Normal.z);
		_pVertices[_Index2].Normal = XMFLOAT3(_Normalized.x + _pVertices[_Index2].Normal.x, _Normalized.y + _pVertices[_Index2].Normal.y, _Normalized.z + _pVertices[_Index2].Normal.z);
	}

	for (int x = 0; x < _VertexCount; x++)
	{
		XMFLOAT3 _Normal = _pVertices[x].Normal;
		float _Length = sqrt(_Normal.x*_Normal.x + _Normal.y*_Normal.y + _Normal.z*_Normal.z);

		_pVertices[x].Normal = XMFLOAT3(_Normal.x / _Length, _Normal.y / _Length, _Normal.z / _Length);

		XMFLOAT3 _BiTangent = XMFLOAT3(0, 0, 1);
		XMFLOAT3 _Tangent = Cross(_pVertices[x].Normal, _BiTangent);
		_Tangent = Normalize(_Tangent);
		_BiTangent = Cross(_Tangent, _pVertices[x].Normal);

		_pVertices[x].Tangent = _Tangent;
		_pVertices[x].BiTangent = _BiTangent;

		int ads = 0;

	}
	
	// Vertex und Index Buffer übertragen
	D3D11_MAPPED_SUBRESOURCE _IBMSR;
	m_pDevCon->Map(m_pIndexBuffer, 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &_IBMSR);
	memcpy(_IBMSR.pData, _pIndices, m_Indices * sizeof(unsigned int));
	m_pDevCon->Unmap(m_pIndexBuffer, 0);


	D3D11_MAPPED_SUBRESOURCE _VBMSR;
	m_pDevCon->Map(m_pVertexBuffer, 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &_VBMSR);
	memcpy(_VBMSR.pData, _pVertices, (_VerticesPerSide * _VerticesPerSide) * sizeof(ShadingDemo_Vertex));
	m_pDevCon->Unmap(m_pVertexBuffer, 0);



	// Matrix Constant Buffer

	D3D11_BUFFER_DESC _CBDesc;
	ZeroMemory(&_CBDesc, sizeof(_CBDesc));

	_CBDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_CONSTANT_BUFFER;
	_CBDesc.ByteWidth = sizeof(ShadingDemo_MatrixConstantBuffer);
	_CBDesc.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
	_CBDesc.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;

	m_pDevice->CreateBuffer(&_CBDesc, nullptr, &m_pMatrixConstantBuffer);


	D3D11_BUFFER_DESC _LBDesc;
	ZeroMemory(&_LBDesc, sizeof(_LBDesc));

	_LBDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_CONSTANT_BUFFER;
	_LBDesc.ByteWidth = sizeof(ShadingDemo_LightConstantBuffer);
	_LBDesc.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
	_LBDesc.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;

	m_pDevice->CreateBuffer(&_LBDesc, nullptr, &m_pLightingConstantBuffer);

	// VertexShader
	ID3DBlob* _pVertexShaderBlob;
	ID3DBlob* _pErrorMessage;

	HRESULT _Result = D3DCompileFromFile(L"ShadingDemo.hlsl", nullptr, nullptr, "VShader", "vs_5_0", 0, 0, &_pVertexShaderBlob, &_pErrorMessage);
	if (_Result != S_OK)
	{
		MessageBox(0, (char*)_pErrorMessage->GetBufferPointer(), "Fehler im VertexShader", MB_OK);
		PostQuitMessage(0);
		return;
	}
	m_pDevice->CreateVertexShader(_pVertexShaderBlob->GetBufferPointer(), _pVertexShaderBlob->GetBufferSize(), nullptr, &m_pVertexShader);


	// PixelShader
	ID3DBlob* _pPixelShaderBlob;

	_Result = D3DCompileFromFile(L"ShadingDemo.hlsl", nullptr, nullptr, "PShader", "ps_5_0", 0, 0, &_pPixelShaderBlob, &_pErrorMessage);
	if (_Result != S_OK)
	{
		MessageBox(0, (char*)_pErrorMessage->GetBufferPointer(), "Fehler im PixelShader", MB_OK);
		PostQuitMessage(0);
		return;
	}
	m_pDevice->CreatePixelShader(_pPixelShaderBlob->GetBufferPointer(), _pPixelShaderBlob->GetBufferSize(), nullptr, &m_pPixelShader);


	// InputLayout

	D3D11_INPUT_ELEMENT_DESC _IEDs[5];
	ZeroMemory(_IEDs, sizeof(D3D11_INPUT_ELEMENT_DESC) * 5);

	_IEDs[0].AlignedByteOffset = 0;
	_IEDs[0].Format = DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT;

	_IEDs[0].SemanticName = "POSITION";
	_IEDs[0].SemanticIndex = 0;

	_IEDs[1].AlignedByteOffset = 12;
	_IEDs[1].Format = DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT;

	_IEDs[1].SemanticName = "NORMAL";
	_IEDs[1].SemanticIndex = 0;

	_IEDs[2].AlignedByteOffset = 24;
	_IEDs[2].Format = DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT;

	_IEDs[2].SemanticName = "TANGENT";
	_IEDs[2].SemanticIndex = 0;

	_IEDs[3].AlignedByteOffset = 36;
	_IEDs[3].Format = DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT;

	_IEDs[3].SemanticName = "TANGENT";
	_IEDs[3].SemanticIndex = 1;

	_IEDs[4].AlignedByteOffset = 48;
	_IEDs[4].Format = DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT;

	_IEDs[4].SemanticName = "TEXCOORD";
	_IEDs[4].SemanticIndex = 0;

	//_IEDs[0].InputSlot = 0;
	//_IEDs[0].InputSlotClass = D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA;
	//_IEDs[0].InstanceDataStepRate = 0;
	
	m_pDevice->CreateInputLayout(_IEDs, 5, _pVertexShaderBlob->GetBufferPointer(), _pVertexShaderBlob->GetBufferSize(), &m_pInputLayout);
}

void ShadingDemo::Update(float p_DeltaTime)
{
	m_TimePassed += p_DeltaTime;
}

void ShadingDemo::Render(Camera* p_pCamera)
{
	ShadingDemo_LightConstantBuffer _NewLightData;

	_NewLightData.Cam_CameraPositionInWorldSpace = p_pCamera->GetPositionAsFloat4();

	_NewLightData.Dir_LightDirectionRGB_SpecularIntensityA =  XMFLOAT4(0,0,1, 1.0f);
	_NewLightData.Dir_LightColorRGB_SpecularExponentA = XMFLOAT4(255 / 256.0f, 224 / 256.0f, 122/256.0f, 140.0f);

	_NewLightData.PL1_LightPositionRGB_RangeA = XMFLOAT4(sin(m_TimePassed) * 3.0f + 2, 1, cos(m_TimePassed) * 3.0f + 2, 4);
	_NewLightData.PL1_LightColorRGB_SpecularExponentA = XMFLOAT4(0, 0, 0, 140);
	_NewLightData.PL1_SpecularIntensityA = XMFLOAT4(0, 0, 0, 1);

	_NewLightData.PL2_LightPositionRGB_RangeA = XMFLOAT4(-sin(m_TimePassed) * 3.0f + 2, 1, -cos(m_TimePassed) * 3.0f + 2, 4);
	_NewLightData.PL2_LightColorRGB_SpecularExponentA = XMFLOAT4(0, 0, 0, 140);
	_NewLightData.PL2_SpecularIntensityA = XMFLOAT4(0, 0, 0, 1);

	_NewLightData.Amb_LightColor = XMFLOAT4(0.3, 0.3, 0.3, 0);

	D3D11_MAPPED_SUBRESOURCE _LBMSR;

	m_pDevCon->Map(m_pLightingConstantBuffer, 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &_LBMSR);
	memcpy(_LBMSR.pData, &_NewLightData, sizeof(ShadingDemo_LightConstantBuffer));
	m_pDevCon->Unmap(m_pLightingConstantBuffer, 0);


	ShadingDemo_MatrixConstantBuffer _NewMatrixData;

	// Struct mit daten füllen
	_NewMatrixData.ModelViewProjection = p_pCamera->GetViewMatrix() * p_pCamera->GetProjectionMatrix();


	// Struct auf die graphikkarte übertragen
	D3D11_MAPPED_SUBRESOURCE _CBMSR;
	m_pDevCon->Map(m_pMatrixConstantBuffer, 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &_CBMSR);
	memcpy(_CBMSR.pData, &_NewMatrixData, sizeof(ShadingDemo_MatrixConstantBuffer));
	m_pDevCon->Unmap(m_pMatrixConstantBuffer, 0);


	// State assignment
	UINT _Stride = sizeof(ShadingDemo_Vertex);
	UINT _Offset = 0;

	m_pDevCon->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &_Stride, &_Offset);
	m_pDevCon->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT::DXGI_FORMAT_R32_UINT, 0);
	m_pDevCon->IASetInputLayout(m_pInputLayout);
	m_pDevCon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


	m_pDevCon->VSSetShader(m_pVertexShader, nullptr, 0);
	m_pDevCon->VSSetConstantBuffers(0, 1, &m_pMatrixConstantBuffer);

	m_pDevCon->PSSetShader(m_pPixelShader, nullptr, 0);
	m_pDevCon->PSSetConstantBuffers(0, 1, &m_pLightingConstantBuffer);
	ID3D11ShaderResourceView* _pNormal = m_pNormal->GetSRV();
	ID3D11ShaderResourceView* _pDiffuse = m_pDiffuse->GetSRV();
	m_pDevCon->PSSetShaderResources(0, 1, &_pNormal);
	m_pDevCon->PSSetShaderResources(1, 1, &_pDiffuse);

	// Rendern
	m_pDevCon->DrawIndexed(m_Indices, 0, 0);
}

void ShadingDemo::RenderShadows(DirectionalLight* p_pLight)
{

	ShadingDemo_MatrixConstantBuffer _NewMatrixData;

	// Struct mit daten füllen
	_NewMatrixData.ModelViewProjection = p_pLight->GetViewProjectionMatrix();


	// Struct auf die graphikkarte übertragen
	D3D11_MAPPED_SUBRESOURCE _CBMSR;
	m_pDevCon->Map(m_pMatrixConstantBuffer, 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &_CBMSR);
	memcpy(_CBMSR.pData, &_NewMatrixData, sizeof(ShadingDemo_MatrixConstantBuffer));
	m_pDevCon->Unmap(m_pMatrixConstantBuffer, 0);


	// State assignment
	UINT _Stride = sizeof(ShadingDemo_Vertex);
	UINT _Offset = 0;

	m_pDevCon->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &_Stride, &_Offset);
	m_pDevCon->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT::DXGI_FORMAT_R32_UINT, 0);
	m_pDevCon->IASetInputLayout(m_pInputLayout);
	m_pDevCon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


	m_pDevCon->VSSetShader(m_pVertexShader, nullptr, 0);
	m_pDevCon->VSSetConstantBuffers(0, 1, &m_pMatrixConstantBuffer);

	m_pDevCon->PSSetShader(nullptr, nullptr, 0);

	// Rendern
	m_pDevCon->DrawIndexed(m_Indices, 0, 0);
}