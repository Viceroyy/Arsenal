#pragma once

#include "../../SDK/SDK.h"

class CMaterials
{
public:
	IMaterial* m_pFlat = nullptr;
	IMaterial* m_pShaded = nullptr;
	IMaterial* m_pFlatNoInvis = nullptr;
	IMaterial* m_pShadedNoInvis = nullptr;

private:
	void Initialize();

	std::map<C_BaseEntity*, bool> m_mapDrawnEntities = {};
	bool m_bRendering = false;
	bool m_bRenderingOriginalMat = false;
	bool m_bCleaningUp = false;

	void DrawEntity(C_BaseEntity* pEntity);

private:
	void RunLagRecords();

public:
	void Run();
	void CleanUp();

	inline bool HasDrawn(C_BaseEntity* pEntity) {
		return m_mapDrawnEntities.find(pEntity) != m_mapDrawnEntities.end();
	}

	inline bool IsRendering() {
		return m_bRendering;
	}

	inline bool IsRenderingOriginalMat() {
		return m_bRenderingOriginalMat;
	}

	inline bool IsUsedMaterial(IMaterial* pMaterial)
	{
		return pMaterial == m_pFlat
			|| pMaterial == m_pShaded
			|| pMaterial == m_pFlatNoInvis
			|| pMaterial == m_pShadedNoInvis;
	}

	inline bool IsCleaningUp() { return m_bCleaningUp; }
};

namespace F { inline CMaterials Materials; }