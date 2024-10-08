#pragma once

#include "../Includes/inetchannel.h"

#define TEXTURE_GROUP_LIGHTMAP 	"Lightmaps"
#define TEXTURE_GROUP_WORLD 		"World textures"
#define TEXTURE_GROUP_MODEL 		"Model textures"
#define TEXTURE_GROUP_VGUI 		"VGUI textures"
#define TEXTURE_GROUP_PARTICLE 	"Particle textures"
#define TEXTURE_GROUP_DECAL 		"Decal textures"
#define TEXTURE_GROUP_SKYBOX 	"SkyBox textures"
#define TEXTURE_GROUP_CLIENT_EFFECTS "ClientEffect textures"
#define TEXTURE_GROUP_OTHER 		"Other textures"
#define TEXTURE_GROUP_PRECACHED 	"Precached"
#define TEXTURE_GROUP_CUBE_MAP 	"CubeMap textures"
#define TEXTURE_GROUP_RENDER_TARGET "RenderTargets"
#define TEXTURE_GROUP_RUNTIME_COMPOSITE "Runtime Composite"
#define TEXTURE_GROUP_UNACCOUNTED "Unaccounted textures"
#define TEXTURE_GROUP_STATIC_INDEX_BUFFER			"Static Indices"
#define TEXTURE_GROUP_STATIC_VERTEX_BUFFER_DISP		"Displacement Verts"
#define TEXTURE_GROUP_STATIC_VERTEX_BUFFER_COLOR	"Lighting Verts"
#define TEXTURE_GROUP_STATIC_VERTEX_BUFFER_WORLD	"World Verts"
#define TEXTURE_GROUP_STATIC_VERTEX_BUFFER_MODELS	"Model Verts"
#define TEXTURE_GROUP_STATIC_VERTEX_BUFFER_OTHER	"Other Verts"
#define TEXTURE_GROUP_DYNAMIC_INDEX_BUFFER			"Dynamic Indices"
#define TEXTURE_GROUP_DYNAMIC_VERTEX_BUFFER			"Dynamic Verts"
#define TEXTURE_GROUP_DEPTH_BUFFER "DepthBuffer"
#define TEXTURE_GROUP_VIEW_MODEL "ViewModel"
#define TEXTURE_GROUP_PIXEL_SHADERS "Pixel Shaders"
#define TEXTURE_GROUP_VERTEX_SHADERS "Vertex Shaders"
#define TEXTURE_GROUP_RENDER_TARGET_SURFACE			"RenderTarget Surfaces"
#define TEXTURE_GROUP_MORPH_TARGETS "Morph Targets"

enum MaterialFogMode_t
{
    MATERIAL_FOG_NONE,
    MATERIAL_FOG_LINEAR,
    MATERIAL_FOG_LINEAR_BELOW_FOG_Z
};

class IMatRenderContext
{
public:

    void SetRenderTarget(ITexture* pTexture)
    {
        using Type = void(__thiscall*)(void*, ITexture*);

        U::VFunc.Get<Type>(this, 6)(this, pTexture);
    }

    void DepthRange(float zNear, float zFar)
    {
        using Type = void(__thiscall*)(void*, float, float);

        U::VFunc.Get<Type>(this, 11)(this, zNear, zFar);
    }

    void ClearBuffers(bool bClearColor, bool bClearDepth, bool bClearStencil = false)
    {
        using Type = void(__thiscall*)(void*, bool, bool, bool);

        U::VFunc.Get<Type>(this, 12)(this, bClearColor, bClearDepth, bClearStencil);
    }

    void Viewport(int x, int y, int width, int height)
    {
        using Type = void(__thiscall*)(void*, int, int, int, int);

        U::VFunc.Get<Type>(this, 38)(this, x, y, width, height);
    }

    void GetViewport(int& x, int& y, int& width, int& height)
    {
        using Type = void(__thiscall*)(void*, int&, int&, int&, int&);

        U::VFunc.Get<Type>(this, 39)(this, x, y, width, height);
    }

    void FogMode(MaterialFogMode_t fogMode)
    {
        using Type = void(__thiscall*)(void*, MaterialFogMode_t);

        U::VFunc.Get<Type>(this, 43)(this, fogMode);
    }

    void FogStart(float fStart)
    {
        using Type = void(__thiscall*)(void*, float);

        U::VFunc.Get<Type>(this, 44)(this, fStart);
    }

    void FogEnd(float fEnd)
    {
        using Type = void(__thiscall*)(void*, float);

        U::VFunc.Get<Type>(this, 45)(this, fEnd);
    }

    void FogColor3fv(float const* rgb)
    {
        using Type = void(__thiscall*)(void*, float const*);

        U::VFunc.Get<Type>(this, 49)(this, rgb);
    }

    void ClearColor3ub(unsigned char r, unsigned char g, unsigned char b)
    {
        using Type = void(__thiscall*)(void*, unsigned char, unsigned char, unsigned char);

        U::VFunc.Get<Type>(this, 72)(this, r, g, b);
    }

    void ClearColor4ub(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
    {
        using Type = void(__thiscall*)(void*, unsigned char, unsigned char, unsigned char, unsigned char);

        U::VFunc.Get<Type>(this, 73)(this, r, g, b, a);
    }

    void OverrideDepthEnable(bool bEnable, bool bDepthEnable)
    {
        using Type = void(__thiscall*)(void*, bool, bool);

        U::VFunc.Get<Type>(this, 74)(this, bEnable, bDepthEnable);
    }

    void DrawScreenSpaceRectangle(IMaterial* A, int B, int C, int D, int E, float F, float G, float H, float I, int J, int K, void* L = NULL, int M = 1, int N = 1)
    {
        using Type = void(__thiscall*)(void*, IMaterial*, int, int, int, int, float, float, float, float, int, int, void*, int, int);

        U::VFunc.Get<Type>(this, 103)(this, A, B, C, D, E, F, G, H, I, J, K, L, M, N);
    }

    void PushRenderTargetAndViewport()
    {
        using Type = void(__thiscall*)(void*);

        U::VFunc.Get<Type>(this, 108)(this);
    }

    void PopRenderTargetAndViewport(void)
    {
        using Type = void(__thiscall*)(void*);

        U::VFunc.Get<Type>(this, 109)(this);
    }

    void SetStencilEnable(bool onoff)
    {
        using Type = void(__thiscall*)(void*, bool);

        U::VFunc.Get<Type>(this, 117)(this, onoff);
    }

    void SetStencilFailOperation(int op)
    {
        using Type = void(__thiscall*)(void*, int);

        U::VFunc.Get<Type>(this, 118)(this, op);
    }

    void SetStencilZFailOperation(int op)
    {
        using Type = void(__thiscall*)(void*, int);

        U::VFunc.Get<Type>(this, 119)(this, op);
    }

    void SetStencilPassOperation(int op)
    {
        using Type = void(__thiscall*)(void*, int);

        U::VFunc.Get<Type>(this, 120)(this, op);
    }

    void SetStencilCompareFunction(int cmpfn)
    {
        using Type = void(__thiscall*)(void*, int);

        U::VFunc.Get<Type>(this, 121)(this, cmpfn);
    }

    void SetStencilReferenceValue(int ref)
    {
        using Type = void(__thiscall*)(void*, int);

        U::VFunc.Get<Type>(this, 122)(this, ref);
    }

    void SetStencilTestMask(uintptr_t msk)
    {
        using Type = void(__thiscall*)(void*, uintptr_t);

        U::VFunc.Get<Type>(this, 123)(this, msk);
    }

    void SetStencilWriteMask(uintptr_t msk)
    {
        using Type = void(__thiscall*)(void*, uintptr_t);

        U::VFunc.Get<Type>(this, 124)(this, msk);
    }

    void BeginPIXEvent(unsigned long color, const char* szName)
    {
        using Type = void(__thiscall*)(void*, unsigned long, const char*);

        U::VFunc.Get<Type>(this, 140)(this, color, szName);
    }

    void EndPIXEvent()
    {
        using Type = void(__thiscall*)(void*);

        U::VFunc.Get<Type>(this, 141)(this);
    }

    void FogMaxDensity(float flMaxDensity)
    {
        using Type = void(__thiscall*)(void*, float);

        U::VFunc.Get<Type>(this, 167)(this, flMaxDensity);
    }
};

enum RenderTargetSizeMode_t
{
    RT_SIZE_NO_CHANGE = 0,
    RT_SIZE_DEFAULT = 1,
    RT_SIZE_PICMIP = 2,
    RT_SIZE_HDR = 3,
    RT_SIZE_FULL_FRAME_BUFFER = 4,
    RT_SIZE_OFFSCREEN = 5,
    RT_SIZE_FULL_FRAME_BUFFER_ROUNDED_UP = 6,
    RT_SIZE_REPLAY_SCREENSHOT = 7,
    RT_SIZE_LITERAL = 8,
    RT_SIZE_LITERAL_PICMIP = 9
};

enum MaterialRenderTargetDepth_t
{
    MATERIAL_RT_DEPTH_SHARED = 0x0,
    MATERIAL_RT_DEPTH_SEPARATE = 0x1,
    MATERIAL_RT_DEPTH_NONE = 0x2,
    MATERIAL_RT_DEPTH_ONLY = 0x3
};

enum CompiledVtfFlags
{
    TEXTUREFLAGS_POINTSAMPLE = 0x00000001,
    TEXTUREFLAGS_TRILINEAR = 0x00000002,
    TEXTUREFLAGS_CLAMPS = 0x00000004,
    TEXTUREFLAGS_CLAMPT = 0x00000008,
    TEXTUREFLAGS_ANISOTROPIC = 0x00000010,
    TEXTUREFLAGS_HINT_DXT5 = 0x00000020,
    TEXTUREFLAGS_SRGB = 0x00000040,
    TEXTUREFLAGS_NORMAL = 0x00000080,
    TEXTUREFLAGS_NOMIP = 0x00000100,
    TEXTUREFLAGS_NOLOD = 0x00000200,
    TEXTUREFLAGS_ALL_MIPS = 0x00000400,
    TEXTUREFLAGS_PROCEDURAL = 0x00000800,
    TEXTUREFLAGS_ONEBITALPHA = 0x00001000,
    TEXTUREFLAGS_EIGHTBITALPHA = 0x00002000,
    TEXTUREFLAGS_ENVMAP = 0x00004000,
    TEXTUREFLAGS_RENDERTARGET = 0x00008000,
    TEXTUREFLAGS_DEPTHRENDERTARGET = 0x00010000,
    TEXTUREFLAGS_NODEBUGOVERRIDE = 0x00020000,
    TEXTUREFLAGS_SINGLECOPY = 0x00040000,
    TEXTUREFLAGS_STAGING_MEMORY = 0x00080000,
    TEXTUREFLAGS_IMMEDIATE_CLEANUP = 0x00100000,
    TEXTUREFLAGS_IGNORE_PICMIP = 0x00200000,
    TEXTUREFLAGS_UNUSED_00400000 = 0x00400000,
    TEXTUREFLAGS_NODEPTHBUFFER = 0x00800000,
    TEXTUREFLAGS_UNUSED_01000000 = 0x01000000,
    TEXTUREFLAGS_CLAMPU = 0x02000000,
    TEXTUREFLAGS_VERTEXTEXTURE = 0x04000000,
    TEXTUREFLAGS_SSBUMP = 0x08000000,
    TEXTUREFLAGS_UNUSED_10000000 = 0x10000000,
    TEXTUREFLAGS_BORDER = 0x20000000,
    TEXTUREFLAGS_STREAMABLE_COARSE = 0x40000000,
    TEXTUREFLAGS_STREAMABLE_FINE = 0x80000000,
    TEXTUREFLAGS_STREAMABLE = (TEXTUREFLAGS_STREAMABLE_COARSE | TEXTUREFLAGS_STREAMABLE_FINE)
};

#define CREATERENDERTARGETFLAGS_HDR 0x00000001
#define CREATERENDERTARGETFLAGS_AUTOMIPMAP 0x00000002
#define CREATERENDERTARGETFLAGS_UNFILTERABLE_OK 0x00000004
#define CREATERENDERTARGETFLAGS_NOEDRAM 0x00000008
#define CREATERENDERTARGETFLAGS_TEMP 0x00000010

enum StencilOperation_t
{
    STENCILOPERATION_KEEP = 1,
    STENCILOPERATION_ZERO = 2,
    STENCILOPERATION_REPLACE = 3,
    STENCILOPERATION_INCRSAT = 4,
    STENCILOPERATION_DECRSAT = 5,
    STENCILOPERATION_INVERT = 6,
    STENCILOPERATION_INCR = 7,
    STENCILOPERATION_DECR = 8,
    STENCILOPERATION_FORCE_DWORD = 0x7fffffff
};

enum StencilComparisonFunction_t
{
    STENCILCOMPARISONFUNCTION_NEVER = 1,
    STENCILCOMPARISONFUNCTION_LESS = 2,
    STENCILCOMPARISONFUNCTION_EQUAL = 3,
    STENCILCOMPARISONFUNCTION_LESSEQUAL = 4,
    STENCILCOMPARISONFUNCTION_GREATER = 5,
    STENCILCOMPARISONFUNCTION_NOTEQUAL = 6,
    STENCILCOMPARISONFUNCTION_GREATEREQUAL = 7,
    STENCILCOMPARISONFUNCTION_ALWAYS = 8,
    STENCILCOMPARISONFUNCTION_FORCE_DWORD = 0x7fffffff
};

enum MaterialThreadMode_t
{
    MATERIAL_SINGLE_THREADED,
    MATERIAL_QUEUED_SINGLE_THREADED,
    MATERIAL_QUEUED_THREADED
};

struct MaterialAdapterInfo_t
{
    char m_pDriverName[512];
    unsigned int m_VendorID;
    unsigned int m_DeviceID;
    unsigned int m_SubSysID;
    unsigned int m_Revision;
    int m_nDXSupportLevel;
    int m_nMaxDXSupportLevel;
    unsigned int m_nDriverVersionHigh;
    unsigned int m_nDriverVersionLow;
};

struct MaterialVideoMode_t
{
    int m_Width;
    int m_Height;
    ImageFormat m_Format;
    int m_RefreshRate;
};

enum HDRType_t
{
    HDR_TYPE_NONE,
    HDR_TYPE_INTEGER,
    HDR_TYPE_FLOAT
};

struct MaterialSystem_SortInfo_t
{
    IMaterial* material;
    int lightmapPageID;
};

enum MaterialContextType_t
{
    MATERIAL_HARDWARE_CONTEXT,
    MATERIAL_QUEUED_CONTEXT,
    MATERIAL_NULL_CONTEXT
};

class IMaterialProxyFactory;
class IMaterialSystemHardwareConfig;
class IShader;
class ITextureCompositor;
struct MaterialSystem_Config_t;
struct MaterialSystemHardwareIdentifier_t;

typedef unsigned short MaterialHandle_t;

typedef void (*MaterialBufferReleaseFunc_t)();
typedef void (*MaterialBufferRestoreFunc_t)(int nChangeFlags);
typedef void (*ModeChangeCallbackFunc_t)(void);

using MaterialLock_t = void*;

class IAsyncTextureOperationReceiver : public IRefCounted
{
public:
    virtual void OnAsyncCreateComplete(ITexture* pTex, void* pExtraArgs) = 0;
    virtual void OnAsyncFindComplete(ITexture* pTex, void* pExtraArgs) = 0;
    virtual void OnAsyncMapComplete(ITexture* pTex, void* pExtraArgs, void* pMemory, int nPitch) = 0;
    virtual void OnAsyncReadbackBegin(ITexture* pDst, ITexture* pSrc, void* pExtraArgs) = 0;
    virtual int GetRefCount() const = 0;
};

class IMaterialSystem : public IAppSystem
{
public:
	virtual bool Connect(CreateInterfaceFn factory) = 0;
	virtual void Disconnect() = 0;
	virtual void* QueryInterface(const char* pInterfaceName) = 0;
	virtual InitReturnVal_t Init() = 0;
	virtual void Shutdown() = 0;
	virtual CreateInterfaceFn Init(char const* pShaderAPIDLL, IMaterialProxyFactory* pMaterialProxyFactory, CreateInterfaceFn fileSystemFactory, CreateInterfaceFn cvarFactory = NULL) = 0;
	virtual void SetShaderAPI(char const* pShaderAPIDLL) = 0;
	virtual void SetAdapter(int nAdapter, int nFlags) = 0;
	virtual void ModInit() = 0;
	virtual void ModShutdown() = 0;
	virtual void SetThreadMode(MaterialThreadMode_t mode, int nServiceThread = -1) = 0;
	virtual MaterialThreadMode_t GetThreadMode() = 0;
	virtual bool IsRenderThreadSafe() = 0;
	virtual void ExecuteQueued() = 0;
	virtual IMaterialSystemHardwareConfig* GetHardwareConfig(const char* pVersion, int* returnCode) = 0;
	virtual bool UpdateConfig(bool bForceUpdate) = 0;
	virtual bool OverrideConfig(const MaterialSystem_Config_t& config, bool bForceUpdate) = 0;
	virtual const MaterialSystem_Config_t& GetCurrentConfigForVideoCard() const = 0;
	virtual bool GetRecommendedConfigurationInfo(int nDXLevel, KeyValues* pKeyValues) = 0;
	virtual int GetDisplayAdapterCount() const = 0;
	virtual int GetCurrentAdapter() const = 0;
	virtual void GetDisplayAdapterInfo(int adapter, MaterialAdapterInfo_t& info) const = 0;
	virtual int GetModeCount(int adapter) const = 0;
	virtual void GetModeInfo(int adapter, int mode, MaterialVideoMode_t& info) const = 0;
	virtual void AddModeChangeCallBack(ModeChangeCallbackFunc_t func) = 0;
	virtual void GetDisplayMode(MaterialVideoMode_t& mode) const = 0;
	virtual bool SetMode(void* hwnd, const MaterialSystem_Config_t& config) = 0;
	virtual bool SupportsMSAAMode(int nMSAAMode) = 0;
	virtual const MaterialSystemHardwareIdentifier_t& GetVideoCardIdentifier(void) const = 0;
	virtual void SpewDriverInfo() const = 0;
	virtual void GetDXLevelDefaults(unsigned int& max_dxlevel, unsigned int& recommended_dxlevel) = 0;
	virtual void GetBackBufferDimensions(int& width, int& height) const = 0;
	virtual ImageFormat GetBackBufferFormat() const = 0;
	virtual bool SupportsHDRMode(HDRType_t nHDRModede) = 0;
	virtual bool AddView(void* hwnd) = 0;
	virtual void RemoveView(void* hwnd) = 0;
	virtual void SetView(void* hwnd) = 0;
	virtual void BeginFrame(float frameTime) = 0;
	virtual void EndFrame() = 0;
	virtual void Flush(bool flushHardware = false) = 0;
	virtual void SwapBuffers() = 0;
	virtual void EvictManagedResources() = 0;
	virtual void ReleaseResources(void) = 0;
	virtual void ReacquireResources(void) = 0;
	virtual void AddReleaseFunc(MaterialBufferReleaseFunc_t func) = 0;
	virtual void RemoveReleaseFunc(MaterialBufferReleaseFunc_t func) = 0;
	virtual void AddRestoreFunc(MaterialBufferRestoreFunc_t func) = 0;
	virtual void RemoveRestoreFunc(MaterialBufferRestoreFunc_t func) = 0;
	virtual void ResetTempHWMemory(bool bExitingLevel = false) = 0;
	virtual void HandleDeviceLost() = 0;
	virtual int ShaderCount() const = 0;
	virtual int GetShaders(int nFirstShader, int nMaxCount, IShader** ppShaderList) const = 0;
	virtual int ShaderFlagCount() const = 0;
	virtual const char* ShaderFlagName(int nIndex) const = 0;
	virtual void GetShaderFallback(const char* pShaderName, char* pFallbackShader, int nFallbackLength) = 0;
	virtual IMaterialProxyFactory* GetMaterialProxyFactory() = 0;
	virtual void SetMaterialProxyFactory(IMaterialProxyFactory* pFactory) = 0;
	virtual void EnableEditorMaterials() = 0;
	virtual void SetInStubMode(bool bInStubMode) = 0;
	virtual void DebugPrintUsedMaterials(const char* pSearchSubString, bool bVerbose) = 0;
	virtual void DebugPrintUsedTextures(void) = 0;
	virtual void ToggleSuppressMaterial(char const* pMaterialName) = 0;
	virtual void ToggleDebugMaterial(char const* pMaterialName) = 0;
	virtual bool UsingFastClipping(void) = 0;
	virtual int StencilBufferBits(void) = 0;
	virtual void UncacheAllMaterials() = 0;
	virtual void UncacheUnusedMaterials(bool bRecomputeStateSnapshots = false) = 0;
	virtual void CacheUsedMaterials() = 0;
	virtual void ReloadTextures() = 0;
	virtual void ReloadMaterials(const char* pSubString = NULL) = 0;
	virtual IMaterial* CreateMaterial(const char* pMaterialName, KeyValues* pVMTKeyValues) = 0;
	virtual IMaterial* FindMaterial(char const* pMaterialName, const char* pTextureGroupName, bool complain = true, const char* pComplainPrefix = NULL) = 0;
	virtual bool IsMaterialLoaded(char const* pMaterialName) = 0;
	virtual MaterialHandle_t FirstMaterial() const = 0;
	virtual MaterialHandle_t NextMaterial(MaterialHandle_t h) const = 0;
	virtual MaterialHandle_t InvalidMaterial() const = 0;
	virtual IMaterial* GetMaterial(MaterialHandle_t h) const = 0;
	virtual int GetNumMaterials() const = 0;
	virtual void SetAsyncTextureLoadCache(void* hFileCache) = 0;
	virtual ITexture* FindTexture(char const* pTextureName, const char* pTextureGroupName, bool complain = true, int nAdditionalCreationFlags = 0) = 0;
	virtual bool IsTextureLoaded(char const* pTextureName) const = 0;
	virtual ITexture* CreateProceduralTexture(const char* pTextureName, const char* pTextureGroupName, int w, int h, ImageFormat fmt, int nFlags) = 0;
	virtual void BeginRenderTargetAllocation() = 0;
	virtual void EndRenderTargetAllocation() = 0;
	virtual ITexture* CreateRenderTargetTexture(int w, int h, RenderTargetSizeMode_t sizeMode, ImageFormat	format, MaterialRenderTargetDepth_t depth = MATERIAL_RT_DEPTH_SHARED) = 0;
	virtual ITexture* CreateNamedRenderTargetTextureEx(const char* pRTName, int w, int h, RenderTargetSizeMode_t sizeMode, ImageFormat format, MaterialRenderTargetDepth_t depth = MATERIAL_RT_DEPTH_SHARED, unsigned int textureFlags = TEXTUREFLAGS_CLAMPS | TEXTUREFLAGS_CLAMPT, unsigned int renderTargetFlags = 0) = 0;
	virtual ITexture* CreateNamedRenderTargetTexture(const char* pRTName, int w, int h, RenderTargetSizeMode_t sizeMode, ImageFormat format, MaterialRenderTargetDepth_t depth = MATERIAL_RT_DEPTH_SHARED, bool bClampTexCoords = true, bool bAutoMipMap = false) = 0;
	virtual ITexture* CreateNamedRenderTargetTextureEx2(const char* pRTName, int w, int h, RenderTargetSizeMode_t sizeMode, ImageFormat format, MaterialRenderTargetDepth_t depth = MATERIAL_RT_DEPTH_SHARED, unsigned int textureFlags = TEXTUREFLAGS_CLAMPS | TEXTUREFLAGS_CLAMPT, unsigned int renderTargetFlags = 0) = 0;
	virtual void BeginLightmapAllocation() = 0;
	virtual void EndLightmapAllocation() = 0;
	virtual int AllocateLightmap(int width, int height, int offsetIntoLightmapPage[2], IMaterial* pMaterial) = 0;
	virtual int AllocateWhiteLightmap(IMaterial* pMaterial) = 0;
	virtual void UpdateLightmap(int lightmapPageID, int lightmapSize[2], int offsetIntoLightmapPage[2], float* pFloatImage, float* pFloatImageBump1, float* pFloatImageBump2, float* pFloatImageBump3) = 0;
	virtual int GetNumSortIDs() = 0;
	virtual void GetSortInfo(MaterialSystem_SortInfo_t* sortInfoArray) = 0;
	virtual void GetLightmapPageSize(int lightmap, int* width, int* height) const = 0;
	virtual void ResetMaterialLightmapPageInfo() = 0;
	virtual void ClearBuffers(bool bClearColor, bool bClearDepth, bool bClearStencil = false) = 0;
	virtual IMatRenderContext* GetRenderContext() = 0;
	virtual bool SupportsShadowDepthTextures(void) = 0;
	virtual void BeginUpdateLightmaps(void) = 0;
	virtual void EndUpdateLightmaps(void) = 0;
	virtual MaterialLock_t Lock() = 0;
	virtual void Unlock(MaterialLock_t) = 0;
	virtual ImageFormat GetShadowDepthTextureFormat() = 0;
	virtual bool SupportsFetch4(void) = 0;
	virtual IMatRenderContext* CreateRenderContext(MaterialContextType_t type) = 0;
	virtual IMatRenderContext* SetRenderContext(IMatRenderContext*) = 0;
	virtual bool SupportsCSAAMode(int nNumSamples, int nQualityLevel) = 0;
	virtual void RemoveModeChangeCallBack(ModeChangeCallbackFunc_t func) = 0;
	virtual IMaterial* FindProceduralMaterial(const char* pMaterialName, const char* pTextureGroupName, KeyValues* pVMTKeyValues) = 0;
	virtual ImageFormat	GetNullTextureFormat() = 0;
	virtual void AddTextureAlias(const char* pAlias, const char* pRealName) = 0;
	virtual void RemoveTextureAlias(const char* pAlias) = 0;
	virtual int AllocateDynamicLightmap(int lightmapSize[2], int* pOutOffsetIntoPage, int frameID) = 0;
	virtual void SetExcludedTextures(const char* pScriptName) = 0;
	virtual void UpdateExcludedTextures(void) = 0;
	virtual bool IsInFrame() const = 0;
	virtual void CompactMemory() = 0;
	virtual void ReloadFilesInList(IFileList* pFilesToReload) = 0;
	virtual	bool AllowThreading(bool bAllow, int nServiceThread) = 0;
	virtual IMaterial* FindMaterialEx(char const* pMaterialName, const char* pTextureGroupName, int nContext, bool complain = true, const char* pComplainPrefix = NULL) = 0;
	virtual void SetRenderTargetFrameBufferSizeOverrides(int nWidth, int nHeight) = 0;
	virtual void GetRenderTargetFrameBufferDimensions(int& nWidth, int& nHeight) = 0;
	virtual char* GetDisplayDeviceName() const = 0;
	virtual ITexture* CreateTextureFromBits(int w, int h, int mips, ImageFormat fmt, int srcBufferSize, byte* srcBits) = 0;
	virtual void OverrideRenderTargetAllocation(bool rtAlloc) = 0;
	virtual ITextureCompositor* NewTextureCompositor(int w, int h, const char* pCompositeName, int nTeamNum, unsigned long long randomSeed, KeyValues* stageDesc, unsigned int texCompositeCreateFlags = 0) = 0;
	virtual void AsyncFindTexture(const char* pFilename, const char* pTextureGroupName, IAsyncTextureOperationReceiver* pRecipient, void* pExtraArgs, bool bComplain = true, int nAdditionalCreationFlags = 0) = 0;
	virtual ITexture* CreateNamedTextureFromBitsEx(const char* pName, const char* pTextureGroupName, int w, int h, int mips, ImageFormat fmt, int srcBufferSize, byte* srcBits, int nFlags) = 0;
};

namespace I { inline IMaterialSystem* MaterialSystem = nullptr; }