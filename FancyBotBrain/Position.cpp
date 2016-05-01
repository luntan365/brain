#include "Position.h"

#include <cmath>

Position::Position(float xx, float yy, float zz)
    : x(xx)
    , y(yy)
    , z(zz)
{
}

Position::Position()
    : x(0)
    , y(0)
    , z(0)
{
}

float Position::DistanceSquared(const Position& other) const
{
    auto dx = other.x - x;
    auto dy = other.y - y;
    auto dz = other.z - z;
    return dx * dx + dy * dy + dz * dz;
}

float Position::Distance(const Position& other) const
{
    return sqrt(DistanceSquared(other));
}

