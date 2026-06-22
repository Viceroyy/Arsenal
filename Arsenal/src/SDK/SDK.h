#pragma once

#include "Vars.h"
#include "Globals.h"
#include "Helpers/ConVars/ConVars.h"
#include "Helpers/Entities/Entities.h"
#include "Helpers/Draw/Draw.h"
#include "Helpers/TraceFilters/TraceFilters.h"
#include "Definitions/Types.h"
#include "Definitions/Interfaces.h"
#include "Definitions/Classes.h"
#include "../Utils/Signatures/Signatures.h"
#include "../Utils/Interfaces/Interfaces.h"
#include "../Utils/Hooks/Hooks.h"
#include "../Utils/Memory/Memory.h"
#include "../Utils/KeyHandler/KeyHandler.h"
#include "../Utils/Hash/Hash.h"
#include "../Utils/Math/Math.h"
#include "../Utils/Timer/Timer.h"
#include "../Utils/Macros/Macros.h"
#include <intrin.h>
#include <random>

#define DEFAULT_COLOR		Color_t(175, 150, 255, 255)
#define ALTERNATE_COLOR		Color_t(175, 150, 255, 127)

#define VK_0              0x30
#define VK_1              0x31
#define VK_2              0x32
#define VK_3              0x33
#define VK_4              0x34
#define VK_5              0x35
#define VK_6              0x36
#define VK_7              0x37
#define VK_8              0x38
#define VK_9              0x39
#define VK_A              0x41
#define VK_B              0x42
#define VK_C              0x43
#define VK_D              0x44
#define VK_E              0x45
#define VK_F              0x46
#define VK_G              0x47
#define VK_H              0x48
#define VK_I              0x49
#define VK_J              0x4A
#define VK_K              0x4B
#define VK_L              0x4C
#define VK_M              0x4D
#define VK_N              0x4E
#define VK_O              0x4F
#define VK_P              0x50
#define VK_Q              0x51
#define VK_R              0x52
#define VK_S              0x53
#define VK_T              0x54
#define VK_U              0x55
#define VK_V              0x56
#define VK_W              0x57
#define VK_X              0x58
#define VK_Y              0x59
#define VK_Z              0x5A

#define OUTPUT_CONSOLE 1 << 0
#define OUTPUT_DEBUG 1 << 1
#define OUTPUT_TOAST 1 << 2
#define OUTPUT_MENU 1 << 3
#define OUTPUT_CHAT 1 << 4
#define OUTPUT_PARTY 1 << 5

MAKE_SIGNATURE(WeaponIDToAlias, "client.dll", "4C 8D 05 ? ? ? ? 33 D2 49 8B C0", 0x0);

namespace SDK
{
	void Output(const char* sFunction, const char* sLog = nullptr, Color_t tColor = { 255, 255, 255, 255 },
		int iTo = OUTPUT_CONSOLE, int iMessageBox = -1,
		const char* sLeft = "[", const char* sRight = "]");

	bool IsGameWindowInFocus();

	inline bool IsLoopback()
	{
		auto pNetChan = I::EngineClient->GetNetChannelInfo();
		return pNetChan && pNetChan->IsLoopback();
	}

	inline void RandomSeed(int iSeed)
	{
		static auto RandomSeed = reinterpret_cast<void(*)(uint32_t)>(GetProcAddress(GetModuleHandleA("vstdlib.dll"), "RandomSeed"));
		RandomSeed(iSeed);
	}

	inline int RandomInt(int iMinVal, int iMaxVal)
	{
		static auto RandomInt = reinterpret_cast<int(*)(int, int)>(GetProcAddress(GetModuleHandleA("vstdlib.dll"), "RandomInt"));
		return RandomInt(iMinVal, iMaxVal);
	}

	inline float RandomFloat(float flMinVal, float flMaxVal)
	{
		static auto RandomFloat = reinterpret_cast<float(*)(float, float)>(GetProcAddress(GetModuleHandleA("vstdlib.dll"), "RandomFloat"));
		return RandomFloat(flMinVal, flMaxVal);
	}

	inline double PlatFloatTime()
	{
		static auto Plat_FloatTime = reinterpret_cast<double(*)()>(GetProcAddress(GetModuleHandleA("tier0.dll"), "Plat_FloatTime"));
		return Plat_FloatTime();
	}

	inline bool IsZoomed()
	{
		return false;
	}

	inline void Trace(const Vector& vecStart, const Vector& vecEnd, unsigned int nMask, ITraceFilter* pFilter, CGameTrace* pTrace)
	{
		Ray_t ray;
		ray.Init(vecStart, vecEnd);
		I::EngineTrace->TraceRay(ray, nMask, pFilter, pTrace);
	}

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

	inline static int RandInt(int min, int max)
	{
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<> distr(min, max);
		return distr(gen);
	}

	inline bool IsOnScreen(CBaseEntity* pEntity, const matrix3x4& transform, float* pLeft = nullptr, float* pRight = nullptr, float* pTop = nullptr, float* pBottom = nullptr)
	{
		ICollideable* coll = pEntity->GetCollideable();

		Vector vMins = coll->OBBMins();
		Vector vMaxs = coll->OBBMaxs();

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

	inline bool IsOnScreen(CBaseEntity* pLocal, CBaseEntity* pEntity)
	{
		Vector vPos = pEntity->GetAbsOrigin();

		if (vPos.DistTo(pLocal->GetAbsOrigin()) > 300.0f)
		{
			Vector2D vScreen = {};

			if (H::Draw.WorldPosToScreenPos(vPos, vScreen))
			{
				if (vScreen.x < -400
					|| vScreen.x > H::Draw.m_nScreenW + 400
					|| vScreen.y < -400
					|| vScreen.y > H::Draw.m_nScreenH + 400)
					return false;
			}

			else return false;
		}

		return true;
	}

	inline bool IsOnScreen(CBaseEntity* pEntity, Vector vOrigin)
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
			Vector vPoint = vOrigin + vPoints[n];

			Vector2D vScreenPos;
			if (!H::Draw.WorldPosToScreenPos(vPoint, vScreenPos))
				return false;

			flLeft = n ? std::min(flLeft, vScreenPos.x) : vScreenPos.x;
			flRight = n ? std::max(flRight, vScreenPos.x) : vScreenPos.x;
			flTop = n ? std::max(flTop, vScreenPos.y) : vScreenPos.y;
			flBottom = n ? std::min(flBottom, vScreenPos.y) : vScreenPos.y;
		}

		return !(flRight < 0 || flLeft > H::Draw.m_nScreenW || flTop < 0 || flBottom > H::Draw.m_nScreenH);
	}

	inline bool IsOnScreen(CBaseEntity* pEntity)
	{
		return IsOnScreen(pEntity, pEntity->RenderableToWorldTransform());
	}

	inline Color_t GetEntityColor(CCSPlayer* pLocal, CBaseEntity* pEntity, bool bRelativeColors)
	{
		if (!pLocal || !pEntity)
			return { 245, 229, 196, 255 };

		if (pEntity->GetClassID() == ECSClassID::CCSPlayer)
		{
			const auto pPlayer = pEntity->As<CCSPlayer>();

			if (pPlayer != pLocal && pPlayer->HasPlayerAsFriend())
				return CFG::Color_Friend;

			if (pPlayer != pLocal)
			{
				PlayerPriority info{};
				F::Players.GetInfo(pPlayer->entindex(), info);

				if (info.Cheater)
				{
					return CFG::Color_Cheater;
				}

				if (info.RetardLegit)
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

	inline std::wstring ConvertUtf8ToWide(const std::string& source)
	{
		int size = MultiByteToWideChar(CP_UTF8, 0, source.data(), -1, nullptr, 0);
		std::wstring result(size, 0);
		MultiByteToWideChar(CP_UTF8, 0, source.data(), -1, result.data(), size);
		return result;
	}

	inline std::string ConvertWideToUtf8(const std::wstring& source)
	{
		int size = WideCharToMultiByte(CP_UTF8, 0, source.data(), -1, nullptr, 0, nullptr, nullptr);
		std::string result(size, 0);
		WideCharToMultiByte(CP_UTF8, 0, source.data(), -1, result.data(), size, nullptr, nullptr);
		return result;
	}

	inline std::wstring GetWeaponName(int wpnid)
	{
		static auto WeaponIDToAlias = reinterpret_cast<const char* (*)(int)>(S::WeaponIDToAlias());

		if (!WeaponIDToAlias(wpnid))
			return L"unknown";

		auto wstr = ConvertUtf8ToWide(WeaponIDToAlias(wpnid));
		return wstr;
	}
}

struct ShaderStencilState_t
{
	bool m_bEnable = false;
	StencilOperation_t m_FailOp = {};
	StencilOperation_t m_ZFailOp = {};
	StencilOperation_t m_PassOp = {};
	StencilComparisonFunction_t m_CompareFunc = {};
	int m_nReferenceValue = 0;
	uint32_t m_nTestMask = 0;
	uint32_t m_nWriteMask = 0;

	ShaderStencilState_t()
	{
		m_bEnable = false;
		m_PassOp = m_FailOp = m_ZFailOp = STENCILOPERATION_KEEP;
		m_CompareFunc = STENCILCOMPARISONFUNCTION_ALWAYS;
		m_nReferenceValue = 0;
		m_nTestMask = m_nWriteMask = 0xFFFFFFFF;
	}

	void SetStencilState(IMatRenderContext* pRenderContext)
	{
		pRenderContext->SetStencilEnable(m_bEnable);
		pRenderContext->SetStencilFailOperation(m_FailOp);
		pRenderContext->SetStencilZFailOperation(m_ZFailOp);
		pRenderContext->SetStencilPassOperation(m_PassOp);
		pRenderContext->SetStencilCompareFunction(m_CompareFunc);
		pRenderContext->SetStencilReferenceValue(m_nReferenceValue);
		pRenderContext->SetStencilTestMask(m_nTestMask);
		pRenderContext->SetStencilWriteMask(m_nWriteMask);
	}
};

class CTraceFilterWorldCustom : public CTraceFilter
{
public:
	virtual bool ShouldHitEntity(IHandleEntity* pServerEntity, int contentsMask)
	{
		if (auto pEntity = static_cast<IClientEntity*>(pServerEntity)->As<CBaseEntity>())
		{
			switch (pEntity->GetClassID())
			{
			case ECSClassID::CCSPlayer: return pEntity == m_pTarget;

			case ECSClassID::CBaseDoor:
			case ECSClassID::CPhysicsProp:
			case ECSClassID::CDynamicProp:
			case ECSClassID::CBaseEntity:
			case ECSClassID::CFuncTrackTrain: return true;

			default: return false;
			}
		}

		return false;
	}
	virtual TraceType_t GetTraceType() const
	{
		return TRACE_EVERYTHING;
	}

public:
	CBaseEntity* m_pTarget = nullptr;
};

class CTraceFilterHitscan : public ITraceFilter
{
public:
	bool ShouldHitEntity(IHandleEntity* pServerEntity, int nContentsMask)
	{
		{
			if (!pServerEntity || pServerEntity == pSkip)
				return false;

			auto pEntity = static_cast<IClientEntity*>(pServerEntity)->As<CBaseEntity>();
			auto pLocal = H::Entities.GetLocal();
			auto pWeapon = H::Entities.GetWeapon();

			const int iTargetTeam = pEntity->m_iTeamNum(), iLocalTeam = pLocal ? pLocal->m_iTeamNum() : iTargetTeam;

			switch (pEntity->GetClassID())
			{
			case ECSClassID::CFuncAreaPortalWindow: return false;
			case ECSClassID::CCSPlayer:
				if (iTargetTeam == iLocalTeam)
					return false;
			}

			return true;
		}
	}
	TraceType_t GetTraceType() const
	{
		return TRACE_EVERYTHING;
	}
	CBaseEntity* pSkip = nullptr;
};
