#include "Transform.h"



Transform::Transform(XMFLOAT3 p_Position, XMFLOAT3 p_Rotation, XMFLOAT3 p_Scale)
{
	Position = XMLoadFloat3(&p_Position);
	Rotation = XMLoadFloat3(&p_Rotation);
	Scale = XMLoadFloat3(&p_Scale);

	m_WorldMatrix = XMMatrixScalingFromVector(Scale)
		* XMMatrixRotationRollPitchYawFromVector(Rotation)
		* XMMatrixTranslationFromVector(Position);
}

void Transform::UpdateMatrix()
{
	XMMATRIX m_TransInv = XMMatrixInverse( nullptr, m_WorldMatrix );
	m_TransInv = XMMatrixTranspose( m_TransInv );
}

void Transform::RotateAround(XMFLOAT3 Axis, float p_AngleInDegree, bool p_LocalRotationAnchor)
{
	if (p_LocalRotationAnchor)
	{
		XMFLOAT3 _Origin3(0, 0, 0);
		XMVECTOR _Origin = XMLoadFloat3(&_Origin3);
		XMVECTOR _Transformed = XMVector3Transform(_Origin, m_WorldMatrix);

		XMMATRIX _ObjectToRoot = XMMatrixTranslationFromVector(_Transformed * -1);
		XMMATRIX _RootToObject = XMMatrixTranslationFromVector(_Transformed);

		XMMATRIX _Rotation = XMMatrixRotationAxis(XMLoadFloat3(&Axis), XMConvertToRadians(p_AngleInDegree));

		m_WorldMatrix = m_WorldMatrix * _ObjectToRoot * _Rotation * _RootToObject;
	}
	else
	{
		XMMATRIX _Rotation = XMMatrixRotationAxis(XMLoadFloat3(&Axis), XMConvertToRadians(p_AngleInDegree));

		m_WorldMatrix = m_WorldMatrix * _Rotation;
	}
	UpdateMatrix();
}

void Transform::Move(XMFLOAT3 p_Movement, bool p_InWorldSpace)
{
	if (p_InWorldSpace)
	{
		XMMATRIX _Translation = XMMatrixTranslationFromVector(XMLoadFloat3(&p_Movement));

		m_WorldMatrix = m_WorldMatrix * _Translation;
	}
	else
	{
		XMMATRIX _Translation = XMMatrixTranslationFromVector(XMLoadFloat3(&p_Movement));

		m_WorldMatrix = _Translation * m_WorldMatrix;
	}
	UpdateMatrix( );
}

XMMATRIX Transform::GetMatrix()
{
	return m_WorldMatrix;
}


XMMATRIX Transform::GetInvertTranspose()
{
	// Transpose:
	// Scale unaffected
	// Rotation inverted
	// Translation breaks

	// invertieren
	// scale inverted
	// rotation inverted
	// translation inverted

	// transpInv
	// scale inverted
	// rotation unaffected
	// translation breaks

	return m_TransInv;

}


XMVECTOR Transform::GetRight()
{
	XMFLOAT3 _Root3(0, 0, 0);
	XMFLOAT3 _Right3(1, 0, 0);

	XMVECTOR _Root = XMLoadFloat3(&_Root3);
	XMVECTOR _Right = XMLoadFloat3(&_Right3);

	_Root = XMVector3Transform(_Root, m_WorldMatrix);
	_Right = XMVector3Transform(_Right, m_WorldMatrix);

	return _Right - _Root;
}

XMVECTOR Transform::GetUp()
{
	XMFLOAT3 _Root3(0, 0, 0);
	XMFLOAT3 _Up3(0, 0, 1);

	XMVECTOR _Root = XMLoadFloat3(&_Root3);
	XMVECTOR _Up = XMLoadFloat3(&_Up3);

	_Root = XMVector3Transform(_Root, m_WorldMatrix);
	_Up = XMVector3Transform(_Up, m_WorldMatrix);

	return _Up - _Root;
}

XMVECTOR Transform::GetForward()
{
	XMFLOAT3 _Root3(0, 0, 0);
	XMFLOAT3 _Forward3(0, 1, 0);

	XMVECTOR _Root = XMLoadFloat3(&_Root3);
	XMVECTOR _Forward = XMLoadFloat3(&_Forward3);

	_Root = XMVector3Transform(_Root, m_WorldMatrix);
	_Forward = XMVector3Transform(_Forward, m_WorldMatrix);

	return _Forward - _Root;
}

void Transform::operator delete(void* p_Ptr)
{
	unsigned char _Dif = *((char*)p_Ptr - 1);
	p_Ptr = (char*)p_Ptr - _Dif;
	free(p_Ptr);
}


void* Transform::operator new(size_t Size)
{
	void* _Ptr = malloc(Size + 16);
void* _PtrNew = (void*)(((int)_Ptr & 0xFFFFFFF0) + 0x00000010);

unsigned char _Dif = (int)_PtrNew - (int)_Ptr;
*((char*)_PtrNew - 1) = _Dif;

return _PtrNew;

}