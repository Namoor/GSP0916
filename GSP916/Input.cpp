#include "Input.h"


bool Input::s_KeyIsPressed[256];

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

void Input::Update()
{
	for (int x = 0; x < 256; x++)
	{
		s_KeyIsPressed[x] = GetAsyncKeyState(x);
	}
}