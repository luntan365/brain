#include "BotIrcClient.h"

using namespace std::chrono;

BotIrcClient::BotIrcClient()
    : mClient()
    , mChannel()
    , mLastSend(0)
{
}

BotIrcClient& BotIrcClient::Instance()
{
    static BotIrcClient sClient;
    return sClient;
}

void BotIrcClient::Connect(std::string host, uint16_t port)
{
    if (mClient.Connected())
    {
        return;
    }
    mClient.InitSocket();
    char* hostStr = (char*)host.c_str();
    mClient.Connect(hostStr, port);
}

void BotIrcClient::SetName(const std::string& nickname)
{
    mClient.Login(nickname, nickname);
}

void BotIrcClient::Log(const std::string& message)
{
    milliseconds currentTimeMs = duration_cast<milliseconds>(
        system_clock::now().time_since_epoch()
    );
    if (mLastSend + 500ms < currentTimeMs)
    {
        mClient.SendIRC("PRIVMSG " + mChannel + " :" + message);
        mLastSend = currentTimeMs;
    }
}

void BotIrcClient::JoinChannel(const std::string& channel)
{
    mChannel = channel;
    mClient.SendIRC("JOIN " + channel);
}

void BotIrcClient::StartLoop()
{
    // while (client.Connected())
    // {
    //     client.ReceiveData();
    // }
}
