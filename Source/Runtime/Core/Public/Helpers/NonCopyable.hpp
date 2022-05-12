#ifndef __NON_COPYABLE_HPP__
#define __NON_COPYABLE_HPP__

/**
 * This is the base class for a non copyable object, meaning it cannot be copied or moved.
 */
class ENGINE_API BASE NonCopyable
{
    public:

    // ============================== [Public Constructors] ============================== //

        /** Deleted copy constructor. */
        NonCopyable(NonCopyable const& p_other) = delete;

    // ============================== [Public Local Operators] ============================== //

        /** Deleted copy operator. */
        NonCopyable& operator=(NonCopyable const& p_other) = delete;

    protected:

    // ============================== [Protected Constructor and Destructor] ============================== //

        /** Default constructor. */
        NonCopyable             ()  = default;

        /** Default destructor. */
        virtual ~NonCopyable    ()  = default;

};  // !class NonCopyable

#endif // !__NON_COPYABLE_HPP__