#include "FPSDisplay.h"


FPSDisplay::FPSDisplay()
{
	TimeSinceLastSecond = 0;
	FramesSinceLastSecond = 0;
	FPS = 0;
}

void FPSDisplay::Update(float deltaTime)
{
	TimeSinceLastSecond += deltaTime;
	FramesSinceLastSecond += 1;

	if (TimeSinceLastSecond > 1)
	{
		FPS = FramesSinceLastSecond / TimeSinceLastSecond;

		TimeSinceLastSecond = 0;
		FramesSinceLastSecond = 0;
	}

}


void FPSDisplay::Render(SpriteBatch* p_pSpriteBatch, SpriteFont* p_pFont, float p_X, float p_Y)
{
	char _CharArray[128];

	sprintf_s(_CharArray, "FPS: %d", FPS);

	p_pSpriteBatch->DrawString(p_pFont, _CharArray, p_X, p_Y, 32, XMFLOAT4(1, 1, 1, 1));
}