#pragma once

#include "../../Interfaces/IMatSystemSurface.h"
#include "../../Interfaces/IInputSystem.h"
#include "../../Interfaces/IClientState.h"
#include "../../Interfaces/IClientModeShared.h"
#include "../../Interfaces/IInput.h"
#include "../../Interfaces/IMaterialSystem.h"
#include "../../Interfaces/IHudChat.h"
#include "../../Interfaces/IVModelInfo.h"

class CHelpers_NetVarManager
{
public:
	int GetOffset(RecvTable* pTable, const char* szNetVar);
	int GetNetVar(const char* szClass, const char* szNetVar);

	RecvProp* GetProp(RecvTable* pTable, const char* szNetVar);
	RecvProp* GetNetProp(const char* szClass, const char* szNetVar);
};

namespace U { inline CHelpers_NetVarManager NetVars; }

#define NETVAR(_name, type, table, name) inline type& _name() \
{ \
	static int nOffset = U::NetVars.GetNetVar(table, name); \
	return *reinterpret_cast<type*>(uintptr_t(this) + nOffset); \
}

#define NETVAR_OFF(_name, type, table, name, offset) inline type& _name() \
{ \
	static int nOffset = U::NetVars.GetNetVar(table, name) + offset; \
	return *reinterpret_cast<type*>(uintptr_t(this) + nOffset); \
}

#define NETVAR_ARRAY(_name, type, table, name) inline type& _name(int iIndex) \
{ \
	static int nOffset = U::NetVars.GetNetVar(table, name); \
	return *reinterpret_cast<type*>(uintptr_t(this) + nOffset + iIndex * sizeof(type)); \
}

#define NETVAR_ARRAY_OFF(_name, type, table, name, offset) inline type& _name(int iIndex) \
{ \
	static int nOffset = U::NetVars.GetNetVar(table, name) + offset; \
	return *reinterpret_cast<type*>(uintptr_t(this) + nOffset + iIndex * sizeof(type)); \
}