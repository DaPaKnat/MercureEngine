#ifndef __PROFILER_MARKER_INL__
#define __PROFILER_MARKER_INL__

#include "ProfilerMarker.hpp"
#include "Profiler.hpp"

// ============================== [Public Constructors and Destructor] ============================== //

/**
 * Default constructor.
 */
INLINE ProfilerMarker::ProfilerMarker   ()  noexcept    :
    m_onStack   { true }
{

}

/**
 * Constructor used to start profiling on a function.
 */
INLINE ProfilerMarker::ProfilerMarker   (char const*    p_fileName,
                                         char const*    p_functionName,
                                         int const      p_lineNumber)  noexcept    :
    m_onStack   { true }
{
    Profiler::Get()->StartProfiling(p_fileName, p_functionName, p_lineNumber);
}

/**
 * Default destructor.
 */
INLINE ProfilerMarker::~ProfilerMarker  ()	noexcept
{
    EndMarker();
}

// ============================== [Public Local Methods] ============================== //

/**
 * Notify the Profiler to stop the profiling process.
 */
INLINE void ProfilerMarker::EndMarker   ()	noexcept
{
    if (m_onStack)
    {
        Profiler::Get()->EndProfiling();

        m_onStack = false;
    }
}

#endif // !__PROFILER_MARKER_INL__