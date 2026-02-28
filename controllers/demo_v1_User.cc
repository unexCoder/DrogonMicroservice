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
    ret["token"] = drogon::utils::getUuid();

    auto resp = drogon::HttpResponse::newHttpJsonResponse(ret);
    callback(resp);
}

void User::getInfo(const drogon::HttpRequestPtr &req,
                   std::function<void (const drogon::HttpResponsePtr &)> &&callback,
                   std::string userId,
                   const std::string &token) const
{
    LOG_DEBUG << "User " << userId << " get his information";

    Json::Value ret;
    ret["result"] = "ok";
    ret["user_name"] = "Jack";
    ret["user_id"] = userId;
    ret["gender"] = 1;

    auto resp = drogon::HttpResponse::newHttpJsonResponse(ret);
    callback(resp);
}