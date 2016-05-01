#include "WoWInventory.h"

#include <numeric>

void WoWInventory::AddBag(const WoWContainer& container)
{
    mBags.emplace_back(container);
}

void WoWInventory::AddBag(uint64_t guid)
{
    auto maybeObj = WoWObject::GetByGUID(guid);
    if (!maybeObj)
    {
        return;
    }
    AddBag(WoWContainer::Read(maybeObj->GetAddress()));
}

void WoWInventory::SetBackpackItem(uint8_t index, uint64_t itemGuid)
{
    mBackpack[index] = itemGuid;
}

void WoWInventory::Clear()
{
    mBags.clear();
}

uint32_t WoWInventory::EmptySlotCount() const
{
    auto emptyBagSlots = std::accumulate(
        mBags.begin(),
        mBags.end(),
        0,
        [](uint32_t acc, const WoWContainer& c)
        {
            return acc + c.EmptySlotCount();
        }
    );

    auto emptyBackpackSlots = std::accumulate(
        mBackpack.begin(),
        mBackpack.end(),
        0,
        [](uint32_t acc, uint64_t guid) {
            return (guid == 0 ? acc + 1 : acc);
        }
    );

    return emptyBagSlots + emptyBackpackSlots;
}

bool WoWInventory::IsFull() const
{
    return EmptySlotCount() == 0;
}
