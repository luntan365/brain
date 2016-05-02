#define _CRT_SECURE_NO_WARNINGS
#include "MoveMapManager.h"

#include "easylogging++.h"

INITIALIZE_EASYLOGGINGPP

#include <iterator>
#include <boost/filesystem.hpp>
#include <boost/range/adaptors.hpp>

namespace bfs = boost::filesystem;
namespace ba = boost::adaptors;


MoveMapManager::MoveMapManager()
{

}

void MoveMapManager::Initialize(const std::string& mmapRoot)
{
    LoadMaps(mmapRoot);
    LoadMapTiles(mmapRoot);
}

void ForEachFileGlob(
        const bfs::path& dir,
        const boost::regex& filter,
        std::function<void (const bfs::path& filepath)> fn)
{
    boost::smatch what;

    for (auto &entry : boost::make_iterator_range(bfs::directory_iterator(dir), {})
    | ba::filtered(static_cast<bool (*)(const bfs::path &)>(&bfs::is_regular_file))
    | ba::filtered([&](const bfs::path &path){ return boost::regex_match(path.filename().string(), what, filter); })
    )
    {
        fn(entry.path());
    }
}

void MoveMapManager::LoadMaps(const std::string& path)
{
    ForEachFileGlob(
        boost::filesystem::path(path),
        boost::regex(".*.mmap"),
        [this] (const bfs::path& p) {
            LoadMapFromFile(p.string());
        }
    );
}

void MoveMapManager::LoadMapTiles(const std::string& path)
{
    ForEachFileGlob(
        boost::filesystem::path(path),
        boost::regex(".*.mmtile"),
        [this] (const bfs::path& p) {
            LoadMapTileFromFile(p.string());
        }
    );
}

void MoveMapManager::LoadMapFromFile(const std::string& spath)
{
	boost::filesystem::path path(spath);
    auto mapId = (uint32_t)std::stoul(path.stem().string());
    std::ifstream input(path.string());
    dtNavMeshParams params;
    input.read((char*)&params, sizeof(dtNavMeshParams));

    std::unique_ptr<dtNavMesh> pMesh(dtAllocNavMesh());
    pMesh->init(&params);

    MoveMap m(mapId, std::move(pMesh));
    mMoveMaps.emplace(mapId, std::move(m));
	LOG(INFO) << "Loaded map " << mapId;
}

void MoveMapManager::LoadMapTileFromFile(const std::string& spath)
{
	boost::filesystem::path path(spath);
    int mapId;
    int x;
    int y;
	const std::string sPath = path.stem().string();
	const char* cPath = sPath.c_str();
    std::sscanf(
        cPath,
        "%03d%02d%02d",
        &mapId,
        &x,
        &y
    );
    std::ifstream input(path.string(), std::ios::binary);
    MmapTileHeader tileHeader;
    input.read((char*)&tileHeader, sizeof(tileHeader));

    unsigned int byteCount = (unsigned int)tileHeader.size;
    unsigned char* data = (unsigned char*)dtAlloc(byteCount, DT_ALLOC_PERM);
	input.read((char*)data, tileHeader.size);

    auto& mmap = mMoveMaps.at(mapId);
    dtTileRef tileRef = 0;
    mmap.GetNavMesh()->addTile((unsigned char*)data, tileHeader.size, DT_TILE_FREE_DATA, 0, &tileRef);

    MoveMapTile tile(mapId, x, y, tileRef);
    mmap.LoadTile(tile);
    LOG(INFO) << "Loaded " << mapId << " (" << x << ", " << y << ")";
}

dtNavMesh* MoveMapManager::GetNavMesh(uint32_t mapId)
{
    if (mMoveMaps.find(mapId) == mMoveMaps.end())
    {
        return nullptr;
    }
    else
    {
        return mMoveMaps.at(mapId).GetNavMesh();
    }
}

dtNavMeshQuery* MoveMapManager::GetNavMeshQuery(uint32_t mapId)
{
    if (mMoveMaps.find(mapId) == mMoveMaps.end())
    {
        return nullptr;
    }
    else
    {
        return mMoveMaps.at(mapId).GetNavMeshQuery();
    }
}
