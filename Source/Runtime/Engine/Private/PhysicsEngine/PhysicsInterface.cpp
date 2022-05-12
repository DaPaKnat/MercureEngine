#include "PCH.hpp"

#include "PhysicsEngine/PhysicsInterface.hpp"
#include "PhysicsEngine/PhysicsEngine.hpp"
#include "PhysicsEngine/PhysicsEnums.hpp"
#include "Components/BoxComponent.hpp"
#include "Components/CapsuleComponent.hpp"
#include "Components/SphereComponent.hpp"
#include "PxPhysicsAPI.h"


struct 
{
    FORCEINLINE bool operator()    (HitResult const&   p_first,
                                    HitResult const&   p_second)   const
    {
        if (p_first.m_hitTime == p_second.m_hitTime)
        {
            return (p_first.m_isBlockingHit == p_second.m_isBlockingHit) ? true : p_second.m_isBlockingHit;
        }

        return p_first.m_hitTime < p_second.m_hitTime;
    }
} HitResultSort;

// ==============================[Public Static Methods]============================== //

bool                PhysicsInterface::AddSweepResults               (bool&                                  p_outHasValidBlockingHit,
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
                                                                     bool const                             p_returnPhysicalMaterial)
{
    p_outHitResults.reserve(p_sweepBuffer.getNbAnyHits());

    bool            convertResult   {true};
    bool            hasBlockingHit  {false};
    physx::PxVec3   direction       {PhysXTypeConverter::TVector3((p_sweepEnd - p_sweepStart).GetSafeNormalized())};

    for (uint32 index {0u}; index < p_sweepBuffer.getNbAnyHits(); ++index)
    {
        physx::PxSweepHit const&  sweepHit {p_sweepBuffer.getAnyHit(index)};

        if (sweepHit.distance <= p_maxDistance)
        {
            HitResult hitResult;

            if (ConvertQueryImpactHit(sweepHit, hitResult, p_sweepLength, p_queryFilter, p_sweepStart, p_sweepEnd, &p_geometry, p_queryTransform, p_returnFaceIndex, p_returnPhysicalMaterial))
            {
                hasBlockingHit |= hitResult.m_isBlockingHit;
                p_outHitResults.push_back(hitResult);
            }

            else
            {
                convertResult = false;
            }

        }
    }

    std::sort(p_outHitResults.begin(), p_outHitResults.end(), HitResultSort);
    p_outHasValidBlockingHit = hasBlockingHit;

    return convertResult;
}

bool                PhysicsInterface::ConvertOverlapResults         (physx::PxOverlapBuffer const&          p_overlapBuffer,
                                                                     physx::PxFilterData const&             p_queryFilter,
                                                                     std::vector<OverlapResult>&            p_outOverlapResults)
{
    bool    blockingFound   {false};
    uint32  overlapCount    {p_overlapBuffer.getNbAnyHits()};

    if (overlapCount > 0)
    {
        auto    expectedSize    {p_outOverlapResults.size() + overlapCount };

        for (uint32 index {0} ; index < overlapCount ; ++index)
        {
            OverlapResult newOverlapResult;
            ConvertQueryOverlap(p_overlapBuffer.getAnyHit(index), p_queryFilter, newOverlapResult);

            if (newOverlapResult.m_isBlockingHit)
            {
                blockingFound = true;
            }
        
            bool existInArray   {false};
            for (uint32 indexCheck {0} ; indexCheck < p_outOverlapResults.size(); ++indexCheck)
            {
                OverlapResult& overlapResultRef {p_outOverlapResults[indexCheck]};

                if (overlapResultRef.m_component == newOverlapResult.m_component)
                {
                    existInArray = true;
                    break;
                }
            }

            if (existInArray == false)
            {
                p_outOverlapResults.push_back(newOverlapResult);
            }
        }
    }

    return blockingFound;
}


bool                PhysicsInterface::ConvertQueryImpactHit         (physx::PxLocationHit const&            p_hit,
                                                                     HitResult&                             p_outHitResult,
                                                                     float                                  p_raycastLength,
                                                                     physx::PxFilterData const&             p_queryFilter,
                                                                     Vector3 const&                         p_traceStart,
                                                                     Vector3 const&                         p_traceEnd,
                                                                     physx::PxGeometry const*               p_geometry,
                                                                     physx::PxTransform const&              p_queryTransform,
                                                                     bool                                   p_returnFaceIndex,
                                                                     bool                                   p_returnPhysicalMaterial)
{
    // Check if it's sweep trace and if the hit an intial overlap.
    bool const initialOverlap   {p_hit.hadInitialOverlap()};
    if (initialOverlap && p_geometry != nullptr && p_hit.distance != 0.0f)
    {
        // ConvertOverlappedShapeToImpactHit
        return true;
    }
    
    // See if this is a 'blocking hit' //
    physx::PxFilterData         shapeFilter {p_hit.shape->getQueryFilterData()};
    physx::PxQueryHitType::Enum hitType     {PhysicsQueryFilterCallback::ComputeQueryHitType(p_queryFilter, shapeFilter)};
    
    p_outHitResult.m_isBlockingHit      = (hitType == physx::PxQueryHitType::eBLOCK);
    p_outHitResult.m_startPenetrating   = initialOverlap;

    // Compute Hit Time
    float const hitTime     {p_hit.distance / p_raycastLength};
    p_outHitResult.m_hitTime = hitTime;
    p_outHitResult.m_distance = p_hit.distance;

    
    // figure out where the the "safe" location for this shape is by moving from the startLoc toward the ImpactPoint
    Vector3 const traceStartToEnd           {p_traceStart, p_traceEnd};
    Vector3 const safeLocationToFitShape    {p_traceStart + (hitTime * traceStartToEnd)};
    p_outHitResult.m_location = safeLocationToFitShape;


    bool const usePhysXPoint    {(p_hit.flags & physx::PxHitFlag::ePOSITION) && !initialOverlap};
    if (usePhysXPoint && !p_hit.position.isFinite())
    {
        p_outHitResult = HitResult();
        LOG(LogPhysics, Error, "ConvertQueryImpactHit() received NaN/Inf for location : (%.2f, %.2f, %.2f)", p_hit.position.x, p_hit.position.y, p_hit.position.z);

        return false;
    }
    p_outHitResult.m_impactLocation = usePhysXPoint ? PhysXTypeConverter::TVector3(p_hit.position) : p_traceStart;


    bool const usePhysXNormal   {(p_hit.flags & physx::PxHitFlag::eNORMAL) && !initialOverlap};
    if (usePhysXNormal && !p_hit.normal.isFinite())
    {
        p_outHitResult = HitResult();
        LOG(LogPhysics, Error, "ConvertQueryImpactHit() received NaN/Inf for normal : (%.2f, %.2f, %.2f)", p_hit.normal.x, p_hit.normal.y, p_hit.normal.z);

        return false;
    }


    Vector3 normal  {usePhysXNormal ? PhysXTypeConverter::TVector3(p_hit.normal).GetSafeNormalized() : -traceStartToEnd.GetSafeNormalized()};
    p_outHitResult.m_normal         = normal;
    p_outHitResult.m_impactNormal   = normal;

    p_outHitResult.m_traceStart    = p_traceStart;
    p_outHitResult.m_traceEnd      = p_traceEnd;

    if (usePhysXNormal && !normal.IsNormalized())
    {
        normal = -traceStartToEnd.GetSafeNormalized();
        p_outHitResult.m_normal = normal;
        p_outHitResult.m_impactNormal = normal;
    }

    physx::PxGeometryType::Enum sweptGeometryType = p_geometry ? p_geometry->getType() : physx::PxGeometryType::eINVALID;

    if (BodyInstance* bodyInstance {(BodyInstance*)p_hit.actor->userData})
    {
        p_outHitResult.m_entity = bodyInstance->m_ownerComponent->GetOwner();
        p_outHitResult.m_primitiveComponent = bodyInstance->m_ownerComponent;
    }

    return true;
}

void                PhysicsInterface::ConvertQueryOverlap           (physx::PxOverlapHit const&             p_overlapHit,
                                                                     physx::PxFilterData const&             p_queryFilter,
                                                                     OverlapResult&                         p_outOverlapResult)
{
    if (BodyInstance const* bodyInstance {(BodyInstance*)p_overlapHit.actor->userData})
    {
        if (PrimitiveComponent* primitiveComponent {bodyInstance->m_ownerComponent})
        {
            p_outOverlapResult.m_actor = primitiveComponent->GetOwner();
            p_outOverlapResult.m_component = primitiveComponent;
        }
    }

    // See if this is a 'blocking' hit
    physx::PxFilterData const           shapeFilter {p_overlapHit.shape->getQueryFilterData()};
    physx::PxQueryHitType::Enum  const  hitType     {PhysicsQueryFilterCallback::ComputeQueryHitType(p_queryFilter, shapeFilter)};
    
    p_outOverlapResult.m_isBlockingHit = (hitType == physx::PxQueryHitType::eBLOCK);
}

bool                PhysicsInterface::ConvertQueryRaycastResults    (bool&                                  p_outHasValidBlockingHit,
                                                                     physx::PxRaycastBuffer const&          p_hitBuffer,
                                                                     std::vector<HitResult>&                p_outHitResults,
                                                                     float                                  p_raycastLength,
                                                                     physx::PxFilterData const&             p_queryFilter,
                                                                     Vector3 const&                         p_traceStart,
                                                                     Vector3 const&                         p_traceEnd,
                                                                     bool                                   p_returnFaceIndex,
                                                                     bool                                   p_returnPhysicalMaterial)
{
    p_outHitResults.clear();
    p_outHitResults.reserve(p_hitBuffer.getNbAnyHits());

    bool convertResult  {true};
    bool hasBlockingHit {false};

    physx::PxTransform startTransform {PhysXTypeConverter::TVector3(p_traceStart)};

    for (uint32 index   {0u}; index < p_hitBuffer.getNbAnyHits(); ++index)
    {
        HitResult                   hitResult;
        physx::PxRaycastHit const&  raycastHit {p_hitBuffer.getAnyHit(index)};

        if (ConvertQueryImpactHit(raycastHit, hitResult, p_raycastLength, p_queryFilter, p_traceStart, p_traceEnd, nullptr, startTransform, p_returnFaceIndex, p_returnPhysicalMaterial))
        {
            hasBlockingHit |= hitResult.m_isBlockingHit;
            p_outHitResults.push_back(hitResult);
        }

        else
        {
            convertResult = false;
        }
    }

    std::sort(p_outHitResults.begin(), p_outHitResults.end(), HitResultSort);
    p_outHasValidBlockingHit = hasBlockingHit;

    return convertResult;

}


PhysicsActorHandle  PhysicsInterface::CreateActorHandle             (PhysicsActorCreationParameters const&  p_actorCreationParam)
{
    PhysicsActorHandle  actorHandle;

    physx::PxTransform  transform   {PhysXTypeConverter::TTransform(p_actorCreationParam.m_transform)};
    PhysicsSceneHandle* scene       {p_actorCreationParam.m_physicsScene};

    // Create physx::PxRigidStatic
    if (p_actorCreationParam.m_isStatic)
    {
        actorHandle.m_actor = GEngine->GetModuleManager()->Get<PhysicsEngine>().m_physXSDK->createRigidStatic(transform);

        if (p_actorCreationParam.m_queryOnly)
        {
            PhysicsInterface::ModifyActorFlag<physx::PxActorFlag::eDISABLE_SIMULATION>(actorHandle.m_actor, true);
        }

    }

    // Create physx::PxRigidDynamic
    else
    {
        physx::PxRigidDynamic*  rigidDynamic    {GEngine->GetModuleManager()->Get<PhysicsEngine>().m_physXSDK->createRigidDynamic(transform)};
        actorHandle.m_actor                     = rigidDynamic; 

        PhysicsInterface::ModifyRigidBodyFlag<physx::PxRigidBodyFlag::eUSE_KINEMATIC_TARGET_FOR_SCENE_QUERIES>(rigidDynamic, true);

        if (p_actorCreationParam.m_queryOnly)
        {
            PhysicsInterface::ModifyActorFlag<physx::PxActorFlag::eDISABLE_SIMULATION>(actorHandle.m_actor, true);
        }

        if (!p_actorCreationParam.m_enableGravity)
        {
            PhysicsInterface::ModifyActorFlag<physx::PxActorFlag::eDISABLE_GRAVITY>(actorHandle.m_actor, true);
        }
    }

    return actorHandle;
}

physx::PxGeometry*  PhysicsInterface::CreateGeometry                (PrimitiveComponent*                    p_primitiveComponent)
{
    if (p_primitiveComponent)
    {
        switch (p_primitiveComponent->GetGeometryType())
        {
            case EGeometryType::Box     :
            {
                BoxComponent* boxComponent  {static_cast<BoxComponent*>(p_primitiveComponent)};
                return new physx::PxBoxGeometry(PhysXTypeConverter::TVector3(boxComponent->GetScaledBoxExtent()));
            }

            case EGeometryType::Capsule :
            {
                CapsuleComponent* capsuleComponent  {static_cast<CapsuleComponent*>(p_primitiveComponent) };
                return new physx::PxCapsuleGeometry(capsuleComponent->GetScaledCapsuleRadius(), capsuleComponent->GetScaledCapsuleHalfHeight());
            }
            
            case EGeometryType::Sphere  :
            {
                SphereComponent* sphereComponent    {static_cast<SphereComponent*>(p_primitiveComponent) };
                return new physx::PxSphereGeometry(sphereComponent->GetScaledSphereRadius());
            }
        }
    }

    return nullptr;
}

physx::PxFilterData PhysicsInterface::CreateQueryFilterData         (ECollisionChannel const                p_channel,
                                                                     bool const                             p_traceComplex,
                                                                     CollisionResponseContainer const&      p_collisionResponseContainer,
                                                                     CollisionQueryParams const&            p_queryParam,
                                                                     CollisionObjectQueryParams const&      p_objectQueryParam,
                                                                     bool const                             p_multiTrace)
{
    if (p_objectQueryParam.IsValid())
    {
        return CreateObjectQueryFilterData(p_traceComplex, p_multiTrace, p_objectQueryParam);
    }

    else
    {
        return CreateTraceQueryFilterData(p_channel, p_traceComplex, p_collisionResponseContainer, p_queryParam);
    }
}

physx::PxFilterData PhysicsInterface::CreateObjectQueryFilterData   (bool const                             p_traceComplex,
                                                                     bool const                             p_multiTrace,
                                                                     CollisionObjectQueryParams const&      p_objectQueryParams)
{
    /**
     * Format for Query Data:
     *      - word0 : QueryType
     *
     * For Trace Queries:
     *      - word1 : Object Type queries
     *      - word2 : Unused
     *      - word3 : p_channel
     */

     physx::PxFilterData    newData;

     newData.word0  = static_cast<uint32>(ECollisionQueryType::ObjectQuery);

     newData.word1  = p_objectQueryParams.GetQueryBitfield();

     newData.word3  = static_cast<uint32>(p_multiTrace ? TRACE_MULTI : TRACE_SINGLE);

     return newData;
}

physx::PxFilterData PhysicsInterface::CreateTraceQueryFilterData    (ECollisionChannel const                p_channel,
                                                                     bool const                             p_traceComplex,
                                                                     CollisionResponseContainer const&      p_collisionResponseContainer,
                                                                     CollisionQueryParams const&            p_queryParams)
{
    /**
     * Format for Query Data:
     *      - word0 : QueryType
     *
     * For Trace Queries:
     *      - word1 : Blocking Channels
     *      - word2 : Touching Channels
     *      - word3 : p_channel
     */

    physx::PxFilterData newData;

    newData.word0 = static_cast<uint32>(ECollisionQueryType::TraceQuery);

    // Sets ord 1 for what index block and word 2 for what index touch"
    for (int32 index = 0 ; index < 32 ; ++index)
    {
        if(p_collisionResponseContainer.m_enumArray[index] == static_cast<uint8>(ECollisionResponse::Block))
		{
			// if index block, set that in word1
            newData.word1 |= (1 << index);
		}
        
        else if (p_collisionResponseContainer.m_enumArray[index] == static_cast<uint8>(ECollisionResponse::Overlap))
		{
			// if index touch, set that in word2
            newData.word2 |= (1 << index);
        }
    }

    newData.word3 = static_cast<uint32>(p_channel);

    return newData;
}
