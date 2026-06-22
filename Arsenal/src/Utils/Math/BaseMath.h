#pragma once
#include <cmath>
#include <algorithm>

template <class T> inline int sign(T t)
{
	return (t > T(0)) - (t < T(0));
}

inline float fnmodf(float flX, float flY)
{
	return fmodf(flX, flY) + (flX < 0 ? flY : 0);
}

namespace Math
{
	template <class T = float>
	constexpr T PI_V = static_cast<T>(3.141592653589793);
	constexpr float PI = PI_V<>;

	template <class T = float>
	constexpr T RAD_V = static_cast<T>(57.29577951308232);
	constexpr float RAD = RAD_V<>;

	template <class T = float>
	constexpr T E_V = static_cast<T>(2.718281828459045);
	constexpr float E = E_V<>;

	template <class T = float>
	inline T Deg2Rad(T v)
	{
		return v * PI_V<T> / T(180);
	}

	template <class T = float>
	inline T Rad2Deg(T v)
	{
		return v * T(180) / PI_V<T>;
	}

	inline void SinCos(float flRadians, float& flSin, float& flCos)
	{
		flSin = std::sin(flRadians);
		flCos = std::cos(flRadians);
	}

	inline float DeltaAngle(float a, float b, float r = 360.f)
	{
		float flOut = fmodf(a - b + r / 2, r);
		return flOut += flOut < 0 ? r / 2 : -r / 2;
	}

	inline float ShortDist(float a, float b, float r = 360.f)
	{
		const float flDelta = fmodf(a - b, r);
		return fmodf(2 * flDelta, r) - flDelta;
	}

	inline float Lerp(float a, float b, float t)
	{
		return a + (b - a) * t;
	}

	inline float LerpAngle(float a, float b, float t, float r = 360.f)
	{
		return a - Math::ShortDist(a, b, r) * t;
	}

	inline float NormalizeAngle(float flAngle, float flRange = 360.f)
	{
		return std::isfinite(flAngle) ? std::remainder(flAngle, flRange) : 0.f;
	}

	inline float NormalizeRad(float flAngle, float flRange = PI * 2)
	{
		return std::isfinite(flAngle) ? std::remainder(flAngle, flRange) : 0.f;
	}

	inline float ClampNormalizeAngle(float flAngle, float flRange = 180.f)
	{
		return std::isfinite(flAngle) ? (flAngle > flRange ? -flRange : flAngle < -flRange ? flRange : flAngle) : 0.f;
	}

	inline float ClampNormalizeRad(float flAngle, float flRange = PI)
	{
		return std::isfinite(flAngle) ? (flAngle > flRange ? -flRange : flAngle < -flRange ? flRange : flAngle) : 0.f;
	}

	inline float SimpleSpline(float val)
	{
		float flSquared = powf(val, 2);
		return 3 * flSquared - 2 * flSquared * val;
	}

	inline float RemapVal(float flVal, float a, float b, float c, float d, bool bClamp = true)
	{
		if (a == b)
			return flVal >= b ? d : c;

		float t = (flVal - a) / (b - a);
		if (bClamp)
			t = std::clamp(t, 0.f, 1.f);

		return Lerp(c, d, t);
	}

	inline float SimpleSplineRemapVal(float flVal, float a, float b, float c, float d, bool bClamp = true)
	{
		if (a == b)
			return flVal >= b ? d : c;

		float t = (flVal - a) / (b - a);
		if (bClamp)
			t = std::clamp(t, 0.f, 1.f);

		return Lerp(c, d, SimpleSpline(t));
	}
}
