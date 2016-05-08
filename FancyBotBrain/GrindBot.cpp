#include "GrindBot.h"

#include <numeric>
#include "hadesmem/detail/trace.hpp"

GrindBot::GrindBot()
    : mConfig()
    , mMoveMapManager()
    , mPathTracker(&mMoveMapManager, WoWPlayer(), 0.0)
    , mCurrentMapId(0)
{
    mConfig.mRestManaPercent = 40;
    mConfig.mRestHealthPercent = 40;
    mConfig.mFoodName = "Forest Mushroom Cap";
    mConfig.mDrinkName = "Refreshing Spring Water";
}

void GrindBot::OnStart()
{
    mMoveMapManager.Initialize("C:\\mmaps");
    mPathTracker = PathTracker(&mMoveMapManager, WoWPlayer(), 10.0);
}

template <typename F>
std::vector<WoWUnit> FilterUnits(const std::vector<WoWUnit>& units, F fn)
{
    std::vector<WoWUnit> out;
    std::for_each(
        units.begin(),
        units.end(),
        [&out, &fn](const WoWUnit& unit)
        {
            if (fn(unit))
            {
                out.emplace_back(unit);
            }
        }
    );
    return out;
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
            return unit.IsAlive();
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

void GrindBot::StopMoving()
{
    mPathTracker.StopMoving();
}

void GrindBot::DoPull(const WoWPlayer& me)
{
    me.CastSpellByName("Fireball");
}

void GrindBot::DoCombat(const WoWPlayer& me, GameState& state)
{
    const auto& enemyUnits = state.ObjectManager().GetEnemyUnits();
    const auto& attackableUnits = GetAttackableUnits(me, enemyUnits);
    const auto& attackers = GetAttackers(me, attackableUnits);
    const auto closestUnit = GetClosestUnit(me, attackers);
    me.SetTarget(closestUnit);
    me.Turn(closestUnit.GetPosition());
    me.CastSpellByName("Fireball");
}

void GrindBot::Tick(GameState& state)
{
    if (!state.GetIsInGame())
    {
        return;
    }
    const auto& me = state.ObjectManager().GetPlayer();
    mPathTracker.SetPlayer(me);

    // TODO
    // UpdateCurrentMap(me)
    const auto& units = state.ObjectManager().Units();
    const auto currentPosition = me.GetPosition();

    const auto lootableUnits = GetLootableUnits(units);
    if (!me.IsAlive())
    {
        me.ReleaseSpirit();
    }
    else if (me.IsGhost())
    {
        if (me.InRangeOf(me.GetCorpsePosition(), 20.0))
        {
            me.ReviveAtCorpse();
        }
        else
        {
            MoveTo(me, me.GetCorpsePosition());
        }
    }
    else if (me.IsInCombat())
    {
        DoCombat(me, state);
    }
    else if (me.ManaPercent() < mConfig.mRestManaPercent)
    {
        StopMoving();
        me.UseItemByName(mConfig.mDrinkName);
    }
    else if (me.HealthPercent() < mConfig.mRestHealthPercent)
    {
        StopMoving();
        me.UseItemByName(mConfig.mFoodName);
    }
    else if ((me.IsDrinking() && me.ManaPercent() < 100) ||
             (me.IsEating() && me.HealthPercent() < 100))
    {
        StopMoving();
        return;
    }
    else if (!lootableUnits.empty() && !me.GetInventory().IsFull())
    {
        const auto& closestUnit = GetClosestUnit(me, lootableUnits);
        me.SetTarget(closestUnit);
        if (me.IsLooting())
        {
            me.AutoLoot();
        }
        else if (me.InRangeOf(closestUnit, 3.0f))
        {
            me.Loot(closestUnit);
        }
        else
        {
            MoveTo(me, closestUnit.GetPosition());
        }
    }
    else
    {
        const auto& enemyUnits = state.ObjectManager().GetEnemyUnits();
        const auto& attackableUnits = GetAttackableUnits(me, enemyUnits);
        const auto closestUnit = GetClosestUnit(me, attackableUnits);

        me.SetTarget(closestUnit);
        if (currentPosition.Distance(closestUnit.GetPosition()) < 25.0)
        {
            me.Turn(closestUnit.GetPosition());
            DoPull(me);
        }
        else
        {
            MoveTo(me, closestUnit.GetPosition());
        }
    }
    mPathTracker.Tick();
}
