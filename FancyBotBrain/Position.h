#pragma once

#include "json.hpp"
#include "PathFinder.h"

class Position : public Vector3
{
public:
    Position();
    Position(float x, float y, float z);
    Position(const Vector3& v);

    void FromJson(const nlohmann::json& json);

    float DistanceSquared(const Vector3& other) const;

    float Distance(const Vector3& other) const;

    float Distance2dSquared(const Vector3& other) const;

    float Distance2d(const Vector3& other) const;

    nlohmann::json ToJson() const;

    bool operator ==(const Vector3& v) const;
};

struct PositionPair
{
    PositionPair(const Vector3& start, const Vector3& end);

    float startX;
    float startY;
    float startZ;
    float endX;
    float endY;
    float endZ;
};

struct Intersection
{
    Intersection();

    float x;
    float y;
    float z;
    float rotation;
};

struct Circle
{
    Circle();

    Circle(const Vector3& v, float r);

    bool ContainsPoint(const Vector3& p) const;

    Position center;
    float radius;
};

Position GetPointOnPath(const Vector3& from, const Vector3 to, float d);
