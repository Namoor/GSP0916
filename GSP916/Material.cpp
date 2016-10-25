#include "Material.h"


void Material::LoadShader(wchar_t* p_ShaderFile, char* p_VertexFunctionName, char* p_PixelFunctionName, ID3D11Device* p_pDevice, ID3D11DeviceContext* p_pDevCon)
{
	m_pDevice = p_pDevice;
	m_pDevCon = p_pDevCon;

	ID3DBlob* _pError;
	ID3DBlob* _pPixelShader;

	// Vertex Shader
	if (D3DCompileFromFile(p_ShaderFile, nullptr, nullptr, p_VertexFunctionName, "vs_5_0", 0, 0, &m_pVertexShaderBlob, &_pError) != S_OK)
	{
		MessageBox(0, (char*)_pError->GetBufferPointer(), "Fehler im Vertexshader", MB_OK);
		PostQuitMessage(0);
		return;
	}
	m_pDevice->CreateVertexShader(m_pVertexShaderBlob->GetBufferPointer(), m_pVertexShaderBlob->GetBufferSize(), nullptr, &m_pVertexShader);

	// Pixel Shader
	if (D3DCompileFromFile(p_ShaderFile, nullptr, nullptr, p_PixelFunctionName, "ps_5_0", 0, 0, &_pPixelShader, &_pError) != S_OK)
	{
		MessageBox(0, (char*)_pError->GetBufferPointer(), "Fehler im Pixelshader", MB_OK);
		PostQuitMessage(0);
		return;
	}
	m_pDevice->CreatePixelShader(_pPixelShader->GetBufferPointer(), _pPixelShader->GetBufferSize(), nullptr, &m_pPixelShader);
}


void Material::AddInputLayoutEntry(char* p_SemanticName, int p_SemanticIndex, int p_ByteOffset, DXGI_FORMAT p_Format)
{
	D3D11_INPUT_ELEMENT_DESC _IED;

	_IED.SemanticName = p_SemanticName;
	_IED.SemanticIndex = p_SemanticIndex;

	_IED.AlignedByteOffset = p_ByteOffset;
	_IED.Format = p_Format;

	_IED.InputSlot = 0;
	_IED.InstanceDataStepRate = 0;
	_IED.InputSlotClass = D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA;


	m_InputElements.push_back(_IED);
}

void Material::FinalizeInputLayout()
{

	D3D11_INPUT_ELEMENT_DESC* _IED = new D3D11_INPUT_ELEMENT_DESC[m_InputElements.size()];

	int i = 0;
	for (auto Iterator = m_InputElements.begin(); Iterator != m_InputElements.end(); Iterator++)
	{
		_IED[i] = *Iterator;
		i++;
	}

	char* _Data = (char*)m_pVertexShaderBlob->GetBufferPointer();
	int _DataC = m_pVertexShaderBlob->GetBufferSize();

	m_pDevice->CreateInputLayout(_IED, m_InputElements.size(), m_pVertexShaderBlob->GetBufferPointer(), m_pVertexShaderBlob->GetBufferSize(), &m_pInputLayout);

}


void Material::Bind()
{
	m_pDevCon->IASetInputLayout(m_pInputLayout);

	m_pDevCon->PSSetShader(m_pPixelShader, nullptr, 0);

	m_pDevCon->VSSetShader(m_pVertexShader, nullptr, 0);
}