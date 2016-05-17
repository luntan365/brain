#pragma once

#include "WoWPlayer.h"

class PathTracker
{
public:
    PathTracker(
        MoveMapManager* pMoveMapManager,
        const WoWPlayer& player,
        float threshold
    );

    concurrency::task<void> Tick();

    void SetPlayer(const WoWPlayer& pPlayer);

    bool SetDestination(const Position& position);

    concurrency::task<void> StopMoving();

    bool ReachedDestination() const;

    bool IsMoving() const;

private:
    bool RegeneratePath(const Position& position);

private:
    PathFinder mPathFinder;
    std::deque<Vector3> mPath;
    WoWPlayer mPlayer;
    Position mDestination;
    float mThreshold;
};
