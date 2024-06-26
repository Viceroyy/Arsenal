#pragma once

#include "../Entities/C_CSPlayer.h"
#include "../../Util/Color/Color.h"

enum EAlign
{
	ALIGN_TOPLEFT,
	ALIGN_TOP,
	ALIGN_TOPRIGHT,
	ALIGN_LEFT,
	ALIGN_CENTER,
	ALIGN_RIGHT,
	ALIGN_BOTTOMLEFT,
	ALIGN_BOTTOM,
	ALIGN_BOTTOMRIGHT
};

enum class EFonts {
	DEBUG,
	ESP,
	ESP_SMALL,
	MENU,
	MENU_OTHER,
	FONT_LAST
};

class CHelpers_DrawManager
{
public:
	class CFont {
	public:
		const char* m_szName;
		int m_nTall;
		int m_nWeight;
		int m_nFlags;
		HFont m_hFont;
	};


	const CFont& Get(EFonts eFont);
	void Initialize();
	void Uninitialize();
	void UpdateMatrix();

public:
	bool WorldPosToScreenPos(const Vector vWorld, Rect_t& vScreen);
	bool WorldPosToScreenPos(const Vector vWorld, Vector2D& vScreen);

	void String(const EFonts& font, int x, int y, const Color_t& clr, const EAlign& align, const char* const str, ...);
	void String(const EFonts& font, int x, int y, const Color_t& clr, const EAlign& align, const wchar_t* const str, ...);

	void Line(const int x, const int y, const int x1, const int y1, const Color_t& clr);
	void Rect(const int x, const int y, const int w, const int h, const Color_t& clr);
	void RectPercent(int x, int y, int w, int h, float t, const Color_t& clr, const Color_t& out = { 0, 0, 0, 255 }, const EAlign& align = ALIGN_LEFT, bool adjust = false);
	void OutlinedRect(const int x, const int y, const int w, const int h, const Color_t& clr);
	void GradientRect(const int x, const int y, const int w, const int h, const Color_t& clrTop, const Color_t& clrBottom, const bool bHorizontal);
	void OutlinedCircle(const int x, const int y, const int r, const int s, const Color_t clr);
	void Circle(const int x, const int y, const int r, const int s, const Color_t clr);
	void Triangle(Vector2D* v, const Color_t clr);
	void Texture(int x, int y, int w, int h, int id, const EAlign& align);

	void StartClipping(int x, int y, int w, int h);
	void EndClipping();

	void Polygon(int count, Vertex_t* vertices, const Color_t& clr);
	int GetFontHeight(const EFonts& font) const;

private:
	std::map<EFonts, CFont> m_Fonts = { };
	std::map<int, int> m_Textures = { };

public:
	int m_nScreenW = 0;
	int m_nScreenH = 0;
	VMatrix m_WorldToProjection = { };
};

namespace H { inline CHelpers_DrawManager Draw; }