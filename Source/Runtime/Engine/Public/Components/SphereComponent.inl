#ifndef __SPHERE_COMPONENT_INL__
#define __SPHERE_COMPONENT_INL__

// ==============================[Public Local Methods]============================== //

/**
 * Gets the radius of the sphere WITH the component scale applied.
 * (The component scale applied on the sphere radius is the minimum axis)
 *
 * @return  The scaled sphere radius.
 */
FORCEINLINE float   SphereComponent::GetScaledSphereRadius      ()  const
{
    return m_sphereRadius * GetShapeScale();
}

/**
 * Get the scale used by this shape. this is a uniform scale that is the minimum of any non-unifor scaling
 *
 * @return The scale used by this shape.
 */
FORCEINLINE float   SphereComponent::GetShapeScale              ()  const
{
    return GetWorldTransform().m_scale.GetAbsMin();
}

/**
 * Gets the radius of the sphere WITHOUT the component scale.
 * (The component scaled applied on the sphere radius is the minimum axis)
 *
 * @return  The sphere radius.
 */
FORCEINLINE float   SphereComponent::GetUnscaledSphereRadius    ()  const
{
    return m_sphereRadius;
}

/**
 * Sets the sphere radius withut triggering physics update.
 *
 * @param p_sphereRadius    The new sphere radius.
 */
FORCEINLINE void    SphereComponent::InitSphereRadius           (float const    p_sphereRadius)
{
    m_sphereRadius = p_sphereRadius;
}

#endif // !__SPHERE_COMPONENT_INL__
