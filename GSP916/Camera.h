#pragma once

#include "d3dinclude.h"

class Camera
{
public:
	Camera(XMFLOAT3 p_TargetPosition, XMFLOAT3 p_ViewDirection, float Distance);

	void RotateX(float p_AngleInDegree, bool p_InWorldSpace);
	void RotateY(float p_AngleInDegree, bool p_InWorldSpace);
	void RotateZ(float p_AngleInDegree, bool p_InWorldSpace);

	void Move(XMFLOAT3 p_Movement, bool p_InWorldSpace);
public:
	XMMATRIX GetViewMatrix();
	XMMATRIX GetProjectionMatrix();
	XMFLOAT4 GetPositionAsFloat4();

	void Update(float p_DeltaTime);

private:
	XMMATRIX m_ProjectionMatrix;
	XMMATRIX m_ViewMatrix;

	XMVECTOR m_Target;
	XMVECTOR m_Direction;
	XMVECTOR m_Right;
	XMVECTOR m_Up;

	float m_Distance;

	XMFLOAT2 m_SavedMousePosition;
public:
	void operator delete(void* p_Ptr);
	void* operator new(size_t Size);
};