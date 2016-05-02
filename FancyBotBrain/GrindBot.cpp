#include "GrindBot.h"

#include <numeric>
#include "hadesmem/detail/trace.hpp"

GrindBot::GrindBot()
    : mMoveMapManager()
    , mPathFinder(&mMoveMapManager, 0)
    , mCurrentMapId(0)
{
}

void GrindBot::OnStart()
{
    mMoveMapManager.Initialize("C:\\mmaps");
    mPathFinder = PathFinder(&mMoveMapManager, 0);
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
            return unit.IsAlive() && me.IsUnitHostile(unit);
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
    HADESMEM_DETAIL_TRACE_A("Calculating path...");
    //if (position == mPathFinder.GetEndPosition())
    //{
        //return;
    //}
    mPathFinder.Calculate(me.GetPosition(), position);
    const auto& path = mPathFinder.GetPath();
    if (path.size() > 1)
    {
        const Position position(path.at(1));
        me.ClickToMove(position);
        HADESMEM_DETAIL_TRACE_A("MOVING");
    }
}

void GrindBot::Tick(GameState& state)
{
    if (!state.GetIsInGame())
    {
        return;
    }
    const auto& me = state.ObjectManager().GetPlayer();
    // TODO
    // UpdateCurrentMap(me)
    const auto& units = state.ObjectManager().Units();
    const auto currentPosition = me.GetPosition();

    const auto lootableUnits = GetLootableUnits(units);
    if (!me.IsAlive())
    {
        MoveTo(me, me.GetCorpsePosition());
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
        const auto& attackableUnits = GetAttackableUnits(me, units);
        const auto closestUnit = GetClosestUnit(me, attackableUnits);

        me.SetTarget(closestUnit);
        if (currentPosition.Distance(closestUnit.GetPosition()) < 25.0)
        {
            me.Turn(closestUnit.GetPosition());
            me.CastSpellByName("Fireball");
        }
        else
        {
            MoveTo(me, closestUnit.GetPosition());
        }
    }
}
