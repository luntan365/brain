#pragma once

#include "EndSceneManager.h"
#include "WoWUnit.h"

class WoWPlayer : public WoWUnit
{
public:
    static const uintptr_t DATA_CORPSE_X = 0x24;
    static const uintptr_t DATA_CORPSE_Y = 0x28;
    static const uintptr_t DATA_CORPSE_Z = 0x2C;

    static WoWPlayer Read(void* pObject);

    static void Read(WoWPlayer* pPlayer, void* address);

    void ClickToMove(const Position& destination) const;

    concurrency::task<void> CastSpellByName(const std::string& name) const;

    concurrency::task<void> SetTarget(uint64_t targetGuid) const;

    concurrency::task<void> Turn(const Position& position) const;

    concurrency::task<void> SendMovementUpdate(uint32_t opcode) const;

    bool IsUnitHostile(const WoWUnit& unit) const;

protected:
    Position mCorpsePosition;
};
