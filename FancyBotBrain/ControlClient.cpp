#include "ControlClient.h"


using boost::asio::ip::tcp;
using Json = nlohmann::json;

ControlClient::ControlClient(
    boost::asio::io_service& ioService,
    tcp_iterator endpointIterator,
    HandlerFn handler)
    : mIoService(ioService)
    , mpSocket()
    , mHandler(handler)
    , mEndpointIterator(endpointIterator)
{
    DoConnect();
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
        mpSocket->close();
    });
}

void
ControlClient::Reconnect()
{
    mpSocket->close();
    DoConnect();
}

void
ControlClient::OnMessage(const Json& json)
{
    mHandler(json);
}

void
ControlClient::DoConnect()
{
    mpSocket.reset(new tcp::socket(mIoService));
    boost::asio::async_connect(
        *mpSocket,
        mEndpointIterator,
        [this](boost::system::error_code ec, tcp::resolver::iterator)
        {
            if (!ec)
            {
                DoReadHeader();
            }
            else
            {
                Reconnect();
            }
        }
    );
}

void
ControlClient::DoReadHeader()
{
    boost::asio::async_read(
        *mpSocket,
        boost::asio::buffer((void*)&mReadMessage, ControlMessage::HEADER_LENGTH),
        [this](boost::system::error_code ec, std::size_t /*length*/)
        {
            if (!ec)
            {
                DoReadBody(mReadMessage.mBuffer, mReadMessage.mLength);
            }
            else
            {
                Reconnect();
            }
        }
    );
}

void
ControlClient::DoReadBody(char* start, uint32_t length)
{
    boost::asio::async_read(
        *mpSocket,
        boost::asio::buffer(start, length),
        [this, start, length]
        (boost::system::error_code ec, std::size_t lengthRead)
        {
            if (ec)
            {
                Reconnect();
            }
            else if (lengthRead == length)
            {
                std::string body(mReadMessage.mBuffer, mReadMessage.mLength);
				auto json = Json::parse(body);
				OnMessage(json);
                DoReadHeader();
            }
            else
            {
                DoReadBody(start + lengthRead, length - lengthRead);
            }
        }
    );
}

void
ControlClient::DoWrite()
{
    boost::asio::async_write(
        *mpSocket,
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
                Reconnect();
            }
        }
    );
}
