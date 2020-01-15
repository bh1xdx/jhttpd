
#include <string>
#include <iostream>
#include <chrono>
#include <ctime>
#include <thread>

#include "jhttpd.h"

class Helper
{
public:
    static uint64_t msSinceEpoch()
    {
        using MS = std::chrono::milliseconds;
        auto tpnow = std::chrono::system_clock::now();
        auto tpms = std::chrono::time_point_cast<MS>(tpnow);
        uint64_t ts = tpms.time_since_epoch().count();
        return (uint64_t)ts;
    }

    static std::string timeByMsSinceEpoch(const uint64_t &ts, const std::string & format = "%Y.%m.%d-%H:%M:%S", bool withms = true)
    {
        uint64_t sec = ts / 1000;
        auto ms = ts - sec * 1000;
        struct tm _tm;
        _local_time(_tm, sec);

        char buf[1024] = { 0 };
        std::strftime(buf, sizeof(buf), format.c_str(), &_tm);
        std::string result(buf);
        if (withms)
        {
            std::snprintf(buf, sizeof(buf), ".%03d", (int)ms);
            result += buf;
        }
        return std::move(result);
    }

    static std::string datetime()
    {
        return timeByMsSinceEpoch(msSinceEpoch());
    }

private:
    static void _local_time(struct tm &_tm, const uint64_t & secSinceEpoch)
    {
        uint64_t sec = secSinceEpoch;

#if defined(HISI3559A) || defined(HISI3519A)
        sec += 8 * 60 * 60;
#endif
#ifdef _WINDOWS
        localtime_s(&_tm, (const time_t*)&sec);
#else // _WINDOWS
        localtime_r((const time_t*)&sec, &_tm);
#endif // _WINDOWS
    }
};



std::string index_html=R"html(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <title>JHTTPD DEMO</title>
    <meta http-equiv="refresh" content="1">
</head>

<style type="text/css">
    body {
        width: 1000px;
        margin-left: auto;
        margin-right: auto;
        font-family: arial,sans-serif;
    }

    h4 {
        text-align: center;
        color: #000000;
    }

    footer {
        text-align: center;
        font-size: 11px;
        padding-top: 1cm;
        line-height: 15px;
        color: #000000;
    }

    table {
        width: 100%;
        margin: 15px 0;
        border: 2px;
    }

        table.hovertable {
            font-family: verdana,arial,sans-serif;
            font-size: 11px;
            border-width: 1px;
            border-color: #999999;
            border-collapse: collapse;
        }

            table.hovertable th {
                background-color: #00a5ff;
                color: #ffffff;
                #background-color:#c3dde0;
                border-width: 1px;
                padding: 8px;
                border-style: solid;
                border-color: #a9c6c9;
            }

            table.hovertable tr {
                background-color: #d4e3e5;
                border: 1px solid #6dcbfe;
            }

                table.hovertable tr:nth-child(odd) {
                    background-color: #aae1fe;
                }

                table.hovertable tr:nth-child(even) {
                    background-color: #fdfdfd;
                }

            table.hovertable td {
                border-width: 1px;
                padding: 8px;
                border-style: solid;
                border-color: #a9c6c9;
            }
</style>
<body>
    <h4>JHTTPD DEMO</h4>
    <table class="hovertable">
        <tr>
            <th>Info Header 1</th>
            <th>Info Header 2</th>
            <th>Info Header 3</th>
        </tr>
        <tr>
            <td>Item 1A</td>
            <td>Item 1B</td>
            <td>Item 1C</td>
        </tr>
        <tr>
            <td>Item 2A</td>
            <td>Item 2B</td>
            <td>Item 2C</td>
        </tr>
        <tr>
            <td>Item 3A</td>
            <td>Item 3B</td>
            <td>Item 3C</td>
        </tr>
        <tr>
            <td>Item 4A</td>
            <td>Item 4B</td>
            <td>Item 4C</td>
        </tr>
        <tr>
            <td>Item 5A</td>
            <td>Item 5B</td>
            <td>Item 5C</td>
        </tr>
    </table>
    <footer>jhttd 2020<br>by jerry-liu@outlook.com</footer>
</body>
</html>

)html";

inline std::string replace_all(std::string str, const std::string& from, const std::string& to) {
    if (!from.empty())
    {
        size_t start_pos = 0;
        while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
            str.replace(start_pos, from.length(), to);
            start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
        }
    }
    return std::move(str);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// JResp
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
class JResp : public JHttpResp
{
public:
    JResp(){}

    uint32_t get(const std::string & path, std::string & ret_type, std::string &ret_datetime, std::string &ret_content)override
    {
        uint32_t res = (uint32_t)-1;
        do
        {

            if (path == "/demo")
            {
                res = 200;
                ret_type = ".html";

                ret_content = "<html><head><title> DEMO</title></head><body>FROM DEMO of JHTTPD</br> " + Helper::datetime() + " </body></html>\r\n";
            }
            else if (path == "/index.html")
            {
                res = 200;
                ret_type = ".html";
                ret_content = replace_all(index_html, "2020", Helper::datetime());
            }
        } while (false);
        return res;
    }
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// JLog
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
class JLog : public JHttpLog
{
public:
    JLog() {};
    void log(Level level, const std::string & log) override;
};

void JLog::log(Level level, const std::string & log)
{
    std::string prefix = "UNK";
    switch (level)
    {
    case Level::Fatal:
        prefix = "FAT";
        break;
    case Level::Error:
        prefix = "ERR";
        break;
    case Level::Warn:
        prefix = "WRN";
        break;
    case Level::Trace:
        prefix = "TRA";
        break;
    case Level::Info:
        prefix = "INF";
        break;
    case Level::Debug:
        prefix = "DBG";
        break;
    default:
        prefix = "XXX";
        break;
    };
    std::cout << "DEMO:" + prefix + ":" + log << std::endl;
}

void main()
{
    JHttpService * s = nullptr;

    JResp resp;
    JLog log;

    do
    {
        s = JHttpCreateService();
        if (!s)
            break;

        s->addResp(&resp);
        s->setLog(&log);

#if 0
        s->run();
#else
        s->run(false);
        std::this_thread::sleep_for(std::chrono::minutes(2));
        s->stop();
#endif
    } while (false);

    JHttpdestroyService(s);
}

