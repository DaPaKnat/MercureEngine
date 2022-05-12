#include "PCH.hpp"

#include "PhysicsEngine/PhysicsMaterialHandle.hpp"

// ==============================[Public Constructor + Destructor]============================== //

/**
 * Default Constructor.
 */
PhysicsMaterialHandle::PhysicsMaterialHandle    ()
:   m_material  {nullptr}
{}

/**
 * Constructor where we initialize the physx::PxMaterial with a pointer.
 */
PhysicsMaterialHandle::PhysicsMaterialHandle    (physx::PxMaterial*             p_material)
:   m_material  {p_material}
{}

/**
 * Copy Constructor.
 */
PhysicsMaterialHandle::PhysicsMaterialHandle    (PhysicsMaterialHandle const&   p_copy)
:   m_material  {p_copy.m_material}
{}

/**
 * Move Constructor.
 */
PhysicsMaterialHandle::PhysicsMaterialHandle    (PhysicsMaterialHandle&&        p_move)
:   m_material  {std::move(p_move.m_material)}
{}

// ==============================[Public Local Methods]============================== //

/**
 * Gets the dynamic frictions of this material.
 *
 * @return Th e dynamic friction of this material.
 */
float   PhysicsMaterialHandle::GetDynamicFriction           ()  const
{
    if (IsValid())
    {
        return m_material->getDynamicFriction();
    }

    return 0.0f;
}

/**
 * Gets the restitution of this material.
 *
 * @return The restitution of this material.
 */
float   PhysicsMaterialHandle::GetRestitution               ()  const
{
    if (IsValid())
    {
        return m_material->getRestitution();
    }

    return 0.0f;
}

/**
 * Gets the static frictions of this material.
 *
 * @return The static friction of this material.
 */
float   PhysicsMaterialHandle::GetStaticFriction            ()  const
{
    if (IsValid())
    {
        return m_material->getStaticFriction();
    }

    return 0.0f;
}

/**
 * Release the physx material.
 */
void    PhysicsMaterialHandle::Release                      ()
{
    if (IsValid())
    {
        m_material->userData    = nullptr;
        m_material->release();
        m_material              = nullptr;
    }
}

/**
 * Sets the dynamic friction of the material.
 * If the new dynamic friction is greater than static friction, the effective value of static friction will be increase to match.
 *
 * @param p_newDynamicFriction  The new dynamic friction to set.Shoud be in [0.0f, FLOAT_MAX[.
 */
void    PhysicsMaterialHandle::SetDynamicFriction           (float const                p_newDynamicFriction)
{
    if (IsValid())
    {
        m_material->setDynamicFriction(p_newDynamicFriction);
    }
}

/**
 * Sets the Friction Combine Mode of this material.
 *
 * @param p_newFrictionCombineMode  The new friction combine mode for this material.
 */
void    PhysicsMaterialHandle::SetFrictionCombineMode       (EPhysicsCombineMode const  p_newFrictionCombineMode)
{
    if (IsValid())
    {
        m_material->setFrictionCombineMode(static_cast<physx::PxCombineMode::Enum>(p_newFrictionCombineMode));
    }
}

/**
 * Sets the Resitution Combine Mode of this material.
 *
 * @param p_newFrictionCombineMode  The new resitution combine mode for this material.
 */
void    PhysicsMaterialHandle::SetRestitutionCombineMode    (EPhysicsCombineMode const  p_newRestitutionCombineMode)
{
    if (IsValid())
    {
        m_material->setRestitutionCombineMode(static_cast<physx::PxCombineMode::Enum>(p_newRestitutionCombineMode));
    }
}

/**
 * Sets the material's coefficient of restitution.
 * If coefficient = 0.0f : The object will not bounce.
 * If coefficient = 1.0f : The object will bounce at 100%.
 *
 * @param p_newRestitution  The new coefficient of restitution. Range [0.0f, 1.0f]
 */
void    PhysicsMaterialHandle::SetRestitution               (float const                p_newRestitution)
{
    if (IsValid())
    {
        m_material->setRestitution(p_newRestitution);
    }
}

/**
 * Sets the static friction of the material.
 *
 * @param p_newStaticFriction  The new static friction to set.Shoud be in [0.0f, FLOAT_MAX[.
 */
void    PhysicsMaterialHandle::SetStaticFrinction           (float const                p_newStaticFriction)
{
    if (IsValid())
    {
        m_material->setStaticFriction(p_newStaticFriction);
    }
}

/**
 * Set physx::PxMaterial->userData of this material.
 *
 * @param p_userData    The user data to set with.
 */
void    PhysicsMaterialHandle::SetUserData                  (void*                      p_useData)
{
    if (IsValid())
    {
        m_material->userData = p_useData;
    }
}

// ==============================[Public Local Operator]============================== //

/**
 * Checks if both PhysicsMaterialHandle correspond to the same physx::PxMaterial.
 *
 * @param p_other   The other PhysicsMaterialHandle to compare with.
 *
 * @return          True if they correspond to the same physx::PxMaterial.
 */
bool                    PhysicsMaterialHandle::operator==   (PhysicsMaterialHandle const&   p_other)    const
{
    return  m_material == p_other.m_material;
}

/**
 * Checks if both PhysicsMaterialHandle not correspond to the same physx::PxMaterial.
 *
 * @param p_other   The other PhysicsMaterialHandle to compare with.
 *
 * @return          True if they correspond to the same physx::PxMaterial.
 */
bool                    PhysicsMaterialHandle::operator!=   (PhysicsMaterialHandle const&   p_other)    const
{
    return  m_material != p_other.m_material;
}

/**
 * Copy another PhysicsMaterialHandle into this one.
 *
 * @param p_other   The other PhysicsMaterialHandle to copy.
 *
 * @return          The reference to PhysicsMaterialHandle after copy.
 */
PhysicsMaterialHandle&  PhysicsMaterialHandle::operator=    (PhysicsMaterialHandle const&   p_copy)
{
    m_material = p_copy.m_material;

    return *this;
}

/**
 * Move another PhysicsMaterialHandle into this one.
 *
 * @param p_other   The other PhysicsMaterialHandle to move.
 *
 * @return          The reference to PhysicsMaterialHandle after move.
 */
PhysicsMaterialHandle&  PhysicsMaterialHandle::operator=    (PhysicsMaterialHandle&&        p_move)
{
    m_material = std::move(p_move.m_material);

    return *this;
}