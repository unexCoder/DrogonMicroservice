#pragma once

#include <drogon/HttpController.h>

namespace demo
{
  namespace v1
  {
    class User : public drogon::HttpController<User>
    {
    public:
      METHOD_LIST_BEGIN
        METHOD_ADD(User::login, "/token?userId={1}&passwd={2}", drogon::Post);
        METHOD_ADD(User::getInfo, "/{1}/info?token={2}", drogon::Get,"OriginRejectFilter");
      METHOD_LIST_END

      void login(const drogon::HttpRequestPtr &req,
                 std::function<void(const drogon::HttpResponsePtr &)> &&callback,
                 std::string &&userId,
                 const std::string &password);

      void getInfo(const drogon::HttpRequestPtr &req,
                   std::function<void(const drogon::HttpResponsePtr &)> &&callback,
                   std::string userId,
                   const std::string &token) const;
    };
  }
}