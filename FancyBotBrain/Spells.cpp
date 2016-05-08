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
        auto spellDBAddr = hadesmem::Read<uint32_t>(
            GetThisProcess(),
            (void*)spellDB
        );
        auto rowOffset = spellDBAddr + id * 4;
        auto rowAddr = hadesmem::Read<uint32_t>(
            GetThisProcess(),
            (void*)rowOffset
        );
        if (rowAddr == 0)
        {
            return "";
        }
        auto nameAddrP = rowAddr + 480;
        auto nameAddr = hadesmem::Read<uint32_t>(
            GetThisProcess(),
            (void*)nameAddrP
        );
        return hadesmem::ReadString<char>(
            GetThisProcess(),
            (void*)nameAddr
        );
    }
}
