#pragma once

#include "WoWContainer.h"

#include <array>

class WoWInventory
{
public:
    typedef std::function<bool (const WoWItem&, uint8_t, uint8_t)> ItemVisitor;

public:
    void AddBag(uint32_t index, uint64_t guid);

    void AddBag(uint32_t index, const WoWContainer& container);

    void SetBackpackItem(uint8_t index, uint64_t itemGuid);

    void Clear();

    uint32_t EmptySlotCount() const;

    bool IsFull() const;

    boost::optional<WoWItem> GetItemByName(const std::string& name) const;

    uint32_t GetItemCountByName(const std::string& name) const;

    void ForEachItem(ItemVisitor f) const;

private:
    std::array<WoWContainer, 4> mBags;
    std::array<WoWItem, 16> mBackpack;
};
