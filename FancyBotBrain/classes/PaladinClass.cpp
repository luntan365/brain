#include "PaladinClass.h"
#include "BotIrcClient.h"

PaladinConfig::PaladinConfig(const GrindBotConfiguration config)
    : GrindBotConfiguration(config)
    , mUseCrusader(true)
    , mUseBlessing(true)
    , mBlessing("Blessing of Might")
{
}

PaladinClass::PaladinClass(const PaladinConfig& config, GrindBot* pBot)
    : mConfig(config)
    , mpBot(pBot)
{
}

bool
HaveSeal(const WoWPlayer& me)
{
    return me.HasAura("Seal of the Crusader")
        || me.HasAura("Seal of Righteousness");
}

concurrency::task<void>
PaladinClass::SealJudgementRotation(const WoWPlayer& me, GameState& state)
{
    const auto targetGuid = me.GetTargetGuid();
    if (!HaveSeal(me))
    {
        auto maybeTarget = state.ObjectManager().GetTarget();
        if (mConfig.mUseCrusader 
            && maybeTarget 
            && !maybeTarget->HasDebuff("Judgement of the Crusader"))
        {
            return me.CastSpellByName("Seal of the Crusader");
        }
        else
        {
            return me.CastSpellByName("Seal of Righteousness");
        }
    }
    else
    {
        return me.CastSpellByName("Judgement");
    }
}

concurrency::task<void>
PaladinClass::DoPull(const WoWPlayer& me, GameState& state)
{
    return SealJudgementRotation(me, state).then([&me] {
        me.Attack();
    });
}


std::string GetShieldSpell(const SpellBook& spells)
{
    const std::vector<std::string> shields {
        "Divine Shield",
        "Divine Protection",
        "Blessing of Protection"
    };
    for (const auto& s : shields)
    {
        if (spells.KnowsSpell(s) && spells.IsReady(s))
        {
            return s;
        }
    }
    return "";
}


concurrency::task<void>
PaladinClass::DoCombat(const WoWPlayer& me, GameState& state)
{
    const auto& enemyUnits = state.ObjectManager().GetEnemyUnits();
    const auto& attackableUnits = GetAttackableUnits(me, enemyUnits);
    const auto& attackers = GetAttackers(me, attackableUnits);
    const auto& spells = me.GetSpellBook();
    if (spells.KnowsSpell("Holy Light"))
    {
        const auto& holyLight = spells.GetMaxRankSpell("Holy Light");
        if (me.HealthPercent() < 40 && me.Mana() >= holyLight.manaCost)
        {
            std::string spell = "Holy Light";
            if (attackers.size() > 1 && !me.HasDebuff("Forbearance")) 
            {
                const auto shield = GetShieldSpell(spells);
                if (shield != "")
                {
                    spell = shield;
                }
            }
            std::vector<concurrency::task<void>> tasks{
                me.SetTarget(me),
                me.CastSpellByName(spell)
            };
            return concurrency::when_all(tasks.begin(), tasks.end())
                .then([]{});
        }
    }
    const auto& losUnitsTask = GetInLosUnits(me, attackers);
    return losUnitsTask.then([this, &me, &state] (const std::vector<WoWUnit>& losUnits) {
        const auto closestUnit = GetClosestUnit(me, losUnits);
        std::vector<concurrency::task<void>> tasks{
            me.SetTarget(closestUnit),
            me.Turn(closestUnit.GetPosition()),
            SealJudgementRotation(me, state),
            me.Attack()
        };
        return concurrency::when_all(tasks.begin(), tasks.end()).then([]{});
    });
}

concurrency::task<bool>
PaladinClass::DoRest(const WoWPlayer& me, GameState& state)
{
    auto& irc = BotIrcClient::Instance();
    if ((me.IsDrinking() && me.ManaPercent() < 100) ||
        (me.IsEating() && me.HealthPercent() < 100))
    {
        return concurrency::task_from_result(true);
    }
    else if (me.HealthPercent() < mConfig.mRestHealthPercent &&
             me.ManaPercent() > 30)
    {
        return me.SetTarget(me).then([&me] {
            return me.CastSpellByName("Holy Light");
        }).then([] {
            return true;
        });
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
PaladinClass::DoBuff(const WoWPlayer& me, GameState& state)
{
    if (mConfig.mUseBlessing && !me.HasAura(mConfig.mBlessing))
    {
        return me.SetTarget(me).then([this, &me] {
            return me.CastSpellByName(mConfig.mBlessing);
        }).then([] {
            return true;
        });
    }
    return concurrency::task_from_result(false);
}
