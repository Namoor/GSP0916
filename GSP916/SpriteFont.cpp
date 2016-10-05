#include "SpriteFont.h"


SpriteFont::SpriteFont()
{
	m_pTexture = nullptr;
	ZeroMemory(m_Chars, sizeof(m_Chars));
}

void SpriteFont::Load(char* p_pFilename, ID3D11Device* p_pDevice)
{
	int asd = 0;
}