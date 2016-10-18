#include "Input.h"


bool Input::s_KeyIsPressed[256];
bool Input::s_KeyWasPressed[256];

Input::Input()
{
	for (int x = 0; x < 256; x++)
	{
		s_KeyIsPressed[x] = 0;
	}
}

bool Input::GetKey(KeyCode::Key p_Key)
{
	return s_KeyIsPressed[p_Key];
}

bool Input::GetKeyLastFrame(KeyCode::Key p_Key)
{
	return s_KeyWasPressed[p_Key];
}

void Input::Update()
{
	for (int x = 0; x < 256; x++)
	{
		s_KeyWasPressed[x] = s_KeyIsPressed[x];
		s_KeyIsPressed[x] = GetAsyncKeyState(x);
	}
}


XMFLOAT2 Input::GetMousePosition()
{
	POINT _MousePos;
	GetCursorPos(&_MousePos);

	return XMFLOAT2(_MousePos.x, _MousePos.y);
}


void Input::SetMousePosition(XMFLOAT2 p_Position)
{
	SetCursorPos(p_Position.x, p_Position.y);
}