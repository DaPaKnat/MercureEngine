#ifndef __PHYSX_ALLOCATOR_CALLBACK_HPP__
#define __PHYSX_ALLOCATOR_CALLBACK_HPP__

#include "PxPhysicsAPI.h"

/**
 * Class used in order to manage the Physx Memory Allocation.
 */
class PhysXAllocatorCallback: public physx::PxAllocatorCallback
{
    public:

    // ==============================[Public Constructor + Destructor]============================== //

        PhysXAllocatorCallback  ()                                      = default;

        PhysXAllocatorCallback  (PhysXAllocatorCallback const&  p_copy) = default;
        
        PhysXAllocatorCallback  (PhysXAllocatorCallback&&  p_copy)      = default;
        
        ~PhysXAllocatorCallback ()                                      = default;
        
    // ==============================[Public Virtual Local Methods]============================== //

        virtual void*   allocate    (size_t         p_size, 
                                     char const*    p_typeName, 
                                     char const*    p_filename, 
                                     int            p_line)     override;

	    virtual void    deallocate  (void*          p_ptr)      override;

}; // !class PhysXAllocatorCallback

#endif // !__PHYSX_ALLOCATOR_CALLBACK_HPP__