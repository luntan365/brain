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
}
