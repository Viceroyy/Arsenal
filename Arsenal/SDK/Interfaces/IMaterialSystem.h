#pragma once

#include "../Includes/inetchannel.h"

#define TEXTURE_GROUP_LIGHTMAP						"Lightmaps"
#define TEXTURE_GROUP_WORLD							"World textures"
#define TEXTURE_GROUP_MODEL							"Model textures"
#define TEXTURE_GROUP_VGUI							"VGUI textures"
#define TEXTURE_GROUP_PARTICLE						"Particle textures"
#define TEXTURE_GROUP_DECAL							"Decal textures"
#define TEXTURE_GROUP_SKYBOX						"SkyBox textures"
#define TEXTURE_GROUP_CLIENT_EFFECTS				"ClientEffect textures"
#define TEXTURE_GROUP_OTHER							"Other textures"
#define TEXTURE_GROUP_PRECACHED						"Precached"
#define TEXTURE_GROUP_CUBE_MAP						"CubeMap textures"
#define TEXTURE_GROUP_RENDER_TARGET					"RenderTargets"
#define TEXTURE_GROUP_RUNTIME_COMPOSITE				"Runtime Composite"
#define TEXTURE_GROUP_UNACCOUNTED					"Unaccounted textures"
#define TEXTURE_GROUP_STATIC_INDEX_BUFFER			"Static Indices"
#define TEXTURE_GROUP_STATIC_VERTEX_BUFFER_DISP		"Displacement Verts"
#define TEXTURE_GROUP_STATIC_VERTEX_BUFFER_COLOR	"Lighting Verts"
#define TEXTURE_GROUP_STATIC_VERTEX_BUFFER_WORLD	"World Verts"
#define TEXTURE_GROUP_STATIC_VERTEX_BUFFER_MODELS	"Model Verts"
#define TEXTURE_GROUP_STATIC_VERTEX_BUFFER_OTHER	"Other Verts"
#define TEXTURE_GROUP_DYNAMIC_INDEX_BUFFER			"Dynamic Indices"
#define TEXTURE_GROUP_DYNAMIC_VERTEX_BUFFER			"Dynamic Verts"
#define TEXTURE_GROUP_DEPTH_BUFFER					"DepthBuffer"
#define TEXTURE_GROUP_VIEW_MODEL					"ViewModel"
#define TEXTURE_GROUP_PIXEL_SHADERS					"Pixel Shaders"
#define TEXTURE_GROUP_VERTEX_SHADERS				"Vertex Shaders"
#define TEXTURE_GROUP_RENDER_TARGET_SURFACE			"RenderTarget Surfaces"
#define TEXTURE_GROUP_MORPH_TARGETS					"Morph Targets"

class IMatRenderContext
{
public:

    void SetRenderTarget(ITexture* pTexture)
    {
        using Type = void(__thiscall*)(void*, ITexture*);

        U::VFunc.Get<Type>(this, 6)(this, pTexture);
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

    void ClearColor3ub(unsigned char r, unsigned char g, unsigned char b)
    {
        using Type = void(__thiscall*)(void*, unsigned char, unsigned char, unsigned char);

        U::VFunc.Get<Type>(this, 72)(this, r, g, b);
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

class IMaterialSystem
{
    using Handle = unsigned short;
public:

    IMaterial* FindMaterial
    (
        char const* pMaterialName,
        const char* pTextureGroupName,
        bool complain = true, const char* pComplainPrefix = NULL) {

        using Type = IMaterial * (__thiscall*)(void*, char const*, const char*, bool, const char*);

        return U::VFunc.Get<Type>(this, 71)(this, pMaterialName, pTextureGroupName, complain, pComplainPrefix);
    }

    Handle FirstMaterial()
    {
        using Type = Handle(__thiscall*)(void*);

        return U::VFunc.Get<Type>(this, 73)(this);
    }

    Handle NextMaterial(Handle h)
    {
        using Type = Handle(__thiscall*)(void*, Handle);

        return U::VFunc.Get<Type>(this, 74)(this, h);
    }

    Handle InvalidMaterial()
    {
        using Type = Handle(__thiscall*)(void*);

        return U::VFunc.Get<Type>(this, 75)(this);
    }

    IMaterial* GetMaterial(Handle h)
    {
        using Type = IMaterial * (__thiscall*)(void*, Handle);

        return U::VFunc.Get<Type>(this, 76)(this, h);
    }

    ITexture* FindTexture(char const* pTextureName, const char* pTextureGroupName, bool complain = true, int nAdditionalCreationFlags = 0)
    {
        using Type = ITexture * (__thiscall*)(void*, const char*, const char*, bool, int);

        return U::VFunc.Get<Type>(this, 79)(this, pTextureName, pTextureGroupName, complain, nAdditionalCreationFlags);
    }

    ITexture* CreateNamedRenderTargetTextureEx(const char* pRTName, int w, int h, RenderTargetSizeMode_t sizeMode, ImageFormat format, MaterialRenderTargetDepth_t depth = MATERIAL_RT_DEPTH_SHARED, unsigned int textureFlags = TEXTUREFLAGS_CLAMPS | TEXTUREFLAGS_CLAMPT, unsigned int renderTargetFlags = 0)
    {
        using Type = ITexture * (__thiscall*)(void*, const char*, int, int, RenderTargetSizeMode_t, ImageFormat, MaterialRenderTargetDepth_t, unsigned int, unsigned int);

        return U::VFunc.Get<Type>(this, 85)(this, pRTName, w, h, sizeMode, format, depth, textureFlags, renderTargetFlags);
    }

    IMatRenderContext* GetRenderContext()
    {
        using Type = IMatRenderContext * (__thiscall*)(void*);

        return U::VFunc.Get<Type>(this, 98)(this);
    }
};

namespace I { inline IMaterialSystem* MaterialSystem = nullptr; }