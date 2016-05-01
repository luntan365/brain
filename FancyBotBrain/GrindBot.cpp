#include "GrindBot.h"

#include <numeric>

void GrindBot::OnStart()
{
}

void GrindBot::Tick(GameState& state)
{
    if (!state.GetIsInGame())
    {
        return;
    }

    const auto& units = state.ObjectManager().Units();
    const auto& me = state.ObjectManager().GetPlayer();
    const auto currentPosition = me.GetPosition();

    const auto& closestUnit = std::accumulate(
        units.begin(),
        units.end(),
        WoWUnit(),
        [&currentPosition, &me](const WoWUnit& current, const std::pair<uint64_t, WoWUnit>& id_unit)
        {
            const auto& unit = id_unit.second;
            if (!unit.IsAlive() || !me.IsUnitHostile(unit))
            {
                return current;
            }
            const auto curDist = currentPosition.DistanceSquared(current.GetPosition());
            const auto testDist = currentPosition.DistanceSquared(unit.GetPosition());
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

    me.SetTarget(closestUnit.GetGUID());
    if (currentPosition.Distance(closestUnit.GetPosition()) < 25.0)
    {
        me.Turn(closestUnit.GetPosition());
        me.CastSpellByName("Fireball");
    }
    else
    {
        me.ClickToMove(closestUnit.GetPosition());
    }
}
