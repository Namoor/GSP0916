#pragma once

#include "d3dinclude.h"
#include <list>

class Shader
{
public:
	void LoadShader(wchar_t* p_ShaderFile, char* p_VertexFunctionName, char* p_PixelFunctionName, ID3D11Device* p_pDevice, ID3D11DeviceContext* p_pDevCon);


	void AddInputLayoutEntry(char* p_SemanticName, int p_SemanticIndex, int p_ByteOffset, DXGI_FORMAT p_Format);
	void FinalizeInputLayout();

	void Bind();

private:
	ID3D11VertexShader* m_pVertexShader;
	ID3D11PixelShader* m_pPixelShader;

	std::list<D3D11_INPUT_ELEMENT_DESC> m_InputElements;

	ID3D11InputLayout* m_pInputLayout;

	ID3D11Device* m_pDevice;
	ID3D11DeviceContext* m_pDevCon;

	ID3DBlob* m_pVertexShaderBlob;
};