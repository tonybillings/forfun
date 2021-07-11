#pragma once

#define HDG_EPSILON .004363323129f

using namespace std;
using namespace bones;
using namespace vmml;

class HeadingGauge : public IViewComponent
{
private:
	GLObject _frame;
	GLObject _markerOutter;
	GLObject _markerInner;

	FLOAT _xOffset;
	FLOAT _yOffset;
	FLOAT _headingRotAngle;
	atomic<FLOAT> _headingRotAngleTarget;

public:
	void SetTargetHeading(FLOAT val) { _headingRotAngleTarget = val * M_PI_180; }

private:
	VERTEX* GetVertices(FLOAT zIndex = 0.f)
	{
		VERTEX* result = new VERTEX[722];
		VERTEX* outter = new VERTEX[361];
		VERTEX* inner = new VERTEX[361];
		FLOAT sliceAngle = M_PI_180;
		FLOAT sliceNumber = 0;

		for (INT i = 0; i < 361; i++)
		{
			outter[i] = VERTEX(cosf(sliceNumber * sliceAngle), sinf(sliceNumber * sliceAngle), zIndex);
			sliceNumber++;
		}

		for (INT i = 0; i < 364; i++)
		{
			auto outterV = outter[i];
			inner[i] = VERTEX(outterV.x() * .9f, outterV.y() * .9f, zIndex);
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

		delete[] outter;
		delete[] inner;
		return result;
	}

	COLOR* GetColors(FLOAT red = 1.f, FLOAT green = 1.f, FLOAT blue = 1.f, FLOAT alpha = 1.f)
	{
		COLOR* result = new COLOR[722];

		for (INT i = 0; i < 722; i++)
			result[i] = COLOR(red, green, blue, alpha);

		return result;
	}

public:
	void Init() override
	{
		_xOffset = (GL::Width() / 2.f) * -1.f;
		_yOffset = (GL::Height() / 2.f) * 1.f;

		InitFrame();
		InitMarker();
	}

	void InitFrame()
	{
		_frame.SetPrimitiveType(GL_TRIANGLE_STRIP);
		_frame.SetPrimitiveCount(720);
		
		_frame.SetVertices(GetVertices(-.3f));
		_frame.SetColors(GetColors());

		_frame.SetScaleMatrix(VECTOR3(500, 500, 1));

		_frame.SetProgram("basic");
		_frame.SetAttributePosition("inPos");
		_frame.SetAttributeColor("inCol");
		_frame.SetUniformMVP("inMVP");
	}

	void InitMarker()
	{
		_headingRotAngle = _headingRotAngleTarget = 0.f;

		_markerOutter.SetPrimitiveType(GL_TRIANGLE_STRIP);
		_markerOutter.SetPrimitiveCount(16);
		_markerOutter.SetVertices(GetVertices(-.2f));
		_markerOutter.SetColors(GetColors(1.f, .3f, .3f, 1.f));
		_markerOutter.SetScaleMatrix(VECTOR3(500, 500, 1));
		_markerOutter.SetRotationMatrix(_headingRotAngle + .0698132f - 1.5708f, VECTOR3::FORWARD);
		_markerOutter.SetProgram("basic");
		_markerOutter.SetAttributePosition("inPos");
		_markerOutter.SetAttributeColor("inCol");
		_markerOutter.SetUniformMVP("inMVP");

		_markerInner.SetPrimitiveType(GL_TRIANGLE_STRIP);
		_markerInner.SetPrimitiveCount(2);
		_markerInner.SetVertices(GetVertices(-.1f));
		_markerInner.SetColors(GetColors(.607f, .607f, 1.f, 1.f));
		_markerInner.SetScaleMatrix(VECTOR3(500, 500, 1));
		_markerInner.SetRotationMatrix(_headingRotAngle + .00872665f - 1.5708f, VECTOR3::FORWARD);
		_markerInner.SetProgram("basic");
		_markerInner.SetAttributePosition("inPos");
		_markerInner.SetAttributeColor("inCol");
		_markerInner.SetUniformMVP("inMVP");
	}

	void Refresh() override
	{
		_frame.Refresh();

		FLOAT delta = _headingRotAngleTarget - _headingRotAngle;
		FLOAT deltaAbs = fabs(delta);

		if (deltaAbs > HDG_EPSILON)
		{
			if (deltaAbs > M_PI)
			{
				FLOAT shortDelta = M_2PI - deltaAbs;
				FLOAT adjustedTarget = (delta < 0) ? _headingRotAngle + shortDelta : _headingRotAngle - shortDelta;
				_headingRotAngle = Lerp(_headingRotAngle, adjustedTarget, .05f);
			}
			else
			{
				_headingRotAngle = Lerp(_headingRotAngle, _headingRotAngleTarget, .05f);
			}

			_headingRotAngle = (_headingRotAngle < 0) ? M_2PI + _headingRotAngle : _headingRotAngle;
			_headingRotAngle = (_headingRotAngle > M_2PI) ? _headingRotAngle - M_2PI : _headingRotAngle;

			_markerOutter.SetRotationMatrix((_headingRotAngle + .0698132f) - 1.5708f, VECTOR3::FORWARD);
			_markerInner.SetRotationMatrix((_headingRotAngle + .00872665f) - 1.5708f, VECTOR3::FORWARD);
		}
		
		_markerOutter.Refresh();
		_markerInner.Refresh();
	}
};