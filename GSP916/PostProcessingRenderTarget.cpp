#include "PostProcessingRenderTarget.h"


void PostProcessingRenderTarget::Init(ID3D11Device* p_pDevice, ID3D11DeviceContext* p_pDevCon, int p_SizeX, int p_SizeY)
{
	//ID3D11DepthStencilView* m_pDSV;
	//ID3D11RenderTargetView* m_pRTV;
	//
	//Texture* m_pTexture;
	//
	//D3D11_VIEWPORT m_ViewPort;

	m_pDevice = p_pDevice;
	m_pDevCon = p_pDevCon;

	// -------------------------------------------------------- Depth --------------------------------------------------------

	ID3D11Texture2D* _pDepthTexture;

	D3D11_TEXTURE2D_DESC _TextureDesc_Depth;
	ZeroMemory(&_TextureDesc_Depth, sizeof(_TextureDesc_Depth));

	_TextureDesc_Depth.Width = p_SizeX;
	_TextureDesc_Depth.Height = p_SizeY;
	_TextureDesc_Depth.ArraySize = 1;
	_TextureDesc_Depth.MipLevels = 1;

	_TextureDesc_Depth.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_DEPTH_STENCIL;
	_TextureDesc_Depth.CPUAccessFlags = 0;
	_TextureDesc_Depth.Format = DXGI_FORMAT::DXGI_FORMAT_R32_TYPELESS;
	_TextureDesc_Depth.MiscFlags = 0;
	_TextureDesc_Depth.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
	_TextureDesc_Depth.SampleDesc.Count = 4; // Try out with 1


	m_pDevice->CreateTexture2D(&_TextureDesc_Depth, nullptr, &_pDepthTexture);

	D3D11_DEPTH_STENCIL_VIEW_DESC _DSVDesc;
	ZeroMemory(&_DSVDesc, sizeof(_DSVDesc));

	_DSVDesc.ViewDimension = D3D11_DSV_DIMENSION::D3D11_DSV_DIMENSION_TEXTURE2DMS;
	_DSVDesc.Format = DXGI_FORMAT::DXGI_FORMAT_D32_FLOAT;

	m_pDevice->CreateDepthStencilView(_pDepthTexture, &_DSVDesc, &m_pDSV);


	// ------------------------------------------------------- Color -------------------------------------------------------------
	ID3D11Texture2D* _pColorTexture;

	D3D11_TEXTURE2D_DESC _TextureDesc_Color;
	ZeroMemory(&_TextureDesc_Color, sizeof(_TextureDesc_Color));

	_TextureDesc_Color.Width = p_SizeX;
	_TextureDesc_Color.Height = p_SizeY;
	_TextureDesc_Color.ArraySize = 1;
	_TextureDesc_Color.MipLevels = 1;

	_TextureDesc_Color.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_RENDER_TARGET | D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE;
	_TextureDesc_Color.CPUAccessFlags = 0;
	_TextureDesc_Color.Format = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_TYPELESS;
	_TextureDesc_Color.MiscFlags = 0;
	_TextureDesc_Color.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
	_TextureDesc_Color.SampleDesc.Count = 4; // Try out with 1


	m_pDevice->CreateTexture2D(&_TextureDesc_Color, nullptr, &_pColorTexture);

	D3D11_RENDER_TARGET_VIEW_DESC _RTVDesc;
	ZeroMemory(&_RTVDesc, sizeof(_RTVDesc));

	_RTVDesc.ViewDimension = D3D11_RTV_DIMENSION::D3D11_RTV_DIMENSION_TEXTURE2DMS;
	_RTVDesc.Format = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;

	m_pDevice->CreateRenderTargetView(_pColorTexture, &_RTVDesc, &m_pRTV);
	
	ID3D11ShaderResourceView* _pSRV_Color;
	
	D3D11_SHADER_RESOURCE_VIEW_DESC _pSRVDesc_Color;
	ZeroMemory(&_pSRVDesc_Color, sizeof(_pSRVDesc_Color));
	
	_pSRVDesc_Color.Format = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
	_pSRVDesc_Color.ViewDimension = D3D11_SRV_DIMENSION::D3D10_1_SRV_DIMENSION_TEXTURE2DMS;
	
	
	m_pDevice->CreateShaderResourceView(_pColorTexture, &_pSRVDesc_Color, &_pSRV_Color);

	m_pTexture = new Texture(p_pDevice, _pSRV_Color);


	m_ViewPort.TopLeftX = 0;
	m_ViewPort.TopLeftY = 0;
	m_ViewPort.Height = p_SizeY;
	m_ViewPort.Width = p_SizeX;

	m_ViewPort.MinDepth = 0;
	m_ViewPort.MaxDepth = 1;

	int asd = 0;
}

void PostProcessingRenderTarget::Bind()
{
	m_pDevCon->RSSetViewports(1, &m_ViewPort);
	m_pDevCon->OMSetRenderTargets(1, &m_pRTV, m_pDSV);

	m_pDevCon->ClearDepthStencilView(m_pDSV, D3D11_CLEAR_FLAG::D3D11_CLEAR_DEPTH, 1.0f, 0);

	float _Color[4];
	_Color[0] = 1.0;
	_Color[1] = 1.0;
	_Color[2] = 1.0;
	_Color[3] = 0;
	m_pDevCon->ClearRenderTargetView(m_pRTV, _Color);

}

Texture* PostProcessingRenderTarget::GetTexture()
{
	return m_pTexture;
}