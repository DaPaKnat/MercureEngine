#include "PCH.hpp"

#include "GameFramework/Entity.hpp"

#include "Components/PrimitiveComponent.hpp"

#include "World.hpp"

#include "PhysicsEngine/CollisionQueryParams.hpp"
#include "PhysicsEngine/CollisionResponseParams.hpp"

class PredicateFIlterCannotOverlap
{
    public:

        PredicateFIlterCannotOverlap    (PrimitiveComponent const&  p_owningComponent)
        :   m_owningComponent {p_owningComponent}
        {}

        bool operator() (OverlapInfo const& p_overlapInfo)  const
        {
            return !PrimitiveComponent::CanGenerateOverlap(&m_owningComponent, p_overlapInfo.m_overlapInfo.m_primitiveComponent);
        }

    private:

        PrimitiveComponent const& m_owningComponent;
};


bool AreEntitieOverlapping  (Entity const&  p_first,
                             Entity const&  p_second)
{
    if (p_first.GetComponents().size() <= p_second.GetComponents().size())
    {
        return p_first.IsOverlapping(&p_second);
    }

    else
    {
        return p_second.IsOverlapping(&p_first);
    }
}

// ==============================[Public Static Methods]============================== //

bool PrimitiveComponent::CanGenerateOverlap  (PrimitiveComponent const* p_firstComponent,
                                              PrimitiveComponent const* p_secondComponent)
{
    return  p_secondComponent &&
            p_secondComponent->GetGenerateOverlapEvents() &&
            p_firstComponent &&
            p_firstComponent->GetGenerateOverlapEvents() &&
            p_firstComponent->GetCollisionResponseToComponent(p_secondComponent) == ECollisionResponse::Overlap;
}

// ==============================[Public Local Contructor + Destructor]============================== //

PrimitiveComponent::PrimitiveComponent  ()
: Super()
{
    m_alwaysCreatePhysicsState      = false;
    m_boundScale                    = 1.0f;
    m_castDynamicShadow             = true;
    m_castShadow                    = false;
    m_castStaticShadow              = true;
    m_ignoreRadialForce             = false;
    m_ignoreRadialImpulse           = false;
    m_neverDistanceCull             = false;
    m_traceComplexCollision         = false;
    m_maxDrawDistance               = 0.0f;
    m_minDrawDistance               = 0.0f;
    
    m_wantsOnUpdateTransform        = true;

    m_bodyInstance.m_ownerComponent = this;

//    SetGenerateOverlapEvents(true);
}

// ==============================[Virtual Public Local Methods]============================== //

/**
 * Add an angular impulse to the rigid body of this component..
 *
 * @param p_impulseRad      The magnitude and direction of impulse to apply. Direction is acis of rotation.
 * @param p_velocityChange  If true, the strengh is take as a change in angular velocity instead of an impulse (ie. mass will have no effect).
 */
void                                PrimitiveComponent::AddAngularImpulseRad                (Vector3 const&                     p_impulseRad,
                                                                                             bool const                         p_velocityChange /* = false */)
{
    if (BodyInstance* bodyInstance {GetBodyInstance()})
    {
        bodyInstance->AddAngularImpulseRad(p_impulseRad, p_velocityChange);
    }
}

/**
 * Add force to the body of this component.
 *
 * @param p_force       The force vector to apply. The magnitudes indicates strength of force.
 * @param p_accelChange If true, p_force is taken as a change in acceleration instead of a physical force (i.e. mass will have no effect).
 */
void                                PrimitiveComponent::AddForce                            (Vector3 const&                     p_force,
                                                                                             bool const                         p_accelChange /* = false */)
{
    if (BodyInstance* bodyInstance {GetBodyInstance()})
    {
        bodyInstance->AddForce(p_force, p_accelChange);
    }
}

/**
 *	Add a force to the rigid body of this component at a particular location. Both Force and Location should be in body space.
 *
 *	@param p_force      The force vector to apply. Magnitude indicates strength of force.
 *	@param p_location   The location to apply force, in component space.
 */
void                                PrimitiveComponent::AddForceAtLocalLocation             (Vector3 const&                     p_force,
                                                                                             Vector3 const&                     p_location)
{
    if (BodyInstance* bodyInstance {GetBodyInstance()})
    {
        bodyInstance->AddForceAtLocalLocation(p_force, p_location);
    }
}

/**
 *	Add a force to the rigid body of this component at a particular location in world space.
 *
 *	@param p_force      The force vector to apply. Magnitude indicates strength of force.
 *	@param p_location   The location to apply force, in world space.
 */
void                                PrimitiveComponent::AddForceAtLocation                  (Vector3 const&                     p_force,
                                                                                             Vector3 const&                     p_location)
{
    if (BodyInstance* bodyInstance {GetBodyInstance()})
    {
        bodyInstance->AddForceAtLocation(p_force, p_location);
    }
}
                                                                                                                                
/**
 * Add an impulse to the rigid body of this component.
 *
 * @param p_impulse         The magnitude and direction of impulse to apply.
 * @param p_velocityChange  If true, the strength is taken as a change in velocity instead of an impulse (ie. mass will have no effect).
 */                                                                                                                     
void                                PrimitiveComponent::AddImpulse                          (Vector3 const&                     p_impulse,
                                                                                             bool const                         p_velocityChange /* = false */ )
{
    if (BodyInstance* bodyInstance {GetBodyInstance()})
    {
        bodyInstance->AddImpulse(p_impulse, p_velocityChange);
    }
}
                                                                                                                                
/**
 * Add an impulse to the rigid body of this component at a specific location (In world space).
 *
 * @param p_impulse     The magnitude and direction of impulse to apply.
 * @param p_location    The point in world space to a impulse at.
 */                                                                                                                     
void                                PrimitiveComponent::AddImpulseAtLocation                (Vector3 const&                     p_impulse,
                                                                                             Vector3 const&                     p_location)
{
    if (BodyInstance* bodyInstance {GetBodyInstance()})
    {
        bodyInstance->AddImpulseAtLocation(p_impulse, p_location);
    }
}
                                                                                                                                
/**
 *	Add an impulse to the rigid body of this component at a particular location in local space.  Both impulse and Location should be in body space.
 *
 *	@param p_impulse    The impulse vector to apply. Magnitude indicates strength of force.
 *	@param p_location   The location to apply force, in local space.
 */
void                                PrimitiveComponent::AddImpulseAtLocalLocation           (Vector3 const&                     p_impulse,
                                                                                             Vector3 const&                     p_location)
{
    if (BodyInstance* bodyInstance {GetBodyInstance()})
    {
        bodyInstance->AddImpulseAtLocalLocation(p_impulse, p_location);
    }
}
                                       
/**
 *	Add a force to to the rigide body of  his component, originating from the supplied world-space location.
 *
 *	@param p_origin		    The origin of force in world space.
 *	@param p_radius		    The radius within which to apply the force.
 *	@param p_strength		The strength of force to apply.
 *  @param p_falloff		Allows you to control the strength of the force as a function of distance from Origin.
 *  @param p_accelChange    If true, Strength is taken as a change in acceleration instead of a physical force (i.e. mass will have no effect).
 */
void                                PrimitiveComponent::AddRadialForce                      (Vector3 const&                     p_origin,
                                                                                             float const                        p_radius, 
                                                                                             float const                        p_strength, 
                                                                                             ERadialImpulseFalloff              p_falloff, 
                                                                                             bool  const                        p_accelChange /* = false*/)
{
    if (m_ignoreRadialForce == false)
    {
        if (BodyInstance* bodyInstance {GetBodyInstance()})
        {
            bodyInstance->AddRadialForce(p_origin, p_radius, p_strength, p_falloff, p_accelChange);
        } 
    }
}

/**
 * Add an impulse to the rigid bodie of this component, radiating out from the specified position.
 *
 * @param p_origin		    The point of origin for the radial impulse blast, in world space.
 * @param p_radius		    The size of radial impulse. Beyond this distance from Origin, there will be no affect.
 * @param p_strength		The maximum strength of impulse applied to body.
 * @param p_falloff		    Allows you to control the strength of the impulse as a function of distance from Origin.
 * @param p_velocityChange  If true, the Strength is taken as a change in velocity instead of an impulse (ie. mass will have no effect).
 */                                                                                                                     
void                                PrimitiveComponent::AddRadialImpulse                    (Vector3 const&                     p_origin,
                                                                                             float const                        p_radius, 
                                                                                             float const                        p_strength, 
                                                                                             ERadialImpulseFalloff              p_falloff, 
                                                                                             bool const                         p_velocityChange /* = false */)
{
    if (m_ignoreRadialImpulse == false)
    {
        if (BodyInstance* bodyInstance {GetBodyInstance()})
        {
            bodyInstance->AddRadialImpulse(p_origin, p_radius, p_strength, p_falloff, p_velocityChange);
        }
    }
}

/**
 *	Add a torque to the rigid body of this component.
 *
 *	@param p_torque		    Torque to apply. Direction is axis of rotation and magnitude is strength of torque.
 *  @param p_AccelChange    If true, p_torque is taken as a change in angular acceleration instead of a physical torque (i.e. mass will have no effect).
 */
void                                PrimitiveComponent::AddTorqueRad                        (Vector3 const&                     p_torque,
                                                                                             bool const                         p_accelChange /* = false */)
{
    if (BodyInstance* bodyInstance {GetBodyInstance()})
    {
        bodyInstance->AddTorqueRad(p_torque, p_accelChange);
    }
}

void                                PrimitiveComponent::DispatchBlockingHit                 (Entity&                            p_owner,
                                                                                             HitResult const&                   p_blockingHit)
{
    PrimitiveComponent* const blockingHitComponent {p_blockingHit.m_primitiveComponent};

    if (blockingHitComponent)
    {
        p_owner.DispatchBlockingHit(blockingHitComponent, this, false, p_blockingHit);

        if (!blockingHitComponent->IsPendingKill())
        {
            if (Entity* const blockingHitEntity {p_blockingHit.m_entity})
            {
                blockingHitEntity->DispatchBlockingHit(blockingHitComponent, this, false, p_blockingHit);
            }
        }
    }
}

/**
 * Get the angular damping of this component.
 *
 * @return The angular damping of this.
 */
float                               PrimitiveComponent::GetAngularDamping                   ()  const
{
    if (BodyInstance* bodyInstance {GetBodyInstance()})
    {
        return bodyInstance->GetAngularDamping();
    }

    return 0.0f;
}

/**
 * Returns the BodyInstance of this component.
 *
 * @return The body instance of this component.
 */
 BodyInstance*                      PrimitiveComponent::GetBodyInstance                     ()  const
 {
    return  const_cast<BodyInstance*>(&m_bodyInstance);
 }

/**
 * Get the collision channel used by this object when it moves.
 *
 * @return  The collision channel of hthis object.
 */
 ECollisionChannel                  PrimitiveComponent::GetCollisionObjectType              ()  const
 {
    return m_bodyInstance.GetObjectType();
 }

/**
 * Gets the form of collision of this component.
 *
 * @return The form of collision of this component.
 */
ECollisionEnabled                   PrimitiveComponent::GetCollisionEnabled                 ()  const
{
    Entity* owner    {GetOwner()};

    if (owner != nullptr && owner->GetEnableCollision() == false)
    {
        return ECollisionEnabled::NoCollision;
    }

    return m_bodyInstance.GetCollisionEnabled();
}

/**
 * Gets the response type given a specific channel.
 *
 * @param p_channel The channel whose response type we want to know.
 *
 * @return The collision response associated to the channel given.
 */
ECollisionResponse                  PrimitiveComponent::GetCollisionResponseToChannel       (ECollisionChannel const            p_channel)  const
{
    return m_bodyInstance.GetResponseToChannel(p_channel);
}

/**
 * Gets a reference constant to CollisionResponseContainer of this component.
 *
 * @return A reference constant to CollisionResponseContainer of this component.
 */
CollisionResponseContainer const&   PrimitiveComponent::GetCollisionResponseToChannels      ()  const
{
    return m_bodyInstance.GetResponseToChannels();
}

CollisionShape                      PrimitiveComponent::GetCollisionShape                   ()  const
{
    return CollisionShape::MakeBox(Vector3(0.5f, 0.5f, 0.5f));
}

EGeometryType                       PrimitiveComponent::GetGeometryType                     ()  const
{
    return EGeometryType::Invalid;
}

/**
 * Get the inertia tensor of this component. The inertia tensor is in local component space.
 *
 * @return The inertia tensor of this component.
 */
Vector3                             PrimitiveComponent::GetInertiaTensor                    ()  const
{
    if (BodyInstance* bodyInstance {GetBodyInstance()})
    {
        return bodyInstance->GetBodyInertiaTensor();
    }

    return Vector3::Zero;
}

/**
 * Get the linear damping of this component.
 *
 * @return The linear damping of this.
 */
float                               PrimitiveComponent::GetLinearDamping                    ()  const

{
    if (BodyInstance* bodyInstance {GetBodyInstance()})
    {
        return bodyInstance->GetLinearDamping();
    }

    return 0.0f;
}

/**
 * Get the mass of this component in Kg.
 *
 * @return The mass of this.
 */
float                               PrimitiveComponent::GetMass                             ()  const

{
    if (BodyInstance* bodyInstance {GetBodyInstance()})
    {
        return bodyInstance->GetBodyMass();
    }

    return 0.0f;
}

float                               PrimitiveComponent::GetMassScale                        ()  const

{
    if (BodyInstance* bodyInstance {GetBodyInstance()})
    {
        return bodyInstance->GetMassScale();
    }

    return 0.0f;
}     

/**
 * Get the number of material element in this primitive.
 *
 * @return The number of material element in this primitive.
 */
int32                               PrimitiveComponent::GetNumMatrials                      ()  const
{
    return 0;
}

std::vector<OverlapInfo> const&     PrimitiveComponent::GetOverlapInfos                     ()  const
{
    return m_overlappingComponents;
}


/**
 * Get the matrix that should be used to render this component.
 *
 * @return The render matrix.
 */
Matrix4x4                           PrimitiveComponent::GetRenderMatrix                     ()  const
{
    return GetWorldTransform().GetMatrix();
}

/**
 * Checks if the physics state of this body is valid.
 *
 * @return True if the physic state of this is valid, otherwise false.
 */
bool                                PrimitiveComponent::HasValidPhysicsState                ()  const
{
    return m_bodyInstance.IsValidBodyInstance();
}


void                                PrimitiveComponent::InitSweepCollisionParams            (CollisionQueryParams&              p_outQueryParams,
                                                                                             CollisionResponseParams&           p_outResponseParams)    const
{
    p_outResponseParams.m_collisionResponseContainer = m_bodyInstance.GetResponseToChannels();
    p_outQueryParams.AddIgnoredEntities(m_moveIgnoreEntities);
    p_outQueryParams.AddIgnoredComponents(m_moveIgnoreComponents);
}

/**
 * Is this component affected by the gravity. Return alway false if the component is not simualted.
 */
bool                                PrimitiveComponent::IsGravityEnabled                    ()  const
{
    if (BodyInstance* bodyInstance {GetBodyInstance()})
    {
        return bodyInstance->IsGravityEnabled();
    }

    return false;   
}

/**
 * Returns whether this component is currently using physics simulation.
 *
 * @return  True if this component is using the physics simulation, otherwise false.
 */
bool                                PrimitiveComponent::IsSimulatingPhysics                 ()  const
{
    BodyInstance* bodyInstance  {GetBodyInstance()};

    if (bodyInstance != nullptr)
    {
        return bodyInstance->IsSimulatingPhysics();
    }

    return false;
}

/**
 * Checks if bound extent is 0.
 * For normal primitive, this is 0 but for ShapeComponent, this will has a valid information.
 * 
 * @return  True if the bound extent is zero, otherwise false.
 */
bool                                PrimitiveComponent::IsZeroExtent                        ()  const
{
    return false;
}


void                                PrimitiveComponent::OnCreatePhysicsState                ()
{
    SceneComponent::OnCreatePhysicsState();

    if (!m_bodyInstance.IsValidBodyInstance())
    {
        Transform bodyTransform   {GetWorldTransform()};

        if (bodyTransform.m_scale.IsNearlyZero())
        {
            bodyTransform.m_scale.Set(MIN_FLOAT);
        }
    
        m_bodyInstance.Initialize(bodyTransform, this, GetWorld()->GetPhysicsScene());
    }
}

void                                PrimitiveComponent::OnDestroyPhysicsState               ()
{
    if (m_bodyInstance.IsValidBodyInstance())
    {
        m_bodyInstance.Release();

        Super::OnDestroyPhysicsState();
    }
}

void                                PrimitiveComponent::OnEntityEnableCollisionChanged      ()
{
    m_bodyInstance.UpdatePhysicsFilterData();
    OnComponentCollisionSettingsChanged();
}

void                                PrimitiveComponent::OnUpdateTransform                   (EUpdateTransformFlags const        p_updateTransformFlags,
                                                                                             ETeleportType const                p_teleportType)
{
    Super::OnUpdateTransform(p_updateTransformFlags, p_teleportType);

    if (m_physicsStateCreated && !(p_updateTransformFlags & EUpdateTransformFlags::PropagateFromParent))
    {
        SendPhysicsTransform(p_teleportType);
    }
}

/**
 * Sets the angular damping of this component.
 *
 * @param p_damping The new angular damping of this compnent.
 */
void                                PrimitiveComponent::SetAngularDamping                   (float const                        p_damping)
{
    if (BodyInstance* bodyInstance {GetBodyInstance()})
    {
        bodyInstance->SetAngularDamping(p_damping);
    }
}

/**
 * Sets the angular velocity of the body of this component.
 *
 * @param p_newAngularVelocityRad   The new angular velocity to apply on the body (In Radians per seconds).
 * @param p_addToCurrent            If true, p_newAngularVelocityRad is added to the existing angular velocity of the body.
 */
void                                PrimitiveComponent::SetAngularVelocityRad               (Vector3 const&                     p_newAngularVelocityRad,
                                                                                             bool const                         p_addToCurrent /* = false */)
{
    if (BodyInstance* bodyInstance {GetBodyInstance()})
    {
        bodyInstance->SetAngularVelocityRad(p_newAngularVelocityRad, p_addToCurrent);
    }
}

/**
 * Controls what kind of collision is enabled for the body of this component.
 *
 * @param p_newType The new type of collision for this body.
 */
void                                PrimitiveComponent::SetCollisionEnabled                 (ECollisionEnabled const            p_newType)
{
    if (m_bodyInstance.GetCollisionEnabled() != p_newType)
    {
        m_bodyInstance.SetCollisionEnabled(p_newType);

        EnsurePhysicsStateCreated();
        OnComponentCollisionSettingsChanged();
    }
}

/**
 * Modify the collision channel used by this object when it moves.
 *
 * @param p_channel The new collision channel of this component.
 */
void                                PrimitiveComponent::SetCollisionObjectType              (ECollisionChannel const            p_channel)
{
    m_bodyInstance.SetObjectType(p_channel);
}

/**
 * Changes all ResponseToChannels container for this PrimitiveComponent. To be NewResponse.
 *
 * @param p_newResponse What the new response should be to the supplied Channel.
 */
void                                PrimitiveComponent::SetCollisionResponseToAllChannels   (ECollisionResponse const           p_newResponse)
{
    m_bodyInstance.SetResponseToAllChannels(p_newResponse);
    OnComponentCollisionSettingsChanged();
}

/**
 * Modifes a member of the ResponseToChannels container for this PrimitiveComponent.
 *
 * @param p_channel      The channel to change the response of.
 * @param p_newResponse  What the new response should be to the supplied Channel.
 */
void                                PrimitiveComponent::SetCollisionResponseToChannel       (ECollisionChannel const            p_channel,
                                                                                             ECollisionResponse const           p_newResponse)
{
    m_bodyInstance.SetResponseToChannel(p_channel, p_newResponse);
    OnComponentCollisionSettingsChanged();
}
/**
 * Changes the whole ResponseToChannels container for this PrimitiveComponent.
 *
 * @param p_newResponses    The new set of responses for this component.
 */
void                                PrimitiveComponent::SetCollisionResponseToChannels      (CollisionResponseContainer const&  p_newResponses)
{
    m_bodyInstance.SetResponseToChannels(p_newResponses);
    OnComponentCollisionSettingsChanged();
}

/** 
 * Enables/disables whether this component is affected by gravity. This applies only to components with bSimulatePhysics set to true. 
 *
 * @param p_gravityEnabled  True: Enables the gravity on this component / False: Disables the gravity on this component.
 */
void                                PrimitiveComponent::SetEnableGravity                    (bool const                         p_gravityEnabled)
{
    if (BodyInstance* bodyInstance {GetBodyInstance()})
    {
        bodyInstance->SetEnableGravity(p_gravityEnabled);
    }
}

/**
 * Sets the linear damping of this component.
 *
 * @param p_damping The new linear damping of this compnent.
 */
void                                PrimitiveComponent::SetLinearDamping                    (float const                        p_damping)
{
    if (BodyInstance* bodyInstance {GetBodyInstance()})
    {
        bodyInstance->SetLinearDamping(p_damping);
    }
}

/**
 * Set the linear velocity of the body of this component.
 *
 * @param p_newLinearVelocity   The linear velocity of the body of this component.
 */
void                                PrimitiveComponent::SetLinearVelocity                   (Vector3 const&                     p_newLinearVelocity,
                                                                                             bool const                         p_addToCurrent /* = false */)
{
    if (BodyInstance* bodyInstance {GetBodyInstance()})
    {
        bodyInstance->SetLinearVelocity(p_newLinearVelocity, p_addToCurrent);
    }
}

/**
 * Set the mass override of the physics body of this component.
 *
 * @param p_massInKg        The new mass override of the body of this component.
 * @param p_overrideMass    Should we override the mass of the physics body of this component.
 */
void                                PrimitiveComponent::SetMassOverrideInKg                 (float const                        p_massInKg /* = 1.0f */,
                                                                                             bool const                         p_overrideMass /* = true */)
{
    if (BodyInstance* bodyInstance {GetBodyInstance()})
    {
        bodyInstance->SetMassOverride(p_massInKg, p_overrideMass);
    }
}

/**
 * Modify the mass scale used to compute the mass of a single physics body.
 *
 * @param p_newMassScale    The new mass scale.
 */
void                                PrimitiveComponent::SetMassScale                        (float const                        p_newMassScale /* = 1.0f */)
{
    if (BodyInstance* bodyInstance {GetBodyInstance()})
    {
        return bodyInstance->SetMassScale(p_newMassScale);
    }
}

/** 
 * Changes the value of bNotifyRigidBodyCollision.
 * @param bNewNotifyRigidBodyCollision - The value to assign to bNotifyRigidBodyCollision
 */
void                                PrimitiveComponent::SetNotifyRigidBodyCollision         (bool const                         p_newNotifyRigidBodyCollision)
{
    m_bodyInstance.SetNotifyRBCollision(p_newNotifyRigidBodyCollision);
    OnComponentCollisionSettingsChanged();
}

/**
 * Modifies the current PhysicalMaterialOverride of this component.
 *
 * @param p_newPhysicalMaterial The new physical material override of this component.
 */
void                                PrimitiveComponent::SetPhysicalMaterialOverride         (PhysicalMaterial*                  p_newPhysicalMaterial)
{
    m_bodyInstance.SetPhysicalMaterialOverride(p_newPhysicalMaterial);
}

/**
 * Sets wheter or not if the body of this component should use physics simulation, or should be kinematic.
 */
void                                PrimitiveComponent::SetSimulatePhysics                  (bool const                         p_simulate)
{
    m_bodyInstance.SetSimulatePhysics(p_simulate);
}

/**
 * Set whether this component should use Continuous COllision Detection.
 *
 * @param p_useCCD  True if this component should use CCD, otherwise false.
 */
void                                PrimitiveComponent::SetUseCCD                           (bool const                         p_useCCD)
{
    if (BodyInstance* bodyInstance {GetBodyInstance()})
    {
        bodyInstance->SetUseCCD(p_useCCD);
    }  
}

/**
 * Should this component create a physics state (Should we call CreatePhysicsState).
 *
 * @return True if we should create a physics state.
 */
bool                                PrimitiveComponent::ShouldCreatePhysicsState            ()  const
{
    return (m_alwaysCreatePhysicsState || m_bodyInstance.GetCollisionEnabled() != ECollisionEnabled::NoCollision);
}

/**
 * Wakes physics simulation of the body of this component.
 */
void                                PrimitiveComponent::WakeRigidBody                       ()
{
    m_bodyInstance.Wake();
}

// ==============================[Public Local Methods]============================== //

/** 
 *	Get the angular velocity of a body of this component, in radians per second. 
 */
Vector3                             PrimitiveComponent::GetAngularVelocityRad       ()  const
{
    if (BodyInstance* bodyInstance  {GetBodyInstance()})
    {
        return bodyInstance->GetAngularVelocityRad();
    }

    return Vector3::Zero;
}

/**
 * Gets a copy of the list of entities we currently ignore when moving.
 *
 * @return The copy of the list
 */
std::vector<Entity*>                PrimitiveComponent::GetCopyMoveIgnoreEntities   ()
{
    for (std::vector<Entity*>::iterator it {m_moveIgnoreEntities.begin()} ; it != m_moveIgnoreEntities.end() ; ++it)
    {
        if (*it == nullptr)
        {
            m_moveIgnoreEntities.erase(it);
        }
    }

    return m_moveIgnoreEntities;
}

/**
 * Gets a copy of the list of components we currently ignore when moving.
 *
 * @return The copy of the list
 */
std::vector<PrimitiveComponent*>    PrimitiveComponent::GetCopyMoveIgnoreComponents ()
{
    for (std::vector<PrimitiveComponent*>::iterator it {m_moveIgnoreComponents.begin()} ; it != m_moveIgnoreComponents.end() ; ++it)
    {
        if (*it == nullptr)
        {
            m_moveIgnoreComponents.erase(it);
        }
    }

    return m_moveIgnoreComponents;
}

/**
 * Gets the linear velocity of the body of this component.
 *
 * @return The linear velocity of the body of this component.
 */
Vector3                             PrimitiveComponent::GetLinearVelocity           ()  const
{
    if (BodyInstance* bodyInstance  {GetBodyInstance()})
    {
        return bodyInstance->GetLinearVelocity();
    }

    return Vector3::Zero;
}

/**
 *	Get the linear velocity of a point on the body of this component.
 *
 *	@param p_point			The point is specified in world space.
 */
Vector3                             PrimitiveComponent::GetLinearVelocityAtPoint    (Vector3 const&         p_point)    const
{
    if (BodyInstance* bodyInstance  {GetBodyInstance()})
    {
        return bodyInstance->GetVelocityAtPoint(p_point);
    }

    return Vector3::Zero; 
}

void                                PrimitiveComponent::BeginOverlap                (OverlapInfo const&     p_otherOverlap,
                                                                                     bool const             p_doNotifies)
{
    if (IsPendingKill())
    {
        return;
    }

    PrimitiveComponent* otherComponent {p_otherOverlap.m_overlapInfo.m_primitiveComponent};

    if (otherComponent)
    {
        // checks if both component are not already overlapping.
        auto otherCompIt {std::find(m_overlappingComponents.begin(), m_overlappingComponents.end(), p_otherOverlap)};
        if (otherCompIt == m_overlappingComponents.end() && CanGenerateOverlap(this, otherComponent))
        {
            Entity* const otherEntity   {otherComponent->GetOwner()};
            Entity* const myEntity      {GetOwner()};

            bool const notifyActorAreOverlapping {p_doNotifies && !AreEntitieOverlapping(*myEntity, *otherEntity)};

            m_overlappingComponents.push_back(p_otherOverlap);
            otherComponent->m_overlappingComponents.push_back(this);

            if (p_doNotifies)
            {
                if (notifyActorAreOverlapping)
                {
                    if (myEntity && !myEntity->IsPendingKill())
                    {
                        myEntity->NotifyBeginOverlap(otherEntity);
                    }

                    if (otherEntity && !otherEntity->IsPendingKill())
                    {
                        otherEntity->NotifyBeginOverlap(myEntity);
                    }
                }
            }
        }
    }
}

void                                PrimitiveComponent::EndOverlap                  (OverlapInfo const&     p_otherOverlap,
                                                                                     bool const             p_doNotifies /** = true */)
{
    PrimitiveComponent* otherComponent {p_otherOverlap.m_overlapInfo.m_primitiveComponent};
    if (otherComponent)
    {
        for (auto otherOverlappingCompIt = otherComponent->m_overlappingComponents.begin() ; otherOverlappingCompIt != otherComponent->m_overlappingComponents.end(); ++otherOverlappingCompIt)
        {
            if (this == otherOverlappingCompIt->m_overlapInfo.m_primitiveComponent)
            {
                otherComponent->m_overlappingComponents.erase(otherOverlappingCompIt);
                break;
            }
        }

        auto otherOverlapIt {std::find(m_overlappingComponents.begin(), m_overlappingComponents.end(), p_otherOverlap)};

        if (otherOverlapIt != m_overlappingComponents.end())
        {
            m_overlappingComponents.erase(otherOverlapIt);
            if (p_doNotifies)
            {
                Entity* const otherEntity   {otherComponent->GetOwner()};
                Entity* const myEntity      {GetOwner()};

                if (myEntity && !AreEntitieOverlapping(*myEntity, *otherEntity))
                {
                    if (!myEntity->IsPendingKill())
                    {
                        myEntity->NotifyEndOverlap(otherEntity);
                    }
                    
                    if (!otherEntity->IsPendingKill())
                    {
                        otherEntity->NotifyEndOverlap(myEntity);
                    }
                }
            }
        }
    }
}

/**
 * Tells to this component wheter to ignore collision with all components of a specific Actor when this component is moved.
 * Components on the other Actor may also need to be told to do the same when they move.
 * Does not affect movement of this component when simulating physics.
 *
 * @param p_actor           The actor to ignore or remove from the list.
 * @param p_shouldIgnore    True : Ignore collision whith the given actor / False : Remove the given actor of the move ignore actor list.
 */
void                                PrimitiveComponent::IgnoreEntityWhenMoving      (Entity*                p_entity,
                                                                                     bool const             p_shouldIgnore)
{
    if (p_entity != nullptr)
    {
        std::vector<Entity*>::iterator it    {std::find(m_moveIgnoreEntities.begin(), m_moveIgnoreEntities.end(), p_entity)};

        if (p_shouldIgnore == true)
        {
            if (it == m_moveIgnoreEntities.end())
            {
                // The actor doesn't exist in the array so we can add it //
                m_moveIgnoreEntities.push_back(p_entity);
            }
        }

        else
        {
            if (it != m_moveIgnoreEntities.end())
            {
                // This actor exist in the list //
                m_moveIgnoreEntities.erase(it);
            }
        }
    }
}

/**
 * Tells to this component wheter to ignore collision with another ocmponent when this component is moved.
 * The other components may also need to be told to do the same when they move.
 * Does not affect movement of this component when simulating physics.
 *
 * @param p_component       The component to ignore or remove from the list.
 * @param p_shouldIgnore    True : Ignore collision whith the given component / False : Remove the given component of the move ignore component list.
 */
void                                PrimitiveComponent::IgnoreComponentWhenMoving   (PrimitiveComponent*    p_component,
                                                                                     bool const             p_shouldIgnore)
{
    if (p_component != nullptr)
    {
        std::vector<PrimitiveComponent*>::iterator it   {std::find(m_moveIgnoreComponents.begin(), m_moveIgnoreComponents.end(), p_component)};

        if (p_shouldIgnore == true)
        {
            if (it == m_moveIgnoreComponents.end())
            {
                // The component doesn't exist in the array so we can add it //
                m_moveIgnoreComponents.push_back(p_component);
            }
        }

        else
        {
            if (it != m_moveIgnoreComponents.end())
            {
                // This component exist in the list //
                m_moveIgnoreComponents.erase(it);
            }
        }
    }
}

bool                                PrimitiveComponent::IsOverlapping               (Entity const*          p_other)    const
{
    if (p_other)
    {
        for (OverlapInfo const& overlapInfo : m_overlappingComponents)
        {
            PrimitiveComponent const* const primitiveComponent {overlapInfo.m_overlapInfo.m_primitiveComponent};

            if (primitiveComponent && primitiveComponent->GetOwner())
            {
                return true;
            }
        }
    }

    return false;
}

/**
 * Get uf thhe body of this component is currently awake and simulating.
 */
bool                                PrimitiveComponent::IsRigidBodyAwake            ()  const
{
    if (BodyInstance* bodyInstance {GetBodyInstance()})
    {
        return bodyInstance->IsAwake();
    }

    return false;   
}

/**
 * Force the body of this component back to sleep.
 */
void                                PrimitiveComponent::PutRigidBodyToSleep         ()
{
    if (BodyInstance* bodyInstance {GetBodyInstance()})
    {
        bodyInstance->PutToSleep();
    }
}

/**
 * Modifies the value of the culling distance.
 */
void                                PrimitiveComponent::SetCullDistance             (float const            p_newCullDistance)
{
    if (p_newCullDistance >= 0.0f && p_newCullDistance != m_maxDrawDistance)
    {
        m_maxDrawDistance = p_newCullDistance;
    }
}

void                                PrimitiveComponent::SetMaxAngularVelocityRad    (float const            p_newMaxAngularVelocityRad, 
                                                                                     bool const             p_addToCurrent /** = false */)
{
    if (BodyInstance* bodyInstance {GetBodyInstance()})
    {
        bodyInstance->SetMaxAngularVelocityRad(p_newMaxAngularVelocityRad, p_addToCurrent);
    }
}

/**
 * Enables / Disables whether this component should generate overlap event when it's overlap.
 *
 * @param p_generateOverlapEvents   True: Enables overlap event / False: Disables overlap event.
 */
void                                PrimitiveComponent::SetGenerateOverlapEvents    (bool const             p_generateOverlapEvents)
{
    if (m_generateOverlapEvents != p_generateOverlapEvents)
    {
        m_generateOverlapEvents = p_generateOverlapEvents;
    }
}

// ==============================[Virtual Protected Local Methods]============================== //

bool    PrimitiveComponent::MoveComponentImpl                   (Vector3 const&         p_deltaWorldOffset,
                                                                 Quaternion const&      p_newWorldRotation,
                                                                 bool const             p_sweep, 
                                                                 HitResult*             p_outHit /** = nullptr */)
{
    if (IsPendingKill())
    {
        if (p_outHit != nullptr)
        {
            *p_outHit = HitResult();
        }

        return false;
    }

    ConditionUpdateLocalToWorld();

    Vector3 const       sweepStart      {GetWorldLocation()};
    Vector3 const       sweepEnd        {sweepStart + p_deltaWorldOffset};
    float               deltaSizeSq     {p_deltaWorldOffset.GetMagnitudeSquared()};
    Quaternion const    initialRotation {GetWorldRotation()};

    HitResult blockingHit;
    blockingHit.m_isBlockingHit = false;
    blockingHit.m_hitTime       = 1.0f;

    std::vector<OverlapInfo>    pendingOverlaps;
    bool                        filledHitResult     {false};
    bool                        hasMoved            {false};
    bool                        includeOverlapAtEnd {false};
    bool                        rotationOnly        {false};

    if (!p_sweep)
    {
        hasMoved = InternalSetWorldLocationAndRotation(GetWorldLocation() + p_deltaWorldOffset, p_newWorldRotation);
        rotationOnly = (deltaSizeSq == 0.0f);
        includeOverlapAtEnd = rotationOnly && IsQueryCollisionEnabled() && Quaternion::IsSame(initialRotation, p_newWorldRotation);
    }

    else
    {
        std::vector<HitResult>  hits;
        bool const              isQueryCollisionEnabled {IsQueryCollisionEnabled()};
        Vector3                 newLocation             {sweepStart};

        if (isQueryCollisionEnabled && deltaSizeSq > 0)
        {
            World const* myWorld    {GetWorld()};

            // Init Qury Collision.
            CollisionResponseParams responseParams {m_bodyInstance.GetResponseToChannels()};
            CollisionQueryParams    queryParams;
            queryParams.AddIgnoredEntities(m_moveIgnoreEntities);
            queryParams.AddIgnoredComponents(m_moveIgnoreComponents);
            queryParams.m_ignoreTouchs |= !GetGenerateOverlapEvents();

            // Make queries test.
            bool hasBlockingHit   {false};
            if (IsZeroExtent())
            {
                hasBlockingHit = myWorld->LineTraceMultiByChannel(hits, sweepStart, sweepEnd, GetCollisionObjectType(), queryParams, responseParams);
            }

            else
            {
                hasBlockingHit = myWorld->SweepMultiByChannel(hits, sweepStart, sweepEnd, initialRotation, GetCollisionObjectType(), GetCollisionShape(), queryParams, responseParams);
            }

            if (hits.size() > 0)
            {
                float const deltaMagnitude {Math::Sqrt(deltaSizeSq)};
                for (HitResult& hit : hits)
                {
                    float const desiredTimeBack {Math::Clamp(0.00001f, 0.00001f / deltaMagnitude , 1.0f / deltaMagnitude) + 0.000001f};
                    hit.m_hitTime = Math::Clamp(hit.m_hitTime - desiredTimeBack, 0.0f, 1.0f);
                }
            }

            if (hasBlockingHit || GetGenerateOverlapEvents())
            {
                // If we have a blocking hit, set the blocking hit value and if we have overlaping value and we generate overlaps event, Add to the pending list.

                int32 blockingHitIndex  {-1};

                for (uint32 index {0u} ; index < hits.size() ; ++index)
                {
                    HitResult& testHit = hits[index];

                    if (testHit.m_isBlockingHit)
                    {
                        if (blockingHitIndex == -1)
                        {
                            blockingHitIndex = index;
                            break;
                        }
                    }

                    else if (GetGenerateOverlapEvents())
                    {
                        PrimitiveComponent* overlapComponent {testHit.m_primitiveComponent};
                        if (overlapComponent && overlapComponent->GetGenerateOverlapEvents())
                        {
                            if (blockingHitIndex >= 0 && testHit.m_hitTime >= hits[blockingHitIndex].m_hitTime)
                            {
                                break;
                            }
                            
                            else
                            {
                                pendingOverlaps.push_back(OverlapInfo(testHit, true));
                            }
                        }
                    }
                }

                // If we have blocking hit, update it
                if (blockingHitIndex >= 0)
                {
                    blockingHit = hits[blockingHitIndex];
                    filledHitResult = true;
                }

            }

            if (!blockingHit.m_isBlockingHit)
            {
                newLocation = sweepEnd;
            }

            else
            {
                newLocation             = sweepStart + (blockingHit.m_hitTime * p_deltaWorldOffset);
                blockingHit.m_location  = newLocation; 
            }

            includeOverlapAtEnd = Quaternion::IsSame(initialRotation, p_newWorldRotation);
        }

        else if (deltaSizeSq > 0)
        {
            newLocation += p_deltaWorldOffset;
            includeOverlapAtEnd = false;
        }

        else if (deltaSizeSq == 0 && isQueryCollisionEnabled)
        {
            includeOverlapAtEnd = Quaternion::IsSame(initialRotation, p_newWorldRotation);
            rotationOnly = true;
        }

        hasMoved = InternalSetWorldLocationAndRotation(newLocation, p_newWorldRotation);

        if (hasMoved)
        {
            UpdateOverlaps(&pendingOverlaps, true, nullptr);
        }
    }

    if (blockingHit.m_isBlockingHit && !IsPendingKill())
    {
        DispatchBlockingHit(*GetOwner(), blockingHit);
    }

    if (p_outHit)
    {
        if (filledHitResult)
        {
            *p_outHit = blockingHit;
        }

        else
        {
            *p_outHit = HitResult(sweepStart, sweepEnd);
        }
    }


    return hasMoved;
}

void    PrimitiveComponent::OnInitialize                        ()
{
    
}

void    PrimitiveComponent::OnComponentCollisionSettingsChanged ()
{
    UpdateOverlaps();
}

void    PrimitiveComponent::UpdateOverlapsImpl                  (std::vector<OverlapInfo> const*    p_newPendingOverlaps /** = nullptr */,
                                                                 bool const                         p_doNotifies /** = true */,
                                                                 std::vector<OverlapInfo> const*    p_overlapAtEndLocation /** = nullptr */)
{
    // If we generate overlap event and that query collision is enabled, we update the overlaps event list and notifies them if required.
    if (GetGenerateOverlapEvents() && IsQueryCollisionEnabled())
    {
        Entity* ownerEntity {GetOwner()};
        if (ownerEntity && ownerEntity->IsInitialized())
        {
            Transform const previousTransform   {GetWorldTransform()};
            bool const      ignoreChildren      {ownerEntity->GetRootComponent() == this};

            if (p_newPendingOverlaps != nullptr)
            {
                std::vector<OverlapInfo> const& newPendingOverlap {*p_newPendingOverlaps};

                for (OverlapInfo const& overlapInfo : newPendingOverlap)
                {
                    BeginOverlap(overlapInfo, p_doNotifies);
                }
            }

            std::vector<OverlapInfo> newOverlappingComponent;

            if (!IsPendingKill())
            {
                // If we have a pre-calculated overlap at end location, add it to the new overlapping component.
                if (p_overlapAtEndLocation != nullptr && Transform::IsSame(previousTransform,GetWorldTransform()))
                {
                    newOverlappingComponent = *p_overlapAtEndLocation;

                    std::remove_if(newOverlappingComponent.begin(), newOverlappingComponent.end(), PredicateFIlterCannotOverlap(*this));
                }

                // otherwise, compute the overlap component at the location of this.
                else
                {
                    World* world    {GetWorld()};
                    std::vector<OverlapResult> overlaps;

                    CollisionResponseParams responseParams {GetCollisionResponseToChannels()};
                    CollisionQueryParams queryParams;
                    queryParams.AddIgnoredEntities(m_moveIgnoreEntities);
                    queryParams.AddIgnoredComponents(m_moveIgnoreComponents);
                    queryParams.m_ignoreBlocks |= true;

                    world->OverlapMutliByChannel(overlaps, GetWorldLocation(), GetWorldRotation(), GetCollisionObjectType(), GetCollisionShape(), queryParams, responseParams);
                
                    for (OverlapResult const& result : overlaps)
                    {
                        PrimitiveComponent* const hitComponent {result.m_component};
                        if (hitComponent && hitComponent != this && hitComponent->GetGenerateOverlapEvents())
                        {
                            newOverlappingComponent.push_back(OverlapInfo(hitComponent));
                        }
                    }
                }
            }

            // filter to find old overlaping component to call EndOverlap and new overlaping component in order to call Begin Overlap.
            if (m_overlappingComponents.size() > 0)
            {
                std::vector<OverlapInfo> oldOverlappingComponents;

                oldOverlappingComponents = m_overlappingComponents;

                for (uint32 oldOverlapIndex {0u} ; oldOverlapIndex < oldOverlappingComponents.size() ; ++oldOverlapIndex)
                {
                    for (uint32 newOverlapIndex {0u} ; newOverlapIndex < newOverlappingComponent.size() ; ++newOverlapIndex)
                    {
                        OverlapInfo const& oldOverlap   {oldOverlappingComponents[oldOverlapIndex]};
                        OverlapInfo const& newOverlap   {newOverlappingComponent[newOverlapIndex]};

                        if (oldOverlap.m_overlapInfo.m_entity == newOverlap.m_overlapInfo.m_entity &&
                            oldOverlap.m_overlapInfo.m_primitiveComponent == newOverlap.m_overlapInfo.m_primitiveComponent)
                        {
                            oldOverlappingComponents.erase(oldOverlappingComponents.begin() + oldOverlapIndex);
                            newOverlappingComponent.erase(newOverlappingComponent.begin() + newOverlapIndex);
                        }
                    }
                }

                for (uint32 index {0u} ;  index < oldOverlappingComponents.size() ; ++index)
                {
                    OverlapInfo const& overlapInfo {oldOverlappingComponents[index]};

                    if (overlapInfo.m_overlapInfo.m_primitiveComponent)
                    {
                        EndOverlap(overlapInfo, p_doNotifies);
                    }

                    else
                    {
                        oldOverlappingComponents.erase(oldOverlappingComponents.begin() + index);
                    }
                }
            }
        
           for (auto it {newOverlappingComponent.begin()} ; it != newOverlappingComponent.end() ; ++it)
           {
                OverlapInfo const&  overlap = *it;
                BeginOverlap(overlap, p_doNotifies);
           }
        }
    }

    // In that case, we end all overlaps that exist.
    else
    {
        if (m_overlappingComponents.size() > 0)
        {

            ClearOverlaps(p_doNotifies);
        }
    }

    // Update childrens.
    std::vector<SceneComponent*> const& childs  {GetAttachChildrens()};
    for (SceneComponent* child : childs)
    {
        if (child)
        {
            child->UpdateOverlaps(nullptr, p_doNotifies, nullptr);
        }
    }
}

// ==============================[Protected Local Methods]============================== //

void    PrimitiveComponent::ClearOverlaps               (bool const             p_doNotifies)
{
    for (uint32 index {0u} ; index < m_overlappingComponents.size() ; ++index)
    {
        EndOverlap(m_overlappingComponents[index], p_doNotifies);
    }
}

/**
* Ensure physics state created.
* If the physics state is not create, it will recreate this one. Otherwise do nothing.
*/
void    PrimitiveComponent::EnsurePhysicsStateCreated   ()
{
    if (IsPhysicsStateCreated() != ShouldCreatePhysicsState())
    {
        RecreatePhysicsState();
    }
}

void    PrimitiveComponent::SendPhysicsTransform        (ETeleportType const    p_teleportType)
{
    m_bodyInstance.SetTransform(GetWorldTransform(), ETeleportType::TeleportPhysics);
    m_bodyInstance.UpdateBodyScale(GetWorldTransform().m_scale);
}