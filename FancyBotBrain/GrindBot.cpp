#include "GrindBot.h"

#include <numeric>
#include <concrt.h>
#include "BotIrcClient.h"
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
    mConfig.mDrinkName = "Conjured Water";
}

void GrindBot::OnStart()
{
    mMoveMapManager.Initialize("C:\\mmaps");
    mPathTracker = PathTracker(&mMoveMapManager, WoWPlayer(), 3.0);
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
GrindBot::DoPull(const WoWPlayer& me)
{
    return me.CastSpellByName("Fireball");
}

concurrency::task<void>
GrindBot::DoCombat(const WoWPlayer& me, GameState& state)
{
    const auto& enemyUnits = state.ObjectManager().GetEnemyUnits();
    const auto& attackableUnits = GetAttackableUnits(me, enemyUnits);
    const auto& attackers = GetAttackers(me, attackableUnits);
    const auto& losUnitsTask = GetInLosUnits(me, attackers);
    return losUnitsTask.then([&me] (const std::vector<WoWUnit>& losUnits) {
        const auto closestUnit = GetClosestUnit(me, losUnits);
        std::vector<concurrency::task<void>> tasks{
            me.SetTarget(closestUnit),
            me.Turn(closestUnit.GetPosition()),
            me.CastSpellByName("Fireball")
        };
        return concurrency::when_all(tasks.begin(), tasks.end()).then([]{});
    });
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
    mPathTracker.SetPlayer(me);

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
        irc.Log("Combat tick");
        return DoCombat(me, state);
    }
    else if ((me.IsDrinking() && me.ManaPercent() < 100) ||
             (me.IsEating() && me.HealthPercent() < 100))
    {
        irc.Log("Eat/Drink Tick");
        return concurrency::task_from_result();
    }
    else if (me.GetInventory().GetItemCountByName(mConfig.mDrinkName) < 2)
    {
        return me.CastSpellByName("Conjure Water");
    }
    else if (me.ManaPercent() < mConfig.mRestManaPercent)
    {
        irc.Log("Drinking");
        return StopMoving().then([this, &me] {
            return me.UseItemByName(mConfig.mDrinkName);
        });
    }
    else if (me.HealthPercent() < mConfig.mRestHealthPercent)
    {
        irc.Log("Eating");
        return StopMoving().then([this, &me] {
            return me.UseItemByName(mConfig.mFoodName);
        });
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
        const auto& losUnitsTask = GetInLosUnits(me, attackableUnits);
        const auto closestUnit = GetClosestUnit(me, attackableUnits);
        return losUnitsTask
            .then([this, &me, &irc, currentPosition, closestUnit]
                    (const std::vector<WoWUnit>& losUnits) {
                const auto closestLosUnit = GetClosestUnit(me, losUnits);
                if (currentPosition.Distance(closestLosUnit.GetPosition()) < 25.0)
                {
                    irc.Log("Pulling mob");
                    std::vector<concurrency::task<void>> tasks = {
                        StopMoving(),
                        me.SetTarget(closestLosUnit),
                        me.Turn(closestLosUnit.GetPosition()),
                        DoPull(me)
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
