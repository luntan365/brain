#pragma once

#include "GameState.h"

class ICustomClass
{
public:
    virtual ~ICustomClass()
    {
    }

    virtual concurrency::task<void>
    DoPull(const WoWPlayer& me, GameState& state) = 0;

    virtual concurrency::task<void>
    DoCombat(const WoWPlayer& me, GameState& state) = 0;

    // return true to keep resting
    virtual concurrency::task<bool>
    DoRest(const WoWPlayer& me, GameState& state) = 0;

    // Return true to keep buffing
    virtual concurrency::task<bool>
    DoBuff(const WoWPlayer& me, GameState& state) = 0;
};
