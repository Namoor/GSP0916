#pragma once

#include "Texture.h"
#include "Shader.h"

class Material
{
public:
	Material( Shader* p_pInternalShader, ID3D11DeviceContext* p_pDevCon );
	
public:
	Texture* m_pTexture;
	Shader* m_pShader;
	ID3D11DeviceContext* m_pDevCon;

	void Bind();
};