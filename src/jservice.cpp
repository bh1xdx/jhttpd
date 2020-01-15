#include <iostream>
#include <string>
#include <cassert>

#include "jservice.h"
#include "jlog.h"


#include "jtools.h"



JHttpdErCode JService::addResp(JHttpResp * resp)
{
    JHttpdErCode r = JHttpdErCode::OK;

    do
    {
        if (resp == nullptr)
        {
            r = JHttpdErCode::INVALID_ARGUMENTS;
            break;
        }

        if (m_resps.find(resp) != m_resps.end())
        {
            r = JHttpdErCode::HAS_EXIST_ARGUMENTS;
            break;
        }

        m_resps.insert(resp);
    } while (false);
    return r;
}

JHttpdErCode JService::delResp(JHttpResp * resp)
{
    JHttpdErCode r = JHttpdErCode::OK;

    do
    {
        if (resp == nullptr)
        {
            r = JHttpdErCode::INVALID_ARGUMENTS;
            break;
        }

        if (m_resps.find(resp) != m_resps.end())
            m_resps.erase(resp);
    } while (false);
    return r;
}

JHttpdErCode JService::setLog(JHttpLog *log)
{
    JLoger::instance().set_output_device(log);
    return JHttpdErCode::OK;
}

JHttpdErCode JService::setRoot(const std::string &root)
{
    m_webroot = root;
    return JHttpdErCode::OK;
}

JHttpdErCode JService::setPort(const uint32_t & port)
{
    if ((port & 0xffff) == 0)
        return JHttpdErCode::INVALID_ARGUMENTS;
    m_port = port & 0xffff;
    return JHttpdErCode::OK;
}

JHttpdErCode JService::setThreadCount(const uint32_t count)
{
    if (count < 1)
        return JHttpdErCode::INVALID_ARGUMENTS;
    m_threadCnt = count;
    return JHttpdErCode::OK;
}

JHttpdErCode JService::run(bool block /*= true*/)
{
    return _start(block);
}

void JService::stop()
{
    _stop();
}

void JService::_stop()
{
    std::lock_guard<std::mutex> _lcker(m_mtx);

    if (m_server)
        m_server->stop();
    if (m_thread)
    {
        if (m_thread->joinable())
            m_thread->join();
        m_thread.reset();
    }
    if (m_server)
        m_server.reset();
    m_status = status_t::stopped;
}

JHttpdErCode JService::_start(bool block)
{
    JHttpdErCode ret =JHttpdErCode::FAIL;
    std::string prefix = name() + ":" + __FUNCTION__;

    std::lock_guard<std::mutex> _lcker(m_mtx);

    JLoger::instance().dbg("%s: start ...", prefix.c_str());

    do
    {
        if (m_status == status_t::running)
        {
            ret = JHttpdErCode::HAS_RUNNING;
            break;
        }

        if (!m_server)
        {
            m_server = std::make_shared<JServer>(m_threadCnt);
            for (auto resp : m_resps)
                m_server->resp_add(resp);
        }

        m_server->setupWebRootPath(m_webroot);
        if(!m_server->bind(m_port))
        {
            ret = JHttpdErCode::BIND_PORT_FAIL;
            m_server->loger().fal("%s: bind port %d failed", prefix.c_str(), m_port);
            break;
        }

        if(!m_server->listen(JHTTPD_SERVER_BACKLOG))
        {
            ret = JHttpdErCode::LISTEN_PORT_FAIL;
            m_server->loger().fal("%s: setting listeing mode failed", prefix.c_str());
            break;
        }

        m_server->loger().inf("%s: web-root:[%s] web-port:[%d]", prefix.c_str(), m_webroot.c_str(), (int)m_port);


        m_status = status_t::running;
        if (block)
        {
            m_server->run();
            break;
        }
        m_thread = std::make_shared<std::thread>([](std::shared_ptr<JServer> s) {s->run(); }, m_server);
        ret = JHttpdErCode::OK;
    } while (false);
    JLoger::instance().dbg("%s: done(%d)", prefix.c_str(), ret);
    return ret;
}

