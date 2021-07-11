#pragma once

#define ATT_EPSILON .004363323129f

using namespace std;
using namespace bones;
using namespace vmml;

class AttitudeGauge : public IViewComponent
{
private:
	GLObject _markerRoll;
	GLObject _markerRollInner;
	GLObject _markerRollLeft;
	GLObject _markerRollRight;
	GLObject _markerPitch;
	GLObject _markerPitchInner;
	GLObject _xAxis;
	GLObject _yAxis;

	FLOAT _rollAngle;
	atomic<FLOAT> _rollAngleTarget;
	FLOAT _pitchAngle;
	atomic<FLOAT> _pitchAngleTarget;

public:
	void SetTargetRoll(FLOAT val) { _rollAngleTarget = val * M_PI_180; }
	void SetTargetPitch(FLOAT val) { _pitchAngleTarget = val * M_PI_180; }

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
		_rollAngle = _rollAngleTarget = _pitchAngle = _pitchAngleTarget = 0;

		_markerRoll.SetPrimitiveType(GL_TRIANGLE_STRIP);
		_markerRoll.SetPrimitiveCount(722);
		_markerRoll.SetVertices(GetVertices(68.5f, .2f, -.1f));
		_markerRoll.SetColors(GetColors(1.f, .3f, .3f, 1.f));
		_markerRoll.SetScaleMatrix(VECTOR3(12.f, 12.f, 1.f));
		_markerRoll.SetProgram("basic");
		_markerRoll.SetAttributePosition("inPos");
		_markerRoll.SetAttributeColor("inCol");
		_markerRoll.SetUniformMVP("inMVP");

		_markerRollInner.SetPrimitiveType(GL_TRIANGLE_STRIP);
		_markerRollInner.SetPrimitiveCount(722);
		_markerRollInner.SetVertices(GetVertices(68.5f, .01f, -.2f));
		_markerRollInner.SetColors(GetColors(.607f, .607f, 1.f, 1.f));
		_markerRollInner.SetScaleMatrix(VECTOR3(12.f, 12.f, 1.f));
		_markerRollInner.SetProgram("basic");
		_markerRollInner.SetAttributePosition("inPos");
		_markerRollInner.SetAttributeColor("inCol");
		_markerRollInner.SetUniformMVP("inMVP");

		_markerRollRight.SetPrimitiveType(GL_TRIANGLE_STRIP);
		_markerRollRight.SetPrimitiveCount(62);
		_markerRollRight.SetFirstPrimitive(149);
		_markerRollRight.SetVertices(GetVertices(0.f, .975f, -.1f));
		_markerRollRight.SetColors(GetColors(1.f, .3f, .3f, 1.f));
		_markerRollRight.SetScaleMatrix(VECTOR3(425.f, 425.f, 1.f));
		_markerRollRight.SetProgram("basic");
		_markerRollRight.SetAttributePosition("inPos");
		_markerRollRight.SetAttributeColor("inCol");
		_markerRollRight.SetUniformMVP("inMVP");

		_markerRollLeft.SetPrimitiveType(GL_TRIANGLE_STRIP);
		_markerRollLeft.SetPrimitiveCount(62);
		_markerRollLeft.SetFirstPrimitive(509);
		_markerRollLeft.SetVertices(GetVertices(0, .975f, -.1f));
		_markerRollLeft.SetColors(GetColors(1.f, .3f, .3f, 1.f));
		_markerRollLeft.SetScaleMatrix(VECTOR3(425, 425, 1));
		_markerRollLeft.SetProgram("basic");
		_markerRollLeft.SetAttributePosition("inPos");
		_markerRollLeft.SetAttributeColor("inCol");
		_markerRollLeft.SetUniformMVP("inMVP");

		_markerPitch.SetPrimitiveType(GL_TRIANGLE_STRIP);
		_markerPitch.SetPrimitiveCount(722);
		_markerPitch.SetVertices(GetVertices(68.5f, .2f, -.3f));
		_markerPitch.SetColors(GetColors(1.f, .3f, .3f, 1.f));
		_markerPitch.SetScaleMatrix(VECTOR3(12.f, 12.f, 1.f));
		_markerPitch.SetProgram("basic");
		_markerPitch.SetAttributePosition("inPos");
		_markerPitch.SetAttributeColor("inCol");
		_markerPitch.SetUniformMVP("inMVP");

		_markerPitchInner.SetPrimitiveType(GL_TRIANGLE_STRIP);
		_markerPitchInner.SetPrimitiveCount(722);
		_markerPitchInner.SetVertices(GetVertices(68.5f, .01f, -.4f));
		_markerPitchInner.SetColors(GetColors(.607f, .607f, 1.f, 1.f));
		_markerPitchInner.SetScaleMatrix(VECTOR3(12.f, 6.f, 1.f));
		_markerPitchInner.SetProgram("basic");
		_markerPitchInner.SetAttributePosition("inPos");
		_markerPitchInner.SetAttributeColor("inCol");
		_markerPitchInner.SetUniformMVP("inMVP");

		_xAxis.SetPrimitiveType(GL_TRIANGLE_STRIP);
		_xAxis.SetPrimitiveCount(722);
		_xAxis.SetVertices(GetVertices(102.f, .005f, -.8f));
		_xAxis.SetColors(GetColors(1.f, .8f, .8f, .5f));
		_xAxis.SetScaleMatrix(VECTOR3(8.f, 4.f, 1.f));
		_xAxis.SetProgram("basic");
		_xAxis.SetAttributePosition("inPos");
		_xAxis.SetAttributeColor("inCol");
		_xAxis.SetUniformMVP("inMVP");

		_yAxis.SetPrimitiveType(GL_TRIANGLE_STRIP);
		_yAxis.SetPrimitiveCount(722);
		_yAxis.SetVertices(GetVertices(102.f, .01f, -.9f));
		_yAxis.SetColors(GetColors(1.f, .8f, .8f, .5f));
		_yAxis.SetRotationMatrix(1.5708f, VECTOR3::FORWARD);
		_yAxis.SetScaleMatrix(VECTOR3(8.f, 4.f, 1));
		_yAxis.SetProgram("basic");
		_yAxis.SetAttributePosition("inPos");
		_yAxis.SetAttributeColor("inCol");
		_yAxis.SetUniformMVP("inMVP");
	}

	void Refresh() override
	{
		FLOAT delta = _rollAngleTarget - _rollAngle;
		FLOAT deltaAbs = fabs(delta);

		if (deltaAbs > ATT_EPSILON)
		{
			_rollAngle = Lerp(_rollAngle, _rollAngleTarget, .05f);

			_markerRoll.SetRotationMatrix(_rollAngle, VECTOR3::FORWARD);
			_markerRollInner.SetRotationMatrix(_rollAngle, VECTOR3::FORWARD);
			_markerRollRight.SetRotationMatrix(_rollAngle, VECTOR3::FORWARD);
			_markerRollLeft.SetRotationMatrix(_rollAngle, VECTOR3::FORWARD);
		}

		delta = _pitchAngleTarget - _pitchAngle;
		deltaAbs = fabs(delta);

		if (deltaAbs > ATT_EPSILON)
		{
			_pitchAngle = Lerp(_pitchAngle, _pitchAngleTarget, .05f);

			_markerPitch.SetTranslationMatrix(VECTOR3(0, (_pitchAngle / 1.5708f) * 400.f, 0));
			_markerPitch.SetScaleMatrix(VECTOR3(12.f - ((fabs(_pitchAngle) / 1.5708f) * 10.f), 12.f, 1.f));
			_markerPitchInner.SetTranslationMatrix(VECTOR3(0, (_pitchAngle / 1.5708f) * 400.f, 0));
			_markerPitchInner.SetScaleMatrix(VECTOR3(12.f - ((fabs(_pitchAngle) / 1.5708f) * 10.f), 12.f, 1.f));
		}

		_markerRoll.Refresh();
		_markerRollInner.Refresh();
		_markerRollRight.Refresh();
		_markerRollLeft.Refresh();
		_markerPitch.Refresh();
		_markerPitchInner.Refresh();
		_xAxis.Refresh();
		_yAxis.Refresh();
	}
};