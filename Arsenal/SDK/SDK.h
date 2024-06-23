#pragma once

#include "../Util/Color/Color.h"
#include "EntityCacher/EntityCacher.h"
#include "../Features/CFG.h"
#include "DrawManager/DrawManager.h"

#include <random>
#include "../Util/Icons.h"

namespace Util
{
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

	inline Color_t GetHealthColor(const int nHealth, const int nMaxHealth)
	{
		if (nHealth > nMaxHealth)
			return { 15, 150, 150, 255 };

		const int nHP = U::Math.Max(0, U::Math.Min(nHealth, nMaxHealth));

		const unsigned int nR = static_cast<unsigned int>(U::Math.Min((510 * (nMaxHealth - nHP)) / nMaxHealth, 200));
		const unsigned int nG = static_cast<unsigned int>(U::Math.Min((510 * nHP) / nMaxHealth, 200));

		return { static_cast<byte>(nR), static_cast<byte>(nG), 0, 255 };
	}

	inline Color_t GetTeamColor(const int iTargetTeam, const int iLocalTeam, bool bRelativeColors)
	{
		if (bRelativeColors)
			return iLocalTeam == iTargetTeam ? CFG::Colors_Team : CFG::Colors_Enemy;
		else
		{
			switch (iTargetTeam)
			{
			case 2: return CFG::Colors_TeamT;
			case 3: return CFG::Colors_TeamCT;
			}
		}

		return Color_t(245, 229, 196, 255);
	}

	inline std::wstring ConvertUtf8ToWide(const std::string& ansi)
	{
		const int size = MultiByteToWideChar(CP_UTF8, 0, ansi.c_str(), -1, nullptr, 0);
		std::wstring result(size, L'\0');
		MultiByteToWideChar(CP_UTF8, 0, ansi.c_str(), -1, result.data(), size);
		return result;
	}

	inline void MakeSafeName(const char* oldName, char* newName, int newNameBufSize)
	{
		assert(newNameBufSize >= sizeof(newName[0]));

		int newpos = 0;

		for (const char* p = oldName; *p != 0 && newpos < newNameBufSize - 1; p++)
		{
			//check for a '#' char at the beginning
			if (p == oldName && *p == '#')
			{
				newName[newpos] = '*';
				newpos++;
			}
			else if (*p == '%')
			{
				// remove % chars
				newName[newpos] = '*';
				newpos++;
			}
			else if (*p == '&')
			{
				//insert another & after this one
				if (newpos + 2 < newNameBufSize)
				{
					newName[newpos] = '&';
					newName[newpos + 1] = '&';
					newpos += 2;
				}
			}
			else
			{
				newName[newpos] = *p;
				newpos++;
			}
		}
		newName[newpos] = 0;
	}

	inline const char* SafeName(const char* oldName)
	{
		static char safeName[MAX_PLAYER_NAME_LENGTH * 2 + 1];
		MakeSafeName(oldName, safeName, sizeof(safeName));

		return safeName;
	}
}