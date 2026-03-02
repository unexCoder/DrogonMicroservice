#include "CulturalNodesCtrl.h"

using namespace drogon;
using namespace drogon::orm;
using namespace drogon_model::culture_hub;

void CulturalNodesCtrl::getAll(const HttpRequestPtr &,
                               std::function<void(const HttpResponsePtr &)> &&callback)
{
    auto client = app().getDbClient();
    auto mapper = std::make_shared<Mapper<CulturalNodes>>(client);

    mapper->findAll(
        [callback, mapper](std::vector<CulturalNodes> nodes)
        {
            Json::Value arr(Json::arrayValue);
            for (auto &n : nodes)
                arr.append(n.toJson());
            callback(HttpResponse::newHttpJsonResponse(arr));
        },
        [callback, mapper](const DrogonDbException &e)
        {
            auto resp = HttpResponse::newHttpResponse();
            resp->setStatusCode(k500InternalServerError);
            resp->setBody(e.base().what());
            callback(resp);
        });
}

void CulturalNodesCtrl::getOne(const HttpRequestPtr &,
                               std::function<void(const HttpResponsePtr &)> &&callback,
                               int id)
{
    auto client = app().getDbClient();
    auto mapper = std::make_shared<Mapper<CulturalNodes>>(client);

    mapper->findByPrimaryKey(
        id,
        [callback, mapper](CulturalNodes node)
        {
            callback(HttpResponse::newHttpJsonResponse(node.toJson()));
        },
        [callback, mapper](const DrogonDbException &)
        {
            auto resp = HttpResponse::newHttpResponse();
            resp->setStatusCode(k404NotFound);
            callback(resp);
        });
}

void CulturalNodesCtrl::create(const HttpRequestPtr &req,
                               std::function<void(const HttpResponsePtr &)> &&callback)
{
    auto json = req->getJsonObject();
    if (!json)
    {
        Json::Value errBody;
        errBody["error"] = "Invalid or missing JSON body";
        auto resp = HttpResponse::newHttpJsonResponse(errBody);
        resp->setStatusCode(k400BadRequest);
        callback(resp);
        return;
    }

    // ↓ convertir objetos JSON a string ANTES de validar
    for (const auto &field : {"social", "contact"})
    {
        if (json->isMember(field) && (*json)[field].isObject())
        {
            Json::StreamWriterBuilder writer;
            (*json)[field] = Json::writeString(writer, (*json)[field]);
        }
    }
    
    std::string err;
    if (!CulturalNodes::validateJsonForCreation(*json, err))
    {
        auto resp = HttpResponse::newHttpResponse();
        resp->setStatusCode(k400BadRequest);
        resp->setBody(err);
        callback(resp);
        return;
    }

    CulturalNodes node(*json);
    auto client = app().getDbClient();
    auto mapper = std::make_shared<Mapper<CulturalNodes>>(client);

    mapper->insert(
        node,
        [callback, mapper](CulturalNodes inserted)
        {
            auto resp = HttpResponse::newHttpJsonResponse(inserted.toJson());
            resp->setStatusCode(k201Created);
            callback(resp);
        },
        [callback, mapper](const DrogonDbException &e)
        {
            auto resp = HttpResponse::newHttpResponse();
            resp->setStatusCode(k500InternalServerError);
            resp->setBody(e.base().what());
            callback(resp);
        });
}

void CulturalNodesCtrl::update(const HttpRequestPtr &req,
                               std::function<void(const HttpResponsePtr &)> &&callback,
                               int id)
{
    auto json = req->getJsonObject();
    if (!json) { /* 400 */ return; }

    (*json)["id"] = id;


    // ↓ convertir objetos JSON a string ANTES de validar
    for (const auto &field : {"social", "contact"})
    {
        if (json->isMember(field) && (*json)[field].isObject())
        {
            Json::StreamWriterBuilder writer;
            (*json)[field] = Json::writeString(writer, (*json)[field]);
        }
    }

    std::string err;
    if (!CulturalNodes::validateJsonForUpdate(*json, err)) { /* 400 */ return; }

    CulturalNodes node(*json);  // construir directo, sin findByPrimaryKey
    auto client = app().getDbClient();
    auto mapper = std::make_shared<Mapper<CulturalNodes>>(client);

    mapper->update(
        node,
        [callback, mapper](size_t count)
        {
            if (count == 0)
            {
                auto resp = HttpResponse::newHttpResponse();
                resp->setStatusCode(k404NotFound);
                callback(resp);
                return;
            }
            callback(HttpResponse::newHttpResponse()); // 200
        },
        [callback, mapper](const DrogonDbException &e)
        {
            auto resp = HttpResponse::newHttpResponse();
            resp->setStatusCode(k500InternalServerError);
            resp->setBody(e.base().what());
            callback(resp);
        });
}

void CulturalNodesCtrl::remove(const HttpRequestPtr &,
                               std::function<void(const HttpResponsePtr &)> &&callback,
                               int id)
{
    auto client = app().getDbClient();
    auto mapper = std::make_shared<Mapper<CulturalNodes>>(client);

    mapper->deleteByPrimaryKey(
        id,
        [callback, mapper](size_t count)
        {
            if (count == 0)
            {
                auto resp = HttpResponse::newHttpResponse();
                resp->setStatusCode(k404NotFound);
                callback(resp);
                return;
            }
            auto resp = HttpResponse::newHttpResponse();
            resp->setStatusCode(k204NoContent);
            callback(resp);
        },
        [callback, mapper](const DrogonDbException &e)
        {
            auto resp = HttpResponse::newHttpResponse();
            resp->setStatusCode(k500InternalServerError);
            resp->setBody(e.base().what());
            callback(resp);
        });
}