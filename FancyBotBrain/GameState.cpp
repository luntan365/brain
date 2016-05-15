#include "stdafx.h"
#include "GameState.h"

#include "Lua.h"
#include "WowOffsets.h"


GameState::GameState()
    : mIsInGame(false)
    , mFirstTick(true)
    , mTickCount(0)
    , mObjectManager()
    , mMerchantPane()
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
	if (mIsInGame)
	{
        if (mFirstTick)
        {
            RegisterLuaFunctions();
            mFirstTick = false;
        }
        mObjectManager.UpdatePlayer();
        if (mTickCount % 100)
        {
    		UpdateInGame();
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
