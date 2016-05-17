#pragma once

#include <cstdlib>
#include <deque>
#include <iostream>
#include <thread>
#include <boost/asio.hpp>
#include "json.hpp"


struct ControlMessage
{
    static const uint64_t HEADER_LENGTH = 8;
    static const uint64_t MAX_BODY_LENGTH = 64 * 1024;
    char mHeader[4];
    uint32_t mLength;
    char mBuffer[MAX_BODY_LENGTH];
};


class ControlClient
{
public:
    typedef std::deque<ControlMessage> ControlMessageQueue;
    typedef boost::asio::ip::tcp::resolver::iterator tcp_iterator;
    typedef std::function<void (const nlohmann::json&)> HandlerFn;

public:
    ControlClient(boost::asio::io_service& ioService,
                  tcp_iterator endpointIterator,
                  HandlerFn handler);

    void Write(const nlohmann::json& json);

    void Write(const ControlMessage& msg);

    void Close();

private:
    void Reconnect();

    void OnMessage(const nlohmann::json& json);

    void DoConnect();

    void DoReadHeader();

    void DoReadBody(char* start, uint32_t length);

    void DoWrite();

private:
    boost::asio::io_service& mIoService;
    std::unique_ptr<boost::asio::ip::tcp::socket> mpSocket;
    ControlMessage mReadMessage;
    ControlMessageQueue mWriteMessages;
    HandlerFn mHandler;
    tcp_iterator mEndpointIterator;
};
