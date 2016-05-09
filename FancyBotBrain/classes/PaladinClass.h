#pragma once

#include "ICustomClass.h"
#include "GrindBot.h"

struct PaladinConfig : public GrindBotConfiguration
{
    PaladinConfig(const GrindBotConfiguration& config);

    bool mUseCrusader;
    bool mUseBlessing;
    std::string mBlessing;
};

class PaladinClass : public ICustomClass
{
public:
    PaladinClass(const PaladinConfig& config, GrindBot* pBot);

    virtual concurrency::task<void>
    DoPull(const WoWPlayer& me, GameState& state);

    virtual concurrency::task<void>
    DoCombat(const WoWPlayer& me, GameState& state);

    virtual concurrency::task<bool>
    DoRest(const WoWPlayer& me, GameState& state);

    virtual concurrency::task<bool>
    DoBuff(const WoWPlayer& me, GameState& state);

private:
    concurrency::task<void>
    SealJudgementRotation(const WoWPlayer& me, GameState& state);

    PaladinConfig mConfig;
    GrindBot* mpBot;
};
