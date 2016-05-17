#pragma once

#include <mutex>
#include "json.hpp"
#include "GossipPane.h"
#include "MerchantPane.h"
#include "WoWObjectManager.h"
#include <chrono>

class GameState
{
public:
	static GameState& Instance();

    std::unique_lock<std::mutex> GetLock();

	~GameState();

	void Update();

    bool GetIsInGame() const;

    WoWObjectManager& ObjectManager();

    const MerchantPane& GetMerchantPane() const;

    const GossipPane& GetGossipPane() const;

    nlohmann::json ToJson() const;

private:
	GameState();

    void UpdateInGame();

private:
	bool mIsInGame;
    bool mFirstTick;
    uint64_t mTickCount;
    std::mutex mMutex;

    WoWObjectManager mObjectManager;
    GossipPane mGossipPane;
    MerchantPane mMerchantPane;

    std::chrono::time_point<std::chrono::steady_clock> mLastTick;
};
