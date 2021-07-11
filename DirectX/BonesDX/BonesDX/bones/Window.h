#pragma once

#include "Win32.h"
#include <memory>
#include <sstream>
#include "resource.h"
#include "HrException.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "Graphics.h"
#include "TypeDefs.h"

class Window
{
private:
	class WindowClass
	{
	private:
		static constexpr const char* _className = "Win32 Window";
		static WindowClass _class;
		HINSTANCE _hInstance;

	private:
		WindowClass() noexcept;
		WindowClass(const WindowClass&) = delete;
		~WindowClass();
	
	public:
		static const char* Name() noexcept;
		static HINSTANCE GetInstance() noexcept;

	private:
		WindowClass& operator=(const WindowClass&) = delete;
	};

private:
	int				_width;
	int				_height;
	HWND			_hWnd;
	List<char>		_mouseBuffer;

public:
	Keyboard	Keyboard;
	Mouse		Mouse;
	Graphics	Graphics;

public:
	Window(int width, int height, const String& name);
	~Window();
	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;

public:
	inline int GetWidth() const noexcept
	{
		return _width;
	}

	inline int GetHeight() const noexcept
	{
		return _height;
	}

public:
	void SetTitle(const String title);
	void CenterCursor();
	static std::optional<int> ProcessMessages();

private:
	static LRESULT CALLBACK HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	static LRESULT CALLBACK HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
};