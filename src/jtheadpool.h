


#ifndef __JHTTPD_THREAD_POOL_H__
#define __JHTTPD_THREAD_POOL_H__


#include "nocopyable.h"

#include <vector>
#include <queue>
#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <functional>
#include <stdexcept>

class ThreadPool : public NoCopyable{

public:
    ThreadPool(size_t);
    ~ThreadPool();

    template<class F, class... Args>
    auto enqueue(F&& f, Args&&... args)->std::future<typename std::result_of<F(Args...)>::type>
    {
        using return_type = typename std::result_of<F(Args...)>::type;

        auto task = std::make_shared< std::packaged_task<return_type()> >(
            std::bind(std::forward<F>(f), std::forward<Args>(args)...)
            );

        std::future<return_type> res = task->get_future();
        {
            std::unique_lock<std::mutex> lock(m_mutex);
            if (m_stop)
                throw std::runtime_error("enqueue on stopped ThreadPool");

            m_tasks.emplace([task]() { (*task)(); });
        }
        m_condition.notify_one();
        return res;
    }

private:
    std::vector<std::thread> m_workers;
    std::queue<std::function<void()>> m_tasks;

    std::mutex m_mutex;
    std::condition_variable m_condition;
    bool m_stop;
};

#endif // __JHTTPD_THREAD_POOL_H__