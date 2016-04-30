#include "HttpApi.h"

#include "GameState.h"

#define HANDLIZE(NAME) \
    [this](const Json& json) { \
        return Handle##NAME(json); \
    }

HttpApi::HttpApi(const uint16_t port, const uint32_t threads)
    : mServer(port, threads)
{
    Register("^/hello$", "GET", HANDLIZE(Hello));
    Register("^/ctm$", "POST", HANDLIZE(Ctm));
    Register("^/cast$", "POST", HANDLIZE(Cast));
}


void HttpApi::Start()
{
    mServer.start();
}


void HttpApi::HandlerWrapper(
    HttpServer::Response& response,
    std::shared_ptr<HttpServer::Request> request,
    HandlerFn fn)
{
    boost::property_tree::ptree pt;
    Response myResponse;
    try
    {
        boost::property_tree::read_json(request->content, pt);
    }
    catch (const std::exception&)
    {
    }
    try
    {
        myResponse = fn(pt);
    }
    catch (const std::exception&)
    {
        myResponse.responseCode = 500;
    }
    myResponse.Send(response);
}


void HttpApi::Register(
    const std::string& regex,
    const std::string& method,
    HandlerFn fn)
{
	mServer.resource[regex][method] =
        [this, fn](HttpServer::Response& response, std::shared_ptr<HttpServer::Request> request) {
            HandlerWrapper(response, request, fn);
        };
}


std::string ResponseCodeToString(uint32_t responseCode)
{
    std::unordered_map<uint32_t, std::string> codeMap = {
        {200, "OK"},
        {201, "CREATED"},
        {202, "Accepted"},
        {203, "Partial Information"},
        {204, "No Response"},
        {400, "Bad Request"},
        {401, "Unauthorized"},
        {403, "Forbidden"},
        {404, "Not Found"},
        {405, "Method Not Allowed"},
        {406, "Not Acceptable"},
        {409, "Conflict"},
        {429, "Too Many Requests"},
        {500, "Internal Service Error"},
        {503, "Service Unavailable"},
    };
    if (codeMap.find(responseCode) != codeMap.end())
    {
        return codeMap.at(responseCode);
    }
    else
    {
        return "";
    }
}


void HttpApi::Response::Send(std::ostream& os)
{
    os << "HTTP/1.1 ";
    os << responseCode;
    os << " " << ResponseCodeToString(responseCode);
    os << "\r\n";
    os << "Content-Type: application/json" << "\r\n";
    std::stringstream ss;
    boost::property_tree::write_json(ss, body);
    auto s = ss.str();
    os << "Content-Length: " << s.size() << "\r\n";
    os << "\r\n";
    os << s;
    os.flush();
}

HttpApi::Response HttpApi::HandleHello(const Json& input)
{
    Response response;
    auto& pt = response.body;
    pt.put("hello", "world");
    pt.put("in-game", GameState::Instance().GetIsInGame());
    pt.put("objects", GameState::Instance().ObjectManager().mObjects.size());

    auto object = GameState::Instance().ObjectManager().GetObjectByGuid(
        GameState::Instance().ObjectManager().GetActivePlayerGUID()
    );
    if (object)
    {
        pt.put("my-guid", object->GetGUID());
        pt.put("x", object->mPosition.x);
        pt.put("y", object->mPosition.y);
        pt.put("z", object->mPosition.z);
    }
    response.responseCode = 200;
    return response;
}

HttpApi::Response HttpApi::HandleCtm(const Json& input)
{
    Response response;
    Position destination(
        input.get<float>("x"),
        input.get<float>("y"),
        input.get<float>("z")
    );
    WoWPlayer::ClickToMove(destination);
    response.responseCode = 201;
    return response;
}

HttpApi::Response HttpApi::HandleCast(const Json& input)
{
    Response response;
    WoWPlayer::CastSpellByName(input.get<std::string>("spell"));
    response.responseCode = 201;
    return response;
}
