#pragma once

#include <drogon/HttpMiddleware.h>
using namespace drogon;

class OriginRejectFilter : public HttpMiddleware<OriginRejectFilter>
{
public:
    OriginRejectFilter();  // constructor

    void invoke(const HttpRequestPtr &req,
                MiddlewareNextCallback &&nextCb,
                MiddlewareCallback &&mcb) override;
};