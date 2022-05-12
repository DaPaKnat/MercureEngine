#include "PCH.hpp"

#include "GameUserSettings.hpp"

#include "GLFW/Window/Window.hpp"

// ============================== [Public Constructor and Destructor] ============================== //

Window::Window  ()
{
    GLFWmonitor*       monitor = glfwGetPrimaryMonitor();
    GLFWvidmode const* mode    = glfwGetVideoMode     (monitor);
    
    m_handle = glfwCreateWindow(mode->width, mode->height, ENGINE_NAME, nullptr, nullptr);

    if (m_handle)
    {
        // Passes this class as the user pointer for the callbacks.
        glfwSetWindowUserPointer(m_handle, this);

        // Sets the newly created window's callbacks.
        glfwSetWindowPosCallback         (m_handle, GLFWWindowPosCallback);
        glfwSetWindowSizeCallback        (m_handle, GLFWWindowSizeCallback);
        glfwSetWindowCloseCallback       (m_handle, GLFWWindowCloseCallback);
        glfwSetWindowRefreshCallback     (m_handle, GLFWWindowRefreshCallback);
        glfwSetWindowFocusCallback       (m_handle, GLFWWindowFocusCallback);
        glfwSetWindowIconifyCallback     (m_handle, GLFWWindowIconifyCallback);
        glfwSetWindowMaximizeCallback    (m_handle, GLFWWindowMaximizeCallback);
        glfwSetFramebufferSizeCallback   (m_handle, GLFWFramebufferSizeCallback);
        glfwSetWindowContentScaleCallback(m_handle, GLFWContentScaleCallback);
        glfwSetMouseButtonCallback       (m_handle, GLFWMouseButtonCallback);
        glfwSetCursorPosCallback         (m_handle, GLFWCursorPosCallback);
        glfwSetCursorEnterCallback       (m_handle, GLFWCursorEnterCallback);
        glfwSetScrollCallback            (m_handle, GLFWScrollCallback);
        glfwSetKeyCallback               (m_handle, GLFWKeyCallback);
        glfwSetCharCallback              (m_handle, GLFWCharCallback);
        glfwSetDropCallback              (m_handle, GLFWDropCallback);

        // Shows the window after initialization.
        glfwShowWindow(m_handle);

        LOG(LogApplication, Display, "New window created");
    }
    
    else
        LOG(LogApplication, Fatal, "Failed to create window");
}

Window::~Window ()
{
    glfwDestroyWindow(m_handle);

    LOG(LogApplication, Display, "Window destroyed");
}

// ============================== [Public Local Methods] ============================== //

void    Window::SetClipboardString  (ANSICHAR const* p_string)  const noexcept
{
    glfwSetClipboardString(m_handle, p_string);
}

void    Window::SetCursorPosition   (double p_xPosition,
                                     double p_yPosition) const noexcept
{
    glfwSetCursorPos(m_handle, p_xPosition, p_yPosition);
}

// ==================================================================================== //

ANSICHAR const* Window::GetClipboardString      () const noexcept
{
    return glfwGetClipboardString(m_handle);
}

Vector2         Window::GetCursorPosition       () const noexcept
{
    double x, y;

    glfwGetCursorPos(m_handle, &x, &y);

    return Vector2(static_cast<float>(x), static_cast<float>(y));
}

EState          Window::GetMouseButton          (EButton p_button) const noexcept
{
    return static_cast<EState>(glfwGetMouseButton(m_handle, static_cast<int32>(p_button)));
}

Offset2D        Window::GetPosition             () const noexcept
{
    Offset2D position;

    glfwGetWindowPos(m_handle, &position.x, &position.y);

    return position;
}

Extent2D        Window::GetExtent               () const noexcept
{
    Extent2D extent;

    glfwGetWindowSize(m_handle, &extent.width, &extent.height);

    return extent;
}

Extent2D        Window::GetFramebufferExtent    () const noexcept
{
    Extent2D extent;

    glfwGetFramebufferSize(m_handle, &extent.width, &extent.height);

    return extent;
}

bool            Window::IsFocused               () const noexcept
{
    return glfwGetWindowAttrib(m_handle, GLFW_FOCUSED) == GLFW_TRUE;
}

bool            Window::IsIconified             () const noexcept
{
    return glfwGetWindowAttrib(m_handle, GLFW_ICONIFIED) == GLFW_TRUE;
}

bool            Window::IsMaximized             () const noexcept
{
    return glfwGetWindowAttrib(m_handle, GLFW_MAXIMIZED) == GLFW_TRUE;
}

// ============================== [Private Local Methods] ============================== //

/*void    Window::SetFullscreen   (bool p_isFullscreen) noexcept
{
    GLFWmonitor*       monitor = glfwGetPrimaryMonitor();
    GLFWvidmode const* mode    = glfwGetVideoMode     (monitor);
    
    // If the window was previously in windowed mode, switches to fullscreen mode.
    if (!GameUserSettings::Get()->IsFullscreen() && p_isFullscreen)
    {
        m_isFullscreen = true;

        glfwSetWindowMonitor(m_handle, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
    }

    // If the window was previously in fullscreen mode, switches to windowed mode.
    if (GameUserSettings::Get()->IsFullscreen() && !p_isFullscreen)
    {
        m_isFullscreen = false;

        glfwSetWindowMonitor(m_handle, nullptr, m_position.x, m_position.y, m_windowSize.width, m_windowSize.height, mode->refreshRate);
    }
}*/

// ============================== [Friend Methods] ============================== //

void    GLFWWindowCloseCallback     (GLFWwindow* p_window) noexcept
{
    if (Window* window = reinterpret_cast<Window*>(glfwGetWindowUserPointer(p_window)))
        GEngine->RequestShutdown();
    else
        LOG(LogApplication, Error, "The user pointer has not been set");
}

void    GLFWWindowRefreshCallback   (GLFWwindow* p_window) noexcept
{
    if (Window* window = reinterpret_cast<Window*>(glfwGetWindowUserPointer(p_window)))
    {

    }

    else
        LOG(LogApplication, Error, "The user pointer has not been set");
}

void    GLFWWindowFocusCallback     (GLFWwindow* p_window,
                                     int32       p_isFocused) noexcept
{
    if (Window* window = reinterpret_cast<Window*>(glfwGetWindowUserPointer(p_window)))
    {

    }

    else
        LOG(LogApplication, Error, "The user pointer has not been set");
}

void    GLFWWindowIconifyCallback   (GLFWwindow* p_window,
                                     int32       p_isIconified) noexcept
{
    if (Window* window = reinterpret_cast<Window*>(glfwGetWindowUserPointer(p_window)))
    {

    }

    else
        LOG(LogApplication, Error, "The user pointer has not been set");
}

void    GLFWWindowMaximizeCallback  (GLFWwindow* p_window,
                                     int32       p_isMaximized) noexcept
{
    if (Window* window = reinterpret_cast<Window*>(glfwGetWindowUserPointer(p_window)))
    {

    }

    else
        LOG(LogApplication, Error, "The user pointer has not been set");
}

void    GLFWWindowPosCallback       (GLFWwindow* p_window,
                                     int32       p_xPosition, 
                                     int32       p_yPosition) noexcept
{
    if (Window* window = reinterpret_cast<Window*>(glfwGetWindowUserPointer(p_window)))
    {

    }

    else
        LOG(LogApplication, Error, "The user pointer has not been set");
}

void    GLFWWindowSizeCallback      (GLFWwindow* p_window,
                                     int32       p_width, 
                                     int32       p_height) noexcept
{
    if (Window* window = reinterpret_cast<Window*>(glfwGetWindowUserPointer(p_window)))
    {
        window->m_windowResizeDelegate.Broadcast(p_width, p_height);
    }

    else
        LOG(LogApplication, Error, "The user pointer has not been set");
}

void    GLFWFramebufferSizeCallback (GLFWwindow* p_window,
                                     int32       p_width,
                                     int32       p_height) noexcept
{
    if (Window* window = reinterpret_cast<Window*>(glfwGetWindowUserPointer(p_window)))
    {
        window->m_framebufferResizeDelegate.Broadcast(p_width, p_height);
    }
    
    else
        LOG(LogApplication, Error, "The user pointer has not been set");
}

void    GLFWContentScaleCallback    (GLFWwindow* p_window,
                                     float       p_xScale,
                                     float       p_yScale) noexcept
{
    if (Window* window = reinterpret_cast<Window*>(glfwGetWindowUserPointer(p_window)))
    {

    }

    else
        LOG(LogApplication, Error, "The user pointer has not been set");
}

void    GLFWMouseButtonCallback     (GLFWwindow* p_window,
                                     int32       p_button,
                                     int32       p_action,
                                     int32       p_mods) noexcept
{
    if (Window* window = reinterpret_cast<Window*>(glfwGetWindowUserPointer(p_window)))
    {
        window->m_mouseButtonDelegate.Broadcast(static_cast<EButton>(p_button), static_cast<EState>(p_action));
    }

    else
        LOG(LogApplication, Error, "The user pointer has not been set");
}

void    GLFWCursorPosCallback       (GLFWwindow* p_window,
                                     double      p_xPosition,
                                     double      p_yPosition) noexcept
{
    if (Window* window = reinterpret_cast<Window*>(glfwGetWindowUserPointer(p_window)))
    {

    }

    else
        LOG(LogApplication, Error, "The user pointer has not been set");
}

void    GLFWCursorEnterCallback     (GLFWwindow* p_window,
                                     int32       p_entered) noexcept
{
    if (Window* window = reinterpret_cast<Window*>(glfwGetWindowUserPointer(p_window)))
    {

    }

    else
        LOG(LogApplication, Error, "The user pointer has not been set");
}

void    GLFWScrollCallback          (GLFWwindow* p_window,
                                     double      p_xOffset,
                                     double      p_yOffset) noexcept
{
    if (Window* window = reinterpret_cast<Window*>(glfwGetWindowUserPointer(p_window)))
    {
        window->m_scrollDelegate.Broadcast(p_xOffset, p_yOffset);
    }

    else
        LOG(LogApplication, Error, "The user pointer has not been set");
}

void    GLFWKeyCallback             (GLFWwindow* p_window,
                                     int32       p_key,
                                     int32       p_scancode,
                                     int32       p_action,
                                     int32       p_mods) noexcept
{
    if (Window* window = reinterpret_cast<Window*>(glfwGetWindowUserPointer(p_window)))
    {
        window->m_keyDelegate.Broadcast(static_cast<EKey>(p_key), static_cast<EState>(p_action));
    }

    else
        LOG(LogApplication, Error, "The user pointer has not been set");
}

void    GLFWCharCallback            (GLFWwindow* p_window,
                                     uint32      p_codepoint) noexcept
{
    if (Window* window = reinterpret_cast<Window*>(glfwGetWindowUserPointer(p_window)))
    {
        window->m_charDelegate.Broadcast(p_codepoint);
    }

    else
        LOG(LogApplication, Error, "The user pointer has not been set");
}

void    GLFWDropCallback            (GLFWwindow*      p_window,
                                     int32            p_count,
                                     ANSICHAR const** p_paths) noexcept
{
    if (Window* window = reinterpret_cast<Window*>(glfwGetWindowUserPointer(p_window)))
    {
        for (int32 i = 0; i < p_count; ++i)
        {
            window->m_fileDropDelegate.Broadcast(p_paths[i]);
        }
    }

    else
        LOG(LogApplication, Error, "The user pointer has not been set");
}