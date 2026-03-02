#pragma once

#include <drogon/HttpController.h>
#include <models/CulturalNodes.h>

class CulturalNodesCtrl : public drogon::HttpController<CulturalNodesCtrl>
{
public:
    METHOD_LIST_BEGIN
    ADD_METHOD_TO(CulturalNodesCtrl::getAll, "/cultural_nodes", drogon::Get);
    ADD_METHOD_TO(CulturalNodesCtrl::getOne, "/cultural_nodes/{1}", drogon::Get);
    ADD_METHOD_TO(CulturalNodesCtrl::create, "/cultural_nodes", drogon::Post);
    ADD_METHOD_TO(CulturalNodesCtrl::remove, "/cultural_nodes/{1}", drogon::Delete);
    ADD_METHOD_TO(CulturalNodesCtrl::update, "/cultural_nodes/{1}", drogon::Put);
    METHOD_LIST_END

    void getAll(const drogon::HttpRequestPtr& req,
                std::function<void (const drogon::HttpResponsePtr &)> &&callback);

    void getOne(const drogon::HttpRequestPtr& req,
                std::function<void (const drogon::HttpResponsePtr &)> &&callback,
                int id);

    void create(const drogon::HttpRequestPtr& req,
                std::function<void (const drogon::HttpResponsePtr &)> &&callback);

    void remove(const drogon::HttpRequestPtr& req,
                std::function<void (const drogon::HttpResponsePtr &)> &&callback,
                int id);

    void update(const drogon::HttpRequestPtr& req,
                std::function<void (const drogon::HttpResponsePtr &)> &&callback,
                int id);                
};