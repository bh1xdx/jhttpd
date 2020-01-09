

/***************************************************************************************************************
 *          Copyright(C) 2019 by J.L
 ***************************************************************************************************************/


#ifndef __JHTTPD_H__
#define __JHTTPD_H__

#include <string>
#include <cstdint>
#include "httpstatuscode.h"

/***************************************************************************************************************/
#if defined _WIN32 || defined __CYGWIN__
    #ifdef JHTTPD_LIB
        #ifdef __GNUC__
            #define JHTTPD_EXPORT __attribute__ ((dllexport))
        #else //__GNUC__
            #define JHTTPD_EXPORT __declspec(dllexport) // Note: actually gcc seems to also supports this syntax.
        #endif //__GNUC__
    #else  // JHTTPD_LIB
        #ifdef __GNUC__
            #define JHTTPD_EXPORT __attribute__ ((dllimport))
        #else // __GNUC__
            #define JHTTPD_EXPORT __declspec(dllimport) // Note: actually gcc seems to also supports this syntax.
        #endif // __GNUC__
    #endif  // JHTTPD_LIB
    #define DLL_LOCAL
#else  // defined _WIN32 || defined __CYGWIN__
    #if __GNUC__ >= 4
        #define JHTTPD_EXPORT __attribute__ ((visibility ("default")))
        #define DLL_LOCAL  __attribute__ ((visibility ("hidden")))
    #else //__GNUC__ >= 4
        #define JHTTPD_EXPORT
        #define DLL_LOCAL
    #endif //__GNUC__ >= 4
#endif // defined _WIN32 || defined __CYGWIN__

#if defined _WIN32
    #define JHTTPD_API  __cdecl
#else
    #define JHTTPD_API
#endif // __GNUC__

#define JHTTPD_RET(T)  T JHTTPD_API

/***************************************************************************************************************/
class JHTTPD_EXPORT JHttpLog
{
public:

    enum class Level
    {
        Fatal,
        Error,
        Warn,
        Trace,
        Info,
        Debug,
    };
    virtual void log(Level level, const std::string & log) = 0;
};


class JHTTPD_EXPORT JHttpRespose
{
public:
    // get resport data
    // type: the data type, for examle, ".json"
    // path: the path of url, start by "/".
    // return http code: 200 for success and other.
    virtual uint32_t get(const std::string & path, std::string & ret_type, std::string &ret_datetime, std::string &ret_content) = 0;
};


enum ErCode
{
    OK,
    FAIL,
};

class JHTTPD_EXPORT JHttpService
{
public:
    virtual ErCode add(const std::string & url, JHttpRespose* response) = 0;
    virtual ErCode del(const std::string & url, JHttpRespose* response) = 0;
    virtual ErCode set(JHttpLog *log) = 0;


    virtual ErCode run() = 0;
    virtual ErCode stop() = 0;
    virtual void exit() = 0;
};

extern "C"
{
    JHTTPD_EXPORT JHttpService * JHttpCreateService(const char * root = "", uint32_t port = 0);
    JHTTPD_EXPORT void JHttpdestroyService(JHttpService *);
}



#endif //__JHTTPD_H__