#pragma once
#include "IScene.h"

class Scene2D : public IScene
{
public:
	virtual void Init(ID3D11Device* p_pDevice, ID3D11DeviceContext* p_pDevCon) override;

	virtual void Update() override;
	virtual void Render(ID3D11Device* p_pDevice, ID3D11DeviceContext* p_pDevCon) override;

private:
	ID3D11Device* m_pDevice;
	ID3D11DeviceContext* m_pDevCon;

private:
	ID3D11Buffer* m_pVBuffer;
	ID3D11Buffer* m_pIBuffer;
	ID3D11VertexShader* m_pVertexShader;
	ID3D11PixelShader* m_pPixelShader;
	ID3D11InputLayout* m_pInputLayout;

	ID3D11Buffer* m_pConstantBuffer;
};