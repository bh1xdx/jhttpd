
#include "jserver.h"
#include "jlog.h"

#include "Tools.h"

#include <thread>
#include <cassert>


class HttpServerTask:public JObject{
private:
    TcpSocket::Handle _tcpSocketHandle;
    std::string _webRootPath;

    TcpSocket::Handle& getTcpSocketHandle() {
        return _tcpSocketHandle;
    }

    const std::string& getWebRootPath() const { 
        return _webRootPath; 
    }

    HttpServerTask(JObject * parent, TcpSocket::Handle socketHandle, const std::string& webRootPath)
        : JObject("HttpServerTask", parent)
        , _tcpSocketHandle(socketHandle)
        , _webRootPath(webRootPath)
    {
    }

public:
    using Handle = std::shared_ptr<HttpServerTask>;

    inline static Handle create(
        JObject * parent,
        TcpSocket::Handle socketHandle,
        const std::string& webRootPath)
    {
        return Handle(new HttpServerTask(parent,
            socketHandle, 
            webRootPath));
    }

    void operator()(Handle task_handle);

private:
    JLoger &loger()
    {
        JObject * r = root();
        JServer * js = dynamic_cast<JServer*>(r);
        assert(js);
        return js->loger();
    }
};


void HttpServerTask::operator()(Handle task_handle)
{
    std::string prefix = name() + ":" + __FUNCTION__;

    (void)task_handle;

    const int sd = getTcpSocketHandle()->getSocketFd();

    // Generates an identifier for recognizing the transaction
    auto transactionId = [sd]() {
        return "[" + std::to_string(sd) + "] " + "["
            + Tools::getLocalTime() + "]";
    };
    prefix += transactionId();


    while (getTcpSocketHandle()) {
        // Create an http socket around a connected tcp socket
        HttpSocket httpSocket(getTcpSocketHandle());

        // Wait for a request from remote peer
        HttpRequest::Handle httpRequest;
        httpSocket >> httpRequest;

        // If an error occoured terminate the task
        if (!httpSocket)
            break;

        // Log the request
        loger().inf("%s:%s", prefix.c_str(), httpRequest->dump(transactionId()).c_str());

        // Build a response to previous HTTP request
        JResponse response(root(), *httpRequest, getWebRootPath());

        // Send the response to remote peer
        httpSocket.sendHeader(response);

        // If HTTP command line method isn't HEAD then send requested URI
        if (httpRequest->getMethod() != HttpRequest::Method::HEAD) {
            if (0 > httpSocket.sendBody(response)) {

                loger().err("%s: error send file [%s]", prefix.c_str(), "body");
                break;
            }
        }

        loger().err("%s: RESP: %s", prefix.c_str(), response.dump(transactionId()).c_str());
    }

    getTcpSocketHandle()->shutdown();
    loger().inf("%s:done", prefix.c_str());
}


/* -------------------------------------------------------------------------- */
// JServer
/* -------------------------------------------------------------------------- */
bool JServer::bind(TranspPort port)
{
    _tcpServer = TcpListener::create();

    if (!_tcpServer) {
        return false;
    }

    return _tcpServer->bind(port);
}

bool JServer::listen(int maxConnections)
{
    if (!_tcpServer) {
        return false;
    }

    return _tcpServer->listen(maxConnections);
}


bool JServer::run()
{
    m_running.store(true);

    // Create a thread for each TCP accepted connection and
    // delegate it to handle HTTP request / response
    while (true) {
        const TcpSocket::Handle handle = accept();

        // Fatal error: we stop the server
        if (!handle) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            continue;
        }

        HttpServerTask::Handle taskHandle = HttpServerTask::create(this,
            handle, 
            getWebRootPath());

        std::thread workerThread(*taskHandle, taskHandle);

        workerThread.detach();
    }

    m_running.store(true);
    return true;
}


JLoger &JServer::loger()
{
    return m_loger;
}


uint32_t JServer::resp_count() const
{
    return m_resps.size();
}

JHttpRespose * JServer::resp_at(uint32_t idx) const
{
    if (idx >= m_resps.size())
        return nullptr;
    return m_resps.at(idx);
}

uint32_t JServer::resp_add(JHttpRespose * resp)
{
    uint32_t res = (uint32_t)(-1);
    if (resp && !m_running.load())
    {
        res = m_resps.size();
        m_resps.push_back(resp);
    }
    return res;
}


