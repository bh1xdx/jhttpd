

/******************************************************
 *          Copyright(C) 2018 aibee.com
 ******************************************************/

#ifndef __JHTTPD_SERVICE_H__
#define __JHTTPD_SERVICE_H__

#include "jhttpd.h"
#include "jconfig.h"
#include "nocopyable.h"
#include "jlog.h"
#include "jobject.h"
#include "jserver.h"

#include <memory>
#include <thread>
#include <set>

class JServer;

 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
 // JService
 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
class JService : public JHttpService
{
    enum class status_t
    {
        stopped,
        running,
        stop,
    };
public:
    JService(const std::string &webroot = JHTTPD_SERVER_WROOT, uint32_t port = JHTTPD_SERVER_PORT)
        : m_webroot(webroot)
        , m_port(port) {
        m_status = status_t::stopped;
        m_threadCnt = 10;
    };

    virtual ~JService() { stop(); };

    JHttpdErCode addResp(JHttpResp * resp) override;
    JHttpdErCode delResp(JHttpResp* resp) override;

    JHttpdErCode setLog(JHttpLog *log) override;
    JHttpdErCode setRoot(const std::string &root) override;
    JHttpdErCode setPort(const uint32_t & port)override;
    JHttpdErCode setThreadCount(const uint32_t count) override;

    JHttpdErCode run(bool block = true) override;
    void stop() override;

private:
    void _stop();
    JHttpdErCode _start( bool block);
    std::string name() { return "jservice"; };

private:
    status_t m_status;

    std::mutex m_mtx;
    std::set<JHttpResp*> m_resps;

    std::shared_ptr<JServer> m_server;
    std::shared_ptr<std::thread> m_thread;
    std::string m_webroot;
    uint32_t m_port;
    uint32_t m_threadCnt;
};




#endif //__JHTTPD_SERVICE_H__