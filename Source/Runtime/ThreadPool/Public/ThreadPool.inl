#include "ThreadPool.hpp"

// ============================== [Public Local Methods] ============================== //

/**
 * Submits a single task to the queue.
 *
 * @param p_isHighPriority  If this task is high priority, it will be added to the front of the queue.
 *
 * @return                  A future containing the return value of the task.
 *
 * @thread_safety           This function may be called from any thread.
 */
template<typename T>
decltype(auto)  ThreadPool::SubmitTask  (T&&  p_task,
                                         bool p_isHighPriority) noexcept
{
    auto wrapper = std::make_shared<std::packaged_task<decltype(p_task()) ()>>(std::move(p_task));
    
    {
        std::unique_lock lock(m_mutex);

        if (p_isHighPriority)
            m_tasks.emplace_front( [wrapper] { (*wrapper)(); });
        else
            m_tasks.emplace_back ( [wrapper] { (*wrapper)(); });
    }

    m_condition.notify_one();

    return wrapper->get_future();
}

/**
 * Submits a bunch of tasks to the queue.
 *
 * @param p_isHighPriority  If the tasks are high priority, they will be added to the front of the queue.
 *
 * @return                  A vector of futures containing the return values of the tasks.
 *
 * @thread_safety           This function may be called from any thread..
 */
template<typename T>
decltype(auto)  ThreadPool::SubmitTasks (std::vector<T>&& p_tasks,
                                         bool             p_isHighPriority) noexcept
{
    std::vector<std::future<decltype(T() ())>> futures(p_tasks.size());

    {
        std::unique_lock lock(m_mutex);

        for (size_t i = 0; i < p_tasks.size(); ++i)
        {
            auto wrapper = std::make_shared<std::packaged_task<decltype(p_tasks[i]()) ()>>(std::move(p_tasks[i]));

            if (p_isHighPriority)
                m_tasks.emplace_front( [wrapper] { (*wrapper)(); });
            else
                m_tasks.emplace_back ( [wrapper] { (*wrapper)(); });

            futures[i] = wrapper->get_future();
        }
    }

    m_condition.notify_all();

    return futures;
}