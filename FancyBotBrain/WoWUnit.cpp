#include "WoWUnit.h"

#include "WowOffsets.h"

WoWUnit WoWUnit::Read(PVOID pObject)
{
    WoWUnit unit;
    Read(&unit, pObject);
    return unit;
}

void WoWUnit::Read(WoWUnit* pUnit, PVOID pObject)
{
    WoWObject::Read(pUnit, pObject);
    auto pDescriptor = GetDataPointer(pObject);
    ReadOffsetInto(AddOffset(pDescriptor, DATA_MAX_HEALTH), &pUnit->mMaxHealth);
    ReadOffsetInto(AddOffset(pDescriptor, DATA_FACTION_ID), &pUnit->mFactionId);
    ReadOffsetInto(AddOffset(pDescriptor, DATA_DYNAMIC_FLAGS), &pUnit->mDynamicFlags);
    ReadOffsetInto(AddOffset(pDescriptor, DATA_MOVEMENT_FLAGS), &pUnit->mMovementState);
    ReadOffsetInto(AddOffset(pDescriptor, DATA_MANA), &pUnit->mMana);
    ReadOffsetInto(AddOffset(pDescriptor, DATA_MAX_MANA), &pUnit->mMaxMana);
    ReadOffsetInto(AddOffset(pDescriptor, DATA_RAGE), &pUnit->mRage);
    ReadOffsetInto(AddOffset(pDescriptor, DATA_TARGET_GUID), &pUnit->mTargetGUID);
}
