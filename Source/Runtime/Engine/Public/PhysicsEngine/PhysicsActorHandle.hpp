#ifndef __PHYSICS_ACTOR_HANDLE_HPP__
#define __PHYSICS_ACTOR_HANDLE_HPP__

#include "CoreMinimal.hpp"

#include "PxPhysicsAPI.h"
#include "PhysXTypeConverter.hpp"

#include "EngineTypes.hpp"

class PhysicsScene;
class PhysicsShapeHandle;

/**
 * Struture used as interface for physx::Actor.
 */
struct ENGINE_API PhysicsActorHandle
{
    // ==============================[Public Local Properties]============================== //
    
        physx::PxRigidActor*    m_actor;

    // ==============================[Public Constructor + Destructor]============================== //

        PhysicsActorHandle  ();

        PhysicsActorHandle  (physx::PxRigidActor*       p_actor);

        PhysicsActorHandle  (PhysicsActorHandle const&  p_copy);

        PhysicsActorHandle  (PhysicsActorHandle&&       p_move);

        ~PhysicsActorHandle ()                                      = default;

    // ==============================[Public Local Methods]============================== //

        void                                AddAngularImpulse               (Vector3 const&                     p_angularImpulse,
                                                                             bool const                         p_velocityChange);

        void                                AddForce                        (Vector3 const&                     p_force,
                                                                             bool const                         p_accelerationChange = false);

        void                                AddForceAtLocation              (Vector3 const&                     p_force,
                                                                             Vector3 const&                     p_location);

        void                                AddForceAtLocalLocation         (Vector3 const&                     p_force,
                                                                             Vector3 const&                     p_location);

        void                                AddImpulse                      (Vector3 const&                     p_impulse,
                                                                             bool const                         p_velocityChange = false);

        void                                AddImpulseAtLocation            (Vector3 const&                     p_impulse,
                                                                             Vector3 const&                     p_location);

        void                                AddImpulseAtLocalLocation       (Vector3 const&                     p_impulse,
                                                                             Vector3 const&                     p_location);

        void                                AddRadialForce                  (Vector3 const&                     p_origin, 
                                                                             float const                        p_radius, 
                                                                             float const                        p_strength, 
                                                                             ERadialImpulseFalloff const        p_falloff,
                                                                             bool const                         p_accelChange = false);

        void                                AddRadialImpulse                (Vector3 const&                     p_origin,
                                                                             float const                        p_radius,
                                                                             float const                        p_strength,
                                                                             ERadialImpulseFalloff const        p_falloff,
                                                                             bool const                         p_velocityChange);

        void                                AddTorque                       (Vector3 const&                     p_torque,
                                                                             bool const                         p_accelChange = false);

        void                                AttachShape                     (PhysicsShapeHandle const&          p_shapeHandle);

        bool                                CanSimulate                     ()                                                                  const;

        void                                DetachShape                     (PhysicsShapeHandle const&          p_shapeHandle,                  
                                                                             bool const                         p_wakeOnLostTouch = true);

        float                               GetAngularDamping               ()                                                                  const;

        Vector3                             GetAngularVelocity              ()                                                                  const;

        Bounds                              GetBounds                       ()                                                                  const;

        Transform                           GetGlobalPose                   ()                                                                  const;

        Transform                           GetKinematicTarget              ()                                                                  const;

        float                               GetLinearDamping                ()                                                                  const;

        Vector3                             GetLinearVelocity               ()                                                                  const;

        Vector3                             GetLocalIntertiaTensor          ()                                                                  const;

        float                               GetMass                         ()                                                                  const;

        FORCEINLINE float                   GetMaxAngularVelocityDeg        ()                                                                  const;

        float                               GetMaxAngularVelocityRad        ()                                                                  const;

        float                               GetMaxDepenetrationVelocity     ()                                                                  const;

        int32                               GetNumShapes                    ()                                                                  const;

        FORCEINLINE physx::PxRigidBody*     GetPxRigidBody                  ()                                                                  const;

        FORCEINLINE physx::PxRigidDynamic*  GetPxRigidDynamic               ()                                                                  const;

        bool                                GetRigidBodyFlag                (physx::PxRigidBodyFlag::Enum const p_rigidBodyFlag)                const;

        PhysicsScene*                       GetScene                        ()                                                                  const;

        void                                GetShapes                       (std::vector<PhysicsShapeHandle>&   p_outShapes)                    const;

        Vector3                             GetVelocityAtPoint              (Vector3 const&                     p_point)                        const;

        float                               GetWakeCounter                  ()                                                                  const;

        bool                                HasKinematicTarget              ()                                                                  const;

        bool                                IsCCDEnabled                    ()                                                                  const;

        bool                                IsDynamic                       ()                                                                  const;
    
        bool                                IsGravityEnabled                ()                                                                  const;

        bool                                IsInScene                       ()                                                                  const;

        bool                                IsKinematic                     ()                                                                  const;

        bool                                IsRigidBody                     ()                                                                  const;
        
        bool                                IsSleeping                      ()                                                                  const;
        
        bool                                IsStatic                        ()                                                                  const;
        
        bool                                IsValid                         ()                                                                  const;
        
        void                                PutToSleep                      ()                                                                  const;

        void                                Release                         ();

        void                                SetAngularDamping               (float const                        p_angularDamping);

        void                                SetAngularVelocity              (Vector3 const&                     p_newAngularVelocity,
                                                                             bool const                         p_autoWake = true);

        void                                SetCCDEnabled                   (bool const                         p_isCCDEnabled);

        void                                SetCenterOfMassLocaPose         (Transform const&                   p_newCenterOfMassLocaPose);

        void                                SetGlobalPose                   (Transform const&                   p_globalPose,
                                                                             bool const                         p_autoWake = true);

        void                                SetGravityEnabled               (bool const                         p_enabled);

        void                                SetIsKinematic                  (bool const                         p_isKinematic);

        void                                SetKinematicTarget              (Transform const&                   p_newTarget);

        void                                SetLinearDamping                (float const                        p_linearDamping);

        void                                SetLinearVelocity               (Vector3 const&                     p_newLinearVelocity,
                                                                             bool const                         p_autoWake = true);

        void                                SetMass                         (float const                        p_newMass);

        void                                SetMassSpaceInertiaTensor       (Vector3 const&                     p_newMassSpaceInertiaTensor);


        void                                SetMaxAngularVelocity           (float const                        p_newMaxAngularVelocity);

        void                                SetMaxDepenetrationVelocity     (float const                        p_newMaxDepenetrationVelocity);


        template <physx::PxRigidBodyFlag::Enum const p_flagToSet>
        void                                SetRigidBodyFlag                (bool const                         p_value);

        void                                SetSendsSleepNotifies           (bool const                         p_sendSleepNotifies);

        void                                SetUserData                     (void*                              p_userData);
        
	    void                                SetWakeCounter                  (float const                        p_wakeCounter);

        void                                WakeUp                          ()                                                                  const;

    // ==============================[Public Local Operator]============================== //

        bool                operator==  (PhysicsActorHandle const&  p_other)    const;
        bool                operator!=  (PhysicsActorHandle const&  p_other)    const;

        PhysicsActorHandle& operator=   (PhysicsActorHandle const&  p_copy);
        PhysicsActorHandle& operator=   (PhysicsActorHandle&&       p_move);

}; // !PhysicsActorHandle

#include "PhysicsActorHandle.inl"

#endif // !__PHYSICS_ACTOR_HANDLE_HPP__