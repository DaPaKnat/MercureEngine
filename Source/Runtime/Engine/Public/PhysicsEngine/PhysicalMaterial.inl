#ifndef __PHYSICAL_MATERIAL_INL__
#define __PHYSICAL_MATERIAL_INL__

// ==============================[Public Local Methods]==================== //

/**
 * Gets the dynamic frictions of this material.
 *
 * @return The dynamic friction of this material.
 */
FORCEINLINE float               PhysicalMaterial::GetDynamicFriction                ()  const
{
    return m_dynamicFriction;
}

/**
 * Gets the override friction combine mode.
 *
 * @return The override friction combine mode.
 */
FORCEINLINE EPhysicsCombineMode PhysicalMaterial::GetOverrideFrictionCombineMode    ()  const
{
    return m_overrideFrictionCombineMode;
}

/**
 * Gets the override restitution combine mode.
 *
 * @return The override restitution combine mode.
 */
FORCEINLINE EPhysicsCombineMode PhysicalMaterial::GetOverrideRestitutionCombineMode ()  const
{
    return m_overrideRestitutionCombineMode;
}

/**
 * Gets the restitution of this material.
 *
 * @return The restitution of this material.
 */
FORCEINLINE float               PhysicalMaterial::GetRestitution                    ()  const
{
    return m_restitution;
}

/**
 * Gets the static frictions of this material.
 *
 * @return The static friction of this material.
 */
FORCEINLINE float               PhysicalMaterial::GetStaticFriction                 ()  const
{
    return m_staticFriction;
}

/**
 * Are we overriding friction combine mode.
 *
 * @return True if we are overrading friction combine mode, otherwise false.
 */
FORCEINLINE bool                PhysicalMaterial::IsOverrideFrictionCombineMode     ()  const
{
    return m_isOverridingFrictionCombineMode;
}

/**
 * Are we overriding restitution combine mode.
 *
 * @return True if we are overrading restitution combine mode, otherwise false.
 */
FORCEINLINE bool                PhysicalMaterial::IsOverrideRestitutionCombineMode  ()  const
{
    return m_isOverridingRestitutionCombineMode;
}

#endif // !__PHYSICAL_MATERIAL_INL__