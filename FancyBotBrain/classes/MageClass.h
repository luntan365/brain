#pragma once

#include "ICustomClass.h"
#include "GrindBot.h"

struct MageConfig : public GrindBotConfiguration
{
    MageConfig(const GrindBotConfiguration& config);
};

class MageClass : public ICustomClass
{
public:
    MageClass(const MageConfig& config, GrindBot* pBot);

    virtual concurrency::task<void>
    DoPull(const WoWPlayer& me, GameState& state);

    virtual concurrency::task<void>
    DoCombat(const WoWPlayer& me, GameState& state);

    virtual concurrency::task<bool>
    DoRest(const WoWPlayer& me, GameState& state);

    virtual concurrency::task<bool>
    DoBuff(const WoWPlayer& me, GameState& state);

private:
    MageConfig mConfig;
    GrindBot* mpBot;
};
