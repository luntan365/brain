#pragma once

#include "http_server.h"
#define BOOST_SPIRIT_THREADSAFE
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

class HttpApi
{
public:
    typedef SimpleWeb::Server<SimpleWeb::HTTP> HttpServer;
    typedef boost::property_tree::ptree Json;

    struct Response
    {
        uint32_t responseCode;
        Json body;

        void Send(std::ostream& os);
    };

    typedef std::function<Response (const Json&)>
        HandlerFn;

    HttpApi(uint16_t port, uint32_t threads);

    void Start();

    void Register(
        const std::string& regex,
        const std::string& method,
        HandlerFn fn
    );

    void HandlerWrapper(
        HttpServer::Response& response,
        std::shared_ptr<HttpServer::Request> request,
        HandlerFn fn
    );

private:
    Response HandleHello(const Json& input);
    Response HandleCtm(const Json& input);
    Response HandleCast(const Json& input);

private:
    HttpServer mServer;
};
