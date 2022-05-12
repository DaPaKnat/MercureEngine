#include "PCH.hpp"

#include "PhysicsEngine/PhysicsActorHandle.hpp"
#include "PhysicsEngine/PhysicsShapehandle.hpp"
#include "PhysicsEngine/PhysicsInterface.hpp"



// ==============================[Public Constructor + Destructor]============================== //

/**
 * Default Constructor.
 */
PhysicsActorHandle::PhysicsActorHandle  ()
:   m_actor {nullptr}
{}

/**
 * Constructor where we initialize the physx::PxRigidActor with a pointer.
 */
PhysicsActorHandle::PhysicsActorHandle  (physx::PxRigidActor*       p_actor)
:   m_actor {p_actor}
{}

/**
 * Copy Constructor.
 */
PhysicsActorHandle::PhysicsActorHandle  (PhysicsActorHandle const&  p_copy)
:   m_actor {p_copy.m_actor}
{}

/**
 * Move Constructor.
 */
PhysicsActorHandle::PhysicsActorHandle  (PhysicsActorHandle&&       p_move)
:   m_actor {std::move(p_move.m_actor)}
{}

// ==============================[Public Constructor + Destructor]============================== //

void                    PhysicsActorHandle::AddAngularImpulse           (Vector3 const&                     p_angularImpulse,
                                                                         bool const                         p_velocityChange)
{
    if (physx::PxRigidBody* rigidBody   {GetPxRigidBody()})
    {
        rigidBody->addTorque(PhysXTypeConverter::TVector3(p_angularImpulse), p_velocityChange ? physx::PxForceMode::eVELOCITY_CHANGE : physx::PxForceMode::eIMPULSE);
    }  
}

void                    PhysicsActorHandle::AddForce                    (Vector3 const&                     p_force,
                                                                         bool const                         p_accelerationChange /* = false */)
{
    physx::PxRigidBody* rigidBody   {GetPxRigidBody()};

    if (rigidBody != nullptr)
    {
        rigidBody->addForce(PhysXTypeConverter::TVector3(p_force), p_accelerationChange ? physx::PxForceMode::eACCELERATION : physx::PxForceMode::eFORCE);
    }
}

void                    PhysicsActorHandle::AddForceAtLocation          (Vector3 const&                     p_force,
                                                                         Vector3 const&                     p_location)
{
    physx::PxRigidBody* rigidBody   {GetPxRigidBody()};

    if (rigidBody != nullptr)
    {
        physx::PxRigidBodyExt::addForceAtPos(*rigidBody, 
                                             PhysXTypeConverter::TVector3(p_force),
                                             PhysXTypeConverter::TVector3(p_location),
                                             physx::PxForceMode::Enum::eFORCE);
    }
}

void                    PhysicsActorHandle::AddForceAtLocalLocation     (Vector3 const&                     p_force,
                                                                         Vector3 const&                     p_location)
{
    if (physx::PxRigidBody* rigidBody   {GetPxRigidBody()})
    {
        physx::PxRigidBodyExt::addForceAtLocalPos(*rigidBody, 
                                                  PhysXTypeConverter::TVector3(p_force),
                                                  PhysXTypeConverter::TVector3(p_location),
                                                  physx::PxForceMode::Enum::eFORCE);
    }
}

void                    PhysicsActorHandle::AddImpulse                  (Vector3 const&                     p_impulse,
                                                                         bool const                         p_velocityChange /* = false */)
{
    if (physx::PxRigidBody* rigidBody   {GetPxRigidBody()})
    {
        rigidBody->addForce(PhysXTypeConverter::TVector3(p_impulse), p_velocityChange ? physx::PxForceMode::eVELOCITY_CHANGE : physx::PxForceMode::eIMPULSE);
    }
}

void                    PhysicsActorHandle::AddImpulseAtLocation        (Vector3 const&                     p_impulse,
                                                                         Vector3 const&                     p_location)
{
    physx::PxRigidBody* rigidBody   {GetPxRigidBody()};

    if (rigidBody != nullptr)
    {
        physx::PxRigidBodyExt::addForceAtPos(*rigidBody, 
                                             PhysXTypeConverter::TVector3(p_impulse), 
                                             PhysXTypeConverter::TVector3(p_location),
                                             physx::PxForceMode::Enum::eIMPULSE);
    }
}

void                    PhysicsActorHandle::AddImpulseAtLocalLocation   (Vector3 const&                     p_impulse,
                                                                         Vector3 const&                     p_location)
{
    physx::PxRigidBody* rigidBody   {GetPxRigidBody()};

    if (rigidBody != nullptr)
    {
        physx::PxRigidBodyExt::addForceAtLocalPos(*rigidBody, 
                                                  PhysXTypeConverter::TVector3(p_impulse), 
                                                  PhysXTypeConverter::TVector3(p_location),
                                                  physx::PxForceMode::Enum::eIMPULSE);
    }
}

void                    PhysicsActorHandle::AddRadialForce              (Vector3 const&                     p_origin, 
                                                                         float const                        p_radius, 
                                                                         float const                        p_strength, 
                                                                         ERadialImpulseFalloff const        p_falloff,
                                                                         bool const                         p_accelChange /* = false */)
{
    if (physx::PxRigidBody* rigidBody   {GetPxRigidBody()})
    {
        if (!(rigidBody->getRigidBodyFlags() & physx::PxRigidBodyFlag::eKINEMATIC))
        {
            float const                 mass                    {rigidBody->getMass()};
            physx::PxTransform const    centerOfMassTransform   {rigidBody->getGlobalPose().transform(rigidBody->getCMassLocalPose())};
            physx::PxVec3 const         centerOfMassPos         {centerOfMassTransform.p};
            physx::PxVec3 const         origin                  {PhysXTypeConverter::TVector3(p_origin)};
            physx::PxVec3               delta                   {centerOfMassPos - origin};
            float const                 magnitude               {delta.magnitude()};

            if (magnitude <= p_radius)
            {
                delta.normalize();

                float                       forceMagnitude    {p_strength};
                physx::PxForceMode::Enum    forceMode         {p_accelChange ? physx::PxForceMode::eACCELERATION : physx::PxForceMode::eFORCE};


                if (p_falloff == ERadialImpulseFalloff::Linear)
                {
                    forceMagnitude *= (1.0f - (magnitude / p_radius));
                }
                
                physx::PxVec3 impulse   {delta * forceMagnitude };
                rigidBody->addForce(impulse, forceMode);
            }
        }
    }
}

void                    PhysicsActorHandle::AddRadialImpulse            (Vector3 const&                     p_origin,
                                                                         float const                        p_radius,
                                                                         float const                        p_strength,
                                                                         ERadialImpulseFalloff const        p_falloff,
                                                                         bool const                         p_velocityChange)
{
    if (physx::PxRigidBody* rigidBody   {GetPxRigidBody()})
    {
        if (!(rigidBody->getRigidBodyFlags() & physx::PxRigidBodyFlag::eKINEMATIC))
        {
            float const                 mass                    {rigidBody->getMass()};
            physx::PxTransform const    centerOfMassTransform   {rigidBody->getGlobalPose().transform(rigidBody->getCMassLocalPose())};
            physx::PxVec3 const         centerOfMassPos         {centerOfMassTransform.p};
            physx::PxVec3 const         origin                  {PhysXTypeConverter::TVector3(p_origin)};
            physx::PxVec3               delta                   {centerOfMassPos - origin};
            float const                 magnitude               {delta.magnitude()};

            if (magnitude <= p_radius)
            {
                delta.normalize();

                float                       impulseMagnitude    {p_strength};
                physx::PxForceMode::Enum    forceMode           {p_velocityChange ? physx::PxForceMode::eVELOCITY_CHANGE : physx::PxForceMode::eIMPULSE};

                if (p_falloff == ERadialImpulseFalloff::Linear)
                {
                    impulseMagnitude *= (1.0f - (magnitude / p_radius));
                }

                physx::PxVec3 impulse   {delta * impulseMagnitude};
                rigidBody->addForce(impulse, forceMode);
            }
        }
    }
}


void                    PhysicsActorHandle::AddTorque                   (Vector3 const&                     p_torque,
                                                                         bool const                         p_accelChange /* = false */)
{
    if (physx::PxRigidBody* rigidBody   {GetPxRigidBody()})
    {
        rigidBody->addTorque(PhysXTypeConverter::TVector3(p_torque), p_accelChange ? physx::PxForceMode::Enum::eACCELERATION : physx::PxForceMode::Enum::eFORCE);
    }
}

void                    PhysicsActorHandle::AttachShape                 (PhysicsShapeHandle const&          p_shapeHandle)
{
    if (IsValid() && p_shapeHandle.IsValid())
    {
        m_actor->attachShape(*p_shapeHandle.m_shape);
    }
}

bool                    PhysicsActorHandle::CanSimulate                 ()  const
{
    if (m_actor != nullptr)
    {
        return !(m_actor->getActorFlags() & physx::PxActorFlag::Enum::eDISABLE_SIMULATION);
    }

    return false;
}

void                    PhysicsActorHandle::DetachShape                 (PhysicsShapeHandle const&          p_shapeHandle,
                                                                         bool const                         p_wakeOnLostTouch /** = true */)
{
    if (IsValid() && p_shapeHandle.IsValid())
    {
        m_actor->detachShape(*p_shapeHandle.m_shape, p_wakeOnLostTouch);
    }
}

float                   PhysicsActorHandle::GetAngularDamping           ()  const
{
    if (physx::PxRigidDynamic* rigidDynamic {GetPxRigidDynamic()})
    {
        return rigidDynamic->getAngularDamping();
    }

    return 0.0f;
}

Vector3                 PhysicsActorHandle::GetAngularVelocity          ()  const
{
    physx::PxRigidBody* rigidBody   {GetPxRigidBody()};

    if (rigidBody != nullptr)
    {
        return  PhysXTypeConverter::TVector3(rigidBody->getAngularVelocity());
    }

    return Vector3::Zero;
}

Bounds                  PhysicsActorHandle::GetBounds                   ()  const
{
    if (m_actor != nullptr)
    {
        return PhysXTypeConverter::TBounds(m_actor->getWorldBounds());
    }

    return Bounds();
}

Transform               PhysicsActorHandle::GetGlobalPose               ()  const
{
    return m_actor ? PhysXTypeConverter::TTransform(m_actor->getGlobalPose()) : Transform::Identity;
}

Transform               PhysicsActorHandle::GetKinematicTarget          ()  const
{
    physx::PxRigidDynamic* rigidDynamic {GetPxRigidDynamic()};

    physx::PxTransform target;

    if (rigidDynamic != nullptr && rigidDynamic->getKinematicTarget(target))
    {
        return PhysXTypeConverter::TTransform(target);
    }

    return Transform::Identity;
}

float                   PhysicsActorHandle::GetLinearDamping            ()  const
{
    if (physx::PxRigidDynamic* rigidDynamic{ GetPxRigidDynamic() })
    {
        return rigidDynamic->getLinearDamping();
    }

    return 0.0f;
}

Vector3                 PhysicsActorHandle::GetLinearVelocity           ()  const
{
    physx::PxRigidBody* rigidBody   {GetPxRigidBody()};

    if (rigidBody != nullptr)
    {
        return  PhysXTypeConverter::TVector3(rigidBody->getLinearVelocity());
    }

    return Vector3::Zero;
}

Vector3                 PhysicsActorHandle::GetLocalIntertiaTensor      ()  const
{
    physx::PxRigidBody* rigidBody   {GetPxRigidBody()};

    if (rigidBody != nullptr)
    {
        return PhysXTypeConverter::TVector3(rigidBody->getMassSpaceInertiaTensor());
    }

    return Vector3::Zero;
}

float                   PhysicsActorHandle::GetMass                     ()  const
{
     if (physx::PxRigidBody* rigidBody {GetPxRigidBody()})
     {
         return rigidBody->getMass();
     }

    return 0.0f;
}

/**
 * Gets the max angular velocity permitted for this physx::PxActor (In Degrees).
 *
 * @return The max angular velocity of this physx::PxActor.
 */
float                   PhysicsActorHandle::GetMaxAngularVelocityRad    ()  const
{
    physx::PxRigidDynamic* rigidBody   {GetPxRigidDynamic()};

    if (rigidBody != nullptr)
    {
        return  rigidBody->getMaxAngularVelocity();
    }

    return 0.0f;
}

float                   PhysicsActorHandle::GetMaxDepenetrationVelocity ()  const
{
    if (physx::PxRigidBody* rigidBody   {GetPxRigidBody()})
    {
        return rigidBody->getMaxDepenetrationVelocity();
    }

    return 0.0f;
}

int32                   PhysicsActorHandle::GetNumShapes                ()  const
{
    return m_actor ? m_actor->getNbShapes() : 0;
}

bool                    PhysicsActorHandle::GetRigidBodyFlag            (physx::PxRigidBodyFlag::Enum const p_rigidBodyFlag)    const
{
    if (m_actor != nullptr)
    {
        if (physx::PxRigidBody* rigidBody {GetPxRigidBody()})
        {
            return rigidBody->getRigidBodyFlags() & p_rigidBodyFlag;
        }
    }

    return false;
}

PhysicsScene*           PhysicsActorHandle::GetScene                    ()  const
{
    if (m_actor != nullptr)
    {
        if (physx::PxScene* scene {m_actor->getScene()})
        {
            return (PhysicsScene*)scene->userData; 
        }
    }

    return nullptr;
}

void                    PhysicsActorHandle::GetShapes                   (std::vector<PhysicsShapeHandle>&   p_outShapes) const
{
    std::vector<physx::PxShape*> tempPhysXShapes;
    p_outShapes.clear();

    uint32 numShapes    {0};

    if (IsValid())
    {
        numShapes = m_actor->getNbShapes();
        tempPhysXShapes.resize(numShapes);
        m_actor->getShapes(tempPhysXShapes.data(), numShapes);
    }

    p_outShapes.reserve(tempPhysXShapes.size());
    for (physx::PxShape* shape : tempPhysXShapes)
    {
        if (shape != nullptr)
        {
            p_outShapes.push_back(PhysicsShapeHandle(shape));
        }
    }
}

Vector3                 PhysicsActorHandle::GetVelocityAtPoint          (Vector3 const&                     p_point)    const
{
    physx::PxRigidBody* rigidBody   {GetPxRigidBody()};

    if (rigidBody != nullptr)
    {
        return PhysXTypeConverter::TVector3(physx::PxRigidBodyExt::getVelocityAtPos(*rigidBody, 
                                                                                    PhysXTypeConverter::TVector3(p_point)));
    }

    return Vector3::Zero;
}

float                   PhysicsActorHandle::GetWakeCounter              ()  const
{
    if (physx::PxRigidDynamic* rigidDynamic {GetPxRigidDynamic()})
    {
        return rigidDynamic->getWakeCounter();
    }

    return 0.0f;
}

bool                    PhysicsActorHandle::HasKinematicTarget          ()  const
{   
    physx::PxRigidDynamic* rigidDynamic {GetPxRigidDynamic()};

    Transform target;
    return rigidDynamic && rigidDynamic->getKinematicTarget(PhysXTypeConverter::TTransform(target));
}

bool                    PhysicsActorHandle::IsCCDEnabled                ()  const
{
    return GetRigidBodyFlag(physx::PxRigidBodyFlag::eENABLE_CCD);
}

bool                    PhysicsActorHandle::IsDynamic                   ()  const
{
    if (m_actor != nullptr)
    {
        return m_actor->is<physx::PxRigidDynamic>() != nullptr;
    }

    return false;
}

bool                    PhysicsActorHandle::IsGravityEnabled            ()  const
{
    if (m_actor != nullptr)
    {
        return !(m_actor->getActorFlags() & physx::PxActorFlag::Enum::eDISABLE_GRAVITY);
    }

    return false;   
}

bool                    PhysicsActorHandle::IsInScene                   ()  const
{
    if (m_actor != nullptr)
    {
        return m_actor->getScene() != nullptr;
    }

    return false;
}

bool                    PhysicsActorHandle::IsKinematic                 ()  const
{
    return GetRigidBodyFlag(physx::PxRigidBodyFlag::Enum::eKINEMATIC);
}

bool                    PhysicsActorHandle::IsRigidBody                 ()  const
{
    if (m_actor != nullptr)
    {
        return m_actor->is<physx::PxRigidBody>();
    }

    return false;
}

bool                    PhysicsActorHandle::IsSleeping                  ()  const
{
    physx::PxRigidDynamic* rigidDynamic {m_actor ? m_actor->is<physx::PxRigidDynamic>() : nullptr};
    
    return !(rigidDynamic == nullptr) || (rigidDynamic->getScene() != nullptr && rigidDynamic->isSleeping());
}

bool                    PhysicsActorHandle::IsStatic                    ()  const
{
    if (m_actor != nullptr)
    {
        return m_actor->is<physx::PxRigidStatic>() != nullptr;
    }
    
    return false;
}

/**
 * Check if physx::PxRigidActor (m_actor) is valid.
 *
 * @return True if m_actor != nullptr, otherwise false.
 */
bool                    PhysicsActorHandle::IsValid                     ()  const
{
    return m_actor != nullptr;
}

void                    PhysicsActorHandle::PutToSleep                  ()  const
{
    physx::PxRigidDynamic* rigidDynamic {GetPxRigidDynamic()};

    if (rigidDynamic != nullptr)
    {
        rigidDynamic->putToSleep();
    }
}

void                    PhysicsActorHandle::Release                     ()
{
    if (m_actor != nullptr)
    {
        m_actor->release();
        m_actor->userData = nullptr;
        m_actor = nullptr;
    }
}

void                    PhysicsActorHandle::SetAngularDamping           (float const                        p_angularDamping)
{
    physx::PxRigidDynamic* rigidDynamic   {GetPxRigidDynamic()};
    
    if (rigidDynamic != nullptr)
    {
        rigidDynamic->setAngularDamping(p_angularDamping);
    }
}

void                    PhysicsActorHandle::SetAngularVelocity          (Vector3 const&                     p_newAngularVelocity,
                                                                         bool const                         p_autoWake /* = true */)
{
    physx::PxRigidBody* rigidBody   {GetPxRigidBody()};

    if (rigidBody != nullptr)
    {
        rigidBody->setAngularVelocity(PhysXTypeConverter::TVector3(p_newAngularVelocity), p_autoWake);
    }
}

void                    PhysicsActorHandle::SetCCDEnabled               (bool const                         p_isCCDEnabled)
{
    SetRigidBodyFlag<physx::PxRigidBodyFlag::eENABLE_CCD>(p_isCCDEnabled);
}

void                    PhysicsActorHandle::SetCenterOfMassLocaPose     (Transform const&                   p_newCenterOfMassLocalPose)
{
    if (physx::PxRigidBody* rigidBody   {GetPxRigidBody()})
    {
        rigidBody->setCMassLocalPose(PhysXTypeConverter::TTransform(p_newCenterOfMassLocalPose));
    }
}

void                    PhysicsActorHandle::SetGlobalPose               (Transform const&                   p_globalePose,
                                                                         bool const                         p_autoWake /* = false*/)
{
    if (m_actor != nullptr)
    {
        m_actor->setGlobalPose(PhysXTypeConverter::TTransform(p_globalePose), p_autoWake);
    }
}

void                    PhysicsActorHandle::SetGravityEnabled           (bool const                         p_enabled)
{
    if (physx::PxRigidBody* rigidBody   {GetPxRigidBody()})
    {
        PhysicsInterface::ModifyActorFlag<physx::PxActorFlag::eDISABLE_GRAVITY>(rigidBody, !p_enabled);
    }
}

void                    PhysicsActorHandle::SetIsKinematic              (bool const                         p_isKinematic)
{
    SetRigidBodyFlag<physx::PxRigidBodyFlag::eKINEMATIC>(p_isKinematic);
}

void                    PhysicsActorHandle::SetKinematicTarget          (Transform const&                   p_newTarget)
{
    physx::PxRigidDynamic*  rigidDynamic    {GetPxRigidDynamic()};

    if (rigidDynamic != nullptr)
    {
        rigidDynamic->setKinematicTarget(PhysXTypeConverter::TTransform(p_newTarget));
    }
}

void                    PhysicsActorHandle::SetLinearDamping            (float const                        p_linearDamping)
{
    if (physx::PxRigidDynamic* rigidDynamic{ GetPxRigidDynamic() })
    {
        rigidDynamic->setLinearDamping(p_linearDamping);
    }
}

void                    PhysicsActorHandle::SetLinearVelocity           (Vector3 const&                     p_newLinearVelocity,
                                                                         bool const                         p_autoWake /* = true */)
{
    physx::PxRigidBody* rigidBody   {GetPxRigidBody()};

    if (rigidBody != nullptr)
    {
        rigidBody->setLinearVelocity(PhysXTypeConverter::TVector3(p_newLinearVelocity), p_autoWake);
    }  
}

void                    PhysicsActorHandle::SetMaxAngularVelocity       (float const                        p_newMaxAngularVelocity)
{
    physx::PxRigidDynamic* rigidBody   {GetPxRigidDynamic()};

    if (rigidBody != nullptr)
    {
        rigidBody->setMaxAngularVelocity(p_newMaxAngularVelocity);
    }
}

void                    PhysicsActorHandle::SetMaxDepenetrationVelocity (float const                        p_newMaxDepenetrationVelocity)
{
    if (physx::PxRigidBody* rigidBody   {GetPxRigidBody()})
    {
        rigidBody->setMaxDepenetrationVelocity(p_newMaxDepenetrationVelocity);
    }
}

void                    PhysicsActorHandle::SetMass                     (float const                        p_newMass)
{
    if (physx::PxRigidBody* rigidBody   {GetPxRigidBody()})
    {
        rigidBody->setMass(p_newMass);
    }
}

void                    PhysicsActorHandle::SetMassSpaceInertiaTensor   (Vector3 const&                     p_newMassSpaceInertiaTensor)
{
    if (physx::PxRigidBody* rigidBody   {GetPxRigidBody()})
    {
        rigidBody->setMassSpaceInertiaTensor(PhysXTypeConverter::TVector3(p_newMassSpaceInertiaTensor));
    }
}

template <physx::PxRigidBodyFlag::Enum const p_flagToSet>
void                    PhysicsActorHandle::SetRigidBodyFlag            (bool const                         p_value)
{
     if (physx::PxRigidBody* rigidBody   {GetPxRigidBody()})
     {
        PhysicsInterface::ModifyRigidBodyFlag<p_flagToSet>(rigidBody, p_value);
     }
}

void                    PhysicsActorHandle::SetSendsSleepNotifies       (bool const                        p_sendSleepNotifies)
{

    if (physx::PxRigidBody* rigidBody   {GetPxRigidBody()})
    {
       PhysicsInterface::ModifyActorFlag<physx::PxActorFlag::eSEND_SLEEP_NOTIFIES>(rigidBody, p_sendSleepNotifies);
    }
}

/**
 * Set physx::RigidActor->userData of m_actor.
 *
 * @param p_userData    The user data to set with.
 */
void                    PhysicsActorHandle::SetUserData                 (void*                              p_userData)
{
    if (m_actor != nullptr)
    {
        m_actor->userData = p_userData;
    }
}

void                    PhysicsActorHandle::SetWakeCounter              (float const                        p_wakeCounter)
{
    if (physx::PxRigidDynamic* rigidDynamic {GetPxRigidDynamic()})
    {
        rigidDynamic->setWakeCounter(p_wakeCounter);
    }
}

void                    PhysicsActorHandle::WakeUp                      ()  const
{
    physx::PxRigidDynamic* rigidDynamic {GetPxRigidDynamic()};

    if (rigidDynamic != nullptr)
    {
        rigidDynamic->wakeUp();
    } 
}

// ==============================[Public Local Operator]============================== //

/**
 * Checks if both PhysicsActorHandle correspond to the same physx::PxRigidActor.
 *
 * @param p_other   The other PhysicsActorHandle to compare with.
 *
 * @return          True if they correspond to the same physx::PxRigidActor.
 */
bool                PhysicsActorHandle::operator==  (PhysicsActorHandle const&  p_other)    const
{
    return  m_actor == p_other.m_actor;
}

/**
 * Checks if both PhysicsActorHandle not correspond to the same physx::PxRigidActor.
 *
 * @param p_other   The other PhysicsActorHandle to compare with.
 *
 * @return          True if they correspond to the same physx::PxRigidActor.
 */
bool                PhysicsActorHandle::operator!=  (PhysicsActorHandle const&  p_other)    const
{
    return  m_actor != p_other.m_actor;
}

/**
 * Copy another PhysicsActorHandle into this one.
 *
 * @param p_other   The other PhysicsActorHandle to copy.
 *
 * @return          The reference to PhysicsActorHandle after copy.
 */
PhysicsActorHandle& PhysicsActorHandle::operator=   (PhysicsActorHandle const&  p_copy)
{
    m_actor = p_copy.m_actor;

    return *this;
}

/**
 * Move another PhysicsActorHandle into this one.
 *
 * @param p_other   The other PhysicsActorHandle to move.
 *
 * @return          The reference to PhysicsActorHandle after move.
 */
PhysicsActorHandle& PhysicsActorHandle::operator=   (PhysicsActorHandle&&       p_move)
{
    m_actor = std::move(p_move.m_actor);

    return *this;
}