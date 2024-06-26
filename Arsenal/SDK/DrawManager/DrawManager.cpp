#include "DrawManager.h"

void CHelpers_DrawManager::Initialize()
{
	m_nScreenH = I::BaseClientDLL->GetScreenHeight();
	m_nScreenW = I::BaseClientDLL->GetScreenWidth();

	m_Fonts[EFonts::DEBUG] = { "Verdana", 12, FW_DONTCARE, EFontFlags::FONTFLAG_OUTLINE };
	m_Fonts[EFonts::MENU] = { "Verdana", 12, FW_DONTCARE, EFontFlags::FONTFLAG_ANTIALIAS };
	m_Fonts[EFonts::MENU_OTHER] = { "Verdana", 12, FW_DONTCARE, EFontFlags::FONTFLAG_OUTLINE };
	m_Fonts[EFonts::ESP] = { "Verdana", 12, FW_DONTCARE, EFontFlags::FONTFLAG_OUTLINE };
	m_Fonts[EFonts::ESP_SMALL] = { "Small Fonts", 11, FW_DONTCARE, EFontFlags::FONTFLAG_OUTLINE };

	for (std::pair<const EFonts, CFont>& f : m_Fonts)
		I::MatSystemSurface->SetFontGlyphSet
		(
			f.second.m_hFont = I::MatSystemSurface->CreateFont(),
			f.second.m_szName,
			f.second.m_nTall,
			f.second.m_nWeight,
			0,
			0,
			f.second.m_nFlags
		);

	I::Cvar->ConsoleColorPrintf({ 15, 150, 15, 255 }, "[Arsenal] DrawManager Initialized!\n");
}

const CHelpers_DrawManager::CFont& CHelpers_DrawManager::Get(EFonts eFont)
{
	return m_Fonts[eFont];
}

void CHelpers_DrawManager::Uninitialize()
{
	m_Fonts.clear();

	I::Cvar->ConsoleColorPrintf({ 220, 20, 60, 255 }, "[Arsenal] DrawManager Uninitialized!\n");
}

void CHelpers_DrawManager::UpdateMatrix()
{
	CViewSetup ViewSetup = { };

	if (I::BaseClientDLL->GetPlayerView(ViewSetup))
	{
		static VMatrix WorldToView = { };
		static VMatrix ViewToProjection = { };
		static VMatrix WorldToPixels = { };

		I::RenderView->GetMatricesForView(ViewSetup, &WorldToView, &ViewToProjection, &m_WorldToProjection, &WorldToPixels);
	}
}

bool CHelpers_DrawManager::WorldPosToScreenPos(const Vector vWorld, Rect_t& vScreen)
{
	const matrix3x4_t& w2s = m_WorldToProjection.As3x4();

	const float w = (w2s[3][0] * vWorld[0] + w2s[3][1] * vWorld[1] + w2s[3][2] * vWorld[2] + w2s[3][3]);

	if (w > 0.001f)
	{
		const float flsw = static_cast<float>(m_nScreenW);
		const float flsh = static_cast<float>(m_nScreenH);
		const float fl1dbw = 1.0f / w;

		vScreen.x = static_cast<int>((flsw / 2.0f) + (0.5f * ((w2s[0][0] * vWorld[0] + w2s[0][1] * vWorld[1] + w2s[0][2] * vWorld[2] + w2s[0][3]) * fl1dbw) * flsw + 0.5f));
		vScreen.y = static_cast<int>((flsh / 2.0f) - (0.5f * ((w2s[1][0] * vWorld[0] + w2s[1][1] * vWorld[1] + w2s[1][2] * vWorld[2] + w2s[1][3]) * fl1dbw) * flsh + 0.5f));

		return true;
	}

	return false;
}

bool CHelpers_DrawManager::WorldPosToScreenPos(const Vector vWorld, Vector2D& vScreen)
{
	const matrix3x4_t& w2s = m_WorldToProjection.As3x4();

	const float w = (w2s[3][0] * vWorld[0] + w2s[3][1] * vWorld[1] + w2s[3][2] * vWorld[2] + w2s[3][3]);

	if (w > 0.001f)
	{
		const float flsw = static_cast<float>(m_nScreenW);
		const float flsh = static_cast<float>(m_nScreenH);
		const float fl1dbw = 1.0f / w;

		vScreen.x = (flsw / 2.0f) + (0.5f * ((w2s[0][0] * vWorld[0] + w2s[0][1] * vWorld[1] + w2s[0][2] * vWorld[2] + w2s[0][3]) * fl1dbw) * flsw + 0.5f);
		vScreen.y = (flsh / 2.0f) - (0.5f * ((w2s[1][0] * vWorld[0] + w2s[1][1] * vWorld[1] + w2s[1][2] * vWorld[2] + w2s[1][3]) * fl1dbw) * flsh + 0.5f);

		return true;
	}

	return false;
}

void CHelpers_DrawManager::String(const EFonts& font, int x, int y, const Color_t& clr, const EAlign& align, const char* const str, ...)
{
	va_list va_alist;
	char cbuffer[1024] = { '\0' };
	wchar_t wstr[1024] = { '\0' };

	va_start(va_alist, str);
	vsprintf_s(cbuffer, str, va_alist);
	va_end(va_alist);

	wsprintfW(wstr, L"%hs", cbuffer);

	const HFont fnt = m_Fonts[font].m_hFont;

	if (align)
	{
		int w = 0, h = 0;
		I::MatSystemSurface->GetTextSize(fnt, wstr, w, h);

		switch (align)
		{
			case ALIGN_TOPLEFT: break;
			case ALIGN_TOP: x -= w / 2; break;
			case ALIGN_TOPRIGHT: x -= w; break;
			case ALIGN_LEFT: y -= h / 2; break;
			case ALIGN_CENTER: x -= w / 2; y -= h / 2; break;
			case ALIGN_RIGHT: x -= w; y -= h / 2; break;
			case ALIGN_BOTTOMLEFT: y -= h; break;
			case ALIGN_BOTTOM: x -= w / 2; y -= h; break;
			case ALIGN_BOTTOMRIGHT: x -= w; y -= h; break;
		}
	}

	I::MatSystemSurface->DrawSetTextPos(x, y);
	I::MatSystemSurface->DrawSetTextFont(fnt);
	I::MatSystemSurface->DrawSetTextColor(clr);
	I::MatSystemSurface->DrawPrintText(wstr, wcslen(wstr));
}

void CHelpers_DrawManager::String(const EFonts& font, int x, int y, const Color_t& clr, const EAlign& align, const wchar_t* const str, ...)
{
	va_list va_alist;
	wchar_t wstr[1024] = { '\0' };

	va_start(va_alist, str);
	vswprintf_s(wstr, str, va_alist);
	va_end(va_alist);

	const HFont fnt = m_Fonts[font].m_hFont;

	if (align)
	{
		int w = 0, h = 0;
		I::MatSystemSurface->GetTextSize(fnt, wstr, w, h);

		switch (align)
		{
			case ALIGN_TOPLEFT: break;
			case ALIGN_TOP: x -= w / 2; break;
			case ALIGN_TOPRIGHT: x -= w; break;
			case ALIGN_LEFT: y -= h / 2; break;
			case ALIGN_CENTER: x -= w / 2; y -= h / 2; break;
			case ALIGN_RIGHT: x -= w; y -= h / 2; break;
			case ALIGN_BOTTOMLEFT: y -= h; break;
			case ALIGN_BOTTOM: x -= w / 2; y -= h; break;
			case ALIGN_BOTTOMRIGHT: x -= w; y -= h; break;
		}
	}

	I::MatSystemSurface->DrawSetTextPos(x, y);
	I::MatSystemSurface->DrawSetTextFont(fnt);
	I::MatSystemSurface->DrawSetTextColor(clr);
	I::MatSystemSurface->DrawPrintText(wstr, wcslen(wstr));
}

void CHelpers_DrawManager::Line(const int x, const int y, const int x1, const int y1, const Color_t& clr)
{
	I::MatSystemSurface->DrawSetColor(clr);
	I::MatSystemSurface->DrawLine(x, y, x1, y1);
}

void CHelpers_DrawManager::Rect(const int x, const int y, const int w, const int h, const Color_t& clr)
{
	I::MatSystemSurface->DrawSetColor(clr);
	I::MatSystemSurface->DrawFilledRect(x, y, x + w, y + h);
}

void CHelpers_DrawManager::RectPercent(int x, int y, int w, int h, float t, const Color_t& clr, const Color_t& out, const EAlign& align, bool adjust)
{
	if (!adjust)
		Rect(x - 1, y - 1, w + 2, h + 2, out);

	int nw = w, nh = h;

	switch (align)
	{
		case ALIGN_LEFT: nw *= t; break;
		case ALIGN_RIGHT: nw *= t; x += w - nw; break;
		case ALIGN_TOP: nh *= t; break;
		case ALIGN_BOTTOM: nh *= t; y += h - nh; break;
	}

	if (adjust)
		Rect(x - 1, y - 1, nw + 2, nh + 2, out);

	Rect(x, y, nw, nh, clr);
}

void CHelpers_DrawManager::OutlinedRect(const int x, const int y, const int w, const int h, const Color_t& clr)
{
	I::MatSystemSurface->DrawSetColor(clr);
	I::MatSystemSurface->DrawOutlinedRect(x, y, x + w, y + h);
}

void CHelpers_DrawManager::GradientRect(const int x, const int y, const int w, const int h, const Color_t& clrTop, const Color_t& clrBottom, const bool bHorizontal)
{
	I::MatSystemSurface->DrawSetColor(clrTop);
	I::MatSystemSurface->DrawFilledRectFade(x, y, x + w, y + h, clrTop.a, clrBottom.a, bHorizontal);

	I::MatSystemSurface->DrawSetColor(clrBottom);
	I::MatSystemSurface->DrawFilledRectFade(x, y, x + w, y + h, clrTop.a, clrBottom.a, bHorizontal);
}

void CHelpers_DrawManager::OutlinedCircle(const int x, const int y, const int r, const int s, const Color_t clr)
{
	I::MatSystemSurface->DrawSetColor(clr);
	I::MatSystemSurface->DrawOutlinedCircle(x, y, r, s);
}

void CHelpers_DrawManager::Circle(const int x, const int y, const int r, const int s, const Color_t clr)
{
	static int s_nTexture = I::MatSystemSurface->CreateNewTextureID(true);

	CUtlVector<Vertex_t> vecVertices = { };

	const float flStep = (6.28318530718f / static_cast<float>(s));

	for (float n = 0.0f; n < 6.28318530718f; n += flStep)
		vecVertices.AddToTail(Vertex_t({ (static_cast<float>(r) * ::cosf(n) + x), (static_cast<float>(r) * ::sinf(n) + y) }, { 0.0f, 0.0f }));

	if (vecVertices.Count())
	{
		I::MatSystemSurface->DrawSetTexture(s_nTexture);
		I::MatSystemSurface->DrawSetColor(clr);
		I::MatSystemSurface->DrawTexturedPolygon(s, vecVertices.Base(), true);
	}
}

void CHelpers_DrawManager::Polygon(int count, Vertex_t* vertices, const Color_t& clr)
{
	static int id = 0;
	if (!I::MatSystemSurface->IsTextureIDValid(id))
		id = I::MatSystemSurface->CreateNewTextureID();

	I::MatSystemSurface->DrawSetColor(clr.r, clr.g, clr.b, clr.a);
	I::MatSystemSurface->DrawSetTexture(id);
	I::MatSystemSurface->DrawTexturedPolygon(count, vertices);
}

int CHelpers_DrawManager::GetFontHeight(const EFonts& font) const
{
	return m_Fonts.at(font).m_nTall;
}

void CHelpers_DrawManager::Triangle(Vector2D* v, const Color_t clr)
{
	static const int s_nTexture = I::MatSystemSurface->CreateNewTextureID(true);

	Vertex_t Vertices[3] = { { v[0] }, { v[1] }, { v[2] } };

	I::MatSystemSurface->DrawSetTexture(s_nTexture);
	I::MatSystemSurface->DrawSetColor(clr);
	I::MatSystemSurface->DrawTexturedPolygon(3, Vertices, true);
}

void CHelpers_DrawManager::Texture(int x, int y, int w, int h, int id, const EAlign& align)
{
	switch (align)
	{
		case ALIGN_TOPRIGHT: x -= w; break;
		case ALIGN_BOTTOMLEFT: y -= h; break;
		case ALIGN_TOP: x -= (w / 2); break;
		case ALIGN_LEFT: y -= (h / 2); break;
	}

	I::MatSystemSurface->DrawSetTexture(id);
	I::MatSystemSurface->DrawSetColor(255, 255, 255, 255);
	I::MatSystemSurface->DrawTexturedRect(x, y, x + w, y + h);
}

void CHelpers_DrawManager::StartClipping(int x, int y, int w, int h)
{
	I::MatSystemSurface->DisableClipping(false);
	I::MatSystemSurface->SetClippingRect(x, y, x + w, y + h);
}

void CHelpers_DrawManager::EndClipping()
{
	I::MatSystemSurface->DisableClipping(true);
}