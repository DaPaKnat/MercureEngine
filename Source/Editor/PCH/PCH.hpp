#ifndef __PCH_HPP__
#define __PCH_HPP__

// ============================== [STL] ============================== //

#include <any>
#include <set>
#include <map>
#include <array>
#include <deque>
#include <mutex>
#include <stack>
#include <queue>
#include <atomic>
#include <future>
#include <iosfwd>
#include <memory>
#include <thread>
#include <cassert>
#include <cstddef>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <optional>
#include <stdarg.h>
#include <algorithm>
#include <filesystem>
#include <functional>
#include <string_view> 
#include <type_traits>
#include <shared_mutex>
#include <unordered_map>
#include <memory_resource>

// ============================== [OS] ============================== //

#include "HAL/Platform.hpp"

#if defined(_WIN64) || defined(_WIN32)
    #define WIN32_LEAN_AND_MEAN
    #define NOMINMAX

    #include <windows.h>
#endif

// ============================== [Graphics API] ============================== //

#if defined(VULKAN)
    #if defined(_WIN64) || defined(_WIN32)
        #define VK_USE_PLATFORM_WIN32_KHR
    #endif

    #include <vulkan.h>
    #include <shaderc/shaderc.hpp>
#endif

// ============================== [Window API] ============================== //

#if defined(GLFW)
    #if defined(VULKAN)
        #define GLFW_INCLUDE_NONE
        #define GLFW_INCLUDE_VULKAN
    #endif

    #include <glfw3.h>
#endif

// ============================== [External Tools] ============================== //

#define IMGUI_DEFINE_MATH_OPERATORS

#include <imgui.h>
#include <imgui_internal.h>

#include <json.hpp>

using Json = nlohmann::json;

#endif // !__PCH_HPP__