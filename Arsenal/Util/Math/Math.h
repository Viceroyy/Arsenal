#pragma once

#include "Vector/Vector.h"

#undef min
#undef max

using matrix3x4_t = float[3][4];

class VMatrix
{
private:
	Vector m[4][4];

public:
	inline const matrix3x4_t& As3x4() const {
		return *((const matrix3x4_t*)this);
	}
};

class Vector4D
{
public:
	float x, y, z, w;
};

using Quaternion = Vector4D;

class CUtil_Math
{
public:
	void SinCos(float r, float* s, float* c);
	void VectorTransform(const Vector input, const matrix3x4_t& matrix, Vector& output);
	void BuildTransformedBox(Vector* v2, const Vector bbmin, const Vector bbmax, const matrix3x4_t& m);
	void PointsFromBox(const Vector mins, const Vector maxs, Vector* points);
	void VectorAngles(const Vector& forward, Vector& angles);
	void AngleVectors(const Vector vAngles, Vector* vForward);
	void ClampAngles(Vector& v);
	void RotateTriangle(Vector2D* v, const float flRotation);
	void AngleVectors(const Vector& angles, Vector* forward, Vector* right, Vector* up);

	float GetFovBetween(const Vector vSrc, const Vector vDst);
	float NormalizeAngle(const float ang);

	float clip(float n, float lower, float upper);

	float RemapValClamped(float val, float A, float B, float C, float D);

	Vector GetAngleToPosition(const Vector vFrom, const Vector vTo);
	Vector VelocityToAngles(const Vector direction);

	void MatrixSetColumn(const Vector& in, int column, matrix3x4_t& out);

	void AngleMatrix(const Vector& angles, matrix3x4_t& matrix);

public:
	template<typename T>
	inline T Clamp(const T val, const T min, const T max) {
		const T t = (val < min) ? min : val;
		return (t > max) ? max : t;
	}

	template<typename T>
	inline T Min(const T a, const T b) {
		return ((a > b) * b) + ((a <= b) * a);
	}

	template<typename T>
	inline T Max(const T a, const T b) {
		return ((a > b) * a) + ((a <= b) * b);
	}
};

namespace U { inline CUtil_Math Math; }