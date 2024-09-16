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
	int Get(const char* const szClass, const char* const szNetVar);

private:
	int GetOffset(RecvTable* pTable, const char* const szNetVar);
};

namespace H { inline CHelpers_NetVarManager NetVar; }

#define NETVAR(_name, type, table, name) inline type &_name() \
{ \
	static const int nOffset = H::NetVar.Get(table, name); \
	return *reinterpret_cast<type *>(reinterpret_cast<DWORD>(this) + nOffset); \
}

#define NETVAR_OFF(_name, type, table, name, offset) inline type &_name() \
{ \
	static const int nOffset = H::NetVar.Get(table, name) + offset; \
	return *reinterpret_cast<type *>(reinterpret_cast<DWORD>(this) + nOffset); \
}