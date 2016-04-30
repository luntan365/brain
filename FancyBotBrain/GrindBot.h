#pragma once

#include "IBot.h"


class GrindBot : public IBot
{
public:
    virtual void OnStart();

    virtual void Tick(GameState& state);

};
