#pragma once

#include "WoWObject.h"

class WoWItem : public WoWObject
{
public:
    static WoWItem Read(void* pObject);

    static void Read(WoWItem* pItem, void* pObject);

private:
    uint64_t mOwner;
    uint64_t mContained;
    uint64_t mCreator;
    uint64_t mGiftCreator;
    uint32_t mStackCount;
    uint32_t mDuration;
    uint32_t mSpellCharges;
    uint32_t mFlags;
    uint32_t mEnchantment;
    uint32_t mPropertySeed;
    uint32_t mRandomPropertiesId;
    uint32_t mItemTextId;
    uint32_t mDurability;
    uint32_t mMaxDurability;
};
