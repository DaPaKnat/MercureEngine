#ifndef __PROFILER_HPP__
#define __PROFILER_HPP__

#include "CoreMinimal.hpp"

#include "Log/Log.hpp"
#include "ProfilerData.hpp"

class ENGINE_API Profiler
{
    typedef std::map<ProfilerData, ProfilerData, ProfilerDataLineComparator> ProfilerDataSet;
    
    typedef std::stack<ProfilerData*> ProfilerDataStack;

    public:
        
    // ============================== [Public Static Properties] ============================== //

        static Profiler*    Get ()	noexcept;

    // ============================== [Public Local Methods] ============================== //

        void    StartProfiling  (char const*    p_fileName,
                                 char const*    p_functionName,
                                 int const      p_linenumber)	noexcept;

        void    EndProfiling    ()	noexcept;

        void    LogProfiling    ()	noexcept;

    private:

    // ============================== [Public Local Properties] ============================== //

        /** Set of ProfilerData used later for logging. */
        ProfilerDataSet     m_profilerDataSet;

        /** Stack of ProfilerData. */    
        ProfilerDataStack   m_profilerDataStack;

        /** Last CPU count recorded. */
        __int64             m_lastCPUCount;

        /** Total CPU count recorded. */
        __int64             m_totalCPUCount;

    // ============================== [Private Constructors and Destructor] ============================== //

        Profiler    ()  noexcept;

        Profiler    (Profiler const&    p_copy) = delete;

        Profiler    (Profiler&&         p_move) = delete;

        ~Profiler   ()  noexcept;

    // ============================== [Private Local Methods] ============================== //

        [[nodiscard]] __int64	GetCPUCount ()  const noexcept;

    // ============================== [Private Local Operators] ============================== //

        Profiler&   operator=   (Profiler const&    p_copy) = delete;

        Profiler&   operator=   (Profiler&&         p_move) = delete;

}; // !class Profiler


#include "Profiler.inl"
#include "ProfilerMarker.hpp"

#endif // !__PROFILER_HPP__