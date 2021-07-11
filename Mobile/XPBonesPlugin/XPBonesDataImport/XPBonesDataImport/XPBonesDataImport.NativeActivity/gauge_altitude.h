#pragma once

#define ALT_EPSILON .004363323129f
#define ALT_MAX_VALUE 40000.f

using namespace std;
using namespace bones;
using namespace vmml;

class AltitudeGauge : public IViewComponent
{
private:
	GLObject _frameTop;
	GLObject _frameBottom;
	GLObject _markerAltitude;
	GLObject _markerAltitudeInner;

	FLOAT _altitude;
	atomic<FLOAT> _altitudeTarget;

public:
	void SetTargetAltitude(FLOAT val) 
	{ 
		_altitudeTarget = Clamp(val, 0.f, ALT_MAX_VALUE); 
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
		_altitude = _altitudeTarget = 0;

		_frameTop.SetPrimitiveType(GL_TRIANGLE_STRIP);
		_frameTop.SetPrimitiveCount(722);
		_frameTop.SetVertices(GetVertices(18.f, .2f, -.1f));
		_frameTop.SetColors(GetColors(1.f, 1.f, 1.f, 1.f));
		_frameTop.SetTranslationMatrix(VECTOR3(740, 540.f, 0));
		_frameTop.SetScaleMatrix(VECTOR3(12, 12, 1));
		_frameTop.SetProgram("basic");
		_frameTop.SetAttributePosition("inPos");
		_frameTop.SetAttributeColor("inCol");
		_frameTop.SetUniformMVP("inMVP");

		_frameBottom.SetPrimitiveType(GL_TRIANGLE_STRIP);
		_frameBottom.SetPrimitiveCount(722);
		_frameBottom.SetVertices(GetVertices(18.f, .2f, -.1f));
		_frameBottom.SetColors(GetColors(1.f, 1.f, 1.f, 1.f));
		_frameBottom.SetTranslationMatrix(VECTOR3(740, -540.f, 0));
		_frameBottom.SetScaleMatrix(VECTOR3(12, 12, 1));
		_frameBottom.SetProgram("basic");
		_frameBottom.SetAttributePosition("inPos");
		_frameBottom.SetAttributeColor("inCol");
		_frameBottom.SetUniformMVP("inMVP");

		_markerAltitude.SetPrimitiveType(GL_TRIANGLE_STRIP);
		_markerAltitude.SetPrimitiveCount(722);
		_markerAltitude.SetVertices(GetVertices(15.f, .2f, -.1f));
		_markerAltitude.SetColors(GetColors(1.f, .3f, .3f, 1.f));
		_markerAltitude.SetTranslationMatrix(VECTOR3(740, -515.f, 0));
		_markerAltitude.SetScaleMatrix(VECTOR3(12, 12, 1));
		_markerAltitude.SetProgram("basic");
		_markerAltitude.SetAttributePosition("inPos");
		_markerAltitude.SetAttributeColor("inCol");
		_markerAltitude.SetUniformMVP("inMVP");

		_markerAltitudeInner.SetPrimitiveType(GL_TRIANGLE_STRIP);
		_markerAltitudeInner.SetPrimitiveCount(722);
		_markerAltitudeInner.SetVertices(GetVertices(15.f, .01f, -.2f));
		_markerAltitudeInner.SetColors(GetColors(.607f, .607f, 1.f, 1.f));
		_markerAltitudeInner.SetTranslationMatrix(VECTOR3(740, -515.f, 0));
		_markerAltitudeInner.SetScaleMatrix(VECTOR3(12, 12, 1));
		_markerAltitudeInner.SetProgram("basic");
		_markerAltitudeInner.SetAttributePosition("inPos");
		_markerAltitudeInner.SetAttributeColor("inCol");
		_markerAltitudeInner.SetUniformMVP("inMVP");
	}

	void Refresh() override
	{
		FLOAT delta = _altitudeTarget - _altitude;
		FLOAT deltaAbs = fabs(delta);

		if (deltaAbs > ALT_EPSILON)
		{
			_altitude = Lerp(_altitude, _altitudeTarget, .05f);

			FLOAT height = (_altitude / ALT_MAX_VALUE) * 1030.f;
			
			_markerAltitude.SetTranslationMatrix(VECTOR3(740, -515.f + height, 0));
			_markerAltitudeInner.SetTranslationMatrix(VECTOR3(740, -515.f + height, 0));
		}

		_frameTop.Refresh();
		_frameBottom.Refresh();
		_markerAltitude.Refresh();
		_markerAltitudeInner.Refresh();
	}
};