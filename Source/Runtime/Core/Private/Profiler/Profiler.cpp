#include "PCH.hpp"

#include "Profiler/Profiler.hpp"

// ============================== [Public Local Methods] ============================== //

/** 
 * Start a profiling session on a function. 
 * Note that only 1 function can be under profiling at the same time.
 */
void    Profiler::StartProfiling    (char const*    p_fileName,
                                     char const*    p_functionName,
                                     int const      p_lineNumber)	noexcept
{
    __int64 runtime = GetCPUCount() - m_lastCPUCount;

    if (m_profilerDataStack.size())
    {
        m_profilerDataStack.top()->m_cycles += runtime;

        m_totalCPUCount += runtime;
    }

    m_profilerDataStack.push(&(m_profilerDataSet.insert(std::make_pair(ProfilerData(p_fileName, p_functionName, p_lineNumber), ProfilerData(p_fileName, p_functionName, p_lineNumber))).first->second));

    m_profilerDataStack.top()->m_calls++;

    m_lastCPUCount = GetCPUCount();
}

/**
 * End a profiling seesion.
 */
void    Profiler::EndProfiling      ()	noexcept
{
    __int64 runTime = GetCPUCount() - m_lastCPUCount;

    m_profilerDataStack.top()->m_cycles += runTime;

    m_totalCPUCount += runTime;

    m_profilerDataStack.pop();

    m_lastCPUCount = GetCPUCount();
}

/**
 *
 */
void    Profiler::LogProfiling      ()	noexcept
{
    std::set<std::pair<ProfilerData, ProfilerData>, ProfilerDataCycleComparator> logs;

    for (ProfilerDataSet::iterator it = m_profilerDataSet.begin(); it != m_profilerDataSet.end(); it++)
        logs.insert(*it);

    ProfilerData    data; 
    double          percent = 0;

    for (std::set<std::pair<ProfilerData, ProfilerData>, ProfilerDataLineComparator>::iterator it = logs.begin(); it != logs.end(); it++)
    {
        data = it->second;

        if (data.m_lineNumber)
        {
            percent = double(data.m_cycles) / double(m_totalCPUCount) * 100;

            LOG(LogCore, Profiler, "[%lld] cycles, (%.3g%%) used in [%lld] call(s) by %s at line %d.",
                data.m_cycles, percent, data.m_calls, data.m_functionName, data.m_lineNumber);
        }
    }
}