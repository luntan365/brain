#include "Spells.h"

#include "Global.h"
#include <hadesmem/read.hpp>

// TODO: maybe memoize
std::string GetSpellName(uint32_t id)
{
    static const uintptr_t spellDB = 0xC0D788;
    static const uintptr_t spellDBTotalRowsAddr = 0xC0D78C;
    auto rows = hadesmem::Read<uint32_t>(
        GetThisProcess(),
        (void*)spellDBTotalRowsAddr
    );
    if (id >= rows)
    {
        return "";
    }
    else
    {
        auto rowOffset = spellDB + id * 4;
        auto rowAddr = hadesmem::Read<uint32_t>(
            GetThisProcess(),
            (void*)rowOffset
        );
        auto nameAddr = rowAddr + 0x480;
        return hadesmem::ReadString<char>(
            GetThisProcess(),
            (void*)nameAddr
        );
    }
}
