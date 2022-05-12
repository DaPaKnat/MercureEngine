#ifndef __PHYSICS_SCENE_HANDLE_HPP__
#define __PHYSICS_SCENE_HANDLE_HPP__

#define QUERY_BUFFER_SIZE 512

#include "PxPhysicsAPI.h"

#include "PhysicsActorHandle.hpp"
#include "HitResult.hpp"
#include "OverlapResult.hpp"
#include "CollisionResponseParams.hpp"
#include "CollisionObjectQueryParams.hpp"
#include "CollisionQueryParams.hpp"
#include "PhysicsQueryFilterCallback.hpp"

// ==============================[Forward Declaration]============================== //
    
    class World;

// ================================================================================= //

class PhysicsSceneHandle
{
    public:

    // ==============================[Public Constructor + Destructor]============================== //

        PhysicsSceneHandle  ()                                  = default;

        PhysicsSceneHandle  (PhysicsSceneHandle const&  p_copy) = default;

        PhysicsSceneHandle  (PhysicsSceneHandle&&       p_move) = default;

        ~PhysicsSceneHandle ()                                  = default;

    // ==============================[Public Local Methods]============================== //

        FORCEINLINE World*  GetOwningWorld              ()  const;

        bool                GeometryOverlapAnyTest      (CollisionShape const&              p_collisionShape, 
                                                         Vector3 const&                     p_overlapLocation, 
                                                         Quaternion const&                  p_overlapRotation, 
                                                         ECollisionChannel const            p_traceChannel, 
                                                         CollisionQueryParams const&        p_queryParams, 
                                                         CollisionResponseParams const&     p_responseParams, 
                                                         CollisionObjectQueryParams const&  p_objectQueryParams = CollisionObjectQueryParams::DefaultObjectQueryParams) const;
        
            
        bool                GeometryOverlapBlockingTest (CollisionShape const&              p_collisionShape, 
                                                         Vector3 const&                     p_overlapLocation, 
                                                         Quaternion const&                  p_overlapRotation, 
                                                         ECollisionChannel const            p_traceChannel, 
                                                         CollisionQueryParams const&        p_queryParams, 
                                                         CollisionResponseParams const&     p_responseParams, 
                                                         CollisionObjectQueryParams const&  p_objectQueryParams = CollisionObjectQueryParams::DefaultObjectQueryParams) const;

        bool                GeometryOverlapMulti        (std::vector<OverlapResult>&        p_outOverlapResut,
                                                         CollisionShape const&              p_collisionShape, 
                                                         Vector3 const&                     p_overlapLocation, 
                                                         Quaternion const&                  p_overlapRotation, 
                                                         ECollisionChannel const            p_traceChannel, 
                                                         CollisionQueryParams const&        p_queryParams, 
                                                         CollisionResponseParams const&     p_responseParams, 
                                                         CollisionObjectQueryParams const&  p_objectQueryParams = CollisionObjectQueryParams::DefaultObjectQueryParams) const;

        bool                GeometrySweepMulti          (std::vector<HitResult>&            p_outHitResults,
                                                         CollisionShape const&              p_collisionShape,
                                                         Quaternion const&                  p_rotation, 
                                                         Vector3 const&                     p_sweepStart, 
                                                         Vector3 const&                     p_sweepEnd, 
                                                         ECollisionChannel const            p_traceChannel, 
                                                         CollisionQueryParams const&        p_queryParams,
                                                         CollisionResponseParams const&     p_responseParams, 
                                                         CollisionObjectQueryParams const&  p_objectQueryParams = CollisionObjectQueryParams::DefaultObjectQueryParams) const;


        bool                GeometrySweepSingle         (HitResult&                         p_outHitResult,
                                                         CollisionShape const&              p_collisionShape,
                                                         Quaternion const&                  p_rotation, 
                                                         Vector3 const&                     p_sweepStart, 
                                                         Vector3 const&                     p_sweepEnd, 
                                                         ECollisionChannel const            p_traceChannel, 
                                                         CollisionQueryParams const&        p_queryParams,
                                                         CollisionResponseParams const&     p_responseParams, 
                                                         CollisionObjectQueryParams const&  p_objectQueryParams = CollisionObjectQueryParams::DefaultObjectQueryParams) const;


        bool                GeometrySweepTest           (CollisionShape const&              p_collisionShape,
                                                         Quaternion const&                  p_rotation, 
                                                         Vector3 const&                     p_sweepStart, 
                                                         Vector3 const&                     p_sweepEnd, 
                                                         ECollisionChannel const            p_traceChannel, 
                                                         CollisionQueryParams const&        p_queryParams,
                                                         CollisionResponseParams const&     p_responseParams, 
                                                         CollisionObjectQueryParams const&  p_objectQueryParams = CollisionObjectQueryParams::DefaultObjectQueryParams) const;
        bool                IsValid                     ()  const;

        bool                RaycastMulti                (std::vector<HitResult>&            p_outHitResults,
                                                         Vector3 const&                     p_traceStart,
                                                         Vector3 const&                     p_traceEnd,
                                                         ECollisionChannel const            p_traceChannel,
                                                         CollisionQueryParams const&        p_queryParams,
                                                         CollisionResponseParams const&     p_responseParams,
                                                         CollisionObjectQueryParams const&  p_objectQueryParams)  const;

        bool                RaycastSingle               (HitResult&                         p_outHitResult,
                                                         Vector3 const&                     p_traceStart,
                                                         Vector3 const&                     p_traceEnd,
                                                         ECollisionChannel const            p_traceChannel,
                                                         CollisionQueryParams const&        p_queryParams,
                                                         CollisionResponseParams const&     p_responseParams,
                                                         CollisionObjectQueryParams const&  p_objectQueryParams)  const;

        bool                RaycastTest                 (Vector3 const&                     p_traceStart,
                                                         Vector3 const&                     p_traceEnd,
                                                         ECollisionChannel const            p_traceChannel,
                                                         CollisionQueryParams const&        p_queryParams,
                                                         CollisionResponseParams const&     p_responseParams,
                                                         CollisionObjectQueryParams const&  p_objectQueryParams = CollisionObjectQueryParams::DefaultObjectQueryParams)  const;

        void                RegisterActor               (PhysicsActorHandle*                p_actorHandle);

        void                Release                     ();

        void                SetPhysXScene               (physx::PxScene*                    p_newPhysicsScene);

        FORCEINLINE void    SetOwningWorld              (World*                             p_newOwningWorld);

        void                Simulate                    (float const                        p_deltaTime);

    private:
        
    // ==============================[Private Local Properties]============================== //

        /** ... */
        physx::PxScene*                     m_physXScene;

        /** ... */
        World*                              m_owningWorld;
    
        /** ... */
        std::vector<PhysicsActorHandle*>    m_actorHandles;

        float                               m_timeBank;

    // ==============================[Private Local Methods]============================== //

        physx::PxGeometry*  CreateGeometryPtr       (CollisionShape const&              p_collisionShape)   const;

        template <bool t_isBlocking>
        bool                GeometryOverlapTestImp  (physx::PxGeometry const&           p_geometry, 
                                                     physx::PxTransform const&          p_geometryTransform, 
                                                     ECollisionChannel                  p_traceChannel, 
                                                     CollisionQueryParams const&        p_queryParams, 
                                                     CollisionResponseParams const&     p_responseParams, 
                                                     CollisionObjectQueryParams const&  p_objectQueryParams)    const;

        void                UpdateBodyTransforms    ();

}; // !class PhysicsSceneHandle

#include "PhysicsSceneHandle.inl"

#endif // !__PHYSICS_SCENE_HANDLE_HPP__