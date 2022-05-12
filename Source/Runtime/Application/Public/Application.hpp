#ifndef __APPLICATION_HPP__
#define __APPLICATION_HPP__

#if defined(GLFW)
    #include "GLFW/Application.hpp"
#else
    #error UNKNOWN WINDOW API
#endif

#endif // !__APPLICATION_HPP__