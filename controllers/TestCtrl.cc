#include "TestCtrl.h"
#include <drogon/HttpResponse.h>
#include <fstream>
#include <sstream>

// Read HTML file once at startup
static std::string loadIndexHTML() {
    std::ifstream file("../public/index.html");
    if (!file.is_open()) {
        LOG_WARN << "Failed to load index.html, using fallback";
        return "<html><body><h1>Culture Hub API</h1><p>Server is running</p></body></html>";
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

// Cache the HTML content
static const std::string INDEX_HTML = loadIndexHTML();

void TestCtrl::asyncHandleHttpRequest(
    const drogon::HttpRequestPtr& req,
    std::function<void (const drogon::HttpResponsePtr &)> &&callback)
{
    auto resp = drogon::HttpResponse::newHttpResponse();
    resp->setStatusCode(drogon::k200OK);
    resp->setContentTypeCode(drogon::CT_TEXT_HTML);
    resp->setBody(INDEX_HTML);
    callback(resp);
}