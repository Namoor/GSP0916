#pragma once

#include "IScene.h"
#include "SpriteBatch.h"
#include "FPSDisplay.h"
#include "SpriteFont.h"
#include "First3DObject.h"

#include "Camera.h"

#include "GameObject.h"

#include "ShadingDemo.h"
#include "DirectionalLight.h"
#include "PostProcessingRenderTarget.h"

#include "PostProcessingEffect.h"

class Scene3D : public IScene
{
public:
	virtual void Init(ID3D11Device* p_pDevice, ID3D11DeviceContext* p_pDevCon, ID3D11RenderTargetView* m_pScreen, ID3D11DepthStencilView* m_pDepthStencil) override;

	virtual void Update(float p_DeltaTime) override;
	virtual void Render(ID3D11Device* p_pDevice, ID3D11DeviceContext* p_pDevCon) override;

private:
	SpriteFont* m_pCalibri;
	SpriteBatch* m_pSpriteBatch;
	FPSDisplay* m_pFPSDisplay;

	ID3D11DeviceContext* m_pDevCon;

	First3DObject* m_pFirstObject;

	Camera* m_pCamera;

	GameObject** m_pGOs;

	ShadingDemo* m_pDemo;
	DirectionalLight* m_pLight;


	ID3D11RenderTargetView* m_pScreen;
	ID3D11DepthStencilView* m_pDepthStencil;

	PostProcessingRenderTarget* m_pMainSceneRender;
	PostProcessingRenderTarget* m_pOffScreenRender;

	PostProcessingEffect* m_pGreyScale;
	PostProcessingEffect* m_pBlurSimple;
	PostProcessingEffect* m_pBlurX;
	PostProcessingEffect* m_pBlurY;

	Texture* m_pTestTexture;

	float TimePassed;
};