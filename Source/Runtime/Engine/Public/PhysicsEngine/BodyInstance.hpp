#ifndef __BODY_INSTANCE_HPP__
#define __BODY_INSTANCE_HPP__

#include "CoreMinimal.hpp"


#include "BodyCollisionFilterData.hpp"
#include "BodyCollisionData.hpp"
#include "BodyCollisionFlags.hpp"
#include "CollisionResponseContainer.hpp"
#include "EngineTypes.hpp"
#include "PhysicsActorHandle.hpp"

// ==============================[Forward Declaration]============================== //

class PhysicalMaterial; // To Create
class PhysicsScene; // To Create

class PrimitiveComponent;
class PhysicsSceneHandle;

// ================================================================================= //

/**
 * Description : TODO !
 */
class ENGINE_API BodyInstance
{
    REFLECT(BodyInstance)

    public:

    // ==============================[Public Local Properties]============================== //

        /** Continuous Collision Detection */
        bool                m_useCCD;

        /** ... */
        bool                m_contactModification;

        /** ... */
        bool                m_startAwake;

        /** ... */
        PROPERTY()
        bool                m_generateWakeEvent;

        /** ... */
        bool                m_updateMassWhenScaleChanges;

        /** When a Locked Axis Mode is selected, will lock translation on the specified axis*/
//	    bool                m_lockTranslation;
	    
	    /** When a Locked Axis Mode is selected, will lock rotation to the specified axis*/
//	    bool                m_lockRotation;

	    /** Lock translation along the X-axis*/
//	    bool                m_lockXTranslation;

	    /** Lock translation along the Y-axis*/
//	    bool                m_lockYTranslation;

	    /** Lock translation along the Z-axis*/
//	    bool                m_lockZTranslation;

        /** Lock rotation about the X-axis*/
//      bool                m_lockXRotation;

	    /** Lock rotation about the Y-axis*/
//	    bool                m_lockYRotation;

	    /** Lock rotation about the Z-axis*/
//	    bool                m_lockZRotation;

        /** ... */
        bool                m_overrideMaxAngularVelocity;

        /** ... */
        Vector3             m_inertiaTensorScale;

        /** ... */
        float               m_maxAngularVelocity;

        /** ... */
        float               m_customSleepTresholdMultiplier;

        /** ... */
        float               m_stabilizationTresholdMultiplier;

        PrimitiveComponent* m_ownerComponent;

        /** ... */
        PROPERTY()
        bool                m_simulatePhysics;


    // ==============================[Public Local Properties]============================== //

        BodyInstance    ();

        BodyInstance    (BodyInstance const&    p_copy) = default;

        BodyInstance    (BodyInstance&&         p_move) = default;

        ~BodyInstance   ()                              = default;

    // ==============================[Public Local Methods]============================== //

        void                                            AddAngularImpulseRad        (Vector3 const                      p_angularImpulseRad,
                                                                                     bool const                         p_velocityChange = false);

        void                                            AddForce                    (Vector3 const&                     p_force, 
                                                                                     bool const                         p_accelChange = false);

	    void                                            AddForceAtLocation          (Vector3 const&                     p_force, 
                                                                                     Vector3 const&                     p_position);

	    void                                            AddForceAtLocalLocation     (Vector3 const&                     p_force, 
                                                                                     Vector3 const&                     p_position);

        void                                            AddImpulse                  (Vector3 const&                     p_impulse,
                                                                                     bool                               p_velocityChange = false);

        void                                            AddImpulseAtLocation        (Vector3 const&                     p_impulse,
                                                                                     Vector3 const&                     p_position);

        void                                            AddImpulseAtLocalLocation   (Vector3 const&                     p_impulse,
                                                                                     Vector3 const&                     p_position);

	    void                                            AddRadialForce              (Vector3 const&                     p_origin, 
                                                                                     float const                        p_radius, 
                                                                                     float const                        p_strength, 
                                                                                     ERadialImpulseFalloff const        p_falloff,
                                                                                     bool const                         p_accelChange = false);

	    void                                            AddRadialImpulse            (Vector3 const&                     p_origin, 
                                                                                     float const                        p_radius, 
                                                                                     float const                        p_strength, 
                                                                                     ERadialImpulseFalloff const        p_falloff, 
                                                                                     bool const                         p_velocityChange = false);

        void                                            AddTorqueRad                (Vector3 const&                     p_torque,
                                                                                     bool const                         p_accelChange);

        void                                            BuildBodyCollisionFlags     (BodyCollisionFlags&                p_outBodyCollisionFlags);

        void                                            BuildBodyFilterData         (BodyCollisionFilterData&           p_outBodyFilterData)                        const;

        void                                            ClearForces                 ();

        FORCEINLINE float                               GetAngularDamping           ()                                                                              const;

        Vector3                                         GetAngularVelocityRad       ()                                                                              const;

        Bounds                                          GetBodyBounds               ()                                                                              const;

        Vector3                                         GetBodyInertiaTensor        ()                                                                              const;                                  

        float                                           GetBodyMass                 ()                                                                              const;

        FORCEINLINE ECollisionEnabled                   GetCollisionEnabled         (bool const                         p_checkOwner = true)                        const;

        FORCEINLINE float                               GetLinearDamping            ()                                                                              const;

        Vector3                                         GetLinearVelocity           ()                                                                              const;

        FORCEINLINE float                               GetMassOverride             ()                                                                              const;

        FORCEINLINE float                               GetMassScale                ()                                                                              const;

	    float                                           GetMaxAngularVelocityRad    ()                                                                              const;

        FORCEINLINE ECollisionChannel                   GetObjectType               ()                                                                              const;
        
        PhysicsActorHandle*                             GetPhysicsActorHandle       ();

        PhysicsScene*                                   GetPhysicsScene             ()                                                                              const;

        FORCEINLINE ECollisionResponse                  GetResponseToChannel        (ECollisionChannel const            p_channel)                                  const;

        FORCEINLINE CollisionResponseContainer const&   GetResponseToChannels       ()                                                                              const;

        Vector3                                         GetVelocityAtPoint          (Vector3 const&                     p_point)                                    const;

        void                                            Initialize                  (Transform const&                   p_transform,
                                                                                     PrimitiveComponent*                p_ownerComponent,
                                                                                     PhysicsSceneHandle*                p_physicsScene);

        bool                                            IsAwake                     ()                                                                              const;

        bool                                            IsDynamic                   ()                                                                              const;

        FORCEINLINE bool                                IsGravityEnabled            ()                                                                              const;

        bool                                            IsNonKinematic              ()                                                                              const;

        bool                                            IsNotifyRigidBodyCollision  ()                                                                              const;

        FORCEINLINE bool                                IsSimulatingPhysics         ()                                                                              const;

        FORCEINLINE bool                                IsValidBodyInstance         ()                                                                              const;


        void                                            PutToSleep                  ();

        void                                            Release                     ();

        void                                            ReplaceResponseToChannels   (ECollisionResponse const           p_oldResponse,
                                                                                     ECollisionResponse const           p_newResponse);

        void                                            SetAngularDamping           (float const                        p_newAngularDamping);

        void                                            SetAngularVelocityRad       (Vector3 const&                     p_newAngularVelocityRad,
                                                                                     bool const                         p_addToCurrent,
                                                                                     bool const                         p_autoWake = true);
        
        void                                            SetCollisionEnabled         (ECollisionEnabled const            p_newTyape,
                                                                                     bool const                         p_updatePhysicsFilterData = true);
        

        void                                            SetEnableGravity            (bool const                         p_gravityEnabled);


        void                                            SetLinearDamping            (float const                        p_newLinearDamping);

        void                                            SetLinearVelocity           (Vector3 const&                     p_newVelocity,
                                                                                     bool const                         p_addToCurrent,
                                                                                     bool const                         p_autoWake = true);

        void                                            SetMassOverride             (float const                        p_newMassInKg,
                                                                                     bool const                         p_newOverrideMass = true); // In Kg

        FORCEINLINE void                                SetMassScale                (float const                        p_newMassScale = 1.0f);

        void                                            SetMaxAngularVelocityRad    (float                              p_newMaxAngularVelocityRad, 
                                                                                     bool const                         p_addToCurrent, 
                                                                                     bool const                         p_updateOverrideMaxAngularVelocity = true);

        void                                            SetNotifyRBCollision        (bool const                         p_newNotifyRBCollision);

        FORCEINLINE void                                SetObjectType               (ECollisionChannel const            p_channel);

        void                                            SetPhysicalMaterialOverride (PhysicalMaterial*                  p_newPhysicalMaterial);

        void                                            SetPhysicsActorHandle       (PhysicsActorHandle const&          p_newPhysicsActorHandle);

        void                                            SetResponseToAllChannels    (ECollisionResponse const           p_newResponse);

        void                                            SetResponseToChannel        (ECollisionChannel const            p_channel, 
                                                                                     ECollisionResponse const           p_newResponse);

        void                                            SetResponseToChannels       (CollisionResponseContainer const&  p_newResponses);

        void                                            SetSimulatePhysics          (bool const                         p_shouldSimulate);

        void                                            SetTransform                (Transform const&                   p_newTransform,
                                                                                     ETeleportType const                p_teleportType,
                                                                                     bool const                         p_autoWake = true);

	    void                                            SetUseCCD                   (bool                               p_useCCD);

        FORCEINLINE bool                                ShouldSimulatingPhysics     ()                                                                          const;

        bool                                            UpdateBodyScale             (Vector3 const&                     p_newScale,
                                                                                     bool const                         p_forceUpdate = false);

        void                                            UpdateMassProperties        ();

        void                                            UpdatePhysicalMaterials     ();

        void                                            UpdatePhysicsFilterData     ();

        void                                            UpdateSimulatePhysics       ();

        void                                            Wake                        ();



        bool                                            GetSquaredDistanceToBody    (Vector3 const&                     p_point, 
                                                                                     float&                             p_outDistanceSquared, 
                                                                                     Vector3&                           p_outPointOnBody) const;

	    float                                           GetDistanceToBody           (Vector3 const&                     p_point, 
                                                                                     Vector3&                           p_outPointOnBody) const;

        void                                            SetContactModification      (bool                               p_newContactModification);

        BodyInstance&   operator=   (BodyInstance const&) = default;


    protected:
    
    // ==============================[Protected Local Properties]============================== //

        /** Internal physics representation of our body instance*/
        PhysicsActorHandle  m_actorHandle;

        /** ... */
        float               m_angularDamping;

        /** ... */
        float               m_massInKgOverride;

        /** ... */
        float               m_massScale;

        /** ... */
        float               m_linearDamping;

        /** Should we called 'Hit' events when this object collides during physics simulation*/
        bool                m_notifyRigidBodyCollision;

        /** ... */
        bool                m_overrideMass;

        /** ... */
        PhysicalMaterial*   m_physicalMaterialOverride;

        Vector3             m_scale;

    private:

    // ==============================[Private Local Properties]============================== //

        /** ... */
        ECollisionChannel           m_objectType;

        /** Type of collision */
        ECollisionEnabled           m_collisionEnabled;

        /** ... */
        CollisionResponseContainer  m_collisionResponses;

        /** ... */
        PROPERTY()
        bool                        m_enableGravity;

    // ==============================[Privaye Local Methods]============================== //
    
        /** ... */
        ECollisionEnabled   GetCollisionEnabledCheckingOwner    ()  const;

}; // !class BodyInstance

#include "BodyInstance.inl"
#include "BodyInstance.generated.hpp"

#endif // !__BODY_INSTANCE_HPP__

