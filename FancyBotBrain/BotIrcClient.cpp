#include "BotIrcClient.h"

BotIrcClient::BotIrcClient()
    : mClient()
    , mChannel("")
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
    mClient.SendIRC("PRIVMSG " + mChannel + " :" + message);
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
