#include "MageClass.h"
#include "BotIrcClient.h"

MageConfig::MageConfig(const GrindBotConfiguration& config)
    : GrindBotConfiguration(config)
{
}

MageClass::MageClass(const MageConfig& config, GrindBot* pBot)
    : mConfig(config)
    , mpBot(pBot)
{
}

concurrency::task<void>
MageClass::DoPull(const WoWPlayer& me, GameState& state)
{
    return me.CastSpellByName("Fireball");
}

concurrency::task<void>
MageClass::DoCombat(const WoWPlayer& me, GameState& state)
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

concurrency::task<bool>
MageClass::DoRest(const WoWPlayer& me, GameState& state)
{
    auto& irc = BotIrcClient::Instance();
    if ((me.IsDrinking() && me.ManaPercent() < 100) ||
        (me.IsEating() && me.HealthPercent() < 100))
    {
        irc.Log("Eat/Drink Tick");
        return concurrency::task_from_result(true);
    }
    else if (me.ManaPercent() < mConfig.mRestManaPercent)
    {
        irc.Log("Drinking");
        return mpBot->StopMoving().then([this, &me] {
            return me.UseItemByName(mConfig.mDrinkName);
        }).then([] {
            return true;
        });
    }
    else if (me.HealthPercent() < mConfig.mRestHealthPercent)
    {
        irc.Log("Eating");
        return mpBot->StopMoving().then([this, &me] {
            return me.UseItemByName(mConfig.mFoodName);
        }).then([] {
            return true;
        });
    }
    return concurrency::task_from_result(false);
}

concurrency::task<bool>
MageClass::DoBuff(const WoWPlayer& me, GameState& state)
{
    if (me.GetInventory().GetItemCountByName(mConfig.mDrinkName) < 2)
    {
        return me.CastSpellByName("Conjure Water").then([] {
            return true;
        });
    }
    return concurrency::task_from_result(false);
}
