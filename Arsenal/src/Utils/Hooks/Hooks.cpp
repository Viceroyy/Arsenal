#include "Hooks.h"

CHook::CHook(const std::string& sName, void* pInitFunc)
{
	m_pInitFunc = pInitFunc;
	U::Hooks.m_mHooks[sName] = this;
}

bool CHooks::Initialize()
{
	MH_Initialize();

	for (auto& [_, pHook] : m_mHooks)
		reinterpret_cast<void(__cdecl*)()>(pHook->m_pInitFunc)();

	m_bFailed = MH_EnableHook(MH_ALL_HOOKS) != MH_OK;
	return !m_bFailed;
}

bool CHooks::Unload()
{
	m_bFailed = MH_Uninitialize() != MH_OK;
	return !m_bFailed;
}
