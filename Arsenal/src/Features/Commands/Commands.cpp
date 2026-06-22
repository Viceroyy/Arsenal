#include "Commands.h"

bool CCommands::Run(const std::string& cmd, std::deque<std::string>& args)
{
	if (auto it = CommandMap.find(cmd); it != CommandMap.end())
	{
		it->second(args); // Invoke the command callback directly
		return true;
	}
	return false;
}

void CCommands::Register(const std::string& name, CommandCallback callback)
{
	CommandMap.emplace(name, std::move(callback));
}

std::string join(const std::deque<std::string>& args, const std::string& delimiter = " ")
{
    return std::accumulate(std::next(args.begin()), args.end(), args.front(),
        [&delimiter](const std::string& a, const std::string& b) {
            return a + delimiter + b;
        });
}

void CCommands::Initialize()
{
    Register("setcvar", [](std::deque<std::string>& args)
        {
            if (args.size() < 2)
            {
                I::Cvar->ConsoleColorPrintf({ 255, 255, 255, 255 }, "[Arsenal] Usage: setcvar <cvar> <value>\n");
                return;
            }

            const auto& cvarName = args.front();
            const auto foundCVar = I::Cvar->FindVar(cvarName.c_str());

            if (!foundCVar)
            {
                I::Cvar->ConsoleColorPrintf({ 255, 255, 255, 255 }, "[Arsenal] Could not find %s\n", cvarName.c_str());
                return;
            }

            args.pop_front(); // Remove the cvar name
            std::string newValue = join(args, " ");
            newValue.erase(std::remove(newValue.begin(), newValue.end(), '\"'), newValue.end());
            foundCVar->SetValue(newValue.c_str());

            I::Cvar->ConsoleColorPrintf({ 255, 255, 255, 255 }, "[Arsenal] Set %s to: %s\n", cvarName.c_str(), newValue.c_str());
        });

    Register("getcvar", [](std::deque<std::string>& args)
        {
            if (args.size() != 1)
            {
                I::Cvar->ConsoleColorPrintf({ 255, 255, 255, 255 }, "[Arsenal] Usage: getcvar <cvar>\n");
                return;
            }

            const auto& cvarName = args.front();
            const auto foundCVar = I::Cvar->FindVar(cvarName.c_str());

            if (!foundCVar)
            {
                I::Cvar->ConsoleColorPrintf({ 255, 255, 255, 255 }, "[Arsenal] Could not find %s\n", cvarName.c_str());
                return;
            }

            I::Cvar->ConsoleColorPrintf({ 255, 255, 255, 255 }, "[Arsenal] Value of %s is: %s\n", cvarName.c_str(), foundCVar->GetString());
        });
}