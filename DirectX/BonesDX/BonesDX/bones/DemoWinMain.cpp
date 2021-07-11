#pragma once

#include "DemoApp.h"

/*******************************************************************************
This is an example of an entry point for a Win32 application using the BonesDX
framework.  To use, uncomment the code below and ensure it is the only instance
where you are defining the WinMain() function.
*******************************************************************************/

/*
int CALLBACK WinMain(
	HINSTANCE	hInstance,
	HINSTANCE	hPrevInstance,
	LPSTR		lpCmdLine,
	int			nCmdShow)
{
	try
	{
		return DemoApp().Run();
	}
	catch (const HrException & ex)
	{
		MessageBox(nullptr, ex.what(), ex.GetType(), MB_OK | MB_ICONEXCLAMATION);
	}
	catch (const Exception & ex)
	{
		MessageBox(nullptr, ex.what(), ex.GetType(), MB_OK | MB_ICONEXCLAMATION);
	}
	catch (const std::exception & ex)
	{
		MessageBox(nullptr, ex.what(), "Standard Exception", MB_OK | MB_ICONEXCLAMATION);
	}
	catch (...)
	{
		MessageBox(nullptr, "No details available", "Unknown Exception", MB_OK | MB_ICONEXCLAMATION);
	}

	return -1;
}
*/