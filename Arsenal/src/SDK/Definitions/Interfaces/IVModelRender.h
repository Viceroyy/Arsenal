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

typedef unsigned short MaterialVarSym_t;
typedef unsigned long FourCC;

class IMaterialVar
{
private:
	char* m_pStringVal;
	int m_intVal;
	Vector4D m_VecVal;
	uint8_t m_Type : 4;
	uint8_t m_nNumVectorComps : 3;
	uint8_t m_bFakeMaterialVar : 1;
	uint8_t m_nTempIndex;
	//CUtlSymbol m_Name;

public:
	virtual ITexture* GetTextureValue(void) = 0;
	virtual char const* GetName(void) const = 0;
	virtual MaterialVarSym_t GetNameAsSymbol() const = 0;
	virtual void SetFloatValue(float val) = 0;
	virtual void SetIntValue(int val) = 0;
	virtual void SetStringValue(char const* val) = 0;
	virtual char const* GetStringValue(void) const = 0;
	virtual void SetFourCCValue(FourCC type, void* pData) = 0;
	virtual void GetFourCCValue(FourCC* type, void** ppData) = 0;
	virtual void SetVecValue(float const* val, int numcomps) = 0;
	virtual void SetVecValue(float x, float y) = 0;
	virtual void SetVecValue(float x, float y, float z) = 0;
	virtual void SetVecValue(float x, float y, float z, float w) = 0;
	virtual void GetLinearVecValue(float* val, int numcomps) const = 0;
	virtual void SetTextureValue(ITexture*) = 0;
	virtual IMaterial* GetMaterialValue(void) = 0;
	virtual void SetMaterialValue(IMaterial*) = 0;
	virtual bool IsDefined() const = 0;
	virtual void SetUndefined() = 0;
	virtual void SetMatrixValue(VMatrix const& matrix) = 0;
	virtual const VMatrix& GetMatrixValue() = 0;
	virtual bool MatrixIsIdentity() const = 0;
	virtual void CopyFrom(IMaterialVar* pMaterialVar) = 0;
	virtual void SetValueAutodetectType(char const* val) = 0;
	virtual IMaterial* GetOwningMaterial() = 0;
	virtual void SetVecComponentValue(float fVal, int nComponent) = 0;
	virtual int GetIntValueInternal(void) const = 0;
	virtual float GetFloatValueInternal(void) const = 0;
	virtual float const* GetVecValueInternal() const = 0;
	virtual void GetVecValueInternal(float* val, int numcomps) const = 0;
	virtual int VectorSizeInternal() const = 0;
};

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

enum PreviewImageRetVal_t
{
	MATERIAL_PREVIEW_IMAGE_BAD = 0,
	MATERIAL_PREVIEW_IMAGE_OK,
	MATERIAL_NO_PREVIEW_IMAGE
};

enum MaterialVarFlags_t
{
	MATERIAL_VAR_DEBUG = (1 << 0),
	MATERIAL_VAR_NO_DEBUG_OVERRIDE = (1 << 1),
	MATERIAL_VAR_NO_DRAW = (1 << 2),
	MATERIAL_VAR_USE_IN_FILLRATE_MODE = (1 << 3),
	MATERIAL_VAR_VERTEXCOLOR = (1 << 4),
	MATERIAL_VAR_VERTEXALPHA = (1 << 5),
	MATERIAL_VAR_SELFILLUM = (1 << 6),
	MATERIAL_VAR_ADDITIVE = (1 << 7),
	MATERIAL_VAR_ALPHATEST = (1 << 8),
	MATERIAL_VAR_MULTIPASS = (1 << 9),
	MATERIAL_VAR_ZNEARER = (1 << 10),
	MATERIAL_VAR_MODEL = (1 << 11),
	MATERIAL_VAR_FLAT = (1 << 12),
	MATERIAL_VAR_NOCULL = (1 << 13),
	MATERIAL_VAR_NOFOG = (1 << 14),
	MATERIAL_VAR_IGNOREZ = (1 << 15),
	MATERIAL_VAR_DECAL = (1 << 16),
	MATERIAL_VAR_ENVMAPSPHERE = (1 << 17),
	MATERIAL_VAR_NOALPHAMOD = (1 << 18),
	MATERIAL_VAR_ENVMAPCAMERASPACE = (1 << 19),
	MATERIAL_VAR_BASEALPHAENVMAPMASK = (1 << 20),
	MATERIAL_VAR_TRANSLUCENT = (1 << 21),
	MATERIAL_VAR_NORMALMAPALPHAENVMAPMASK = (1 << 22),
	MATERIAL_VAR_NEEDS_SOFTWARE_SKINNING = (1 << 23),
	MATERIAL_VAR_OPAQUETEXTURE = (1 << 24),
	MATERIAL_VAR_ENVMAPMODE = (1 << 25),
	MATERIAL_VAR_SUPPRESS_DECALS = (1 << 26),
	MATERIAL_VAR_HALFLAMBERT = (1 << 27),
	MATERIAL_VAR_WIREFRAME = (1 << 28),
	MATERIAL_VAR_ALLOWALPHATOCOVERAGE = (1 << 29),
	MATERIAL_VAR_IGNORE_ALPHA_MODULATION = (1 << 30)
};

enum MaterialPropertyTypes_t
{
	MATERIAL_PROPERTY_NEEDS_LIGHTMAP = 0,
	MATERIAL_PROPERTY_OPACITY,
	MATERIAL_PROPERTY_REFLECTIVITY,
	MATERIAL_PROPERTY_NEEDS_BUMPED_LIGHTMAPS
};

typedef uint64_t VertexFormat_t;
typedef unsigned int MorphFormat_t;

class IMaterial
{
public:
	virtual const char* GetName() const = 0;
	virtual const char* GetTextureGroupName() const = 0;
	virtual PreviewImageRetVal_t GetPreviewImageProperties(int* width, int* height, ImageFormat* imageFormat, bool* isTranslucent) const = 0;
	virtual PreviewImageRetVal_t GetPreviewImage(unsigned char* data, int width, int height, ImageFormat imageFormat) const = 0;
	virtual int GetMappingWidth() = 0;
	virtual int GetMappingHeight() = 0;
	virtual int GetNumAnimationFrames() = 0;
	virtual bool InMaterialPage(void) = 0;
	virtual	void GetMaterialOffset(float* pOffset) = 0;
	virtual void GetMaterialScale(float* pScale) = 0;
	virtual IMaterial* GetMaterialPage(void) = 0;
	virtual IMaterialVar* FindVar(const char* varName, bool* found, bool complain = true) = 0;
	virtual void IncrementReferenceCount(void) = 0;
	virtual void DecrementReferenceCount(void) = 0;
	inline void AddRef() { IncrementReferenceCount(); }
	inline void Release() { DecrementReferenceCount(); }
	virtual int GetEnumerationID(void) const = 0;
	virtual void GetLowResColorSample(float s, float t, float* color) const = 0;
	virtual void RecomputeStateSnapshots() = 0;
	virtual bool IsTranslucent() = 0;
	virtual bool IsAlphaTested() = 0;
	virtual bool IsVertexLit() = 0;
	virtual VertexFormat_t	GetVertexFormat() const = 0;
	virtual bool HasProxy(void) const = 0;
	virtual bool UsesEnvCubemap(void) = 0;
	virtual bool NeedsTangentSpace(void) = 0;
	virtual bool NeedsPowerOfTwoFrameBufferTexture(bool bCheckSpecificToThisFrame = true) = 0;
	virtual bool NeedsFullFrameBufferTexture(bool bCheckSpecificToThisFrame = true) = 0;
	virtual bool NeedsSoftwareSkinning(void) = 0;
	virtual void AlphaModulate(float alpha) = 0;
	virtual void ColorModulate(float r, float g, float b) = 0;
	virtual void SetMaterialVarFlag(MaterialVarFlags_t flag, bool on) = 0;
	virtual bool GetMaterialVarFlag(MaterialVarFlags_t flag) const = 0;
	virtual void GetReflectivity(Vector& reflect) = 0;
	virtual bool GetPropertyFlag(MaterialPropertyTypes_t type) = 0;
	virtual bool IsTwoSided() = 0;
	virtual void SetShader(const char* pShaderName) = 0;
	virtual int GetNumPasses(void) = 0;
	virtual int GetTextureMemoryBytes(void) = 0;
	virtual void Refresh() = 0;
	virtual bool NeedsLightmapBlendAlpha(void) = 0;
	virtual bool NeedsSoftwareLighting(void) = 0;
	virtual int ShaderParamCount() const = 0;
	virtual IMaterialVar** GetShaderParams(void) = 0;
	virtual bool IsErrorMaterial() const = 0;
	virtual void SetUseFixedFunctionBakedLighting(bool bEnable) = 0;
	virtual float GetAlphaModulation() = 0;
	virtual void GetColorModulation(float* r, float* g, float* b) = 0;
	virtual MorphFormat_t GetMorphFormat() const = 0;
	virtual IMaterialVar* FindVarFast(char const* pVarName, unsigned int* pToken) = 0;
	virtual void SetShaderAndParams(KeyValues* pKeyValues) = 0;
	virtual const char* GetShaderName() const = 0;
	virtual void DeleteIfUnreferenced() = 0;
	virtual bool IsSpriteCard() = 0;
	virtual void CallBindProxy(void* proxyData) = 0;
	virtual IMaterial* CheckProxyReplacement(void* proxyData) = 0;
	virtual void RefreshPreservingMaterialVars() = 0;
	virtual bool WasReloadedFromWhitelist() = 0;
	virtual bool IsPrecached() const = 0;
};

inline bool IsErrorMaterial(IMaterial* pMat)
{
	return !pMat || pMat->IsErrorMaterial();
}

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