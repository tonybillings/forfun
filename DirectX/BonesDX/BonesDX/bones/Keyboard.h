#pragma once

#include <queue>
#include <bitset>
#include "Win32.h"

#define VK_Q 0x51
#define VK_W 0x57
#define VK_E 0x45
#define VK_R 0x52
#define VK_T 0x54

#define VK_A 0x41
#define VK_S 0x53
#define VK_D 0x44
#define VK_F 0x46
#define VK_G 0x47

#define VK_Z 0x5A
#define VK_X 0x58
#define VK_C 0x43
#define VK_V 0x56
#define VK_B 0x42

#define VK_0 0x30
#define VK_1 0x31
#define VK_2 0x32
#define VK_3 0x33
#define VK_4 0x34
#define VK_5 0x35
#define VK_6 0x36
#define VK_7 0x37
#define VK_8 0x38
#define VK_9 0x39

class Keyboard
{
	friend class Window;

public:
	class Event
	{
	public:
		enum class Type
		{
			Press,
			Release,
			Invalid
		};

	private:
		Type type;
		unsigned char code;

	public:
		Event()
			:
			type(Type::Invalid),
			code(0u)
		{}

		Event(Type type, unsigned char code)
			:
			type(type),
			code(code)
		{}

		bool IsPress() const
		{
			return type == Type::Press;
		}

		bool IsRelease() const
		{
			return type == Type::Release;
		}

		bool IsValid() const
		{
			return type != Type::Invalid;
		}

		unsigned char Code() const
		{
			return code;
		}
	};

public:
	Keyboard() = default;
	Keyboard(const Keyboard&) = delete;
	Keyboard& operator=(const Keyboard&) = delete;
	bool KeyIsPressed(unsigned char keycode) const;
	Event ReadKey();
	bool KeyIsEmpty() const;
	char ReadChar();
	bool CharIsEmpty() const;
	void FlushKey();
	void FlushChar();
	void Flush();
	void EnableAutorepeat();
	void DisableAutorepeat();
	bool AutorepeatIsEnabled() const;

private:
	void OnKeyPressed(unsigned char keycode);
	void OnKeyReleased(unsigned char keycode);
	void OnChar(char character);
	void ClearState();
	template<typename T>
	void TrimBuffer(std::queue<T>& buffer);

private:
	inline static STICKYKEYS startupStickyKeys = { sizeof(STICKYKEYS), 0 };
	inline static TOGGLEKEYS startupToggleKeys = { sizeof(TOGGLEKEYS), 0 };
	inline static FILTERKEYS startupFilterKeys = { sizeof(FILTERKEYS), 0 };
	inline static bool hasSavedAccessibilitySettings = false;

public:
	void AllowAccessibilityShortcutKeys(bool bAllowKeys)
	{
		if (!hasSavedAccessibilitySettings)
		{
			SystemParametersInfo(SPI_GETSTICKYKEYS, sizeof(STICKYKEYS), &startupStickyKeys, 0);
			SystemParametersInfo(SPI_GETTOGGLEKEYS, sizeof(TOGGLEKEYS), &startupToggleKeys, 0);
			SystemParametersInfo(SPI_GETFILTERKEYS, sizeof(FILTERKEYS), &startupFilterKeys, 0);
			hasSavedAccessibilitySettings = true;
		}

		if (bAllowKeys)
		{    
			STICKYKEYS sk = startupStickyKeys;
			TOGGLEKEYS tk = startupToggleKeys;
			FILTERKEYS fk = startupFilterKeys;

			SystemParametersInfo(SPI_SETSTICKYKEYS, sizeof(STICKYKEYS), &startupStickyKeys, 0);
			SystemParametersInfo(SPI_SETTOGGLEKEYS, sizeof(TOGGLEKEYS), &startupToggleKeys, 0);
			SystemParametersInfo(SPI_SETFILTERKEYS, sizeof(FILTERKEYS), &startupFilterKeys, 0);
		}
		else
		{
			STICKYKEYS skOff = startupStickyKeys;
			if ((skOff.dwFlags & SKF_STICKYKEYSON) == 1)
			{
				skOff.dwFlags &= ~SKF_HOTKEYACTIVE;
				skOff.dwFlags &= ~SKF_CONFIRMHOTKEY;

				SystemParametersInfo(SPI_SETSTICKYKEYS, sizeof(STICKYKEYS), &skOff, 0);
			}

			TOGGLEKEYS tkOff = startupToggleKeys;
			if ((tkOff.dwFlags & TKF_TOGGLEKEYSON) == 1)
			{
				tkOff.dwFlags &= ~TKF_HOTKEYACTIVE;
				tkOff.dwFlags &= ~TKF_CONFIRMHOTKEY;

				SystemParametersInfo(SPI_SETTOGGLEKEYS, sizeof(TOGGLEKEYS), &tkOff, 0);
			}

			FILTERKEYS fkOff = startupFilterKeys;
			if ((fkOff.dwFlags & FKF_FILTERKEYSON) == 1)
			{
				fkOff.dwFlags &= ~FKF_HOTKEYACTIVE;
				fkOff.dwFlags &= ~FKF_CONFIRMHOTKEY;

				SystemParametersInfo(SPI_SETFILTERKEYS, sizeof(FILTERKEYS), &fkOff, 0);
			}
		}
	}

private:
	static constexpr unsigned int nKeys = 256u;
	static constexpr unsigned int bufferSize = 4u;
	bool autorepeatEnabled = false;
	std::bitset<nKeys> keystates;
	std::queue<Event> keybuffer;
	std::queue<char> charbuffer;
};