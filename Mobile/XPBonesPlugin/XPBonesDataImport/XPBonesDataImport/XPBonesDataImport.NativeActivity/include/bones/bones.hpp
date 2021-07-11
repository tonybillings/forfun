#pragma once

#define NULL_GLID -1
#define NULL_GLUID 0
#define MATRIX_ID vmml::Matrix<4, 4>::IDENTITY
#define MATRIX_ZERO vmml::Matrix<4, 4>::ZERO
#define M_2PI 6.283185307179586476925286766559
#define M_180_PI 57.295779513082320876798154814105
#define M_PI_180 0.01745329251994329576923690768489

namespace bones
{
	typedef float FLOAT;
	typedef int INT;
	typedef int BOOL;
	typedef int BITFLAG;
	typedef const char* CSTR;
	typedef unsigned char BYTE;
	typedef char SBYTE;
	typedef GLint GLID;
	typedef GLuint GLUID;
	typedef GLchar GLCHAR;
	typedef GLsizei GLSIZE;
	typedef GLuint GLCOUNT;
	typedef GLint GLINDEX;
	typedef GLuint GLENUM;
	typedef vmml::Vector<2> VECTOR2;
	typedef vmml::Vector<3> VECTOR3;
	typedef vmml::Vector<4> VECTOR4;
	typedef vmml::Matrix<4, 4> MATRIX;
	typedef vmml::Vector<4, int> RGBA;
	typedef vmml::Vector<4, float> COLOR;
	typedef vmml::Vector<3, float> VERTEX;

	static inline FLOAT Lerp(FLOAT current, FLOAT target, FLOAT amount)
	{
		return (current * (1.f - amount)) + (target * amount);
	}

	template <class T>
	static inline T Clamp(T value, T lower, T upper)
	{
		return std::min<T>(upper, std::max<T>(value, lower));
	}
}

#include "APP.hpp"
#include "GLObject.hpp"
