#include "Position.h"

#include <cmath>

Position::Position()
    : Position(0, 0, 0)
{
}

Position::Position(float x, float y, float z)
    : Vector3(x, y, z)
{
}

Position::Position(const Vector3& v)
    : Vector3(v)
{
}

float Position::DistanceSquared(const Vector3& other) const
{
    auto dx = other.x - x;
    auto dy = other.y - y;
    auto dz = other.z - z;
    return dx * dx + dy * dy + dz * dz;
}

float Position::Distance(const Vector3& other) const
{
    return sqrt(DistanceSquared(other));
}

bool Position::operator==(const Vector3& v) const
{
    return v.x == x && v.y == y && v.z == z;
}
