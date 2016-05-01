#pragma once

#include "WoWObject.h"

class WoWUnit : public WoWObject
{
public:
    static const uintptr_t DATA_HEALTH = 0x58;
    static const uintptr_t DATA_MAX_HEALTH = 0x70;
    static const uintptr_t DATA_FACTION_ID = 0x8C;
    static const uintptr_t DATA_DYNAMIC_FLAGS = 0x23C;
    static const uintptr_t DATA_MOVEMENT_FLAGS = 0x9E8;
    static const uintptr_t DATA_MANA = 0x5C;
    static const uintptr_t DATA_MAX_MANA = 0x74;
    static const uintptr_t DATA_RAGE = 0x60;
    static const uintptr_t DATA_TARGET_GUID = 0x40;

    static WoWUnit Read(void* pObject);

    static void Read(WoWUnit* pUnit, void* address);

    bool IsAlive() const;

    uint32_t GetFaction() const;

protected:
    int32_t mHealth;
    int32_t mMaxHealth;
    int32_t mMana;
    int32_t mMaxMana;
    int32_t mRage;
    int32_t mFactionId;
    uint64_t mTargetGUID;
    uint32_t mDynamicFlags;
    uint32_t mMovementState;
};
