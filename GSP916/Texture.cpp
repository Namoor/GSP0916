#include "Texture.h"


Texture::Texture(ID3D11Device* p_pDevice, wchar_t* p_pFilename)
{
	ID3D11Resource* _pResource;
	CreateWICTextureFromFile(p_pDevice, p_pFilename, &_pResource, &m_pSRV);

	ID3D11Texture2D* _pTex;
	_pResource->QueryInterface<ID3D11Texture2D>(&_pTex);

	D3D11_TEXTURE2D_DESC _TD;
	_pTex->GetDesc(&_TD);

	m_X = _TD.Width;
	m_Y = _TD.Height;
}

ID3D11ShaderResourceView* Texture::GetSRV()
{
	return m_pSRV;
}

POINT Texture::GetSize()
{
	POINT _P;
	_P.x = m_X;
	_P.y = m_Y;
	return _P;
}


int Texture::GetXSize()
{
	return m_X;
}

int Texture::GetYSize()
{
	return m_Y;
}