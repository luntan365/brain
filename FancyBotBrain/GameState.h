#pragma once

#include "WoWObjectManager.h"

class GameState
{
public:
	static GameState& Instance();

	~GameState();

	void Update();

    bool GetIsInGame() const;

private:
	GameState();

    void UpdateInGame();

public:
	bool mIsInGame;
    WoWObjectManager mObjectManager;
};

