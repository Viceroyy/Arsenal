#pragma once
#include "C_BaseAnimating.h"

class Activity;
struct WeaponProficiency_t;
struct WeaponProficiencyInfo_t;
class CHudTexture;
class C_BasePlayer;
class C_BaseViewModel;

typedef struct
{
	int			baseAct;
	int			weaponAct;
	bool		required;
} acttable_t;

struct poseparamtable_t
{
	const char* pszName;
	float		flValue;
};

#define MAX_WEAPON_STRING	80
#define MAX_WEAPON_PREFIX	16
#define MAX_WEAPON_AMMO_NAME		32

typedef enum {
	EMPTY,
	SINGLE,
	SINGLE_NPC,
	WPN_DOUBLE, // Can't be "DOUBLE" because windows.h uses it.
	DOUBLE_NPC,
	BURST,
	RELOAD,
	RELOAD_NPC,
	MELEE_MISS,
	MELEE_HIT,
	MELEE_HIT_WORLD,
	SPECIAL1,
	SPECIAL2,
	SPECIAL3,
	TAUNT,
	DEPLOY,

	// Add new shoot sound types here

	NUM_SHOOT_SOUND_TYPES,
} WeaponSound_t;

#pragma pack (push, 1)

class CCSWeaponInfo
{
public:
	virtual void Parse(KeyValues*, const char*) = 0;

	bool bParsedScript; //0x0004
	bool bLoadedHudElements; //0x0005
	char szClassName[MAX_WEAPON_STRING]; //0x0006
	char szPrintName[MAX_WEAPON_STRING]; //0x0056
	char szViewModel[MAX_WEAPON_STRING]; //0x00A6
	char szWorldModel[MAX_WEAPON_STRING]; //0x00F6
	char szAnimationPrefix[MAX_WEAPON_PREFIX]; //0x0146
	char pad_0156[2]; //0x0156
	int32_t iSlot; //0x0158
	int32_t iPosition; //0x015C
	int32_t iMaxClip1; //0x0160
	int32_t iMaxClip2; //0x0164
	int32_t iDefaultClip1; //0x0168
	int32_t iDefaultClip2; //0x016C
	int32_t iWeight; //0x0170
	int32_t iRumbleEffect; //0x0174
	bool bAutoSwitchTo; //0x0178
	bool bAutoSwitchFrom; //0x0179
	char pad_017A[2]; //0x017A
	int32_t iFlags; //0x017C
	char szAmmo1[MAX_WEAPON_AMMO_NAME]; //0x0180
	char szAmmo2[MAX_WEAPON_AMMO_NAME]; //0x01A0
	char aShootSounds[NUM_SHOOT_SOUND_TYPES][MAX_WEAPON_STRING]; //0x01C0
	int32_t iAmmoType; //0x06C0
	int32_t iAmmo2Type; //0x06C4
	bool m_bMeleeWeapon; //0x06C8
	bool m_bBuiltRightHanded; //0x06C9
	bool m_bAllowFlipping; //0x06CA
	char pad_06CB[1]; //0x06CB
	int32_t iSpriteCount; //0x06CC
	class CHudTexture* iconActive; //0x06D0
	class CHudTexture* iconInactive; //0x06D4
	class CHudTexture* iconAmmo; //0x06D8
	class CHudTexture* iconAmmo2; //0x06DC
	class CHudTexture* iconCrosshair; //0x06E0
	class CHudTexture* iconAutoaim; //0x06E4
	class CHudTexture* iconZoomedCrosshair; //0x06E8
	class CHudTexture* iconZoomedAutoaim; //0x06EC
	class CHudTexture* iconSmall; //0x06F0
	bool bShowUsageHint; //0x06F4
	char pad_06F5[3]; //0x06F5
	float m_flMaxSpeed; //0x06F8
	int32_t m_WeaponType; //0x06FC
	bool bFullAuto; //0x0700
	char pad_0701[3]; //0x0701
	int32_t m_iTeam; //0x0704
	float m_flBotAudibleRange; //0x0708
	float m_flArmorRatio; //0x070C
	int32_t m_iCrosshairMinDistance; //0x0710
	int32_t m_iCrosshairDeltaDistance; //0x0714
	bool m_bCanUseWithShield; //0x0718
	char m_WrongTeamMsg[32]; //0x0719
	char m_szAnimExtension[16]; //0x0739
	char m_szShieldViewModel[64]; //0x0749
	char m_szAddonModel[MAX_WEAPON_STRING]; //0x0789
	char m_szDroppedModel[MAX_WEAPON_STRING]; //0x07D9
	char m_szSilencerModel[MAX_WEAPON_STRING]; //0x0829
	char pad_0879[3]; //0x0879
	int32_t m_iMuzzleFlashStyle; //0x087C
	float m_flMuzzleScale; //0x0880
	int32_t m_iPenetration; //0x0884
	int32_t m_iDamage; //0x0888
	float m_flRange; //0x088C
	float m_flRangeModifier; //0x0890
	int32_t m_iBullets; //0x0894
	float m_flCycleTime; //0x0898
	bool m_bAccuracyQuadratic; //0x089C
	char pad_089D[3]; //0x089D
	float m_flAccuracyDivisor; //0x08A0
	float m_flAccuracyOffset; //0x08A4
	float m_flMaxInaccuracy; //0x08A8
	float m_flSpread; //0x08AC
	float m_flSpreadAlt; //0x08B0
	float m_flInaccuracyCrouch; //0x08B4
	float m_flInaccuracyCrouchAlt; //0x08B8
	float m_flInaccuracyStand; //0x08BC
	float m_flInaccuracyStandAlt; //0x08C0
	float m_flInaccuracyJump; //0x08C4
	float m_flInaccuracyJumpAlt; //0x08C8
	float m_flInaccuracyLand; //0x08CC
	float m_flInaccuracyLandAlt; //0x08D0
	float m_flInaccuracyLadder; //0x08D4
	float m_flInaccuracyLadderAlt; //0x08D8
	float m_flInaccuracyFire; //0x08DC
	float m_flInaccuracyFireAlt; //0x08E0
	float m_flInaccuracyMove; //0x08E4
	float m_flInaccuracyMoveAlt; //0x08E8
	float m_flRecoveryTimeStand; //0x08EC
	float m_flRecoveryTimeCrouch; //0x08F0
	float m_flInaccuracyReload; //0x08F4
	float m_flInaccuracyAltSwitch; //0x08F8
	float m_flTimeToIdleAfterFire; //0x08FC
	float m_flIdleInterval; //0x0900
	int32_t m_iWeaponPrice; //0x0904
	int32_t m_iDefaultPrice; //0x0908
	int32_t m_iPreviousPrice; //0x090C
}; //Size: 0x0910

#pragma pack(pop)

class C_BaseCombatWeapon : public C_BaseAnimating
{
public:
	virtual					~C_BaseCombatWeapon() = 0;

	virtual bool			IsPredicted(void) const = 0;
	virtual int				GetSubType(void) = 0;
	virtual void			SetSubType(int iType) = 0;
	virtual void			Equip(C_BaseCombatCharacter* pOwner) = 0;
	virtual void			Drop(const Vector& vecVelocity) = 0;
	virtual	int				UpdateClientData(C_BasePlayer* pPlayer) = 0;
	virtual bool			IsAllowedToSwitch(void) = 0;
	virtual bool			CanBeSelected(void) = 0;
	virtual bool			VisibleInWeaponSelection(void) = 0;
	virtual bool			HasAmmo(void) = 0;
	virtual void			SetPickupTouch(void) = 0;
	virtual void 			DefaultTouch(C_BaseEntity* pOther) = 0;
	virtual void			GiveTo(C_BaseEntity* pOther) = 0;
	virtual bool			ShouldDisplayAltFireHUDHint() = 0;
	virtual void			DisplayAltFireHudHint() = 0;
	virtual void			RescindAltFireHudHint() = 0;
	virtual bool			ShouldDisplayReloadHUDHint() = 0;
	virtual void			DisplayReloadHudHint() = 0;
	virtual void			RescindReloadHudHint() = 0;
	virtual void			SetViewModelIndex(int index = 0) = 0;
	virtual bool			SendWeaponAnim(int iActivity) = 0;
	virtual void			SendViewModelAnim(int nSequence) = 0;
	virtual void			SetViewModel() = 0;
	virtual bool			HasWeaponIdleTimeElapsed(void) = 0;
	virtual void			SetWeaponIdleTime(float time) = 0;
	virtual float			GetWeaponIdleTime(void) = 0;
	virtual bool			HasAnyAmmo(void) = 0;
	virtual bool			HasPrimaryAmmo(void) = 0;
	virtual bool			HasSecondaryAmmo(void) = 0;
	virtual bool			CanHolster(void) const = 0;
	virtual bool			DefaultDeploy(char* szViewModel, char* szWeaponModel, int iActivity, char* szAnimExt) = 0;
	virtual bool			CanDeploy(void) = 0;
	virtual bool			Deploy(void) = 0;
	virtual bool			Holster(C_BaseCombatWeapon* pSwitchingTo = NULL) = 0;
	virtual C_BaseCombatWeapon* GetLastWeapon(void) = 0;
	virtual void			SetWeaponVisible(bool visible) = 0;
	virtual bool			IsWeaponVisible(void) = 0;
	virtual bool			ReloadOrSwitchWeapons(void) = 0;
	virtual void			OnActiveStateChanged(int iOldState) = 0;
	virtual bool			HolsterOnDetach() = 0;
	virtual bool			IsHolstered() = 0;
	virtual void			Detach() = 0;
	virtual void			ItemPreFrame(void) = 0;
	virtual void			ItemPostFrame(void) = 0;
	virtual void			ItemBusyFrame(void) = 0;
	virtual void			ItemHolsterFrame(void) = 0;
	virtual void			WeaponIdle(void) = 0;
	virtual void			HandleFireOnEmpty() = 0;
	virtual bool			CanPerformSecondaryAttack() const = 0;
	virtual bool			ShouldBlockPrimaryFire() = 0;
	virtual void			CreateMove(float flInputSampleTime, CUserCmd* pCmd, const Vector& vecOldViewAngles) = 0;
	virtual bool			IsWeaponZoomed() = 0;
	virtual	void			CheckReload(void) = 0;
	virtual void			FinishReload(void) = 0;
	virtual void			AbortReload(void) = 0;
	virtual bool			Reload(void) = 0;
	virtual bool			AutoFiresFullClip(void) const = 0;
	virtual void			UpdateAutoFire(void) = 0;
	virtual void			PrimaryAttack(void) = 0;
	virtual void			SecondaryAttack(void) = 0;
	virtual Activity		GetPrimaryAttackActivity(void) = 0;
	virtual Activity		GetSecondaryAttackActivity(void) = 0;
	virtual Activity		GetDrawActivity(void) = 0;
	virtual float			GetDefaultAnimSpeed(void) = 0;
	virtual int				GetBulletType(void) = 0;
	virtual const Vector&	GetBulletSpread(void) = 0;
	virtual Vector			GetBulletSpread(WeaponProficiency_t proficiency) = 0;
	virtual float			GetSpreadBias(WeaponProficiency_t proficiency) = 0;
	virtual float			GetFireRate(void) = 0;
	virtual int				GetMinBurst() = 0;
	virtual int				GetMaxBurst() = 0;
	virtual float			GetMinRestTime() = 0;
	virtual float			GetMaxRestTime() = 0;
	virtual int				GetRandomBurst() = 0;
	virtual void			WeaponSound(WeaponSound_t sound_type, float soundtime = 0.0f) = 0;
	virtual void			StopWeaponSound(WeaponSound_t sound_type) = 0;
	virtual const WeaponProficiencyInfo_t* GetProficiencyValues() = 0;
	virtual float			GetMaxAutoAimDeflection() = 0;
	virtual float			WeaponAutoAimScale() = 0;
	virtual bool			StartSprinting(void) = 0;
	virtual bool			StopSprinting(void) = 0;
	virtual float			GetDamage(float flDistance, int iLocation) = 0;
	virtual void			SetActivity(Activity act, float duration) = 0;
	virtual void			AddViewKick(void) = 0;
	virtual char*			GetDeathNoticeName(void) = 0;
	virtual void			OnPickedUp(C_BaseCombatCharacter* pNewOwner) = 0;
	virtual void			AddViewmodelBob(C_BaseViewModel* viewmodel, Vector& origin, Vector& angles) = 0;
	virtual float			CalcViewmodelBob(void) = 0;
	virtual void 			GetControlPanelInfo(int nPanelIndex, const char*& pPanelName) = 0;
	virtual void			GetControlPanelClassName(int nPanelIndex, const char*& pPanelName) = 0;
	virtual bool			ShouldShowControlPanels(void) = 0;
	virtual bool			CanBePickedUpByNPCs(void) = 0;
	virtual int				GetSkinOverride() const = 0;
	virtual const char*		GetViewModel(int viewmodelindex = 0) const = 0;
	virtual const char*		GetWorldModel(void) const = 0;
	virtual const char*		GetAnimPrefix(void) const = 0;
	virtual int				GetMaxClip1(void) const = 0;
	virtual int				GetMaxClip2(void) const = 0;
	virtual int				GetDefaultClip1(void) const = 0;
	virtual int				GetDefaultClip2(void) const = 0;
	virtual int				GetWeight(void) const = 0;
	virtual bool			AllowsAutoSwitchTo(void) const = 0;
	virtual bool			AllowsAutoSwitchFrom(void) const = 0;
	virtual bool			ForceWeaponSwitch(void) const = 0;
	virtual int				GetWeaponFlags(void) const = 0;
	virtual int				GetSlot(void) const = 0;
	virtual int				GetPosition(void) const = 0;
	virtual char const*		GetName(void) const = 0;
	virtual char const*		GetPrintName(void) const = 0;
	virtual char const*		GetShootSound(int iIndex) const = 0;
	virtual int				GetRumbleEffect() const = 0;
	virtual bool			UsesClipsForAmmo1(void) const = 0;
	virtual bool			UsesClipsForAmmo2(void) const = 0;
	virtual const unsigned char* GetEncryptionKey(void) = 0;
	virtual int				GetPrimaryAmmoType(void)  const = 0;
	virtual int				GetSecondaryAmmoType(void)  const = 0;
	virtual int				Clip1() = 0;
	virtual int				Clip2() = 0;
	virtual CHudTexture const* GetSpriteActive(void) const = 0;
	virtual CHudTexture const* GetSpriteInactive(void) const = 0;
	virtual CHudTexture const* GetSpriteAmmo(void) const = 0;
	virtual CHudTexture const* GetSpriteAmmo2(void) const = 0;
	virtual CHudTexture const* GetSpriteCrosshair(void) const = 0;
	virtual CHudTexture const* GetSpriteAutoaim(void) const = 0;
	virtual CHudTexture const* GetSpriteZoomedCrosshair(void) const = 0;
	virtual CHudTexture const* GetSpriteZoomedAutoaim(void) const = 0;
	virtual Activity		ActivityOverride(Activity baseAct, bool* pRequired) = 0;
	virtual	acttable_t*		ActivityList(int& iActivityCount) = 0;
	virtual void			PoseParameterOverride(bool bReset) = 0;
	virtual poseparamtable_t* PoseParamList(int& iPoseParamCount) = 0;
	virtual bool			ShouldUseLargeViewModelVROverride() = 0;
	virtual bool			OnFireEvent(C_BaseViewModel* pViewModel, const Vector& origin, const Vector& angles, int event, const char* options) = 0;
	virtual void			RestartParticleEffect(void) = 0;
	virtual void			Redraw(void) = 0;
	virtual void			ViewModelDrawn(C_BaseViewModel* pViewModel) = 0;
	virtual void			DrawCrosshair(void) = 0;
	virtual bool			ShouldDrawCrosshair(void) = 0;
	virtual bool			IsCarriedByLocalPlayer(void) = 0;
	virtual bool			ShouldDrawUsingViewModel(void) = 0;
	virtual bool			IsActiveByLocalPlayer(void) = 0;
	virtual bool			ShouldDrawPickup(void) = 0;
	virtual void			HandleInput(void) = 0;
	virtual void			OverrideMouseInput(float* x, float* y) = 0;
	virtual int				KeyInput(int down, ButtonCode_t keynum, const char* pszCurrentBinding) = 0;
	virtual bool			AddLookShift(void) = 0;
	virtual void			GetViewmodelBoneControllers(C_BaseViewModel* pViewModel, float controllers[MAXSTUDIOBONECTRLS]) = 0;
	virtual int				GetWorldModelIndex(void) = 0;
	virtual void			GetWeaponCrosshairScale(float& flScale) = 0;
	virtual bool			ViewModel_IsTransparent(void) = 0;
	virtual bool			ViewModel_IsUsingFBTexture(void) = 0;
	virtual bool			IsOverridingViewmodel(void) = 0;
	virtual int				DrawOverriddenViewmodel(C_BaseViewModel* pViewmodel, int flags) = 0;
	virtual bool			CanLower(void) = 0;
	virtual bool			Ready(void) = 0;
	virtual bool			Lower(void) = 0;
	virtual void			HideThink(void) = 0;
	virtual bool			CanReload(void) = 0;
	virtual void			NetworkStateChanged_m_nNextThinkTick(void) = 0;
	virtual void			NetworkStateChanged_m_nNextThinkTick(void* pVar) = 0;

	NETVAR(m_iClip1, int, "CBaseCombatWeapon", "m_iClip1");
	NETVAR(m_iClip2, int, "CBaseCombatWeapon", "m_iClip2");
	NETVAR(m_iPrimaryAmmoType, int, "CBaseCombatWeapon", "m_iPrimaryAmmoType");
	NETVAR(m_iSecondaryAmmoType, int, "CBaseCombatWeapon", "m_iSecondaryAmmoType");
	NETVAR(m_nViewModelIndex, int, "CBaseCombatWeapon", "m_nViewModelIndex");
	NETVAR(m_bFlipViewModel, bool, "CBaseCombatWeapon", "m_bFlipViewModel");
	NETVAR(m_flNextPrimaryAttack, float, "CBaseCombatWeapon", "m_flNextPrimaryAttack");
	NETVAR(m_flNextSecondaryAttack, float, "CBaseCombatWeapon", "m_flNextSecondaryAttack");
	NETVAR(m_nNextThinkTick, int, "CBaseCombatWeapon", "m_nNextThinkTick");
	NETVAR(m_flTimeWeaponIdle, float, "CBaseCombatWeapon", "m_flTimeWeaponIdle");
	NETVAR(m_iViewModelIndex, int, "CBaseCombatWeapon", "m_iViewModelIndex");
	NETVAR(m_iWorldModelIndex, int, "CBaseCombatWeapon", "m_iWorldModelIndex");
	NETVAR(m_iState, int, "CBaseCombatWeapon", "m_iState");
	NETVAR(m_hOwner, EHANDLE, "CBaseCombatWeapon", "m_hOwner");

	CCSWeaponInfo* GetWpnData()
	{
		static uint64_t oGetWpnData = U::Pattern.Find("client.dll", "0F B7 81 ? ? ? ? 50 E8 ? ? ? ? 83 C4 04 C3");
		return reinterpret_cast<CCSWeaponInfo * (__thiscall*)(C_BaseCombatWeapon*)>(oGetWpnData)(this);
	}
};