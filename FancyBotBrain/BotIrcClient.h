#pragma once

#include <chrono>
#include "IRCClient.h"

class BotIrcClient
{
public:
    static BotIrcClient& Instance();

    void Connect(std::string host, uint16_t port);

    void SetName(const std::string& nickname);

    void Log(const std::string& message);

    void JoinChannel(const std::string& channel);

    void StartLoop();

private:
    BotIrcClient();

    IRCClient mClient;
    std::string mChannel;
    std::chrono::milliseconds mLastSend;
};
