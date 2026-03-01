#include <drogon/drogon.h>
#include "DbHealthController.h"
#include <drogon/HttpAppFramework.h>
#include <drogon/orm/DbClient.h>

void DbHealthController::check(const drogon::HttpRequestPtr &req,
                                std::function<void(const drogon::HttpResponsePtr &)> &&callback) const
{
    auto db = drogon::app().getDbClient("default");
    
    if (!db)
    {
        Json::Value res;
        res["status"] = "error";
        res["message"] = "No database client available";
        auto resp = drogon::HttpResponse::newHttpJsonResponse(res);
        resp->setStatusCode(drogon::k503ServiceUnavailable);
        callback(resp);
        return;
    }

    // Run SHOW TABLES to verify connection and get table list
    db->execSqlAsync(
        "SHOW TABLES",
        [callback](const drogon::orm::Result &result)
        {
            Json::Value tables(Json::arrayValue);
            for (const auto &row : result)
            {
                tables.append(row[0].as<std::string>());
            }

            Json::Value res;
            res["status"] = "ok";
            res["message"] = "Database connection is healthy";
            res["table_count"] = static_cast<int>(tables.size());
            res["tables"] = tables;

            auto resp = drogon::HttpResponse::newHttpJsonResponse(res);
            resp->setStatusCode(drogon::k200OK);
            callback(resp);
        },
        [callback](const drogon::orm::DrogonDbException &e)
        {
            LOG_ERROR << "DB health check failed: " << e.base().what();

            Json::Value res;
            res["status"] = "error";
            res["message"] = std::string("Database query failed: ") + e.base().what();

            auto resp = drogon::HttpResponse::newHttpJsonResponse(res);
            resp->setStatusCode(drogon::k503ServiceUnavailable);
            callback(resp);
        });
}