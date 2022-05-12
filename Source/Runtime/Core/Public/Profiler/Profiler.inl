#ifndef __PROFILER_INL__
#define __PROFILER_INL__

#include "Profiler.hpp"

// ============================== [Public Static Methods] ============================== //

/**
 * Return the singleton instance and create it if it was not.
 */
INLINE Profiler*    Profiler::Get   ()	noexcept
{
    static Profiler profilerInstance;

    return &profilerInstance;
}

// ============================== [Private Constructors and Destructor] ============================== //

/**
 * Default constructor.
 */
INLINE Profiler::Profiler   ()  noexcept    :
    m_profilerDataSet   { std::map<ProfilerData, ProfilerData, ProfilerDataLineComparator>() },
    m_profilerDataStack { std::stack<ProfilerData*>() },
    m_lastCPUCount      { 0 },
    m_totalCPUCount     { 0 }
{
    m_profilerDataSet.insert(std::make_pair(ProfilerData(__FILE__, "Root", 0), ProfilerData(__FILE__, "Root", 0)));

    m_profilerDataStack.push(&m_profilerDataSet.begin()->second);

    m_lastCPUCount = GetCPUCount();
}

/**
 * Default destructor.
 */
INLINE Profiler::~Profiler  ()  noexcept
{
    LogProfiling();
}

// ============================== [Private Local Methods] ============================== //

/**
 * Return the current CPU count.
 */
INLINE __int64  Profiler::GetCPUCount   ()  const noexcept
{
    #if defined(_M_IX86)
        _asm rdtsc
    #else
        return clock();
    #endif
}

#endif // !__PROFILER_INL__