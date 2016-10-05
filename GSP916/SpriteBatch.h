#pragma once

#include "d3dinclude.h"
#include "Rectangle.h"
#include <list>
#include "Texture.h"
#include "SpriteFont.h"

#define DRAWCOMMANDBATCHSIZE 1024

struct SpriteRenderCommand
{
	Rect m_Destination;
	Rect m_Source;
	XMFLOAT4 m_Color;
};


class SpriteBatch
{
public:
	SpriteBatch();

	void Init(ID3D11Device* p_pDevice, ID3D11DeviceContext* p_pDevCon);

	void Begin();
	void End();

	void Draw(Texture* p_pTexture, Rect p_Destination, Rect p_Source, XMFLOAT4 p_Color);
	void Draw(Texture* p_pTexture, Rect p_Destination, XMFLOAT4 p_Color);
	void Draw(Texture* p_pTexture, Rect p_Destination, Rect p_Source);
	void Draw(Texture* p_pTexture, Rect p_Destination);

	void DrawString(SpriteFont* p_pFont, const char* p_pText, float p_X, float p_Y, float p_Size, XMFLOAT4 p_Color);

private:
	std::list<SpriteRenderCommand> m_DrawCommands;
	int m_DrawCommandCount;

private:
	ID3D11Buffer* m_pVertexBuffer;
	ID3D11Buffer* m_pIndexBuffer;
	ID3D11InputLayout* m_pInputLayout;
	ID3D11VertexShader* m_pVertexShader;
	ID3D11PixelShader* m_pPixelShader;

	ID3D11BlendState* m_pBlendState;

	ID3D11Device* m_pDevice;
	ID3D11DeviceContext* m_pDevCon;
	



	Texture* m_pCurrentTexture;
};