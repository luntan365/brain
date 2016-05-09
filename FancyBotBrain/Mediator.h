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

    void RunBotThread();

private:
    std::unique_ptr<ControlClient> mpControl;
    GrindBot mBot;

    std::mutex mMessageMutex;
    std::deque<nlohmann::json> mMessageQueue;

    std::vector<std::thread> mThreads;
};
