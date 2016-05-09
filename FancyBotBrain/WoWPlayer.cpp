#define _USE_MATH_DEFINES
#include "WoWPlayer.h"

#include "Lua.h"
#include "WowOffsets.h"

#include <hadesmem/write.hpp>
#include <chrono>

using namespace std::chrono;

WoWPlayer WoWPlayer::Read(void* pObject)
{
    WoWPlayer player;
    Read(&player, pObject);
    return player;
}

bool GetIsLooting(void* pObject)
{
    typedef bool (__thiscall *Player_IsLooting)(void*);
    return ((Player_IsLooting)0x006126B0)(pObject);
}

void WoWPlayer::Read(WoWPlayer* pPlayer, void* pObject)
{
    WoWObject::Read(pPlayer, pObject);
    WoWUnit::Read(pPlayer, pObject);
    auto pDescriptor = GetDataPointer(pObject);
    ReadOffsetInto(StaticFields::STATIC_CORPSE_X, &pPlayer->mCorpsePosition.x);
    ReadOffsetInto(StaticFields::STATIC_CORPSE_Y, &pPlayer->mCorpsePosition.y);
    ReadOffsetInto(StaticFields::STATIC_CORPSE_Z, &pPlayer->mCorpsePosition.z);
    ReadOffsetInto(
        StaticFields::STATIC_AUTO_REPEATING_SPELL,
        &pPlayer->mAutoRepeatingSpell
    );

    for (uint8_t backpackIdx = 0; backpackIdx < 16; backpackIdx++)
    {
        uint64_t itemGuid;
        ReadOffsetInto(
            pDescriptor,
            PlayerFields::PLAYER_FIELD_PACK_SLOT_1 + backpackIdx * 8,
            &itemGuid
        );
        pPlayer->mInventory.SetBackpackItem(backpackIdx, itemGuid);
    }

    for (auto bagIdx = 0; bagIdx < 4; bagIdx++)
    {
        uint64_t bagGuid;
        auto offset = PlayerFields::PLAYER_FIELD_INV_SLOT_HEAD
            + (EQUIP_LOC_BAG_1 + bagIdx) * 8;
        ReadOffsetInto(pDescriptor, offset, &bagGuid);
        pPlayer->mInventory.AddBag(bagGuid);
    }

    pPlayer->mIsLooting = GetIsLooting(pObject);
    pPlayer->mIsVendorOpen = GetLuaResult("r = IsVendorOpen()", "r") == "true";
}

void WoWPlayer::Reset()
{
    mInventory.Clear();
	mBuffs.clear();
	mDebuffs.clear();
}

concurrency::task<void>
WoWPlayer::CTM(uint64_t targetGuid, const Vector3& destination, uint32_t flag) const
{
    typedef bool (__thiscall *ClickToMoveFn)
        (void*, const uint32_t, const uint64_t*, const void*, float);
    auto address = GetAddress();
    return EndSceneManager::Instance()
        .Execute([address, flag, destination, targetGuid] {
            auto fn = ((ClickToMoveFn)0x611130);
            auto pDest = (void*)&destination.x;
            fn(address, flag, &targetGuid, pDest, 5.0);
        });
}

concurrency::task<void>
WoWPlayer::ClickToMove(const Vector3& destination) const
{
    return CTM(0, destination, 0x4);
}

concurrency::task<void>
WoWPlayer::InteractWith(const WoWUnit& unit) const
{
    return CTM(unit.GetGUID(), unit.GetPosition(), 0x5);
}

concurrency::task<void>
WoWPlayer::StopMoving() const
{
    return CTM(0, GetPosition(), 0xD);
}

concurrency::task<void>
WoWPlayer::Loot(const WoWUnit& unit) const
{
    typedef void (__thiscall *OnRightClickFn)(void*, bool);
    auto addr = unit.GetAddress();
    return EndSceneManager::Instance()
        .Execute([addr] {
            auto fn = ((OnRightClickFn)0x60BEA0);
            fn(addr, 1);
        });
}

concurrency::task<void>
AsyncExecuteLua(const std::string& script)
{
    return EndSceneManager::Instance().Execute([script] {
        ExecuteLua(script);
    });
}

concurrency::task<std::string>
AsyncGetLuaResult(const std::string& script, const std::string& argument)
{
    return EndSceneManager::Instance().Execute([script, argument] {
        return GetLuaResult(script, argument);
    });
}

concurrency::task<void>
WoWPlayer::Attack() const
{
    return AsyncExecuteLua(
        "if IsCurrentAction('24') == nil "
        "then CastSpellByName('Attack') end"
    );
}

concurrency::task<void>
WoWPlayer::CastSpellByName(const std::string& name) const
{
    return AsyncExecuteLua("CastSpellByName('" + name + "')");
}

concurrency::task<void>
WoWPlayer::SetTarget(const WoWUnit& unit) const
{
    return SetTarget(unit.GetGUID());
}

concurrency::task<void>
WoWPlayer::SetTarget(uint64_t targetGuid) const
{
    typedef void (__stdcall *CGameUI_SetTarget)(uint64_t guid);
    return EndSceneManager::Instance().Execute([targetGuid] {
        ((CGameUI_SetTarget)0x493540)(targetGuid);
    });
}

concurrency::task<void>
WoWPlayer::Turn(const Vector3& target) const
{
    auto cur = GetPosition();
    auto angle = atan2(target.y - cur.y, target.x - cur.x);
    if (angle < 0.0)
    {
        angle += 2 * M_PI;
    }
    else if (angle > 2 * M_PI)
    {
        angle -= 2 * M_PI;
    }
    typedef void (__fastcall *SetFacing)(void*, float);
    auto movementAddr = AddOffset(mAddress, OBJ_MOVEMENT);
    return EndSceneManager::Instance().Execute([this, angle, movementAddr] {
        ((SetFacing)0x7C6F30)(movementAddr, angle);
        return SendMovementUpdate(0xDA);
    });
}

concurrency::task<void>
WoWPlayer::SendMovementUpdate(uint32_t opcode) const
{
    milliseconds currentTimeMs = duration_cast<milliseconds>(
        system_clock::now().time_since_epoch()
    );
    auto timestamp = (uint32_t)currentTimeMs.count();
    typedef void (__thiscall *SendMovementPacket)(void*, uint32_t, uint32_t, uint32_t, uint32_t);
    auto addr = mAddress;
    return EndSceneManager::Instance().Execute([addr, opcode, timestamp] {
        ((SendMovementPacket)0x600A30)(addr, timestamp, opcode, 0, 0);
    });
}

concurrency::task<void>
WoWPlayer::AutoLoot() const
{
    typedef void (*DoAutoLoot)();
    return EndSceneManager::Instance().Execute([] {
        ((DoAutoLoot)0x4C1FA0)();
    });
}

concurrency::task<void>
WoWPlayer::ReleaseSpirit() const
{
    return AsyncExecuteLua("RepopMe()");
}

concurrency::task<void>
WoWPlayer::ReviveAtCorpse() const
{
    return AsyncExecuteLua("RetrieveCorpse()");
}

concurrency::task<void>
WoWPlayer::SellAll() const
{
    return AsyncExecuteLua(
        "for bag = 0,4,1 do "
        "for slot = 1, GetContainerNumSlots(bag), 1 do "
        "local name = GetContainerItemLink(bag,slot); "
        "if name then UseContainerItem(bag,slot) end end end"
    );
}

concurrency::task<void>
WoWPlayer::UseItemByName(const std::string& itemName) const
{
    const auto maybeItem = mInventory.GetItemByName(itemName);
    if (maybeItem)
    {
        return maybeItem->Use();
    }
    return concurrency::task<void>();
}

bool
WoWPlayer::IsUnitHostile(const WoWUnit& unit) const
{
    return unit.GetFaction() != 14 && GetFaction() != unit.GetFaction();
}

bool
WoWPlayer::InRangeOf(const Vector3& v, float distance) const
{
    return GetPosition().Distance(v) <= distance;
}

bool
WoWPlayer::InRangeOf(const WoWUnit& unit, float distance) const
{
    return InRangeOf(unit.GetPosition(), distance);
}

bool
WoWPlayer::IsLooting() const
{
    return mIsLooting;
}

const WoWInventory&
WoWPlayer::GetInventory() const
{
    return mInventory;
}

const Position&
WoWPlayer::GetCorpsePosition() const
{
    return mCorpsePosition;
}

bool
WoWPlayer::IsGhost() const
{
    return mCorpsePosition.x != 0
        || mCorpsePosition.y != 0
        || mCorpsePosition.z != 0;
}

bool
WoWPlayer::IsEating() const
{
    return HasAura("Food");
}

bool
WoWPlayer::IsDrinking() const
{
    return HasAura("Drink");
}

concurrency::task<bool>
WoWPlayer::InLosWith(const Vector3& v) const
{
    const auto& start = GetPosition();
    const auto& end = v;
    return EndSceneManager::Instance().Execute([start, end] {
        Intersection intersection;
        PositionPair startEnd(start, end);
        startEnd.startZ += 2.0;
        startEnd.endZ += 2.0;
        float distance = Position(start).Distance(end);
        typedef uint8_t (__fastcall *IntersectFn)(PositionPair*, float*, Intersection*, uint32_t);
        auto b = ((IntersectFn)0x6AA160)(&startEnd, &distance, &intersection, 0x100111);
        return intersection;
    }).then([] (const Intersection& intersection) {
        return intersection.x == 0.0
            && intersection.y == 0.0
            && intersection.z == 0.0
            && intersection.rotation == 0.0;
    });
}

concurrency::task<bool>
WoWPlayer::InLosWith(const WoWUnit& unit) const
{
    return InLosWith(unit.GetPosition());
}

int32_t WoWPlayer::GetAutoRepeatingSpell() const
{
    return mAutoRepeatingSpell;
}
