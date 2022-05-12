#ifndef __LOG_LEVEL_HPP__
#define __LOG_LEVEL_HPP__

#include "HAL/Platform.hpp"

namespace Log
{
    enum Level : uint8
    {
        NoLog,

        Fatal,

        Error,

        Warning,

        Display,

        Profiler,

        Log,

        All = Log
    };
}

#endif // !__LOG_LEVEL_HPP__