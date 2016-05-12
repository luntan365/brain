#pragma once

#include "GameState.h"

class IBotConfig;

class IBot
{
public:
    virtual ~IBot()
    {
    }

    virtual void OnStart() = 0;

    virtual void OnStop() = 0;

    virtual IBotConfig* GetConfig() = 0;

    virtual IBotConfig* GetProfile() = 0;

    virtual concurrency::task<void> Tick(GameState& state) = 0;
};
