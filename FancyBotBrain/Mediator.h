#pragma once

#include "ControlClient.h"
#include "GrindBot.h"

#include <deque>
#include <mutex>
#include <thread>
#include <vector>

class Mediator
{
public:
    ~Mediator();

    static Mediator& Instance();

    void Start();

    void Stop();

private:
    Mediator();

    void StartControlClient();

    void OnControlMessage(const nlohmann::json& json);

    void HandleControlRequests();

    void RunBotThread();

    void BotIteration();

    void StartBot();

    void StopBot();

private:
    std::unique_ptr<ControlClient> mpControl;
    std::unique_ptr<IBot> mpBot;

    std::mutex mMessageMutex;
    std::deque<nlohmann::json> mMessageQueue;

    std::vector<std::thread> mThreads;

    std::atomic<bool> mStartBot;
    std::atomic<bool> mStopBot;
    bool mBotRunning;
};
