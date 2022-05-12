#ifndef __GLFW_APPLICATION_HPP__
#define __GLFW_APPLICATION_HPP__

#include "EngineModule.hpp"

#include "Window/Window.hpp"

class ENGINE_API Application : public EngineModule
{
    public:

    // ============================== [Public Static Methods] ============================== //

        static INLINE Application& Get() noexcept
        {
            return GEngine->GetModuleManager()->Get<Application>();
        }

    // ============================== [Public Constructor and Destructor] ============================== //

        Application () = default;

        ~Application() = default;

    // ============================== [Module Public Local Methods] ============================== //

        /**
         * Initializes the GLFW library, creates the window and links the input manager.
         */
        void    Initialize  (EngineKey const& p_passkey) final override;

        /**
         * Processes all pending events.
         *
         * This function should be called each tick of the main loop.
         */
        void    Update      (EngineKey const& p_passkey) final override;

        /**
         * Shutdowns the input manager, detroys the window and terminates the GLFW library.
         */
        void    Shutdown    (EngineKey const& p_passkey) final override;

    // ============================== [Interface Public Local Methods] ============================== //

        /**
         * @thread_safety This function may be called from any thread.
         */
        INLINE std::unique_ptr<Window> const& GetMainWindow() const noexcept { return m_window; }

    private:

    // ============================== [Private Local Properties] ============================== //

        std::unique_ptr<Window> m_window;

    // ============================== [Friend Methods] ============================== //

        /**
         * This function is called with an error code and a human-readable description each time a GLFW error occurs.
         *
         * @param p_error       An error code as an integer.
         * @param p_description A UTF-8 encoded string describing the error.
         */
        friend void ErrorCallback   (int32              p_error,
                                     ANSICHAR const*    p_description)  noexcept;

        /**
         * This function is called when a monitor is connected to or disconnected from the system.
         *
         * @param p_monitor The handle to the monitor that was connected or disconnected.
         * @param p_event   One of "GLFW_CONNECTED" or "GLFW_DISCONNECTED".
         */
        friend void MonitorCallback (GLFWmonitor*       p_monitor,
                                     int32              p_event)        noexcept;

};  // !class Application

#endif // !__GLFW_APPLICATION_HPP__