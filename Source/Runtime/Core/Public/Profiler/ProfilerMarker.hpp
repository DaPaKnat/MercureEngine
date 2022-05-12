#ifndef __PROFILER_MARKER_HPP__
#define __PROFILER_MARKER_HPP__

#define STRINGIZE(x) #x

#define NAMED_PROFILER_MARKER(x) ProfilerMarker x(__FILE__, STRINGIZE(x), __LINE__);

#define PROFILER_MARKER ProfilerMarker marker(__FILE__, __FUNCTION__, __LINE__);

#define END_NAMED_PROFILER_MARKER(x) x.EndMarker();

/**
 * Used to mark a function or a code block, enabling profiling for it.
 */
class ProfilerMarker
{
    public:

    // ============================== [Public Constructors and Destructor] ============================== //

        ProfilerMarker  ()  noexcept;

        ProfilerMarker  (char const*            p_fileName,
                         char const*            p_functionName,
                         int const              p_lineNumber) noexcept;

        ProfilerMarker  (ProfilerMarker const&  p_copy) noexcept = delete;

        ProfilerMarker  (ProfilerMarker&&       p_move) noexcept = delete;

        ~ProfilerMarker ()  noexcept;

    // ============================== [Public Local Methods] ============================== //

        void    EndMarker   ()	noexcept;
        
    // ============================== [Public Local Operators] ============================== //

        ProfilerMarker& operator=   (ProfilerMarker const&  p_copy) noexcept = delete;

        ProfilerMarker& operator=   (ProfilerMarker&&       p_move) noexcept = delete;

    private:

    // ============================== [Private Local Properties] ============================== //
        
        bool    m_onStack;

}; // !class ProfilerMarker

#include "ProfilerMarker.inl"

#endif // !__PROFILER_MARKER_HPP__