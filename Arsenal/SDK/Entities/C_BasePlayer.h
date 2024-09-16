#pragma once
#include "C_BaseCombatCharacter.h"

MAKE_SIGNATURE(C_BasePlayer_UsingStandardWeaponsInVehicle, "client.dll", "56 57 8B F9 8B B7 ? ? ? ? 83 FE FF 74 4D", 0x0);
MAKE_SIGNATURE(C_BasePlayer_UpdateButtonState, "client.dll", "55 8B EC 8B 81 ? ? ? ? 8B D0", 0x0);

class CSteamID;
class CUserCmd;
class IRagdoll;
class C_BaseViewModel;
struct stepsoundtimes_t;
struct surfacedata_t;
class CSoundParameters;
class CHintSystem;

enum PLAYER_ANIM
{
	PLAYER_IDLE,
	PLAYER_WALK,
	PLAYER_JUMP,
	PLAYER_SUPERJUMP,
	PLAYER_DIE,
	PLAYER_ATTACK1,
	PLAYER_IN_VEHICLE,
	PLAYER_RELOAD,
	PLAYER_START_AIMING,
	PLAYER_LEAVE_AIMING,
};

class C_BasePlayer : public C_BaseCombatCharacter, public CGameEventListener
{
public:
	virtual ~C_BasePlayer() = 0;

	virtual void				SharedSpawn() = 0;
	virtual bool				GetSteamID(CSteamID* pID) = 0;
	virtual float				GetPlayerMaxSpeed() = 0;
	virtual void				CalcView(Vector& eyeOrigin, Vector& eyeAngles, float& zNear, float& zFar, float& fov) = 0;
	virtual void				CalcViewModelView(const Vector& eyeOrigin, const Vector& eyeAngles) = 0;
	virtual float				CalcRoll(const Vector& angles, const Vector& velocity, float rollangle, float rollspeed) = 0;
	virtual void				SetPlayerUnderwater(bool state) = 0;
	virtual Vector				Weapon_ShootPosition() = 0;
	virtual void				Weapon_DropPrimary(void) = 0;
	virtual Vector				GetAutoaimVector(float flScale) = 0;
	virtual bool				CreateMove(float flInputSampleTime, CUserCmd* pCmd) = 0;
	virtual void				AvoidPhysicsProps(CUserCmd* pCmd) = 0;
	virtual void				PlayerUse(void) = 0;
	virtual bool				IsUseableEntity(C_BaseEntity* pEntity, unsigned int requiredCaps) = 0;
	virtual int					GetObserverMode() const = 0;
	virtual C_BaseEntity*		GetObserverTarget() const = 0;
	virtual IRagdoll*			GetRepresentativeRagdoll() const = 0;
	virtual void				TeamChange(int iNewTeam) = 0;
	virtual bool				IsAllowedToSwitchWeapons(void) = 0;
	virtual C_BaseCombatWeapon* GetActiveWeaponForSelection(void) = 0;
	virtual C_BaseAnimating*	GetRenderedWeaponModel() = 0;
	virtual bool				IsOverridingViewmodel(void) = 0;
	virtual int					DrawOverriddenViewmodel(C_BaseViewModel* pViewmodel, int flags) = 0;
	virtual float				GetDefaultAnimSpeed(void) = 0;
	virtual void				ThirdPersonSwitch(bool bThirdperson) = 0;
	virtual bool				CanSetSoundMixer(void) = 0;
	virtual int					GetVisionFilterFlags(bool bWeaponsCheck = false) = 0;
	virtual void				CalculateVisionUsingCurrentFlags(void) = 0;
	virtual bool				ViewModel_IsTransparent(void) = 0;
	virtual bool				ViewModel_IsUsingFBTexture(void) = 0;
	virtual void				PreThink(void) = 0;
	virtual void				PostThink(void) = 0;
	virtual void				ItemPreFrame(void) = 0;
	virtual void				ItemPostFrame(void) = 0;
	virtual void				AbortReload(void) = 0;
	virtual void				SelectLastItem(void) = 0;
	virtual void				Weapon_SetLast(C_BaseCombatWeapon* pWeapon) = 0;
	virtual bool				Weapon_ShouldSetLast(C_BaseCombatWeapon* pOldWeapon, C_BaseCombatWeapon* pNewWeapon) = 0;
	virtual bool				Weapon_ShouldSelectItem(C_BaseCombatWeapon* pWeapon) = 0;
	virtual C_BaseCombatWeapon* GetLastWeapon(void) = 0;
	virtual void 				SelectItem(const char* pstr, int iSubType = 0) = 0;
	virtual void				UpdateClientData(void) = 0;
	virtual float				GetFOV(void) = 0;
	virtual bool				IsZoomed(void) = 0;
	virtual void				ViewPunch(const Vector& angleOffset) = 0;
	virtual void				OverrideView(CViewSetup* pSetup) = 0;
	virtual const Vector		GetPlayerMins(void) const = 0;
	virtual const Vector		GetPlayerMaxs(void) const = 0;
	virtual void				SetVehicleRole(int nRole) = 0;
	virtual void				SetAnimation(PLAYER_ANIM playerAnim) = 0;
	virtual float				GetMinFOV() const = 0;
	virtual void				PlayPlayerJingle() = 0;
	virtual void				UpdateStepSound(surfacedata_t* psurface, const Vector& vecOrigin, const Vector& vecVelocity) = 0;
	virtual void				PlayStepSound(Vector& vecOrigin, surfacedata_t* psurface, float fvol, bool force) = 0;
	virtual surfacedata_t*		GetFootstepSurface(const Vector& origin, const char* surfaceName) = 0;
	virtual void				GetStepSoundVelocities(float* velwalk, float* velrun) = 0;
	virtual void				SetStepSoundTime(stepsoundtimes_t iStepSoundTime, bool bWalking) = 0;
	virtual const char*			GetOverrideStepSound(const char* pszBaseStepSoundName) = 0;
	virtual void				OnEmitFootstepSound(const CSoundParameters& params, const Vector& vecOrigin, float fVolume) = 0;
	virtual void				ExitLadder() = 0;
	virtual CHintSystem*		Hints(void) = 0;
	virtual	IMaterial*			GetHeadLabelMaterial(void) = 0;
	virtual void				OnAchievementAchieved(int iAchievement) = 0;
	virtual bool				CanUseFirstPersonCommand(void) = 0;
	virtual void				CalcObserverView(Vector& eyeOrigin, Vector& eyeAngles, float& fov) = 0;
	virtual Vector				GetChaseCamViewOffset(C_BaseEntity* target) = 0;
	virtual void				CalcInEyeCamView(Vector& eyeOrigin, Vector& eyeAngles, float& fov) = 0;
	virtual float				GetDeathCamInterpolationTime() = 0;
	virtual void				CalcDeathCamView(Vector& eyeOrigin, Vector& eyeAngles, float& fov) = 0;
	virtual void				CalcFreezeCamView(Vector& eyeOrigin, Vector& eyeAngles, float& fov) = 0;
	virtual void				SetLocalViewAngles(const Vector& viewAngles) = 0;
	virtual void				SetViewAngles(const Vector& ang) = 0;
	virtual bool				IsDucked(void) const = 0;
	virtual bool				IsDucking(void) const = 0;
	virtual float				GetFallVelocity(void) = 0;

public:
	NETVAR(m_Local, void*, "CBasePlayer", "m_Local");
	NETVAR(m_chAreaBits, void*, "CBasePlayer", "m_chAreaBits");
	NETVAR(m_chAreaPortalBits, void*, "CBasePlayer", "m_chAreaPortalBits");
	NETVAR(m_iHideHUD, int, "CBasePlayer", "m_iHideHUD");
	NETVAR(m_flFOVRate, float, "CBasePlayer", "m_flFOVRate");
	NETVAR(m_bDucked, bool, "CBasePlayer", "m_bDucked");
	NETVAR(m_bDucking, bool, "CBasePlayer", "m_bDucking");
	NETVAR(m_bInDuckJump, bool, "CBasePlayer", "m_bInDuckJump");
	NETVAR(m_flDucktime, float, "CBasePlayer", "m_flDucktime");
	NETVAR(m_flDuckJumpTime, float, "CBasePlayer", "m_flDuckJumpTime");
	NETVAR(m_flJumpTime, float, "CBasePlayer", "m_flJumpTime");
	NETVAR(m_flFallVelocity, float, "CBasePlayer", "m_flFallVelocity");
	NETVAR(m_vecPunchAngle, Vector, "CBasePlayer", "m_vecPunchAngle");
	NETVAR(m_vecPunchAngleVel, Vector, "CBasePlayer", "m_vecPunchAngleVel");
	NETVAR(m_bDrawViewmodel, bool, "CBasePlayer", "m_bDrawViewmodel");
	NETVAR(m_bWearingSuit, bool, "CBasePlayer", "m_bWearingSuit");
	NETVAR(m_bPoisoned, bool, "CBasePlayer", "m_bPoisoned");
	NETVAR(m_flStepSize, float, "CBasePlayer", "m_flStepSize");
	NETVAR(m_bAllowAutoMovement, bool, "CBasePlayer", "m_bAllowAutoMovement");
	NETVAR(m_vecViewOffset, Vector, "CBasePlayer", "m_vecViewOffset[0]");
	NETVAR(m_flFriction, float, "CBasePlayer", "m_flFriction");
	NETVAR(m_iAmmo, void*, "CBasePlayer", "m_iAmmo");
	NETVAR(m_fOnTarget, int, "CBasePlayer", "m_fOnTarget");
	NETVAR(m_nTickBase, int, "CBasePlayer", "m_nTickBase");
	NETVAR(m_nNextThinkTick, int, "CBasePlayer", "m_nNextThinkTick");
	NETVAR(m_hLastWeapon, EHANDLE, "CBasePlayer", "m_hLastWeapon");
	NETVAR(m_hGroundEntity, EHANDLE, "CBasePlayer", "m_hGroundEntity");
	NETVAR(m_vecVelocity, Vector, "CBasePlayer", "m_vecVelocity[0]");
	NETVAR(m_vecBaseVelocity, Vector, "CBasePlayer", "m_vecBaseVelocity");
	NETVAR(m_hConstraintEntity, EHANDLE, "CBasePlayer", "m_hConstraintEntity");
	NETVAR(m_vecConstraintCenter, Vector, "CBasePlayer", "m_vecConstraintCenter");
	NETVAR(m_flConstraintRadius, float, "CBasePlayer", "m_flConstraintRadius");
	NETVAR(m_flConstraintWidth, float, "CBasePlayer", "m_flConstraintWidth");
	NETVAR(m_flConstraintSpeedFactor, float, "CBasePlayer", "m_flConstraintSpeedFactor");
	NETVAR(m_flDeathTime, float, "CBasePlayer", "m_flDeathTime");
	NETVAR(m_nWaterLevel, byte, "CBasePlayer", "m_nWaterLevel");
	NETVAR(m_flLaggedMovementValue, float, "CBasePlayer", "m_flLaggedMovementValue");
	NETVAR(pl, void*, "CBasePlayer", "pl");
	NETVAR(deadflag, int, "CBasePlayer", "deadflag");
	NETVAR(m_iFOV, int, "CBasePlayer", "m_iFOV");
	NETVAR(m_iFOVStart, int, "CBasePlayer", "m_iFOVStart");
	NETVAR(m_flFOVTime, float, "CBasePlayer", "m_flFOVTime");
	NETVAR(m_iDefaultFOV, int, "CBasePlayer", "m_iDefaultFOV");
	NETVAR(m_hZoomOwner, EHANDLE, "CBasePlayer", "m_hZoomOwner");
	NETVAR(m_hVehicle, EHANDLE, "CBasePlayer", "m_hVehicle");
	NETVAR(m_hUseEntity, EHANDLE, "CBasePlayer", "m_hUseEntity");
	NETVAR(m_iHealth, int, "CBasePlayer", "m_iHealth");
	NETVAR(m_lifeState, int, "CBasePlayer", "m_lifeState");
	NETVAR(m_iBonusProgress, int, "CBasePlayer", "m_iBonusProgress");
	NETVAR(m_iBonusChallenge, int, "CBasePlayer", "m_iBonusChallenge");
	NETVAR(m_flMaxspeed, float, "CBasePlayer", "m_flMaxspeed");
	NETVAR(m_fFlags, int, "CBasePlayer", "m_fFlags");
	NETVAR(m_iObserverMode, int, "CBasePlayer", "m_iObserverMode");
	NETVAR(m_hObserverTarget, EHANDLE, "CBasePlayer", "m_hObserverTarget");
	NETVAR(m_hViewModel, EHANDLE, "CBasePlayer", "m_hViewModel[0]");
	NETVAR(m_szLastPlaceName, const char*, "CBasePlayer", "m_szLastPlaceName");

	NETVAR_OFF(m_pCurrentCommand, CUserCmd*, "CBasePlayer", "m_hConstraintEntity", -4);
	NETVAR_OFF(m_nButtons, int, "CBasePlayer", "m_hConstraintEntity", -8);
	NETVAR_OFF(m_afButtonLast, int, "CBasePlayer", "m_hConstraintEntity", -20);

public:

	inline bool UsingStandardWeaponsInVehicle()
	{
		return reinterpret_cast<bool(__thiscall*)(void*)>(S::C_BasePlayer_UsingStandardWeaponsInVehicle())(this);
	}

	inline void UpdateButtonState(int nUserCmdButtonMask)
	{
		reinterpret_cast<void(__thiscall*)(void*, int)>(S::C_BasePlayer_UpdateButtonState())(this, nUserCmdButtonMask);
	}

	inline int& m_nImpulse()
	{
		return *reinterpret_cast<int*>(reinterpret_cast<DWORD>(this) + 0x10C4);
	}

	inline MoveType_t& m_MoveType()
	{
		return *reinterpret_cast<MoveType_t*>(reinterpret_cast<DWORD>(this) + 0x178);
	}

	bool IsSwimming()
	{
		return m_nWaterLevel() > WL_Feet;
	}

	bool OnSolid()
	{
		return m_hGroundEntity() || (m_fFlags() & FL_ONGROUND);
	}

	Vector GetShootPos()
	{
		return (m_vecOrigin() + m_vecViewOffset());
	}

	Vector GetEyePosition()
	{
		return (GetAbsOrigin() + m_vecViewOffset());
	}
};

namespace Util
{
	inline C_BasePlayer* PlayerByIndex(const int entindex)
	{
		IClientEntity* pEntity = Util::EntityByIndex(entindex);

		return pEntity ? pEntity->As<C_BasePlayer>() : nullptr;
	}
}