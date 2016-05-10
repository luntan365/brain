#include "Mediator.h"

#include "BotIrcClient.h"
#include <chrono>

using namespace std::chrono_literals;
using boost::asio::ip::tcp;

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

    mMoveMapManager.Initialize("C:\\mmaps");
    irc.Log("Move Maps Loaded.");

    mThreads.emplace_back([this] {
        StartControlClient();
    });
    mThreads.emplace_back([this] {
        RunBotThread();
    });
}

void
Mediator::Stop()
{
    mpControl->Close();
    StopBot();

    for (auto& thread : mThreads)
    {
        thread.join();
    }
}

Mediator::Mediator()
    : mMoveMapManager()
    , mpControl()
    , mpBot()
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
    mpControl.reset(
        new ControlClient(
            ioService,
            endpointIterator,
            [this](const nlohmann::json& json) {
                OnControlMessage(json);
            }
        )
    );
    ioService.run();
}

void
Mediator::OnControlMessage(const nlohmann::json& json)
{
    std::unique_lock<std::mutex> lock(mMessageMutex);
    mMessageQueue.push_back(json);
}

void
Mediator::HandleControlRequests()
{
    std::unique_lock<std::mutex> lock(mMessageMutex);
    while (!mMessageQueue.empty())
    {
        const auto& json = mMessageQueue.front();

        const std::string& type = json["type"];
        bool success = false;
        if (type == "start")
        {
            StartBot();
            success = true;
        }
        else if (type == "stop")
        {
            StopBot();
            success = true;
        }
        nlohmann::json response;
        response["type"] = "response";
        response["success"] = success;
        if (json.find("id") != json.end())
        {
            response["request-id"] = json["id"];
        }
        mpControl->Write(response);

        mMessageQueue.pop_front();
    }
}

void
Mediator::RunBotThread()
{
    while (true)
    {
        HandleControlRequests();
        if (mStartBot)
        {
            mpBot->OnStart();
            mBotRunning = true;
            mStartBot = false;
            mStopBot = false;
        }
        else if (mStopBot)
        {
            mpBot->OnStop();
            mBotRunning = false;
            mStopBot = false;
            mStartBot = false;
        }
        else if (mBotRunning)
        {
            BotIteration();
        }
    }
}

void
Mediator::BotIteration()
{
    std::this_thread::sleep_for(100ms);
    auto& gs = GameState::Instance();
    concurrency::task<void> curTask;
    {
        auto lock = gs.GetLock();
        if (gs.ObjectManager().GetPlayer().GetAddress() == nullptr)
        {
            // Object manager hasn't been tick'd yet
            return;
        }
        lock.lock();
        curTask = mpBot->Tick(gs);
    }
    if (!curTask.is_done())
    {
        curTask.wait();
    }
}

void
Mediator::StopBot()
{
    if (mBotRunning)
    {
        mStopBot = true;
        mStartBot = false;
    }
}

void
Mediator::StartBot()
{
    if (!mBotRunning)
    {
        mpBot.reset(new GrindBot(&mMoveMapManager));
        mStartBot = true;
        mStopBot = false;
    }
}
