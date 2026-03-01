#include "TimeFilter.h"
#include <trantor/utils/Date.h>
#include <trantor/utils/Logger.h>
#include <drogon/HttpResponse.h>

#define VDate "visitDate"

void TimeFilter::doFilter(const drogon::HttpRequestPtr &req,
                          drogon::FilterCallback &&cb,
                          drogon::FilterChainCallback &&ccb)
{
    auto session = req->session();
    if (!session)
    {
        ccb();
        return;
    }

    const int minIntervalSec = 10;

    auto now = trantor::Date::now();

    if (session->find(VDate))
    {
        auto lastDate = session->get<trantor::Date>(VDate);

        // Calculate elapsed time in seconds
        int64_t elapsedMicro =
            now.microSecondsSinceEpoch() - lastDate.microSecondsSinceEpoch();

        double elapsedSec = elapsedMicro / 1000000.0;
        double remainingSec = minIntervalSec - elapsedSec;

        if (elapsedSec >= minIntervalSec)
        {
            // Update session timestamp
            session->modify<trantor::Date>(VDate,
                [now](trantor::Date &vdate)
                {
                    vdate = now;
                });

            ccb();   // allow request
            return;
        }
        else
        {
            Json::Value json;
            json["result"] = "error";
            json["message"] = "Access interval should be at least 10 seconds";
            json["elapsed_seconds"] = elapsedSec;
            json["remaining_seconds"] = remainingSec > 0 ? remainingSec : 0.0;

            auto res = drogon::HttpResponse::newHttpJsonResponse(json);
            res->setStatusCode(drogon::k429TooManyRequests);  // better HTTP semantic
            cb(res);
            return;
        }
    }

    // First visit
    session->insert(VDate, now);
    ccb();
}