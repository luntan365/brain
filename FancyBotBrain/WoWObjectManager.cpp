#include "stdafx.h"
#include "WoWObjectManager.h"
#include "WowOffsets.h"

WoWObjectManager::WoWObjectManager()
	: mObjects()
{
}


WoWObjectManager::~WoWObjectManager()
{
}

bool __fastcall ObjectIter(uint64_t guid, void* arg)
{
    auto pObjectManager = (WoWObjectManager*)arg;
	pObjectManager->NewObject(guid);
    return true;
}

void
WoWObjectManager::Read(WoWObjectManager* pManager, void* basePtr)
{
    pManager->ClearObjects();
    auto fn = (ObjectManagerEnumVisibleObjects)EnumVisableObjectsAddress;
    fn(&ObjectIter, (void*)pManager);
}

void
WoWObjectManager::ReadObject(void* basePtr)
{
    auto obj = WoWObject::Read(basePtr);
    mObjects.emplace(obj.GetGUID(), std::move(obj));
}

boost::optional<WoWObject>
WoWObjectManager::GetObjectByGuid(uint64_t guid)
{
    return WoWObject::GetByGUID(guid);
}

void
WoWObjectManager::NewObject(uint64_t guid)
{
    auto maybeObject = GetObjectByGuid(guid);
    if (!maybeObject)
    {
        return;
    }
    auto& object = *maybeObject;

    switch (object.GetType())
    {
        case OT_NONE:
        case OT_ITEM:
        case OT_CONTAINER:
        case OT_GAMEOBJ:
        case OT_DYNOBJ:
        case OT_CORPSE:
            break;

        case OT_UNIT:
            NewUnit(object);
            return;

        case OT_PLAYER:
            NewPlayer(object);
            return;
    }

    mObjects.emplace(guid, *maybeObject);
}

void
WoWObjectManager::NewPlayer(const WoWObject& object)
{
    if (object.GetGUID() == GetActivePlayerGUID())
    {
        WoWPlayer::Read(&mPlayer, object.GetAddress());
    }
    else
    {
        auto player = WoWPlayer::Read(object.GetAddress());
        mUnits.emplace(player.GetGUID(), player);
    }
}

void
WoWObjectManager::NewUnit(const WoWObject& object)
{
    auto unit = WoWUnit::Read(object.GetAddress());
    mUnits.emplace(unit.GetGUID(), unit);
}

void
WoWObjectManager::ClearObjects()
{
    mObjects.clear();
    mUnits.clear();
}

uint64_t
WoWObjectManager::GetActivePlayerGUID()
{
    return ((ObjectManagerGetActivePlayerGUID)GetActivePlayerGUIDAddress)();
}

const WoWPlayer&
WoWObjectManager::GetPlayer()
{
    return mPlayer;
}

const WoWObjectManager::ObjectMap&
WoWObjectManager::Objects() const
{
    return mObjects;
}

const WoWObjectManager::UnitMap&
WoWObjectManager::Units() const
{
    return mUnits;
}
