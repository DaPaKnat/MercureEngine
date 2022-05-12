#ifndef __ENGINE_HPP__
#define __ENGINE_HPP__

#include "EngineKey.hpp"

#define ENGINE_NAME     "MercureEngine"
#define ENGINE_VERSION  MAKE_VERSION(1, 0, 0)

// ============================== [Forward Declarations] ============================== //

class World;
class GameUserSettings;
class EngineModuleManager;

// ==================================================================================== //

class ENGINE_API Engine : public UniqueObject
{
    public:

    // ============================== [Public Constructor and Destructor] ============================== //

        Engine  ();

        ~Engine ();

    // ============================== [Public Local Methods] ============================== //

        /**
         * Engine's main loop.
         *
         * @warning This function must not be called outside of "main".
         */
        void    Run             ();

        /**
         * Engine's tick function.
         *
         * @warning This function must not be called outside of "main".
         */
        void    Tick            (float p_deltaTime);

        /**
         * Returns whether or not the Engine is running.
         *
         * @thread_safety This function may be called from any thread.
         */
        bool    IsRunning       ()  const noexcept;

        /**
         * Requests the Engine to shutdown.
         *
         * @thread_safety This function may be called from any thread.
         */
        void    RequestShutdown ()  noexcept;

        World*  GetWorld        ()  noexcept;

    // ==================================================================================== //

        /**
         * @thread_safety This function may be called from any thread.
         */
        INLINE std::unique_ptr<EngineModuleManager> const&  GetModuleManager    ()  const noexcept  { return m_moduleManager; }

        /**
         * @thread_safety This function may be called from any thread.
         */
        INLINE std::unique_ptr<GameUserSettings>    const&  GetGameUserSettings ()  const noexcept  { return m_gameUserSettings; }

    private:

    // ============================== [Private Local Properties] ============================== //

        EngineKey                               m_passkey;

        std::atomic_bool                        m_running;

        std::unique_ptr<EngineModuleManager>    m_moduleManager;

        std::unique_ptr<GameUserSettings>       m_gameUserSettings;

        std::unique_ptr<World>                  m_world;

};  // !class Engine

extern ENGINE_API Engine* GEngine;

#endif // !__ENGINE_HPP__