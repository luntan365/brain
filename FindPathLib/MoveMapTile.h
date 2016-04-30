#pragma once

#include "DetourNavMesh.h"

#include <functional>

typedef std::tuple<int, int> Point;

namespace std {
    template<> struct hash<Point> {
    public:
        size_t operator()(const Point& point) const {
            return hash<int>()(std::get<0>(point)) ^ hash<int>()(std::get<1>(point));
        }
    };
}

class MoveMapTile {
public:
    MoveMapTile(int mapId, int x, int y, dtTileRef tileRef);

    const int mMapId;
    const int mX;
    const int mY;
    const dtTileRef mTileRef;
};

