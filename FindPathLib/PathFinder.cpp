#include "PathFinder.h"
#include "DetourCommon.h"

#include "easylogging++.h"

static const int VERTEX_SIZE = 3;

Vector3::Vector3(float xx, float yy, float zz)
    : x(xx), y(yy), z(zz)
{
}

Vector3::Vector3()
    : x(0), y(0), z(0)
{
}

PathFinder::PathFinder(MoveMapManager* pMMM, uint32_t mapId)
    : mPolyLength(0)
    , mType(PATHFIND_BLANK)
    , mNavMesh(pMMM->GetNavMesh(mapId))
    , mNavMeshQuery(pMMM->GetNavMeshQuery(mapId))
{
    mFilter.setIncludeFlags(NAV_GROUND | NAV_WATER);
    mFilter.setExcludeFlags(0);
}


void PathFinder::Calculate(const Vector3& startPos, const Vector3& endPos)
{
    SetEndPosition(endPos);
    SetStartPosition(startPos);

    float distToStartPoly, distToEndPoly;
    float startPoint[VERTEX_SIZE] = {startPos.y, startPos.z, startPos.x};
    float endPoint[VERTEX_SIZE] = {endPos.y, endPos.z, endPos.x};

    dtPolyRef startPoly = GetPolyByLocation(startPoint, &distToStartPoly);
    dtPolyRef endPoly = GetPolyByLocation(endPoint, &distToEndPoly);

    dtStatus dtResult;

    if (startPoly == INVALID_POLYREF || endPoly == INVALID_POLYREF)
    {
    }

    bool farFromPoly = (distToStartPoly > 7.0f || distToEndPoly > 7.0f);
    if (farFromPoly)
    {
        bool buildShortcut = false;
        if (buildShortcut)
        {
            BuildShortcut();
            mType = PathType(PATHFIND_NORMAL | PATHFIND_NOT_USING_PATH);
            return;
        }
        else
        {
            float closestPoint[VERTEX_SIZE];

            dtResult = mNavMeshQuery->closestPointOnPoly(
                endPoly,
                endPoint,
                closestPoint,
                NULL
            );

            if (dtStatusSucceed(dtResult))
            {
                dtVcopy(endPoint, closestPoint);
                SetActualEndPosition(Vector3(endPoint[2], endPoint[0], endPoint[1]));
            }

            mType = PATHFIND_INCOMPLETE;
        }
    }

    if (startPoly == endPoly)
    {
        BuildShortcut();

        mPathPolyRefs[0] = startPoly;
        mPolyLength = 1;

        mType = farFromPoly ? PATHFIND_INCOMPLETE : PATHFIND_NORMAL;
        return;
    }

    bool startPolyFound = false;
    bool endPolyFound = false;
    uint32_t pathStartIndex;
    uint32_t pathEndIndex;

    if (mPolyLength)
    {
        for (pathStartIndex = 0; pathStartIndex < mPolyLength; pathStartIndex++)
        {
            if (mPathPolyRefs[pathStartIndex] == startPoly)
            {
                startPolyFound = true;
                break;
            }
        }

        for (pathEndIndex = mPolyLength - 1; pathEndIndex > pathStartIndex; pathEndIndex--)
        {
             if (mPathPolyRefs[pathEndIndex] == endPoly)
             {
                 endPolyFound = true;
                 break;
             }
        }
    }

    if (startPolyFound && endPolyFound)
    {
        mPolyLength = pathEndIndex - pathStartIndex + 1;
        memmove(
            mPathPolyRefs.data(),
            mPathPolyRefs.data() + pathStartIndex,
            mPolyLength * sizeof(dtPolyRef)
        );
    }
    else if (startPolyFound && !endPolyFound)
    {
        mPolyLength -= pathStartIndex;

        // take ~80% of the original length
        // TODO : play with the values here
        uint32_t prefixPolyLength = (uint32_t)(mPolyLength * 0.8f + 0.5f);
        memmove(
            mPathPolyRefs.data(),
            mPathPolyRefs.data() + pathStartIndex,
            prefixPolyLength * sizeof(dtPolyRef)
        );

        float suffixEndPoint[VERTEX_SIZE];

        dtPolyRef suffixStartPoly = mPathPolyRefs[prefixPolyLength - 1];
        dtResult = mNavMeshQuery->closestPointOnPoly(
            suffixStartPoly,
            endPoint,
            suffixEndPoint,
            NULL
        );

        if (dtStatusFailed(dtResult))
        {
            --prefixPolyLength;
            suffixStartPoly = mPathPolyRefs[prefixPolyLength - 1];
            dtResult = mNavMeshQuery->closestPointOnPoly(
                suffixStartPoly,
                endPoint,
                suffixEndPoint,
                NULL
            );
            if (dtStatusFailed(dtResult))
            {
                // suffixStartPoly is still invalid, error state
				BuildShortcut();
                mType = PATHFIND_NOPATH;
                return;
            }
        }

        // generate suffix
        uint32_t suffixPolyLength = 0;
        dtResult = mNavMeshQuery->findPath(
            suffixStartPoly,
            endPoly,
            suffixEndPoint,
            endPoint,
            &mFilter,
            mPathPolyRefs.data() + prefixPolyLength - 1,
            (int*)&suffixPolyLength,
            MAX_PATH_LENGTH - prefixPolyLength
        );

        mPolyLength = prefixPolyLength + suffixPolyLength - 1;
    }
    else
    {
        Clear();

        dtResult = mNavMeshQuery->findPath(
            startPoly,
            endPoly,
            startPoint,
            endPoint,
            &mFilter,
            mPathPolyRefs.data(),
            (int*)&mPolyLength,
            MAX_PATH_LENGTH
        );

        if (!mPolyLength || dtStatusFailed(dtResult))
        {
            // only happens if we passed bad data to findPath(), or navmesh is messed up
			BuildShortcut();
            mType = PATHFIND_NOPATH;
            return;
        }
    }

    // by now we know what type of path we can get
    if (mPathPolyRefs[mPolyLength - 1] == endPoly && !(mType & PATHFIND_INCOMPLETE))
    {
        mType = PATHFIND_NORMAL;
    }
    else
    {
        mType = PATHFIND_INCOMPLETE;
    }

    // generate the point-path out of our up-to-date poly-path
    BuildPointPath(startPoint, endPoint);
}


void PathFinder::BuildPointPath(const float* startPoint, const float* endPoint)
{
    float pathPoints[MAX_POINT_PATH_LENGTH * VERTEX_SIZE];
    uint32_t pointCount = 0;
    dtStatus dtResult = DT_FAILURE;
    //if (mUseStraightPath)
    //{
    dtResult = mNavMeshQuery->findStraightPath(
        startPoint,         // start position
        endPoint,           // end position
        mPathPolyRefs.data(),     // current path
        mPolyLength,       // lenth of current path
        pathPoints,         // [out] path corner points
        NULL,               // [out] flags
        NULL,               // [out] shortened path
        (int*)&pointCount,
        MAX_POINT_PATH_LENGTH
    );   // maximum number of points/polygons to use
    //}
    //else
    //{
        //dtResult = findSmoothPath(
                       //startPoint,         // start position
                       //endPoint,           // end position
                       //mPathPolyRefs,     // current path
                       //mPolyLength,       // length of current path
                       //pathPoints,         // [out] path corner points
                       //(int*)&pointCount,
                       //mPointPathLimit);    // maximum number of points
    //}

    if (pointCount < 2 || dtStatusFailed(dtResult))
    {
        BuildShortcut();
        mType = PATHFIND_NOPATH;
        return;
    }

    mPathPoints.resize(pointCount);
    for (uint32_t i = 0; i < pointCount; ++i)
    {
        mPathPoints[i] = Vector3(
            pathPoints[i * VERTEX_SIZE + 2],
            pathPoints[i * VERTEX_SIZE],
            pathPoints[i * VERTEX_SIZE + 1]
        );
	}

    // first point is always our current location - we need the next one
    SetActualEndPosition(mPathPoints[pointCount - 1]);

    // force the given destination, if needed
    //if (mForceDestination &&
        //(!(mType & PATHFIND_NORMAL) || !inRange(getEndPosition(), getActualEndPosition(), 1.0f, 1.0f)))
    //{
        //// we may want to keep partial subpath
        //if (dist3DSqr(getActualEndPosition(), getEndPosition()) <
            //0.3f * dist3DSqr(getStartPosition(), getEndPosition()))
        //{
            //setActualEndPosition(getEndPosition());
            //mPathPoints[mPathPoints.size() - 1] = getEndPosition();
        //}
        //else
        //{
            //setActualEndPosition(getEndPosition());
            //BuildShortcut();
        //}

        //mType = PathType(PATHFIND_NORMAL | PATHFIND_NOT_USING_PATH);
    //}
}

dtPolyRef PathFinder::GetPolyByLocation(const float* point, float* distance) const
{
    dtPolyRef polyRef = INVALID_POLYREF;
    // dtPolyRef polyRef = GetPathPolyByPosition(
    //     mPathPolyRefs,
    //     mPolyLength,
    //     point,
    //     distance
    // );
    // if (polyRef != INVALID_POLYREF)
    // {
    //     return polyRef;
    // }

    float extents[VERTEX_SIZE] = {3.0f, 5.0f, 3.0f};    // bounds of poly search area
    float closestPoint[VERTEX_SIZE] = {0.0f, 0.0f, 0.0f};
    dtStatus dtResult = mNavMeshQuery->findNearestPoly(
        point,
        extents,
        &mFilter,
        &polyRef,
        closestPoint
    );
    if (dtStatusSucceed(dtResult) && polyRef != INVALID_POLYREF)
    {
        *distance = dtVdist(closestPoint, point);
        return polyRef;
    }

    extents[1] = 200.0f;
    dtResult = mNavMeshQuery->findNearestPoly(
        point,
        extents,
        &mFilter,
        &polyRef,
        closestPoint
    );
    if (dtStatusSucceed(dtResult) && polyRef != INVALID_POLYREF)
    {
        *distance = dtVdist(closestPoint, point);
        return polyRef;
    }

    return INVALID_POLYREF;
}


dtPolyRef PathFinder::GetPathPolyByPosition(
    const std::array<dtPolyRef, MAX_PATH_LENGTH>& polyPath,
    uint32_t polyPathSize,
    const float* point,
    float* distance) const
{
    if (!polyPathSize)
    {
        return INVALID_POLYREF;
    }

    dtPolyRef nearestPoly = INVALID_POLYREF;
    float minDist2d = FLT_MAX;
    float minDist3d = 0.0f;

    for (uint32_t i = 0; i < polyPathSize; ++i)
    {
        float closestPoint[VERTEX_SIZE];
        dtStatus dtResult = mNavMeshQuery->closestPointOnPoly(
            polyPath[i],
            point,
            closestPoint,
            NULL
        );

        if (dtStatusFailed(dtResult))
        {
            continue;
        }

        float d = dtVdist2DSqr(point, closestPoint);
        if (d < minDist2d)
        {
            minDist2d = d;
            nearestPoly = polyPath[i];
            minDist3d = dtVdistSqr(point, closestPoint);
        }

        if (minDist2d < 1.0f) // shortcut out - close enough for us
        {
            break;
        }
    }

    if (distance)
    {
        *distance = dtMathSqrtf(minDist3d);
    }

    return (minDist2d < 3.0f) ? nearestPoly : INVALID_POLYREF;
}

void PathFinder::Clear()
{
    mPolyLength = 0;
    mPathPoints.clear();
}

void PathFinder::BuildShortcut()
{
    LOG(INFO) << "Building shortcut...";
    Clear();
    mPathPoints.emplace_back(GetStartPosition());
    mPathPoints.emplace_back(GetActualEndPosition());
    mType = PATHFIND_SHORTCUT;
}

Vector3 PathFinder::GetStartPosition()
{
    return mStartPosition;
}

Vector3 PathFinder::GetEndPosition()
{
    return mEndPosition;
}

Vector3 PathFinder::GetActualEndPosition()
{
    return mActualEndPosition;
}

void PathFinder::SetStartPosition(const Vector3& point)
{
    mStartPosition = point;
}

void PathFinder::SetEndPosition(const Vector3& point)
{
    mEndPosition = point;
}

void PathFinder::SetActualEndPosition(const Vector3& point)
{
    mActualEndPosition = point;
}

const std::vector<Vector3>& PathFinder::GetPath() const
{
    return mPathPoints;
}

PathType PathFinder::GetType() const
{
    return mType;
}
