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

void Position::FromJson(const nlohmann::json& json)
{
    x = json["x"];
    y = json["y"];
    z = json["z"];
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

float Position::Distance2dSquared(const Vector3& other) const
{
    auto dx = other.x - x;
    auto dy = other.y - y;
    return dx * dx + dy * dy;
}

float Position::Distance2d(const Vector3& other) const
{
    return sqrt(Distance2dSquared(other));
}

nlohmann::json Position::ToJson() const
{
    nlohmann::json j;
    j["x"] = x;
    j["y"] = y;
    j["z"] = z;
    return j;
}

bool Position::operator==(const Vector3& v) const
{
    return v.x == x && v.y == y && v.z == z;
}

PositionPair::PositionPair(const Vector3& start, const Vector3& end)
    : startX(start.x)
    , startY(start.y)
    , startZ(start.z)
    , endX(end.x)
    , endY(end.y)
    , endZ(end.z)
{
}

Intersection::Intersection()
    : x(0.0)
    , y(0.0)
    , z(0.0)
    , rotation(0.0)
{
}

Circle::Circle()
    : center()
    , radius(0.0)
{
}

Circle::Circle(const Vector3& v, float r)
    : center(v)
    , radius(r)
{
}

bool Circle::ContainsPoint(const Vector3& p) const
{
    return center.Distance2dSquared(p) < radius * radius;
}

Position GetPointOnPath(const Vector3& from, const Vector3 to, float d)
{
    auto magnitude = Position(from).Distance(to);
    if (d < magnitude)
    {
        // unit vector * distance 
        auto x = (to.x - from.x) / magnitude * d;
        auto y = (to.y - from.y) / magnitude * d;
        auto z = (to.z - from.z) / magnitude * d;
        return Position(from.x + x, from.y + y, from.z + z);
    }
    else
    {
        return to;
    }
}
