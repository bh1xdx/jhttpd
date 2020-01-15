

#include "jlog.h"
#include "jhelper.h"

#include <cstdarg>
#include <iostream>


#define VIP_LOG_MAX_LENGTH_PER_LINE 1024

JLoger::JLoger()
{
    m_output_device = nullptr;
}


JLoger::~JLoger()
{
}

void JLoger::log(Level level, const std::string & log)
{
    do
    {
        {
            std::lock_guard<std::mutex> _locker(m_mutex);
            if (m_output_device)
            {
                m_output_device->log(level, log);
                break;
            }
        }

        if (level == Level::Fatal || level == Level::Error || level == Level::Fatal)
            std::cerr << log << std::endl;
        else
            std::cout << log << std::endl;
    } while (false);
}

#define LOG_OUTPUT(L) do { va_list args; va_start(args, format); output2(L, format, args);  va_end(args); } while (false)
void JLoger::output(JHttpLog::Level level, const std::string format, ...)
{
    LOG_OUTPUT(level);
}

void JLoger::fal(const std::string format, ...)
{
    LOG_OUTPUT(Level::Fatal);
}

void JLoger::err(const std::string format, ...)
{
    LOG_OUTPUT(Level::Error);
}

void JLoger::wrn(const std::string format, ...)
{
    LOG_OUTPUT(Level::Warn);
}

void JLoger::tra(const std::string format, ...)
{
    LOG_OUTPUT(Level::Trace);
}

void JLoger::inf(const std::string format, ...)
{
    LOG_OUTPUT(Level::Info);
}

void JLoger::dbg(const std::string format, ...)
{
    LOG_OUTPUT(Level::Debug);
}

void JLoger::output2(JLoger::Level level, const std::string & format, va_list &args)
{
    char szBuff[VIP_LOG_MAX_LENGTH_PER_LINE] = { 0 };
    std::vsnprintf(szBuff, sizeof(szBuff), format.c_str(), args);
    auto pre = prefix(level);
    std::string _log = pre + ": " + szBuff;
    log(level, _log);
}

std::string JLoger::prefix(JLoger::Level level)
{
    std::string ret;
    switch (level)
    {
    case Level::Fatal:
        ret = "FAL";
        break;
    case Level::Error:
        ret = "ERR";
        break;
    case Level::Warn:
        ret = "WRN";
        break;
    case Level::Trace:
        ret = "TRA";
        break;
    case Level::Info:
        ret = "INF";
        break;
    case Level::Debug:
        ret = "DBG";
        break;
    default:
        ret = "UNK";
        break;
    }
    return Helper::datetime() + ":" + ret + "$";
}




