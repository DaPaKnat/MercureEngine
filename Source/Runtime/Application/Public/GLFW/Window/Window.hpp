#ifndef __GLFW_WINDOW_HPP__
#define __GLFW_WINDOW_HPP__

#include "InputEnum.hpp"

// ============================== [Delegate Declarations] ============================== //

DECLARE_MULTICAST_DELEGATE_OneParam(FileDropDelegate, ANSICHAR const*);
DECLARE_MULTICAST_DELEGATE_OneParam(CharDelegate,     uint32);

DECLARE_MULTICAST_DELEGATE_TwoParams(WindowResizeDelegate,      int32,   int32);
DECLARE_MULTICAST_DELEGATE_TwoParams(FramebufferResizeDelegate, int32,   int32);
DECLARE_MULTICAST_DELEGATE_TwoParams(ScrollDelegate,            double,  double);
DECLARE_MULTICAST_DELEGATE_TwoParams(KeyDelegate,               EKey,    EState);
DECLARE_MULTICAST_DELEGATE_TwoParams(MouseButtonDelegate,       EButton, EState);

template<typename T> using FileDropCallback          = void (T::*)(ANSICHAR const*);
template<typename T> using CharCallback              = void (T::*)(uint32);

template<typename T> using WindowResizeCallback      = void (T::*)(int32,   int32);
template<typename T> using FramebufferResizeCallback = void (T::*)(int32,   int32);
template<typename T> using ScrollCallback            = void (T::*)(double,  double);
template<typename T> using KeyCallback               = void (T::*)(EKey,    EState);
template<typename T> using MouseButtonCallback       = void (T::*)(EButton, EState);

// ============================== [Data Structures] ============================== //

struct ENGINE_API Offset2D
{
    int32 x;
    int32 y;

};  // !struct Offset2D

struct ENGINE_API Extent2D
{
    int32 width;
    int32 height;

};  // !struct Extent2D

// =============================================================================== //

class ENGINE_API Window : public UniqueObject
{
    public:

    // ============================== [Public Constructor and Destructor] ============================== //

        Window  ();

        ~Window ();
    
    // ============================== [Interface Public Local Methods] ============================== //

        /**
         * @thread_safety This function may be called from any thread.
         */
        INLINE GLFWwindow* GetHandle() const noexcept { return m_handle; }

    // ============================================================================================== //

        void    SetClipboardString  (ANSICHAR const*    p_string)       const noexcept;

        void    SetCursorPosition   (double             p_xPosition,
                                     double             p_yPosition)    const noexcept;

    // ============================================================================================== //

        ANSICHAR const* GetClipboardString      ()  const noexcept;

        Vector2         GetCursorPosition       ()  const noexcept;

        EState          GetMouseButton          (EButton p_button)  const noexcept;

        /**
         * Returns the position, in screen coordinates, of the upper-left corner of the content area.
         *
         * @thread_safety This function must only be called from the main thread.
         */
        Offset2D        GetPosition             ()  const noexcept;

        /**
         * Returns the size, in screen coordinates, of the content area.
         *
         * @thread_safety This function must only be called from the main thread.
         */
        Extent2D        GetExtent               ()  const noexcept;

        /**
         * Returns the size, in pixels, of the framebuffer.
         *
         * @thread_safety This function must only be called from the main thread.
         */
        Extent2D        GetFramebufferExtent    ()  const noexcept;

        /**
         * @thread_safety This function must only be called from the main thread.
         */
        bool            IsFocused               ()  const noexcept;

        /**
         * @thread_safety This function must only be called from the main thread.
         */
        bool            IsIconified             ()  const noexcept;

        /**
         * @thread_safety This function must only be called from the main thread.
         */
        bool            IsMaximized             ()  const noexcept;

    // ============================== [Public Local Methods] ============================== //
        
        template<typename T, FileDropCallback<T> function>
        void    BindFileDropCallback            (T* p_object) noexcept;

        template<typename T, CharCallback<T> function>
        void    BindCharCallback                (T* p_object) noexcept;

        template<typename T, WindowResizeCallback<T> function>
        void    BindWindowResizeCallback        (T* p_object) noexcept;

        template<typename T, FramebufferResizeCallback<T> function>
        void    BindFramebufferResizeCallback   (T* p_object) noexcept;

        template<typename T, ScrollCallback<T> function>
        void    BindScrollCallback              (T* p_object) noexcept;

        template<typename T, KeyCallback<T> function>
        void    BindKeyCallback                 (T* p_object) noexcept;

        template<typename T, MouseButtonCallback<T> function>
        void    BindMouseButtonCallback         (T* p_object) noexcept;

        template<typename T, FileDropCallback<T> function>
        void    RemoveFileDropCallback          (T* p_object) noexcept;

        template<typename T, CharCallback<T> function>
        void    RemoveCharCallback              (T* p_object) noexcept;

        template<typename T, WindowResizeCallback<T> function>
        void    RemoveWindowResizeCallback      (T* p_object) noexcept;

        template<typename T, FramebufferResizeCallback<T> function>
        void    RemoveFramebufferResizeCallbac  (T* p_object) noexcept;

        template<typename T, ScrollCallback<T> function>
        void    RemoveScrollCallback            (T* p_object) noexcept;

        template<typename T, KeyCallback<T> function>
        void    RemoveKeyCallback               (T* p_object) noexcept;

        template<typename T, MouseButtonCallback<T> function>
        void    RemoveMouseButtonCallback       (T* p_object) noexcept;

    private:
 
    // ============================== [Private Local Properties] ============================== //

        GLFWwindow* m_handle;

    // ======================================================================================== //

        FileDropDelegate            m_fileDropDelegate;

        CharDelegate                m_charDelegate;

        WindowResizeDelegate        m_windowResizeDelegate;

        FramebufferResizeDelegate   m_framebufferResizeDelegate;

        ScrollDelegate              m_scrollDelegate;

        KeyDelegate                 m_keyDelegate;

        MouseButtonDelegate         m_mouseButtonDelegate;

    // ============================== [Friend Methods] ============================== //

        /**
         * This function is called when the window is moved.
         *
         * @param p_window      The window that was moved.
         * @param p_xPosition   The new x-coordinate, in screen coordinates, of the upper-left corner of the content area of the window.
         * @param p_yPosition   The new y-coordinate, in screen coordinates, of the upper-left corner of the content area of the window.
         */
        friend void GLFWWindowPosCallback       (GLFWwindow*        p_window,
                                                 int32              p_xPosition,
                                                 int32              p_yPosition)    noexcept;

        /**
         * This function is called when the window is resized.
         *
         * @param p_window  The window that was resized.
         * @param p_width   The new width, in screen coordinates, of the window.
         * @param p_height  The new height, in screen coordinates, of the window.
         */
        friend void GLFWWindowSizeCallback      (GLFWwindow*        p_window,
                                                 int32              p_width,
                                                 int32              p_height)       noexcept;

        /**
         * This function is called when the user attempts to close the window.
         *
         * The close callback is not triggered by glfwDestroyWindow.
         *
         * @param p_window  The window that the user attempted to close.
         */
        friend void GLFWWindowCloseCallback     (GLFWwindow*        p_window)       noexcept;

        /**
         * This function is called when the content area of the window needs to be redrawn.
         *
         * @param p_window  The window whose content needs to be refreshed.
         */
        friend void GLFWWindowRefreshCallback   (GLFWwindow*        p_window)       noexcept;

        /**
         * This function is called when the window gains or loses input focus.
         *
         * @param p_window      The window that gained or lost input focus.
         * @param p_isfocused   "GLFW_TRUE" if the window was given input focus, or "GLFW_FALSE" if it lost it.
         */
        friend void GLFWWindowFocusCallback     (GLFWwindow*        p_window,
                                                 int32              p_isFocused)    noexcept;

        /**
         * This function is called when the window is iconified or restored.
         *
         * @param p_window      The window that was iconified or restored.
         * @param p_isIconified GLFW_"TRUE" if the window was iconified, or GLFW_"FALSE" if it was restored.
         */
        friend void GLFWWindowIconifyCallback   (GLFWwindow*        p_window,
                                                 int32              p_isIconified)  noexcept;

        /**
         * This function is called when the window is maximized or restored.
         *
         * @param p_window      The window that was maximized or restored.
         * @param p_isMaximized "GLFW_TRUE" if the window was maximized, or "GLFW_FALSE" if it was restored.
         */
        friend void GLFWWindowMaximizeCallback  (GLFWwindow*        p_window,
                                                 int32              p_isMaximized)  noexcept;

        /**
         * This function is called when the framebuffer of the specified window is resized.
         *
         * @param p_window  The window whose framebuffer was resized.
         * @param p_width   The new width, in pixels, of the framebuffer.
         * @param p_height  The new height, in pixels, of the framebuffer.
         */
        friend void GLFWFramebufferSizeCallback (GLFWwindow*        p_window,
                                                 int32              p_width,
                                                 int32              p_height)       noexcept;

        /**
         * This function is called when the content scale of the specified window changes.
         *
         * @param p_window  The window whose content scale changed.
         * @param p_xScale  The new x-axis content scale of the window.
         * @param p_yScale  The new y-axis content scale of the window.
         */
        friend void GLFWContentScaleCallback    (GLFWwindow*        p_window,
                                                 float              p_xScale,
                                                 float              p_yScale)       noexcept;

        /**
         * This function is called when a mouse button is pressed or released.
         *
         * When a window loses input focus, it will generate synthetic mouse button release events for all pressed mouse buttons.
         *
         * @param p_window  The window that received the event.
         * @param p_button  The mouse button that was pressed or released.
         * @param p_action  One of "GLFW_PRESS" or "GLFW_RELEASE".
         * @param p_mods    Bit field describing which modifier keys were held down.
         */
        friend void GLFWMouseButtonCallback     (GLFWwindow*        p_window,
                                                 int32              p_button,
                                                 int32              p_action,
                                                 int32              p_mods)         noexcept;

        /**
         * This function is called when the cursor is moved.
         *
         * The callback is provided with the position, in screen coordinates, relative to the upper-left corner of the content area of the window.
         *
         * @param p_window      The window that received the event.
         * @param p_xPosition   The new cursor x-coordinate, relative to the left edge of the content area.
         * @param p_yPosition   The new cursor y-coordinate, relative to the left edge of the content area.
         */
        friend void GLFWCursorPosCallback       (GLFWwindow*        p_window,
                                                 double             p_xPosition,
                                                 double             p_yPosition)    noexcept;

        /**
         * This function is called when the cursor enters or leaves the content area of the window.
         *
         * @param p_window  The window that received the event.
         * @param p_entered "GLFW_TRUE" if the cursor entered the window's content area, or "GLFW_FALS" if it left it.
         */
        friend void GLFWCursorEnterCallback     (GLFWwindow*        p_window,
                                                 int32              p_entered)      noexcept;

        /**
         * This function is called when a scrolling device is used, such as a mouse wheel or scrolling area of a touchpad.
         *
         * @param p_window  The window that received the event.
         * @param p_xOffset The scroll offset along the x-axis.
         * @param p_yOffset The scroll offset along the y-axis.
         */
        friend void GLFWScrollCallback          (GLFWwindow*        p_window,
                                                 double             p_xOffset,
                                                 double             p_yOffset)      noexcept;

        /**
         * This function is called when a key is pressed, repeated or released.
         *
         * @param p_window      The window that received the event.
         * @param p_key         The window that received the event.
         * @param p_scancode    The system-specific scancode of the key.
         * @param p_action      "GLFW_PRESS", "GLFW_RELEASE" or "GLFW_REPEAT".
         * @param p_mods        Bit field describing which modifier keys were held down.
         */
        friend void GLFWKeyCallback             (GLFWwindow*        p_window,
                                                 int32              p_key,
                                                 int32              p_scancode,
                                                 int32              p_action,
                                                 int32              p_mods)         noexcept;

        /**
         * This function is called when a Unicode character is input.
         *
         * @param p_window      The window that received the event.
         * @param p_codepoint   The Unicode code point of the character.
         */
        friend void GLFWCharCallback            (GLFWwindow*        p_window,
                                                 uint32             p_codepoint)    noexcept;

        /**
         * This function is called when one or more dragged files are dropped on the window.
         *
         * @param p_window  The window that received the event.
         * @param p_count   The number of dropped files.
         * @param p_paths   The UTF-8 encoded file and/or directory path names.
         */
        friend void GLFWDropCallback            (GLFWwindow*        p_window,
                                                 int32              p_count,
                                                 ANSICHAR const**   p_paths)        noexcept;

};  // !class Window

#include "Window.inl"

#endif // !__GLFW_WINDOW_HPP__