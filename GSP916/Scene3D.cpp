#include "Scene3D.h"
#include "Input.h"


void Scene3D::Init(ID3D11Device* p_pDevice, ID3D11DeviceContext* p_pDevCon)
{
	m_pSpriteBatch = new SpriteBatch();
	m_pSpriteBatch->Init(p_pDevice, p_pDevCon);

	m_pCalibri = new SpriteFont();
	m_pCalibri->Load("Calibri.fnt", p_pDevice);

	m_pFPSDisplay = new FPSDisplay();

	m_pFirstObject = new First3DObject();
	m_pFirstObject->Init(p_pDevice, p_pDevCon);



	m_pCamera = new Camera(XMFLOAT3(0, 0, 0), XMFLOAT3(0, -1, 1), 5);


	Input::SetMousePosition(XMFLOAT2(500, 400));
}

void Scene3D::Update(float p_DeltaTime)
{
	m_pFPSDisplay->Update(p_DeltaTime);

	m_pFirstObject->Update(p_DeltaTime);

	
	m_pCamera->Update(p_DeltaTime);

	
}

void Scene3D::Render(ID3D11Device* p_pDevice, ID3D11DeviceContext* p_pDevCon)
{
	m_pFirstObject->Render(m_pCamera);

	m_pSpriteBatch->Begin();

	m_pFPSDisplay->Render(m_pSpriteBatch, m_pCalibri, 0, 0);

	m_pSpriteBatch->End();
}