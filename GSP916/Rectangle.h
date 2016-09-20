#pragma once


struct Rect
{
public:
	Rect(int p_X, int p_Y, int p_Width, int p_Height);
	Rect();


	int x;
	int y;
	int width;
	int height;
};