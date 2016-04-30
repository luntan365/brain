#pragma once

#include <memory>
#include <unordered_map>

#include "MoveMapTile.h"
#include "DetourNavMesh.h"

class MoveMap {
public:
    MoveMap(int id, std::unique_ptr<dtNavMesh> pNavMesh);

    void LoadTile(MoveMapTile t);

    dtNavMesh* GetNavMesh();

private:
    const int mMapId;
    std::unique_ptr<dtNavMesh> mpNavMesh;
    std::unordered_map<Point, MoveMapTile> mTiles;
};

