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
    for (const auto& item : mBackpack)
    {
        if (item.GetName() == name)
        {
            return boost::make_optional(item);
        }
    }
    for (const auto& bag : mBags)
    {
        for (const auto& k_v : bag.mSlots)
        {
            const auto& item = k_v.second;
            if (item.GetName() == name)
            {
                return boost::make_optional(item);
            }
        }
    }
    return boost::none;
}

uint32_t
WoWInventory::GetItemCountByName(const std::string& name) const
{
    uint32_t count = 0;
    for (const auto& item : mBackpack)
    {
        if (item.GetName() == name)
        {
            count += item.GetStackSize();
        }
    }
    for (const auto& bag : mBags)
    {
        for (const auto& k_v : bag.mSlots)
        {
            const auto& item = k_v.second;
            if (item.GetName() == name)
            {
                count += item.GetStackSize();
            }
        }
    }
    return count;
}
