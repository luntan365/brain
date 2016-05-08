#pragma once

#include "IBot.h"
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

    concurrency::task<void>DoPull(const WoWPlayer& me);

    concurrency::task<void> DoCombat(const WoWPlayer& me, GameState& state);

private:
    void MoveTo(const WoWPlayer& me, const Position& p);

    concurrency::task<void> StopMoving();

    GrindBotConfiguration mConfig;
    MoveMapManager mMoveMapManager;
    PathTracker mPathTracker;
    uint32_t mCurrentMapId;

};
