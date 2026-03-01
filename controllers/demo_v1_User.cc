#include "demo_v1_User.h"
#include <drogon/HttpResponse.h>
#include <drogon/utils/Utilities.h>

using namespace demo::v1;

void User::login(const drogon::HttpRequestPtr &req,
                 std::function<void (const drogon::HttpResponsePtr &)> &&callback,
                 std::string &&userId,
                 const std::string &password)
{
    LOG_DEBUG << "User " << userId << " login";

    Json::Value ret;
    ret["result"] = "ok";
    ret["id"] = userId;                     // <-- added id field
    ret["token"] = drogon::utils::getUuid(); // <-- generated token

    auto resp = drogon::HttpResponse::newHttpJsonResponse(ret);
    callback(resp);
}

void User::getInfo(const drogon::HttpRequestPtr &req,
                   std::function<void (const drogon::HttpResponsePtr &)> &&callback,
                   std::string userId,
                   const std::string &token)
{
    
    Json::Value ret;
    ret["result"] = "ok";
    ret["id"] = userId;     // <-- return id as JSON
    ret["token"] = token;   // <-- return token as JSON

    auto resp = drogon::HttpResponse::newHttpJsonResponse(ret);
    callback(resp);
}