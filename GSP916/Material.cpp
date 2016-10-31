#include "Material.h"



Material::Material( Shader* p_pInternalShader, ID3D11DeviceContext* p_pDevCon )
{
	m_pDevCon = p_pDevCon;
	m_pShader = p_pInternalShader;
	m_pTexture = nullptr;
}


void Material::Bind()
{
	m_pShader->Bind();

	
	ID3D11ShaderResourceView* _pSRV;
	if (m_pTexture != nullptr)
		_pSRV = m_pTexture->GetSRV();
	else
		_pSRV = nullptr;

	m_pDevCon->PSSetShaderResources( 0, 1, &_pSRV );
}