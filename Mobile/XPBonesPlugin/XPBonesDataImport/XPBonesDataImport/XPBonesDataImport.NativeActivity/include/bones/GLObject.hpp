#pragma once

#include "IViewComponent.hpp"

#define PRIM_TYPE_SET	0x01
#define PRIM_COUNT_SET	0x02
#define VERTICES_SET	0x04
#define COLORS_SET		0x08
#define PROGRAM_SET		0x10
#define ATT_POS_SET		0x20
#define ATT_COL_SET		0x40
#define UNI_MVP_SET		0x80
#define SHADER_SET		0xF0
#define READY			0xFF

using std::string;

namespace bones
{
	class GLObject : public IViewComponent
	{
	private:
		BITFLAG _state;

		GLENUM _primitiveType;
		GLCOUNT _primitiveCount;
		GLINDEX _firstPrimitive;
		GLINDEX _firstVertex;
		GLSIZE _posBufferSize;
		GLSIZE _colBufferSize;
		GLCOUNT _vertexCount;

		VERTEX* _vertices;
		COLOR* _colors;

		MATRIX _xProjection;
		MATRIX _xTranslation;
		MATRIX _xRotation;
		MATRIX _xScale;

		GLUID _program;
		GLID _attPosition;
		GLID _attColor;
		GLID _uniMVP;
		GLID _uniAlpha;

		string _programName;
		string _attPositionName;
		string _attColorName;
		string _uniMvpName;

		GLUID _vboPos;
		GLUID _vboCol;

	public:
		GLObject()
		{
			_state = 0;
			
			_primitiveType = _firstVertex = _posBufferSize = _colBufferSize = _vertexCount = 0;
			_firstPrimitive = 1;
			_vertices = nullptr;
			_colors = nullptr;
			_attPosition = _attColor = _uniMVP = -1;
			_vboPos = _vboCol = 0;
			_program = 0;

			GLUID vboPos, vboCol;
			if (!GL::GenBuffers(1, &vboPos) || !GL::GenBuffers(1, &vboCol)) throw;
			_vboPos = vboPos;  _vboCol = vboCol;

			float left = (GL::Width() / 2) * -1;
			float right = GL::Width() / 2;
			float bottom = (GL::Height() / 2) * -1;
			float top = GL::Height() / 2;

			vmml::Frustum<float> frustum(left, right, bottom, top, .01f, 10.f);
			_xProjection = frustum.compute_ortho_matrix();

			VECTOR3 cameraPos(0, 0, 200);

			VECTOR3 zaxis = normalize(cameraPos - VECTOR3::ZERO);
			VECTOR3 xaxis = normalize(cross<3, FLOAT>(VECTOR3::UP, zaxis));
			VECTOR3 yaxis = cross<3, FLOAT>(zaxis, xaxis);

			MATRIX view;
			view.set_column(0, VECTOR4(xaxis.x(), xaxis.y(), xaxis.z(), -dot(xaxis, cameraPos)));
			view.set_column(1, VECTOR4(yaxis.x(), yaxis.y(), yaxis.z(), -dot(yaxis, cameraPos)));
			view.set_column(2, VECTOR4(zaxis.x(), zaxis.y(), zaxis.z(), -dot(zaxis, cameraPos)));
			view.set_column(3, VECTOR4(0, 0, 0, 1));

			//_xProjection = _xProjection * view;
		}
		
		~GLObject()
		{
			if (_vertices != nullptr) delete[] _vertices;
			if (_colors != nullptr) delete[] _colors;
		}

	private:
		void SetBufferSize()
		{
			switch (_primitiveType)
			{
			case GL_POINTS:
				_posBufferSize = _primitiveCount * 12;
				_colBufferSize = _primitiveCount * 16;
				break;
			case GL_LINES:
				_posBufferSize = (_primitiveCount * 2) * 12;
				_colBufferSize = (_primitiveCount * 2) * 16;
				break;
			case GL_LINE_STRIP:
				_posBufferSize = (_primitiveCount + 1) * 12;
				_colBufferSize = (_primitiveCount + 1) * 16;
				break;
			case GL_TRIANGLES:
				_posBufferSize = (_primitiveCount * 3) * 12;
				_colBufferSize = (_primitiveCount * 3) * 16;
				break;
			case GL_TRIANGLE_STRIP:
			case GL_TRIANGLE_FAN:
				_posBufferSize = (_primitiveCount + 2) * 12;
				_colBufferSize = (_primitiveCount + 2) * 16;
				break;
			}
		}

		void SetVertexCount()
		{
			_vertexCount = _posBufferSize / 12;
		}

		void SetFirstVertex()
		{
			switch (_primitiveType)
			{
			case GL_POINTS:
			case GL_LINE_STRIP:
				_firstVertex = _firstPrimitive - 1;
				break;
			case GL_LINES:
				_firstVertex = (_primitiveCount * 2) - 2;
				break;
			case GL_TRIANGLES:
				_firstVertex = (_primitiveCount * 3) - 3;
				break;
			case GL_TRIANGLE_STRIP:
			case GL_TRIANGLE_FAN:
				_firstVertex = _firstPrimitive - 1;
				break;
			}
		}

		void SetShader()
		{
			_program = GL::GetProgramId(_programName);
			_attPosition = GL::GetAttribLocation(_program, _attPositionName);
			_attColor = GL::GetAttribLocation(_program, _attColorName);
			_uniMVP = GL::GetUniformLocation(_program, _uniMvpName);
		}

		void TrySetShader()
		{
			if ((_state & SHADER_SET) == SHADER_SET) SetShader();
		}

	public:
		GLENUM GetPrimitiveType() const { return _primitiveType; }
		void SetPrimitiveType(GLENUM val) 
		{ 
			_primitiveType = val; 
			SetBufferSize();
			SetVertexCount();
			SetFirstVertex();
			_state |= PRIM_TYPE_SET;
		}

		GLCOUNT GetPrimitiveCount() const { return _primitiveCount; }
		void SetPrimitiveCount(GLCOUNT val) 
		{ 
			_primitiveCount = val; 
			SetBufferSize();
			SetVertexCount();
			SetFirstVertex();
			_state |= PRIM_COUNT_SET;
		}

		GLINDEX GetFirstPrimitive() const { return _firstPrimitive; }
		void SetFirstPrimitive(GLINDEX val) 
		{ 
			_firstPrimitive = val; 
			SetFirstVertex();
		}

		VERTEX* GetVertices() const { return _vertices; }
		void SetVertices(VERTEX* val)
		{ 
			_vertices = val; 
			_state |= VERTICES_SET;
		}

		COLOR* GetColors() const { return _colors; }
		void SetColors(COLOR* val)
		{ 
			_colors = val; 
			_state |= COLORS_SET;
		}

		MATRIX GetProjectionMatrix() const { return _xProjection; }
		void SetProjectionMatrix(MATRIX val) { _xProjection = val; }

		MATRIX GetTranslationMatrix() const { return _xTranslation; }
		void SetTranslationMatrix(MATRIX val) { _xTranslation = val; }
		void SetTranslationMatrix(VECTOR3 position)
		{
			_xTranslation = MATRIX().set_translation(position);
		}

		MATRIX GetRotationMatrix() const { return _xRotation; }
		void SetRotationMatrix(MATRIX val) { _xRotation = val; }
		void SetRotationMatrix(FLOAT angle, VECTOR3 axis) 
		{ 
			_xRotation = MATRIX().rotate(angle, axis);
		}

		MATRIX GetScaleMatrix() const { return _xScale; }
		void SetScaleMatrix(MATRIX val) { _xScale = val; }
		void SetScaleMatrix(VECTOR3 scale) 
		{ 
			_xScale = MATRIX().scale(scale);
		}

		void SetProgram(CSTR name) 
		{ 
			_programName = string(name);
			_state |= PROGRAM_SET;
			TrySetShader();
		}

		void SetAttributePosition(CSTR name) 
		{ 
			_attPositionName = string(name);
			_state |= ATT_POS_SET;
			TrySetShader();
		}

		void SetAttributeColor(CSTR name) 
		{ 
			_attColorName = string(name);
			_state |= ATT_COL_SET;
			TrySetShader();
		}

		void SetUniformMVP(CSTR name) 
		{ 
			_uniMvpName = string(name);
			_state |= UNI_MVP_SET;
			TrySetShader();
		}

	public:
		virtual void Init() override {}

		virtual void Refresh() override
		{
			if (_state != READY) return;

			glUseProgram(_program);
			
			glBindBuffer(GL_ARRAY_BUFFER, _vboPos);
			glBufferData(GL_ARRAY_BUFFER, _posBufferSize, _vertices + _firstVertex, GL_STATIC_DRAW);
			glVertexAttribPointer(_attPosition, 3, GL_FLOAT, false, 0, 0);

			glBindBuffer(GL_ARRAY_BUFFER, _vboCol);
			glBufferData(GL_ARRAY_BUFFER, _colBufferSize, _colors, GL_STATIC_DRAW);
			glVertexAttribPointer(_attColor, 4, GL_FLOAT, true, 0, 0);

			glUniformMatrix4fv(_uniMVP, 1, false, _xProjection * _xTranslation * _xRotation * _xScale);

			glBindBuffer(GL_ARRAY_BUFFER, 0);

			glEnableVertexAttribArray(_attPosition);
			glEnableVertexAttribArray(_attColor);

			glDrawArrays(_primitiveType, 0, _vertexCount);

			glDisableVertexAttribArray(_attPosition);
			glDisableVertexAttribArray(_attColor);
		}

		virtual bool IsReady()
		{
			return _state == READY;
		}
	};
}