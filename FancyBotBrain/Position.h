#pragma once

#include "PathFinder.h"

class Position : public Vector3
{
public:
    Position();
    Position(float x, float y, float z);
    Position(const Vector3& v);

    float DistanceSquared(const Vector3& other) const;

    float Distance(const Vector3& other) const;

    bool operator ==(const Vector3& v) const;
};
