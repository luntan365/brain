#pragma once

#include "IBot.h"
#include "MoveMapManager.h"
#include "PathFinder.h"


class GrindBot : public IBot
{
public:
    GrindBot();

    virtual void OnStart();

    virtual void Tick(GameState& state);

private:
    void MoveTo(const WoWPlayer& me, const Position& p);

    MoveMapManager mMoveMapManager;
    PathFinder mPathFinder;
    uint32_t mCurrentMapId;
};
