#pragma once
#include "../Macros/Macros.h"
#include <cstdint>
#include <vector>

class CSignature
{
private:
	uintptr_t m_dwVal;
	const char* m_sDLLName;
	const char* m_sSignature;
	int8_t m_nOffset;
	const char* m_sName;

public:
	CSignature(const char* sDLLName, const char* sSignature, int8_t nOffset, const char* sName);

	bool Initialize();

	inline uintptr_t operator()()
	{
		return m_dwVal;
	}

	template <typename T>
	inline T As()
	{
		return reinterpret_cast<T>(m_dwVal);
	}

	template <typename T, typename... Args>
	inline T Call(Args... args) const
	{
		return reinterpret_cast<T(__fastcall*)(Args...)>(m_dwVal)(args...);
	}
};

class CSignatures
{
private:
	std::vector<CSignature*> m_vSignatures = {};
	bool m_bFailed = false;

public:
	bool Initialize();

	inline void AddSignature(CSignature* pSignature)
	{
		m_vSignatures.push_back(pSignature);
	}
};

ADD_FEATURE_CUSTOM(CSignatures, Signatures, U);
