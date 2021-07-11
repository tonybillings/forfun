#pragma once

#include "ControllerComponent.h"
#include "TransformComponent.h"

#define BTN_SPACE	2
#define BTN_SHIFT	3
#define BTN_CONTROL	4
#define BTN_MENU	5
#define BTN_CAPITAL	6
#define BTN_TAB		7
#define BTN_1		8
#define BTN_2		9
#define BTN_3		10
#define BTN_4		11
#define BTN_5		12
#define BTN_6		13
#define BTN_7		14
#define BTN_8		15
#define BTN_9		16
#define BTN_0		17
#define BTN_Q		18
#define BTN_E		19
#define BTN_R		20
#define BTN_F		21
#define BTN_G		22
#define BTN_Z		23
#define BTN_X		24
#define BTN_C		25
#define BTN_V		26

class FirstPersonController : public ControllerComponent
{
protected:
	TransformComponent*		_transform;
	DirectX::XMVECTOR		_forward;
	DirectX::XMVECTOR		_right;
	DirectX::XMVECTOR		_up;
	float					_moveSpeed;
	float					_fastMoveSpeed;
	bool					_isMovingFast;
	float					_lookSpeed;
	float					_rollSpeed;
	int						_lastMouseX;
	int						_lastMouseY;
	float					_zRotation;

public:
	FirstPersonController(IComponentContainer* owner, const String& name = "");

public:
	inline float GetMoveSpeed() const noexcept
	{
		return _moveSpeed;
	}

	inline void SetMoveSpeed(float value) noexcept
	{
		_moveSpeed = value;
	}

	inline float GetFastMoveSpeed() const noexcept
	{
		return _fastMoveSpeed;
	}

	inline void SetFastMoveSpeed(float value) noexcept
	{
		_fastMoveSpeed = value;
	}

	inline float GetLookSpeed() const noexcept
	{
		return _lookSpeed;
	}

	inline void SetLookSpeed(float value) noexcept
	{
		_lookSpeed = value;
	}

	inline float GetRollSpeed() const noexcept
	{
		return _rollSpeed;
	}

	inline void SetRollSpeed(float value) noexcept
	{
		_rollSpeed = value;
	}

public:
	void UpdateKeyboardMouse(const AppState& appState);
	void UpdateGamePad(const AppState& appState);
	void UpdateJoystick(const AppState& appState);
	void UpdateController(const AppState& appState);

public:
	virtual void Init(const AppState& appState) override;
	virtual void Tick(const AppState& appState) override;

public:
	static inline SharedPtr<FirstPersonController> New(IComponentContainer* owner, const String& name = "") noexcept
	{
		return std::make_shared<FirstPersonController>(owner, name);
	}
};