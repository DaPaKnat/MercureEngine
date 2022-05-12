#include "PCH.hpp"

#include "GLFW/Application.hpp"

// ============================== [Module Public Local Methods] ============================== //

void    Application::Initialize (EngineKey const& p_passkey)
{
    LOG(LogApplication, Warning, "\nInitializing Application...\n");

    // Initializes the GLFW library.
    if (glfwInit() == GLFW_FALSE)
        LOG(LogApplication, Fatal,   "GLFW initialization : FAILURE");
    else
        LOG(LogApplication, Display, "GLFW initialization : SUCCESS");

    // Setups the default window hints.
    glfwWindowHint(GLFW_VISIBLE,          GLFW_FALSE);
    glfwWindowHint(GLFW_CLIENT_API,       GLFW_NO_API);
    glfwWindowHint(GLFW_MAXIMIZED,        GLFW_TRUE);
    glfwWindowHint(GLFW_SCALE_TO_MONITOR, GLFW_TRUE);

    // Setups the general callbacks.
    glfwSetErrorCallback  (ErrorCallback);
    glfwSetMonitorCallback(MonitorCallback);

    // Creates the main window.
    m_window = std::make_unique<Window>();

    m_initialized = true;

    LOG(LogApplication, Warning, "\nApplication initialized\n");
}

void    Application::Update     (EngineKey const& p_passkey)
{
    if (!m_initialized)
    {
        LOG(LogApplication, Error, "Application was not initialized");
        return;
    }

    glfwPollEvents();
}

void    Application::Shutdown   (EngineKey const& p_passkey)
{
    LOG(LogApplication, Warning, "\nShutting down Application...\n");

    m_window.reset();

    glfwTerminate();

    m_initialized = false;

    LOG(LogApplication, Warning, "\nApplication shut down\n");
}

// ============================== [Friend Methods] ============================== //

void    ErrorCallback   (int32           p_error, 
                         ANSICHAR const* p_description) noexcept
{
    std::string message;

    switch (p_error)
    {
    #define RESULT(res) case GLFW_##res: message = #res; break
            RESULT(NOT_INITIALIZED);
            RESULT(NO_CURRENT_CONTEXT);
            RESULT(INVALID_ENUM);
            RESULT(INVALID_VALUE);
            RESULT(OUT_OF_MEMORY);
            RESULT(API_UNAVAILABLE);
            RESULT(VERSION_UNAVAILABLE);
            RESULT(PLATFORM_ERROR);
            RESULT(FORMAT_UNAVAILABLE);
            RESULT(NO_WINDOW_CONTEXT);
    #undef  RESULT
    
        default:
            message = "UNKNOWN_ERROR_CODE";
            break;
    }

    message += ": ";
    message += p_description;

    LOG(LogApplication, Error, message.c_str());
}

void    MonitorCallback (GLFWmonitor* p_monitor,
                         int32        p_event) noexcept
{
    if (p_event == GLFW_CONNECTED)
    {
        LOG(LogApplication, Display, "New monitor connected : %s", glfwGetMonitorName(p_monitor));
    }

    else
    {
        LOG(LogApplication, Display, "Monitor disconnected : %s", glfwGetMonitorName(p_monitor));
    }
}