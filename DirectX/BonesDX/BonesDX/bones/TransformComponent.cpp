#include "TransformComponent.h"

TransformComponent::TransformComponent(IComponentContainer* owner, const String& name)
	: Component(owner, name),
	_position(VECTOR_ORIGIN),
	_rotation(QUAT_IDENTITY),
	_scale(VECTOR_IDENTITY),
	_orbitPosition(VECTOR_ORIGIN),
	_orbitRotation(QUAT_IDENTITY),
	_worldPosition(VECTOR_ORIGIN),
	_forward(VECTOR_FORWARD),
	_up(VECTOR_UP),
	_worldMatrix(MATRIX_IDENTITY),
	Position(FLOAT4_ORIGIN),
	Rotation(FLOAT4_ORIGIN),
	Scale(FLOAT4_IDENTITY),
	OrbitPosition(FLOAT4_ORIGIN),
	OrbitRotation(FLOAT4_ORIGIN) 
{ }

TransformComponent& TransformComponent::RotateTo(FVector target) noexcept
{
	if (VEqual(_worldPosition, VECTOR_ZERO) && VEqual(target, VECTOR_ZERO))
	{
		VStore(&Rotation, QUAT_IDENTITY);
		return *this;
	}

	auto&& forward = VSub(target, _worldPosition);

	if (VEqual(forward, VECTOR_ZERO))
	{
		VStore(&Rotation, QUAT_IDENTITY);
		return *this;
	}

	auto&& nForward = VNorm(forward);

	auto&& right = VCross(nForward, VECTOR_UP);

	if (VEqual(right, VECTOR_ZERO))
		right = VECTOR_RIGHT;

	auto&& nRight = VNorm(right);
	auto&& up = VCross(nForward, nRight);

	if (VEqual(up, VECTOR_ZERO))
	{
		VStore(&Rotation, QUAT_IDENTITY);
		return *this;
	}

	auto&& nUp = VNorm(up);
	VStore(&Rotation, QRotMat(MInv(MLookAt(_worldPosition, target, nUp))));

	return *this;
}

void TransformComponent::Tick(const AppState& appState)
{
	_position = VLoad(&Position);
	_rotation = VLoad(&Rotation);
	_scale = VLoad(&Scale);
	_orbitPosition = VLoad(&OrbitPosition);
	_orbitRotation = VLoad(&OrbitRotation);

	auto&& scalingMatrix = MScale(_scale);
	auto&& rotationMatrix = MRot(_rotation);
	auto&& orbitMatrix = MMul(MTran(_orbitPosition), MRot(_orbitRotation));
	auto&& translationMatrix = MTran(_position);

	auto&& positionMatrix = MMul(orbitMatrix, translationMatrix);
	_worldPosition = VTran(VECTOR_ORIGIN, positionMatrix);
	_worldMatrix = MMul(scalingMatrix, MMul(rotationMatrix, MTran(_worldPosition)));

	_forward = VNorm(VRot(VECTOR_FORWARD, _rotation));
	_up = VNorm(VRot(VECTOR_UP, _rotation));
}