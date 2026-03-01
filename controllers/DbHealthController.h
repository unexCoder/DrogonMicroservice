#pragma once
#include <drogon/HttpController.h>

class DbHealthController : public drogon::HttpController<DbHealthController>
{
public:
    METHOD_LIST_BEGIN
        ADD_METHOD_TO(DbHealthController::check, "/health/db", drogon::Get);
    METHOD_LIST_END

    void check(const drogon::HttpRequestPtr &req,
               std::function<void(const drogon::HttpResponsePtr &)> &&callback) const;
};