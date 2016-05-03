#pragma once

#include "IBot.h"
#include "MoveMapManager.h"
#include "PathTracker.h"


class GrindBot : public IBot
{
public:
    GrindBot();

    virtual void OnStart();

    virtual void Tick(GameState& state);

private:
    void MoveTo(const WoWPlayer& me, const Position& p);

    MoveMapManager mMoveMapManager;
    PathTracker mPathTracker;
    uint32_t mCurrentMapId;
};
