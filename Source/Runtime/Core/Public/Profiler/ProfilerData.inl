#ifndef __PROFILER_DATA_INL__
#define __PROFILER_DATA_INL__

#include <memory>

#include "ProfilerData.hpp"

// ============================== [Public Constructors and Destructor] ============================== //

/**
 * Default constructor.
 */
INLINE ProfilerData::ProfilerData   ()  noexcept    :
    m_fileName      { "" },
    m_functionName  { "" },
    m_lineNumber    { 0 },
    m_cycles        { 0 },
    m_calls         { 0 }
{

}

/**
 * Complete constructor.
 */
INLINE ProfilerData::ProfilerData   (char const*    p_fileName,
                                     char const*    p_functionName,
                                     int const      p_lineNumber,
                                     __int64        p_cycles,
                                     __int64        p_calls)    noexcept    :
    m_fileName      { p_fileName },
    m_functionName  { p_functionName },
    m_lineNumber    { p_lineNumber },
    m_cycles        { p_cycles },
    m_calls         { p_calls }
{

}

/**
 * Copy constructor.
 */
INLINE ProfilerData::ProfilerData   (ProfilerData const&    p_copy) noexcept    :
    m_fileName      { p_copy.m_fileName },
    m_functionName  { p_copy.m_functionName },
    m_lineNumber    { p_copy.m_lineNumber },
    m_cycles        { p_copy.m_cycles },
    m_calls         { p_copy.m_calls }
{

}

/**
 * Move constructor.
 */
INLINE ProfilerData::ProfilerData   (ProfilerData&& p_move) noexcept    :
    m_fileName      { std::move(p_move.m_fileName) },
    m_functionName  { std::move(p_move.m_functionName) },
    m_lineNumber    { std::move(p_move.m_lineNumber) },
    m_cycles        { std::move(p_move.m_cycles) },
    m_calls         { std::move(p_move.m_calls) }
{

}

/**
 * Default destructor.
 */
INLINE ProfilerData::~ProfilerData  ()  noexcept
{

}

// ============================== [Public Local Operators] ============================== //

/**
 * Copy operator.
 */
INLINE ProfilerData&    ProfilerData::operator= (ProfilerData const&    p_copy) noexcept
{
    if (this == &p_copy)
        return *this;

    m_fileName          = p_copy.m_fileName;
    m_functionName      = p_copy.m_functionName;
    (int&)m_lineNumber  = p_copy.m_lineNumber;
    m_cycles            = p_copy.m_cycles;
    m_calls             = p_copy.m_calls;

    return *this;
}

/**
 * Move operator.
 */
INLINE ProfilerData&    ProfilerData::operator= (ProfilerData&& p_move) noexcept
{
    if (this == &p_move)
        return *this;

    m_fileName          = std::move(p_move.m_fileName);
    m_functionName      = std::move(p_move.m_functionName);
    (int&)m_lineNumber  = std::move(p_move.m_lineNumber);
    m_cycles            = std::move(p_move.m_cycles);
    m_calls             = std::move(p_move.m_calls);
    
    return *this;
}

/**
 * Comparison operator (between the line number of 2 ProfilerData).
 */
INLINE bool ProfilerDataLineComparator::operator()  (ProfilerData const&   p_lhs,
                                                     ProfilerData const&   p_rhs) const noexcept
{ 
    return (p_lhs.m_lineNumber == p_rhs.m_lineNumber) ? (strcmp(p_lhs.m_fileName, p_rhs.m_fileName) < 0) : (p_lhs.m_lineNumber < p_rhs.m_lineNumber);
}

/**
 * Comparison operator (between cycles of 2 ProfilerData).
 */
INLINE bool ProfilerDataCycleComparator::operator() (std::pair<ProfilerData, ProfilerData> const&   p_lhs,
                                                     std::pair<ProfilerData, ProfilerData> const&   p_rhs) const noexcept
{
    return p_lhs.second.m_cycles > p_rhs.second.m_cycles;
}

#endif // !__PROFILER_DATA_INL__