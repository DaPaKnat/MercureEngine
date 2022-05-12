#ifndef __ENGINE_TYPE_ENUMS_INL
#define __ENGINE_TYPE_ENUMS_INL

// ==============================[Globale Methods]============================== //

/**
 * Checks if Physics collision is enabled in the ECollisionEnabled parameter given.
 *
 * @param p_collisionEnabled    The parameter to check.
 *
 * @return True  p_collisionEnabled == ECollisionEnabled::PhysicsOnly || p_collisionEnabled == ECollisionEnabled::QueryAndPhysics.
 * @return False p_collisionEnabled == ECollisionEnabled::NoCollision || p_collisionEnabled == ECollisionEnabled::QueryOnly.
 */
FORCEINLINE bool    CollisionEnabledHasPhysics  (ECollisionEnabled const    p_collisionEnabled)
{
    return  p_collisionEnabled == ECollisionEnabled::PhysicsOnly ||
            p_collisionEnabled == ECollisionEnabled::QueryAndPhysics;
}

/**
 * Checks if Query collision is enabled in the ECollisionEnabled parameter given.
 *
 * @param p_collisionEnabled    The parameter to check.
 *
 * @return True  p_collisionEnabled == ECollisionEnabled::QueryOnly|| p_collisionEnabled == ECollisionEnabled::QueryAndPhysics.
 * @return False p_collisionEnabled == ECollisionEnabled::NoCollision || p_collisionEnabled == ECollisionEnabled::PhysicsOnly.
 */
FORCEINLINE bool    CollisionEnabledHasQuery    (ECollisionEnabled const    p_collisionEnabled)
{
    return  p_collisionEnabled == ECollisionEnabled::QueryOnly ||
            p_collisionEnabled == ECollisionEnabled::QueryAndPhysics;
}

#endif // !__ENGINE_TYPE_ENUMS_INL
