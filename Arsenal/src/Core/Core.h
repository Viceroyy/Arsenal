#pragma once
#include "../Utils/Macros/Macros.h"
#include "../Utils/ExceptionHandler/ExceptionHandler.h"

#include <sstream>
#include <vector>

class CCore
{
private:
	bool m_bFailed = false;
	bool m_bFailed2 = false;
	ULONGLONG m_nLoadStartTime = 0;
	std::vector<std::string> m_vFailReasons;

public:
	void Load();
	void Loop();
	void Unload();

	void AppendFailText(const char* sMessage);
	void LogFailText();

	bool m_bUnload = false;
};

ADD_FEATURE_CUSTOM(CCore, Core, U);
