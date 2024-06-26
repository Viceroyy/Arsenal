#pragma once
#include <string>
#include <format>

using byte = unsigned char;
struct Color_t
{
	byte r = 0, g = 0, b = 0, a = 0;

	bool operator==(Color_t other) const
	{
		return r == other.r && g == other.g && b == other.b && a == other.a;
	}

	bool operator!=(Color_t other) const
	{
		return r != other.r || g != other.g || b != other.b || a != other.a;
	}

	std::string ToHex() const
	{
		return std::format("\x7{:02X}{:02X}{:02X}", r, g, b);
	}

	std::string ToHexA() const
	{
		return std::format("\x8{:02X}{:02X}{:02X}{:02X}", r, g, b, a);
	}

	Color_t Lerp(Color_t to, float t) const
	{
		return {
			byte(r + (to.r - r) * t),
			byte(g + (to.g - g) * t),
			byte(b + (to.b - b) * t),
			byte(a + (to.a - a) * t),
		};
	}
};

#define COLOR_RED		Color_t(255, 0, 0, 255)
#define COLOR_BLUE		Color_t(0, 0, 255, 255)
#define COLOR_YELLOW	Color_t(255, 178, 0, 255)
#define COLOR_GREEN		Color_t(0, 255, 0, 255)
#define COLOR_GREY		Color_t(204, 204, 204, 255)
#define COLOR_WHITE		Color_t(255, 255, 255, 255)
#define COLOR_BLACK		Color_t(0, 0, 0, 255)