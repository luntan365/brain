#pragma once

#include "PathFinder.h"

class Position : public Vector3
{
public:
    Position();
    Position(float x, float y, float z);
    Position(const Vector3& v);

    float DistanceSquared(const Position& other) const;

    float Distance(const Position& other) const;
};
