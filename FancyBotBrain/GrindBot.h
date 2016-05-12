#pragma once

#include "IBot.h"
#include "IBotConfig.h"
#include "ICustomClass.h"
#include "PathTracker.h"

class MoveMapManager;

struct GrindBotProfile : public IBotConfig
{
    virtual nlohmann::json ToJson();

    virtual bool FromJson(const nlohmann::json& json);

    std::vector<Position> mGrindPositions;
};

struct GrindBotConfiguration : public IBotConfig
{
    virtual nlohmann::json ToJson();

    virtual bool FromJson(const nlohmann::json& json);

    Position mVendorPosition;
    float mRestManaPercent;
    float mRestHealthPercent;
    std::string mDrinkName;
    std::string mFoodName;
    float mCombatRange;
    Position mGrindPosition;
    float mGrindMaxDistance;
};


class GrindBot : public IBot
{
public:
    GrindBot(MoveMapManager* pMoveMapManager);

    virtual void OnStart();

    virtual void OnStop();

    virtual IBotConfig* GetConfig();

    virtual IBotConfig* GetProfile();

    virtual concurrency::task<void> Tick(GameState& state);

    void MoveTo(const WoWPlayer& me, const Position& p);

    concurrency::task<void> StopMoving();

private:
    concurrency::task<void> DoPull(const WoWPlayer& me, GameState& state);

    concurrency::task<void> DoCombat(const WoWPlayer& me, GameState& state);

    concurrency::task<void> DoRest(const WoWPlayer& me, GameState& state);

    concurrency::task<void> DoBuff(const WoWPlayer& me, GameState& state);

    void InitializeGrindPositions(const WoWPlayer& me);

    void UpdateGrindPosition(const WoWPlayer& me);

    Position GetGrindPosition();

private:
    GrindBotConfiguration mConfig;
    GrindBotProfile mProfile;
    std::unique_ptr<ICustomClass> mpClass;
    PathTracker mPathTracker;
    uint32_t mCurrentMapId;

    bool mBuff;
    bool mRest;
    bool mFirstTick;

    uint32_t mGrindPositionIndex;
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
