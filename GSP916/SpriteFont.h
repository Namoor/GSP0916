#pragma once

#include "d3dinclude.h"

#include "Texture.h"

struct CharDesc
{
	float m_X;
	float m_Y;
	float m_Width;
	float m_Height;

	float RelativeHeight;
	float RelativeWidth;

	float RelativeOffsetY;
};

class SpriteFont
{
public:
	SpriteFont();

	void Load(char* p_pFilename, ID3D11Device* p_pDevice);
	
	
	Texture* m_pTexture;

	CharDesc m_Chars[256];
};


