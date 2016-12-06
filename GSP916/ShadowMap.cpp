#include "ShadowMap.h"

ShadowMap::ShadowMap()
{
	m_pDepthStencilView = nullptr;

	m_pDevCon = nullptr;
	m_pDevice = nullptr;
}

void ShadowMap::Init(ID3D11Device* p_pDevice, ID3D11DeviceContext* p_pDevCon, int p_SizeX, int p_SizeY)
{
	m_pDevCon = p_pDevCon;
	m_pDevice = p_pDevice;

	ID3D11Texture2D* _pDepthTexture;

	D3D11_TEXTURE2D_DESC _TextureDesc;
	ZeroMemory(&_TextureDesc, sizeof(_TextureDesc));

	_TextureDesc.Width = p_SizeX;
	_TextureDesc.Height = p_SizeY;
	_TextureDesc.ArraySize = 1;
	_TextureDesc.MipLevels = 1;

	_TextureDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE;
	_TextureDesc.CPUAccessFlags = 0;
	_TextureDesc.Format = DXGI_FORMAT::DXGI_FORMAT_R32_TYPELESS;
	_TextureDesc.MiscFlags = 0;
	_TextureDesc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
	_TextureDesc.SampleDesc.Count = 4; // Try out with 1


	m_pDevice->CreateTexture2D(&_TextureDesc, nullptr, &_pDepthTexture);

	D3D11_DEPTH_STENCIL_VIEW_DESC _DSVDesc;
	ZeroMemory(&_DSVDesc, sizeof(_DSVDesc));

	_DSVDesc.ViewDimension = D3D11_DSV_DIMENSION::D3D11_DSV_DIMENSION_TEXTURE2DMS;
	_DSVDesc.Format = DXGI_FORMAT::DXGI_FORMAT_D32_FLOAT;

	m_pDevice->CreateDepthStencilView(_pDepthTexture, &_DSVDesc, &m_pDepthStencilView);

	ID3D11ShaderResourceView* _pSRV;

	D3D11_SHADER_RESOURCE_VIEW_DESC _SRVDesc;
	ZeroMemory(&_SRVDesc, sizeof(_SRVDesc));

	_SRVDesc.Format = DXGI_FORMAT::DXGI_FORMAT_R32_FLOAT;
	_SRVDesc.ViewDimension = D3D11_SRV_DIMENSION::D3D10_1_SRV_DIMENSION_TEXTURE2DMS;


	m_pDevice->CreateShaderResourceView(_pDepthTexture,&_SRVDesc, &_pSRV);

	m_pTextureView = new Texture(p_pDevice, _pSRV);

	m_Viewport.TopLeftX = 0;
	m_Viewport.TopLeftY = 0;
	m_Viewport.Height = p_SizeY;
	m_Viewport.Width = p_SizeX;

	m_Viewport.MinDepth = 0;
	m_Viewport.MaxDepth = 1;
}


void ShadowMap::Bind()
{
	m_pDevCon->OMSetRenderTargets(0, nullptr, m_pDepthStencilView);
	m_pDevCon->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_FLAG::D3D11_CLEAR_DEPTH, 1.0f, 0);
	m_pDevCon->RSSetViewports(1, &m_Viewport);
}


Texture* ShadowMap::GetTextureView()
{
	return m_pTextureView;
}