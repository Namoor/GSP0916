#include "Scene3D.h"
#include "Input.h"
#include "Shader.h"


void Scene3D::Init( ID3D11Device* p_pDevice, ID3D11DeviceContext* p_pDevCon, ID3D11RenderTargetView* p_pScreen, ID3D11DepthStencilView* p_pDepthStencil)
{
	m_pScreen = p_pScreen;
	m_pDepthStencil = p_pDepthStencil;
	m_pDevCon = p_pDevCon;

	m_pTestTexture = new Texture(p_pDevice, "ColorPreview.png");
	m_pMainSceneRender = new PostProcessingRenderTarget();
	m_pMainSceneRender->Init(p_pDevice, p_pDevCon, 800, 600);

	m_pOffScreenRender = new PostProcessingRenderTarget();
	m_pOffScreenRender->Init(p_pDevice, p_pDevCon, 800, 600);

	m_pGreyScale = new PostProcessingEffect();
	m_pGreyScale->Init(p_pDevice, p_pDevCon, L"BlackWhite.hlsl");

	m_pBlurSimple = new PostProcessingEffect();
	m_pBlurSimple->Init(p_pDevice, p_pDevCon, L"Blur.hlsl");


	m_pBlurX = new PostProcessingEffect();
	m_pBlurX->Init(p_pDevice, p_pDevCon, L"BlurX.hlsl");

	m_pBlurY = new PostProcessingEffect();
	m_pBlurY->Init(p_pDevice, p_pDevCon, L"BlurY.hlsl");

	m_pSpriteBatch = new SpriteBatch();
	m_pSpriteBatch->Init( p_pDevice, p_pDevCon );

	m_pCalibri = new SpriteFont();
	m_pCalibri->Load( "Calibri.fnt", p_pDevice );

	m_pFPSDisplay = new FPSDisplay();

	m_pFirstObject = new First3DObject();
	m_pFirstObject->Init( p_pDevice, p_pDevCon );

	m_pGOs = new GameObject*[16 * 16 * 16];

	Mesh* _pMesh = Mesh::CreateCubeMesh( p_pDevice, p_pDevCon );

	//Mesh* _pTestObj = Mesh::CreateMeshFromObj(p_pDevice, p_pDevCon, "duck.obj");

	// Material
	Shader* _pDiffuseShader = new Shader();
	_pDiffuseShader->LoadShader( L"First3DShader.hlsl", "VShader", "PShader", p_pDevice, p_pDevCon );

	_pDiffuseShader->AddInputLayoutEntry( "POSITION", 0, 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT );
	_pDiffuseShader->AddInputLayoutEntry( "COLOR", 0, 12, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT );
	_pDiffuseShader->AddInputLayoutEntry( "TEXCOORD", 0, 28, DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT );
	_pDiffuseShader->AddInputLayoutEntry( "NORMAL", 0, 36, DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT );

	_pDiffuseShader->FinalizeInputLayout();

	Material* _pSmileyMaterial = new Material( _pDiffuseShader, p_pDevCon );

	Texture* _pSmileyTexture = new Texture( p_pDevice, "HappyFace.bmp" );
	_pSmileyMaterial->m_pTexture = _pSmileyTexture;

	Material* _pWoodMaterial = new Material( _pDiffuseShader, p_pDevCon );

	Texture* _pWoodTexture = new Texture( p_pDevice, "Wood.bmp" );
	_pWoodMaterial->m_pTexture = _pWoodTexture;

	m_pLight = new DirectionalLight();
	m_pLight->Init(p_pDevice, p_pDevCon);

	//for (int x = 0; x < 1; x++)
	//for (int y = 0; y < 1; y++)
	//for (int z = 0; z < 1; z++)
	//{
	//	GameObject* _pGO1 = new GameObject();
	//	_pGO1->Init( p_pDevice, p_pDevCon, _pTestObj, _pSmileyMaterial );
	//
	//	_pGO1->m_pTransform->Move( XMFLOAT3( x * 2, y * 2, z * 2 ), true );
	//	
	//
	//
	//	m_pGOs[x + y * 16 + z * 256] = _pGO1;
	//}



	TimePassed = 0;

	//m_pGO2->m_pTransform->Move(XMFLOAT3(2, 0, 0), true);

	m_pCamera = new Camera( XMFLOAT3( 0, 0, 0 ), XMFLOAT3( 0, -1, 1 ), 5 );


	Input::SetMousePosition( XMFLOAT2( 500, 400 ) );

	m_pDemo = new ShadingDemo();
	m_pDemo->Init(p_pDevice, p_pDevCon, 255);
}

void Scene3D::Update( float p_DeltaTime )
{
	TimePassed += p_DeltaTime * 0.1f;

	m_pFPSDisplay->Update( p_DeltaTime );

	m_pFirstObject->Update( p_DeltaTime );


	m_pCamera->Update( p_DeltaTime );

	m_pDemo->Update(p_DeltaTime);
}

void Scene3D::Render( ID3D11Device* p_pDevice, ID3D11DeviceContext* p_pDevCon )
{

	m_pLight->SetDirection(XMFLOAT3(sin(TimePassed), 0.5f, cos(TimePassed)));
	//m_pFirstObject->Render(m_pCamera);

	//m_pGO1->Render( m_pCamera );
	//m_pGO2->Render( m_pCamera );

	//for(int x = 0; x < 1; x++)
	//	m_pGOs[x]->Render( m_pCamera );

	// Shadowmap rendern ------------------
	m_pLight->GetShadowMap()->Bind();

	m_pDemo->RenderShadows(m_pLight);


	// Bind PostProcessingRenderTarget
	m_pMainSceneRender->Bind();
	// Scene Rendern mithilfe der ShadowMap
	m_pDemo->Render(m_pCamera, m_pLight);

	
	// Bind Screen
	//m_pOffScreenRender->Bind();
	//
	//// Postprocess Rendern
	//m_pBlurY->Blit(m_pMainSceneRender->GetTexture()); //m_pTestTexture); //
	//
	//float _Color[4];
	//_Color[0] = 1.0; // (rand() % 256) / 256.0f;
	//_Color[1] = 1.0; // (rand() % 256) / 256.0f;
	//_Color[2] = 1.0; // (rand() % 256) / 256.0f;
	//_Color[3] = 1;
	//
	//m_pDevCon->ClearRenderTargetView(m_pScreen, _Color);
	p_pDevCon->OMSetRenderTargets(1, &m_pScreen, m_pDepthStencil);
	//// Postprocess Rendern
	m_pBlurX->Blit(m_pMainSceneRender->GetTexture()); //m_pTestTexture); //


	//m_pMainSceneRender->Bind();
	//// Postprocess Rendern
	//m_pGreyScale->Blit(m_pMainSceneRender->GetTexture()); //m_pTestTexture); //


	m_pSpriteBatch->Begin();

	m_pFPSDisplay->Render( m_pSpriteBatch, m_pCalibri, 0, 0 );

	m_pSpriteBatch->Draw(m_pMainSceneRender->GetTexture(), Rect(0, 0, 400, 400));
	//m_pSpriteBatch->Draw(m_pMainSceneRender->GetTexture(), Rect(200, 0, 400, 400));
	//m_pSpriteBatch->Draw(m_pMainSceneRender->GetTexture(), Rect(0, 200, 400, 400));
	//m_pSpriteBatch->Draw(m_pMainSceneRender->GetTexture(), Rect(200, 200, 400, 400));

	//m_pSpriteBatch->Draw(m_pLight->GetShadowMap()->GetTextureView(), Rect(0, 0, 400, 400));

	m_pSpriteBatch->End();
}