#include "Window.h"

Window::WindowClass Window::WindowClass::_class;

Window::WindowClass::WindowClass() noexcept
	: _hInstance(GetModuleHandle(nullptr))
{
	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(wc);
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = HandleMsgSetup;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = GetInstance();
	wc.hIcon = static_cast<HICON>(LoadImage(_hInstance, MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 16, 16, 0));
	wc.hCursor = nullptr;
	wc.hbrBackground = nullptr;
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = _className;
	wc.hIconSm = static_cast<HICON>(LoadImage(_hInstance, MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 16, 16, 0));
	RegisterClassEx(&wc);
}

Window::WindowClass::~WindowClass()
{
	UnregisterClass(_className, GetInstance());
}

const char* Window::WindowClass::Name() noexcept
{
	return _className;
}

HINSTANCE Window::WindowClass::GetInstance() noexcept
{
	return _class._hInstance;
}

Window::Window(int width, int height, const String& name)
	: _width(width), _height(height)
{
	RECT wr;
	wr.left = 100;
	wr.right = width + wr.left;
	wr.top = 100;
	wr.bottom = height + wr.top;

	if (AdjustWindowRect(&wr, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE) == 0)
		throw LAST_HR_EX();

	_hWnd = CreateWindow(
		WindowClass::Name(), name.c_str(),
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
		CW_USEDEFAULT, CW_USEDEFAULT, wr.right - wr.left, wr.bottom - wr.top,
		nullptr, nullptr, WindowClass::GetInstance(), this
	);

	if (_hWnd == nullptr)
		throw LAST_HR_EX();

	Graphics.Init(_hWnd, width, height);

	ShowWindow(_hWnd, SW_SHOWDEFAULT);

	CenterCursor();

	Keyboard.AllowAccessibilityShortcutKeys(false);
}

Window::~Window()
{
	Keyboard.AllowAccessibilityShortcutKeys(true);
	DestroyWindow(_hWnd);
}

void Window::SetTitle(const String title)
{
	if (SetWindowText(_hWnd, title.c_str()) == 0)
		throw LAST_HR_EX();
}

void Window::CenterCursor()
{
	POINT pt;
	pt.x = _width / 2;
	pt.y = _height / 2;

	ClientToScreen(_hWnd, &pt);
	SetCursorPos(pt.x, pt.y);
}

std::optional<int> Window::ProcessMessages()
{
	MSG msg;
	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		if (msg.message == WM_QUIT)
			return msg.wParam;

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return {};
}

LRESULT WINAPI Window::HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	if (msg == WM_NCCREATE)
	{
		const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
		Window* const pWnd = static_cast<Window*>(pCreate->lpCreateParams);
		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
		SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::HandleMsgThunk));
		return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT WINAPI Window::HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	Window* const pWnd = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
}

LRESULT Window::HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	if (msg == WM_CLOSE)
	{
		PostQuitMessage(0);
		return 0;
	}

	if (Mouse.IsRawEnabled())
	{
		if (msg == WM_INPUT)
		{
			UINT size = 0;

			if (GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, nullptr, &size, sizeof(RAWINPUTHEADER)) != -1)
			{
				_mouseBuffer.resize(size);

				if (GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, _mouseBuffer.data(), &size, sizeof(RAWINPUTHEADER)) == size)
				{
					auto& ri = reinterpret_cast<const RAWINPUT&>(*_mouseBuffer.data());

					if (ri.header.dwType == RIM_TYPEMOUSE && (ri.data.mouse.lLastX != 0 || ri.data.mouse.lLastY != 0))
						Mouse.OnRawDelta(ri.data.mouse.lLastX, ri.data.mouse.lLastY);
				}
			}
		}
	}
	else
	{
		if (msg == WM_MOUSEMOVE)
		{
			const POINTS pt = MAKEPOINTS(lParam);

			if (pt.x >= 0 && pt.x < _width && pt.y >= 0 && pt.y < _height)
			{
				Mouse.OnMouseMove(pt.x, pt.y);

				if (!Mouse.IsInWindow())
				{
					SetCapture(hWnd);
					Mouse.OnMouseEnter();
				}
			}
			else
			{
				if (Mouse.LeftIsPressed() || Mouse.RightIsPressed())
				{
					Mouse.OnMouseMove(pt.x, pt.y);
				}
				else
				{
					ReleaseCapture();
					Mouse.OnMouseLeave();
				}
			}
		}
	}

	switch (msg)
	{
		case WM_LBUTTONDOWN:
		{
			const POINTS pt = MAKEPOINTS(lParam);
			Mouse.OnLeftPressed(pt.x, pt.y);
			break;
		}
		case WM_RBUTTONDOWN:
		{
			const POINTS pt = MAKEPOINTS(lParam);
			Mouse.OnRightPressed(pt.x, pt.y);
			break;
		}
		case WM_LBUTTONUP:
		{
			const POINTS pt = MAKEPOINTS(lParam);
			Mouse.OnLeftReleased(pt.x, pt.y);
			break;
		}
		case WM_RBUTTONUP:
		{
			const POINTS pt = MAKEPOINTS(lParam);
			Mouse.OnRightReleased(pt.x, pt.y);
			break;
		}
		case WM_MOUSEWHEEL:
		{
			const POINTS pt = MAKEPOINTS(lParam);
			const int delta = GET_WHEEL_DELTA_WPARAM(wParam);
			Mouse.OnWheelDelta(pt.x, pt.y, delta);
			break;
		}
		case WM_KILLFOCUS:
		{
			Keyboard.ClearState();
			break;
		}
		case WM_SYSKEYDOWN:
		case WM_KEYDOWN:
		{
			if (!(lParam & 0x40000000) || Keyboard.AutorepeatIsEnabled())
				Keyboard.OnKeyPressed(static_cast<unsigned char>(wParam));
			break;
		}
		case WM_KEYUP:
		{
			Keyboard.OnKeyReleased(static_cast<unsigned char>(wParam));
			break;
		}
		case WM_CHAR:
		{
			Keyboard.OnChar(static_cast<unsigned char>(wParam));
			break;
		}
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}