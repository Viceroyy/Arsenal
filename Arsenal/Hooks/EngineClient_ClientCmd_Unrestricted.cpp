#include "../SDK/SDK.h"
#include <deque>
#include "../Features/Commands/Commands.h"

void split(const std::string& str, std::deque<std::string>& result)
{
    bool in_quotes = false;
    auto start = str.cbegin();

    for (auto it = str.cbegin(); it != str.cend(); ++it)
    {
        if (*it == '\"')
        {
            in_quotes = !in_quotes;
        }
        else if (*it == ' ' && !in_quotes)
        {
            if (start != it)
                result.emplace_back(start, it);
            start = it + 1;
        }
    }

    if (start != str.cend())
        result.emplace_back(start, str.cend());
}

MAKE_HOOK(EngineClient_ClientCmd_Unrestricted, U::VFunc.Get<void*>(I::EngineClient, 106u), void, __fastcall,
	void* ecx, void* edx, const char* szCmdString)
{
    std::string cmdString = szCmdString;
    std::deque<std::string> cmdArgs;
    split(cmdString, cmdArgs);

    if (!cmdArgs.empty())
    {
        auto cmdName = std::move(cmdArgs.front());
        cmdArgs.pop_front();

        if (F::Commands.Run(cmdName, cmdArgs))
            return;
    }

    CALL_ORIGINAL(ecx, edx, cmdString.c_str());
}