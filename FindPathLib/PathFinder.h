#pragma once

#include "DetourNavMesh.h"
#include "DetourNavMeshQuery.h"

#include "MoveMapManager.h"

#include <array>
#include <vector>

static const int MAX_PATH_LENGTH = 74;
static const int MAX_POINT_PATH_LENGTH = 74;

#define INVALID_POLYREF 0

struct Vector3
{
    Vector3(float x, float y, float z);
    Vector3();

    float x;
    float y;
    float z;
};

enum NavTerrain
{
    NAV_EMPTY   = 0x00,
    NAV_GROUND  = 0x01,
    NAV_MAGMA   = 0x02,
    NAV_SLIME   = 0x04,
    NAV_WATER   = 0x08,
    NAV_UNUSED1 = 0x10,
    NAV_UNUSED2 = 0x20,
    NAV_UNUSED3 = 0x40,
    NAV_UNUSED4 = 0x80
    // we only have 8 bits
};

enum PathType
{
    PATHFIND_BLANK          = 0x0000,   // path not built yet
    PATHFIND_NORMAL         = 0x0001,   // normal path
    PATHFIND_SHORTCUT       = 0x0002,   // travel through obstacles, terrain, air, etc (old behavior)
    PATHFIND_INCOMPLETE     = 0x0004,   // we have partial path to follow - getting closer to target
    PATHFIND_NOPATH         = 0x0008,   // no valid path at all or error in generating one
    PATHFIND_NOT_USING_PATH = 0x0010    // used when we are either flying/swiming or on map w/o mmaps
};

class PathFinder
{
public:
    PathFinder(MoveMapManager* pMMM, uint32_t mapId);

    void Calculate(const Vector3& start, const Vector3& end);

    const std::vector<Vector3>& GetPath() const;

    PathType GetType() const;

private:
    void Clear();

    dtPolyRef GetPathPolyByPosition(
        const std::array<dtPolyRef, MAX_PATH_LENGTH>& polyPath,
        uint32_t polyPathSize,
        const float* point,
        float* distance
    ) const;

    dtPolyRef GetPolyByLocation(const float* point, float* distance) const;

    void BuildPointPath(const float* startPoint, const float* endPoint);

    void BuildShortcut();

    Vector3 GetStartPosition();

    Vector3 GetEndPosition();

    Vector3 GetActualEndPosition();

    void SetStartPosition(const Vector3& point);

    void SetEndPosition(const Vector3& point);

    void SetActualEndPosition(const Vector3& point);

private:
    std::array<dtPolyRef, MAX_PATH_LENGTH> mPathPolyRefs;
    uint32_t mPolyLength;
    PathType mType;
    std::vector<Vector3> mPathPoints;

    const dtNavMesh* mNavMesh;
    const dtNavMeshQuery* mNavMeshQuery;
    dtQueryFilter mFilter;

    Vector3 mStartPosition;
    Vector3 mEndPosition;
    Vector3 mActualEndPosition;
};
