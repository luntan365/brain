#include "stdafx.h"
#include "GameState.h"

#include "Lua.h"
#include "WowOffsets.h"

using namespace std::chrono_literals;

GameState::GameState()
    : mIsInGame(false)
    , mFirstTick(true)
    , mTickCount(0)
    , mObjectManager()
    , mMerchantPane(this)
{
}

GameState::~GameState()
{
}

GameState& GameState::Instance()
{
	static GameState state;
	return state;
}

std::unique_lock<std::mutex> GameState::GetLock()
{
    std::unique_lock<std::mutex> lock(mMutex, std::defer_lock);
    return lock;
}

void GameState::Update()
{
	mIsInGame = Offsets::IsInGame::Read();
    UpdatePerformanceCounter();
	if (mIsInGame)
	{
        if (mFirstTick)
        {
            RegisterLuaFunctions();
            mFirstTick = false;
        }
        mObjectManager.UpdatePlayer();
        if (std::chrono::steady_clock::now() - mLastTick > 10ms)
        {
    		UpdateInGame();
            mLastTick = std::chrono::steady_clock::now();
        }
	}
	else
	{

	}
    mTickCount++;
}

void GameState::UpdateInGame()
{
    Offsets::ObjectManager::Read(&mObjectManager);
    mGossipPane.Read();
    mMerchantPane.Read();
}


bool GameState::GetIsInGame() const
{
    return mIsInGame;
}


WoWObjectManager& GameState::ObjectManager()
{
    return mObjectManager;
}


const MerchantPane& GameState::GetMerchantPane() const
{
    return mMerchantPane;
}


const GossipPane& GameState::GetGossipPane() const
{
    return mGossipPane;
}


uint32_t GameState::GetPerformanceCounter() const 
{
    return mPerformanceCounter;
}

nlohmann::json GameState::ToJson() const
{
    if (mIsInGame)
    {
        auto j = mObjectManager.ToJson();
        j["in_game"] = true;
        return j;
    }
    else
    {
        nlohmann::json j({});
        j["in_game"] = false;
        return j;
    }
}

void GameState::UpdatePerformanceCounter() 
{
    typedef uint32_t (*GetPerfCounterFn)();
    mPerformanceCounter = 
        ((GetPerfCounterFn)StaticFields::STATIC_GET_PERFORMANCE_COUNTER)();
}
