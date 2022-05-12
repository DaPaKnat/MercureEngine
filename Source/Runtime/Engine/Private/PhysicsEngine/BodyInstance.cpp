#include "PCH.hpp"

#include "PhysicsEngine/BodyInstance.hpp"
#include "PhysicsEngine/InitBodyHelper.hpp"
#include "PhysicsEngine/PhysicsShapeHandle.hpp"

#include "Components/PrimitiveComponent.hpp"
#include "Components/BoxComponent.hpp"
#include "Components/CapsuleComponent.hpp"
#include "Components/SphereComponent.hpp"

#include "GameFramework/Entity.hpp"

// ==============================[Public Local Properties]============================== //

/**
 * Default Constructor. (NOT DEFINED YET)
 */
 BodyInstance::BodyInstance ()
 :  m_useCCD                            {false},
    m_contactModification               {false},
    m_startAwake                        {true},
    m_generateWakeEvent                 {false},
    m_updateMassWhenScaleChanges        {false},
    m_overrideMaxAngularVelocity        {false},
    m_inertiaTensorScale                {1.0f},
    m_maxAngularVelocity                {360.0f},
    m_customSleepTresholdMultiplier     {1.0f},
    m_stabilizationTresholdMultiplier   {1.0f},
    m_ownerComponent                    {nullptr},
    m_simulatePhysics                   {false},
    m_angularDamping                    {0.0f},
    m_massInKgOverride                  {1.0f},
    m_massScale                         {1.0f},
    m_linearDamping                     {1.0f},
    m_notifyRigidBodyCollision          {false},
    m_overrideMass                      {false},
    m_physicalMaterialOverride          {nullptr},
    m_objectType                        {ECollisionChannel::WorldStatic},
    m_collisionEnabled                  {ECollisionEnabled::QueryAndPhysics},
    m_collisionResponses                {CollisionResponseContainer::GetDefaultResponseContainer()},
    m_enableGravity                     {true}
 {
 }


// ==============================[Public Local Methods]============================== //

/**
 * Add a rotational impulse of this body
 *
 * @param p_angularImpulseDeg   The rotational impulse to add to this body (In Degrees)
 * @param p_velocityChange      ...
 */
void                BodyInstance::AddAngularImpulseRad          (Vector3 const                      p_angularImpulseRad,
                                                                 bool const                         p_velocityChange)
{
    if (m_actorHandle.IsRigidBody() && m_actorHandle.IsInScene() && !m_actorHandle.IsKinematic())
    {
        m_actorHandle.AddAngularImpulse(p_angularImpulseRad, p_velocityChange);
    }
}

/**
 * Add force to this body.
 *
 * @param p_force       The force vector to apply. The magnitudes indicates strength of force.
 * @param p_accelChange If true, p_force is taken as a change in acceleration instead of a physical force (i.e. mass will have no effect).
 */
void                BodyInstance::AddForce                      (Vector3 const&                     p_force, 
                                                                 bool const                         p_accelChange /* = false */)
{
    if (!m_actorHandle.IsKinematic())
    {   
        m_actorHandle.AddForce(p_force, p_accelChange);
    }
}
/**
 *	Add a force to the body at a particular location in world space.
 *
 *	@param p_force      The force vector to apply. Magnitude indicates strength of force.
 *	@param p_location   The location to apply force, in world space.
 */
void                BodyInstance::AddForceAtLocation            (Vector3 const&                     p_impulse,
                                                                 Vector3 const&                     p_position)
{
    if (m_actorHandle.IsRigidBody() && m_actorHandle.IsInScene() && !m_actorHandle.IsKinematic())
    {
        m_actorHandle.AddForceAtLocation(p_impulse, p_position);
    }
}

/**
 *	Add a force to the body at a particular location. Both Force and Location should be in body space.
 *
 *	@param p_force      The force vector to apply. Magnitude indicates strength of force.
 *	@param p_location   The location to apply force, in component space.
 */
void                BodyInstance::AddForceAtLocalLocation       (Vector3 const&                     p_impulse,
                                                                 Vector3 const&                     p_position)
{
    if (m_actorHandle.IsRigidBody() && m_actorHandle.IsInScene() && !m_actorHandle.IsKinematic())
    {
        m_actorHandle.AddForceAtLocalLocation(p_impulse, p_position);
    }
}

/**
 * Add an impuse to this body.
 *
 * @param p_impulse         The impulse to add.
 * @param p_velocityChange  
 */
void                BodyInstance::AddImpulse                    (Vector3 const&                     p_impulse,
                                                                 bool                               p_velocityChange /* = false */)
{
   if (m_actorHandle.IsRigidBody() && m_actorHandle.IsInScene() && !m_actorHandle.IsKinematic())
   {
       m_actorHandle.AddImpulse(p_impulse, p_velocityChange);
   }
}

/**
 * Add an impulse to this body and a particular world position.
 *
 * @param p_impulse     The impulse to add.
 * @param p_position    Where to applye the impulse.
 */
void                BodyInstance::AddImpulseAtLocation          (Vector3 const&                     p_impulse,
                                                                 Vector3 const&                     p_position)
{
    if (m_actorHandle.IsRigidBody() && m_actorHandle.IsInScene() && !m_actorHandle.IsKinematic())
    {
        m_actorHandle.AddImpulseAtLocation(p_impulse, p_position);
    }
}

/**
 *	Add an impulse to the body a particular location in local space.  Both impulse and Location should be in body space.
 *
 *	@param p_impulse    The impulse vector to apply. Magnitude indicates strength of force.
 *	@param p_location   The location to apply force, in local space.
 */
void                BodyInstance::AddImpulseAtLocalLocation     (Vector3 const&                     p_impulse,
                                                                 Vector3 const&                     p_position)
{
    if (m_actorHandle.IsRigidBody() && m_actorHandle.IsInScene() && !m_actorHandle.IsKinematic())
    {
        m_actorHandle.AddImpulseAtLocalLocation(p_impulse, p_position);
    }
}

/**
 *	Add a force to this body, originating from the supplied world-space location.
 *
 *	@param p_origin		    The origin of force in world space.
 *	@param p_radius		    The radius within which to apply the force.
 *	@param p_strength		The strength of force to apply.
 *  @param p_falloff		Allows you to control the strength of the force as a function of distance from Origin.
 *  @param p_accelChange    If true, Strength is taken as a change in acceleration instead of a physical force (i.e. mass will have no effect).
 */
void                BodyInstance::AddRadialForce                (Vector3 const&                     p_origin, 
                                                                 float const                        p_radius, 
                                                                 float const                        p_strength, 
                                                                 ERadialImpulseFalloff const        p_falloff,
                                                                 bool const                         p_accelChange /* = false */)
{
    if (m_actorHandle.IsRigidBody() && m_actorHandle.IsInScene() && !m_actorHandle.IsKinematic())
    {
        m_actorHandle.AddRadialForce(p_origin, p_radius, p_strength, p_falloff, p_accelChange);
    } 
}

/**
 * Add an impulse to this body, radiating out from the specified position.
 *
 * @param p_origin		    The point of origin for the radial impulse blast, in world space
 * @param p_radius		    The size of radial impulse. Beyond this distance from Origin, there will be no affect.
 * @param p_strength		The maximum strength of impulse applied to body.
 * @param p_falloff		    Allows you to control the strength of the impulse as a function of distance from Origin.
 * @param p_velocityChange  If true, the Strength is taken as a change in velocity instead of an impulse (ie. mass will have no effect).
 */
void                BodyInstance::AddRadialImpulse              (Vector3 const&                     p_origin, 
                                                                 float const                        p_radius, 
                                                                 float const                        p_strength, 
                                                                 ERadialImpulseFalloff const        p_falloff, 
                                                                 bool const                         p_velocityChange /* = false */)
{
    if (m_actorHandle.IsRigidBody() && m_actorHandle.IsInScene() && !m_actorHandle.IsKinematic())
    {
        m_actorHandle.AddRadialImpulse(p_origin, p_radius, p_strength, p_falloff, p_velocityChange);
    }
}

/**
 *	Add a torque to this body.
 *
 *	@param p_torque		    The torque to apply. Direction is axis of rotation and magnitude is strength of torque.
 *  @param p_AccelChange    If true, p_torque is taken as a change in angular acceleration instead of a physical torque (i.e. mass will have no effect).
 */
void                BodyInstance::AddTorqueRad                  (Vector3 const&                     p_torque,
                                                                 bool const                         p_accelChange /* = false */)
{
    if (!m_actorHandle.IsKinematic())
    {
        m_actorHandle.AddTorque(p_torque, p_accelChange);
    }
}

void                BodyInstance::BuildBodyCollisionFlags       (BodyCollisionFlags&                p_outBodyCollisionFlags)
{
    ECollisionEnabled collisionEnabled  {GetCollisionEnabled()};

    if (collisionEnabled != ECollisionEnabled::NoCollision)
    {
        // Query Collision
        p_outBodyCollisionFlags.m_enableQueryCollision = CollisionEnabledHasQuery(collisionEnabled);

        bool const simulateCollision    {CollisionEnabledHasPhysics(collisionEnabled)};

        if (simulateCollision == true)
        {
            p_outBodyCollisionFlags.m_enableSimulationCollisionSimple   = true;
            p_outBodyCollisionFlags.m_enableSimulationCollisionComplex  = false;
        }

        else
        {
            p_outBodyCollisionFlags.m_enableSimulationCollisionSimple   = false;
            p_outBodyCollisionFlags.m_enableSimulationCollisionComplex  = false;
        }
    }
}

void                BodyInstance::BuildBodyFilterData           (BodyCollisionFilterData&           p_outBodyFilterData)   const
{
    PrimitiveComponent*         ownerPrimitiveComponent         {m_ownerComponent};
    Entity*                     ownerEntity                     {ownerPrimitiveComponent ? ownerPrimitiveComponent->GetOwner() : nullptr};
    bool const                  physicsStatic                   {!ownerPrimitiveComponent || (ownerPrimitiveComponent->GetMobility() == EComponentMobility::Static && GetObjectType() == ECollisionChannel::WorldStatic)};
    
    ECollisionEnabled           useCollisionEnabled             {GetCollisionEnabled()};
    bool const                  useNotitfyRigidBodyCollision    {m_notifyRigidBodyCollision};
    CollisionResponseContainer  useResponse                     {GetResponseToChannels()};
    ECollisionChannel           useChannel                      {GetObjectType()};

    if (useCollisionEnabled != ECollisionEnabled::NoCollision)
    {
        bool const  useCCD  {m_useCCD};

        CollisionFilterData simulationFilterData;
        CollisionFilterData simpleQueryData;

        uint32              entityID            {ownerEntity ? static_cast<uint32>(ownerEntity->GetInstanceID()) : 0};
        uint32              componentID         {ownerPrimitiveComponent ? static_cast<uint32>(ownerPrimitiveComponent->GetInstanceID()) : 0};

        uint32 blockingBits {0};
        uint32 touchingBits {0};
        for (int32 index {0}; index < 32; ++index)
        {
            uint8 const response   {useResponse.m_enumArray[index]};
            if      (response == static_cast<uint8>(ECollisionResponse::Block))
            {
                blockingBits |= (1<<index);
            }

            else if (response == static_cast<uint8>(ECollisionResponse::Overlap))
            {
                touchingBits |= (1<<index);
            }
        }

        p_outBodyFilterData.m_simulationFilter          = CollisionFilterData();
        p_outBodyFilterData.m_simulationFilter.m_word0  = entityID;
        p_outBodyFilterData.m_simulationFilter.m_word1  = blockingBits;
        p_outBodyFilterData.m_simulationFilter.m_word2  = componentID;
        p_outBodyFilterData.m_simulationFilter.m_word3  = static_cast<uint8>(useChannel);
        
        p_outBodyFilterData.m_querySimpleFilter         = CollisionFilterData();
        p_outBodyFilterData.m_querySimpleFilter.m_word0 = entityID;
        p_outBodyFilterData.m_querySimpleFilter.m_word1 = blockingBits;
        p_outBodyFilterData.m_querySimpleFilter.m_word2 = touchingBits;
        p_outBodyFilterData.m_querySimpleFilter.m_word3 = static_cast<uint8>(useChannel);

        p_outBodyFilterData.m_queryComplexFilter        = p_outBodyFilterData.m_querySimpleFilter;
    }
}

/**
 * Gets the current angular velocity in world space from physics body in radians.
 *
 * @return  The current angular velocity in radian of thi body.
 */
Vector3             BodyInstance::GetAngularVelocityRad         ()  const
{
    return m_actorHandle.GetAngularVelocity();
}

/**
 * Get the bounds of physics representation of this body.
 *
 * @return The bounds of this body.
 */
Bounds              BodyInstance::GetBodyBounds                 ()  const
{
    return m_actorHandle.GetBounds();
}

/**
 * Gets the body's mass.
 *
 * @return The mass of this body.
 */
float               BodyInstance::GetBodyMass                   ()  const
{
    return m_actorHandle.GetMass();
}

/**
 * Gets the body's inertia tensor.
 *
 * @return The inertia tensor of this body (In local mass space).
 */
Vector3             BodyInstance::GetBodyInertiaTensor          ()  const
{
    return m_actorHandle.GetLocalIntertiaTensor();
}

/**
 * Gets the current linear velocity in world space fron physics body.
 *
 * @return The linear velocity of this body.
 */
Vector3             BodyInstance::GetLinearVelocity             ()  const
{
    return m_actorHandle.GetLinearVelocity();
}

PhysicsActorHandle* BodyInstance::GetPhysicsActorHandle         ()
{
    return &m_actorHandle;
}

PhysicsScene*       BodyInstance::GetPhysicsScene               ()  const
{
    if (m_actorHandle.IsValid())
    {
        return m_actorHandle.GetScene();
    }

    return nullptr;
}

/**
 * Get the current velocity of a point on this physics body (In world space).
 *
 * @param p_point   The position we wish to determine the velocity for (In worl space).
 *
 * @return The velocity of the body at the position given.
 */
Vector3             BodyInstance::GetVelocityAtPoint            (Vector3 const&                     p_point)    const
{
    return m_actorHandle.GetVelocityAtPoint(p_point);
}

/**
 * Checks if this body is awake.
 *
 * @return True, if the body is awake, otherwise false.
 */
bool                BodyInstance::IsAwake                       ()  const
{
    bool isAwake    {false};

    if (m_actorHandle.IsDynamic())
    {
        isAwake = !m_actorHandle.IsSleeping();
    }

    return isAwake;
}

/**
 * Checks if the body is Dynamic.
 *
 * @return True if the body is dynamic, otherwise false (In this cas the body is static).
 */
bool                BodyInstance::IsDynamic                     ()  const
{
    return m_actorHandle.IsDynamic();
}

void                BodyInstance::Initialize                    (Transform const&                   p_transform,
                                                                 PrimitiveComponent*                p_ownerComponent,
                                                                 PhysicsSceneHandle*                p_physicsScene)
{
    InitBodyHelper initBodyHelper   {this, p_transform, p_ownerComponent, p_physicsScene};
    initBodyHelper.InitBody();
}

/**
 * Checks if the body is non kinematic.
 *
 * @return True if the body is non kinematic, otherwise false.
 */
bool                BodyInstance::IsNonKinematic                ()  const
{
    return m_simulatePhysics;
}

bool                BodyInstance::IsNotifyRigidBodyCollision    ()  const
{
    return m_notifyRigidBodyCollision;
}


/**
 * Force the body to sleep.
 * The body must be dynamic, in a scene and non kinematic to have to the possibility to put sleep.
 */
void                BodyInstance::PutToSleep                    ()
{
    if (m_actorHandle.IsDynamic() && m_actorHandle.IsInScene() && !m_actorHandle.IsKinematic())
    {
        m_actorHandle.PutToSleep();
    }
}

void                BodyInstance::Release                       ()
{
    m_actorHandle.Release();
    m_ownerComponent = nullptr;
}

/** 
 * Replace the channels on this body matching the old response with the new response.
 *
 * @param p_oldResponse The old response to remplace.
 * @param p_newResponse The new response to apply.
 */
void                BodyInstance::ReplaceResponseToChannels     (ECollisionResponse const           p_oldResponse,
                                                                 ECollisionResponse const           p_newResponse)
{
    m_collisionResponses.ReplaceChannels(p_oldResponse, p_newResponse);
    UpdatePhysicsFilterData();
}

/**
 * Sets the angular damping of this body.
 *
 * @param p_newAngularDamping The new angular damping of this body.
 */
void                BodyInstance::SetAngularDamping             (float const                        p_newAngularDamping)
{
    if (m_actorHandle.IsDynamic())
    {
        m_actorHandle.SetAngularDamping(p_newAngularDamping);
        m_angularDamping = m_actorHandle.GetAngularDamping();
    }
}

/**
 * Sets the angular velocity of this body.
 *
 * @param p_newAngVelocityDeg   The new angular velocity of this body (In Radians).
 * @param p_addToCurrent        Should we add this angular velocity to the current one.
 * @param p_autoWake            Should the body auto wake with the modification.
 */
void                BodyInstance::SetAngularVelocityRad         (Vector3 const&                     p_newAngularVelocityRad, 
                                                                bool const                         p_addToCurrent,
                                                                bool const                         p_autoWake /* = true */)
{
    if (!m_actorHandle.IsKinematic())
    {
        Vector3 finalVelocity   {p_newAngularVelocityRad};

        if (p_addToCurrent == true)
        {
            finalVelocity += GetAngularVelocityRad();
        }

        m_actorHandle.SetAngularVelocity(finalVelocity, p_autoWake);
    }
}

/**
 * Modifies what kind of collision is enabled for this body.
 *
 * @param p_newType                 The new type of collision for this body.
 * @param p_updatePhysicsFilterData Should we update Physics Filter Data.
 */
void                BodyInstance::SetCollisionEnabled           (ECollisionEnabled const            p_newType,
                                                                 bool const                         p_updatePhysicsFilterData /* = true */)
{
    if (m_collisionEnabled != p_newType)
    {
        bool const              wasPhysicsEnabled   {CollisionEnabledHasPhysics(m_collisionEnabled)};
        bool const              isPhysicsEnabled    {CollisionEnabledHasPhysics(p_newType)};

        m_collisionEnabled = p_newType;

        if (p_updatePhysicsFilterData)
        {
            UpdatePhysicsFilterData();
        }

        if (wasPhysicsEnabled != isPhysicsEnabled)
        {
            if (m_ownerComponent != nullptr)
            {
                m_ownerComponent->RecreatePhysicsState();
            }
        }

    }
}

/**
 * Enables / Disables whether this body is affected by the gravity.
 *
 * @param p_gravityEnabled  If true, enables the gravity on this body, otherwise it disables.
 */
void                BodyInstance::SetEnableGravity              (bool const                         p_gravityEnabled)
{
    if (m_enableGravity != p_gravityEnabled)
    {
        m_enableGravity = p_gravityEnabled;

        if (m_actorHandle.IsRigidBody())
        {
            m_actorHandle.SetGravityEnabled(m_enableGravity);
        }

        if (m_enableGravity == true)
        {
            Wake();
        }
    }
}

/**
 * Sets the linear damping of this body.
 *
 * @param p_newLinearDamping The new linear damping of this body.
 */
void                BodyInstance::SetLinearDamping              (float const                        p_newLinearDamping)
{
    if (m_actorHandle.IsDynamic())
    {
        m_actorHandle.SetLinearDamping(p_newLinearDamping);
        m_linearDamping = m_actorHandle.GetLinearDamping();
    }
}

/**
 * Sets the linear velocity of this body.
 *
 * @param p_newVelocity The new velocity of this body.
 * @param p_autoWake    Should the body auto wake with the modification.
 */
void                BodyInstance::SetLinearVelocity             (Vector3 const&                     p_newVelocity,
                                                                 bool const                         p_addToCurrent,
                                                                 bool const                         p_autoWake /* = true */)
{
    if (!m_actorHandle.IsKinematic())
    {
        Vector3 finalVelocity   {p_newVelocity};

        if (p_addToCurrent == true)
        {
            finalVelocity += GetLinearVelocity();
        }

        m_actorHandle.SetLinearVelocity(finalVelocity, p_autoWake);
    }
}

/**
 * Sets the mass override of this body.
 *
 * @param p_newMassInKg     The new mass override in Kg of this body.
 * @param p_newOverrideMass Shoud this body override the mass.
 */
void                BodyInstance::SetMassOverride               (float const                        p_newMassInKg,
                                                                 bool const                         p_newOverrideMass /* = true */)
{
    m_massInKgOverride  = p_newMassInKg;
    m_overrideMass      = p_newOverrideMass;
    UpdateMassProperties();
}

/**
 * Set the maximum angular vecolity of this boddy.
 *
 * @param p_newMaxAngularVelocityDeg            The new maximum angular velocity (In Radians).
 * @param p_addToCurrent                        Should we add the angular velocity given to the old one.
 * @param p_updateOverrideMaxAngularVelocity    Should we update override max angular velocity.
 */
void                BodyInstance::SetMaxAngularVelocityRad      (float                              p_newMaxAngularVelocityRad, 
                                                                 bool const                         p_addToCurrent, 
                                                                 bool const                         p_updateOverrideMaxAngularVelocity /* = true */)
{
    float newMaxAngularVelocityDeg  {Math::RadToDeg(p_newMaxAngularVelocityRad)};

    if (p_addToCurrent == true)
    {
        float const oldMaxAngularVelocityRad    {m_actorHandle.GetMaxAngularVelocityRad()};
        
        p_newMaxAngularVelocityRad += oldMaxAngularVelocityRad;
        newMaxAngularVelocityDeg = Math::RadToDeg(p_newMaxAngularVelocityRad);
    }
    m_actorHandle.SetMaxAngularVelocity(p_newMaxAngularVelocityRad);

    m_maxAngularVelocity = newMaxAngularVelocityDeg;

    if (p_updateOverrideMaxAngularVelocity == true)
    {
        m_overrideMaxAngularVelocity = true;
    }
}

/** 
 * Set whether we should get a notification about physics collisions 
 *
 * @param p_newNotifyRBCollision    True : Enable notify rigidbody collision / False : Disable notify rigidbody collision.
 */
void                BodyInstance::SetNotifyRBCollision          (bool const                         p_newNotifyRBCollision)
{
    if (m_notifyRigidBodyCollision != p_newNotifyRBCollision)
    {
        m_notifyRigidBodyCollision = p_newNotifyRBCollision;
        UpdatePhysicsFilterData();
    }
}

/** 
 * Set physical material override for this body.
 */
void                BodyInstance::SetPhysicalMaterialOverride   (PhysicalMaterial*                  p_newPhysicalMaterial)
{
    if (m_physicalMaterialOverride != p_newPhysicalMaterial)
    {
        m_physicalMaterialOverride = p_newPhysicalMaterial;

        UpdatePhysicalMaterials();
        UpdateMassProperties();
    }
}

void                BodyInstance::SetPhysicsActorHandle         (PhysicsActorHandle const&          p_physicsActorHandle)
{
    m_actorHandle = p_physicsActorHandle;
}

/** 
 * Set the response of this body to all channels.
 *
 * @param p_newResponse The new response to set to all channels.
 */
void                BodyInstance::SetResponseToAllChannels      (ECollisionResponse const           p_newResponse)
{
    m_collisionResponses.SetAllChannels(p_newResponse);
    UpdatePhysicsFilterData();
}

/**
 * Set the collision response of this body to a particular channel.
 *
 * @param p_channel     The channel whose response we are adusting.
 * @param p_newResponse The new response to set toe the given channel.
 */
void                BodyInstance::SetResponseToChannel          (ECollisionChannel const            p_channel, 
                                                                 ECollisionResponse const           p_newResponse)
{
    m_collisionResponses.SetResponse(p_channel, p_newResponse);
    UpdatePhysicsFilterData();
}

/**
 * Sets the responses of this body.
 *
 * @param p_newResponses    The new collision response container of this body.
 */
void                BodyInstance::SetResponseToChannels         (CollisionResponseContainer const&  p_newResponses)
{
    m_collisionResponses = p_newResponses;
    UpdatePhysicsFilterData();
}

void                BodyInstance::SetSimulatePhysics            (bool const                         p_shoulSimulate)
{
    m_simulatePhysics = p_shoulSimulate;

    UpdateSimulatePhysics();
}

void                BodyInstance::SetTransform                  (Transform const&                   p_newTransform,
                                                                 ETeleportType const                p_teleportType,
                                                                 bool const                         p_autoWake /** = true */)
{
    PhysicsScene* scene {GetPhysicsScene()};

    if (m_actorHandle.IsValid() && scene)
    {
        if (m_actorHandle.IsDynamic())
        {
            bool const isKinematic          {m_actorHandle.IsKinematic()};
            bool const isSimulated          {m_actorHandle.CanSimulate()};
            bool const isKinematicSimulated {isKinematic && isSimulated};
        
            if (isKinematicSimulated && p_teleportType == ETeleportType::None)
            {
                m_actorHandle.SetKinematicTarget(p_newTransform);
            }
            
            else
            {
                if (isKinematicSimulated)
                {
                    m_actorHandle.SetKinematicTarget(p_newTransform);
                }

                m_actorHandle.SetGlobalPose(p_newTransform);
            }
        }
    }

    else if (scene)
    {
        m_actorHandle.SetGlobalPose(p_newTransform);
    }

}


/** 
 * Enable/disable Continuous Collidion Detection feature.
 *
 * @param p_useCCD  True: Enables CCD / False: Disable CCD.
 */
void                BodyInstance::SetUseCCD                     (bool const                         p_useCCD)
{
    if (m_useCCD != p_useCCD)
    {
        if (m_actorHandle.IsRigidBody())
        {
            m_useCCD = p_useCCD;
            m_actorHandle.SetCCDEnabled(p_useCCD);
            UpdatePhysicsFilterData();
        }
    }
}

bool                BodyInstance::UpdateBodyScale               (Vector3 const&                     p_newScale,
                                                                 bool const                         p_forceUpdate /** = false */)
{
    bool hasBeenUpdated {false};

    if (IsValidBodyInstance() && (m_scale != p_newScale || p_forceUpdate))
    {
        Vector3                         updatedScale;    
        std::vector<PhysicsShapeHandle> shapes;

        physx::PxBoxGeometry            boxGeometry;
        physx::PxSphereGeometry         sphereGeometry;
        physx::PxCapsuleGeometry        capsuleGeometry;

        m_actorHandle.GetShapes(shapes);

        for (PhysicsShapeHandle& shape : shapes)
        {
            ECollisionShapeType shapeType           {shape.GetShapeType()};
            physx::PxGeometry*  updatedGeometry     {nullptr};

            switch (shapeType)
            {
                case ECollisionShapeType::Box:
                {
                    BoxComponent* boxComponent  {static_cast<BoxComponent*>(m_ownerComponent)};
                    if (boxComponent)
                    {
                        boxGeometry             = shape.m_shape->getGeometry().box();
                        boxGeometry.halfExtents = PhysXTypeConverter::TVector3(p_newScale * boxComponent->GetUnscaledBoxExtent());
                        
                        if (boxGeometry.isValid())
                        {
                            updatedGeometry = &boxGeometry;
                            hasBeenUpdated  = true;
                        }
                    }
                    break;
                }

                case ECollisionShapeType::Capsule:
                {
                    CapsuleComponent* capsuleComponent  {static_cast<CapsuleComponent*>(m_ownerComponent)};
                    if (capsuleComponent)
                    {
                        capsuleGeometry             = shape.m_shape->getGeometry().capsule();
                        capsuleGeometry.radius      = capsuleComponent->GetUnscaledCapsuleRadius() * (p_newScale.m_x < p_newScale.m_z) ? p_newScale.m_x : p_newScale.m_z;
                        capsuleGeometry.halfHeight  = capsuleComponent->GetUnscaledCapsuleRadius() * p_newScale.m_y;
                        
                        if (capsuleGeometry.isValid())
                        {
                            updatedGeometry = &capsuleGeometry;
                            hasBeenUpdated  = true;
                        }
                    }
                    break;
                }

                case ECollisionShapeType::Sphere:
                {
                    SphereComponent* sphereComponent = static_cast<SphereComponent*>(m_ownerComponent);
                    if (sphereComponent)
                    {
                        sphereGeometry = shape.m_shape->getGeometry().sphere();
                        sphereGeometry.radius = p_newScale.GetAbsMin() * sphereComponent->GetUnscaledSphereRadius();
                        
                        if (sphereGeometry.isValid())
                        {
                            updatedGeometry = &sphereGeometry;
                            hasBeenUpdated  = true;
                        }
                    }
                    break;
                }

                default: 
                {
                    LOG(LogPhysics, Error, "Unknow Shape Type.");
                }
            }

            if (updatedGeometry)
            {
                m_scale = p_newScale;
                shape.SetGeometry(*updatedGeometry);
            }

        }
    }

    return hasBeenUpdated;
}

void                BodyInstance::UpdateMassProperties          ()
{
    LOG(LogPhysics, Warning, "THIS FUNCTION IS NOT DEFINED !");
}

void                BodyInstance::UpdatePhysicalMaterials       ()
{
    LOG(LogPhysics, Warning, "THIS FUNCTION IS NOT DEFINED !");
}

void                BodyInstance::UpdatePhysicsFilterData       ()
{
    if (IsValidBodyInstance())
    {
        std::vector<PhysicsShapeHandle> shapes;
        m_actorHandle.GetShapes(shapes);

        // Computes Body Collision Data
        BodyCollisionData bodyCollisionData;
        BuildBodyCollisionFlags(bodyCollisionData.m_collisionFlags);
        BuildBodyFilterData(bodyCollisionData.m_collisionFilterData);

        bool shouldUpdateMassProperties = false;

        for (int32 shapeIndex {0} ; shapeIndex < shapes.size() ; ++shapeIndex)
        {
            PhysicsShapeHandle&         shapeRef                {shapes[shapeIndex]};

            bool const                  wasSimulationShape      {shapeRef.IsSimulationShape()};
            bool const                  isStatic                {m_actorHandle.IsStatic()};
            bool const                  newQueryShape           {bodyCollisionData.m_collisionFlags.m_enableQueryCollision};
            bool const                  newSimulationShape      {bodyCollisionData.m_collisionFlags.m_enableSimulationCollisionSimple};
        
            shapeRef.SetIsQueryShape(newQueryShape);
            shapeRef.SetIsSimulationShape(newSimulationShape);

            if (wasSimulationShape != newSimulationShape)
            {
                shouldUpdateMassProperties = true;
            }

            shapeRef.SetSimulationFilter(bodyCollisionData.m_collisionFilterData.m_simulationFilter);
            shapeRef.SetQueryFilter(bodyCollisionData.m_collisionFilterData.m_querySimpleFilter);
        }

        if (shouldUpdateMassProperties == true)
        {
            UpdateMassProperties();
        }
    }
}

void                BodyInstance::UpdateSimulatePhysics         ()
{
    bool const useSimulate {IsSimulatingPhysics()};

    m_actorHandle.SetIsKinematic(!useSimulate);
    m_actorHandle.SetCCDEnabled(m_useCCD);

    if (ShouldSimulatingPhysics() && m_startAwake)
    {
        m_actorHandle.WakeUp();
    }
}

/**
 * Wake the body insrance.
 */
void                BodyInstance::Wake                          ()
{
    if (m_actorHandle.IsDynamic() && m_actorHandle.IsInScene() && !m_actorHandle.IsKinematic())
    {
        m_actorHandle.WakeUp();
    }
}

// ==============================[Privaye Local Methods]============================== //

/**
 * Gets the current type of collision enabled but check if the Actor owner has collision enabled.
 *
 * @Return ECollisionEnabled::NoCollision if actor owner enable collision is false, otherwise return body's enable collision.
 */
ECollisionEnabled   BodyInstance::GetCollisionEnabledCheckingOwner  ()  const
{
    Entity const* OwnerEntity {m_ownerComponent ? m_ownerComponent->GetOwner() : nullptr};

    if (OwnerEntity != nullptr && OwnerEntity->GetEnableCollision() == false)
    {
        return ECollisionEnabled::NoCollision;
    }

    else
    {
        return m_collisionEnabled;
    }
}