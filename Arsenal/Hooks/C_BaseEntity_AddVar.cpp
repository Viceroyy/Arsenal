#include "../SDK/SDK.h"

class IInterpolatedVar
{
public:
	virtual ~IInterpolatedVar() = 0;
	virtual void Setup(void* pValue, int type) = 0;
	virtual void SetInterpolationAmount(float seconds) = 0;
	virtual void NoteLastNetworkedValue() = 0;
	virtual bool NoteChanged(float changetime, bool bUpdateLastNetworkedValue) = 0;
	virtual void Reset() = 0;
	virtual int Interpolate(float currentTime) = 0;
	virtual int  GetType() const = 0;
	virtual void RestoreToLastNetworked() = 0;
	virtual void Copy(IInterpolatedVar* pSrc) = 0;
	virtual const char* GetDebugName() = 0;
	virtual void SetDebugName(const char* pName) = 0;
	virtual void SetDebug(bool bDebug) = 0;
};

MAKE_SIGNATURE(C_BaseEntity_AddVar, "client.dll", "55 8B EC 83 EC ? 53 8B D9 56 33 F6", 0x0);

MAKE_HOOK(C_BaseEntity_AddVar, S::C_BaseEntity_AddVar(), void, __fastcall, 
	void* ecx, void* edx, void* data, IInterpolatedVar* watcher, int type, bool bSetup)
{
	if (CFG::Visuals_NoInterpolation && watcher)
	{
		auto hash = FNV1A::Hash(watcher->GetDebugName());

		static const auto m_iv_vecVelocity = FNV1A::HashConst("C_BaseEntity::m_iv_vecVelocity");
		static const auto m_iv_angEyeAngles = FNV1A::HashConst("C_CSPlayer::m_iv_angEyeAngles");
		static const auto m_iv_flPoseParameter = FNV1A::HashConst("C_BaseAnimating::m_iv_flPoseParameter");
		static const auto m_iv_flCycle = FNV1A::HashConst("C_BaseAnimating::m_iv_flCycle");
		static const auto m_iv_flMaxGroundSpeed = FNV1A::HashConst("CMultiPlayerAnimState::m_iv_flMaxGroundSpeed");

		if (hash == m_iv_vecVelocity
			|| hash == m_iv_flPoseParameter
			|| hash == m_iv_flCycle
			|| hash == m_iv_flMaxGroundSpeed)
			return;

		if (ecx != H::EntityCache.GetLocal())
		{
			if (hash == m_iv_angEyeAngles)
				return;
		}
	}

	CALL_ORIGINAL(ecx, edx, data, watcher, type, bSetup);
}