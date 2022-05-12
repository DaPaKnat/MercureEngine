#include "PCH.hpp"

#include "PhysicsEngine/PhysXAllocatorCallback.hpp"

// ==============================[Public Virtual Local Methods]============================== //

/**
 * Allocates size bytes of memory, which must be 16-byte aligned.
 * This method should never return NULL. If you run out of memory, then you should terminate the app or take some other appropriate action.
 * Threading: This function should be thread safe as it can be called in the context of the user thread and physics processing thread(s).
 *
 * @param p_size        The number of bytes to allocate.
 * @param p_typeName    The name of databyte that is being allocated.
 * @param p_filename    The source file which allocated the memory.
 * @param p_line        The source line which allocated the memory.
 *
 * @return The allocated block of memory.
 */
void*   PhysXAllocatorCallback::allocate    (size_t        p_size, 
                                             char const*   p_typeName, 
                                             char const*   p_filename, 
                                             int           p_line)
{
#if defined (_WIN32) || defined(_WIN64) 

    return _aligned_malloc(p_size, 16);

#endif
}

/**
 * Frees memory previously allocated by allocate().
 * Threading: This function should be thread safe as it can be called in the context of the user thread and physics processing thread(s).
 *
 * @param p_ptr The memory to free.
 */
void    PhysXAllocatorCallback::deallocate  (void*          p_ptr)
{
#if defined (_WIN32) || defined(_WIN64) 

    _aligned_free(p_ptr);

#endif
}