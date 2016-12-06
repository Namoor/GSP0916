#include "Texture.h"

const wchar_t* GetWC(const char* c)
{
	const size_t cSize = strlen(c) + 1;
	wchar_t* wc = new wchar_t[cSize];
	size_t ConversionCount = 0;
	mbstowcs_s(&ConversionCount, wc, cSize, c, cSize);

	return wc;
}

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

Texture::Texture(ID3D11Device* p_pDevice, char* p_pFilename)
{
	ID3D11Resource* _pResource;
	CreateWICTextureFromFile(p_pDevice, GetWC(p_pFilename), &_pResource, &m_pSRV);

	ID3D11Texture2D* _pTex;
	_pResource->QueryInterface<ID3D11Texture2D>(&_pTex);

	D3D11_TEXTURE2D_DESC _TD;
	_pTex->GetDesc(&_TD);

	m_X = _TD.Width;
	m_Y = _TD.Height;
}


Texture::Texture(ID3D11Device* p_pDevice, ID3D11ShaderResourceView* p_pSRV)
{
	m_pSRV = p_pSRV;
	ID3D11Resource* _pResource;
	m_pSRV->GetResource(&_pResource);

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