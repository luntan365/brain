#pragma once

#include "EndSceneManager.h"
#include "WoWObject.h"

struct ItemCacheEntry
{
    uint32_t id;
    int32_t pad_0;
	uint32_t nameAddress;
    uint8_t pad[460];

	// [FieldOffset(28)]
	// public int Quality;
    //
	// [FieldOffset(36)]
	// public int BuyPrice;
    //
	// [FieldOffset(40)]
	// public int SellPrice;
    //
	// [FieldOffset(44)]
	// public int InventoryType;
    //
	// [FieldOffset(56)]
	// public int ItemLevel;
    //
	// [FieldOffset(60)]
	// public int RequiredLevel;
    //
	// [FieldOffset(92)]
	// public int MaxCount;
    //
	// [FieldOffset(96)]
	// public int MaxStackCount;
    //
	// [FieldOffset(100)]
	// public int ContainerSlots;
    //
	// [FieldOffset(104)]
	// public ItemCacheEntry._ItemStats ItemStats;
    //
	// [FieldOffset(184)]
	// public ItemCacheEntry._Damage Damage;
    //
	// [FieldOffset(244)]
	// public int Armor;
    //
	// [FieldOffset(276)]
	// public int AmmoType;
    //
	// [FieldOffset(452)]
	// public int MaxDurability;
    //
	// [FieldOffset(464)]
	// public int BagFamily;
};

class WoWItem : public WoWObject
{
public:
    static WoWItem Read(void* pObject);

    static void Read(WoWItem* pItem, void* pObject);

    const std::string& GetName() const;

    uint32_t GetId() const;

    concurrency::task<void> Use() const;

private:
    void* GetItemCacheAddress(uint32_t itemId);

private:
    uint32_t mId;
    uint64_t mOwner;
    uint64_t mContained;
    uint64_t mCreator;
    uint64_t mGiftCreator;
    uint32_t mStackCount;
    uint32_t mDuration;
    uint32_t mSpellCharges;
    uint32_t mFlags;
    uint32_t mEnchantment;
    uint32_t mPropertySeed;
    uint32_t mRandomPropertiesId;
    uint32_t mItemTextId;
    uint32_t mDurability;
    uint32_t mMaxDurability;

    ItemCacheEntry mItemCache;
    std::string mName;

};
