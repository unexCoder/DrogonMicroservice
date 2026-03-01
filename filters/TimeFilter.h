#pragma once

#include <drogon/HttpFilter.h>

class TimeFilter : public drogon::HttpFilter<TimeFilter>
{
  public:
    void doFilter(const drogon::HttpRequestPtr &req,
                  drogon::FilterCallback &&cb,
                  drogon::FilterChainCallback &&ccb);
};