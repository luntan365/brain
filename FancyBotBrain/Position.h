#pragma once

class Position
{
public:
    Position(float xx, float yy, float zz)
        : x(xx)
        , y(yy)
        , z(zz)
    {
    }

    Position()
        : x(0)
        , y(0)
        , z(0)
    {
    }

    float x;
    float y;
    float z;
};
