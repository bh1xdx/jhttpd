#include <iostream>
#include <string>

#include "jservice.h"


#include "Tools.h"


ErCode JService::add(const std::string & url, JHttpRespose* resp)
{
    m_server.resp_add(resp);
    return ErCode::OK;
}
ErCode JService::del(const std::string & url, JHttpRespose*)
{
    return ErCode::OK;
}
ErCode JService::set(JHttpLog *log)
{
    m_server.loger().set_output_device(log);
    return ErCode::OK;
}

ErCode JService::run()
{
    task();
    return ErCode::OK;
}

ErCode JService::stop()
{
    return ErCode::OK;
}

void JService::exit()
{
}


int JService::task()
{
    int ret = -1;
    std::string prefix = name() + ":" + __FUNCTION__;
    m_server.loger().dbg("%s: start ...", prefix.c_str());

    do
    {
        m_server.setupWebRootPath(m_webroot);

        bool res = m_server.bind(m_port);

        if (!res) {
            m_server.loger().err("%s: bind port %d failed", prefix.c_str(), m_port);
            break;
        }

        res = m_server.listen(HTTP_SERVER_BACKLOG);
        if (!res) {
            m_server.loger().err("%s: setting listeing mode failed", prefix.c_str());
            break;
        }

        std::cout << Tools::getLocalTime() << std::endl
            << HTTP_SERVER_NAME << " is listening on TCP port " << m_port << std::endl
            << "Working directory is '" << m_webroot << "'\n";

        if (!m_server.run()) {
            std::cerr << "Error starting the server\n";
            break;
        }
        ret = 0;
    } while (false);
    m_server.loger().dbg("%s: done(%d)", prefix.c_str(), ret);
    return ret;
}


