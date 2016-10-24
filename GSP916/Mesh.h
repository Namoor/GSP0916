#pragma once

#include "d3dinclude.h"
#include <list>

class Mesh
{
public:
	void Init(int p_VertexCount, int p_ByteSizePerVertex, int p_IndexCount, ID3D11Device* p_pDevice, ID3D11DeviceContext* p_pDevCon);
	void SetData(void* VertexData, void* IndexData);

	void AddInputLayoutEntry(char* p_SemanticName, int p_SemanticIndex, int p_ByteOffset, DXGI_FORMAT p_Format);
	void FinalizeInputLayout();

	void Render();

private:
	ID3D11Buffer* m_pVertexBuffer;
	ID3D11Buffer* m_pIndexBuffer;
	std::list<D3D11_INPUT_ELEMENT_DESC> m_pInputElements;

	ID3D11InputLayout* m_pInputLayout;

	int m_VertexCount;
	int m_ByteSizePerVertex;
	int m_IndexCount;

	ID3D11Device* m_pDevice;
	ID3D11DeviceContext* m_pDevCon;
};