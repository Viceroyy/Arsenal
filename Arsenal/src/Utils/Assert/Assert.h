#pragma once
#include "../Macros/Macros.h"

#define FUNCSIG __FUNCSIG__
#define STRINGIZE(x) STRINGIZE2(x)
#define STRINGIZE2(x) #x
#define LINE_STRING STRINGIZE(__LINE__)

#define Validate(x) AssertCustom(x, std::format("Interfaces failed to initialize {}", #x).c_str())
#define XASSERT(x) if (x) MessageBoxA(0, #x, "FATAL ERROR", MB_ICONERROR)
