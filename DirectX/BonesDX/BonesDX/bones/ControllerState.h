#pragma once

typedef unsigned int VirtualKey;

struct Analog1D
{
	float Z;
	Analog1D(float z = 0) : Z(z) {}
};

struct Analog2D
{
	float X;
	float Y;
	Analog2D(float x = 0, float y = 0) : X(x), Y(y) {}
};

struct Button
{
	bool IsPressed;
	Button(bool isPressed = false) : IsPressed(isPressed) {}
};

struct ControllerState
{
	Analog2D	Analog2D_0;
	Analog2D	Analog2D_1;
	
	Analog1D	Analog1D_0;
	Analog1D	Analog1D_1;
	Analog1D	Analog1D_2;
	Analog1D	Analog1D_3;

	Button		Buttons[32];
};