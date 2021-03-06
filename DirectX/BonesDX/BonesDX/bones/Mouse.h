#pragma once

#include "Win32.h"
#include <queue>
#include <optional>

class Mouse
{
	friend class Window;

public:
	struct RawDelta
	{
		int x, y;
	};

	class Event
	{
	public:
		enum class Type
		{
			LPress,
			LRelease,
			RPress,
			RRelease,
			WheelUp,
			WheelDown,
			Move,
			Enter,
			Leave
		};

	private:
		Type type;
		bool leftIsPressed;
		bool rightIsPressed;
		int x;
		int y;

	public:
		Event(Type type, const Mouse& parent) noexcept
			:
			type(type),
			leftIsPressed(parent.leftIsPressed),
			rightIsPressed(parent.rightIsPressed),
			x(parent.x),
			y(parent.y)
		{}

		Type GetType() const noexcept
		{
			return type;
		}

		std::pair<int, int> Pos() const noexcept
		{
			return{ x,y };
		}

		int PosX() const noexcept
		{
			return x;
		}

		int PosY() const noexcept
		{
			return y;
		}

		bool LeftIsPressed() const noexcept
		{
			return leftIsPressed;
		}

		bool RightIsPressed() const noexcept
		{
			return rightIsPressed;
		}
	};

public:
	Mouse() = default;
	Mouse(const Mouse&) = delete;
	Mouse& operator=(const Mouse&) = delete;
	std::pair<int, int> Pos() const noexcept;
	std::optional<RawDelta> ReadRawDelta() noexcept;
	int PosX() const noexcept;
	int PosY() const noexcept;
	bool IsInWindow() const noexcept;
	bool LeftIsPressed() const noexcept;
	bool RightIsPressed() const noexcept;
	std::optional<Mouse::Event> Read() noexcept;
	bool IsEmpty() const noexcept
	{
		return buffer.empty();
	}
	void Flush() noexcept;
	void EnableRaw();
	void DisableRaw();
	bool IsRawEnabled() const noexcept;

private:
	void OnMouseMove(int x, int y) noexcept;
	void OnMouseLeave() noexcept;
	void OnMouseEnter() noexcept;
	void OnRawDelta(int dx, int dy) noexcept;
	void OnLeftPressed(int x, int y) noexcept;
	void OnLeftReleased(int x, int y) noexcept;
	void OnRightPressed(int x, int y) noexcept;
	void OnRightReleased(int x, int y) noexcept;
	void OnWheelUp(int x, int y) noexcept;
	void OnWheelDown(int x, int y) noexcept;
	void TrimBuffer() noexcept;
	void TrimRawInputBuffer() noexcept;
	void OnWheelDelta(int x, int y, int delta) noexcept;

private:
	static constexpr unsigned int bufferSize = 16u;
	int x = 0;
	int y = 0;
	bool leftIsPressed = false;
	bool rightIsPressed = false;
	bool isInWindow = false;
	int wheelDeltaCarry = 0;
	bool rawEnabled = false;
	std::queue<Event> buffer;
	std::queue<RawDelta> rawDeltaBuffer;
};