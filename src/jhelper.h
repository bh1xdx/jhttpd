


#ifndef __JHTTPD_HELPER_H__
#define __JHTTPD_HELPER_H__

#include <string>
#include <cstdint>

#ifdef _WINDOWS
#define __builtin_expect(EXP, C)  (EXP)
#endif // _WINDOWS

#define jhttpd_likely(x)    __builtin_expect(!!(x), 1)
#define jhttpd_unlikely(x)  __builtin_expect(!!(x), 0)

class Helper
{
public:
    static uint64_t tick();
    static uint64_t msSinceEpoch();
    static std::string timeByMsSinceEpoch(const uint64_t &ts, const std::string & format = "%Y.%m.%d-%H:%M:%S", bool ms = true, bool local = true);
    static std::string datetime(const uint64_t &ts = 0) {return timeByMsSinceEpoch(ts ? ts: msSinceEpoch());}
    static std::string datetime_rfc7232(const uint64_t &ts = 0) { return timeByMsSinceEpoch( ts ? ts: msSinceEpoch(), "%a, %d %b %Y %T GMT", false, false); };

private:
    static void _local_time(struct tm &tm, const uint64_t &secSinceEpoch);
    static void _gmt_time(struct tm &tm, const uint64_t &secSinceEpoch);
};


#endif // __JHTTPD_HELPER_H__