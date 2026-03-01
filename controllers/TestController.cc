#include "TestController.h"

void TestController::asyncHandleHttpRequest(
    const drogon::HttpRequestPtr &req,
    std::function<void(const drogon::HttpResponsePtr &)> &&callback)
{
    auto path = req->path();

    if (path == "/list_para")
    {
        auto para = req->getParameters();
        drogon::HttpViewData data;
        data.insert("title", "ListParameters");
        data.insert("parameters", para);

        auto resp = drogon::HttpResponse::newHttpViewResponse(
            "ListParameters.csp", data);

        callback(resp);
    }
    else if (path == "/slow")
    {
        Json::Value json;
        json["result"] = "ok";
        auto resp = drogon::HttpResponse::newHttpJsonResponse(json);
        callback(resp);
    }
}