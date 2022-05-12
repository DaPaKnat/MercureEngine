#include "PCH.hpp"

#include "PhysicsEngine/PhysicsShapeHandle.hpp"
#include "PhysicsEngine/BodyInstance.hpp"
#include "PhysicsEngine/PhysicsShapeHandle.hpp"


// ==============================[Public Constructor + Destructor]============================== //

/**
 * Default Constructor.
 */
PhysicsShapeHandle::PhysicsShapeHandle  ()
:   m_shape {nullptr}
{}

/**
 * Constructor where we initialize the physx::PxShape with a pointer.
 */
PhysicsShapeHandle::PhysicsShapeHandle  (physx::PxShape*            p_shape)
:   m_shape {p_shape}
{
    if (p_shape != nullptr)
        p_shape->userData = this;
}

/**
 * Copy Constructor.
 */
PhysicsShapeHandle::PhysicsShapeHandle  (PhysicsShapeHandle const&  p_copy)
:   m_shape {p_copy.m_shape}
{}

/**
 * Move Constructor.
 */
PhysicsShapeHandle::PhysicsShapeHandle  (PhysicsShapeHandle&&       p_move)
:   m_shape {std::move(p_move.m_shape)}
{}

// ==============================[Public Local Methods]============================== //

/**
 * Gets the transform of this shape in the physx::actor space.
 *
 * @return The transform of this shape in physx::actor space.
 */
Transform           PhysicsShapeHandle::GetLocalTransform       ()  const
{
    if (IsValid())
    {
        return PhysXTypeConverter::TTransform(m_shape->getLocalPose());
    }

    return Transform::Identity;
}

CollisionFilterData PhysicsShapeHandle::GetQueryFilterData      ()  const
{
    if (IsValid())
    {
        return PhysXTypeConverter::TFilterData(m_shape->getQueryFilterData());
    }

    return CollisionFilterData();
}

ECollisionShapeType PhysicsShapeHandle::GetShapeType            ()  const
{
    if (IsValid())
    {
        return PhysXTypeConverter::TShapeType(m_shape->getGeometryType());
    }

    return ECollisionShapeType::None;
}

CollisionFilterData PhysicsShapeHandle::GetSimulationFilter     ()  const
{
    if (IsValid())
    {
        return PhysXTypeConverter::TFilterData(m_shape->getSimulationFilterData());
    }

    return CollisionFilterData();
}

Transform           PhysicsShapeHandle::GetTransform            ()  const
{
    if (IsValid())
    {
        physx::PxRigidActor*    owningPxRigidActor  {m_shape->getActor()};
        BodyInstance*           bodyInstance        {static_cast<BodyInstance*>(owningPxRigidActor->userData)};

        if (bodyInstance != nullptr && bodyInstance->IsValidBodyInstance())
        {
            return GetLocalTransform() * bodyInstance->GetPhysicsActorHandle()->GetGlobalPose();
        }
    }

    return Transform::Identity;
}

/**
 *
 */
void*               PhysicsShapeHandle::GetUserData             ()  const
{
    if (IsValid())
    {
        return m_shape->userData;
    }

    return nullptr;
}

bool                PhysicsShapeHandle::IsQueryShape            ()  const
{
    if (IsValid() == true)
    {
        return m_shape->getFlags() & physx::PxShapeFlag::eSCENE_QUERY_SHAPE;
    }

    return false;
}

bool                PhysicsShapeHandle::IsShapeType             (ECollisionShapeType const  p_shapeType)
{
    if (IsValid())
    {
        return m_shape->getGeometryType() == PhysXTypeConverter::TShapeType(p_shapeType);
    }

    return false;
}

bool                PhysicsShapeHandle::IsSimulationShape       ()  const
{
    if (IsValid() == true)
    {
        return m_shape->getFlags() & physx::PxShapeFlag::eSIMULATION_SHAPE;
    }

    return false;
}

void                PhysicsShapeHandle::SetGeometry             (physx::PxGeometry const&   p_geometry)
{
    if (IsValid())
    {
        m_shape->setGeometry(p_geometry);
    }
}

void                PhysicsShapeHandle::SetIsQueryShape         (bool const                 p_isQueryShape)
{
    if (IsValid())
    {
        PhysicsInterface::ModifyShapeFlag<physx::PxShapeFlag::eSCENE_QUERY_SHAPE>(m_shape, p_isQueryShape);
    }
}

void                PhysicsShapeHandle::SetIsSimulationShape    (bool const                 p_isSimulationShape)
{
    if (IsValid())
    {
        PhysicsInterface::ModifyShapeFlag<physx::PxShapeFlag::eSIMULATION_SHAPE>(m_shape, p_isSimulationShape);
    }
}

void                PhysicsShapeHandle::SetLocalTransform       (Transform const&           p_newLocalTransform)
{
    if (IsValid())
    {
        m_shape->setLocalPose(PhysXTypeConverter::TTransform(p_newLocalTransform));
    }
}

void                PhysicsShapeHandle::SetQueryFilter          (CollisionFilterData const& p_filterData)
{
    if (IsValid())
    {
        m_shape->setQueryFilterData(PhysXTypeConverter::TFilterData(p_filterData));
    }
}

void                PhysicsShapeHandle::SetSimulationFilter     (CollisionFilterData const& p_filterData)
{
    if (IsValid())
    {
        m_shape->setSimulationFilterData(PhysXTypeConverter::TFilterData(p_filterData));
    }
}

void                PhysicsShapeHandle::SetUserData             (void*                      p_userData)
{
    if (IsValid())
    {
        m_shape->userData = p_userData;
    }
}

void                PhysicsShapeHandle::Release                 ()
{
    if (IsValid())
    {
        m_shape->release();
    }
}

// ==============================[Public Local Operator]============================== //

/**
 * Checks if both PhysicsShapeHandle correspond to the same physx::PxShape.
 *
 * @param p_other   The other PhysicsShapeHandle to compare with.
 *
 * @return          True if they correspond to the same physx::PxShape.
 */
bool                PhysicsShapeHandle::operator==  (PhysicsShapeHandle const&  p_other)    const
{
    return  m_shape == p_other.m_shape;
}

/**
 * Checks if both PhysicsShapeHandle not correspond to the same physx::PxShape.
 *
 * @param p_other   The other PhysicsShapeHandle to compare with.
 *
 * @return          True if they correspond to the same physx::PxShape.
 */
bool                PhysicsShapeHandle::operator!=  (PhysicsShapeHandle const&  p_other)    const
{
    return  m_shape != p_other.m_shape;
}

/**
 * Copy another PhysicsShapeHandle into this one.
 *
 * @param p_other   The other PhysicsShapeHandle to copy.
 *
 * @return          The reference to PhysicsShapeHandle after copy.
 */
PhysicsShapeHandle& PhysicsShapeHandle::operator=   (PhysicsShapeHandle const&  p_copy)
{
    m_shape = p_copy.m_shape;

    return *this;
}

/**
 * Move another PhysicsShapeHandle into this one.
 *
 * @param p_other   The other PhysicsShapeHandle to move.
 *
 * @return          The reference to PhysicsShapeHandle after move.
 */
PhysicsShapeHandle& PhysicsShapeHandle::operator=   (PhysicsShapeHandle&&       p_move)
{
    m_shape = std::move(p_move.m_shape);

    return *this;
}