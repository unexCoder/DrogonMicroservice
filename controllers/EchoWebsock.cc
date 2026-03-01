#include "EchoWebsock.h"

void EchoWebsock::handleNewMessage(const drogon::WebSocketConnectionPtr& wsConnPtr, std::string &&message, const drogon::WebSocketMessageType &type)
{
    // write your application logic here
    wsConnPtr->send(message);
}

void EchoWebsock::handleNewConnection(const drogon::HttpRequestPtr &req, const drogon::WebSocketConnectionPtr& wsConnPtr)
{
    // write your application logic here
}

void EchoWebsock::handleConnectionClosed(const drogon::WebSocketConnectionPtr& wsConnPtr)
{
    // write your application logic here
}
