#pragma once

#include <string>
#include <algorithm>
#include "File.h"
#include "Exception.h"
#include "GfxTypes.h"

class Model
{
protected:
	void*							_vertexData;
	EVertexType						_vertexType;
	UINT							_vertexCount;
	UINT							_vertexStride;
	List<Vector3D>					_positions;
	List<Vector3D>					_normals;
	List<TxtCoord2D>				_txtCoords;
	List<Color>						_colors;
	List<int>						_positionsIndices;
	List<int>						_normalsIndices;
	List<int>						_txtCoordsIndices;
	List<int>						_colorsIndices;

public:
	Model();
	Model(const String& fileName);
	Model(const Model& other) noexcept;
	Model(Model&& other) noexcept;
	virtual ~Model();

public:
	Model& operator=(const Model& other) noexcept;
	Model& operator=(Model&& other) noexcept;

protected:
	void LoadVertexPositionColorVertices();
	void LoadVertexPositionNormalVertices();
	void LoadVertexPositionNormalTextureVertices();

public:
	void LoadFromFile(String fileName);
	void ScaleTextureCoordinates(float xScaleFactor, float yScaleFactor);

	inline EVertexType GetVertexType() const noexcept
	{
		return _vertexType;
	}

	inline void SetVertexType(EVertexType value) noexcept
	{
		_vertexType = value;

		switch (_vertexType)
		{
		case EVertexType::VertexPositionColor:
			_vertexStride = sizeof(VertexPositionColor);
			break;
		case EVertexType::VertexPositionNormal:
			_vertexStride = sizeof(VertexPositionNormal);
			break;
		case EVertexType::VertexPositionNormalTexture:
			_vertexStride = sizeof(VertexPositionNormalTexture);
			break;
		}
	}

	inline UINT GetStride() const noexcept
	{
		return _vertexStride;
	}

	inline const UINT* const GetStrides()
	{
		return &_vertexStride;
	}

	inline const void* const GetVertexData() noexcept
	{
		return _vertexData;
	}

	template <class T>
	inline const T* const GetVertexData() noexcept
	{
		return reinterpret_cast<T*>(_vertexData);
	}

	inline UINT GetVertexCount() const noexcept
	{
		return _vertexCount;
	}

	inline bool IsEmpty() const noexcept
	{
		return _vertexCount == 0;
	}
};