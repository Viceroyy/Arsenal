#pragma once

#include "../Util/Color/Color.h"
#include "EntityCacher/EntityCacher.h"
#include "../Features/CFG.h"
#include "DrawManager/DrawManager.h"
#include <random>
#include "../Util/Icons.h"
#include "../Features/Players/Players.h"

namespace Util
{
	inline Color_t Rainbow()
	{
		float t = TICKS_TO_TIME(I::GlobalVars->tickcount);

		int r = static_cast<int>(std::round(std::cos(I::GlobalVars->realtime + t + 0.0f) * 127.5f + 127.5f));
		int g = static_cast<int>(std::round(std::cos(I::GlobalVars->realtime + t + 2.0f) * 127.5f + 127.5f));
		int b = static_cast<int>(std::round(std::cos(I::GlobalVars->realtime + t + 4.0f) * 127.5f + 127.5f));

		return Color_t{ static_cast<byte>(r), static_cast<byte>(g), static_cast<byte>(b), 255 };
	}

	inline Color_t RainbowTickOffset(int nTick)
	{
		float t = TICKS_TO_TIME(nTick);

		int r = static_cast<int>(std::lround(std::cos((I::GlobalVars->realtime * 2.0f) + t + 0.0f) * 127.5f + 127.5f));
		int g = static_cast<int>(std::lround(std::cos((I::GlobalVars->realtime * 2.0f) + t + 2.0f) * 127.5f + 127.5f));
		int b = static_cast<int>(std::lround(std::cos((I::GlobalVars->realtime * 2.0f) + t + 4.0f) * 127.5f + 127.5f));

		return Color_t{ static_cast<byte>(r), static_cast<byte>(g), static_cast<byte>(b), 255 };
	}

	inline int CreateTextureFromArray(const unsigned char* rgba, int w, int h)
	{
		int nTextureIdOut = I::MatSystemSurface->CreateNewTextureID(true);
		I::MatSystemSurface->DrawSetTextureRGBAEx(nTextureIdOut, rgba, w, h, IMAGE_FORMAT_BGRA8888);
		return nTextureIdOut;
	}

	inline int GetCat(int nFrame)
	{
		if (nFrame < 0 || nFrame > 3)
			return 0;

		static const int arrFrames[4] =
		{
			CreateTextureFromArray(Icons::cat_0, 12, 12),
			CreateTextureFromArray(Icons::cat_1, 12, 12),
			CreateTextureFromArray(Icons::cat_2, 12, 12),
			CreateTextureFromArray(Icons::cat_3, 12, 12)
		};

		return arrFrames[nFrame];
	}

	inline int GetCat2(int nFrame)
	{
		if (nFrame < 0 || nFrame > 3)
			return 0;

		static const int arrFrames[4] =
		{
			CreateTextureFromArray(Icons::cat2_0, 12, 12),
			CreateTextureFromArray(Icons::cat2_1, 12, 12),
			CreateTextureFromArray(Icons::cat2_2, 12, 12),
			CreateTextureFromArray(Icons::cat2_3, 12, 12)
		};

		return arrFrames[nFrame];
	}

	inline int GetCatSleep(int nFrame)
	{
		if (nFrame < 0 || nFrame > 3)
			return 0;

		static const int arrFrames[4] =
		{
			CreateTextureFromArray(Icons::cat_sleep0, 16, 8),
			CreateTextureFromArray(Icons::cat_sleep1, 16, 8),
			CreateTextureFromArray(Icons::cat_sleep2, 16, 8),
			CreateTextureFromArray(Icons::cat_sleep3, 16, 8)
		};

		return arrFrames[nFrame];
	}

	inline int GetCatRun(int nFrame)
	{
		if (nFrame < 0 || nFrame > 7)
			return 0;

		static const int arrFrames[8] =
		{
			CreateTextureFromArray(Icons::cat_run0, 20, 13),
			CreateTextureFromArray(Icons::cat_run1, 20, 13),
			CreateTextureFromArray(Icons::cat_run2, 20, 13),
			CreateTextureFromArray(Icons::cat_run3, 20, 13),
			CreateTextureFromArray(Icons::cat_run4, 20, 13),
			CreateTextureFromArray(Icons::cat_run5, 20, 13),
			CreateTextureFromArray(Icons::cat_run6, 20, 13),
			CreateTextureFromArray(Icons::cat_run7, 20, 13)
		};

		return arrFrames[nFrame];
	}

	inline static int RandInt(int min, int max)
	{
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<> distr(min, max);
		return distr(gen);
	}

	inline bool IsOnScreen(C_BaseEntity* pEntity, const matrix3x4_t& transform, float* pLeft, float* pRight, float* pTop, float* pBottom)
	{
		Vector vMins = pEntity->m_vecMins(), vMaxs = pEntity->m_vecMaxs();

		float flLeft = 0.f, flRight = 0.f, flTop = 0.f, flBottom = 0.f;
		const Vector vPoints[] =
		{
			Vector(0.f, 0.f, vMins.z),
			Vector(0.f, 0.f, vMaxs.z),
			Vector(vMins.x, vMins.y, vMaxs.z * 0.5f),
			Vector(vMins.x, vMaxs.y, vMaxs.z * 0.5f),
			Vector(vMaxs.x, vMins.y, vMaxs.z * 0.5f),
			Vector(vMaxs.x, vMaxs.y, vMaxs.z * 0.5f)
		};
		for (int n = 0; n < 6; n++)
		{
			Vector vPoint; U::Math.VectorTransform(vPoints[n], transform, vPoint);

			Vector2D vScreenPos;
			if (!H::Draw.WorldPosToScreenPos(vPoint, vScreenPos))
				return false;

			flLeft = n ? std::min(flLeft, vScreenPos.x) : vScreenPos.x;
			flRight = n ? std::max(flRight, vScreenPos.x) : vScreenPos.x;
			flTop = n ? std::max(flTop, vScreenPos.y) : vScreenPos.y;
			flBottom = n ? std::min(flBottom, vScreenPos.y) : vScreenPos.y;
		}

		if (pLeft) *pLeft = flLeft;
		if (pRight) *pRight = flRight;
		if (pTop) *pTop = flTop;
		if (pBottom) *pBottom = flBottom;

		return !(flRight < 0 || flLeft > H::Draw.m_nScreenW || flTop < 0 || flBottom > H::Draw.m_nScreenH);
	}

	inline Color_t GetEntityColor(C_CSPlayer* pLocal, C_BaseEntity* pEntity, bool bRelativeColors)
	{
		if (!pLocal || !pEntity)
			return { 245, 229, 196, 255 };

		if (pEntity->GetClassID() == ECSClientClass::CCSPlayer)
		{
			auto pPlayer = pEntity->As<C_CSPlayer>();

			if (pPlayer != pLocal)
			{
				PlayerManager::PlayerInfo custom_info{};

				PlayerManager::GetInfo(pPlayer->entindex(), custom_info);

				if (custom_info.m_bIgnored)
				{
					return CFG::Color_Friend;
				}

				if (custom_info.m_bCheater)
				{
					return CFG::Color_Cheater;
				}

				if (custom_info.m_bRetardLegit)
				{
					return CFG::Color_RetardLegit;
				}
			}
		}

		if (bRelativeColors)
			return pLocal->m_iTeamNum() == pEntity->m_iTeamNum() ? CFG::Colors_Team : CFG::Colors_Enemy;
		else
		{
			switch (pEntity->m_iTeamNum())
			{
				case 2: return CFG::Colors_TeamT;
				case 3: return CFG::Colors_TeamCT;
			}
		}

		return { 245, 229, 196, 255 };
	}

	inline std::wstring ConvertUtf8ToWide(const std::string& ansi)
	{
		const int size = MultiByteToWideChar(CP_UTF8, 0, ansi.c_str(), -1, nullptr, 0);
		std::wstring result(size, L'\0');
		MultiByteToWideChar(CP_UTF8, 0, ansi.c_str(), -1, result.data(), size);
		return result;
	}

	inline std::string ConvertWideToUtf8(const std::wstring& unicode)
	{
		const int size = WideCharToMultiByte(CP_UTF8, 0, unicode.c_str(), -1, nullptr, 0, nullptr, nullptr);
		std::string result(size, '\0');
		WideCharToMultiByte(CP_UTF8, 0, unicode.c_str(), -1, result.data(), size, nullptr, nullptr);
		return result;
	}

	inline std::unique_ptr<char[]> ConvertToUppercase(const std::wstring& wstr)
	{
		size_t length = wstr.length();
		auto upper_wstr = std::make_unique<wchar_t[]>(length + 1);
		wcscpy_s(upper_wstr.get(), length + 1, wstr.c_str());

		for (size_t i = 0; i < length; ++i) {
			upper_wstr[i] = std::toupper(upper_wstr[i]);
		}

		std::string utf8_str = ConvertWideToUtf8(upper_wstr.get());
		auto result = std::make_unique<char[]>(utf8_str.size() + 1);
		strcpy_s(result.get(), utf8_str.size() + 1, utf8_str.c_str());

		return result;
	}
}