#include "Mediator.h"

Mediator::~Mediator()
{
    Stop();
}

Mediator& Mediator::Instance()
{
    static Mediator sMediator;
    return sMediator;
}

void Mediator::Start()
{
    auto& irc = BotIrcClient::Instance();
    irc.Connect("irc.freenode.net", 6667);
    irc.SetName("Phuzad_Bot0");
    irc.JoinChannel("#phuzad-land");
    irc.Log("Hello World!");

    mThreads.emplace_back([this] {
        StartControlClient();
    });
    mThread.emplace_back([this] {
        RunBotThread();
    });
}

void Stop()
{
    mpControl->Close();
    mBot.Stop();

    for (auto& thread : mThreads)
    {
        thread.join();
    }
}

Mediator::Mediator()
    : mpControl()
    , mBot()
    , mMessageMutex()
    , mMessageQueue()
    , mThreads()
{
}

void
Mediator::StartControlClient()
{
    boost::asio::io_service ioService;
    tcp::resolver resolver(ioService);
    tcp::resolver::query query("localhost", "1337");
    auto endpointIterator = resolver.resolve(query);
    mpControl.reset(new ControlClient(ioService, endpointIterator));
    ioService.run();
}

void
Mediator::OnControlMessage(const nlohmann::json& json)
{
    std::unique_lock<std::mutex> lock(mMessageMutex);
    mMessageQueue.push(json);
}

void
Mediator::RunBotThread()
{
    bot.OnStart();
}
