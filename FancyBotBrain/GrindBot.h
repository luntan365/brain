#pragma once

#include "IBot.h"
#include "ICustomClass.h"
#include "MoveMapManager.h"
#include "PathTracker.h"


struct GrindBotConfiguration
{
    Position mVendorPosition;

    float mRestManaPercent;
    float mRestHealthPercent;

    std::string mDrinkName;
    std::string mFoodName;
};


class GrindBot : public IBot
{
public:
    GrindBot();

    virtual void OnStart();

    virtual concurrency::task<void> Tick(GameState& state);

    void MoveTo(const WoWPlayer& me, const Position& p);

    concurrency::task<void> StopMoving();

private:
    concurrency::task<void> DoPull(const WoWPlayer& me, GameState& state);

    concurrency::task<void> DoCombat(const WoWPlayer& me, GameState& state);

    concurrency::task<void> DoRest(const WoWPlayer& me, GameState& state);

    concurrency::task<void> DoBuff(const WoWPlayer& me, GameState& state);

    GrindBotConfiguration mConfig;
    std::unique_ptr<ICustomClass> mpClass;
    MoveMapManager mMoveMapManager;
    PathTracker mPathTracker;
    uint32_t mCurrentMapId;

    bool mBuff;
    bool mRest;

};

// TODO: move these somewhere more appropriate
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

std::vector<WoWUnit> GetLootableUnits(const std::vector<WoWUnit>& units);

std::vector<WoWUnit> GetAttackableUnits(
    const WoWPlayer& me,
    const std::vector<WoWUnit>& units
);

std::vector<WoWUnit> GetAttackers(
    const WoWPlayer& me,
    const std::vector<WoWUnit>& units
);

concurrency::task<std::vector<WoWUnit>>
GetInLosUnits(
    const WoWPlayer& me,
    const std::vector<WoWUnit>& units
);

const WoWUnit GetClosestUnit(
    const WoWPlayer& me,
    const std::vector<WoWUnit>& units
);
