#include "Model.h"

Model::Model()
	: _vertexData(nullptr), _vertexType(EVertexType::VertexPositionColor), _vertexCount(0), _vertexStride(0)
{
	SetVertexType(EVertexType::VertexPositionColor);
}

Model::Model(const String& fileName)
	: _vertexData(nullptr), _vertexType(EVertexType::VertexPositionColor), _vertexCount(0), _vertexStride(0)
{
	SetVertexType(EVertexType::VertexPositionColor);
	LoadFromFile(fileName);
}

Model::Model(const Model& other) noexcept
{
	this->_vertexType = other._vertexType;
	this->_vertexCount = other._vertexCount;
	this->_vertexStride = other._vertexStride;

	switch (_vertexType)
	{
	case EVertexType::VertexPositionColor:
		_vertexData = new VertexPositionColor[_vertexCount];
		break;
	case EVertexType::VertexPositionNormal:
		_vertexData = new VertexPositionNormal[_vertexCount];
		break;
	case EVertexType::VertexPositionNormalTexture:
		_vertexData = new VertexPositionNormalTexture[_vertexCount];
		break;
	}

	memcpy(this->_vertexData, other._vertexData, (size_t)_vertexCount * (size_t)_vertexStride);

	this->_positions = other._positions;
	this->_normals = other._normals;
	this->_txtCoords = other._txtCoords;
	this->_colors = other._colors;
	this->_positionsIndices = other._positionsIndices;
	this->_normalsIndices = other._normalsIndices;
	this->_txtCoordsIndices = other._txtCoordsIndices;
	this->_colorsIndices = other._colorsIndices;
}

Model::Model(Model&& other) noexcept
{
	this->_vertexData = other._vertexData;
	this->_vertexType = other._vertexType;
	this->_vertexCount = other._vertexCount;
	this->_vertexStride = other._vertexStride;
	this->_positions = other._positions;
	this->_normals = other._normals;
	this->_txtCoords = other._txtCoords;
	this->_colors = other._colors;
	this->_positionsIndices = other._positionsIndices;
	this->_normalsIndices = other._normalsIndices;
	this->_txtCoordsIndices = other._txtCoordsIndices;
	this->_colorsIndices = other._colorsIndices;

	other._vertexData = nullptr;
}

Model::~Model()
{
	if (_vertexData != nullptr)
		delete[] _vertexData;
}

Model& Model::operator=(const Model& other) noexcept
{
	this->_vertexType = other._vertexType;
	this->_vertexCount = other._vertexCount;
	this->_vertexStride = other._vertexStride;

	switch (_vertexType)
	{
	case EVertexType::VertexPositionColor:
		_vertexData = new VertexPositionColor[_vertexCount];
		break;
	case EVertexType::VertexPositionNormal:
		_vertexData = new VertexPositionNormal[_vertexCount];
		break;
	case EVertexType::VertexPositionNormalTexture:
		_vertexData = new VertexPositionNormalTexture[_vertexCount];
		break;
	}

	memcpy(this->_vertexData, other._vertexData, (size_t)_vertexCount * (size_t)_vertexStride);

	this->_positions = other._positions;
	this->_normals = other._normals;
	this->_txtCoords = other._txtCoords;
	this->_colors = other._colors;
	this->_positionsIndices = other._positionsIndices;
	this->_normalsIndices = other._normalsIndices;
	this->_txtCoordsIndices = other._txtCoordsIndices;
	this->_colorsIndices = other._colorsIndices;

	return *this;
}

Model& Model::operator=(Model&& other) noexcept
{
	this->_vertexData = other._vertexData;
	this->_vertexType = other._vertexType;
	this->_vertexCount = other._vertexCount;
	this->_vertexStride = other._vertexStride;
	this->_positions = other._positions;
	this->_normals = other._normals;
	this->_txtCoords = other._txtCoords;
	this->_colors = other._colors;
	this->_positionsIndices = other._positionsIndices;
	this->_normalsIndices = other._normalsIndices;
	this->_txtCoordsIndices = other._txtCoordsIndices;
	this->_colorsIndices = other._colorsIndices;

	other._vertexData = nullptr;
	return *this;
}

void Model::LoadVertexPositionColorVertices()
{
	auto vertices = new VertexPositionColor[_positionsIndices.size()];
	_vertexCount = (UINT)_positionsIndices.size();

	for (int i = 0; i < (int)_vertexCount; i++)
	{
		VertexPositionColor vertex;
		vertex.Position = _positions[_positionsIndices[i]];
		vertex.Color = _colors[_colorsIndices[i]];
		vertices[i] = vertex;
	}

	_vertexData = vertices;
}

void Model::LoadVertexPositionNormalVertices()
{
	auto vertices = new VertexPositionNormal[_positionsIndices.size()];
	_vertexCount = (UINT)_positionsIndices.size();

	for (int i = 0; i < (int)_vertexCount; i++)
	{
		VertexPositionNormal vertex;
		vertex.Position = _positions[_positionsIndices[i]];
		vertex.Normal = _normals[_normalsIndices[i]];
		vertices[i] = vertex;
	}

	_vertexData = vertices;
}

void Model::LoadVertexPositionNormalTextureVertices()
{
	auto vertices = new VertexPositionNormalTexture[_positionsIndices.size()];
	_vertexCount = (UINT)_positionsIndices.size();

	for (int i = 0; i < (int)_vertexCount; i++)
	{
		VertexPositionNormalTexture vertex;
		vertex.Position = _positions[_positionsIndices[i]];
		vertex.Normal = _normals[_normalsIndices[i]];
		vertex.Texture = _txtCoords[_txtCoordsIndices[i]];
		vertices[i] = vertex;
	}

	_vertexData = vertices;
}

void Model::ScaleTextureCoordinates(float uScaleFactor, float vScaleFactor)
{
	for (auto&& uv : _txtCoords)
	{
		uv.U *= uScaleFactor;
		uv.V *= vScaleFactor;
	}

	switch (_vertexType)
	{
		case EVertexType::VertexPositionColor:
			LoadVertexPositionColorVertices();
			break;
		case EVertexType::VertexPositionNormal:
			LoadVertexPositionNormalVertices();
			break;
		case EVertexType::VertexPositionNormalTexture:
			LoadVertexPositionNormalTextureVertices();
			break;
	}
}

void Model::LoadFromFile(String fileName)
{
	using namespace std;

	auto modelBlob = File::Get(fileName);

	auto size = modelBlob->GetBufferSize();
	auto modelString = String((char*)modelBlob->GetBufferPointer());
	auto ssFile = stringstream{ modelString };

	for (String line; getline(ssFile, line, '\n');)
	{
		auto ssLine = stringstream{ line };
		List<String> tokens;

		for (String token; getline(ssLine, token, ' ');)
		{
			if (token == String("")) continue;
			replace(token.begin(), token.end(), '\r', '\0');
			tokens.push_back(token);
		}

		if (tokens.size() < 2) continue;

		String firstToken = tokens[0];

		if (firstToken == String("v"))
		{
			_positions.push_back(Vector3D(stof(tokens[1]), stof(tokens[2]), stof(tokens[3])));
		}
		else if (firstToken == String("vn"))
		{
			if (_vertexType == EVertexType::VertexPositionColor)
				SetVertexType(EVertexType::VertexPositionNormal);

			_normals.push_back(Vector3D(stof(tokens[1]), stof(tokens[2]), stof(tokens[3])));
		}
		else if (firstToken == String("vt"))
		{
			SetVertexType(EVertexType::VertexPositionNormalTexture);

			if (tokens.size() > 2)
				_txtCoords.push_back(TxtCoord2D(stof(tokens[1]), 1.f - stof(tokens[2])));
		}
		else if (firstToken == String("vc"))
		{
			if (tokens.size() > 3)
				_colors.push_back(Color(stoi(tokens[1]), stoi(tokens[2]), stoi(tokens[3])));
		}
		else if (firstToken == String("f"))
		{
			auto ProcessFace = [&](stringstream ssToken) {
				String indices[4];
				int i = 0;
				
				for (String index; getline(ssToken, index, '/');)
					indices[i++] = index;

				_positionsIndices.push_back(stoi(indices[0]) - 1);

				if (indices[1] != String(""))
					_txtCoordsIndices.push_back(stoi(indices[1]) - 1);

				if (indices[2] != String(""))
					_normalsIndices.push_back(stoi(indices[2]) - 1);

				if (indices[3] != String(""))
					_colorsIndices.push_back(stoi(indices[3]) - 1);
			};

			ProcessFace(stringstream{ tokens[1] });
			ProcessFace(stringstream{ tokens[2] });
			ProcessFace(stringstream{ tokens[3] });
		}
	}

	switch (_vertexType)
	{
		case EVertexType::VertexPositionColor:
			LoadVertexPositionColorVertices();
			break;
		case EVertexType::VertexPositionNormal:
			LoadVertexPositionNormalVertices();
			break;
		case EVertexType::VertexPositionNormalTexture:
			LoadVertexPositionNormalTextureVertices();
			break;
	}
}