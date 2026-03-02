#include "CulturalNodesCtrl.h"

using namespace drogon;
using namespace drogon::orm;
using namespace drogon_model::culture_hub;

void CulturalNodesCtrl::getAll(const HttpRequestPtr &req,   // ← req nombrado
                               std::function<void(const HttpResponsePtr &)> &&callback)
{
    auto client = app().getDbClient();
    auto mapper = std::make_shared<Mapper<CulturalNodes>>(client);

    int page  = 1;
    int limit = 20;
    auto pageStr    = req->getParameter("page");
    auto limitStr   = req->getParameter("limit");
    auto sortFilter = req->getParameter("sort");

    if (!pageStr.empty())  page  = std::stoi(pageStr);
    if (!limitStr.empty()) limit = std::stoi(limitStr);
    if (limit > 100) limit = 100;

    auto callbackLambda = [callback](std::vector<CulturalNodes> nodes)
    {
        Json::Value arr(Json::arrayValue);
        for (auto &n : nodes)
            arr.append(n.toJson());
        callback(HttpResponse::newHttpJsonResponse(arr));
    };

    auto errorLambda = [callback](const DrogonDbException &e)
    {
        LOG_ERROR << "DB error: " << e.base().what();
        Json::Value errBody;
        errBody["error"] = "Internal server error";
        auto resp = HttpResponse::newHttpJsonResponse(errBody);
        resp->setStatusCode(k500InternalServerError);
        callback(resp);
    };

    mapper->orderBy(CulturalNodes::Cols::_name)
           .paginate(page, limit);

    if (!sortFilter.empty())
        mapper->findBy(
            Criteria(CulturalNodes::Cols::_sort, CompareOperator::EQ, sortFilter),
            callbackLambda,
            errorLambda);
    else
        mapper->findAll(callbackLambda, errorLambda);
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
        Json::Value errBody;
        errBody["error"] = err;
        auto resp = HttpResponse::newHttpJsonResponse(errBody);
        resp->setStatusCode(k400BadRequest);
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
            LOG_ERROR << "DB error: " << e.base().what();
            Json::Value errBody;
            errBody["error"] = "Internal server error";
            auto resp = HttpResponse::newHttpJsonResponse(errBody);
            resp->setStatusCode(k500InternalServerError);
            callback(resp);
        });
}

void CulturalNodesCtrl::update(const HttpRequestPtr &req,
                               std::function<void(const HttpResponsePtr &)> &&callback,
                               int id)
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

    (*json)["id"] = id;

    for (const auto &field : {"social", "contact"})
    {
        if (json->isMember(field) && (*json)[field].isObject())
        {
            Json::StreamWriterBuilder writer;
            (*json)[field] = Json::writeString(writer, (*json)[field]);
        }
    }

    std::string err;
    if (!CulturalNodes::validateJsonForUpdate(*json, err))
    {
        Json::Value errBody;
        errBody["error"] = err;
        auto resp = HttpResponse::newHttpJsonResponse(errBody);
        resp->setStatusCode(k400BadRequest);
        callback(resp);
        return;
    }

    CulturalNodes node(*json);
    auto client = app().getDbClient();
    auto mapper = std::make_shared<Mapper<CulturalNodes>>(client);

    mapper->update(
        node,
        [callback, mapper, node](size_t count)
        {
            if (count == 0)
            {
                auto resp = HttpResponse::newHttpResponse();
                resp->setStatusCode(k404NotFound);
                callback(resp);
                return;
            }
            callback(HttpResponse::newHttpJsonResponse(node.toJson())); // 200 con body
        },
        [callback, mapper](const DrogonDbException &e)
        {
            LOG_ERROR << "DB error: " << e.base().what();
            Json::Value errBody;
            errBody["error"] = "Internal server error";
            auto resp = HttpResponse::newHttpJsonResponse(errBody);
            resp->setStatusCode(k500InternalServerError);
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
            LOG_ERROR << "DB error: " << e.base().what();
            Json::Value errBody;
            errBody["error"] = "Internal server error";
            auto resp = HttpResponse::newHttpJsonResponse(errBody);
            resp->setStatusCode(k500InternalServerError);
            callback(resp);
        });
}