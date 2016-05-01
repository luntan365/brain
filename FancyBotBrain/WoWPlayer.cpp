#define _USE_MATH_DEFINES
#include "WoWPlayer.h"

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
}

void WoWPlayer::Reset()
{
    mInventory.Clear();
}

concurrency::task<void>
WoWPlayer::CTM(uint64_t targetGuid, const Position& destination, uint32_t flag) const
{
    typedef bool (__thiscall *ClickToMoveFn)
        (void*, const uint32_t, const uint64_t*, const void*, float);
    auto address = GetAddress();
    return EndSceneManager::Instance()
        .Execute([address, flag, destination, targetGuid] {
            auto fn = ((ClickToMoveFn)0x611130);
            fn(address, flag, &targetGuid, (const void*)&destination.x, 0.0);
        });
}

concurrency::task<void>
WoWPlayer::ClickToMove(const Position& destination) const
{
    return CTM(0, destination, 0x4);
}

concurrency::task<void>
WoWPlayer::InteractWith(const WoWUnit& unit) const
{
    return CTM(unit.GetGUID(), unit.GetPosition(), 0x5);
}

concurrency::task<void>
WoWPlayer::Loot(const WoWUnit& unit) const
{
    return CTM(unit.GetGUID(), unit.GetPosition(), 0x6);
}

concurrency::task<void>
ExecuteLua(const std::string& script)
{
    return EndSceneManager::Instance().Execute([script] {
        typedef void (__fastcall *FrameScript__Execute)(const char*);
        ((FrameScript__Execute)0x704CD0)(script.c_str());
    });
}

concurrency::task<std::string>
GetLuaResult(const std::string& script, const std::string& argument)
{
    return ExecuteLua(script).then([argument] {
        typedef char* (__fastcall *GetText)(const char*);
        char* result = ((GetText)0x703BF0)(argument.c_str());
        return std::string(result);
    });
}

concurrency::task<void>
WoWPlayer::CastSpellByName(const std::string& name) const
{
    return ExecuteLua("CastSpellByName('" + name + "')");
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
WoWPlayer::Turn(const Position& target) const
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

bool
WoWPlayer::IsUnitHostile(const WoWUnit& unit) const
{
    return unit.GetFaction() != 14 && GetFaction() != unit.GetFaction();
}

bool
WoWPlayer::InRangeOf(const WoWUnit& unit, float distance) const
{
    return GetPosition().Distance(unit.GetPosition()) <= distance;
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
