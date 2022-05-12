#ifndef __BODY_INSTANCE_INL__
#define __BODY_INSTANCE_INL__

// ==============================[Public Local Methods]============================== //

/**
 * Get the angular damping of this body.
 *
 * @return The angular damping of this.
 */
FORCEINLINE float                               BodyInstance::GetAngularDamping         ()  const
{
    return m_angularDamping;
}

/**
 * Gets the current type of collision enabled.
 *
 * @return The current type of collision enabled for this body.
 */
FORCEINLINE ECollisionEnabled                   BodyInstance::GetCollisionEnabled       (bool const                 p_checkOwner /* = true*/ )  const
{
    return p_checkOwner ? GetCollisionEnabledCheckingOwner() : m_collisionEnabled;
}

/**
 * Get the linear damping of this body.
 *
 * @return The linear damping of this.
 */
FORCEINLINE float                               BodyInstance::GetLinearDamping          ()  const
{
    return m_linearDamping;
}

/**
 * Gets the mass override (In Kg).
 *
 * @retrun The mass override of this body.
 */
FORCEINLINE float                               BodyInstance::GetMassOverride           ()  const
{
    return  m_massInKgOverride;
}

/**
 * Get the mass scale of this body.
 *
 * @return the mass scale of this.
 */
FORCEINLINE float                               BodyInstance::GetMassScale              ()  const
{
    return m_massScale;
}

/**
 * Gets the movement channel of this body.
 *
 * @return The movement channel if this body.
 */
FORCEINLINE ECollisionChannel                   BodyInstance::GetObjectType             ()  const
{
    return m_objectType;
}

/** 
 * Get the collision response of this body to a particular channel 
 *
 * @param p_channel The channel whose response type we want to know.
 *
 * @return The collision response associated to the channel given.
 */
FORCEINLINE ECollisionResponse                  BodyInstance::GetResponseToChannel      (ECollisionChannel const    p_channel)  const
{
    return m_collisionResponses.GetResponse(p_channel);
}

/** 
 * Get Collision ResponseToChannels container of this body.
 *
 * @return The collision response container of this body.
 */
FORCEINLINE CollisionResponseContainer const&   BodyInstance::GetResponseToChannels     ()  const
{
    return m_collisionResponses;
}

/**
 * Is this body affected by the gravity. Return Always false if this body is not simulated.
 *
 * @return True if the body is affected by the gravity.
 */
FORCEINLINE bool                                BodyInstance::IsGravityEnabled          ()  const
{
    return m_enableGravity;
}

/**
 * Tells if this body is simulating physics.
 *
 * @return True if this body is simulating, otherwise false (If its kinematic, return false).
 */
FORCEINLINE bool                                BodyInstance::IsSimulatingPhysics       ()  const
{
    return ShouldSimulatingPhysics() && IsValidBodyInstance();
}

/**
 * Checks if this body is valid>
 * 
 * @return True if this body is valid, otherwise false.
 */
FORCEINLINE bool                                BodyInstance::IsValidBodyInstance       ()  const
{
    return m_actorHandle.IsValid();
}

/**
 * Modify the mass scale of this body.
 *
 * @param p_newMassScale    The new mass scale.
 */
FORCEINLINE void                                BodyInstance::SetMassScale              (float const                p_newMassScale /* = 1.0f */)
{
    m_massScale = 1.0f;
    UpdateMassProperties();
}

/**
 * Set the movement channel of this body.
 *
 * @param p_channel The new object type of this body. 
 */
FORCEINLINE void                                BodyInstance::SetObjectType             (ECollisionChannel const    p_channel)
{
    m_objectType = p_channel;
    UpdatePhysicsFilterData();
}

/**
 * Should simulate physics on this body.
 *
 * @return True if we should simulate physics on this, otherwise false.
 */
FORCEINLINE bool                                BodyInstance::ShouldSimulatingPhysics   ()  const
{
    return m_simulatePhysics;
}

#endif // !__BODY_INSTANCE_INL__