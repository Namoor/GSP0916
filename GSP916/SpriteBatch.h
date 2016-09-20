#pragma once

#include "d3dinclude.h"
#include "Rectangle.h"
#include <list>

#define DRAWCOMMANDBATCHSIZE 1024

struct SpriteRenderCommand
{
	Rect m_Destination;
	XMFLOAT4 m_Color;
};


class SpriteBatch
{
public:
	SpriteBatch();

	void Init(ID3D11Device* p_pDevice, ID3D11DeviceContext* p_pDevCon);

	void Begin();
	void Draw(Rect p_Destination, XMFLOAT4 p_Color);
	void End();

private:
	std::list<SpriteRenderCommand> m_DrawCommands;
	int m_DrawCommandCount;

private:
	ID3D11Buffer* m_pVertexBuffer;
	ID3D11Buffer* m_pIndexBuffer;
	ID3D11InputLayout* m_pInputLayout;
	ID3D11VertexShader* m_pVertexShader;
	ID3D11PixelShader* m_pPixelShader;

	ID3D11Device* m_pDevice;
	ID3D11DeviceContext* m_pDevCon;
};