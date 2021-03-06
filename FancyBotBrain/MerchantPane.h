#pragma once

#include <cstdint>
#include <ppltasks.h>
#include <vector>

class GameState;

struct VendorItem
{
    uint32_t mIndex;
    uint32_t mItemId;
    uint32_t mUnknown1;
    uint32_t mUnknown2;
    uint32_t mCost;
    uint32_t mUnknown3;
    uint32_t mStackCount; 
};

class MerchantPane
{
public:
    MerchantPane(GameState* pGameState);

    void Read();

    bool IsOpen() const;

    bool CanRepair() const;

    std::vector<VendorItem> ListItems() const;

    concurrency::task<void> BuyItem(uint32_t index, uint32_t quantity) const;

    concurrency::task<void> RepairAll() const;

private:
    GameState* mpGameState;
    bool mOpen;
    bool mCanRepair;
    uint32_t mItemCount;
    std::vector<VendorItem> mItems;
};
