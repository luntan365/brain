#include "WoWInventory.h"

#include <numeric>

void WoWInventory::AddBag(uint32_t bagIndex, const WoWContainer& container)
{
    mBags[bagIndex] = container;
}

void WoWInventory::AddBag(uint32_t bagIdx, uint64_t guid)
{
    auto maybeObj = WoWObject::GetByGUID(guid);
    if (maybeObj)
    {
        AddBag(bagIdx, WoWContainer::Read(maybeObj->GetAddress()));
    }
    else
    {
        AddBag(bagIdx, WoWContainer());
    }
}

void WoWInventory::SetBackpackItem(uint8_t index, uint64_t itemGuid)
{
    auto maybeObj = WoWObject::GetByGUID(itemGuid);
    if (!maybeObj)
    {
        mBackpack[index] = WoWItem();
    }
    else
    {
        mBackpack[index] = WoWItem::Read(maybeObj->GetAddress());
    }
}

void WoWInventory::Clear()
{
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
        [](uint32_t acc, const WoWItem& item) {
            return (item.GetGUID() == 0 ? acc + 1 : acc);
        }
    );

    return emptyBagSlots + emptyBackpackSlots;
}

bool WoWInventory::IsFull() const
{
    return EmptySlotCount() == 0;
}

boost::optional<WoWItem>
WoWInventory::GetItemByName(const std::string& name) const
{
    boost::optional<WoWItem> output = boost::none;
    ForEachItem([&name, &output] (const WoWItem& item, uint8_t, uint8_t) {
        if (item.GetName() == name)
        {
            output = boost::make_optional(item);
            return false;
        }
        return true;
    });
    return output;
}

uint32_t
WoWInventory::GetItemCountByName(const std::string& name) const
{
    uint32_t count = 0;
    ForEachItem([&count, &name] (const WoWItem& item, uint8_t, uint8_t) {
        if (item.GetName() == name)
        {
            count += item.GetStackSize();
        }
        return true;
    });
    return count;
}

void WoWInventory::ForEachItem(ItemVisitor f) const
{
    for (uint32_t slotIndex = 0; slotIndex < mBackpack.size(); slotIndex++)
    {
        const auto& item = mBackpack[slotIndex];
        if (item.GetGUID() != 0)
        {
            bool keepGoing = f(item, 0, slotIndex);
            if (!keepGoing)
            {
                return;
            }
        }
    }
    for (uint32_t bagIndex = 0; bagIndex < mBags.size(); bagIndex++)
    {
        const auto& bag = mBags[bagIndex];
        for (const auto& k_v : bag.mSlots)
        {
            const auto slotIndex = k_v.first;
            const auto& item = k_v.second;
            bool keepGoing = f(item, bagIndex, slotIndex);
            if (!keepGoing)
            {
                return;
            }
        }
    }
}
