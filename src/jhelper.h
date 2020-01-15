


#ifndef __JHTTPD_HELPER_H__
#define __JHTTPD_HELPER_H__

#include <string>
#include <cstdint>


class Helper
{
public:
    static uint64_t tick();
    static uint64_t msSinceEpoch();
    static std::string timeByMsSinceEpoch(const uint64_t &ts, const std::string & format = "%Y.%m.%d-%H:%M:%S", bool ms = true);
    static  std::string datetime() {return timeByMsSinceEpoch(msSinceEpoch());}
    static void _local_time(struct tm &tm, const uint64_t &secSinceEpoch);
};


#endif // __JHTTPD_HELPER_H__