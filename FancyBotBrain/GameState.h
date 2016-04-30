#pragma once

#include "WoWObjectManager.h"

class GameState
{
public:
	static GameState& Instance();

	~GameState();

	void Update();

    bool GetIsInGame() const;

    WoWObjectManager& ObjectManager();

private:
	GameState();

    void UpdateInGame();

private:
	bool mIsInGame;
    WoWObjectManager mObjectManager;
};

