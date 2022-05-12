#ifndef __CAPSULE_COMPONENT_INL__
#define __CAPSULE_COMPONENT_INL__

// ==============================[Public Local Methods]============================== //

/**
 * Changes the half-height radius. This is the unscaled half-height, before component scale is applied.
 *
 * @param p_newHalfHeight   The new capsule radius.
 * @param p_updateOverlaps  TODO.
 */
FORCEINLINE void    CapsuleComponent::SetCapsuleHalfHeight                          (float const    p_newHalfHeight,
                                                                                     bool const     p_updateOverlap /* = false */)
{
    SetCapsuleSize(GetUnscaledCapsuleRadius(), p_newHalfHeight, p_updateOverlap);
}
/**
 * Changes the capsule radius. This is the unscaled radius, before component scale is applied.
 *
 * @param p_newSphereRadius The new capsule radius.
 * @param p_updateOverlaps  TODO.
 */
FORCEINLINE void    CapsuleComponent::SetCapsuleRadius                              (float const    p_newRadius,
                                                                                     bool const     p_updateOverlap /* = false */)
{
    SetCapsuleSize(p_newRadius, GetUnscaledCapsuleHalfHeight(), p_updateOverlap);
}

/**
 * Gets the Half-Height of the capsule WITH the component scale applied.
 * (The component scale applied on the capsule Half-Height is the minimum axis)
 *
 * @return  The scaled Half-Height of the capsule.
 */
FORCEINLINE float   CapsuleComponent::GetScaledCapsuleHalfHeight                    ()  const
{
    return m_capsuleHalfHeight * GetWorldScale().m_y;
}

/**
 * Gets the Half-Height minus radius (to exclude hemisphere) of the capsule WITH the component scale applied.
 * It's corresponding to the distance from the center to the center of the hemisphere ( = The cylinder portion).
 * (The component scale applied is the minimum axis of this one)
 *
 * @return  The scaled Half-Height without hemisphere of the capsule.
 */
FORCEINLINE float   CapsuleComponent::GetScaledCapsuleHalfHeightWithoutHemisphere   ()  const
{                                     
    return GetScaledCapsuleHalfHeight() - GetScaledCapsuleRadius();;
}

/**
 * Gets the Radius of the capsule WITH the component scale applied.
 * (The component scale applied on the capsule Radius is the minimum axis)
 *
 * @return  The scaled Radius of the capsule.
 */
FORCEINLINE float   CapsuleComponent::GetScaledCapsuleRadius                        ()  const
{
    Vector3 const& scale    {GetWorldTransform().m_scale};
    return m_capsuleRadius * ((scale.m_x < scale.m_z) ? scale.m_x : scale.m_z);
}

/**
 * Gets the Radius and Half-Height of the capsule WITH the component scale applied.
 * (The component scale applied on the capsule Radius and Half-Height is the minimum axis)
 *
 * @param p_outScaledRadius      The scaled radius of the capsule.
 * @param p_outScaledHalfHeight  The scaled half-height of the capsule
 */
FORCEINLINE void    CapsuleComponent::GetScaledCapsuleSize                          (float&         p_outScaledRadius,
                                                                                     float&         p_outScaledHalfHeight)  const
{
    float const scale   {GetShapeScale()};
    
    p_outScaledRadius       = m_capsuleRadius * scale;
    p_outScaledHalfHeight   = m_capsuleHalfHeight* scale;
}
/**
 * Gets the Radius and Half-Height without hemisphere of the capsule WITH the component scale applied.
 * (The component scale applied on the capsule Radius and Half-Height is the minimum axis)
 *
 * @param p_outScaledRadius                       The scaled radius of the capsule.
 * @param p_outScaledHalfHeightWithoutHemisphere  The scaled half-height without hemisphere of the capsule
 */
FORCEINLINE void    CapsuleComponent::GetScaledCapsuleSizeWithoutHemisphere         (float&         p_outScaledRadius,
                                                                                     float&         p_outScaledHalfHeightWithoutHemisphere)  const
{
    float const scale   {GetShapeScale()};
    
    p_outScaledRadius                       = m_capsuleRadius * scale;
    p_outScaledHalfHeightWithoutHemisphere  = (m_capsuleHalfHeight - m_capsuleRadius) * scale;
}

/**
 * Get the scale used by this shape. this is a uniform scale that is the minimum of any non-unifor scaling
 *
 * @return The scale used by this shape.
 */
FORCEINLINE float   CapsuleComponent::GetShapeScale                                 ()  const
{
    return GetWorldTransform().m_scale.GetAbsMin();
}

/**
 * Gets the Half-Height of the capsule WITHOUT the component scale applied.
 * (The component scale applied on the capsule Half-Height is the minimum axis)
 *
 * @return  The scaled Half-Height of the capsule.
 */
FORCEINLINE float   CapsuleComponent::GetUnscaledCapsuleHalfHeight                  ()  const
{
    return m_capsuleHalfHeight;
}

/**
 * Gets the Half-Height minus radius (to exclude hemisphere) of the capsule WITHOUT the component scale applied.
 * It's corresponding to the distance from the center to the center of the hemisphere ( = The cylinder portion).
 * (The component scale applied is the minimum axis of this one)
 *
 * @return  The Half-Height without hemisphere of the capsule.
 */
FORCEINLINE float   CapsuleComponent::GetUnscaledCapsuleHalfHeightWithoutHemisphere ()  const
{
    return (m_capsuleHalfHeight - m_capsuleRadius);
}

/**
 * Gets the Radius of the capsule WITHOUT the component scale applied.
 * (The component scale applied on the capsule Radius is the minimum axis)
 *
 * @return  The scaled Radius of the capsule.
 */
FORCEINLINE float   CapsuleComponent::GetUnscaledCapsuleRadius                      ()  const
{
    return m_capsuleRadius;
}

/**
 * Gets the Radius and Half-Height of the capsule WITOUT the component scale applied.
 * (The component scale applied on the capsule Radius and Half-Height is the minimum axis)
 *
 * @param p_outScaledRadius      The radius of the capsule.
 * @param p_outScaledHalfHeight  The half-height of the capsule
 */
FORCEINLINE void    CapsuleComponent::GetUnscaledCapsuleSize                        (float&         p_outUnscaledRadius,
                                                                                     float&         p_outUnscaledHalfHeight)  const
{
    p_outUnscaledRadius     = m_capsuleRadius;
    p_outUnscaledHalfHeight = m_capsuleHalfHeight;
}
/**
 * Gets the Radius and Half-Height without hemisphere of the capsule WITHOUT the component scale applied.
 * (The component scale applied on the capsule Radius and Half-Height is the minimum axis)
 *
 * @param p_outScaledRadius                       The radius of the capsule.
 * @param p_outScaledHalfHeightWithoutHemisphere  The half-height without hemisphere of the capsule
 */
FORCEINLINE void    CapsuleComponent::GetUnscaledCapsuleSizeWithoutHemisphere       (float&         p_outUnscaledRadius,
                                                                                     float&         p_outUnscaledHalfHeightWithoutHemisphere)  const
{
    p_outUnscaledRadius                         = m_capsuleRadius;
    p_outUnscaledHalfHeightWithoutHemisphere    = (m_capsuleHalfHeight - m_capsuleRadius);
}

/**
 * Sets the sphere radius and the half-height without triggering physics update.
 *
 * @param p_sphereRadius    The new capsule radius.
 * @param p_sphereRadius    The new capsule half-height.
 */
FORCEINLINE void    CapsuleComponent::InitCapsuleSize                               (float const    p_radius,
                                                                                     float const    p_halfHeight)
{
    m_capsuleHalfHeight = Math::Max3(0.0f, p_radius, p_halfHeight);
    m_capsuleRadius     = Math::Max(0.0f, p_radius);
}

#endif // !__CAPSULE_COMPONENT_INL__
