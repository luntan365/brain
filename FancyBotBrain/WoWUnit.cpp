#include "WoWUnit.h"

#include "Spells.h"
#include "WowOffsets.h"

WoWUnit WoWUnit::Read(void* pObject)
{
    WoWUnit unit;
    Read(&unit, pObject);
    return unit;
}

void WoWUnit::Read(WoWUnit* pUnit, void* pObject)
{
    WoWObject::Read(pUnit, pObject);
    auto pDescriptor = GetDataPointer(pObject);
    ReadOffsetInto(AddOffset(pDescriptor, DATA_HEALTH), &pUnit->mHealth);
    ReadOffsetInto(AddOffset(pDescriptor, DATA_MAX_HEALTH), &pUnit->mMaxHealth);
    ReadOffsetInto(AddOffset(pDescriptor, DATA_FACTION_ID), &pUnit->mFactionId);
    ReadOffsetInto(AddOffset(pDescriptor, DATA_DYNAMIC_FLAGS), &pUnit->mDynamicFlags);
    ReadOffsetInto(AddOffset(pDescriptor, DATA_MOVEMENT_FLAGS), &pUnit->mMovementState);
    ReadOffsetInto(AddOffset(pDescriptor, DATA_MANA), &pUnit->mMana);
    ReadOffsetInto(AddOffset(pDescriptor, DATA_MAX_MANA), &pUnit->mMaxMana);
    ReadOffsetInto(AddOffset(pDescriptor, DATA_RAGE), &pUnit->mRage);
    ReadOffsetInto(AddOffset(pDescriptor, DATA_TARGET_GUID), &pUnit->mTargetGUID);
    ReadOffsetInto(AddOffset(pDescriptor, UnitFields::UNIT_FIELD_FLAGS), &pUnit->mFlags);
    ReadOffsetInto(AddOffset(pUnit->GetAddress(), 0xC8C), &pUnit->mCastingSpell);
    ReadOffsetInto(AddOffset(pDescriptor, UnitFields::UNIT_CHANNEL_SPELL), &pUnit->mChannelingSpell);

    for (auto i = 0; i < 10; i++)
    {
        uint32_t aura = 0;
        auto offset = UnitFields::UNIT_FIELD_AURA + i * 4;
        ReadOffsetInto(AddOffset(pDescriptor, offset), &aura);
        if (aura != 0)
        {
            pUnit->mBuffs.push_back(aura);
        }
    }

    for (auto i = 0; i < 16; i++)
    {
        uint32_t aura = 0;
        auto offset = UnitFields::UNIT_FIELD_AURA_DEBUFF + i * 4;
        ReadOffsetInto(AddOffset(pDescriptor, offset), &aura);
        if (aura != 0)
        {
            pUnit->mDebuffs.push_back(aura);
        }
    }

}

bool WoWUnit::IsAlive() const
{
    return mHealth > 0;
}

bool WoWUnit::IsLootable() const
{
    return (mDynamicFlags & 0x1) == 0x1;
}

uint32_t WoWUnit::GetFaction() const
{
    return mFactionId;
}

bool WoWUnit::IsInCombat() const
{
    return CHECK_BITMASK(mFlags, UNIT_FLAG_IN_COMBAT);
}

bool WoWUnit::HasAura(const std::string& aura) const
{
    return std::any_of(
        mBuffs.begin(),
        mBuffs.end(),
        [&aura](uint32_t buff) {
            return GetSpellName(buff) == aura;
        });
}

bool WoWUnit::HasDebuff(const std::string& debuff) const
{
    return std::any_of(
        mDebuffs.begin(),
        mDebuffs.end(),
        [&debuff](uint32_t buff) {
            return GetSpellName(buff) == debuff;
        });
}

float WoWUnit::HealthPercent() const
{
    return (float)mHealth / (float)mMaxHealth * 100.0;
}

float WoWUnit::ManaPercent() const
{
    return (float)mMana / (float)mMaxMana * 100.0;
}

bool WoWUnit::TappedByMe() const
{
    return mDynamicFlags <= 2;
}

bool WoWUnit::IsCasting() const
{
    if (mCastingSpell == 0)
    {
        return false;
    }
    const auto& name = GetSpellName(mCastingSpell);
    return (name != "Heroic Strike" && name != "Maul");
}

bool WoWUnit::IsChanneling() const
{
    return mChannelingSpell != 0;
}
