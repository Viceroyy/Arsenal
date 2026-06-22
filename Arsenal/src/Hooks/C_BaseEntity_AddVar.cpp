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

MAKE_SIGNATURE(CBaseEntity_AddVar, "client.dll", "48 89 5C 24 ? 48 89 6C 24 ? 57 41 56 41 57 48 83 EC ? 33 DB 48 89 74 24", 0x0);

MAKE_HOOK(CBaseEntity_AddVar, S::CBaseEntity_AddVar(), void,
	C_BaseEntity* ecx, void* data, IInterpolatedVar* watcher, int type, bool bSetup)
{
	if (CFG::Visuals_NoInterpolation && watcher)
	{
		const auto hash = FNV1A::Hash32(watcher->GetDebugName());

		static constexpr auto m_iv_vecVelocity = FNV1A::Hash32Const("C_BaseEntity::m_iv_vecVelocity");
		static constexpr auto m_iv_angEyeAngles = FNV1A::Hash32Const("C_CSPlayer::m_iv_angEyeAngles");
		static constexpr auto m_iv_flPoseParameter = FNV1A::Hash32Const("C_BaseAnimating::m_iv_flPoseParameter");
		static constexpr auto m_iv_flCycle = FNV1A::Hash32Const("C_BaseAnimating::m_iv_flCycle");
		static constexpr auto m_iv_flMaxGroundSpeed = FNV1A::Hash32Const("CMultiPlayerAnimState::m_iv_flMaxGroundSpeed");

		if (hash == m_iv_vecVelocity
			|| hash == m_iv_flPoseParameter
			|| hash == m_iv_flCycle
			|| hash == m_iv_flMaxGroundSpeed)
			return;

		if (ecx != H::Entities.GetLocal())
		{
			if (hash == m_iv_angEyeAngles)
				return;
		}
	}

	CALL_ORIGINAL(ecx, data, watcher, type, bSetup);
}