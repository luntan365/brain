#include "ControlClient.h"


using boost::asio::ip::tcp;
using Json = nlohmann::json;

ControlClient::ControlClient(
    boost::asio::io_service& ioService,
    tcp_iterator endpointIterator,
    HandlerFn handler)
    : mIoService(ioService)
    , mSocket(ioService)
    , mHandler(handler)
{
    DoConnect(endpointIterator);
}

void
ControlClient::Write(const Json& json)
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

void
ControlClient::Write(const ControlMessage& msg)
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

void
ControlClient::Close()
{
    mIoService.post([this]() {
        mSocket.close();
    });
}

void
ControlClient::OnMessage(const Json& json)
{
    mHandler(json);
}

void
ControlClient::DoConnect(tcp::resolver::iterator endpointIterator)
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

void
ControlClient::DoReadHeader()
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

void
ControlClient::DoReadBody()
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

void
ControlClient::DoWrite()
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
