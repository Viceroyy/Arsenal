//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//
//===========================================================================//

#ifndef IVMODELRENDER_H
#define IVMODELRENDER_H

#ifdef _WIN32
#pragma once
#endif

#include "istudiorender.h"

struct mstudioanimdesc_t;
struct mstudioseqdesc_t;
struct model_t;
class IClientRenderable;
class Vector;
struct studiohdr_t;
class IMaterial;
class CStudioHdr;
class IMaterialVar;

FORWARD_DECLARE_HANDLE(LightCacheHandle_t);

struct DrawModelState_t
{
	studiohdr_t* m_pStudioHdr;
	studiohwdata_t* m_pStudioHWData;
	IClientRenderable* m_pRenderable;
	const matrix3x4_t* m_pModelToWorld;
	StudioDecalHandle_t		m_decals;
	int						m_drawFlags;
	int						m_lod;
};

typedef unsigned short ModelInstanceHandle_t;

enum
{
	MODEL_INSTANCE_INVALID = (ModelInstanceHandle_t)~0
};

struct ModelRenderInfo_t
{
	Vector origin;
	Vector angles;
	IClientRenderable* pRenderable;
	const model_t* pModel;
	const matrix3x4_t* pModelToWorld;
	const matrix3x4_t* pLightingOffset;
	const Vector* pLightingOrigin;
	int flags;
	int entity_index;
	int skin;
	int body;
	int hitboxset;
	ModelInstanceHandle_t instance;

	ModelRenderInfo_t()
	{
		pModelToWorld = NULL;
		pLightingOffset = NULL;
		pLightingOrigin = NULL;
	}
};

struct StaticPropRenderInfo_t
{
	const matrix3x4_t* pModelToWorld;
	const model_t* pModel;
	IClientRenderable* pRenderable;
	Vector* pLightingOrigin;
	short					skin;
	ModelInstanceHandle_t	instance;
};

enum class MaterialVarFlags
{
    MATERIAL_VAR_NO_DRAW = (1 << 2),
    MATERIAL_VAR_FLAT = (1 << 12),
    MATERIAL_VAR_IGNOREZ = (1 << 15),
    MATERIAL_VAR_TRANSLUCENT = (1 << 21),
    MATERIAL_VAR_WIREFRAME = (1 << 28),
};

class IMaterial
{
public:

    const char* GetName()
    {
        using Type = const char* (__thiscall*)(void*);

        return U::VFunc.Get<Type>(this, 0)(this);
    }

    const char* GetTextureGroupName()
    {
        using Type = const char* (__thiscall*)(void*);

        return U::VFunc.Get<Type>(this, 1)(this);
    }

    void IncrementReferenceCount()
    {
        using Type = void(__thiscall*)(void*);

        return U::VFunc.Get<Type>(this, 12)(this);
    }

    void DecrementReferenceCount()
    {
        using Type = void(__thiscall*)(void*);

        return U::VFunc.Get<Type>(this, 13)(this);
    }

    void AlphaModulate(float alpha)
    {
        using Type = void(__thiscall*)(void*, float);

        return U::VFunc.Get<Type>(this, 27)(this, alpha);
    }

    void ColorModulate(float r, float g, float b)
    {
        using Type = void(__thiscall*)(void*, float, float, float);

        return U::VFunc.Get<Type>(this, 28)(this, r, g, b);
    }

    void SetMaterialVarFlag(MaterialVarFlags flag, bool on)
    {
        using Type = void(__thiscall*)(void*, MaterialVarFlags, bool);

        return U::VFunc.Get<Type>(this, 29)(this, flag, on);
    }

    bool GetMaterialVarFlag(MaterialVarFlags flag)
    {
        using Type = bool(__thiscall*)(void*, MaterialVarFlags);

        return U::VFunc.Get<Type>(this, 30)(this, flag);
    }

    bool IsErrorMaterial()
    {
        using Type = bool(__thiscall*)(void*);

        return U::VFunc.Get<Type>(this, 42)(this);
    }

    const bool IsPrecached()
    {
        using Type = const bool(__thiscall*)(void*);

        return U::VFunc.Get<Type>(this, 56)(this);
    }

    void Precache()
    {
        using Type = void(__thiscall*)(void*);

        return U::VFunc.Get<Type>(this, 62)(this);
    }

    IMaterialVar* FindVar(const char* varName, bool* found, bool complain = true)
    {
        using Type = IMaterialVar * (__thiscall*)(void*, const char*, bool*, bool);

        return U::VFunc.Get<Type>(this, 11)(this, varName, found, complain);
    }

public:

    inline void AddRef()
    {
        IncrementReferenceCount();
    }

    inline void Release()
    {
        DecrementReferenceCount();
    }
};

class IVModelRender
{
public:
	virtual int		DrawModel(int flags, IClientRenderable * pRenderable, ModelInstanceHandle_t instance, int entity_index, const model_t * model, Vector const& origin, Vector const& angles, int skin, int body, int hitboxset, const matrix3x4_t * modelToWorld = NULL, const matrix3x4_t * pLightingOffset = NULL) = 0;
	virtual void	ForcedMaterialOverride(IMaterial* newMaterial, OverrideType_t nOverrideType = OVERRIDE_NORMAL) = 0;
	virtual void	SetViewTarget(const CStudioHdr* pStudioHdr, int nBodyIndex, const Vector& target) = 0;
	virtual ModelInstanceHandle_t CreateInstance(IClientRenderable* pRenderable, LightCacheHandle_t* pCache = NULL) = 0;
	virtual void DestroyInstance(ModelInstanceHandle_t handle) = 0;
	virtual void SetStaticLighting(ModelInstanceHandle_t handle, LightCacheHandle_t* pHandle) = 0;
	virtual LightCacheHandle_t GetStaticLighting(ModelInstanceHandle_t handle) = 0;
	virtual bool ChangeInstance(ModelInstanceHandle_t handle, IClientRenderable* pRenderable) = 0;
	virtual void AddDecal(ModelInstanceHandle_t handle, Ray_t const& ray, Vector const& decalUp, int decalIndex, int body, bool noPokeThru = false, int maxLODToDecal = ADDDECAL_TO_ALL_LODS) = 0;
	virtual void RemoveAllDecals(ModelInstanceHandle_t handle) = 0;
	virtual void RemoveAllDecalsFromAllModels() = 0;
	virtual matrix3x4_t* DrawModelShadowSetup(IClientRenderable* pRenderable, int body, int skin, DrawModelInfo_t* pInfo, matrix3x4_t* pCustomBoneToWorld = NULL) = 0;
	virtual void DrawModelShadow(IClientRenderable* pRenderable, const DrawModelInfo_t& info, matrix3x4_t* pCustomBoneToWorld = NULL) = 0;
	virtual bool RecomputeStaticLighting(ModelInstanceHandle_t handle) = 0;
	virtual void ReleaseAllStaticPropColorData(void) = 0;
	virtual void RestoreAllStaticPropColorData(void) = 0;
	virtual int	DrawModelEx(ModelRenderInfo_t& pInfo) = 0;
	virtual int	DrawModelExStaticProp(ModelRenderInfo_t& pInfo) = 0;
	virtual bool DrawModelSetup(ModelRenderInfo_t& pInfo, DrawModelState_t* pState, matrix3x4_t* pCustomBoneToWorld, matrix3x4_t** ppBoneToWorldOut) = 0;
	virtual void DrawModelExecute(const DrawModelState_t& state, const ModelRenderInfo_t& pInfo, matrix3x4_t* pCustomBoneToWorld = NULL) = 0;
	virtual void SetupLighting(const Vector& vecCenter) = 0;
	virtual int DrawStaticPropArrayFast(StaticPropRenderInfo_t* pProps, int count, bool bShadowDepth) = 0;
	virtual void SuppressEngineLighting(bool bSuppress) = 0;
	virtual void SetupColorMeshes(int nTotalVerts) = 0;
	virtual void AddColoredDecal(ModelInstanceHandle_t handle, Ray_t const& ray, Vector const& decalUp, int decalIndex, int body, Color cColor, bool noPokeThru = false, int maxLODToDecal = ADDDECAL_TO_ALL_LODS) = 0;
	virtual void GetMaterialOverride(IMaterial** ppOutForcedMaterial, OverrideType_t* pOutOverrideType) = 0;
};

namespace I { inline IVModelRender* ModelRender = nullptr; }

#endif // IVMODELRENDER_H