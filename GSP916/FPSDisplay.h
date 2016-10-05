#pragma once
#include "SpriteBatch.h"

class FPSDisplay
{
public:
	FPSDisplay();

	void Update(float deltaTime);
	void Render(SpriteBatch* p_pSpriteBatch, SpriteFont* p_pFont, float p_X, float p_Y);

private:
	int FPS;
	int FramesSinceLastSecond;
	float TimeSinceLastSecond;


};