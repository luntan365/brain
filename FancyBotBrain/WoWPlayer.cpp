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

void WoWPlayer::ClickToMove(const Position& destination)
{
    //milliseconds currentTimeMs = duration_cast<milliseconds>(
        //system_clock::now().time_since_epoch()
    //);
    hadesmem::Write(GetThisProcess(), (void*)0xC4D890, destination.x);
    hadesmem::Write(GetThisProcess(), (void*)0xC4D894, destination.y);
    hadesmem::Write(GetThisProcess(), (void*)0xC4D898, destination.z);
    //hadesmem::Write(GetThisProcess(), (void*)0xC4DA68, currentTimeMs);
    hadesmem::Write(GetThisProcess(), (void*)0xC4D888, 0x4);
    HADESMEM_DETAIL_TRACE_FORMAT_A("CTM(%f, %f, %f)", destination.x, destination.y, destination.z);
}

void ExecuteLua(const std::string& script)
{
    typedef void (__fastcall *FrameScript__Execute)(const char*);
    ((FrameScript__Execute)0x704CD0)(script.c_str());
}

void WoWPlayer::CastSpellByName(const std::string& name)
{
    ExecuteLua("CastSpellByName('" + name + "')");
}
