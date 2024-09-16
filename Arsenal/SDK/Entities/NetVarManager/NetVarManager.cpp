#include "NetVarManager.h"

int CHelpers_NetVarManager::Get(const char* const szClass, const char* const szVarName)
{
	ClientClass* pCC = I::BaseClientDLL->GetAllClasses();

	while (pCC)
	{
		if (FNV1A::Hash(szClass) == FNV1A::Hash(pCC->m_pNetworkName))
			return GetOffset(pCC->m_pRecvTable, szVarName);

		pCC = pCC->m_pNext;
	}

	return 0;
}

int CHelpers_NetVarManager::GetOffset(RecvTable* pTable, const char* const szVarName)
{
	int n;
	for (n = 0; n < pTable->m_nProps; n++)
	{
		RecvProp Prop = pTable->m_pProps[n];

		if (FNV1A::Hash(Prop.m_pVarName) == FNV1A::Hash(szVarName))
			return Prop.GetOffset();

		RecvTable* pTable = Prop.GetDataTable();

		if (pTable)
		{
			const int nOff = GetOffset(pTable, szVarName);

			if (nOff)
				return (nOff + Prop.GetOffset());
		}
	}

	return 0;
}