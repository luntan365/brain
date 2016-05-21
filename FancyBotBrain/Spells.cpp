#include "Spells.h"

#include "Global.h"
#include <hadesmem/read.hpp>
#include <unordered_map>

std::unordered_map<uint32_t, SpellEntry> sEntries;

void FetchSpellEntry(uint32_t id)
{
    static const uintptr_t spellDB = 0xC0D788;
    static const uintptr_t spellDBTotalRowsAddr = 0xC0D78C;
    auto rows = hadesmem::Read<uint32_t>(
        GetThisProcess(),
        (void*)spellDBTotalRowsAddr
    );
    if (id >= rows)
    {
        sEntries.emplace(id, SpellEntry());
        return;
    }
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
        sEntries.emplace(id, SpellEntry());
        return;
    }
    sEntries.emplace(
        id, 
        hadesmem::Read<SpellEntry>(GetThisProcess(), (void*)rowAddr)
    );
}

const SpellEntry& GetSpellEntry(uint32_t id)
{
    if (sEntries.find(id) == sEntries.end())
    {
        FetchSpellEntry(id);
    }
    return sEntries.at(id);
}

std::string GetSpellName(uint32_t id)
{
    const auto& entry = GetSpellEntry(id);
    if (entry.spellName[0] != nullptr)
    {
        return std::string(entry.spellName[0]);
    }
    else
    {
        return "";
    }
}
