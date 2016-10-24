#include "Camera.h"
#include "Input.h"

Camera::Camera(XMFLOAT3 p_TargetPosition, XMFLOAT3 p_ViewDirection, float Distance)
{
	m_Target = XMLoadFloat3( &p_TargetPosition);


	m_Direction = XMLoadFloat3(&p_ViewDirection);
	m_Direction = XMVector3Normalize(m_Direction);

	m_Distance = Distance;

	m_ProjectionMatrix = XMMatrixPerspectiveFovLH(XMConvertToRadians(60), 800.0f / 600.0f, 0.1f, 1000.0f);

	XMFLOAT3 _Up3(0, 1, 0);

	XMVECTOR _Up = XMLoadFloat3(&_Up3);
	m_Right = XMVector3Cross(_Up, m_Direction);
	m_Right = XMVector3Normalize(m_Right);
	m_Up = XMVector3Cross(m_Direction, m_Right);
	m_Up = XMVector3Normalize(m_Up);

	m_ViewMatrix = XMMatrixLookAtLH(m_Target - m_Direction * m_Distance, m_Target, m_Up);
}

void Camera::RotateX(float p_AngleInDegree, bool p_InWorldSpace)
{
	XMMATRIX _Rotation;
	
	if(p_InWorldSpace)
		_Rotation = XMMatrixRotationX(XMConvertToRadians(p_AngleInDegree));
	else
		_Rotation = XMMatrixRotationAxis(m_Right, XMConvertToRadians(p_AngleInDegree));


	m_Direction = XMVector3Transform(m_Direction, _Rotation);
	m_Up = XMVector3Transform(m_Up, _Rotation);
	m_Right = XMVector3Transform(m_Right, _Rotation);


	m_ViewMatrix = XMMatrixLookAtLH(m_Target - m_Direction * m_Distance, m_Target, m_Up);
}

void Camera::RotateY(float p_AngleInDegree, bool p_InWorldSpace)
{
	XMMATRIX _Rotation;

	if (p_InWorldSpace)
		_Rotation = XMMatrixRotationY(XMConvertToRadians(p_AngleInDegree));
	else
		_Rotation = XMMatrixRotationAxis(m_Up, XMConvertToRadians(p_AngleInDegree));


	m_Direction = XMVector3Transform(m_Direction, _Rotation);
	m_Up = XMVector3Transform(m_Up, _Rotation);
	m_Right = XMVector3Transform(m_Right, _Rotation);


	m_ViewMatrix = XMMatrixLookAtLH(m_Target - m_Direction * m_Distance, m_Target, m_Up);
}

void Camera::RotateZ(float p_AngleInDegree, bool p_InWorldSpace)
{
	XMMATRIX _Rotation;

	if (p_InWorldSpace)
		_Rotation = XMMatrixRotationZ(XMConvertToRadians(p_AngleInDegree));
	else
		_Rotation = XMMatrixRotationAxis(m_Direction, XMConvertToRadians(p_AngleInDegree));


	m_Direction = XMVector3Transform(m_Direction, _Rotation);
	m_Up = XMVector3Transform(m_Up, _Rotation);
	m_Right = XMVector3Transform(m_Right, _Rotation);


	m_ViewMatrix = XMMatrixLookAtLH(m_Target - m_Direction * m_Distance, m_Target, m_Up);
}


void Camera::Move(XMFLOAT3 p_Movement, bool p_InWorldSpace)
{
	if (p_InWorldSpace)
	{
		m_Target += XMLoadFloat3(&p_Movement);

		m_ViewMatrix = XMMatrixLookAtLH(m_Target - m_Direction * m_Distance, m_Target, m_Up);
	}
	else
	{
		m_Target += m_Right * p_Movement.x;
		m_Target += m_Up * p_Movement.y;
		m_Target += m_Direction * p_Movement.z;

		m_ViewMatrix = XMMatrixLookAtLH(m_Target - m_Direction * m_Distance, m_Target, m_Up);
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


void Camera::Update(float p_DeltaTime)
{
	if (Input::GetKey(KeyCode::D))
		Move(XMFLOAT3(p_DeltaTime * 1, 0, 0), false);
	if (Input::GetKey(KeyCode::A))
		Move(XMFLOAT3(p_DeltaTime * -1, 0, 0), false);


	if (Input::GetKey(KeyCode::Space))
		Move(XMFLOAT3(0, p_DeltaTime * 1, 0), false);
	if (Input::GetKey(KeyCode::LeftShift))
		Move(XMFLOAT3(0, p_DeltaTime * -1, 0), false);

	if (Input::GetKey(KeyCode::W))
		Move(XMFLOAT3(0, 0, p_DeltaTime * 1), false);
	if (Input::GetKey(KeyCode::S))
		Move(XMFLOAT3(0, 0, p_DeltaTime * -1), false);

	if (Input::GetKey(KeyCode::RightMouse) && !Input::GetKeyLastFrame(KeyCode::RightMouse))
	{
		m_SavedMousePosition = Input::GetMousePosition();
		Input::SetMousePosition(XMFLOAT2(500, 400));
		ShowCursor(false);
	}

	if (Input::GetKey(KeyCode::RightMouse))
	{

		XMFLOAT2 _Pos = Input::GetMousePosition();
		_Pos.x -= 500;
		_Pos.y -= 400;

		RotateY(_Pos.x, true);
		RotateX(_Pos.y, false);

		Input::SetMousePosition(XMFLOAT2(500, 400));
	}

	if (!Input::GetKey(KeyCode::RightMouse) && Input::GetKeyLastFrame(KeyCode::RightMouse))
	{
		Input::SetMousePosition(m_SavedMousePosition);
		ShowCursor(true);
	}
}


void Camera::operator delete(void* p_Ptr)
{
	unsigned char _Dif = *((char*)p_Ptr - 1);
	p_Ptr = (char*)p_Ptr - _Dif;
	free(p_Ptr);
}


void* Camera::operator new(size_t Size)
{
	void* _Ptr = malloc(Size + 16);
	void* _PtrNew = (void*)(((int)_Ptr & 0xFFFFFFF0) + 0x00000010);

	unsigned char _Dif = (int)_PtrNew - (int)_Ptr;
	*((char*)_PtrNew - 1) = _Dif;

	return _PtrNew;

}