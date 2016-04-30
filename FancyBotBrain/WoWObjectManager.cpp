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
WoWObjectManager::Read(WoWObjectManager* pManager, PVOID basePtr)
{
    pManager->ClearObjects();
    auto fn = (ObjectManagerEnumVisibleObjects)EnumVisableObjectsAddress;
    fn(&ObjectIter, (void*)pManager);
}

void
WoWObjectManager::ReadObject(PVOID basePtr)
{
    auto obj = WoWObject::Read(basePtr);
    mObjects.emplace(obj.GetGUID(), std::move(obj));
}

boost::optional<WoWObject>
WoWObjectManager::GetObject(uint64_t guid)
{
    if (mObjects.find(guid) != mObjects.end())
    {
        return WoWObject::GetByGUID(guid);
    }
    else
    {
        return boost::none;
    }
}

void
WoWObjectManager::NewObject(uint64_t guid)
{
    mObjects.emplace(guid, WoWObject());
}

void
WoWObjectManager::ClearObjects()
{
    mObjects.clear();
}

uint64_t
WoWObjectManager::GetActivePlayerGUID()
{
    return ((ObjectManagerGetActivePlayerGUID)GetActivePlayerGUIDAddress)();
}
