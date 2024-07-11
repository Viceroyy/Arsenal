#pragma once
#include "../SDK/SDK.h"

class CGlobal_Entry
{
public:
	void Load();
	void Unload();
};

namespace G { inline CGlobal_Entry Entry; }