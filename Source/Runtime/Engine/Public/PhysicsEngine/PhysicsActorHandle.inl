#ifndef __PHYSICS_ACTOR_HANDLE_INH__
#define __PHYSICS_ACTOR_HANDLE_INH__

// ==============================[Public Local Methods]============================== //

/**
 * Gets the max angular velocity permitted for this physx::PxActor (In Degrees).
 *
 * @return The max angular velocity of this physx::PxActor.
 */
FORCEINLINE float                   PhysicsActorHandle::GetMaxAngularVelocityDeg    ()  const
{
    return Math::RadToDeg(GetMaxAngularVelocityRad());
}

/**
 * Gets the PxRigidActor (m_actor) of this PhysicsActorHandle casted into a PxRigidBody.
 *
 * @return If the cast of m_actor is successed, get this one casted into a PxRigidBody, otherwise nullptr.
 */
FORCEINLINE physx::PxRigidBody*     PhysicsActorHandle::GetPxRigidBody              ()  const
{
    return m_actor ? m_actor->is<physx::PxRigidBody>() : nullptr;

}

/**
 * Gets the PxRigidActor (m_actor) of this PhysicsActorHandle casted into a PxRigidDynamic.
 *
 * @return If the cast of m_actor is successed, get this one casted into a PxRigidDynamic, otherwise nullptr.
 */
FORCEINLINE physx::PxRigidDynamic*  PhysicsActorHandle::GetPxRigidDynamic           ()  const
{
    return m_actor ? m_actor->is<physx::PxRigidDynamic>() : nullptr;
}

#endif // !__PHYSICS_ACTOR_HANDLE_INH__