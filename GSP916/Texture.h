#pragma once
#include "d3dinclude.h"

class Texture
{
public:
	Texture(ID3D11Device* p_pDevice, wchar_t* p_pFilename);
	Texture(ID3D11Device* p_pDevice, char* p_pFilename);


	Texture(ID3D11Device* p_pDevice, ID3D11ShaderResourceView* p_pSRV);

	ID3D11ShaderResourceView* GetSRV();
	POINT GetSize();

	int GetXSize();
	int GetYSize();
private:
	ID3D11ShaderResourceView* m_pSRV;
	int m_X;
	int m_Y;
};