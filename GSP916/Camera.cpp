#include "Camera.h"


Camera::Camera(XMFLOAT3 p_TargetPosition, XMFLOAT3 p_ViewDirection, float Distance)
{
	m_Target = XMLoadFloat3( &p_TargetPosition);
	m_Direction = XMLoadFloat3(&p_ViewDirection);
	m_Distance = Distance;

	m_ProjectionMatrix = XMMatrixPerspectiveFovLH(XMConvertToRadians(60), 800.0f / 600.0f, 0.1f, 1000.0f);

	XMFLOAT3 _Up(0, 1, 0);
	m_ViewMatrix = XMMatrixLookAtLH(m_Target - m_Direction * m_Distance, m_Target, XMLoadFloat3(&_Up));
}

void Camera::RotateX(float p_AngleInDegree, bool p_InWorldSpace)
{
	XMMATRIX _Rotation = XMMatrixRotationX(XMConvertToRadians(p_AngleInDegree));
	if (p_InWorldSpace)
	{
		XMMATRIX _Temp = _Rotation * m_ViewMatrix;
		m_ViewMatrix = _Temp;
	}
	else
	{
		XMMATRIX _Temp = m_ViewMatrix * _Rotation;
		m_ViewMatrix = _Temp;
	}
}

void Camera::RotateY(float p_AngleInDegree, bool p_InWorldSpace)
{
	XMMATRIX _Rotation = XMMatrixRotationY(XMConvertToRadians(p_AngleInDegree));
	if (p_InWorldSpace)
	{
		XMMATRIX _Temp = _Rotation * m_ViewMatrix;
		m_ViewMatrix = _Temp;
	}
	else
	{
		XMMATRIX _Temp = m_ViewMatrix * _Rotation;
		m_ViewMatrix = _Temp;
	}
}

void Camera::RotateZ(float p_AngleInDegree, bool p_InWorldSpace)
{
	XMMATRIX _Rotation = XMMatrixRotationZ(XMConvertToRadians(p_AngleInDegree));
	if (p_InWorldSpace)
	{
		XMMATRIX _Temp = _Rotation * m_ViewMatrix;
		m_ViewMatrix = _Temp;
	}
	else
	{
		XMMATRIX _Temp = m_ViewMatrix * _Rotation;
		m_ViewMatrix = _Temp;
	}
}


XMMATRIX Camera::GetViewMatrix()
{
	return m_ViewMatrix;
}

XMMATRIX Camera::GetProjectionMatrix()
{
	return m_ProjectionMatrix;
}


void Camera::operator delete(void* p_Ptr)
{
	unsigned char _Dif = *((char*)p_Ptr - 1);
	p_Ptr = (char*)p_Ptr - _Dif;
	free(p_Ptr);
}


void* Camera::operator new(unsigned int Size)
{
	void* _Ptr = malloc(Size + 16);
	void* _PtrNew = (void*)(((int)_Ptr & 0xFFFFFFF0) + 0x00000010);

	unsigned char _Dif = (int)_PtrNew - (int)_Ptr;
	*((char*)_PtrNew - 1) = _Dif;

	return _PtrNew;

}