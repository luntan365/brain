#include <cstdlib>
#include <deque>
#include <iostream>
#include <thread>
#include <boost/asio.hpp>
#include "json.hpp"

typedef boost::asio::ip::tcp::resolver::iterator tcp_iterator;

using boost::asio::ip::tcp;
using Json = nlohmann::json;

struct ControlMessage
{
    static const uint64_t HEADER_LENGTH = 8;
    static const uint64_t MAX_BODY_LENGTH = 1024;
    char mHeader[4];
    uint32_t mLength;
    char mBuffer[MAX_BODY_LENGTH];
};

typedef std::deque<ControlMessage> ControlMessageQueue;

class ControlClient
{
public:
    ControlClient(boost::asio::io_service& ioService,
                  tcp_iterator endpointIterator)
        : mIoService(ioService)
        , mSocket(ioService)
    {
        DoConnect(endpointIterator);
    }

    void Write(const Json& json)
    {
        ControlMessage m;
        m.mHeader[0] = 'J';
        m.mHeader[1] = 'S';
        m.mHeader[2] = 'O';
        m.mHeader[3] = 'N';
        auto serialized = json.dump();
        m.mLength = serialized.size();
        std::copy(serialized.begin(), serialized.end(), m.mBuffer);
        Write(m);
    }

    void Write(const ControlMessage& msg)
    {
        mIoService.post(
            [this, msg]()
            {
                bool write_in_progress = !mWriteMessages.empty();
                mWriteMessages.push_back(msg);
                if (!write_in_progress)
                {
                    DoWrite();
                }
            }
        );
    }

    void Close()
    {
        mIoService.post([this]() {
            mSocket.close();
        });
    }

private:
    void OnMessage(const Json& json)
    {
        std::cout << json.dump() << "\n";
    }

    void DoConnect(tcp::resolver::iterator endpointIterator)
    {
        boost::asio::async_connect(
            mSocket,
            endpointIterator,
            [this](boost::system::error_code ec, tcp::resolver::iterator)
            {
                if (!ec)
                {
                    DoReadHeader();
                }
            }
        );
    }

    void DoReadHeader()
    {
        boost::asio::async_read(
            mSocket,
            boost::asio::buffer((void*)&mReadMessage, ControlMessage::HEADER_LENGTH),
            [this](boost::system::error_code ec, std::size_t /*length*/)
            {
                if (!ec)
                {
                    DoReadBody();
                }
                else
                {
                    mSocket.close();
                }
            }
        );
    }

    void DoReadBody()
    {
        boost::asio::async_read(
            mSocket,
            boost::asio::buffer(mReadMessage.mBuffer, mReadMessage.mLength),
            [this](boost::system::error_code ec, std::size_t /*length*/)
            {
                if (!ec)
                {
                    std::string body(mReadMessage.mBuffer, mReadMessage.mLength);
                    OnMessage(Json::parse(body));
                    DoReadHeader();
                }
                else
                {
                    mSocket.close();
                }
            }
        );
    }

    void DoWrite()
    {
        boost::asio::async_write(
            mSocket,
            boost::asio::buffer(
                (void*)&mWriteMessages.front(),
                mWriteMessages.front().mLength + ControlMessage::HEADER_LENGTH
            ),
            [this](boost::system::error_code ec, std::size_t /*length*/)
            {
                if (!ec)
                {
                    mWriteMessages.pop_front();
                    if (!mWriteMessages.empty())
                    {
                        DoWrite();
                    }
                }
                else
                {
                    mSocket.close();
                }
            }
        );
    }

private:
    boost::asio::io_service& mIoService;
    tcp::socket mSocket;
    ControlMessage mReadMessage;
    ControlMessageQueue mWriteMessages;
};

int main(int argc, char* argv[])
{
    try
    {
        if (argc != 3)
        {
            std::cerr << "Usage: ControlClient <host> <port>\n";
            return 1;
        }

        boost::asio::io_service ioService;

        tcp::resolver resolver(ioService);
        tcp::resolver::query query(argv[1], argv[2]);
        auto endpointIterator = resolver.resolve(query);
        ControlClient c(ioService, endpointIterator);

        std::thread t([&ioService](){ ioService.run(); });

        char line[ControlMessage::MAX_BODY_LENGTH + 1];
        while (std::cin.getline(line, ControlMessage::MAX_BODY_LENGTH + 1))
        {
            auto j = Json::parse(line);
            c.Write(j);
        }

        c.Close();
        t.join();
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}
