#include "GrindBot.h"

#include <numeric>
#include <concrt.h>
#include <tuple>
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
    mVendor.FromJson(json["npcs"]["vendor"]);
    mRestock.FromJson(json["npcs"]["restock"]);
    mRepair.FromJson(json["npcs"]["repair"]);
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
    , mPathTracker(pMoveMapManager, WoWPlayer(), 5.0)
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
            for (uint32_t i = 0; i < inLos.size(); i++)
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

bool GrindBot::MoveTo(const WoWPlayer& me, const Position& position)
{
    return mPathTracker.SetDestination(position);
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
GrindBot::WalkToAndInteractWithNpcId(
    const WoWPlayer& me,
    const std::vector<WoWUnit>& nearbyUnits,
    const Position& position, 
    uint32_t npcId)
{
    // Are we in the user-provided position yet?
    if (me.GetPosition().Distance2d(position) > 30.0) 
    {
        MoveTo(me, position);
    }
    const auto& units = FilterUnits(
        nearbyUnits,
        [&me, npcId](const WoWUnit& unit) {
            if (unit.GetId() == npcId)
            {
                return true;
            }
            return false;
        }
    );
    if (units.size() > 0)
    {
        const auto& unit = GetClosestUnit(me, units);
        if (me.GetPosition().Distance2d(unit.GetPosition()) < 5.0)
        {
            return me.InteractWith(unit);
        }
        else
        {
            MoveTo(me, unit.GetPosition());
        }
    } 
    return mPathTracker.Tick();
} 

boost::optional<std::tuple<uint8_t, uint8_t>>
GetNextSellItem(const WoWPlayer& me)
{
    boost::optional<std::tuple<uint8_t, uint8_t>> sellItem = boost::none;
    me.GetInventory().ForEachItem(
        [&sellItem] (const WoWItem& item, uint8_t bag, uint8_t slot) {
            if (item.GetSellPrice() == 0)
            {
                return true;
            }
            sellItem = boost::make_optional(std::make_tuple(bag, slot));
            return false;
        }
    );
    return sellItem;
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
    else if (state.GetMerchantPane().IsOpen() 
        && mGrindState == GrindBotState::VENDORING)
    {
        auto maybeItem = GetNextSellItem(me);
        if (maybeItem)
        {
            return me.UseContainerItem(
                std::get<0>(*maybeItem),
                std::get<1>(*maybeItem)
            );
        }
        else
        {
            mGrindState = GrindBotState::REPAIRING;
            return concurrency::task_from_result();
        }
    }
    else if (state.GetMerchantPane().IsOpen() 
        && mGrindState == GrindBotState::REPAIRING)
    {
        if (!me.IsFullyRepaired())
        {
            return state.GetMerchantPane().RepairAll();
        }        
        else
        {
            mGrindState = GrindBotState::GRINDING;
            return concurrency::task_from_result();
        }
    }
    else if (state.GetGossipPane().IsOpen())
    {
        return state.GetGossipPane().SelectFirstOptionWithType(
            GossipType::VENDOR
        );
    }
    else if (mGrindState == GrindBotState::RESTOCKING)
    {
        return WalkToAndInteractWithNpcId(
            me,
            units,
            mProfile.mRestock.mPosition,
            mProfile.mRestock.mId
        );
    }
    else if (mGrindState == GrindBotState::REPAIRING)
    {
        return WalkToAndInteractWithNpcId(
            me,
            units,
            mProfile.mRepair.mPosition,
            mProfile.mRepair.mId
        );
    }
    else if (mGrindState == GrindBotState::VENDORING)
    {
        return WalkToAndInteractWithNpcId(
            me,
            units,
            mProfile.mVendor.mPosition,
            mProfile.mVendor.mId
        );
    }
    else if (
        (me.GetInventory().IsFull() && mProfile.mVendor.mEnabled) ||
        (me.HasBrokenEquipment() && mProfile.mRepair.mEnabled) ||
        (false && mProfile.mRestock.mEnabled))
    {
        mGrindState = GrindBotState::VENDORING;
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
        else if (me.InRangeOf(closestUnit, 5.0f))
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
        auto& losUnitsTask = GetInLosUnits(me, inZoneUnits);
        return losUnitsTask
            .then([this, &me, &irc, &state, currentPosition]
                    (std::vector<WoWUnit>& losUnits) {
                std::sort(
                    losUnits.begin(), 
                    losUnits.end(),
                    [&me](const WoWUnit& a, const WoWUnit& b) {
                        return me.GetPosition().Distance(a.GetPosition()) 
                            < me.GetPosition().Distance(b.GetPosition());
                    }
                );
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
                    for (const auto& unit : losUnits)
                    {
                        irc.Log("Walking towards mob");
                        if (MoveTo(me, unit.GetPosition()))
                        {
                            break;
                        }
                    }
                }
                return mPathTracker.Tick();
            });
    }

    return mPathTracker.Tick();
}
