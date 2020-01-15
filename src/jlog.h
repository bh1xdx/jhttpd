


#ifndef __JHTTPD_JLOG_H__
#define __JHTTPD_JLOG_H__

#include <mutex>

#include "jhttpd.h"


class JLoger : public JHttpLog
{
    JLoger();
    ~JLoger();

public:
    static JLoger & instance() { static JLoger l; return l; };

    void set_output_device(JHttpLog * d) {
        std::lock_guard<std::mutex> _locker(m_mutex);
        m_output_device = d;
    }
    void output(Level level, const std::string format, ...);

    void fal(const std::string format, ...);
    void err(const std::string format, ...);
    void wrn(const std::string format, ...);
    void tra(const std::string format, ...);
    void inf(const std::string format, ...);
    void dbg(const std::string format, ...);


protected:
    void log(Level level, const std::string & log) override;
private:
    void output2(Level level, const std::string & format, va_list &args);
    std::string prefix(Level level);

    JHttpLog *m_output_device;
    std::mutex m_mutex;

};

#endif // __JHTTPD_JLOG_H__