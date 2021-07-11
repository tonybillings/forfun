#include "CameraComponent.h"

CameraComponent::CameraComponent(IComponentContainer* owner, const String& name)
	: Component(owner, name),
	_ownerTransform(&owner->GetComponent<TransformComponent>()),
	_fovRadians(DEGREES_45),
	_nearPlane(.1f),
	_farPlane(10000.f),
	_viewMatrix(MATRIX_IDENTITY),
	_projMatrix(MATRIX_IDENTITY),
	_viewProjMatrix(MATRIX_IDENTITY)
{
}

void CameraComponent::Init(const AppState& state)
{
	auto transform = _owner->GetComponent(TransformComponent::ClassID());

	_ownerTransform = dynamic_cast<TransformComponent*>(transform);

	_viewMatrix = MLookAt(VSet(0, 0, -10.f, 1.f), VECTOR_FORWARD, VECTOR_UP);
	_projMatrix = MProject(_fovRadians, (float)state.WindowWidth / (float)state.WindowHeight, _nearPlane, _farPlane);
	_viewProjMatrix = MMul(_viewMatrix, _projMatrix);
}

void CameraComponent::Tick(const AppState& state)
{
	auto&& position = _ownerTransform->GetWorldPosition();
	auto&& forward = _ownerTransform->GetForwardVector();
	auto&& up = _ownerTransform->GetUpVector();
	auto&& target = VAdd(position, forward);

	_viewMatrix = MLookAt(position, target, up);
	_viewProjMatrix = MMul(_viewMatrix, _projMatrix);
}