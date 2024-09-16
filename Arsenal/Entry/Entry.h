#pragma once
#include "../SDK/SDK.h"

class CGlobal_Entry
{
public:
	void Load();
	void Unload();

	bool bUnload = false;
};

namespace U { inline CGlobal_Entry Entry; }