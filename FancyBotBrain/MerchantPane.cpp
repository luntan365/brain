#include "MerchantPane.h"

#include "WowOffsets.h"


MerchantPane::MerchantPane()
{
}


void MerchantPane::Read()
{
    mItems.clear();
    ReadOffsetInto(STATIC_MERCHANT_OPEN, &mOpen);
    if (!mOpen)
    {
        return;
    }
    ReadOffsetInto(STATIC_MERCHANT_ITEMS_COUNT, &mItemCount);
    for (uint32_t i = 0; i < mItemCount; i++)
    {
        auto address = STATIC_MERCHANT_ITEMS_START + sizeof(VendorItem) * i;
        VendorItem item;
        ReadOffsetItem((void*)address, &item);
        mItems.emplace_back(item);
    }
}

bool MerchantPane::IsOpen() const
{
    return mOpen;
}

bool MerchantPane::CanRepair() const
{
    // TODO LUA
    return false; 
}

std::vector<VendorItem> MerchantPane::ListItems() const
{
    return mItems;
}

concurrency::task<void> 
MerchantPane::BuyItem(uint32_t index, uint32_t quantity)
{
    return concurrency::task_from_result();
}
