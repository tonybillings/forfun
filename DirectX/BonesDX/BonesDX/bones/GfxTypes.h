#pragma once

#include "Win32.h"
#include <d3d11.h>
#include <DirectXMath.h>

typedef DirectX::XMFLOAT2A		Float2;
typedef DirectX::XMFLOAT3A		Float3;
typedef DirectX::XMFLOAT4A		Float4;
typedef DirectX::XMFLOAT4X4A	Float4x4;

typedef DirectX::XMVECTOR		Vector;
typedef DirectX::FXMVECTOR		FVector;
typedef DirectX::GXMVECTOR		GVector;
typedef DirectX::HXMVECTOR		HVector;
typedef DirectX::CXMVECTOR		CVector;

typedef DirectX::XMMATRIX		Matrix;
typedef DirectX::FXMMATRIX		FMatrix;
typedef DirectX::CXMMATRIX		CMatrix;

typedef unsigned char byte;

enum class EVertexType { VertexPositionColor, VertexPositionNormal, VertexPositionNormalTexture };
enum class EPrimitiveTopology { PointList = 1, LineList = 2, LineStrip = 3, TriangleList = 4, TriangleStrip = 5 };

#define VLoad(pSource) DirectX::XMLoadFloat4A(pSource)
#define VStore(pDestination, vector) DirectX::XMStoreFloat4A(pDestination, vector)
#define VSet(x, y, z, w) DirectX::XMVectorSet(x, y, z, w)
#define VRep(value) DirectX::XMVectorReplicate(value)
#define VAdd(v1, v2) DirectX::XMVectorAdd(v1, v2)
#define VSub(v1, v2) DirectX::XMVectorSubtract(v1, v2)
#define VMul(v1, v2) DirectX::XMVectorMultiply(v1, v2)
#define VDiv(v1, v2) DirectX::XMVectorDivide(v1, v2)
#define VDot(v1, v2) DirectX::XMVector3Dot(v1, v2)
#define VCross(v1, v2) DirectX::XMVector3Cross(v1, v2)
#define VScale(vector, float1) DirectX::XMVectorScale(vector, float1)
#define VNorm(vector) DirectX::XMVector3Normalize(vector)
#define VRot(vector, quat) DirectX::XMVector3Rotate(vector, quat)
#define VTran(vector, matrix) DirectX::XMVector3Transform(vector, matrix)
#define VGetX(vector) DirectX::XMVectorGetX(vector)
#define VGetY(vector) DirectX::XMVectorGetY(vector)
#define VGetZ(vector) DirectX::XMVectorGetZ(vector)
#define VGetW(vector) DirectX::XMVectorGetW(vector)
#define VSetX(vector, value) DirectX::XMVectorSetX(vector, value)
#define VSetY(vector, value) DirectX::XMVectorSetY(vector, value)
#define VSetZ(vector, value) DirectX::XMVectorSetZ(vector, value)
#define VSetW(vector, value) DirectX::XMVectorSetW(vector, value)
#define VEqual(v1, v2) DirectX::XMVector3Equal(v1, v2)

#define MLoad(float4) DirectX::XMLoadFloat4x4A(float4)
#define MStore(destination, float4) DirectX::XMStoreFloat4x4A(destination, float4)
#define MTranspose(matrix) DirectX::XMMatrixTranspose(matrix)
#define MScale(vector) DirectX::XMMatrixScalingFromVector(vector)
#define MRot(quat) DirectX::XMMatrixRotationQuaternion(quat)
#define MTran(vector) DirectX::XMMatrixTranslationFromVector(vector)
#define MInv(matrix) DirectX::XMMatrixInverse(nullptr, matrix)
#define MMul(m1, m2) DirectX::XMMatrixMultiply(m1, m2)
#define MLookAt(position, target, up) DirectX::XMMatrixLookAtLH(position, target, up)
#define MProject(fov, aspectRatio, nearPlane, farPlane) DirectX::XMMatrixPerspectiveFovLH(fov, aspectRatio, nearPlane, farPlane)

#define QMul(q1, q2) DirectX::XMQuaternionMultiply(q1, q2)
#define QRotAxis(vector, angle) DirectX::XMQuaternionRotationAxis(vector, angle)
#define QRotMat(matrix) DirectX::XMQuaternionRotationMatrix(matrix)
#define QRotNorm(vector, angle) DirectX::XMQuaternionRotationNormal(vector, angle)
#define QRotRPY(pitch, yaw, roll) DirectX::XMQuaternionRotationRollPitchYaw(pitch, yaw, roll)

#define FLOAT4_IDENTITY Float4(1.f, 1.f, 1.f, 1.f)
#define FLOAT4_ZERO Float4(0.f, 0.f, 0.f, 0)
#define FLOAT4_RIGHT Float4(1.f, 0.f, 0.f, 1.f)
#define FLOAT4_UP Float4(0.f, 1.f, 0.f, 1.f)
#define FLOAT4_FORWARD Float4(0.f, 0.f, 1.f, 1.f)
#define FLOAT4_ORIGIN Float4(0.f, 0.f, 0.f, 1.f)
#define FLOAT4_EPSILON Float4(0.000000001f, 0.000000001f, 0.000000001f, 0.000000001f)

#define VECTOR_IDENTITY DirectX::XMVectorSet(1.f, 1.f, 1.f, 1.f)
#define VECTOR_ZERO DirectX::XMVectorZero()
#define VECTOR_RIGHT DirectX::XMVectorSet(1.f, 0.f, 0.f, 1.f)
#define VECTOR_UP DirectX::XMVectorSet(0.f, 1.f, 0.f, 1.f)
#define VECTOR_FORWARD DirectX::XMVectorSet(0.f, 0.f, 1.f, 1.f)
#define VECTOR_ORIGIN DirectX::XMVectorSet(0.f, 0.f, 0.f, 1.f)
#define VECTOR_EPSILON DirectX::XMVectorSet(0.000000001f, 0.000000001f, 0.000000001f, 0.000000001f)

#define MATRIX_IDENTITY DirectX::XMMatrixIdentity()
#define QUAT_IDENTITY DirectX::XMQuaternionIdentity()

#define VERTEX_POSITION_COLOR			EVertexType::VertexPositionColor
#define VERTEX_POSITION_NORMAL			EVertexType::VertexPositionNormal
#define VERTEX_POSITION_NORMAL_TEXTURE	EVertexType::VertexPositionNormalTexture

#define POINT_LIST		EPrimitiveTopology::PointList
#define LINE_LIST		EPrimitiveTopology::LineList
#define LINE_STRIP		EPrimitiveTopology::LineStrip
#define TRIANGLE_LIST	EPrimitiveTopology::TriangleList
#define TRIANGLE_STRIP	EPrimitiveTopology::TriangleStrip

#define DIRECTIONAL_LIGHT	0
#define POINT_LIGHT			1
#define SPOT_LIGHT			2

#define DEGREES_45	0.78539f
#define DEGREES_90	1.57079f
#define DEGREES_180 3.14159f
#define DEGREES_270 4.71239f
#define DEGREES_360 6.28319f

struct TxtCoord2D
{
	float U; float V;
	TxtCoord2D() : U(0), V(0) {}
	TxtCoord2D(float u, float v) : U(u), V(v) {}
};

struct TxtCoord3D
{
	float U; float V; float W;
	TxtCoord3D() : U(0), V(0), W(0) {}
	TxtCoord3D(float u, float v, float w) : U(u), V(v), W(w) {}
};

struct Vector2D
{
	float X; float Y;
	Vector2D() : X(0), Y(0) {}
	Vector2D(float x, float y) : X(x), Y(y) {}
};

struct Vector3D
{
	float X; float Y; float Z;
	Vector3D() : X(0), Y(0), Z(0) {}
	Vector3D(float x, float y, float z) : X(x), Y(y), Z(z) {}
};

struct Vector4D
{
	float X; float Y; float Z; float W;
	Vector4D() : X(0), Y(0), Z(0), W(0) {}
	Vector4D(float x, float y, float z, float w) : X(x), Y(y), Z(z), W(w) {}
};

struct Color
{
	byte R; byte G; byte B; byte A;
	Color() : R(0), G(0), B(0), A(255) {}
	Color(byte r, byte g, byte b, byte a = 255) : R(r), G(g), B(b), A(a) {}
};

struct VertexPositionColor
{
	Vector3D	Position;
	Color		Color;
};

struct VertexPositionNormal
{
	Vector3D	Position;
	Vector3D	Normal;
};

struct VertexPositionNormalTexture
{
	Vector3D	Position;
	Vector3D	Normal;
	TxtCoord2D	Texture;
};

struct CBTransform
{
	Float4x4 WVP;
	Float4x4 World;
	Float4x4 WorldInverse;

	CBTransform()
	{
		MStore(&WVP, MTranspose(MATRIX_IDENTITY));
		MStore(&World, MTranspose(MATRIX_IDENTITY));
		MStore(&WorldInverse, MTranspose(MATRIX_IDENTITY));
	}

	CBTransform(CMatrix worldViewProjection, CMatrix world, CMatrix worldInverse)
	{
		MStore(&WVP, worldViewProjection);
		MStore(&World, world);
		MStore(&WorldInverse, worldInverse);
	}
};

struct CBBasicMaterial
{
	Float4	Emissive;
	Float4	Ambient;
	Float4	Diffuse;
	Float4	Specular;
	float	SpecularPower;
	int		UseTexture;
	float	Padding[2];

	CBBasicMaterial()
		: Padding{ 0.f }
	{
		Emissive = FLOAT4_ZERO;
		Ambient = FLOAT4_ZERO;
		Diffuse = FLOAT4_ZERO;
		Specular = FLOAT4_ZERO;
		SpecularPower = 128.f;
		UseTexture = false;
	}

	CBBasicMaterial(FVector emissive,
		FVector ambient,
		FVector diffuse,
		CVector specular,
		float specularPower = 128.f,
		bool useTexture = false)
		: Padding{ 0.f }
	{
		VStore(&Emissive, emissive);
		VStore(&Ambient, ambient);
		VStore(&Diffuse, diffuse);
		VStore(&Specular, specular);
		SpecularPower = specularPower;
		UseTexture = useTexture;
	}
};

struct CBLightProperties
{
	Float4	Position;
	Float4	Direction;
	Float4	Color;
	float	SpotAngle;
	float	ConstantAttenuation;
	float	LinearAttenuation;
	float	QuadraticAttenuation;
	int		LightType;
	int		IsEnabled;
	int		Padding[2];

	CBLightProperties()
		: Padding{ 0 }
	{
		Position = FLOAT4_ORIGIN;
		Direction = FLOAT4_FORWARD;
		Color = FLOAT4_ORIGIN;
		SpotAngle = DirectX::XM_PIDIV2;
		ConstantAttenuation = 1.f;
		LinearAttenuation = 0.08f;
		QuadraticAttenuation = 0;
		LightType = 0;
		IsEnabled = false;
	}
};

struct CBBasicLighting
{
	Float4				CameraPosition;
	Float4				GlobalAmbient;
	CBLightProperties	Lights[8];

	CBBasicLighting()
	{
		CameraPosition = FLOAT4_ZERO;
		GlobalAmbient = FLOAT4_IDENTITY;
	}
};