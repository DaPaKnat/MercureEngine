#ifndef __UNIQUE_OBJECT_HPP__
#define __UNIQUE_OBJECT_HPP__

/**
 * This is the base class for an unique object, meaning it cannot be copied or moved.
 */
class ENGINE_API BASE UniqueObject
{
    public:

    // ============================== [Public Constructors] ============================== //

        /** Deleted copy constructor. */
        UniqueObject    (UniqueObject const&    p_copy) = delete;

        /** Deleted move constructor. */
        UniqueObject    (UniqueObject&&         p_move) = delete;

    // ============================== [Public Local Operators] ============================== //

        /** Deleted copy operator. */
        UniqueObject&   operator=   (UniqueObject const&    p_copy) = delete;

        /** Deleted move operator. */
        UniqueObject&   operator=   (UniqueObject&&         p_move) = delete;

    protected:

    // ============================== [Protected Constructor and Destructor] ============================== //

        /** Default constructor. */
        UniqueObject            ()  = default;

        /** Default destructor. */
        virtual ~UniqueObject   ()  = default;

};  // !class UniqueObject

#endif // !__UNIQUE_OBJECT_HPP__