#include "MerchantPane.h"

#include "GameState.h"
#include "Lua.h"
#include "WowOffsets.h"


MerchantPane::MerchantPane(GameState* pGameState)
    : mpGameState(pGameState)
{
}


void MerchantPane::Read()
{
    mItems.clear();
    mOpen = mpGameState->ObjectManager().GetPlayer().GetVendorGuid() != 0;
    if (!mOpen)
    {
        return;
    }
    ReadOffsetInto(StaticFields::STATIC_MERCHANT_ITEMS_COUNT, &mItemCount);
    for (uint32_t i = 0; i < mItemCount; i++)
    {
        auto address = 
            StaticFields::STATIC_MERCHANT_ITEMS_START + sizeof(VendorItem) * i;
        VendorItem item;
        ReadOffsetInto((void*)address, &item);
        mItems.emplace_back(item);
    }
    auto canRepairStr = 
        GetLuaResult("_canRepair = CanMerchantRepair()", "_canRepair");
    mCanRepair = canRepairStr == "1";
}

bool MerchantPane::IsOpen() const
{
    return mOpen;
}

bool MerchantPane::CanRepair() const
{
    return mCanRepair; 
}

std::vector<VendorItem> MerchantPane::ListItems() const
{
    return mItems;
}

concurrency::task<void> 
MerchantPane::BuyItem(uint32_t index, uint32_t quantity) const
{
    return concurrency::task_from_result();
}

concurrency::task<void>
MerchantPane::RepairAll() const
{
    return AsyncExecuteLua("RepairAllItems()");
}
