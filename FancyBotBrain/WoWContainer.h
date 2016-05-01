#pragma once

#include "WoWItem.h"
#include "WoWObject.h"

#include <unordered_map>

class WoWContainer : public WoWObject
{
public:
    static WoWContainer Read(void* pObject);

    static void Read(WoWContainer* pObject, void* address);

    bool IsFull() const;

    uint32_t EmptySlotCount() const;

protected:
    uint32_t mNumSlots;
    std::unordered_map<uint32_t, uint64_t> mSlots;
};
