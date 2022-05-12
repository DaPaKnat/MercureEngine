#include "PCH.hpp"

#include "Components/SphereComponent.hpp"

// ==============================[Public Constructor + Destructor]============================== //

/**
 * Default Constructor
 */
SphereComponent::SphereComponent    ()
:   Super  (),
    m_sphereRadius  {0.5f}
{}

// ==============================[Virtual Public Local Methods]============================== //

CollisionShape  SphereComponent::GetCollisionShape  ()  const
{
    return CollisionShape::MakeSphere(GetScaledSphereRadius());
}

EGeometryType   SphereComponent::GetGeometryType    ()  const
{
    return EGeometryType::Sphere;
}

/**
 * Checks if bound extent is 0.
 * For normal primitive, this is 0 but for ShapeComponent, this will has a valid information.
 * 
 * @return  True if the bound extent is zero, otherwise false.
 */
bool            SphereComponent::IsZeroExtent       ()  const
{
    return m_sphereRadius == 0.0f;
}

// ==============================[Public Local Methods]============================== //

/**
 * TODO: Note finished (We have the applied physics).
 *
 * Changes the sphere radius. This is the unscaled radius, before component scale is applied.
 *
 * @param p_newSphereRadius The new sphere radius.
 * @param p_updateOverlaps  TODO.
 */
void    SphereComponent::SetSphereRadius    (float const    p_newSphereRadius,
                                             bool const     p_updateOverlaps /* = true */)
{
    m_sphereRadius = p_newSphereRadius;

    // Update Physics //
}