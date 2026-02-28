#include "OriginRejectFilter.h"

OriginRejectFilter::OriginRejectFilter()
{
}

void OriginRejectFilter::invoke(const HttpRequestPtr &req,
                                MiddlewareNextCallback &&nextCb,
                                MiddlewareCallback &&mcb)
{
    std::string origin = req->getHeader("origin");

    // Reject specific origin
    if (!origin.empty() &&
        origin.find("www.some-evil-place.com") != std::string::npos)
    {
        mcb(HttpResponse::newNotFoundResponse(req));
        return;
    }

    // Handle CORS preflight
    if (req->method() == Options)
    {
        auto resp = HttpResponse::newHttpResponse();

        if (!origin.empty())
        {
            resp->addHeader("Access-Control-Allow-Origin", origin);
            resp->addHeader("Access-Control-Allow-Credentials", "true");
            resp->addHeader("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS");
            resp->addHeader("Access-Control-Allow-Headers", "Content-Type, Authorization");
        }

        resp->setStatusCode(k200OK);
        mcb(resp);
        return;
    }

    // Continue middleware chain
    std::move(nextCb)(
        [origin, mcb = std::move(mcb)](const HttpResponsePtr &resp) mutable {
            if (!origin.empty())
            {
                resp->addHeader("Access-Control-Allow-Origin", origin);
                resp->addHeader("Access-Control-Allow-Credentials", "true");
            }
            mcb(resp);
        });
}