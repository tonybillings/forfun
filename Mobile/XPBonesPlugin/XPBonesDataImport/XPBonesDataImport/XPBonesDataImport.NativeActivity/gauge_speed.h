#pragma once

#define SPD_EPSILON .004363323129f
#define SPD_MAX_VALUE 600.f

using namespace std;
using namespace bones;
using namespace vmml;

class SpeedGauge : public IViewComponent
{
private:
	GLObject _frameTop;
	GLObject _frameBottom;
	GLObject _markerSpeed;
	GLObject _markerSpeedInner;

	FLOAT _speed;
	atomic<FLOAT> _speedTarget;

public:
	void SetTargetSpeed(FLOAT val)
	{
		_speedTarget = Clamp(val, 0.f, SPD_MAX_VALUE);
	}

private:
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
	void Init() override
	{
		_speed = _speedTarget = 0;

		_frameTop.SetPrimitiveType(GL_TRIANGLE_STRIP);
		_frameTop.SetPrimitiveCount(722);
		_frameTop.SetVertices(GetVertices(18.f, .2f, -.1f));
		_frameTop.SetColors(GetColors(1.f, 1.f, 1.f, 1.f));
		_frameTop.SetTranslationMatrix(VECTOR3(-740, 540.f, 0));
		_frameTop.SetScaleMatrix(VECTOR3(12, 12, 1));
		_frameTop.SetProgram("basic");
		_frameTop.SetAttributePosition("inPos");
		_frameTop.SetAttributeColor("inCol");
		_frameTop.SetUniformMVP("inMVP");

		_frameBottom.SetPrimitiveType(GL_TRIANGLE_STRIP);
		_frameBottom.SetPrimitiveCount(722);
		_frameBottom.SetVertices(GetVertices(18.f, .2f, -.1f));
		_frameBottom.SetColors(GetColors(1.f, 1.f, 1.f, 1.f));
		_frameBottom.SetTranslationMatrix(VECTOR3(-740, -540.f, 0));
		_frameBottom.SetScaleMatrix(VECTOR3(12, 12, 1));
		_frameBottom.SetProgram("basic");
		_frameBottom.SetAttributePosition("inPos");
		_frameBottom.SetAttributeColor("inCol");
		_frameBottom.SetUniformMVP("inMVP");

		_markerSpeed.SetPrimitiveType(GL_TRIANGLE_STRIP);
		_markerSpeed.SetPrimitiveCount(722);
		_markerSpeed.SetVertices(GetVertices(15.f, .2f, -.1f));
		_markerSpeed.SetColors(GetColors(1.f, .3f, .3f, 1.f));
		_markerSpeed.SetTranslationMatrix(VECTOR3(-740, -515.f, 0));
		_markerSpeed.SetScaleMatrix(VECTOR3(12, 12, 1));
		_markerSpeed.SetProgram("basic");
		_markerSpeed.SetAttributePosition("inPos");
		_markerSpeed.SetAttributeColor("inCol");
		_markerSpeed.SetUniformMVP("inMVP");

		_markerSpeedInner.SetPrimitiveType(GL_TRIANGLE_STRIP);
		_markerSpeedInner.SetPrimitiveCount(722);
		_markerSpeedInner.SetVertices(GetVertices(15.f, .01f, -.2f));
		_markerSpeedInner.SetColors(GetColors(.607f, .607f, 1.f, 1.f));
		_markerSpeedInner.SetTranslationMatrix(VECTOR3(-740, -515.f, 0));
		_markerSpeedInner.SetScaleMatrix(VECTOR3(12, 12, 1));
		_markerSpeedInner.SetProgram("basic");
		_markerSpeedInner.SetAttributePosition("inPos");
		_markerSpeedInner.SetAttributeColor("inCol");
		_markerSpeedInner.SetUniformMVP("inMVP");
	}

	void Refresh() override
	{
		FLOAT delta = _speedTarget - _speed;
		FLOAT deltaAbs = fabs(delta);

		if (deltaAbs > SPD_EPSILON)
		{
			_speed = Lerp(_speed, _speedTarget, .05f);

			FLOAT height = (_speed / SPD_MAX_VALUE) * 1030.f;

			_markerSpeed.SetTranslationMatrix(VECTOR3(-740, -515.f + height, 0));
			_markerSpeedInner.SetTranslationMatrix(VECTOR3(-740, -515.f + height, 0));
		}

		_frameTop.Refresh();
		_frameBottom.Refresh();
		_markerSpeed.Refresh();
		_markerSpeedInner.Refresh();
	}
};