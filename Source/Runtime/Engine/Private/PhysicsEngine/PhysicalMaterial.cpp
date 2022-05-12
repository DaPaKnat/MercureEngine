#include "PCH.hpp"

#include "PhysicsEngine/PhysicalMaterial.hpp"

// ==============================[Public Constructor + Destructor]==================== //

/**
 * Default Constructor.
 */
PhysicalMaterial::PhysicalMaterial  ()
:   m_dynamicFriction                       {0.6f},
    m_isOverridingFrictionCombineMode       {false},    
    m_isOverridingRestitutionCombineMode    {false},
    m_materialHandle                        {nullptr},
    m_overrideFrictionCombineMode           {EPhysicsCombineMode::Average},
    m_overrideRestitutionCombineMode        {EPhysicsCombineMode::Average},
    m_restitution                           {0.3f},
    m_staticFriction                        {0.6f}
{}

// ==============================[Public Local Methods]==================== //

PhysicsMaterialHandle&  PhysicalMaterial::GetPhysicsMaterialHandle          ()
{
    return m_materialHandle;
}

/**
 * Sets the dynamic friction of the material.
 * If the new dynamic friction is greater than static friction, the effective value of static friction will be increase to match.
 *
 * @param p_newDynamicFriction  The new dynamic friction to set.Shoud be in [0.0f, FLOAT_MAX[.
 */
void                    PhysicalMaterial::SetDynamicFriction                (float const                p_newDynamicFriction)
{
    if (m_materialHandle.IsValid())
    {
        m_materialHandle.SetDynamicFriction(p_newDynamicFriction);
        m_dynamicFriction = m_materialHandle.GetDynamicFriction();
    }
}

/**
 * Sets the override friction combine mode.
 *
 * @param p_newOverrideFrictionCombineMode  The new override friction combine mode.
 * @param p_overrideFrictionCombineMode     Should we override friction combine move after modification.
 */
void                    PhysicalMaterial::SetOverrideFrictionCombineMode    (EPhysicsCombineMode const  p_newOverrideFrictionCombineMode,
                                                                             bool const                 p_overrideFrictionCombineMode /* = true */)
{
    if (m_materialHandle.IsValid())
    {
        m_overrideFrictionCombineMode = p_newOverrideFrictionCombineMode;

        if (p_overrideFrictionCombineMode == true)
        {
            m_isOverridingFrictionCombineMode= true;
            m_materialHandle.SetFrictionCombineMode(m_overrideFrictionCombineMode);
        }
    }
}

/**
 * Sets the override restitution combine mode.
 *
 * @param p_newOverrideFrictionCombineMode  The new override restitution combine mode.
 * @param p_overrideFrictionCombineMode     Should we override restitution combine move after modification.
 */
void                    PhysicalMaterial::SetOverrideRestitutionCombineMode (EPhysicsCombineMode const p_newOverrideRestitutionCombineMode,
                                                                             bool const                 p_overrideResitutionCombineMode /* = true */)
{
    if (m_materialHandle.IsValid())
    {
        m_overrideRestitutionCombineMode = p_newOverrideRestitutionCombineMode;

        if (p_overrideResitutionCombineMode == true)
        {
            m_isOverridingRestitutionCombineMode = true;
            m_materialHandle.SetFrictionCombineMode(m_overrideRestitutionCombineMode);
        }
    }
}

/**
 * Sets the material's coefficient of restitution.
 * If coefficient = 0.0f : The object will not bounce.
 * If coefficient = 1.0f : The object will bounce at 100%.
 *
 * @param p_newRestitution  The new coefficient of restitution. Range [0.0f, 1.0f].
 */
void                    PhysicalMaterial::SetRestitution                    (float const    p_newRestitution)
{
    if (m_materialHandle.IsValid())
    {
        m_materialHandle.SetRestitution(p_newRestitution);
        m_restitution = m_materialHandle.GetRestitution();
    }
}

/**
 * Sets the static friction of the material.
 *
 * @param p_newStaticFriction  The new static friction to set.Shoud be in [0.0f, FLOAT_MAX[.
 */
void                    PhysicalMaterial::SetStaticFriction                 (float const    p_newStaticFriction)
{
    if (m_materialHandle.IsValid())
    {
        m_materialHandle.SetStaticFrinction(p_newStaticFriction);
        m_staticFriction = m_materialHandle.GetStaticFriction();
    }
}