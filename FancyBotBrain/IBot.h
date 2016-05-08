#pragma once

#include "GameState.h"


class IBot
{
public:
    virtual ~IBot()
    {
    }

    virtual void OnStart() = 0;
    virtual concurrency::task<void> Tick(GameState& state) = 0;
};
