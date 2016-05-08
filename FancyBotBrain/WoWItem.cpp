#include "WoWItem.h"

#include "WowOffsets.h"

WoWItem WoWItem::Read(void* pObject)
{
    WoWItem item;
    Read(&item, pObject);
    return item;
}

void WoWItem::Read(WoWItem* pItem, void* pObject)
{
    WoWObject::Read(pItem, pObject);
    auto pDescriptor = GetDataPointer(pObject);
	ReadOffsetInto(pDescriptor, ObjectFields::OBJECT_FIELD_ID, &pItem->mId);
	ReadOffsetInto(pDescriptor, ItemFields::ITEM_FIELD_OWNER, &pItem->mOwner);
	ReadOffsetInto(pDescriptor, ItemFields::ITEM_FIELD_CONTAINED, &pItem->mContained);
	ReadOffsetInto(pDescriptor, ItemFields::ITEM_FIELD_CREATOR, &pItem->mCreator);
	ReadOffsetInto(pDescriptor, ItemFields::ITEM_FIELD_GIFTCREATOR, &pItem->mGiftCreator);
	ReadOffsetInto(pDescriptor, ItemFields::ITEM_FIELD_STACK_COUNT, &pItem->mStackCount);
	ReadOffsetInto(pDescriptor, ItemFields::ITEM_FIELD_DURATION, &pItem->mDuration);
	ReadOffsetInto(pDescriptor, ItemFields::ITEM_FIELD_SPELL_CHARGES, &pItem->mSpellCharges);
	ReadOffsetInto(pDescriptor, ItemFields::ITEM_FIELD_FLAGS, &pItem->mFlags);
	ReadOffsetInto(pDescriptor, ItemFields::ITEM_FIELD_ENCHANTMENT, &pItem->mEnchantment);
	ReadOffsetInto(pDescriptor, ItemFields::ITEM_FIELD_PROPERTY_SEED, &pItem->mPropertySeed);
	ReadOffsetInto(pDescriptor, ItemFields::ITEM_FIELD_RANDOM_PROPERTIES_ID, &pItem->mRandomPropertiesId);
	ReadOffsetInto(pDescriptor, ItemFields::ITEM_FIELD_ITEM_TEXT_ID, &pItem->mItemTextId);
	ReadOffsetInto(pDescriptor, ItemFields::ITEM_FIELD_DURABILITY, &pItem->mDurability);
	ReadOffsetInto(pDescriptor, ItemFields::ITEM_FIELD_MAXDURABILITY, &pItem->mMaxDurability);

    void* pItemCache = pItem->GetItemCacheAddress(pItem->mId);
	pItem->mItemCache = hadesmem::Read<ItemCacheEntry>(GetThisProcess(), pItemCache);

    pItem->mName = hadesmem::ReadString<char>(
        GetThisProcess(),
        (void*)pItem->mItemCache.nameAddress
    );
}

const std::string&
WoWItem::GetName() const
{
    return mName;
}

void ItemCacheCallback(int32_t)
{
    return;
}

void* WoWItem::GetItemCacheAddress(uint32_t itemId)
{
    uint64_t guid = 0;
    typedef void* (__thiscall *ItemCacheDBGetRowFn)(void*, int32_t, uint64_t*, void*, int32_t, int32_t);
    auto pItemDatabaseCache = (void*)0xC0E2A0;
    return ((ItemCacheDBGetRowFn)0x55BA30)(pItemDatabaseCache, itemId, &guid, &ItemCacheCallback, 0, 0);
}

uint32_t WoWItem::GetId() const
{
    return mId;
}

concurrency::task<void>
WoWItem::Use() const
{
    static const uintptr_t UseItemAddr = 0x5D8D00;
    typedef void (__thiscall *UseItemFn)(void*, uint32_t*, uint32_t);
    auto address = GetAddress();
    return EndSceneManager::Instance().Execute([address] {
        uint32_t n;
        ((UseItemFn)UseItemAddr)(address, &n, 0);
    });
}
