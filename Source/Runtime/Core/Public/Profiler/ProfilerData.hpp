#ifndef __PROFILER_DATA_HPP__
#define __PROFILER_DATA_HPP__

/**
 * Used to store Profiler data (function line, number of calls, etc).
 */
struct ProfilerData
{
    // ============================== [Public Local Properties] ============================== //

        /** File name containing the called function. */
        char const* m_fileName;

        /** Called function name. */
        char const* m_functionName;

        /** Line number of the called function in the file. */
        int const   m_lineNumber;

        /** Number of CPU cycles used for the called function. */
        __int64     m_cycles;

        /** Number of calls to the function. */
        __int64     m_calls;

    // ============================== [Public Constructors and Destructor] ============================== //

        ProfilerData    () noexcept;

        ProfilerData    (char const*            p_fileName,
                         char const*            p_functionName,
                         int const              p_lineNumber,
                         __int64                p_cycles = 0,
                         __int64                p_calls = 0) noexcept;

        ProfilerData    (ProfilerData const&    p_copy) noexcept;

        ProfilerData    (ProfilerData&&         p_move) noexcept;

        ~ProfilerData   () noexcept;

    // ============================== [Public Local Operators] ============================== //

        ProfilerData&   operator=   (ProfilerData const&    p_copy) noexcept;

        ProfilerData&   operator=   (ProfilerData&&         p_move) noexcept;

}; // !struct ProfilerData

/**
 * Struct used to store a () operator for ProfilerData comparisons.
 */
struct ProfilerDataLineComparator
{
    // ============================== [Public Local Operators] ============================== //

        bool    operator()  (ProfilerData const&    p_lhs,
                             ProfilerData const&    p_rhs)  const noexcept;

}; // !struct ProfilerDataLineComparator

/**
 * Struct used to store a () operator for std::pair<ProfilerData, ProfilerData> comparisons.
 */
struct ProfilerDataCycleComparator
{
    // ============================== [Public Local Operators] ============================== //

        bool    operator()  (std::pair<ProfilerData, ProfilerData> const&    p_lhs,
                             std::pair<ProfilerData, ProfilerData> const&    p_rhs)  const noexcept;

}; // !struct ProfilerDataCycleComparator

#include "ProfilerData.inl"

#endif // !__PROFILER_DATA_HPP__