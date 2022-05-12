#ifndef __PHYSICS_SHAPE_HANDLE_INL__
#define __PHYSICS_SHAPE_HANDLE_INL__

// ==============================[Public Local Methods]============================== //

/**
 * Checks if the PhysicsShapeHandle is valid (If m_shape != nullptr).
 *
 * @return True if this PhysicsShapeHandle  is valid. 
 */
FORCEINLINE bool    PhysicsShapeHandle::IsValid ()  const
{
    return m_shape != nullptr;
}

#endif // !__PHYSICS_SHAPE_HANDLE_INL__