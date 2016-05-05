#pragma once

#include "IBot.h"
#include "MoveMapManager.h"
#include "PathTracker.h"


struct GrindBotConfiguration
{
    Position mVendorPosition;

};


class GrindBot : public IBot
{
public:
    GrindBot();

    virtual void OnStart();

    virtual void Tick(GameState& state);

private:
    void MoveTo(const WoWPlayer& me, const Position& p);

    MoveMapManager mMoveMapManager;
    PathTracker mPathTracker;
    uint32_t mCurrentMapId;

};


AddBpt(0x005dd500);
AddBpt(0x005e9aa0);
AddBpt(0x005e9bc0);
AddBpt(0x005dec90);
AddBpt(0x005f1f70);
AddBpt(0x005f1f10);
AddBpt(0x005f1f30);
AddBpt(0x00469f10);
AddBpt(0x005f1f40);
AddBpt(0x005f1f60);
AddBpt(0x00600320);
AddBpt(0x00605960);
AddBpt(0x00608e00);
AddBpt(0x0060ce70);
AddBpt(0x005ec950);
AddBpt(0x00608560);
AddBpt(0x005ec8f0);
AddBpt(0x00607ea0);
AddBpt(0x00608d50);
AddBpt(0x005f1ea0);
AddBpt(0x005f1eb0);
AddBpt(0x0060be60);
AddBpt(0x005f1ec0);
AddBpt(0x00469ff0);
AddBpt(0x0060bea0);
AddBpt(0x0046a010);
AddBpt(0x005f1ee0);
AddBpt(0x0060d2d0);
AddBpt(0x005f1ed0);
AddBpt(0x0046a080);
AddBpt(0x00614fe0);
AddBpt(0x005ec9e0);
AddBpt(0x005eca40);
AddBpt(0x005eca80);
AddBpt(0x0062f880);
AddBpt(0x0062fa30);
AddBpt(0x0062fb70);
AddBpt(0x005ec430);
AddBpt(0x005ec240);
AddBpt(0x005ec350);
AddBpt(0x005ec380);
AddBpt(0x005ed570);
AddBpt(0x005ea690);
AddBpt(0x005eda20);
AddBpt(0x005eda80);
AddBpt(0x005ee150);
AddBpt(0x005f1f80);
AddBpt(0x005f1060);
AddBpt(0x005f10e0);
AddBpt(0x005f16f0);
AddBpt(0x005f11a0);
AddBpt(0x005f0e90);
AddBpt(0x005f0ee0);
AddBpt(0x005f1630);
AddBpt(0x005f0fa0);
