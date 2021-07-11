#pragma once

using namespace std;
using namespace bones;
using namespace vmml;

class TestView : public View
{
private:
	MATRIX _viewMatrix;
	MATRIX _projMatrix;

	GLObject _testObj;

private:
	void CreatePrograms()
	{
		GL::CreateProgram("basic", { "basic.vert", "basic.frag" });
	}

	void AddComponents()
	{
		_testObj.SetPrimitiveType(GL_TRIANGLE_STRIP);
		_testObj.SetPrimitiveCount(722);
		_testObj.SetVertices(GetVertices(18.f, .2f, -.1f));
		_testObj.SetColors(GetColors(1.f, 1.f, 1.f, 1.f));
		_testObj.SetTranslationMatrix(VECTOR3(-740, 540.f, 0));
		_testObj.SetScaleMatrix(VECTOR3(12, 12, 1));
		_testObj.SetProgram("basic");
		_testObj.SetAttributePosition("inPos");
		_testObj.SetAttributeColor("inCol");
		_testObj.SetUniformMVP("inMVP");
	}

	VERTEX* GetVertices(FLOAT length, FLOAT thickness, FLOAT zIndex = 0.f)
	{
		VERTEX* result = new VERTEX[724];
		VERTEX* outter = new VERTEX[361];
		VERTEX* inner = new VERTEX[361];
		FLOAT sliceAngle = M_PI_180;
		FLOAT sliceNumber = -90;

		length *= .5f;

		for (INT i = 0; i < 361; i++)
		{
			outter[i] = VERTEX(cosf(sliceNumber * sliceAngle), sinf(sliceNumber * sliceAngle), zIndex);
			sliceNumber++;
		}

		for (INT i = 0; i < 361; i++)
		{
			VERTEX* o = &outter[i];

			if (i < 180)
			{
				inner[i] = VERTEX((o->x() * thickness) + length, o->y() * thickness, zIndex);
				*o = VERTEX(o->x() + length, o->y(), zIndex);
			}
			else
			{
				inner[i] = VERTEX((o->x() * thickness) - length, o->y() * thickness, zIndex);
				*o = VERTEX(o->x() - length, o->y(), zIndex);
			}
		}

		INT oIndex, iIndex;
		oIndex = iIndex = 0;

		for (INT i = 0; i < 722; i++)
		{
			if (i % 2 == 0)
				result[i] = outter[oIndex++];
			else
				result[i] = inner[iIndex++];
		}

		result[722] = outter[0];
		result[723] = inner[0];

		delete[] outter;
		delete[] inner;
		return result;
	}

	COLOR* GetColors(FLOAT red = 1.f, FLOAT green = 1.f, FLOAT blue = 1.f, FLOAT alpha = 1.f)
	{
		COLOR* result = new COLOR[724];

		for (INT i = 0; i < 724; i++)
			result[i] = COLOR(red, green, blue, alpha);

		return result;
	}

public:
	TestView() { }

	~TestView()
	{

	}

public:
	void Init() override
	{
		CreatePrograms();
		AddComponents();
		GL::BackgroundColor(COLOR(0.f, 0.f, 0.f, 1.f));
	}

	void Refresh() override
	{
		_testObj.Refresh();
	}
};
