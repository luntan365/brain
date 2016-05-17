#include "Lua.h"
#include "EndSceneManager.h"

void
ExecuteLua(const std::string& script)
{
    typedef void (__fastcall *FrameScript__Execute)(const char*, uint32_t);
    ((FrameScript__Execute)0x704CD0)(script.c_str(), 0);
}

std::string
GetLuaResult(const std::string& script, const std::string& argument)
{
    ExecuteLua(script);
    typedef char* (__fastcall *GetText)(const char*, uint32_t, uint32_t);
    char* result = ((GetText)0x703BF0)(argument.c_str(), 0xFFFFFFFF, 0);
    return std::string(result);
}

concurrency::task<void>
AsyncExecuteLua(const std::string& script)
{
    return EndSceneManager::Instance().Execute([script] {
        ExecuteLua(script);
    });
}

concurrency::task<std::string>
AsyncGetLuaResult(const std::string& script, const std::string& argument)
{
    return EndSceneManager::Instance().Execute([script, argument] {
        return GetLuaResult(script, argument);
    });
}

void RegisterLuaFunctions()
{
    for (const auto& f : LUA_FUNCTION_DEFS)
    {
        ExecuteLua(f);
    }
}
