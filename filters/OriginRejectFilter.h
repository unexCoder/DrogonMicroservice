#pragma once

#include <drogon/HttpMiddleware.h>
#include <drogon/HttpResponse.h>

class OriginRejectFilter : public drogon::HttpMiddleware<OriginRejectFilter>
{
public:
    OriginRejectFilter();  // constructor

    void invoke(const drogon::HttpRequestPtr &req,
                drogon::MiddlewareNextCallback &&nextCb,
                drogon::MiddlewareCallback &&mcb) override;
};