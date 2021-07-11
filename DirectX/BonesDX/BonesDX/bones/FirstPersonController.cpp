#include "FirstPersonController.h"

FirstPersonController::FirstPersonController(IComponentContainer* owner, const String& name)
	: ControllerComponent(owner, name), 
	_transform(nullptr), 
	_forward(VECTOR_FORWARD),
	_right(VECTOR_RIGHT),
	_up(VECTOR_UP),
	_moveSpeed(1.f), 
	_fastMoveSpeed(2.f),
	_isMovingFast(false),
	_lookSpeed(.3f), 
	_rollSpeed(1.f),
	_lastMouseX(0), 
	_lastMouseY(0),
	_zRotation(0)
{
}

void FirstPersonController::UpdateKeyboardMouse(const AppState& appState)
{
	using namespace DirectX;

	auto kb = appState.Keyboard;
	auto mouse = appState.Mouse;

	auto W = kb->KeyIsPressed(VK_W);
	auto S = kb->KeyIsPressed(VK_S);
	auto A = kb->KeyIsPressed(VK_A);
	auto D = kb->KeyIsPressed(VK_D);

	if (W && !S)
		_state.Analog2D_0.Y = 1.f;
	else if (!W && S)
		_state.Analog2D_0.Y = -1.f;
	else
		_state.Analog2D_0.Y = 0;

	if (A && !D)
		_state.Analog2D_0.X = -1.f;
	else if (!A && D)
		_state.Analog2D_0.X = 1.f;
	else
		_state.Analog2D_0.X = 0;

	if (mouse->IsRawEnabled())
	{
		float x = 0;
		float y = 0;

		while (const auto mDelta = mouse->ReadRawDelta())
		{
			x += mDelta->x;
			y += mDelta->y;
		}

		_state.Analog2D_1.X = x;
		_state.Analog2D_1.Y = y;
	}
	else
	{
		_state.Analog2D_1.X = (float)((mouse->PosX() - _lastMouseX));
		_state.Analog2D_1.Y = (float)((mouse->PosY() - _lastMouseY));
	}

	_state.Buttons[0] = mouse->LeftIsPressed();
	_state.Buttons[1] = mouse->RightIsPressed();

	_state.Buttons[BTN_SPACE] = kb->KeyIsPressed(VK_SPACE);
	_state.Buttons[BTN_SHIFT] = kb->KeyIsPressed(VK_SHIFT);
	_state.Buttons[BTN_CONTROL] = kb->KeyIsPressed(VK_CONTROL);
	_state.Buttons[BTN_MENU] = kb->KeyIsPressed(VK_MENU);
	_state.Buttons[BTN_CAPITAL] = kb->KeyIsPressed(VK_CAPITAL);
	_state.Buttons[BTN_TAB] = kb->KeyIsPressed(VK_TAB);

	_state.Buttons[BTN_1] = kb->KeyIsPressed(VK_1);
	_state.Buttons[BTN_2] = kb->KeyIsPressed(VK_2);
	_state.Buttons[BTN_3] = kb->KeyIsPressed(VK_3);
	_state.Buttons[BTN_4] = kb->KeyIsPressed(VK_4);
	_state.Buttons[BTN_5] = kb->KeyIsPressed(VK_5);
	_state.Buttons[BTN_6] = kb->KeyIsPressed(VK_6);
	_state.Buttons[BTN_7] = kb->KeyIsPressed(VK_7);
	_state.Buttons[BTN_8] = kb->KeyIsPressed(VK_8);
	_state.Buttons[BTN_9] = kb->KeyIsPressed(VK_9);
	_state.Buttons[BTN_0] = kb->KeyIsPressed(VK_0);

	_state.Buttons[BTN_Q] = kb->KeyIsPressed(VK_Q);
	_state.Buttons[BTN_E] = kb->KeyIsPressed(VK_E);
	_state.Buttons[BTN_R] = kb->KeyIsPressed(VK_R);
	_state.Buttons[BTN_F] = kb->KeyIsPressed(VK_F);
	_state.Buttons[BTN_G] = kb->KeyIsPressed(VK_G);
	_state.Buttons[BTN_Z] = kb->KeyIsPressed(VK_Z);
	_state.Buttons[BTN_X] = kb->KeyIsPressed(VK_X);
	_state.Buttons[BTN_C] = kb->KeyIsPressed(VK_C);
	_state.Buttons[BTN_V] = kb->KeyIsPressed(VK_V);

	_lastMouseX = mouse->PosX();
	_lastMouseY = mouse->PosY();

	_isMovingFast = _state.Buttons[BTN_SHIFT].IsPressed;
}

void FirstPersonController::UpdateGamePad(const AppState& appState)
{
	//TODO: implement
}

void FirstPersonController::UpdateJoystick(const AppState& appState)
{
	//TODO: implement
}

void FirstPersonController::UpdateController(const AppState& appState)
{
	float yRotation = _state.Analog2D_1.X * _lookSpeed * appState.DeltaTime;
	float xRotation = _state.Analog2D_1.Y * _lookSpeed * appState.DeltaTime;
	float zRotation = 0;
	
	if (_state.Buttons[BTN_Q].IsPressed && !_state.Buttons[BTN_E].IsPressed)
		zRotation = 1.f * _rollSpeed * appState.DeltaTime;
	else if (!_state.Buttons[BTN_Q].IsPressed && _state.Buttons[BTN_E].IsPressed)
		zRotation = -1.f * _rollSpeed * appState.DeltaTime;

	auto&& yawRotation = QRotNorm(_up, yRotation);
	_right = VRot(_right, yawRotation);
	_forward = VRot(_forward, yawRotation);

	auto&& rightNorm = VNorm(_right);
	auto&& pitchRotation = QRotNorm(rightNorm, xRotation);
	_up = VRot(_up, pitchRotation);
	_forward = VRot(_forward, pitchRotation);

	auto&& forwardNorm = VNorm(_forward);
	auto&& rollRotation = QRotNorm(forwardNorm, zRotation);
	_up = VRot(_up, rollRotation);
	_right = VRot(_right, rollRotation);

	float moveSpeed = ((_isMovingFast ? _fastMoveSpeed : _moveSpeed)) * appState.DeltaTime;

	auto&& newRotation = QMul(_transform->GetRotation(), QMul(QMul(yawRotation, pitchRotation), rollRotation));
	_transform->SetRotation(newRotation);
	
	auto&& newPosition = VAdd(VAdd(_transform->GetPosition(), VScale(VScale(forwardNorm, _state.Analog2D_0.Y), moveSpeed)), VScale(VScale(rightNorm, _state.Analog2D_0.X), moveSpeed));

	auto&& upNorm = VNorm(_up);

	if (_state.Buttons[BTN_SPACE].IsPressed && !_state.Buttons[BTN_CONTROL].IsPressed)
		newPosition = VAdd(newPosition, VScale(upNorm, moveSpeed));
	else if (!_state.Buttons[BTN_SPACE].IsPressed && _state.Buttons[BTN_CONTROL].IsPressed)
		newPosition = VSub(newPosition, VScale(upNorm, moveSpeed));

	_transform->SetPosition(newPosition);
}

void FirstPersonController::Init(const AppState& appState)
{
    _transform = &_owner->GetComponent<TransformComponent>();

	_lastMouseX = appState.WindowWidth / 2;
	_lastMouseY = appState.WindowHeight / 2;
}

void FirstPersonController::Tick(const AppState& appState)
{
	switch (_type)
	{
	case EControllerType::KeyboardMouse:
		UpdateKeyboardMouse(appState);
		break;
	case EControllerType::GamePad:
		UpdateGamePad(appState);
		break;
	case EControllerType::Joystick:
		UpdateJoystick(appState);
		break;
	}

	UpdateController(appState);
}