#ifndef __THREAD_POOL_HPP__
#define __THREAD_POOL_HPP__

#include "EngineModule.hpp"

class ENGINE_API ThreadPool : public EngineModule
{
    public:

    // ============================== [Alias] ============================== //

        using Task = std::function<void()>;

    // ============================== [Public Static Method] ============================== //

        static INLINE ThreadPool& Get() noexcept
        { 
            return GEngine->GetModuleManager()->Get<ThreadPool>();
        }

    // ============================== [Public Constructor and Destructor] ============================== //

        ThreadPool  () = default;

        ~ThreadPool () = default;

    public:

    // ============================== [Module Public Local Methods] ============================== //

        /**
         * Spawns the maximum number of threads (hardware_concurrency - 1) and launches them in an infinite loop while the ThreadPool is running.
         */
        void    Initialize  (EngineKey const& p_passkey) noexcept final override;

        /**
         * This function actually does nothing.
         */
        void    Update      (EngineKey const& p_passkey) noexcept final override;

        /**
         * Notifies each thread to stop running after the queue has been emptied and waits for them to finish execution.
         */
        void    Shutdown    (EngineKey const& p_passkey) noexcept final override;

    // ============================== [Public Local Methods] ============================== //

        /**
         * Retrieves one task from the queue and executes it.
         *
         * @thread_safety This function may be called from any thread.
         */
        void ExecuteTask() noexcept;

    // ==================================================================================== //

        template<typename T>
        decltype(auto)  SubmitTask  (T&&                p_task,
                                     bool               p_isHighPriority = false)   noexcept;

        template<typename T>
        decltype(auto)  SubmitTasks (std::vector<T>&&   p_tasks,
                                     bool               p_isHighPriority = false)   noexcept;

    // ==================================================================================== //

        INLINE std::vector<std::thread> const& GetThreads() const noexcept { return m_threads; }

    private:

    // ============================== [Private Local Properties] ============================== //
        
        bool                        m_running;

        std::mutex                  m_mutex;

        std::condition_variable     m_condition;

        std::deque<Task>            m_tasks;

        std::vector<std::thread>    m_threads;

};  // !class ThreadPool

#include "ThreadPool.inl"

#endif // !__THREAD_POOL_HPP__