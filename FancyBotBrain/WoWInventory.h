#pragma once

#include "WoWContainer.h"

#include <array>

class WoWInventory
{
public:
    void AddBag(uint64_t guid);

    void AddBag(const WoWContainer& container);

    void SetBackpackItem(uint8_t index, uint64_t itemGuid);

    void Clear();

    uint32_t EmptySlotCount() const;

    bool IsFull() const;

    boost::optional<WoWItem> GetItemByName(const std::string& name) const;

    uint32_t GetItemCountByName(const std::string& name) const;

private:
    std::vector<WoWContainer> mBags;
    std::array<WoWItem, 16> mBackpack;
};
