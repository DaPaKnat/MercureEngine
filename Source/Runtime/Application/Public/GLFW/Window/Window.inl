#ifndef __GLFW_WINDOW_INL__
#define __GLFW_WINDOW_INL__

/**
 * Binds a member function to be called whenever a file is dropped in the window.
 *
 * @thread_safety This function must only be called from the main thread.
 */
template<typename T, FileDropCallback<T> function>
void    Window::BindFileDropCallback            (T* p_object) noexcept
{
    m_fileDropDelegate.Add<T, function>(p_object);
}

/**
 * Binds a member function to be called whenever a character is typed on the keyboard.
 *
 * @thread_safety This function must only be called from the main thread.
 */
template<typename T, CharCallback<T> function>
void    Window::BindCharCallback                (T* p_object) noexcept
{
    m_charDelegate.Add<T, function>(p_object);
}

/**
 * Binds a member function to be called whenever the window is resized.
 *
 * @thread_safety This function must only be called from the main thread.
 */
template<typename T, WindowResizeCallback<T> function>
void    Window::BindWindowResizeCallback        (T* p_object) noexcept
{
    m_windowResizeDelegate.Add<T, function>(p_object);
}

/**
 * Binds a member function to be called whenever the framebuffer is resized.
 *
 * @thread_safety This function must only be called from the main thread.
 */
template<typename T, FramebufferResizeCallback<T> function>
void    Window::BindFramebufferResizeCallback   (T* p_object) noexcept
{
    m_framebufferResizeDelegate.Add<T, function>(p_object);
}

/**
 * Binds a member function to be called whenever the mouse is scrolled.
 *
 * @thread_safety This function must only be called from the main thread.
 */
template<typename T, ScrollCallback<T> function>
void    Window::BindScrollCallback              (T* p_object) noexcept
{
    m_scrollDelegate.Add<T, function>(p_object);
}

/**
 * Binds a member function to be called whenever a key is pressed.
 *
 * @thread_safety This function must only be called from the main thread.
 */
template<typename T, KeyCallback<T> function>
void    Window::BindKeyCallback                 (T* p_object) noexcept
{
    m_keyDelegate.Add<T, function>(p_object);
}

/**
 * Binds a member function to be called whenever a mouse button is pressed.
 *
 * @thread_safety This function must only be called from the main thread.
 */
template<typename T, MouseButtonCallback<T> function>
void    Window::BindMouseButtonCallback         (T* p_object) noexcept
{
    m_mouseButtonDelegate.Add<T, function>(p_object);
}

/**
 * Removes a member function from the callback list.
 *
 * @thread_safety This function must only be called from the main thread.
 */
template<typename T, FileDropCallback<T> function>
void    Window::RemoveFileDropCallback          (T* p_object) noexcept
{
    m_fileDropDelegate.Remove<T, function>(p_object);
}

/**
 * Removes a member function from the callback list.
 *
 * @thread_safety This function must only be called from the main thread.
 */
template<typename T, CharCallback<T> function>
void    Window::RemoveCharCallback              (T* p_object) noexcept
{
    m_charDelegate.Remove<T, function>(p_object);
}

/**
* Removes a member function from the callback list.
 *
 * @thread_safety This function must only be called from the main thread.
 */
template<typename T, WindowResizeCallback<T> function>
void    Window::RemoveWindowResizeCallback      (T* p_object) noexcept
{
    m_windowResizeDelegate.Remove<T, function>(p_object);
}

/**
 * Removes a member function from the callback list.
 *
 * @thread_safety This function must only be called from the main thread.
 */
template<typename T, FramebufferResizeCallback<T> function>
void    Window::RemoveFramebufferResizeCallbac  (T* p_object) noexcept
{
    m_framebufferResizeDelegate.Remove<T, function>(p_object);
}

/**
 * Removes a member function from the callback list.
 *
 * @thread_safety This function must only be called from the main thread.
 */
template<typename T, ScrollCallback<T> function>
void    Window::RemoveScrollCallback            (T* p_object) noexcept
{
    m_scrollDelegate.Remove<T, function>(p_object);
}

/**
 * Removes a member function from the callback list.
 *
 * @thread_safety This function must only be called from the main thread.
 */
template<typename T, KeyCallback<T> function>
void    Window::RemoveKeyCallback               (T* p_object) noexcept
{
    m_keyDelegate.Remove<T, function>(p_object);
}

/**
 * Removes a member function from the callback list.
 *
 * @thread_safety This function must only be called from the main thread.
 */
template<typename T, MouseButtonCallback<T> function>
void    Window::RemoveMouseButtonCallback       (T* p_object) noexcept
{
    m_mouseButtonDelegate.Remove<T, function>(p_object);
}

#endif // !__GLFW_WINDOW_INL__