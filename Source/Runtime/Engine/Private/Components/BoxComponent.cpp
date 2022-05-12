#include "PCH.hpp"

#include "Components/BoxComponent.hpp"

// ==============================[Public Constructor + Destructor]============================== //

/**
 * Default Constructor
 */
BoxComponent::BoxComponent  ()
:   Super(),
    m_boxExtent     {Vector3::One*0.5f}
{}

// ==============================[Virtual Public Local Methods]============================== //

CollisionShape  BoxComponent::GetCollisionShape ()  const
{
    return CollisionShape::MakeBox(GetScaledBoxExtent());
}

EGeometryType   BoxComponent::GetGeometryType   ()  const
{
    return EGeometryType::Box;
}

/**
 * Checks if bound extent is 0.
 * For normal primitive, this is 0 but for ShapeComponent, this will has a valid information.
 * 
 * @return  True if the bound extent is zero, otherwise false.
 */
bool            BoxComponent::IsZeroExtent      ()  const
{
    return m_boxExtent.IsZero();
}

// ==============================[Public Local Methods]============================== //

/**
 * TODO: Note finished (We have the applied physics).
 *
 * Changes the box extent. This is the unscaled box extent, before component scale is applied.
 *
 * @param p_newBoxExtent    The new boxExtent.
 * @param p_updateOverlaps  TODO.
 */
void    BoxComponent::SetBoxExtent  (Vector3 const& p_newBoxExtent,
                                     bool const     p_updateOverlap/* = true*/)
{
    m_boxExtent = p_newBoxExtent;
    // Update Physics //
}

/**
 * TODO: Note finished (We have the applied physics).
 *
 * Changes the box extent. This is the unscaled box extent, before component scale is applied.
 *
 * @param p_newBoxExtentXYZ The new value of each extent axis.
 * @param p_updateOverlaps  TODO.
 */
void    BoxComponent::SetBoxExtent  (float const    p_newBoxExtentXYZ,
                                     bool const     p_updateOverlap/* = true*/)
{
    m_boxExtent.Set(p_newBoxExtentXYZ);
    // Update Physics //
}

/**
 * TODO: Note finished (We have the applied physics).
 *
 * Changes the box extent. This is the unscaled box extent, before component scale is applied.
 *
 * @param p_newBoxExtentX   The new box extent on X Axis.
 * @param p_newBoxExtentY   The new box extent on Y Axis.
 * @param p_newBoxExtentZ   The new box extent on Z Axis.
 * @param p_updateOverlaps  TODO.
 */
void    BoxComponent::SetBoxExtent  (float const    p_boxExtentX,
                                     float const    p_boxExtentY,
                                     float const    p_boxExtentZ,
                                     bool const     p_updateOverlap/* = true*/)
{
    m_boxExtent.Set(p_boxExtentX, p_boxExtentY, p_boxExtentZ);
    // Update Physics //
}