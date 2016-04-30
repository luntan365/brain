#include "MoveMapTile.h"

MoveMapTile::MoveMapTile(int mapId, int x, int y, dtTileRef tileRef)
    : mMapId(mapId)
    , mX(x)
    , mY(y)
    , mTileRef(tileRef)
{
}

