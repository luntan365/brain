#include "PathTracker.h"


PathTracker::PathTracker(
    MoveMapManager* pMoveMapManager,
    const WoWPlayer& player,
    float threshold)
    : mPathFinder(pMoveMapManager, 0)
    , mPath()
    , mPlayer(player)
    , mDestination()
    , mThreshold(threshold)
{
}

concurrency::task<void>
PathTracker::Tick()
{
    if (!IsMoving())
    {
        return concurrency::task_from_result();
    }
    if (mPlayer.InRangeOf(mPath.front(), mThreshold))
    {
        mPath.pop_front();
    }
    if (!ReachedDestination())
    {
        return mPlayer.ClickToMove(
            GetPointOnPath(
                mPlayer.GetPosition(),
                mPath.front(),
                10 
            )
        );
    }
    return concurrency::task_from_result();
}

concurrency::task<void>
PathTracker::StopMoving()
{
    mPath.clear();
    return mPlayer.StopMoving();
}

void
PathTracker::SetPlayer(const WoWPlayer& player)
{
    mPlayer = player;
}

void
PathTracker::SetDestination(const Position& position)
{
    if (position == mDestination)
    {
        return;
    }
    mDestination = position;
    RegeneratePath();
}

bool
PathTracker::ReachedDestination() const
{
    return mPath.empty() || mPlayer.InRangeOf(mDestination, mThreshold);
}

bool
PathTracker::IsMoving() const
{
    return !mPath.empty();
}

void
PathTracker::RegeneratePath()
{
    mPathFinder.Calculate(mPlayer.GetPosition(), mDestination);
    mPath = std::deque<Vector3>(
        mPathFinder.GetPath().begin(),
        mPathFinder.GetPath().end()
    );
}
