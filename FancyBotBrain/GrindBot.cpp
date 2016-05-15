#include "GrindBot.h"

#include <numeric>
#include <concrt.h>
#include "BotIrcClient.h"
#include "classes/MageClass.h"
#include "classes/PaladinClass.h"
#include "hadesmem/detail/trace.hpp"


nlohmann::json
NpcConfiguration::ToJson()
{
    nlohmann::json json(mPosition.ToJson());
    json["enabled"] = mEnabled;
    json["npcId"] = mId;
    return json;
}

bool
NpcConfiguration::FromJson(const nlohmann::json& json)
{
    mEnabled = json["enabled"];
    mId = json["npcId"];
    mPosition.FromJson(json);
    return true;
}

nlohmann::json
GrindBotProfile::ToJson()
{
    nlohmann::json json({});
    json["positions"] = nlohmann::json::array();
    for (const auto& p : mGrindPositions)
    {
        json["positions"].push_back(p.ToJson());
    }
    return json;
}

bool
GrindBotProfile::FromJson(const nlohmann::json& json)
{
    mGrindPositions.clear();
    for (const auto& posObj : json["positions"])
    {
        Position p;
        p.FromJson(posObj);
        mGrindPositions.push_back(p);
    }
    return true;
}

nlohmann::json
GrindBotConfiguration::ToJson()
{
    nlohmann::json json({});
    json["vendor_position_x"] = mVendorPosition.x;
    json["vendor_position_y"] = mVendorPosition.y;
    json["vendor_position_z"] = mVendorPosition.z;
    json["rest_health_percentage"] = mRestHealthPercent;
    json["rest_mana_percentage"] = mRestManaPercent;
    json["drink_name"] = mDrinkName;
    json["food_name"] = mFoodName;
    json["combat_range"] = mCombatRange;
    json["max_grind_distance"] = mGrindMaxDistance;
    return json;
}

bool
GrindBotConfiguration::FromJson(const nlohmann::json& json)
{
    mVendorPosition.x = json["vendor_position_x"];
    mVendorPosition.y = json["vendor_position_y"];
    mVendorPosition.z = json["vendor_position_z"];
    mRestHealthPercent = json["rest_health_percentage"];
    mRestManaPercent = json["rest_mana_percentage"];
    mDrinkName = json["drink_name"];
    mFoodName = json["food_name"];
    mCombatRange = json["combat_range"];
    mGrindMaxDistance = json["max_grind_distance"];
    return true;
}

GrindBot::GrindBot(MoveMapManager* pMoveMapManager)
    : mConfig()
    , mPathTracker(pMoveMapManager, WoWPlayer(), 3.0)
    , mCurrentMapId(0)
    , mBuff(true)
    , mRest(true)
{
    mConfig.mRestManaPercent = 40;
    mConfig.mRestHealthPercent = 40;
    mConfig.mFoodName = "Forest Mushroom Cap";
    mConfig.mDrinkName = "Conjured Water";
    mConfig.mCombatRange = 4.0;
}

void GrindBot::OnStart()
{
    PaladinConfig config(mConfig);
    mConfig = config;
    mpClass.reset(new PaladinClass(config, this));
    mFirstTick = true;
}

void GrindBot::OnStop()
{
}

IBotConfig*
GrindBot::GetConfig()
{
    return &mConfig;
}

IBotConfig*
GrindBot::GetProfile()
{
    return &mProfile;
}

std::vector<WoWUnit> GetLootableUnits(const std::vector<WoWUnit>& units)
{
    return FilterUnits(
        units,
        [](const WoWUnit& unit)
        {
            return unit.IsLootable();
        }
    );
}

std::vector<WoWUnit> GetAttackableUnits(
    const WoWPlayer& me,
    const std::vector<WoWUnit>& units)
{
    return FilterUnits(
        units,
        [me](const WoWUnit& unit)
        {
            return unit.IsAlive() && !unit.TappedByOther();
        }
    );
}

std::vector<WoWUnit> GetAttackers(
    const WoWPlayer& me,
    const std::vector<WoWUnit>& units)
{
    return FilterUnits(
        units,
        [me](const WoWUnit& unit)
        {
            return unit.IsInCombat() && unit.TappedByMe();
        }
    );
}

concurrency::task<std::vector<WoWUnit>>
GetInLosUnits(
    const WoWPlayer& me,
    const std::vector<WoWUnit>& units)
{
    std::vector<concurrency::task<bool>> tasks;
    for (const auto& unit : units)
    {
        tasks.emplace_back(
            me.InLosWith(unit)
        );
    }
    return concurrency::when_all(tasks.begin(), tasks.end())
        .then([units](const std::vector<bool>& inLos) {
            std::vector<WoWUnit> unitsOut;
            for (auto i = 0; i < inLos.size(); i++)
            {
                if (inLos[i])
                {
                    unitsOut.push_back(units[i]);
                }
            }
            return unitsOut;
        });
}

const WoWUnit GetClosestUnit(
    const WoWPlayer& me,
    const std::vector<WoWUnit>& units)
{
	WoWUnit baseCase;
    return std::accumulate(
        units.begin(),
        units.end(),
        baseCase,
        [&me](const WoWUnit& current, const WoWUnit& unit)
        {
            const auto curDist =
                me.GetPosition().DistanceSquared(current.GetPosition());
            const auto testDist =
                me.GetPosition().DistanceSquared(unit.GetPosition());
            if (testDist < curDist)
            {
                return unit;
            }
            else
            {
                return current;
            }
        }
    );
}

void GrindBot::MoveTo(const WoWPlayer& me, const Position& position)
{
    mPathTracker.SetDestination(position);
}

concurrency::task<void>
GrindBot::StopMoving()
{
    return mPathTracker.StopMoving();
}

concurrency::task<void>
GrindBot::DoPull(const WoWPlayer& me, GameState& state)
{
    return mpClass->DoPull(me, state);
}

concurrency::task<void>
GrindBot::DoCombat(const WoWPlayer& me, GameState& state)
{
    mRest = true;
    mBuff = true;
    return mpClass->DoCombat(me, state);
}

concurrency::task<void>
GrindBot::DoRest(const WoWPlayer& me, GameState& state)
{
    return mpClass->DoRest(me, state).then([this] (bool moreRest) {
        mRest = moreRest;
    });
}

concurrency::task<void>
GrindBot::DoBuff(const WoWPlayer& me, GameState& state)
{
    return mpClass->DoBuff(me, state).then([this] (bool moreBuff) {
        mBuff = moreBuff;
    });
}

void GrindBot::InitializeGrindPositions(const WoWPlayer& me)
{
    const auto mePos = me.GetPosition();
    const auto& positions = mProfile.mGrindPositions;
    float minDistance = mePos.DistanceSquared(positions.at(0));
    uint32_t minIndex = 0;
    for (uint32_t index = 1; index < positions.size(); index++)
    {
        const auto& position = positions.at(index);
        auto d = mePos.DistanceSquared(position);
        if (d < minDistance)
        {
            minDistance = d;
            minIndex = index;
        }
    }
    mGrindPositionIndex = minIndex;
}

void GrindBot::UpdateGrindPosition(const WoWPlayer& me)
{
    auto d = me.GetPosition().Distance2d(GetGrindPosition());
    if (d < 10.0)
    {
        auto totalGrindSpots = mProfile.mGrindPositions.size();
        auto nextIndex = (mGrindPositionIndex + 1) % totalGrindSpots;
        mGrindPositionIndex = nextIndex; 
    }
}

Position GrindBot::GetGrindPosition()
{
    return mProfile.mGrindPositions.at(mGrindPositionIndex);
}

concurrency::task<void>
GrindBot::Tick(GameState& state)
{
    if (!state.GetIsInGame())
    {
		return concurrency::task_from_result();
    }
    auto& irc = BotIrcClient::Instance();
    const auto& me = state.ObjectManager().GetPlayer();
    if (mFirstTick)
    {
        InitializeGrindPositions(me);
        mFirstTick = false;
    }
    mPathTracker.SetPlayer(me);
    UpdateGrindPosition(me);

    // TODO
    // UpdateCurrentMap(me)
    const auto& units = state.ObjectManager().Units();
    const auto currentPosition = me.GetPosition();

    const auto lootableUnits = GetLootableUnits(units);
    if (!me.IsAlive())
    {
        irc.Log("Releasing spirit");
        return me.ReleaseSpirit();
    }
    else if (me.IsGhost())
    {
        if (me.InRangeOf(me.GetCorpsePosition(), 20.0))
        {
            irc.Log("Reviving");
            return me.ReviveAtCorpse();
        }
        else
        {
            irc.Log("Walking to corpse");
            MoveTo(me, me.GetCorpsePosition());
        }
    }
    else if (me.IsCasting() || me.IsChanneling())
    {
        return concurrency::task_from_result();
    }
    else if (me.IsInCombat())
    {
        irc.Log("Auto repeat: " + std::to_string(me.GetAutoRepeatingSpell()));
        return DoCombat(me, state);
    }
    else if (false && mProfile.mRestock.mEnabled)
    {
        // TODO do restock
        MoveTo(me, mProfile.mRestock.mPosition);
    }
    else if (me.GetInventory().IsFull() && mProfile.mVendor.mEnabled)
    {
        MoveTo(me, mProfile.mVendor.mPosition);
    }
    else if (me.HasBrokenEquipment() && mProfile.mRepair.mEnabled)
    {
        MoveTo(me, mProfile.mRepair.mPosition);
    }
    else if (mBuff)
    {
        return DoBuff(me, state);
    }
    else if (mRest)
    {
        return DoRest(me, state);
    }
    else if (!lootableUnits.empty() && !me.GetInventory().IsFull())
    {
        const auto& closestUnit = GetClosestUnit(me, lootableUnits);
        auto t = me.SetTarget(closestUnit);
        if (me.IsLooting())
        {
            irc.Log("Looting");
            return t.then([&me] {
                return me.AutoLoot();
            });
        }
        else if (me.InRangeOf(closestUnit, 3.0f))
        {
            irc.Log("Open Loot Window");
            return t.then([this, &me, closestUnit] {
                return StopMoving();
            }).then([&me, closestUnit] {
                return me.Loot(closestUnit);
            });
        }
        else
        {
            irc.Log("Moving in to loot");
            MoveTo(me, closestUnit.GetPosition());
        }
    }
    else
    {
        const auto& enemyUnits = state.ObjectManager().GetEnemyUnits();
        const auto& attackableUnits = GetAttackableUnits(me, enemyUnits);
        const Circle grindZone(GetGrindPosition(), mConfig.mGrindMaxDistance);
        const auto inZoneUnits = FilterUnits(
            attackableUnits,
            [&grindZone](const WoWUnit& unit)
            {
                return grindZone.ContainsPoint(unit.GetPosition());
            }
        );
        const auto& losUnitsTask = GetInLosUnits(me, inZoneUnits);
        const auto closestUnit = GetClosestUnit(me, inZoneUnits);
        return losUnitsTask
            .then([this, &me, &irc, &state, currentPosition, closestUnit]
                    (const std::vector<WoWUnit>& losUnits) {
                if (losUnits.empty())
                {
                    MoveTo(me, GetGrindPosition());
                    return mPathTracker.Tick();
                }
                const auto closestLosUnit = GetClosestUnit(me, losUnits);
                if (currentPosition.Distance(closestLosUnit.GetPosition())
                    < mConfig.mCombatRange)
                {
                    irc.Log("Pulling mob");
                    std::vector<concurrency::task<void>> tasks = {
                        StopMoving(),
                        me.SetTarget(closestLosUnit),
                        me.Turn(closestLosUnit.GetPosition()),
                        DoPull(me, state)
                    };
                    return concurrency::when_all(tasks.begin(), tasks.end())
                        .then([]{});
                }
                else
                {
                    irc.Log("Walking towards mob");
                    MoveTo(me, closestUnit.GetPosition());
                    return mPathTracker.Tick();
                }
            });
    }

    return mPathTracker.Tick();
}
