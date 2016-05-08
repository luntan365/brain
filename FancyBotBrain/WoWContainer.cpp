#include "WoWContainer.h"

#include "WowOffsets.h"

WoWContainer WoWContainer::Read(void* pObject)
{
    WoWContainer container;
    Read(&container, pObject);
    return container;
}

void WoWContainer::Read(WoWContainer* pContainer, void* pObject)
{
    WoWObject::Read(pContainer, pObject);
    auto pDescriptor = GetDataPointer(pObject);
	ReadOffsetInto(
        pDescriptor,
        ContainerFields::CONTAINER_FIELD_NUM_SLOTS,
        &pContainer->mNumSlots
    );
    for (uint32_t i = 0; i < pContainer->mNumSlots; i++)
    {
        auto itemOffset = ContainerFields::CONTAINER_FIELD_SLOT_1 + 8 * i;
        uint64_t itemId = 0;
        ReadOffsetInto(
            pDescriptor,
            itemOffset,
            &itemId
        );
        if (itemId == 0)
        {
            continue;
        }
        auto maybeObj = WoWObject::GetByGUID(itemId);
        if (!maybeObj)
        {
            continue;
        }
        pContainer->mSlots.emplace(
            i,
            WoWItem::Read(maybeObj->GetAddress())
        );
    }
}

bool WoWContainer::IsFull() const
{
    return EmptySlotCount() == 0;
}

uint32_t WoWContainer::EmptySlotCount() const
{
    return mNumSlots - mSlots.size();
}
