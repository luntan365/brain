#include "WoWPlayer.h"

#include "WowOffsets.h"

WoWPlayer WoWPlayer::Read(PVOID pObject)
{
    WoWPlayer player;
    Read(&player, pObject);
    return player;
}

void WoWPlayer::Read(WoWPlayer* pPlayer, PVOID pObject)
{
    WoWObject::Read(pPlayer, pObject);
    WoWUnit::Read(pPlayer, pObject);
    auto pDescriptor = GetDataPointer(pObject);
    ReadOffsetInto(AddOffset(pDescriptor, DATA_CORPSE_X), &pPlayer->mCorpsePosition.x);
    ReadOffsetInto(AddOffset(pDescriptor, DATA_CORPSE_Y), &pPlayer->mCorpsePosition.y);
    ReadOffsetInto(AddOffset(pDescriptor, DATA_CORPSE_Z), &pPlayer->mCorpsePosition.z);
}
