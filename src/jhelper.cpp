

#include "jhelper.h"

#include <chrono>
#include <ctime>


uint64_t Helper::tick()
{
    using MS = std::chrono::milliseconds;
    auto tpnow = std::chrono::steady_clock::now();
    auto tpms = std::chrono::time_point_cast<MS>(tpnow);
    return (uint64_t)tpms.time_since_epoch().count();
}

uint64_t Helper::msSinceEpoch()
{
    using MS = std::chrono::milliseconds;
    auto tpnow = std::chrono::system_clock::now();
    auto tpms = std::chrono::time_point_cast<MS>(tpnow);
    uint64_t ts = tpms.time_since_epoch().count();
    return (uint64_t)ts;
}

std::string Helper::timeByMsSinceEpoch(const uint64_t &ts, const std::string & format /*= "%Y.%m.%d-%H:%M:%S"*/, bool withms /*= true*/, bool local /*= true*/)
{
    uint64_t sec = ts / 1000;
    auto ms = ts - sec * 1000;
    struct tm _tm;

    if (jhttpd_likely(local))
        _local_time(_tm, sec);
    else
        _gmt_time(_tm, sec);

    char buf[1024] = { 0 };
    std::strftime(buf, sizeof(buf), format.c_str(), &_tm);
    std::string result(buf);
    if (withms)
    {
        std::snprintf(buf, sizeof(buf), ".%03d", (int)ms);
        result += buf;
    }
    return std::move(result);
}

void Helper::_local_time(struct tm &_tm, const uint64_t & secSinceEpoch)
{
    uint64_t sec = secSinceEpoch;

#if defined(HISI3559A) || defined(HISI3519A)
    sec += 8 * 60 * 60;
#endif
#ifdef _WINDOWS
    localtime_s(&_tm, (const time_t*)&sec);
#else // _WINDOWS
    localtime_r((const time_t*)&sec, &_tm);
#endif // _WINDOWS
}

void Helper::_gmt_time(struct tm &_tm, const uint64_t & secSinceEpoch)
{
    uint64_t sec = secSinceEpoch;
#ifdef _WINDOWS
    gmtime_s(&_tm, (const time_t*)&sec);
#else // _WINDOWS
    gmtime_r((const time_t*)&sec, &_tm);
#endif // _WINDOWS
}

