#include "Rectangle.h"



Rect::Rect(int p_X, int p_Y, int p_Width, int p_Height)
{
	x = p_X;
	y = p_Y;
	width = p_Width;
	height = p_Height;
}

Rect::Rect()
{
	x = 0;
	y = 0;
	width = 0;
	height = 0;
}