

#ifndef __JHTTPD_JSERVER_H__
#define __JHTTPD_JSERVER_H__



#include <nocopyable.h>
#include <jobject.h>
#include <jlog.h>

#include <string>
#include <vector>

#include "jhttpd.h"

#include "jsocket.h"
#include "jtcplistener.h"
#include "jtheadpool.h"

#include "jconfig.h"

class JServer : public JObject, public NoCopyable
{
public:

    JServer(uint32_t threadcount)
        : JObject("JServer")
    {
        m_running.store(false);
        m_threadPool = std::make_shared<ThreadPool>(threadcount);
    }
    ~JServer() {};

    using TranspPort = TcpListener::TranspPort;
    enum { DEFAULT_PORT = JHTTPD_SERVER_PORT };

public:

    std::string getWebRootPath() const { 
       return _webRootPath; 
    }

    void setupWebRootPath(const std::string& webRootPath) {
        _webRootPath = webRootPath;
    }

    const TranspPort getLocalPort() const { 
       return _serverPort;
    }
    bool bind(TranspPort port = DEFAULT_PORT);

    bool listen(int maxConnections);
    bool run();
    void stop();


    JLoger &loger();

    uint32_t resp_count() const;
    JHttpResp *resp_at(uint32_t idx) const;
    uint32_t resp_add(JHttpResp *);

protected:
    TcpSocket::Handle accept() { 
       return _tcpServer->accept(); 
    }

private:
    TranspPort _serverPort = DEFAULT_PORT;
    TcpListener::Handle _tcpServer;
    std::string _webRootPath = "/tmp";

    std::vector<JHttpResp*> m_resps;
    std::atomic<bool> m_running;
    std::shared_ptr<ThreadPool> m_threadPool;
};


/* -------------------------------------------------------------------------- */

#endif // __JHTTPD_JSERVER_H__
