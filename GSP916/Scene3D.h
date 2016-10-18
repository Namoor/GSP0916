#pragma once

#include "IScene.h"
#include "SpriteBatch.h"
#include "FPSDisplay.h"
#include "SpriteFont.h"
#include "First3DObject.h"

#include "Camera.h"

class Scene3D : public IScene
{
public:
	virtual void Init(ID3D11Device* p_pDevice, ID3D11DeviceContext* p_pDevCon) override;

	virtual void Update(float p_DeltaTime) override;
	virtual void Render(ID3D11Device* p_pDevice, ID3D11DeviceContext* p_pDevCon) override;

private:
	SpriteFont* m_pCalibri;
	SpriteBatch* m_pSpriteBatch;
	FPSDisplay* m_pFPSDisplay;

	First3DObject* m_pFirstObject;

	Camera* m_pCamera;

};