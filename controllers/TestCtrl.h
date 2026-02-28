#pragma once
#include <drogon/HttpSimpleController.h>

class TestCtrl : public drogon::HttpSimpleController<TestCtrl>
{
  public:
    void asyncHandleHttpRequest(
        const drogon::HttpRequestPtr& req,
        std::function<void (const drogon::HttpResponsePtr &)> &&callback
    ) override;

    PATH_LIST_BEGIN
        PATH_ADD("/", drogon::Get, drogon::Post);
        PATH_ADD("/test", drogon::Get, drogon::Post);
    PATH_LIST_END
};