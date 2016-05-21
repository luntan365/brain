#include "SpellBook.h"

#define NOMINMAX

#include "GameState.h"
#include "WowOffsets.h"
#include <boost/assert.hpp>
#include <limits>

void
SpellBook::Read()
{
    for (int i = 0; i < 1024; i++)
    {
        auto id = ReadOffset<uint32_t>(StaticFields::STATIC_SPELLBOOK, i * 4);
		if (id == 0)
		{
			break;
		}
        const auto& entry = GetSpellEntry(id);
        const auto& name = GetSpellName(id);
        if (mByName.find(name) == mByName.end())
        {
            std::vector<SpellEntry> ranks;
            ranks.push_back(entry);
            mByName.emplace(name, ranks);
        }
        else
        {
            mByName.at(name).emplace_back(entry);
        }
        DynamicSpellInfo info;
        info.mCooldown = GetSpellCooldown(id);
        mDynamicSpellInfo.emplace(id, info);
    }
}

bool
SpellBook::KnowsSpell(const std::string& name) const
{
    return mByName.find(name) != mByName.end();
}

uint32_t
SpellBook::GetRankOfSpell(const std::string& name) const
{
    if (!KnowsSpell(name))
    {
        return 0;
    }
    else
    {
        return mByName.at(name).size();
    }
}

const SpellEntry&
SpellBook::GetMaxRankSpell(const std::string& name) const
{
    BOOST_ASSERT(KnowsSpell(name));
    return mByName.at(name).back();
}

uint32_t
SpellBook::GetSpellCooldown(uint32_t spellId)
{
    typedef bool (__fastcall *GetSpellCooldownFn)(uint32_t, int32_t, int32_t*, int32_t*, int32_t*);
    auto fn = (GetSpellCooldownFn)(StaticFields::STATIC_GET_SPELL_COOLDOWN);
    bool isPet = false;
    int32_t duration = 0;
    int32_t start = 0;
    int32_t isEnabledI = 0;
    fn(
        spellId, 
        isPet, 
        &duration,
        &start,
        &isEnabledI
        );
    bool isEnabled = isEnabledI;
    auto count = GameState::Instance().GetPerformanceCounter();
    auto result = start + duration - count;
    if (isEnabled)
    {
        return std::numeric_limits<uint32_t>::max();
    }
    else if (result > 0)
    {
        return (uint32_t)result;
    }
    else
    {
        return (uint32_t)0;
    }
}

const DynamicSpellInfo&
SpellBook::GetDynamicSpellInfo(const std::string& name) const
{
    if (!KnowsSpell(name))
    {
        return DynamicSpellInfo();
    }
    const auto& spell = GetMaxRankSpell(name);
    return mDynamicSpellInfo.at(spell.id);
}

uint32_t
SpellBook::GetSpellCooldown(const std::string& name) const
{
    return GetDynamicSpellInfo(name).mCooldown;
}

bool
SpellBook::IsReady(const std::string& name) const
{
    return GetSpellCooldown(name) == 0;
}
