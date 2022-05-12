#ifndef __COLLISION_SHAPE_INL__
#define __COLLISION_SHAPE_INL__

// ==============================[Public Local Methods]============================== //

/**
 * Gets the Box Half-Extents.
 *
 * @return The Box Half-Extents.
 */
FORCEINLINE Vector3 CollisionShape::GetBoxHalfExtent                        ()  const
{
    return Box.m_halfExtent;
}

/**
 * Gets distance from center of capsule to center of sphere ends.
 *
 * @return The distance from center of capsule to center of sphere ends.
 */
FORCEINLINE float   CollisionShape::GetCapsuleHalfHeightWithoutHemisphere   ()  const
{
    return Math::Max(Capsule.m_halfHeight - Capsule.m_radius, MIN_FLOAT);
}

/**
 * Gets the Capsule Half-height.
 *
 * @return The Capsule Half-height.
 */
FORCEINLINE float   CollisionShape::GetCapsuleHalfHeight                    ()  const
{
    return Capsule.m_halfHeight;
}

/**
 * Gets the Capsule Radius.
 *
 * @return The Capsule Radius.
 */
FORCEINLINE float   CollisionShape::GetCapsuleRadius                        ()  const
{
    return Capsule.m_radius;
}

/**
 * Gets the Sphere Radius.
 *
 * @return The sphere Radius.
 */
FORCEINLINE float   CollisionShape::GetSphereRadius                         ()  const
{
    return Sphere.m_radius;
}

/** 
 * Is the shape currently a Line 
 *
 * @return True if the shape is Line, otherwise false.
 */
FORCEINLINE bool    CollisionShape::IsBox                                   ()  const
{
    return m_shapeType == ECollisionShapeType::Box;
}

/** 
 * Is the shape currently a Capsule 
 *
 * @return True if the shape is Capsule, otherwise false.
 */
FORCEINLINE bool    CollisionShape::IsCapsule                               ()  const
{
    return m_shapeType == ECollisionShapeType::Capsule;
}

/** 
 * Is the shape currently a Sphere 
 *
 * @return True if the shape is Sphere , otherwise false.
 */
FORCEINLINE bool    CollisionShape::IsSphere                                ()  const
{
    return m_shapeType == ECollisionShapeType::Sphere;
}

/**
 * Set the shape as a Box Shape.
 *
 * @param p_halfExtent  The Half-Extent of the Box Shape.
 */
FORCEINLINE void    CollisionShape::SetBox                                  (Vector3 const& p_halfExtent)
{
    m_shapeType         = ECollisionShapeType::Box;
    Box.m_halfExtent    = p_halfExtent;
}

/**
 * Set the shape as a Box Shape.
 *
 * @param p_halfExtentX  The X Half-Extent of the Box Shape.
 * @param p_halfExtentY  The Y Half-Extent of the Box Shape.
 * @param p_halfExtentZ  The Z Half-Extent of the Box Shape.
 */
FORCEINLINE void    CollisionShape::SetBox                                  (float const    p_halfExtentX,
                                                                             float const    p_halfExtentY,
                                                                             float const    p_halfExtentZ)
{
    m_shapeType         = ECollisionShapeType::Box;
    Box.m_halfExtent.Set(p_halfExtentX, p_halfExtentY, p_halfExtentZ);
}
/**
 * Set the shape as a Capsule Shape.
 *
 * @param p_extent  
 */
FORCEINLINE void    CollisionShape::SetCapsule                              (Vector3 const& p_extent)
{
    m_shapeType             = ECollisionShapeType::Capsule;
    Capsule.m_halfHeight    = p_extent.m_y;
    Capsule.m_radius        = Math::Max(p_extent.m_x, p_extent.m_z);
}

/**
 * Set the shape as a Capsule Shape.
 *
 * @param p_radius      The radius of the capsule.
 * @param p_halfHeight  The Half-Height of the capsule.
 */
FORCEINLINE void    CollisionShape::SetCapsule                              (float const    p_radius,
                                                                             float const    p_halfHeight)
{
    m_shapeType             = ECollisionShapeType::Capsule;
    Capsule.m_halfHeight    = p_halfHeight;
    Capsule.m_radius        = p_radius;
}

/**
 * Set the shape as a Capsule Sphere.
 *
 * @param p_radius      The radius of the sphere.
 */
FORCEINLINE void    CollisionShape::SetSphere                               (float const    p_radius)
{
    m_shapeType     = ECollisionShapeType::Sphere;
    Sphere.m_radius = p_radius;
}

#endif // !__COLLISION_SHAPE_INL__