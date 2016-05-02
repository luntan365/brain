#pragma once

#include <memory>
#include <unordered_map>

#include "MoveMapTile.h"
#include "DetourNavMesh.h"
#include "DetourNavMeshQuery.h"

class MoveMap {
public:
    MoveMap(int id, std::unique_ptr<dtNavMesh> pNavMesh);

    void LoadTile(MoveMapTile t);

    dtNavMesh* GetNavMesh();

    dtNavMeshQuery* GetNavMeshQuery();

private:
    const int mMapId;
    std::unique_ptr<dtNavMesh> mpNavMesh;
    std::unique_ptr<dtNavMeshQuery> mpNavMeshQuery;
    std::unordered_map<Point, MoveMapTile> mTiles;
};

