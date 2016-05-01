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

void WoWPlayer::Read(WoWPlayer* pPlayer, void* pObject)
{
    WoWObject::Read(pPlayer, pObject);
    WoWUnit::Read(pPlayer, pObject);
    auto pDescriptor = GetDataPointer(pObject);
    ReadOffsetInto(AddOffset(pDescriptor, DATA_CORPSE_X), &pPlayer->mCorpsePosition.x);
    ReadOffsetInto(AddOffset(pDescriptor, DATA_CORPSE_Y), &pPlayer->mCorpsePosition.y);
    ReadOffsetInto(AddOffset(pDescriptor, DATA_CORPSE_Z), &pPlayer->mCorpsePosition.z);
}

void WoWPlayer::ClickToMove(const Position& destination) const
{
    hadesmem::Write(GetThisProcess(), (void*)0xC4D890, destination.x);
    hadesmem::Write(GetThisProcess(), (void*)0xC4D894, destination.y);
    hadesmem::Write(GetThisProcess(), (void*)0xC4D898, destination.z);
    hadesmem::Write(GetThisProcess(), (void*)0xC4D888, 0x4);
    HADESMEM_DETAIL_TRACE_FORMAT_A("CTM(%f, %f, %f)", destination.x, destination.y, destination.z);
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

bool
WoWPlayer::IsUnitHostile(const WoWUnit& unit) const
{
    return unit.GetFaction() != 14 && GetFaction() != unit.GetFaction();
}
