#include "TestCtrl.h"
#include <drogon/HttpResponse.h>

void TestCtrl::asyncHandleHttpRequest(
    const drogon::HttpRequestPtr& req,
    std::function<void (const drogon::HttpResponsePtr &)> &&callback)
{
    auto resp = drogon::HttpResponse::newHttpResponse();

    resp->setStatusCode(drogon::k200OK);
    resp->setContentTypeCode(drogon::CT_TEXT_HTML);
    resp->setBody("Hello unexCoder!");

    callback(resp);
}