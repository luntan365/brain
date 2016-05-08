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

    void SetDestination(const Position& position);

    concurrency::task<void> StopMoving();

    bool ReachedDestination() const;

    bool IsMoving() const;

private:
    void RegeneratePath();

private:
    PathFinder mPathFinder;
    std::deque<Vector3> mPath;
    WoWPlayer mPlayer;
    Position mDestination;
    float mThreshold;
};
