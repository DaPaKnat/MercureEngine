#ifndef __PHYSICS_INTERFACE_HPP__
#define __PHYSICS_INTERFACE_HPP__

#include "CoreMinimal.hpp"

#include "PxPhysicsAPI.h"

#include "PhysicsActorCreationParameters.hpp"
#include "PhysicsActorHandle.hpp"
#include "OverlapResult.hpp"

/**
 * Struct containing methods use for Physics.
 */
struct PhysicsInterface
{
    // ==============================[Public Static Methods]============================== //

        static bool                     AddSweepResults                                             (bool&                                  p_outHasValidBlockingHit,
                                                                                                     physx::PxSweepBuffer const&            p_sweepBuffer,
                                                                                                     std::vector<HitResult>&                p_outHitResults,
                                                                                                     float const                            p_sweepLength,
                                                                                                     physx::PxFilterData const&             p_queryFilter,
                                                                                                     Vector3 const&                         p_sweepStart,
                                                                                                     Vector3 const&                         p_sweepEnd,
                                                                                                     physx::PxGeometry const&               p_geometry,
                                                                                                     physx::PxTransform const&              p_queryTransform,
                                                                                                     float const                            p_maxDistance,
                                                                                                     bool const                             p_returnFaceIndex,
                                                                                                     bool const                             p_returnPhysicalMaterial);

        static bool                     ConvertOverlapResults                                       (physx::PxOverlapBuffer const&          p_overlapBuffer,
                                                                                                     physx::PxFilterData const&             p_queryFilter,
                                                                                                     std::vector<OverlapResult>&            p_outOverlapResults);

        static bool                     ConvertQueryImpactHit                                       (physx::PxLocationHit const&            p_hit,
                                                                                                     HitResult&                             p_outHitResult,
                                                                                                     float                                  p_raycastLength,
                                                                                                     physx::PxFilterData const&             p_queryFilter,
                                                                                                     Vector3 const&                         p_traceStart,
                                                                                                     Vector3 const&                         p_traceEnd,
                                                                                                     physx::PxGeometry const*               p_geometry,
                                                                                                     physx::PxTransform const&              p_queryTransform,
                                                                                                     bool                                   p_returnFaceIndex,
                                                                                                     bool                                   p_returnPhysicalMaterial);

        static void                     ConvertQueryOverlap                                         (physx::PxOverlapHit const&             p_overlapHit,
                                                                                                     physx::PxFilterData const&             p_queryFilter,
                                                                                                     OverlapResult&                         p_outOverlapResult);

        static bool                     ConvertQueryRaycastResults                                  (bool&                                  p_outHasValidBlockingHit,
                                                                                                     physx::PxRaycastBuffer const&          p_hitBuffer,
                                                                                                     std::vector<HitResult>&                p_outHitReuslts,
                                                                                                     float                                  p_raycastLength,
                                                                                                     physx::PxFilterData const&             p_queryFilter,
                                                                                                     Vector3 const&                         p_traceStart,
                                                                                                     Vector3 const&                         p_traceEnd,
                                                                                                     bool                                   p_returnFaceIndex,
                                                                                                     bool                                   p_returnPhysicalMaterial);

        static PhysicsActorHandle       CreateActorHandle                                           (PhysicsActorCreationParameters const&  p_actorCreationParams);

        static physx::PxGeometry*       CreateGeometry                                              (PrimitiveComponent*                    p_primitiveComponent);

        static physx::PxFilterData      CreateObjectQueryFilterData                                 (bool const                             p_traceComplex,
                                                                                                     bool const                             p_multiTrace,
                                                                                                     CollisionObjectQueryParams const&      p_objectQueryParams);

        static physx::PxFilterData      CreateQueryFilterData                                       (ECollisionChannel const                p_channel,
                                                                                                     bool const                             p_traceComplex,
                                                                                                     CollisionResponseContainer const&      p_collisionResponseContainer,
                                                                                                     CollisionQueryParams const&            p_queryParam,
                                                                                                     CollisionObjectQueryParams const&      p_objectQueryParam,
                                                                                                     bool const                             p_multiTrace);

        static physx::PxFilterData      CreateTraceQueryFilterData                                  (ECollisionChannel const                p_channel,
                                                                                                     bool const                             p_traceComplex,
                                                                                                     CollisionResponseContainer const&      p_collisionResponseContainer,
                                                                                                     CollisionQueryParams const&            p_queryParams);

        template <const physx::PxActorFlag::Enum p_flagToSet>
        static INLINE void              ModifyActorFlag                                             (physx::PxActorFlags&                   p_flagContainer,
                                                                                                     bool const                             p_value);

        template <const physx::PxActorFlag::Enum p_flagToSet >
        static INLINE void              ModifyActorFlag                                             (physx::PxActor*                        p_actor,
                                                                                                     bool const                             p_value);

        template <typename FlagContainer, typename FlagType>
        static INLINE void              ModifyFlagDefault                                           (FlagContainer&                         p_flagContainer,
                                                                                                     FlagType const                         p_flagtoSet,
                                                                                                     bool const                             p_value);

        template <const physx::PxRigidBodyFlag::Enum p_flagToSet>
        static INLINE void              ModifyRigidBodyFlag                                         (physx::PxRigidBodyFlags&               p_flagContainer,
                                                                                                     bool const                             p_value);
        template<>
        static INLINE void              ModifyRigidBodyFlag<physx::PxRigidBodyFlag::eKINEMATIC>     (physx::PxRigidBodyFlags&               p_flagContainer,
                                                                                                     bool const                             p_value);

        template<>
        static INLINE void              ModifyRigidBodyFlag<physx::PxRigidBodyFlag::eENABLE_CCD>    (physx::PxRigidBodyFlags&               p_flagContainer,
                                                                                                     bool const                             p_value);

        template <const physx::PxRigidBodyFlag::Enum p_flagToSet>
        static INLINE void              ModifyRigidBodyFlag                                         (physx::PxRigidBody*                    p_rigidbody,
                                                                                                     bool const                             p_value);
        
        template <const physx::PxShapeFlag::Enum p_flagToSet>
        static INLINE void              ModifyShapeFlag                                             (physx::PxShapeFlags&                   p_flagContainer,
                                                                                                     bool const                             p_value);

        template <const physx::PxShapeFlag::Enum p_flagToSet>
        static INLINE void              ModifyShapeFlag                                             (physx::PxShape*                        p_shape,
                                                                                                     bool const                             p_value);


}; // !struct PhysicsInterface

#include "PhysicsInterface.inl"

#endif // !__PHYSICS_INTERFACE_HPP__