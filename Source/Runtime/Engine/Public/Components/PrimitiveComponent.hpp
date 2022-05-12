#ifndef __PRIMITIVE_COMPONENT_HPP__
#define __PRIMITIVE_COMPONENT_HPP__

#include "SceneComponent.hpp"

#include "PhysicsEngine/BodyInstance.hpp"

// ==============================[Forward Declaration]============================== //

class CollisionQueryParams;
class CollisionResponseParams;

// ================================================================================= //

class ENGINE_API PrimitiveComponent : public SceneComponent
{
	REFLECT(PrimitiveComponent)

    typedef SceneComponent Super;

    public:

    // ==============================[Public Static Methods]============================== //

        static bool CanGenerateOverlap  (PrimitiveComponent const* p_firstComponent,
                                         PrimitiveComponent const* p_secondComponent);

    // ==============================[Public Local Properties]============================== //

        /** ... */
		PROPERTY()
        bool                m_alwaysCreatePhysicsState;

        /** ... */
        float               m_boundScale;

        /** ... */
        bool                m_castDynamicShadow;

        /** Should the primitive component cast a shadow or not. */
        bool                m_castShadow;

        /** ... */
        bool                m_castShadowWhenHidden;

        /** ... */
        bool                m_castStaticShadow;

        /** ... */
        bool                m_ignoreRadialForce;

        /** ... */
        bool                m_ignoreRadialImpulse;

        /** If enabled, this obhect will not be culled by distance. */
        bool                m_neverDistanceCull;

        /** ... */
        bool                m_traceComplexCollision;


    // ==============================[Public Local Contructor + Destructor]============================== //
    
        PrimitiveComponent  ();
        
        PrimitiveComponent  (PrimitiveComponent const&  p_copy) = default;

        PrimitiveComponent  (PrimitiveComponent&&       p_move) = default;
        
        ~PrimitiveComponent ()                                  = default;

    // ==============================[Virtual Public Local Methods]============================== //

        virtual void                                AddAngularImpulseRad                (Vector3 const&                     p_impulseRad,
                                                                                         bool const                         p_velocityChange = false);

        virtual void                                AddForce                            (Vector3 const&                     p_force, 
                                                                                         bool const                         p_accelChange = false);

        virtual void                                AddForceAtLocation                  (Vector3 const&                     p_force, 
                                                                                         Vector3 const&                     p_location);

        virtual void                                AddForceAtLocalLocation             (Vector3 const&                     p_force, 
                                                                                         Vector3 const&                     p_location);

        virtual void                                AddImpulse                          (Vector3 const&                     p_impulse,
                                                                                         bool const                         p_velocityChange = false);

        virtual void                                AddImpulseAtLocation                (Vector3 const&                     p_impulse,
                                                                                         Vector3 const&                     p_location);

        virtual void                                AddImpulseAtLocalLocation           (Vector3 const&                     p_impulse,
                                                                                         Vector3 const&                     p_location);

        virtual void                                AddRadialForce                      (Vector3 const&                     p_origin, 
                                                                                         float const                        p_radius, 
                                                                                         float const                        p_strength, 
                                                                                         ERadialImpulseFalloff              p_falloff, 
                                                                                         bool  const                        p_accelChange = false);

	    virtual void                                AddRadialImpulse                    (Vector3 const&                     p_origin, 
                                                                                         float const                        p_radius, 
                                                                                         float const                        p_strength, 
                                                                                         ERadialImpulseFalloff              p_falloff, 
                                                                                         bool const                         p_velocityChange = false);
        
        virtual void                                AddTorqueRad                        (Vector3 const&                     p_torque, 
                                                                                         bool const                         p_accelChange = false);

        virtual float                               GetAngularDamping                   ()                                                                  const;
        
        virtual BodyInstance*                       GetBodyInstance                     ()                                                                  const;

        virtual ECollisionChannel                   GetCollisionObjectType              ()                                                                  const override;

        virtual ECollisionEnabled                   GetCollisionEnabled                 ()                                                                  const override;

        virtual ECollisionResponse                  GetCollisionResponseToChannel       (ECollisionChannel const            p_channel)                      const override;

        virtual CollisionResponseContainer const&   GetCollisionResponseToChannels      ()                                                                  const override;

        virtual CollisionShape                      GetCollisionShape                   ()                                                                  const;

        virtual EGeometryType                       GetGeometryType                     ()                                                                  const;

        virtual Vector3                             GetInertiaTensor                    ()                                                                  const;

        virtual float                               GetLinearDamping                    ()                                                                  const;

        virtual float                               GetMass                             ()                                                                  const;

        virtual float                               GetMassScale                        ()                                                                  const;

        virtual int32                               GetNumMatrials                      ()                                                                  const;

        virtual Matrix4x4                           GetRenderMatrix                     ()                                                                  const;

//      virtual bool                                GetSquaredDistanceToCollision       (Vector3 const&                     p_point, 
//                                                                                       float&                             p_outSquaredDistance, 
//   

        virtual bool                                HasValidPhysicsState                ()                                                                  const override;

        virtual void                                InitSweepCollisionParams            (CollisionQueryParams&              p_outQueryParams,
                                                                                         CollisionResponseParams&           p_outResponseParams)            const;

        virtual bool                                IsGravityEnabled                    ()                                                                  const;
  
        virtual bool                                IsSimulatingPhysics                 ()                                                                  const override;

        virtual bool                                IsZeroExtent                        ()                                                                  const;

//      virtual bool                                LineTraceComponent                  (HitResult&                         p_outHit, 
//                                                                                       Vector3 const&                     p_start, 
//                                                                                       Vector3 const&                     p_end, 
//                                                                                       CollisionQueryParams const&        p_params);

        //virtual void                                OnComponentDestroyed                (bool const                         p_destroyingHierarchy)          override;

        virtual void                                OnCreatePhysicsState                ()                                                                  override;

        virtual void                                OnDestroyPhysicsState               ()                                                                  override;

        virtual void                                OnEntityEnableCollisionChanged      ()                                                                  override;

        virtual void                                OnUpdateTransform                   (EUpdateTransformFlags const        p_updateTransformFlags,
                                                                                         ETeleportType const                p_teleportType = ETeleportType::None)   override;

        virtual void                                SetAngularDamping                   (float const                        p_damping);

        virtual void                                SetAngularVelocityRad               (Vector3 const&                     p_newAngularVelocityRad,
                                                                                         bool const                         p_addToCurrent = false);
                                                                                            
        virtual void                                SetCollisionEnabled                 (ECollisionEnabled const            p_newType);

        virtual void                                SetCollisionObjectType              (ECollisionChannel const            p_channel);

        virtual void                                SetCollisionResponseToAllChannels   (ECollisionResponse const              p_newResponse);
        
        virtual void                                SetCollisionResponseToChannel       (ECollisionChannel const            p_channel, 
                                                                                         ECollisionResponse const           p_newResponse);

        virtual void                                SetCollisionResponseToChannels      (CollisionResponseContainer const&  p_newResponses);

        virtual void                                SetEnableGravity                    (bool const                         p_gravityEnabled);

        virtual void                                SetLinearDamping                    (float const                        p_damping);

        virtual void                                SetLinearVelocity                   (Vector3 const&                     p_newLinearVelocity,
                                                                                         bool const                         p_addToCurrent = false);

        virtual void                                SetMassOverrideInKg                 (float const                        p_massInKg = 1.f, 
                                                                                         bool const                         p_overrideMass = true);

        virtual void                                SetMassScale                        (float const                        p_massScale = 1.0f);

        virtual void                                SetNotifyRigidBodyCollision         (bool const                         p_newNotifyRigidBodyCollision);

        virtual void                                SetSimulatePhysics                  (bool const                         p_simulate);

        virtual void                                SetUseCCD                           (bool const                         p_useCCD);

        virtual bool                                ShouldCreatePhysicsState            ()                                                                  const override;

        virtual void                                SetPhysicalMaterialOverride         (PhysicalMaterial*                  p_newPhysicalMaterial);

//      virtual bool                    SweepComponent                  ();
//
        virtual void                                WakeRigidBody                       ();

    // ==============================[Public Local Methods]============================== //

        FORCEINLINE void                                    AddAngularImpulseDeg            (Vector3 const&                     p_angularImpulseDeg,
                                                                                             bool                               p_velchange = false);   

        FORCEINLINE void                                    AddTorqueDeg                    (Vector3 const&                     p_torqueDeg, 
                                                                                             bool                               p_accelChange = false);

        void                                                BeginOverlap                    (OverlapInfo const&                 p_otherOverlap,
                                                                                             bool const                         p_doNotifies);


//      bool                                                BoxOverlapComponent             (Vector3 const&                     p_inBoxCentre, 
//                                                                                           Box const&                         p_inBox, 
//                                                                                           bool const                         p_traceComplex, 
//                                                                                           bool const                         p_showTrace, 
//                                                                                           bool const                         p_persistentShowTrace, 
//                                                                                           Vector3&                           p_hitLocation, 
//                                                                                           Vector3&                           p_hitNormal, 
//                                                                                           /*FName&                           p_boneName,*/ 
//                                                                                           HitResult&                         p_outHit);

        FORCEINLINE void                                    ClearMoveIgnoreEntities         ();

        FORCEINLINE void                                    ClearMoveIgnoreComponents       ();

//      bool                                                ComponentOverlapComponent       ();

//      bool                                                ComponentOverlapComponent       ();

        void                                                DispatchBlockingHit             (Entity&                            p_owner, 
                                                                                             HitResult const&                   p_blockingHit);

//      void                                                DispatchWakeEvents              (/*ESleepEvent                      p_wakeEvent, 
//                                                                                           FName                              p_boneName*/);

        void                                                EndOverlap                      (OverlapInfo const&                 p_otherOverlap,
                                                                                             bool const                         p_doNotifies = true);

        FORCEINLINE Vector3                                 GetAngularVelocityDeg           ()                                                              const;

        Vector3                                             GetAngularVelocityRad           ()                                                              const;

//      float                                               GetDistanceToCollision          (Vector3 const&                     p_point,
//                                                                                           Vector3&                           p_closestPointOnCollision)  const;

//      Vector3                                             GetCenterOfMass                 ()                                                              const;

        std::vector<Entity*>                                GetCopyMoveIgnoreEntities       ();

        std::vector<PrimitiveComponent*>                    GetCopyMoveIgnoreComponents     ();

        FORCEINLINE bool                                    GetGenerateOverlapEvents        ()                                                              const;

        Vector3                                             GetLinearVelocity               ()                                                              const;

        Vector3                                             GetLinearVelocityAtPoint        (Vector3 const&                     p_point)                    const;


//      FORCEINLINE MaskFilter                              GetMaskFilterOnBodyInstance     ()                                                              const;

        FORCEINLINE float                                   GetMaxDrawDistance              ()                                                              const;

        FORCEINLINE float                                   GetMinDrawDistance              ()                                                              const;

        FORCEINLINE std::vector<Entity*> const&             GetMoveIgnoreEntities           ()                                                              const;

        FORCEINLINE std::vector<PrimitiveComponent*> const& GetMoveIgnoreComponents         ()                                                              const;

//      void                                                GetOverlappingActors            (std::vector<Actor*>&               p_outOverlappingActor)      const;

//      void                                                GetOverlappingComponents        (std::vector<PrimitiveComponent>&   p_outOverlappingComponents) const;

//      bool                                                GetOverlapsWithActor            (Actor const*                       p_actor, 
//                                                                                           std::vector<OverlapInfo>&          p_outOverlaps)              const;

        std::vector<OverlapInfo> const&                     GetOverlapInfos                 ()                                                              const;


        void                                                IgnoreEntityWhenMoving          (Entity*                            p_entity,
                                                                                             bool const                         p_shouldIgnore);

        void                                                IgnoreComponentWhenMoving       (PrimitiveComponent*                p_component,
                                                                                             bool const                         p_shouldIgnore);

//      bool                                                IsComponentIndividuallySelected ()                                                              const;

        bool                                                IsOverlapping                   (Entity const*                      p_otherActor)               const;

//      bool                                                IsOverlappingComponent          (PrimitiveComponent const*          p_otherComponent)           const;

//      bool                                                IsOverlappingComponent          (OverlapInfo const&                 p_overlapInfo)              const;

        bool                                                IsRigidBodyAwake                ()                                                              const;

//      bool                                                LineTraceComponent              (Vector3 const&                     p_traceStart, 
//                                                                                           Vector3 const&                     p_traceEnd, 
//                                                                                           bool const                         p_traceComplex, 
//                                                                                           bool const                         p_showTrace, 
//                                                                                           bool const                         p_persistentShowTrace, 
//                                                                                           Vector3&                           p_hitLocation, 
//                                                                                           Vector3&                           p_hitNormal, 
//                                                                                           /*FName&                           p_boneName,*/ 
//                                                                                           HitResult&                         p_outHit);

        void                                                PutRigidBodyToSleep             ();

        FORCEINLINE void                                    SetAngularVelocityDeg           (Vector3 const&                     p_newAngularVelocityDeg,
                                                                                             bool const                         p_addToCurrent = false);

//      void                                                SetBoundScale                   (float const                        p_newBoundScale = 1.0f);

//      void                                                SetCastShadow                   (bool const                         p_newCastShadow);

//      void                                                SetCenterOfMass                 (Vector3 const&                     p_newCenterofMass
//                                                                                           /*FName                            p_boneName = NAME_None*/);
//
        void                                                SetCullDistance                 (float const                        p_newCullDistance);

        void                                                SetGenerateOverlapEvents        (bool const                         p_generateOverlapEvents);
//
//      void                                                SetMaskFilterOnBodyInstance     (MaskFilter const                   p_newMaskFilter);

        FORCEINLINE void                                    SetMaxAngularVelocityDeg        (float const                        p_newMaxAngularVelocityDeg,
                                                                                             bool const                         p_addToCurrent = false);

        void                                                SetMaxAngularVelocityRad        (float const                        p_newMaxAngularVelocityRad, 
                                                                                             bool const                         p_addToCurrent = false);

        FORCEINLINE void                                    SetMinDrawDistance              (float const                        p_newMinDrawDistance);

//      void                                                SetOnlyOwnerSee                 (bool const                         p_newOnlyOwnerSee);

//      void                                                SetOwnerNoSee                   (bool const                         p_newOwnerNoSee);

//      void                                                SetRigidBodyReplicatedTarget    (RigidBodyState const&              p_updatedState
//                                                                                           /*const FName                      p_boneName = NAME_None*/);

//      bool                                                ShouldComponentAddToScene       ()                                                              const;

//      bool                                                SphereOverlapComponent          (Vector3 const&                     p_inSphereCentre, 
//                                                                                           float const                        p_inSphereRadius, 
//                                                                                           bool const                         p_traceComplex, 
//                                                                                           bool const                         p_showTrace, 
//                                                                                           bool const                         p_persistentShowTrace, 
//                                                                                           Vector3&                           p_hitLocation, 
//                                                                                           Vector3&                           p_hitNormal, 
//                                                                                           /*Name&                            p_boneName,*/ 
//                                                                                           HitResult&                         p_outHit);

//      bool                                                SphereTraceComponent            (Vector3 const&                     p_traceStart, 
//                                                                                           Vector3 const&                     p_traceEnd, 
//                                                                                           float const                        p_sphereRadius, 
//                                                                                           bool const                         p_traceComplex, 
//                                                                                           bool const                         p_showTrace, 
//                                                                                           bool const                         p_persistentShowTrace, 
//                                                                                           Vector3&                           p_hitLocation, 
//                                                                                           Vector3&                           p_hitNormal, 
//                                                                                           /*FName&                           p_boneName,*/ 
//                                                                                           HitResult&                         p_outHit);

    // ==============================[Public Local Operators]============================== //

        PrimitiveComponent& operator=   (PrimitiveComponent const&  p_copy) = default;

    protected:

    // ==============================[Protected Local Properties]============================== //

        /** 
         * The maximum draw distance of this component used for culling 
         * The distance is measured in world space unit from the center of the primitive's bounding sphere to the camera position.
         */
        float                               m_maxDrawDistance;

        /**
         * The minimum distance at which the primitive should be rendered.
         * The distance is measured in world space unit from the center of the primitive's bounding sphere to the camera position.
         */
        float                               m_minDrawDistance;

        /** ... */
        std::vector<Entity*>                m_moveIgnoreEntities;

        /** ... */
        std::vector<PrimitiveComponent*>    m_moveIgnoreComponents;

        /** Physics scene information for this component, holds a single rigid body with multiples shapes */
        PROPERTY()
        BodyInstance                        m_bodyInstance;

    // ==============================[Virtual Protected Local Methods]============================== //

        //virtual bool            ComponentOverlapComponentImpl   ();

        virtual bool            MoveComponentImpl               (Vector3 const&         p_deltaWorldOffset,
                                                                 Quaternion const&      p_newWorldRotation,
                                                                 bool const             p_sweep, 
                                                                 HitResult*             p_outHit = nullptr)   override;

        //virtual void            OnActorEnableCollisionChanged   ()                                                          override;

        //virtual void            OnAttachmentChanged             ()                                                          override;

        virtual void            OnInitialize                        ();

        virtual void            OnComponentCollisionSettingsChanged ();
        
        virtual void            UpdateOverlapsImpl              (std::vector<OverlapInfo> const*    p_newPendingOverlaps = nullptr,
                                                                 bool const                         p_doNotifies = true,
                                                                 std::vector<OverlapInfo> const*    p_overlapAtEndLocation = nullptr)   override;


    // ==============================[Protected Local Methods]============================== //
    
        void    ClearOverlaps               (bool const             p_doNotifies);

        void    EnsurePhysicsStateCreated   ();

        void    SendPhysicsTransform        (ETeleportType const    p_teleport);

    private:

    // ==============================[Private Local Properties]============================== //
        
        /** ... */
		PROPERTY()
        bool                        m_generateOverlapEvents;

        /** ... */
        std::vector<OverlapInfo>    m_overlappingComponents;

}; // !class PrimitiveComponent

#include "PrimitiveComponent.inl"
#include "PrimitiveComponent.generated.hpp"

#endif // !__PRIMITIVE_COMPONENT_HPP__
