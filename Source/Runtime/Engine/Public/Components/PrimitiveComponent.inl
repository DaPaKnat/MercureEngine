#ifndef __PRIMITIVE_COMPONENT_INL__
#define __PRIMITIVE_COMPONENT_INL__

// ==============================[Public Local Methods]============================== //

/**
 * Add an angular impulse to the rigid body of this component..
 *
 * @param p_impulseDeg      The magnitude and direction of impulse to apply. Direction is acis of rotation.
 * @param p_velocityChange  If true, the strengh is take as a change in angular velocity instead of an impulse (ie. mass will have no effect).
 */
FORCEINLINE void                                    PrimitiveComponent::AddAngularImpulseDeg            (Vector3 const& p_impulseDeg,
                                                                                                         bool const     p_velocityChange /* = false */)
{
    AddAngularImpulseRad(Vector3::DegToRad(p_impulseDeg), p_velocityChange);
}

/**
 *	Add a torque to the rigid body of this component.
 *
 *	@param p_torque		    Torque to apply. Direction is axis of rotation and magnitude is strength of torque.
 *  @param p_AccelChange    If true, p_torque is taken as a change in angular acceleration instead of a physical torque (i.e. mass will have no effect).
 */
FORCEINLINE void                                    PrimitiveComponent::AddTorqueDeg                    (Vector3 const& p_torqueDeg,
                                                                                                         bool           p_accelChange /* = false */)
{
    AddTorqueRad(Vector3::DegToRad(p_torqueDeg), p_accelChange);
}

/**
 * Clear the list of entities we ignore when moving.
 */
FORCEINLINE void                                    PrimitiveComponent::ClearMoveIgnoreEntities         ()
{
    m_moveIgnoreEntities.clear();
}

/**
 * Clear the list of components we ignore when moving.
 */
FORCEINLINE void                                    PrimitiveComponent::ClearMoveIgnoreComponents       ()
{
    m_moveIgnoreComponents.clear();
}

/**
 *	Get the angular velocity of a body of this component, in degrees per second.
 */
FORCEINLINE Vector3                                 PrimitiveComponent::GetAngularVelocityDeg           ()  const
{
    return Vector3::RadToDeg(GetAngularVelocityRad());
}

/**
 * Ask if the body should call overlap event when the component overlap an another component.
 *
 * @return True if It generates overlap event, otherwise false.
 */
FORCEINLINE bool                                    PrimitiveComponent::GetGenerateOverlapEvents        ()  const
{
    return m_generateOverlapEvents;
}

/**
 * Gets the maximum draw distance of this component used in culling.
 *
 * @return The maximum draw distance of this component.m 
 */
FORCEINLINE float                                   PrimitiveComponent::GetMaxDrawDistance              ()  const
{
    return m_maxDrawDistance;
}

/**
 * Gets the minimum draw distance of this component used in culling.
 *
 * @return The minimum draw distance of this component.m 
 */
FORCEINLINE float                                   PrimitiveComponent::GetMinDrawDistance              ()  const
{
    return m_minDrawDistance;
}

/**
 * Gets a reference to the list of actors we currently ignore when moving.
 *
 * @return The reference to the list of actors we currently ignore when moving.
 */
FORCEINLINE std::vector<Entity*> const&             PrimitiveComponent::GetMoveIgnoreEntities           ()  const
{
    return m_moveIgnoreEntities;
}

/**
 * Gets a reference to the list of components we currently ignore when moving.
 *
 * @return The reference to the list of components we currently ignore when moving.
 */
FORCEINLINE std::vector<PrimitiveComponent*> const& PrimitiveComponent::GetMoveIgnoreComponents         ()  const
{
    return m_moveIgnoreComponents;
}


/**
 * Sets the angular velocity of the body of this component.
 *
 * @param p_newAngularVelocityDeg   The new angular velocity to apply on the body (In degrees per seconds)
 */
FORCEINLINE void                                    PrimitiveComponent::SetAngularVelocityDeg           (Vector3 const& p_newAngularVelocityDeg,
                                                                                                         bool const     p_addToCurrent /* = false */)
{
    SetAngularVelocityRad(Vector3::DegToRad(p_newAngularVelocityDeg), p_addToCurrent);
}

/**
 * Sets the maximum angular velocity of the body of this component.
 *
 * @param p_newMaxAngularVelocityDeg    The new maximum angular velocity (In Degrees).
 * @param p_addToCurrent                Should we add the new max angular velocity to the current.
 */
FORCEINLINE void                                    PrimitiveComponent::SetMaxAngularVelocityDeg        (float const    p_newMaxAngularVelocityDeg,
                                                                                                         bool const      p_addToCurrent /* = false */)
{
    SetMaxAngularVelocityRad(Math::DegToRad(p_newMaxAngularVelocityDeg), p_addToCurrent);
}

/**
 * Sets the mimnum draw distance of this component used in culling distance.
 *
 * @param p_newMinDrawDistance  The new minimum draw distance.
 */
FORCEINLINE void                                    PrimitiveComponent::SetMinDrawDistance              (float const    p_newMinDrawDistance)
{
    m_minDrawDistance = p_newMinDrawDistance;
}


#endif // !__PRIMITIVE_COMPONENT_INL__