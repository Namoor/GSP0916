#pragma once

#include "d3dinclude.h"

#include "Texture.h"

class PostProcessingEffect
{
public:
	void Init(ID3D11Device* p_pDevice, ID3D11DeviceContext* p_pDevCon, wchar_t* ShaderFileName);

	void Blit(Texture* m_pSource);

private:
	ID3D11Buffer* m_pVertexBuffer;
	ID3D11Buffer* m_pIndexBuffer;

	ID3D11PixelShader* m_pPixelShader;
	ID3D11VertexShader* m_pVertexShader;

	ID3D11Device* m_pDevice;
	ID3D11DeviceContext* m_pDevCon;

	ID3D11InputLayout* m_pInputLayout;

};
