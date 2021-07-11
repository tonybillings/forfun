#pragma once

#include "Keyboard.h"
#include "Mouse.h"
#include "Graphics.h"

struct AppState
{
	double		TotalTime;
	float		DeltaTime;
	Keyboard*	Keyboard;
	Mouse*		Mouse;
	int			WindowWidth;
	int			WindowHeight;
	Graphics*	Graphics;
};