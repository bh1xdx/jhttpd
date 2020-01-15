

#include "jtheadpool.h"

ThreadPool::ThreadPool(size_t threads)
    : m_stop(false)
{
    if (threads < 1)
        threads = 1;

    for (size_t i = 0; i < threads; ++i)
        m_workers.emplace_back(
            [this]
    {
        for (;;)
        {
            std::function<void()> task;
            {
                std::unique_lock<std::mutex> lock(this->m_mutex);
                this->m_condition.wait(lock, [this] { return this->m_stop || !this->m_tasks.empty(); });

                if (this->m_stop && this->m_tasks.empty())
                    return;

                task = std::move(this->m_tasks.front());
                this->m_tasks.pop();
            }
            task();
        }
    }
    );
}

ThreadPool::~ThreadPool()
{
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        m_stop = true;
    }
    m_condition.notify_all();
    for (auto &w : m_workers)
        w.join();
}



