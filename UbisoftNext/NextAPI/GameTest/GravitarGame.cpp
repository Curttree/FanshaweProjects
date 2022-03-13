//------------------------------------------------------------------------
// GameTest.cpp
//------------------------------------------------------------------------
#include "stdafx.h"
//------------------------------------------------------------------------
#include <windows.h> 
#include <math.h>  
//------------------------------------------------------------------------
#include "app\app.h"
//------------------------------------------------------------------------
#include "cWorldSpace.h"
//------------------------------------------------------------------------
// Eample data....
//------------------------------------------------------------------------
cWorldSpace* worldSpace = cWorldSpace::Instance();
//------------------------------------------------------------------------

//------------------------------------------------------------------------
// Called before first update. Do any initial setup here.
//------------------------------------------------------------------------
void Init()
{
	worldSpace->Init();
}

//------------------------------------------------------------------------
// Update your simulation here. deltaTime is the elapsed time since the last update in ms.
// This will be called at no greater frequency than the value of APP_MAX_FRAME_RATE
//------------------------------------------------------------------------
void Update(float deltaTime)
{
	worldSpace->Update(deltaTime);
	if (App::GetController().GetLeftThumbStickX() > 0.5f)
	{
		worldSpace->player->SetAngle(worldSpace->player->GetAngle() - 0.1f);
	}
	if (App::GetController().GetLeftThumbStickX() < -0.5f)
	{
		worldSpace->player->SetAngle(worldSpace->player->GetAngle() + 0.1f);
	}

	if (App::GetController().GetRightTrigger() > TRIGGER_THRESHOLD)
	{
		worldSpace->player->Thrust(App::GetController().GetRightTrigger());
		//TODO: Move sound responsibilities to appropriate classes.
		if (!App::IsSoundPlaying(ENGINE_SOUND)) {
			App::PlaySound(ENGINE_SOUND,true);
		}
	}
	else if (App::IsSoundPlaying(ENGINE_SOUND)) {
		App::StopSound(ENGINE_SOUND);
	}

	if (App::GetController().GetLeftTrigger() > TRIGGER_THRESHOLD)
	{
		//TODO: Move sound responsibilities to appropriate classes.
		if (!App::IsSoundPlaying(LASER_SOUND)) {
			App::PlaySound(LASER_SOUND, false);
		}
		worldSpace->player->Fire();
	}
}

//------------------------------------------------------------------------
// Add your display calls here (DrawLine,Print, DrawSprite.) 
// See App.h 
//------------------------------------------------------------------------
void Render()
{
	//------------------------------------------------------------------------
	// Draw the objects present in the game world.
	worldSpace->Draw();
	//------------------------------------------------------------------------

	//------------------------------------------------------------------------
	// HUD Text.
	//------------------------------------------------------------------------
	App::Print(400, 700, "SCORE");
	App::Print(400, 650, "FUEL");
	App::Print(550, 700, "0");
	App::Print(550, 650, "0");

	App::Print(100, 700, "LIVES");
	App::Print(250, 700, "3");

	//------------------------------------------------------------------------
	// Example Line Drawing.
	//------------------------------------------------------------------------
	//static float a = 0.0f;
	//float r = 1.0f;
	//float g = 1.0f;
	//float b = 1.0f;
	//a += 0.1f;
	//for (int i = 0; i < 20; i++)
	//{

	//	float sx = 200 + sinf(a + i * 0.1f) * 60.0f;
	//	float sy = 200 + cosf(a + i * 0.1f) * 60.0f;
	//	float ex = 700 - sinf(a + i * 0.1f) * 60.0f;
	//	float ey = 700 - cosf(a + i * 0.1f) * 60.0f;
	//	g = (float)i / 20.0f;
	//	b = (float)i / 20.0f;
	//	App::DrawLine(sx, sy, ex, ey, r, g, b);
	//}
}
//------------------------------------------------------------------------
// Add your shutdown code here. Called when the APP_QUIT_KEY is pressed.
// Just before the app exits.
//------------------------------------------------------------------------
void Shutdown()
{
	worldSpace->Cleanup();
}