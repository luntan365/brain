#include "stdafx.h"
#include "GameState.h"

#include "WowOffsets.h"


GameState::GameState()
    : mIsInGame(false)
    , mObjectManager()
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


void GameState::Update()
{
	mIsInGame = Offsets::IsInGame::Read();
	if (mIsInGame)
	{
		UpdateInGame();
	}
	else
	{

	}
}


void GameState::UpdateInGame()
{
    Offsets::ObjectManager::Read(&mObjectManager);
}


bool GameState::GetIsInGame() const
{
    return mIsInGame;
}
