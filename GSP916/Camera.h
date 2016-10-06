#pragma once

#include "d3dinclude.h"

	class Camera
{
public:
	Camera(XMFLOAT3 p_TargetPosition, XMFLOAT3 p_ViewDirection, float Distance);

	void RotateX(float p_AngleInDegree, bool p_InWorldSpace);
	void RotateY(float p_AngleInDegree, bool p_InWorldSpace);
	void RotateZ(float p_AngleInDegree, bool p_InWorldSpace);
public:
	XMMATRIX GetViewMatrix();
	XMMATRIX GetProjectionMatrix();


private:
	XMMATRIX m_ProjectionMatrix;
	XMMATRIX m_ViewMatrix;

	XMVECTOR m_Target;
	XMVECTOR m_Direction;
	float m_Distance;

public:
	void operator delete(void* p_Ptr);
	void* operator new(unsigned int Size);
};