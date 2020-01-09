

#ifndef __JHTTPD_JSERVER_H__
#define __JHTTPD_JSERVER_H__



#include <nocopyable.h>
#include <jobject.h>
#include <jlog.h>

#include <string>
#include <vector>

#include "jhttpd.h"

#include "HttpSocket.h"
#include "TcpListener.h"

#include "config.h"

class JLoger;

class JServer : public JObject, public NoCopyable
{
public:

    JServer()
        : JObject("JServer")
    {
        m_running.store(false);
    }
    ~JServer() {};


    using TranspPort = TcpListener::TranspPort;
    enum { DEFAULT_PORT = HTTP_SERVER_PORT };

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


    JLoger &loger();

    uint32_t resp_count() const;
    JHttpRespose *resp_at(uint32_t idx) const;
    uint32_t resp_add(JHttpRespose *);

protected:
    TcpSocket::Handle accept() { 
       return _tcpServer->accept(); 
    }

private:
    JLoger m_loger;

    TranspPort _serverPort = DEFAULT_PORT;
    TcpListener::Handle _tcpServer;
    std::string _webRootPath = "/tmp";

    std::vector<JHttpRespose*> m_resps;
    std::atomic<bool> m_running;
};


/* -------------------------------------------------------------------------- */

#endif // __JHTTPD_JSERVER_H__
