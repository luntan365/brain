#include "MoveMap.h"

#include "easylogging++.h"

MoveMap::MoveMap(int id, std::unique_ptr<dtNavMesh> pNavMesh)
    : mMapId(id)
    , mpNavMesh(std::move(pNavMesh))
    , mTiles()
{
}

void MoveMap::LoadTile(MoveMapTile t)
{
    mTiles.emplace(std::make_tuple(t.mX, t.mY), std::move(t));
}

dtNavMesh* MoveMap::GetNavMesh()
{
    return mpNavMesh.get();
}

