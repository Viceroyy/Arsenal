#pragma once
#include "IClientEntity.h"
//#include "../../Util/Signatures/Signatures.h"

MAKE_SIGNATURE(C_BaseEntity_SetPredictionRandomSeed, "client.dll", "48 85 C9 75 ? C7 05 ? ? ? ? ? ? ? ? C3", 0x0);
MAKE_SIGNATURE(C_BaseEntity_SetAbsOrigin, "client.dll", "48 89 5C 24 ? 57 48 83 EC ? 48 8B FA 48 8B D9 E8 ? ? ? ? F3 0F 10 83", 0x0);
MAKE_SIGNATURE(C_BaseEntity_SetAbsAngles, "client.dll", "48 89 5C 24 ? 57 48 81 EC ? ? ? ? 48 8B FA 48 8B D9 E8 ? ? ? ? F3 0F 10 83", 0x0);
//MAKE_SIGNATURE(C_BaseEntity_InvalidateBoneCache, "client.dll", "A1 ? ? ? ? 48 C7 81", 0x0);
MAKE_SIGNATURE(C_BaseEntity_PhysicsRunThink, "client.dll", "4C 8B DC 49 89 73 ? 57 48 81 EC ? ? ? ? 8B 81", 0x0);
MAKE_SIGNATURE(PredictionPlayer, "client.dll", "48 89 3D ? ? ? ? 66 0F 6E 87", 0x0);

namespace I { inline C_BasePlayer** PredictionPlayer = nullptr; }

class IClientVehicle;
struct FireBulletsInfo_t;
class CTakeDamageInfo;
class CDmgAccumulator;
class CEntityMapData;
class ISave;
class IRestore;
class IPhysicsObject;
class C_BaseAnimating;
class C_Team;
struct EmitSound_t;
struct RenderGroup_t;
struct CollideType_t;
class CNewParticleEffect;
class CDamageModifier;
class KeyValues;
class IInterpolatedVar;

class VarMapEntry_t
{
public:
	unsigned short type;
	unsigned short m_bNeedsToInterpolate;
	void* data;
	IInterpolatedVar* watcher;
};

struct VarMapping_t
{
	CUtlVector<VarMapEntry_t> m_Entries;
	int m_nInterpolatedEntries;
	float m_lastInterpolationTime;
};

enum thinkmethods_t
{
	THINK_FIRE_ALL_FUNCTIONS,
	THINK_FIRE_BASE_ONLY,
	THINK_FIRE_ALL_BUT_BASE,
};

typedef CHandle<C_BaseEntity> EHANDLE;

class C_BaseEntity : public IClientEntity
{
public:
	virtual							~C_BaseEntity() = 0;

	virtual void* GetDataDescMap() = 0; //datamap_t*
	virtual void* YouForgotToImplementOrDeclareClientClass() = 0;
	virtual void* GetPredDescMap() = 0; //datamap_t*

	virtual void					FireBullets(const FireBulletsInfo_t& info) = 0;
	virtual void					ModifyFireBulletsDamage(CTakeDamageInfo* dmgInfo) = 0;
	virtual bool					ShouldDrawUnderwaterBulletBubbles() = 0;
	virtual bool					ShouldDrawWaterImpacts(void) = 0;
	virtual bool					HandleShotImpactingWater(const FireBulletsInfo_t& info, const Vector& vecEnd, ITraceFilter* pTraceFilter, Vector* pVecTracerDest) = 0;
	virtual ITraceFilter*			GetBeamTraceFilter(void) = 0;
	virtual void					DispatchTraceAttack(const CTakeDamageInfo& info, const Vector& vecDir, trace_t* ptr, CDmgAccumulator* pAccumulator = NULL) = 0;
	virtual void					TraceAttack(const CTakeDamageInfo& info, const Vector& vecDir, trace_t* ptr, CDmgAccumulator* pAccumulator = NULL) = 0;
	virtual void					DoImpactEffect(trace_t& tr, int nDamageType) = 0;
	virtual void					MakeTracer(const Vector& vecTracerSrc, const trace_t& tr, int iTracerType) = 0;
	virtual int						GetTracerAttachment(void) = 0;
	virtual int						BloodColor() = 0;
	virtual const char*				GetTracerType() = 0;
	virtual void					Spawn(void) = 0;
	virtual void					SpawnClientEntity(void) = 0;
	virtual void					Precache(void) = 0;
	virtual void					Activate() = 0;
	virtual void					ParseMapData(CEntityMapData* mapData) = 0;
	virtual bool					KeyValue(const char* szKeyName, const char* szValue) = 0;
	virtual bool					KeyValue(const char* szKeyName, float flValue) = 0;
	virtual bool					KeyValue(const char* szKeyName, const Vector& vecValue) = 0;
	virtual bool					GetKeyValue(const char* szKeyName, char* szValue, int iMaxLen) = 0;
	virtual bool					Init(int entnum, int iSerialNum) = 0;
	virtual C_BaseAnimating*		GetBaseAnimating() = 0;
	virtual void					SetClassname(const char* className) = 0;
	virtual Vector					GetObserverCamOrigin(void) = 0;
	virtual bool					TestCollision(const Ray_t& ray, unsigned int fContentsMask, trace_t& tr) = 0;
	virtual bool					TestHitboxes(const Ray_t& ray, unsigned int fContentsMask, trace_t& tr) = 0;
	virtual float					GetAttackDamageScale(void) = 0;
	virtual void					ValidateModelIndex(void) = 0;
	virtual void					SetDormant(bool bDormant) = 0;
	virtual int						GetEFlags() const = 0;
	virtual void					SetEFlags(int iEFlags) = 0;
	virtual bool					ShouldSavePhysics() = 0;
	virtual void					OnSave() = 0;
	virtual void					OnRestore() = 0;
	virtual int						ObjectCaps(void) = 0;
	virtual int						Save(ISave& save) = 0;
	virtual int						Restore(IRestore& restore) = 0;
	virtual bool					CreateVPhysics() = 0;
	virtual void					VPhysicsDestroyObject(void) = 0;
	virtual void					VPhysicsUpdate(IPhysicsObject* pPhysics) = 0;
	virtual int						VPhysicsGetObjectList(IPhysicsObject** pList, int listMax) = 0;
	virtual bool					VPhysicsIsFlesh(void) = 0;
	virtual void					AddEntity(void) = 0;
	virtual const Vector&			GetPrevLocalOrigin() const = 0;
	virtual const Vector&			GetPrevLocalAngles() const = 0;
	virtual int						CalcOverrideModelIndex() = 0;
	virtual const Vector&			WorldAlignMins() const = 0;
	virtual const Vector&			WorldAlignMaxs() const = 0;
	virtual const Vector&			WorldSpaceCenter() const = 0;
	virtual void					ComputeWorldSpaceSurroundingBox(Vector* pVecWorldMins, Vector* pVecWorldMaxs) = 0;
	virtual float					GetHealthBarHeightOffset() const = 0;
	virtual SolidType_t				GetSolid(void) const = 0;
	virtual int			 			GetSolidFlags(void) const = 0;
	virtual bool					GetAttachment(int number, Vector& origin) = 0;
	virtual bool					GetAttachmentVelocity(int number, Vector& originVel, Quaternion& angleVel) = 0;
	virtual C_Team*					GetTeam(void) = 0;
	virtual int						GetTeamNumber(void) const = 0;
	virtual void					ChangeTeam(int iTeamNum) = 0;
	virtual int						GetRenderTeamNumber(void) = 0;
	virtual bool					InSameTeam(C_BaseEntity* pEntity) = 0;
	virtual bool					InLocalTeam(void) = 0;
	virtual bool					IsValidIDTarget(void) = 0;
	virtual const char*				GetIDString(void) = 0;
	virtual void					ModifyEmitSoundParams(EmitSound_t& params) = 0;
	virtual bool					InitializeAsClientEntity(const char* pszModelName, RenderGroup_t renderGroup) = 0;
	virtual void					Simulate() = 0;
	virtual IClientVehicle*			GetClientVehicle() = 0;
	virtual void					GetAimEntOrigin(IClientEntity* pAttachedTo, Vector* pAbsOrigin, Vector* pAbsAngles) = 0;
	virtual const Vector&			GetOldOrigin() = 0;
	virtual RenderGroup_t			GetRenderGroup() = 0;
	virtual void					GetToolRecordingState(KeyValues* msg) = 0;
	virtual void					CleanupToolRecordingState(KeyValues* msg) = 0;
	virtual CollideType_t			GetCollideType(void) = 0;
	virtual void					UpdateVisibility() = 0;
	virtual bool					IsSelfAnimating() = 0;
	virtual	void					OnLatchInterpolatedVariables(int flags) = 0;
	virtual CStudioHdr*				OnNewModel() = 0;
	virtual void					OnNewParticleEffect(const char* pszParticleName, CNewParticleEffect* pNewParticleEffect) = 0;
	virtual void					ResetLatched() = 0;
	virtual bool					Interpolate(float currentTime) = 0;
	virtual bool					IsSubModel(void) = 0;
	virtual void					CreateLightEffects(void) = 0;
	virtual void					Clear(void) = 0;
	virtual int						DrawBrushModel(bool bTranslucent, int nFlags, bool bTwoPass) = 0;
	virtual float					GetTextureAnimationStartTime() = 0;
	virtual void					TextureAnimationWrapped() = 0;
	virtual void					SetNextClientThink(float nextThinkTime) = 0;
	virtual void					SetHealth(int iHealth) = 0;
	virtual int						GetHealth() const = 0;
	virtual int						GetMaxHealth() const = 0;
	virtual bool					IsVisibleToTargetID(void) const = 0;
	virtual bool					IsHealthBarVisible(void) const = 0;
	virtual void					AddDecal(const Vector& rayStart, const Vector& rayEnd, const Vector& decalCenter, int hitbox, int decalIndex, bool doTrace, trace_t& tr, int maxLODToDecal = ADDDECAL_TO_ALL_LODS) = 0;
	virtual void					AddColoredDecal(const Vector& rayStart, const Vector& rayEnd, const Vector& decalCenter, int hitbox, int decalIndex, bool doTrace, trace_t& tr, Color cColor, int maxLODToDecal = ADDDECAL_TO_ALL_LODS) = 0;
	virtual bool					IsClientCreated(void) const = 0;
	virtual void					UpdateOnRemove(void) = 0;
	virtual void					SUB_Remove(void) = 0;
	virtual void					SetPredictable(bool state) = 0;
	virtual int						RestoreData(const char* context, int slot, int type) = 0;
	virtual char const*				DamageDecal(int bitsDamageType, int gameMaterial) = 0;
	virtual void					DecalTrace(trace_t* pTrace, char const* decalName) = 0;
	virtual void					ImpactTrace(trace_t* pTrace, int iDamageType, const char* pCustomImpactName) = 0;
	virtual bool					ShouldPredict(void) = 0;
	virtual void					Think(void) = 0;
	virtual bool					CanBePoweredUp(void) = 0;
	virtual bool					AttemptToPowerup(int iPowerup, float flTime, float flAmount = 0, C_BaseEntity* pAttacker = NULL, CDamageModifier* pDamageModifier = NULL) = 0;
	virtual bool					IsCurrentlyTouching(void) const = 0;
	virtual void					StartTouch(C_BaseEntity* pOther) = 0;
	virtual void					Touch(C_BaseEntity* pOther) = 0;
	virtual void					EndTouch(C_BaseEntity* pOther) = 0;
	virtual unsigned int			PhysicsSolidMaskForEntity(void) const = 0;
	virtual void					PhysicsSimulate(void) = 0;
	virtual bool					IsAlive(void) = 0;
	virtual bool					IsPlayer(void) const = 0;
	virtual bool					IsBaseCombatCharacter(void) = 0;
	virtual class C_BaseCombatCharacter* MyCombatCharacterPointer(void) = 0;
	virtual bool					IsNPC(void) = 0;
	virtual bool					IsNextBot() = 0;
	virtual bool					IsBaseObject(void) const = 0;
	virtual bool					IsBaseCombatWeapon(void) const = 0;
	virtual class C_BaseCombatWeapon* MyCombatWeaponPointer() = 0;
	virtual bool					IsCombatItem(void) const = 0;
	virtual bool					IsBaseTrain(void) const = 0;
	virtual Vector					EyePosition(void) = 0;
	virtual const Vector&			EyeAngles(void) = 0;
	virtual const Vector&			LocalEyeAngles(void) = 0;
	virtual Vector					EarPosition(void) = 0;
	virtual bool					ShouldCollide(int collisionGroup, int contentsMask) const = 0;
	virtual const Vector&			GetViewOffset() const = 0;
	virtual void					SetViewOffset(const Vector& v) = 0;
	virtual bool					ShouldInterpolate() = 0;
	virtual unsigned char			GetClientSideFade() = 0;
	virtual void					BoneMergeFastCullBloat(Vector& localMins, Vector& localMaxs, const Vector& thisEntityMins, const Vector& thisEntityMaxs) const = 0;
	virtual bool					OnPredictedEntityRemove(bool isbeingremoved, C_BaseEntity* predicted) = 0;
	virtual C_BaseEntity*			GetShadowUseOtherEntity(void) const = 0;
	virtual void					SetShadowUseOtherEntity(C_BaseEntity* pEntity) = 0;
	virtual bool					AddRagdollToFadeQueue(void) = 0;
	virtual bool					IsDeflectable() = 0;
	virtual int						GetStudioBody(void) = 0;
	virtual void					PerformCustomPhysics(Vector* pNewPosition, Vector* pNewVelocity, Vector* pNewAngles, Vector* pNewAngVelocity) = 0;

public:
	NETVAR(m_flAnimTime, float, "CBaseEntity", "m_flAnimTime");
	NETVAR(m_flSimulationTime, float, "CBaseEntity", "m_flSimulationTime");
	NETVAR(m_ubInterpolationFrame, int, "CBaseEntity", "m_ubInterpolationFrame");
	NETVAR(m_vecOrigin, Vector, "CBaseEntity", "m_vecOrigin");
	NETVAR(m_angRotation, Vector, "CBaseEntity", "m_angRotation");
	NETVAR(m_nModelIndex, int, "CBaseEntity", "m_nModelIndex");
	NETVAR(m_fEffects, int, "CBaseEntity", "m_fEffects");
	NETVAR(m_nRenderMode, int, "CBaseEntity", "m_nRenderMode");
	NETVAR(m_nRenderFX, int, "CBaseEntity", "m_nRenderFX");
	NETVAR(m_clrRender, int, "CBaseEntity", "m_clrRender");
	NETVAR(m_iTeamNum, int, "CBaseEntity", "m_iTeamNum");
	NETVAR(m_CollisionGroup, int, "CBaseEntity", "m_CollisionGroup");
	NETVAR(m_flElasticity, float, "CBaseEntity", "m_flElasticity");
	NETVAR(m_flShadowCastDistance, float, "CBaseEntity", "m_flShadowCastDistance");
	NETVAR(m_hOwnerEntity, EHANDLE, "CBaseEntity", "m_hOwnerEntity");
	NETVAR(m_hEffectEntity, EHANDLE, "CBaseEntity", "m_hEffectEntity");
	NETVAR(moveparent, int, "CBaseEntity", "moveparent");
	NETVAR(m_iParentAttachment, int, "CBaseEntity", "m_iParentAttachment");
	NETVAR(m_Collision, void*, "CBaseEntity", "m_Collision");
	NETVAR(m_vecMinsPreScaled, Vector, "CBaseEntity", "m_vecMinsPreScaled");
	NETVAR(m_vecMaxsPreScaled, Vector, "CBaseEntity", "m_vecMaxsPreScaled");
	NETVAR(m_vecMins, Vector, "CBaseEntity", "m_vecMins");
	NETVAR(m_vecMaxs, Vector, "CBaseEntity", "m_vecMaxs");
	NETVAR(m_nSolidType, int, "CBaseEntity", "m_nSolidType");
	NETVAR(m_usSolidFlags, int, "CBaseEntity", "m_usSolidFlags");
	NETVAR(m_nSurroundType, int, "CBaseEntity", "m_nSurroundType");
	NETVAR(m_triggerBloat, int, "CBaseEntity", "m_triggerBloat");
	NETVAR(m_vecSpecifiedSurroundingMinsPreScaled, Vector, "CBaseEntity", "m_vecSpecifiedSurroundingMinsPreScaled");
	NETVAR(m_vecSpecifiedSurroundingMaxsPreScaled, Vector, "CBaseEntity", "m_vecSpecifiedSurroundingMaxsPreScaled");
	NETVAR(m_vecSpecifiedSurroundingMins, Vector, "CBaseEntity", "m_vecSpecifiedSurroundingMins");
	NETVAR(m_vecSpecifiedSurroundingMaxs, Vector, "CBaseEntity", "m_vecSpecifiedSurroundingMaxs");
	NETVAR(m_iTextureFrameIndex, int, "CBaseEntity", "m_iTextureFrameIndex");
	NETVAR(m_PredictableID, int, "CBaseEntity", "m_PredictableID");
	NETVAR(m_bIsPlayerSimulated, bool, "CBaseEntity", "m_bIsPlayerSimulated");
	NETVAR(m_bSimulatedEveryTick, bool, "CBaseEntity", "m_bSimulatedEveryTick");
	NETVAR(m_bAnimatedEveryTick, bool, "CBaseEntity", "m_bAnimatedEveryTick");
	NETVAR(m_bAlternateSorting, bool, "CBaseEntity", "m_bAlternateSorting");

	NETVAR_OFF(m_flOldSimulationTime, float, "CBaseEntity", "m_flSimulationTime", 4);

public:
	void SetInterpolation(bool m_bInterpolation)
	{
		VarMapping_t* map = reinterpret_cast<VarMapping_t*>(uintptr_t(this) + 0x18);

		for (int i = 0; i < map->m_nInterpolatedEntries; i++)
		{
			VarMapEntry_t* e = &map->m_Entries[i];

			e->m_bNeedsToInterpolate = m_bInterpolation;
		}

		// da real interp killa
		map->m_nInterpolatedEntries = m_bInterpolation ? 6 : 0;
	}

	inline C_BaseEntity* GetMoveParent()
	{
		static int nOffset = U::NetVars.GetNetVar("CBaseEntity", "moveparent") - 8;
		auto m_pMoveParent = reinterpret_cast<EHANDLE*>(uintptr_t(this) + nOffset);

		return m_pMoveParent ? m_pMoveParent->Get() : nullptr;
	}
	inline C_BaseEntity* NextMovePeer()
	{
		static int nOffset = U::NetVars.GetNetVar("CBaseEntity", "moveparent") - 16;
		auto m_pMovePeer = reinterpret_cast<EHANDLE*>(uintptr_t(this) + nOffset);

		return m_pMovePeer ? m_pMovePeer->Get() : nullptr;
	}
	inline C_BaseEntity* FirstMoveChild()
	{
		static int nOffset = U::NetVars.GetNetVar("CBaseEntity", "moveparent") - 24;
		auto m_pMoveChild = reinterpret_cast<EHANDLE*>(uintptr_t(this) + nOffset);

		return m_pMoveChild ? m_pMoveChild->Get() : nullptr;
	}

	Vector GetSize()
	{
		return m_vecMaxs() - m_vecMins();
	}

	Vector GetCenter()
	{
		return m_vecOrigin() + Vector(0, 0, (m_vecMins().z + m_vecMaxs().z) / 2);
	}

	Vector GetRenderCenter()
	{
		Vector vMin = {}, vMax = {};
		GetRenderBounds(vMin, vMax);
		return GetRenderOrigin() + Vector(0.f, 0.f, (vMin.z + vMax.z) / 2);
	}

	void SetAbsOrigin(const Vector& absOrigin)
	{
		reinterpret_cast<void(__thiscall*)(void*, const Vector&)>(S::C_BaseEntity_SetAbsOrigin())(this, absOrigin);
	}

	void SetAbsAngles(const Vector& absAngles)
	{
		reinterpret_cast<void(__thiscall*)(void*, const Vector&)>(S::C_BaseEntity_SetAbsAngles())(this, absAngles);
	}

	static void SetPredictionRandomSeed(const CUserCmd* cmd)
	{
		reinterpret_cast<void(*)(const CUserCmd*)>(S::C_BaseEntity_SetPredictionRandomSeed())(cmd);
	}

	static void SetPredictionPlayer(C_BasePlayer* player)
	{
		*reinterpret_cast<C_BasePlayer**>(*reinterpret_cast<DWORD*>(S::PredictionPlayer() + 0x03) + S::PredictionPlayer() + 0x07) = player;
	}

	/*void InvalidateBoneCache()
	{
		reinterpret_cast<void(__thiscall*)(void*)>(S::C_BaseEntity_InvalidateBoneCache())(this);
	}*/

	inline bool PhysicsRunThink(thinkmethods_t thinkMethod = THINK_FIRE_ALL_FUNCTIONS)
	{
		return reinterpret_cast<bool(__thiscall*)(void*, thinkmethods_t)>(S::C_BaseEntity_PhysicsRunThink())(this, thinkMethod);
	}
};

class IGameResources
{
public:
};

class C_PlayerResource : public C_BaseEntity, public IGameResources
{
public:
	NETVAR_ARRAY(m_iPing, int, "CPlayerResource", "m_iPing");
	NETVAR_ARRAY(m_iScore, int, "CPlayerResource", "m_iScore");
	NETVAR_ARRAY(m_iDeaths, int, "CPlayerResource", "m_iDeaths");
	NETVAR_ARRAY(m_bConnected, bool, "CPlayerResource", "m_bConnected");
	NETVAR_ARRAY(m_iTeam, int, "CPlayerResource", "m_iTeam");
	NETVAR_ARRAY(m_bAlive, bool, "CPlayerResource", "m_bAlive");
	NETVAR_ARRAY(m_iHealth, int, "CPlayerResource", "m_iHealth");
	NETVAR_ARRAY(m_iAccountID, unsigned, "CPlayerResource", "m_iAccountID");
	NETVAR_ARRAY(m_bValid, bool, "CPlayerResource", "m_bValid");
	NETVAR_ARRAY(m_iUserID, int, "CPlayerResource", "m_iUserID");
};

class C_CSPlayerResource : public C_PlayerResource
{
public:
	NETVAR_ARRAY(m_iPlayerC4, int, "CCSPlayerResource", "m_iPlayerC4");
	NETVAR_ARRAY(m_iPlayerVIP, int, "CCSPlayerResource", "m_iPlayerVIP");
	NETVAR_ARRAY(m_vecC4, Vector, "CCSPlayerResource", "m_vecC4");
	NETVAR_ARRAY(m_bHostageAlive, bool, "CCSPlayerResource", "m_bHostageAlive");
	NETVAR_ARRAY(m_isHostageFollowingSomeone, bool, "CCSPlayerResource", "m_isHostageFollowingSomeone");
	NETVAR_ARRAY(m_iHostageEntityIDs, int, "CCSPlayerResource", "m_iHostageEntityIDs");
	NETVAR_ARRAY(m_iHostageX, int, "CCSPlayerResource", "m_iHostageX");
	NETVAR_ARRAY(m_iHostageY, int, "CCSPlayerResource", "m_iHostageY");
	NETVAR_ARRAY(m_iHostageZ, int, "CCSPlayerResource", "m_iHostageZ");
	NETVAR_ARRAY(m_bombsiteCenterA, Vector, "CCSPlayerResource", "m_bombsiteCenterA");
	NETVAR_ARRAY(m_bombsiteCenterB, Vector, "CCSPlayerResource", "m_bombsiteCenterB");
	NETVAR_ARRAY(m_hostageRescueX, int, "CCSPlayerResource", "m_hostageRescueX");
	NETVAR_ARRAY(m_hostageRescueY, int, "CCSPlayerResource", "m_hostageRescueY");
	NETVAR_ARRAY(m_hostageRescueZ, int, "CCSPlayerResource", "m_hostageRescueZ");
	NETVAR_ARRAY(m_bBombSpotted, bool, "CCSPlayerResource", "m_bBombSpotted");
	NETVAR_ARRAY(m_bPlayerSpotted, bool, "CCSPlayerResource", "m_bPlayerSpotted");
	NETVAR_ARRAY(m_iMVPs, int, "CCSPlayerResource", "m_iMVPs");
	NETVAR_ARRAY(m_bHasDefuser, bool, "CCSPlayerResource", "m_bHasDefuser");
	NETVAR_ARRAY(m_szClan, const char*, "CCSPlayerResource", "m_szClan");
};