#include "PCH.hpp"

#include "PhysicsEngine/CollisionShape.hpp"

// ==============================[Public Static Methods]============================== //

/**
 * Methods to make a Box Shape.
 *
 * @param p_halfExtent  The Half-Extent of the Box Shape.
 *
 * @return The Box shape.
 */
CollisionShape  CollisionShape::MakeBox     (Vector3 const& p_halfExtent)
{
    CollisionShape boxShape;
    boxShape.SetBox(p_halfExtent);
    return boxShape;
}

/**
 * Methods to make a Box Shape.
 *
 * @param p_halfExtentX  The X Half-Extent of the Box Shape.
 * @param p_halfExtentY  The Y Half-Extent of the Box Shape.
 * @param p_halfExtentZ  The Z Half-Extent of the Box Shape.
 *
 * @return The Box shape.
 */
CollisionShape  CollisionShape::MakeBox     (float const    p_halfExtentX,
                                             float const    p_halfExtentY,
                                             float const    p_halfExtentZ)
{
    CollisionShape boxShape;
    boxShape.SetBox(p_halfExtentX, p_halfExtentY, p_halfExtentZ);
    return boxShape;
}

/**
 * Methods to make a Capsule.
 *
 * @param p_extent
 *
 * @return The Capsule shape.
 */
CollisionShape  CollisionShape::MakeCapsule (Vector3 const& p_extent)
{
    CollisionShape capsuleShape;
    capsuleShape.SetCapsule(p_extent);
    return capsuleShape;
}

/**
 * Methods to make a Capsule.
 *
 * @param p_radius      The radius of the capsule.
 * @param p_halfHeight  The Half-Height of the capsule.
 *
 * @return The Capsule shape.
 */
CollisionShape  CollisionShape::MakeCapsule (float const    p_radius,
                                             float const    p_halfHeight)
{
    CollisionShape capsuleShape;
    capsuleShape.SetCapsule(p_radius, p_halfHeight);
    return capsuleShape;
}

/**
 * Methods to make a Sphere.
 *
 * @param p_radius      The radius of the sphere.
 *
 * @return The Sphere shape.
 */
CollisionShape  CollisionShape::MakeSphere  (float const    p_radius)
{
    CollisionShape sphereShape;
    sphereShape.SetSphere(p_radius);
    return sphereShape;
}

// ==============================[Public Constructor + Destructor]============================== //

/**
 * Default Constructor.
 */
 CollisionShape::CollisionShape ()
 :  m_shapeType         {ECollisionShapeType::None}
 {
    Box.m_halfExtent.Set(0.0f);
 }

 /**
 * Copy Constructor.
 */
 CollisionShape::CollisionShape (CollisionShape const&  p_copy)
 :  m_shapeType         {p_copy.m_shapeType}
 {
    Box.m_halfExtent    = p_copy.Box.m_halfExtent;
 }
 
 /**
 * Move Constructor.
 */
 CollisionShape::CollisionShape (CollisionShape&&       p_move)
 :  m_shapeType         {std::move(p_move.m_shapeType)}
 {
    Box.m_halfExtent    = std::move(p_move.Box.m_halfExtent);
 }

// ==============================[Public Local Methods]============================== //

/** ... */
bool    CollisionShape::IsNearlyZero    ()  const
{
    switch (m_shapeType)
    {
        case ECollisionShapeType::Box:
        {
            return Box.m_halfExtent.IsNearlyZero();
        }

        case ECollisionShapeType::Capsule:
        {
            return Math::IsNearlyZero(Capsule.m_radius);
        }

        case ECollisionShapeType::Sphere:
        {
            return Math::IsNearlyZero(Sphere.m_radius);
        }
    }

    return true;
}

/** ... */
Vector3 CollisionShape::GetExtent       ()  const
{
    switch (m_shapeType)
    {
        case ECollisionShapeType::Box:
        {
            return Box.m_halfExtent;
        }

        case ECollisionShapeType::Capsule:
        {
            return Vector3(Capsule.m_radius, Capsule.m_halfHeight, Capsule.m_radius);
        }

        case ECollisionShapeType::Sphere:
        {
            return Vector3(Sphere.m_radius);
        }
    }

    return Vector3::Zero;
}