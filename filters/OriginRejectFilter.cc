#include "OriginRejectFilter.h"

OriginRejectFilter::OriginRejectFilter()
{
}

void OriginRejectFilter::invoke(const drogon::HttpRequestPtr &req,
                                drogon::MiddlewareNextCallback &&nextCb,
                                drogon::MiddlewareCallback &&mcb)
{
    std::string origin = req->getHeader("origin");

    // Reject specific origin
    if (!origin.empty() &&
        origin.find("www.some-evil-place.com") != std::string::npos)
    {
        auto resp = drogon::HttpResponse::newHttpResponse();
        resp->setStatusCode(drogon::k403Forbidden);
        mcb(resp);
        return;
    }

    // Handle CORS preflight
    if (req->method() == drogon::Options)
    {
        auto resp = drogon::HttpResponse::newHttpResponse();

        if (!origin.empty())
        {
            resp->addHeader("Access-Control-Allow-Origin", origin);
            resp->addHeader("Access-Control-Allow-Credentials", "true");
            resp->addHeader("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS");
            resp->addHeader("Access-Control-Allow-Headers", "Content-Type, Authorization");
        }

        resp->setStatusCode(drogon::k200OK);
        mcb(resp);
        return;
    }

    // Continue middleware chain
    auto responseCb = [origin](const drogon::HttpResponsePtr &resp) {
        if (!resp)
        {
            return;
        }
        
        if (!origin.empty())
        {
            resp->addHeader("Access-Control-Allow-Origin", origin);
            resp->addHeader("Access-Control-Allow-Credentials", "true");
        }
    };
    
    nextCb(std::move(responseCb));
}