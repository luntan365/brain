#pragma once

#include "MoveMap.h"

#include <cstdint>
#include <functional>
#include <unordered_map>

#define MMAP_MAGIC 0x4d4d4150   // 'MMAP'
#define MMAP_VERSION 4
struct MmapTileHeader
{
    uint32_t mmapMagic;
    uint32_t dtVersion;
    uint32_t mmapVersion;
    uint32_t size;
    bool usesLiquids : 1;

    MmapTileHeader() : mmapMagic(MMAP_MAGIC), dtVersion(DT_NAVMESH_VERSION),
        mmapVersion(MMAP_VERSION), size(0), usesLiquids(true) {}
};

class MoveMapManager {
public:
    MoveMapManager();

    void Initialize(const std::string& path);

    dtNavMesh* GetNavMesh(uint32_t mapId);

    dtNavMeshQuery* GetNavMeshQuery(uint32_t mapId);

private:
    void LoadMaps(const std::string& path);
    void LoadMapFromFile(const std::string& path);
    void LoadMapTiles(const std::string& path);
    void LoadMapTileFromFile(const std::string& path);

    std::unordered_map<int, MoveMap> mMoveMaps;

};
