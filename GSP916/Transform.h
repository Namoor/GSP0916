#pragma once

#include "d3dinclude.h"

class Transform
{
public:
	Transform(XMFLOAT3 p_Position, XMFLOAT3 p_Rotation, XMFLOAT3 p_Scale);
public:
	void Move(XMFLOAT3 p_Movement, bool p_InWorldSpace);

	void RotateAround(XMFLOAT3 Axis, float p_AngleInDegree, bool p_LocalRotationAnchor);

	XMVECTOR GetRight();
	XMVECTOR GetUp();
	XMVECTOR GetForward();

public:
	XMMATRIX GetMatrix();

	XMMATRIX GetInvertTranspose();

private:
	XMMATRIX m_WorldMatrix;
	XMVECTOR Position;
	XMVECTOR Scale;
	XMVECTOR Rotation;



public:
	void operator delete(void* p_Ptr);
	void* operator new(size_t Size);
};