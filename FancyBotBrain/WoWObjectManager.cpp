#include "stdafx.h"
#include "WoWObjectManager.h"
#include "WowOffsets.h"

WoWObjectManager::WoWObjectManager()
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
    pManager->UpdateTarget();
}

void
WoWObjectManager::ReadObject(void* basePtr)
{
    auto obj = WoWObject::Read(basePtr);
    mObjects.emplace(obj.GetGUID(), std::move(obj));
}

void
WoWObjectManager::UpdatePlayer()
{
    auto addr = mPlayer.GetAddress();
    if (addr != nullptr)
    {
        WoWPlayer::Read(&mPlayer, addr);
    }
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
        case OT_GAMEOBJ:
        case OT_DYNOBJ:
        case OT_CORPSE:
        case OT_ITEM:
        case OT_CONTAINER:
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
        mUnits.emplace_back(player);
        mUnitMap.emplace(player.GetGUID(), player);
    }
}

void
WoWObjectManager::UpdateTarget()
{
    auto targetGuid = mPlayer.GetTargetGuid();
    if (targetGuid == 0)
    {
        mTarget == boost::none;
        return;
    }
    if (mUnitMap.find(targetGuid) == mUnitMap.end())
    {
        mTarget == boost::none;
        return;
    }
    mTarget = boost::make_optional(mUnitMap.at(targetGuid));
}

void
WoWObjectManager::NewUnit(const WoWObject& object)
{
    auto unit = WoWUnit::Read(object.GetAddress());
    if (IsUnitEnemy(unit))
    {
        mEnemyUnits.push_back(unit);
    }
    mUnits.emplace_back(unit);
    mUnitMap.emplace(unit.GetGUID(), unit);
}

void
WoWObjectManager::ClearObjects()
{
    mObjects.clear();
    mUnits.clear();
    mUnitMap.clear();
    mEnemyUnits.clear();
    mPlayer.Reset();
}

uint64_t
WoWObjectManager::GetActivePlayerGUID()
{
    return ((ObjectManagerGetActivePlayerGUID)GetActivePlayerGUIDAddress)();
}

uint32_t
WoWObjectManager::GetUnitReaction(const WoWUnit& unit) const
{
    typedef uint32_t (__thiscall *Unit_GetUnitReaction)(void*, void*);
    auto thisaddr = mPlayer.GetAddress();
    auto addr = unit.GetAddress();
    return ((Unit_GetUnitReaction)0x6061E0)(thisaddr, addr);
}

bool
WoWObjectManager::IsUnitEnemy(const WoWUnit& unit) const
{
    return GetUnitReaction(unit) <= 3;
}

bool
WoWObjectManager::IsUnitFriendly(const WoWUnit& unit) const
{
    return !IsUnitEnemy(unit);
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

const WoWObjectManager::UnitContainer&
WoWObjectManager::Units() const
{
    return mUnits;
}

const WoWObjectManager::UnitContainer&
WoWObjectManager::GetEnemyUnits() const
{
    return mEnemyUnits;
}

const boost::optional<WoWUnit>&
WoWObjectManager::GetTarget() const
{
    return mTarget;
}

nlohmann::json
WoWObjectManager::ToJson() const
{
    nlohmann::json j;
    j["player_position"] = mPlayer.GetPosition().ToJson();
    auto maybeTarget = GetTarget();
    if (maybeTarget)
    {
        nlohmann::json targetJson = {
            {"position", maybeTarget->GetPosition().ToJson()},
            {"id", maybeTarget->GetId()},
        };
        j["target"] = targetJson;
    }
    return j;
}
