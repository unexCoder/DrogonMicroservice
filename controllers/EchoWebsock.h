#pragma once
#include <drogon/WebSocketController.h>

class EchoWebsock : public drogon::WebSocketController<EchoWebsock>
{
public:
    virtual void handleNewMessage(const drogon::WebSocketConnectionPtr&,
                                std::string &&,
                                const drogon::WebSocketMessageType &) override;
    virtual void handleNewConnection(const drogon::HttpRequestPtr &,
                                    const drogon::WebSocketConnectionPtr&) override;
    virtual void handleConnectionClosed(const drogon::WebSocketConnectionPtr&) override;
    WS_PATH_LIST_BEGIN
    WS_PATH_ADD("/echo");
    WS_PATH_LIST_END
};
