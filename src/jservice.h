

/******************************************************
 *          Copyright(C) 2018 aibee.com
 ******************************************************/

#ifndef __JHTTPD_SERVICE_H__
#define __JHTTPD_SERVICE_H__

#include "jhttpd.h"
#include "config.h"
#include "nocopyable.h"
#include "jlog.h"
#include "jobject.h"
#include "jserver.h"

#include <memory>


 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
 // JService
 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
class JService : public JHttpService
{
public:
    JService(const std::string &webroot = HTTP_SERVER_WROOT, uint32_t port = HTTP_SERVER_PORT)
        : m_webroot(webroot)
        , m_port(port) {};

    virtual ~JService() {};

    ErCode add(const std::string & url, JHttpRespose* resp) override;
    ErCode del(const std::string & url, JHttpRespose* resp) override;
    ErCode set(JHttpLog *log) override;

    ErCode run() override;
    ErCode stop() override;
    void exit() override;

private:
    int task();

    std::string name() { return "service"; };

private:
    JServer m_server;

    std::string m_webroot;
    uint32_t m_port;
};




#endif //__JHTTPD_SERVICE_H__