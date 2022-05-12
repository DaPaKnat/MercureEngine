#include "PCH.hpp"

#include "Components/CapsuleComponent.hpp"

// ==============================[Virtual Public Local Methods]============================== //

/**
 * Checks if bound extent is 0.
 * For normal primitive, this is 0 but for ShapeComponent, this will has a valid information.
 * 
 * @return  True if the bound extent is zero, otherwise false.
 */
bool            CapsuleComponent::IsZeroExtent      ()  const
{
    return (m_capsuleHalfHeight == 0.0f) && (m_capsuleRadius == 0.0f);
}

CollisionShape  CapsuleComponent::GetCollisionShape ()  const
{
    return CollisionShape::MakeCapsule(GetScaledCapsuleRadius(), GetScaledCapsuleHalfHeight());
}

EGeometryType   CapsuleComponent::GetGeometryType   ()  const
{
    return EGeometryType::Capsule;
}

// ==============================[Public Constructor + Destructor]============================== //

/**
 * Default Constructor.
 */
CapsuleComponent::CapsuleComponent  ()
:   Super(),
    m_capsuleHalfHeight {1.0f},
    m_capsuleRadius     {0.5f}
{}

// ==============================[Public Local Methods]============================== //

/**
 * TODO: Note finished (We have the applied physics).
 *
 * Changes the capsule radius and half-height. This is the unscaled radius, before component scale is applied.
 *
 * @param p_newSphereRadius The new capsule radius.
 * @param p_newHalfHeight   The new capsule half-height.
 * @param p_updateOverlaps  TODO.
 */
void    CapsuleComponent::SetCapsuleSize        (float const    p_newRadius,
                                                 float const    p_newHalfHeight,
                                                 bool const     p_updateOverlap /* = false */)
{
    m_capsuleHalfHeight = Math::Max3(0.0f, p_newRadius, p_newHalfHeight);
    m_capsuleRadius     = Math::Max(0.0f, p_newRadius);

    // Update Physics //
}