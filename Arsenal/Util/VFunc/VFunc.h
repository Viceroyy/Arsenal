#pragma once

#include "../Hook/HooksUtil.h"

class CUtil_VFunc
{
public:
	template<typename T>
	inline T Get(void* inst, const unsigned int index, const unsigned int offset = 0u) {
		return reinterpret_cast<T>(Get_VTable(inst, offset)[index]);
	}

	inline void* Get(void* instance, size_t index)
	{
		const auto vtable = *static_cast<void***>(instance);
		return vtable[index];
	}

private:
	void**& Get_VTable(void* inst, const unsigned int offset = 0u);
	const void** Get_VTable(const void* inst, const unsigned int offset = 0u);
};

namespace U { inline CUtil_VFunc VFunc; }