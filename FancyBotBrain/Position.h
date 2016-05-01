#pragma once

class Position
{
public:
    Position(float xx, float yy, float zz);

    Position();

    float DistanceSquared(const Position& other) const;

    float Distance(const Position& other) const;

    float x;
    float y;
    float z;
};
