#pragma once

#include "../../SDK/SDK.h"

class CMaterials
{
	void Initialize();

	std::map<C_BaseEntity*, bool> m_mapDrawnEntities = {};
	bool m_bRendering = false;
	bool m_bRenderingOriginalMat = false;
	bool m_bCleaningUp = false;

	void DrawEntity(C_BaseEntity* pEntity);

public:
	IMaterial* m_pFlat = nullptr;
	IMaterial* m_pShaded = nullptr;
	IMaterial* m_pGlossy = nullptr;
	IMaterial* m_pGlow = nullptr;
	IMaterial* m_pPlastic = nullptr;
	IMaterialVar* m_pGlowEnvmapTint = nullptr;
	IMaterialVar* m_pGlowSelfillumTint = nullptr;

	void Run();
	void CleanUp();
	void RunLagRecords(const DrawModelState_t& pState, const ModelRenderInfo_t& pInfo);

	bool HasDrawn(C_BaseEntity* pEntity)
	{
		return m_mapDrawnEntities.contains(pEntity);
	}

	bool IsRendering()
	{
		return m_bRendering;
	}

	bool IsRenderingOriginalMat()
	{
		return m_bRenderingOriginalMat;
	}

	bool IsUsedMaterial(const IMaterial* pMaterial)
	{
		return pMaterial == m_pFlat
			|| pMaterial == m_pShaded
			|| pMaterial == m_pGlossy
			|| pMaterial == m_pGlow
			|| pMaterial == m_pPlastic;
	}

	bool IsCleaningUp() { return m_bCleaningUp; }
};

namespace F { inline CMaterials Materials; }