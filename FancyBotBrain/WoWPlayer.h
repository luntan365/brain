#pragma once

#include "WoWUnit.h"

class WoWPlayer : public WoWUnit
{
public:
    static const uintptr_t DATA_CORPSE_X = 0x24;
    static const uintptr_t DATA_CORPSE_Y = 0x28;
    static const uintptr_t DATA_CORPSE_Z = 0x2C;

    static WoWPlayer Read(void* pObject);

    static void Read(WoWPlayer* pPlayer, void* address);

    static void ClickToMove(const Position& destination);

    static void CastSpellByName(const std::string& name);

protected:
    Position mCorpsePosition;
};
