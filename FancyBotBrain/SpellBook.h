#pragma once

#include "Spells.h"
#include <vector>
#include <unordered_map>

struct DynamicSpellInfo
{
    uint32_t mCooldown;
};

class SpellBook
{
public:
    typedef std::vector<SpellEntry> SpellRankList;

    void Read();

    bool KnowsSpell(const std::string& name) const;

    uint32_t GetRankOfSpell(const std::string& name) const;

    const SpellEntry& GetMaxRankSpell(const std::string& name) const;

    const DynamicSpellInfo& GetDynamicSpellInfo(const std::string& name) const;

    uint32_t GetSpellCooldown(const std::string& name) const;

    bool IsReady(const std::string& name) const;

private:
    uint32_t GetSpellCooldown(uint32_t id);

    std::unordered_map<std::string, SpellRankList> mByName;
    std::unordered_map<uint32_t, DynamicSpellInfo> mDynamicSpellInfo;
};
