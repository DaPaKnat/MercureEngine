#include "PCH.hpp"

#include "ThreadPool.hpp"

// ============================== [Module Public Local Methods] ============================== //

void    ThreadPool::Initialize    (EngineKey const& p_passkey) noexcept
{
    LOG(LogThreadPool, Warning, "\nInitializing ThreadPool...\n");

    m_running = true;

    for (uint32 i = 0u; i < std::thread::hardware_concurrency() - 1u; ++i)
    {
        m_threads.emplace_back( [&]
        {
            while (true)
            {
                Task task;

                {
                    std::unique_lock lock(m_mutex);

                    // Waits for a task to be available or the shutdown.
                    m_condition.wait(lock, [&] { return !m_running || !m_tasks.empty(); });

                    // Stops running when the module has been shut down and the queue has been emptied.
                    if (!m_running && m_tasks.empty())
                        break;

                    if (!m_tasks.empty())
                    {
                        task = std::move(m_tasks.front());

                        m_tasks.pop_front();
                    }
                }

                std::invoke(task);
            }
        });
    }

    LOG(LogThreadPool, Display, "%d threads were created", m_threads.size());

    m_initialized = true;

    LOG(LogThreadPool, Warning, "\nThreadPool initialized\n");
}

void    ThreadPool::Update        (EngineKey const& p_passkey) noexcept
{
    if (!m_initialized)
    {
        LOG(LogThreadPool, Error, "ThreadPool was not initialized");
        return;
    }
}

void    ThreadPool::Shutdown      (EngineKey const& p_passkey) noexcept
{
    LOG(LogThreadPool, Warning, "\nShutting down ThreadPool...\n");
    
    {
        std::unique_lock lock(m_mutex);

        m_running = false;
    }
    
    m_condition.notify_all();

    for (auto& thread : m_threads)
    {
        if (thread.joinable())
            thread.join();
    }

    m_initialized = false;

    LOG(LogThreadPool, Warning, "\nThreadPool shut down\n");
}

// ============================== [Public Local Methods] ============================== //

void    ThreadPool::ExecuteTask () noexcept
{
    Task task;

    {
        std::unique_lock lock(m_mutex);

        if (m_tasks.empty())
            return;
            
        task = std::move(m_tasks.front());

        m_tasks.pop_front();
    }

    std::invoke(task);
}