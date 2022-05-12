#include "PCH.hpp"

#include "PhysicsEngine/PhysicsSceneHandle.hpp"
#include "PhysicsEngine/PhysicsEngine.hpp"
#include "PhysicsEngine/BodyInstance.hpp"
#include "PhysicsEngine/PhysicsQueryFilterCallbackSweep.hpp"
#include "PhysicsEngine/PhysicsInterface.hpp"
#include "Components/PrimitiveComponent.hpp"

#include "Engine.hpp"

// ==============================[Public Local Methods]============================== //

bool    PhysicsSceneHandle::GeometryOverlapAnyTest      (CollisionShape const&              p_collisionShape,
                                                         Vector3 const&                     p_overlapLocation, 
                                                         Quaternion const&                  p_overlapRotation, 
                                                         ECollisionChannel const            p_traceChannel, 
                                                         CollisionQueryParams const&        p_queryParams, 
                                                         CollisionResponseParams const&     p_responseParams, 
                                                         CollisionObjectQueryParams const&  p_objectQueryParams /** = CollisionObjectQueryParams::DefaultObjectQueryParams */) const
{
    physx::PxGeometry*  geometry    {CreateGeometryPtr(p_collisionShape)};
    
    if (geometry != nullptr)
    {
        physx::PxTransform  geometryTransform   {PhysXTypeConverter::TVector3(p_overlapLocation), PhysXTypeConverter::TQuaternion(p_overlapRotation) };
        if (geometry->getType() == physx::PxGeometryType::eCAPSULE)
            geometryTransform.q *= physx::PxQuat(physx::PxHalfPi, physx::PxVec3(0.0f, 0.0f, 1.0f));
        
        bool                hasBlockingHit      {GeometryOverlapTestImp<false>(*geometry, geometryTransform, p_traceChannel, p_queryParams, p_responseParams,p_objectQueryParams)};

        delete geometry;

        return hasBlockingHit;
    }

    return false;
}
        
bool    PhysicsSceneHandle::GeometryOverlapBlockingTest (CollisionShape const&              p_collisionShape,
                                                         Vector3 const&                     p_overlapLocation, 
                                                         Quaternion const&                  p_overlapRotation, 
                                                         ECollisionChannel const            p_traceChannel, 
                                                         CollisionQueryParams const&        p_queryParams, 
                                                         CollisionResponseParams const&     p_responseParams, 
                                                         CollisionObjectQueryParams const&  p_objectQueryParams /** = CollisionObjectQueryParams::DefaultObjectQueryParams*/) const
{
    physx::PxGeometry*  geometry    {CreateGeometryPtr(p_collisionShape)};
    
    if (geometry != nullptr)
    {
        physx::PxTransform  geometryTransform   {PhysXTypeConverter::TVector3(p_overlapLocation), PhysXTypeConverter::TQuaternion(p_overlapRotation) };
        if (geometry->getType() == physx::PxGeometryType::eCAPSULE)
            geometryTransform.q *= physx::PxQuat(physx::PxHalfPi, physx::PxVec3(0.0f, 0.0f, 1.0f));

        bool                hasBlockingHit      {GeometryOverlapTestImp<true>(*geometry, geometryTransform, p_traceChannel, p_queryParams, p_responseParams,p_objectQueryParams)};

        delete geometry;

        return hasBlockingHit;
    }

    return false;
}

bool    PhysicsSceneHandle::GeometryOverlapMulti        (std::vector<OverlapResult>&        p_outOverlapResults,
                                                         CollisionShape const&              p_collisionShape, 
                                                         Vector3 const&                     p_overlapLocation, 
                                                         Quaternion const&                  p_overlapRotation, 
                                                         ECollisionChannel const            p_traceChannel, 
                                                         CollisionQueryParams const&        p_queryParams, 
                                                         CollisionResponseParams const&     p_responseParams, 
                                                         CollisionObjectQueryParams const&  p_objectQueryParams /** = CollisionObjectQueryParams::DefaultObjectQueryParams */) const
{
    bool                hasBlockingHit  {false};
    physx::PxGeometry*  geometry        {CreateGeometryPtr(p_collisionShape)};

    if (geometry != nullptr)
    {
        physx::PxTransform                          geometryTransform   {PhysXTypeConverter::TVector3(p_overlapLocation), PhysXTypeConverter::TQuaternion(p_overlapRotation)};
        if (geometry->getType() == physx::PxGeometryType::eCAPSULE)
            geometryTransform.q *= physx::PxQuat(physx::PxHalfPi, physx::PxVec3(0.0f, 0.0f, 1.0f));
        

        physx::PxOverlapBufferN<QUERY_BUFFER_SIZE>  overlapBuffer;
        physx::PxFilterData                         filterData          {PhysicsInterface::CreateQueryFilterData(p_traceChannel, 
                                                                                                     p_queryParams.m_traceComplex, 
                                                                                                     p_responseParams.m_collisionResponseContainer,
                                                                                                     p_queryParams,
                                                                                                     p_objectQueryParams,
                                                                                                     true)};
        physx::PxSceneQueryFilterData               queryFilterData     {filterData, 
                                                                         p_queryParams.GetPhysXQueryMobilityFlags() | 
                                                                         physx::PxQueryFlag::ePREFILTER};

        PhysicsQueryFilterCallback                  queryFilterCallback {p_queryParams};
        queryFilterCallback.m_isOverlapQuery = true;

        m_physXScene->overlap(*geometry, geometryTransform, overlapBuffer, queryFilterData, &queryFilterCallback);
        hasBlockingHit = overlapBuffer.hasBlock;
    
        hasBlockingHit = PhysicsInterface::ConvertOverlapResults(overlapBuffer, filterData, p_outOverlapResults);

        delete geometry;
    }

    return hasBlockingHit;
}

bool    PhysicsSceneHandle::GeometrySweepMulti          (std::vector<HitResult>&            p_outHitResults,
                                                         CollisionShape const&              p_collisionShape,
                                                         Quaternion const&                  p_rotation, 
                                                         Vector3 const&                     p_sweepStart, 
                                                         Vector3 const&                     p_sweepEnd, 
                                                         ECollisionChannel const            p_traceChannel, 
                                                         CollisionQueryParams const&        p_queryParams,
                                                         CollisionResponseParams const&     p_responseParams, 
                                                         CollisionObjectQueryParams const&  p_objectQueryParams /** = CollisionObjectQueryParams::DefaultObjectQueryParams */) const
{
    bool hasBlockingHit  {false};

    p_outHitResults.clear();

    if (m_physXScene != nullptr)
    {
        physx::PxGeometry* geometry     {CreateGeometryPtr(p_collisionShape)};

        if (geometry != nullptr)
        {
            Vector3 const                               delta                       {p_sweepStart, p_sweepEnd};
            float const                                 deltaMagnitude              {delta.GetMagnitude()};

            physx::PxQuat                               rotation                    {PhysXTypeConverter::TQuaternion(p_rotation)};
            if (geometry->getType() == physx::PxGeometryType::eCAPSULE)
            rotation *= physx::PxQuat(physx::PxHalfPi, physx::PxVec3(0.0f, 0.0f, 1.0f));

            physx::PxVec3 const                         direction                   {(deltaMagnitude == 0.0f) ? physx::PxVec3(1.0f, 0.0f, 0.0f) : PhysXTypeConverter::TVector3(delta / deltaMagnitude)};
            physx::PxTransform const                    startTransform              {PhysXTypeConverter::TVector3(p_sweepStart), rotation};

            physx::PxSweepBufferN<QUERY_BUFFER_SIZE>    sweepBuffer;
            physx::PxFilterData                         filterData                  {PhysicsInterface::CreateQueryFilterData(p_traceChannel, 
                                                                                                                             p_queryParams.m_traceComplex, 
                                                                                                                             p_responseParams.m_collisionResponseContainer,
                                                                                                                             p_queryParams,
                                                                                                                             p_objectQueryParams,
                                                                                                                             true)};
            physx::PxQueryFilterData                    queryFilterData             {filterData, 
                                                                                     p_queryParams.GetPhysXQueryMobilityFlags() | 
                                                                                     physx::PxQueryFlag::ePREFILTER             |
                                                                                     physx::PxQueryFlag::ePOSTFILTER};

            physx::PxHitFlags                           hitFlags                    {physx::PxHitFlag::ePOSITION   | 
                                                                                     physx::PxHitFlag::eNORMAL     | 
                                                                                     physx::PxHitFlag::eDISTANCE   | 
                                                                                     physx::PxHitFlag::eMTD        | 
                                                                                     physx::PxHitFlag::eFACE_INDEX};

            PhysicsQueryFilterCallbackSweep             queryFilterCallbackSweep    {p_queryParams};

            m_physXScene->sweep(*geometry, startTransform, direction, deltaMagnitude, sweepBuffer, hitFlags, queryFilterData, &queryFilterCallbackSweep);
            
            hasBlockingHit = sweepBuffer.hasBlock;

            // Fill hit result if an object has been hit by raycast
            if (hasBlockingHit == true)
            {
                if (!PhysicsInterface::AddSweepResults(hasBlockingHit, sweepBuffer, p_outHitResults, deltaMagnitude, filterData, p_sweepStart, p_sweepEnd, *geometry, startTransform, sweepBuffer.block.distance, p_queryParams.m_returnFaceIndex, p_queryParams.m_returnPhysicalMaterial))
                {
                    hasBlockingHit = false;
                    LOG(LogPhysics, Error, "Geometry Sweep Multi resulted in a NaN/INF in hitResult!");
                }
            }
            

            delete geometry;
        }
    }

    return hasBlockingHit; 
}


bool    PhysicsSceneHandle::GeometrySweepSingle         (HitResult&                         p_outHitResult,
                                                         CollisionShape const&              p_collisionShape,
                                                         Quaternion const&                  p_rotation, 
                                                         Vector3 const&                     p_sweepStart, 
                                                         Vector3 const&                     p_sweepEnd, 
                                                         ECollisionChannel const            p_traceChannel, 
                                                         CollisionQueryParams const&        p_queryParams,
                                                         CollisionResponseParams const&     p_responseParams, 
                                                         CollisionObjectQueryParams const&  p_objectQueryParams /** = CollisionObjectQueryParams::DefaultObjectQueryParams */) const
{
    bool hasBlockingHit  {false};

    p_outHitResult              = HitResult();
    p_outHitResult.m_traceStart = p_sweepStart;
    p_outHitResult.m_traceEnd   = p_sweepEnd;

    if (m_physXScene != nullptr)
    {
        physx::PxGeometry* geometry     {CreateGeometryPtr(p_collisionShape)};

        if (geometry != nullptr)
        {
            Vector3 const                   delta                       {p_sweepStart, p_sweepEnd};
            float const                     deltaMagnitude              {delta.GetMagnitude()};

            physx::PxQuat                   rotation                    {PhysXTypeConverter::TQuaternion(p_rotation)};
            if (geometry->getType() == physx::PxGeometryType::eCAPSULE)
                rotation *= physx::PxQuat(physx::PxHalfPi, physx::PxVec3(0.0f, 0.0f, 1.0f));

            physx::PxVec3 const             direction                   {(deltaMagnitude == 0.0f) ? physx::PxVec3(1.0f, 0.0f, 0.0f) : PhysXTypeConverter::TVector3(delta / deltaMagnitude)};
            physx::PxTransform const        startTransform              {PhysXTypeConverter::TVector3(p_sweepStart), rotation};

            physx::PxSweepBuffer            sweepBuffer;
            physx::PxFilterData             filterData                  {PhysicsInterface::CreateQueryFilterData(p_traceChannel, 
                                                                                                                 p_queryParams.m_traceComplex, 
                                                                                                                 p_responseParams.m_collisionResponseContainer,
                                                                                                                 p_queryParams,
                                                                                                                 p_objectQueryParams,
                                                                                                                 false)};
            physx::PxQueryFilterData        queryFilterData             {filterData, 
                                                                         p_queryParams.GetPhysXQueryMobilityFlags() | 
                                                                         physx::PxQueryFlag::ePREFILTER             |
                                                                         physx::PxQueryFlag::ePOSTFILTER};

            physx::PxHitFlags               hitFlags                    {physx::PxHitFlag::ePOSITION   | 
                                                                         physx::PxHitFlag::eNORMAL     | 
                                                                         physx::PxHitFlag::eDISTANCE   | 
                                                                         physx::PxHitFlag::eMTD        | 
                                                                         physx::PxHitFlag::eFACE_INDEX};

            PhysicsQueryFilterCallbackSweep queryFilterCallbackSweep    {p_queryParams};

            m_physXScene->sweep(*geometry, startTransform, direction, deltaMagnitude, sweepBuffer, hitFlags, queryFilterData, &queryFilterCallbackSweep);
            
            hasBlockingHit = sweepBuffer.hasBlock;

            int32 hitCount = sweepBuffer.getNbAnyHits();

            // Fill hit result if an object has been hit by raycast
            if (hasBlockingHit == true)
            {
                if (!PhysicsInterface::ConvertQueryImpactHit(sweepBuffer.block, p_outHitResult, deltaMagnitude, filterData, p_sweepStart, p_sweepEnd, nullptr, startTransform, p_queryParams.m_returnFaceIndex, p_queryParams.m_returnPhysicalMaterial))
                {
                    hasBlockingHit = false;
                }

            }
            

            delete geometry;
        }
    }

    return hasBlockingHit; 
}


bool    PhysicsSceneHandle::GeometrySweepTest           (CollisionShape const&              p_collisionShape,
                                                         Quaternion const&                  p_rotation, 
                                                         Vector3 const&                     p_sweepStart, 
                                                         Vector3 const&                     p_sweepEnd, 
                                                         ECollisionChannel const            p_traceChannel, 
                                                         CollisionQueryParams const&        p_queryParams,
                                                         CollisionResponseParams const&     p_responseParams, 
                                                         CollisionObjectQueryParams const&  p_objectQueryParams /** = CollisionObjectQueryParams::DefaultObjectQueryParams */) const
{
    bool hasBlockingHit  {false};

    if (m_physXScene != nullptr)
    {
        physx::PxGeometry* geometry     {CreateGeometryPtr(p_collisionShape)};

        if (geometry != nullptr)
        {
            Vector3 const   delta           {p_sweepStart, p_sweepEnd};
            float const     deltaMagnitude  {delta.GetMagnitude()};

            if (Math::IsNearlyZero(deltaMagnitude) == false)
            {
                physx::PxQuat                   rotation                    {PhysXTypeConverter::TQuaternion(p_rotation)};
                if (geometry->getType() == physx::PxGeometryType::eCAPSULE)
                    rotation *= physx::PxQuat(physx::PxHalfPi, physx::PxVec3(0.0f, 0.0f, 1.0f));

                physx::PxVec3 const             direction                   {PhysXTypeConverter::TVector3(delta / deltaMagnitude)};
                physx::PxTransform const        startTransform              {PhysXTypeConverter::TVector3(p_sweepStart), rotation};

                physx::PxSweepBuffer            sweepBuffer;
                physx::PxFilterData             filterData                  {PhysicsInterface::CreateQueryFilterData(p_traceChannel, 
                                                                                                                     p_queryParams.m_traceComplex, 
                                                                                                                     p_responseParams.m_collisionResponseContainer,
                                                                                                                     p_queryParams,
                                                                                                                     p_objectQueryParams,
                                                                                                                     false)};
                physx::PxQueryFilterData        queryFilterData             {filterData, 
                                                                             p_queryParams.GetPhysXQueryMobilityFlags() | 
                                                                             physx::PxQueryFlag::ePREFILTER             |
                                                                             physx::PxQueryFlag::ePOSTFILTER            |
                                                                             physx::PxQueryFlag::eANY_HIT};

                physx::PxHitFlags               hitFlags                    {physx::PxHitFlag::ePOSITION   | 
                                                                             physx::PxHitFlag::eNORMAL     | 
                                                                             physx::PxHitFlag::eDISTANCE   | 
                                                                             physx::PxHitFlag::eMTD};

                PhysicsQueryFilterCallbackSweep queryFilterCallbackSweep    {p_queryParams};
                queryFilterCallbackSweep.m_ignoreTouches = true;

                m_physXScene->sweep(*geometry, startTransform, direction, deltaMagnitude, sweepBuffer, hitFlags, queryFilterData, &queryFilterCallbackSweep);
                
                hasBlockingHit = sweepBuffer.hasBlock;
            }

            delete geometry;
        }
    }

    return hasBlockingHit;

}


bool    PhysicsSceneHandle::IsValid                     ()  const
{
    return m_physXScene != nullptr;
}

void    PhysicsSceneHandle::RegisterActor               (PhysicsActorHandle*                p_newActorHandle)
{
    m_actorHandles.push_back(p_newActorHandle);
    m_physXScene->addActor(*p_newActorHandle->m_actor);
}

void    PhysicsSceneHandle::Release                     ()
{
    if (m_physXScene)
    {
        m_physXScene->release();
    }
}

void    PhysicsSceneHandle::SetPhysXScene               (physx::PxScene*                    p_newScene)
{
    m_physXScene = p_newScene;

    if (m_physXScene != nullptr)
    {
        m_physXScene->userData = this;
    }
}

void    PhysicsSceneHandle::Simulate                    (float const                        p_deltaTime)
{
    PhysicsEngine&  physicsEngine       {GEngine->GetModuleManager()->Get<PhysicsEngine>()};
    uint32          stepCount           {0u};

    m_timeBank += p_deltaTime;

    while (m_timeBank >= physicsEngine.m_fixeDeltaTime && stepCount < physicsEngine.m_maxSubstep)
    {
        stepCount++;
        m_timeBank -= physicsEngine.m_fixeDeltaTime;

        m_physXScene->simulate(physicsEngine.m_fixeDeltaTime);
        m_physXScene->fetchResults(true);
    }
    
    if (stepCount > 0u)
    {
        if (stepCount == physicsEngine.m_maxSubstep)
        {
            m_timeBank = 0.0f;
        }

        UpdateBodyTransforms();
    }
}

bool    PhysicsSceneHandle::RaycastMulti                (std::vector<HitResult>&            p_outHitResults,
                                                         Vector3 const&                     p_traceStart,
                                                         Vector3 const&                     p_traceEnd,
                                                         ECollisionChannel const            p_traceChannel,
                                                         CollisionQueryParams const&        p_queryParams,
                                                         CollisionResponseParams const&     p_responseParams,
                                                         CollisionObjectQueryParams const&  p_objectQueryParams)    const
{
    bool hasBlockingHit         {false};

    p_outHitResults.clear();

    if (m_physXScene != nullptr)
    {

        Vector3 const   delta           {p_traceStart, p_traceEnd};
        float const     deltaMagnitude  {delta.GetMagnitude()};

        if (Math::IsNearlyZero(deltaMagnitude) == false)
        {
            physx::PxVec3 const                         direction           {PhysXTypeConverter::TVector3(delta / deltaMagnitude)};
            physx::PxVec3 const                         start               {PhysXTypeConverter::TVector3(p_traceStart)};

            physx::PxRaycastBufferN<QUERY_BUFFER_SIZE>  raycastBuffer;
            physx::PxFilterData                         filterData          {PhysicsInterface::CreateQueryFilterData(p_traceChannel, 
                                                                                                                     p_queryParams.m_traceComplex, 
                                                                                                                     p_responseParams.m_collisionResponseContainer,
                                                                                                                     p_queryParams,
                                                                                                                     p_objectQueryParams,
                                                                                                                     true)};
            physx::PxSceneQueryFilterData   queryFilterData                 {filterData, 
                                                                             p_queryParams.GetPhysXQueryMobilityFlags() | 
                                                                             physx::PxQueryFlag::ePREFILTER};

            physx::PxHitFlags               hitFlags                        {physx::PxHitFlag::ePOSITION   | 
                                                                             physx::PxHitFlag::eNORMAL     | 
                                                                             physx::PxHitFlag::eDISTANCE   | 
                                                                             physx::PxHitFlag::eMTD        | 
                                                                             physx::PxHitFlag::eFACE_INDEX};

            PhysicsQueryFilterCallback      queryFilterCallback             {p_queryParams};

            m_physXScene->raycast(start,
                                  direction, 
                                  deltaMagnitude, 
                                  raycastBuffer, 
                                  hitFlags, 
                                  queryFilterData, 
                                  &queryFilterCallback);
            
            hasBlockingHit = raycastBuffer.hasBlock;
    
            uint32 hitCount {raycastBuffer.getNbAnyHits()};

            if (hasBlockingHit == true)
            {
                float const maxDistance {raycastBuffer.hits[hitCount - 1].distance};
                uint32 testHitCount     {hitCount - 1};

                for (uint32 index   {testHitCount}; index-- > 0;)
                {
                    if (raycastBuffer.hits[index].distance >= maxDistance)
                    {
                        raycastBuffer.hits[index] = raycastBuffer.hits[--testHitCount];
                    }
                }

                if (testHitCount < hitCount - 1)
                {
                    raycastBuffer.hits[testHitCount] = raycastBuffer.hits[hitCount - 1];
                    hitCount = testHitCount + 1;
                }
            }

            LOG(LogPhysics, Error, "%f", raycastBuffer.getAnyHit(0).position.z);
            LOG(LogPhysics, Error, "%f", raycastBuffer.getAnyHit(1).position.z);

            if (hitCount > 0)
            {
                if (!PhysicsInterface::ConvertQueryRaycastResults(hasBlockingHit, raycastBuffer, p_outHitResults, deltaMagnitude, filterData, p_traceStart, p_traceEnd, p_queryParams.m_returnFaceIndex, p_queryParams.m_returnPhysicalMaterial))
                {
                    LOG(LogPhysics, Error, "RaycastMulti resulted in a NaN/Inf value in hit results");
                }
            }
        }
    }

    return hasBlockingHit;
}

bool    PhysicsSceneHandle::RaycastSingle               (HitResult&                         p_outHitResult,
                                                         Vector3 const&                     p_traceStart,
                                                         Vector3 const&                     p_traceEnd,
                                                         ECollisionChannel const            p_traceChannel,
                                                         CollisionQueryParams const&        p_queryParams,
                                                         CollisionResponseParams const&     p_responseParams,
                                                         CollisionObjectQueryParams const&  p_objectQueryParams)  const
{
    bool hasBlockingHit         {false};

    p_outHitResult              = HitResult();
    p_outHitResult.m_traceStart = p_traceStart;
    p_outHitResult.m_traceEnd   = p_traceEnd;

    if (m_physXScene != nullptr)
    {

        Vector3 const   delta           {p_traceStart, p_traceEnd};
        float const     deltaMagnitude  {delta.GetMagnitude()};

        if (Math::IsNearlyZero(deltaMagnitude) == false)
        {
            physx::PxVec3 const             direction           {PhysXTypeConverter::TVector3(delta / deltaMagnitude)};
            physx::PxVec3 const             start               {PhysXTypeConverter::TVector3(p_traceStart)};

            physx::PxRaycastBuffer          raycastBuffer;
            physx::PxFilterData             filterData          {PhysicsInterface::CreateQueryFilterData(p_traceChannel, 
                                                                                                         p_queryParams.m_traceComplex, 
                                                                                                         p_responseParams.m_collisionResponseContainer,
                                                                                                         p_queryParams,
                                                                                                         p_objectQueryParams,
                                                                                                         false)};
            physx::PxSceneQueryFilterData   queryFilterData     {filterData, 
                                                                 p_queryParams.GetPhysXQueryMobilityFlags() | 
                                                                 physx::PxQueryFlag::ePREFILTER};

            physx::PxHitFlags               hitFlags            {physx::PxHitFlag::ePOSITION   | 
                                                                 physx::PxHitFlag::eNORMAL     | 
                                                                 physx::PxHitFlag::eDISTANCE   | 
                                                                 physx::PxHitFlag::eMTD        | 
                                                                 physx::PxHitFlag::eFACE_INDEX};

            PhysicsQueryFilterCallback      queryFilterCallback {p_queryParams};
            queryFilterCallback.m_ignoreTouches = true;

            m_physXScene->raycast(start,
                                  direction, 
                                  deltaMagnitude, 
                                  raycastBuffer, 
                                  hitFlags, 
                                  queryFilterData, 
                                  &queryFilterCallback);
            
            hasBlockingHit = raycastBuffer.hasBlock;
        
            // Fill hit result if an object has been hit by raycast
            if (hasBlockingHit == true)
            {
                physx::PxTransform startTransform {PhysXTypeConverter::TVector3(p_traceStart)};
                if (!PhysicsInterface::ConvertQueryImpactHit(raycastBuffer.block, p_outHitResult, deltaMagnitude, filterData, p_traceStart, p_traceEnd, nullptr, startTransform, p_queryParams.m_returnFaceIndex, p_queryParams.m_returnPhysicalMaterial))
                {
                    hasBlockingHit = false;
                    LOG(LogPhysics, Error, "RaycastSingle resulted in a NaN/INF in hit result!");
                }
            }
        }
    }

    return hasBlockingHit;
}

bool    PhysicsSceneHandle::RaycastTest                 (Vector3 const&                     p_traceStart,
                                                         Vector3 const&                     p_traceEnd,
                                                         ECollisionChannel const            p_traceChannel,
                                                         CollisionQueryParams const&        p_queryParams,
                                                         CollisionResponseParams const&     p_responseParams,
                                                         CollisionObjectQueryParams const&  p_objectQueryParams /** = CollisionObjectQueryParams::DefaultObjectQueryParams */)  const
{
    bool hasBlockingHit {false};

    if (m_physXScene != nullptr)
    {

        Vector3 const   delta           {p_traceStart, p_traceEnd};
        float const     deltaMagnitude  {delta.GetMagnitude()};

        if (Math::IsNearlyZero(deltaMagnitude) == false)
        {
            physx::PxVec3 const             direction           {PhysXTypeConverter::TVector3(delta / deltaMagnitude)};
            
            physx::PxRaycastBuffer          raycastBuffer;
            physx::PxFilterData             filterData          {PhysicsInterface::CreateQueryFilterData(p_traceChannel, 
                                                                                                         p_queryParams.m_traceComplex, 
                                                                                                         p_responseParams.m_collisionResponseContainer,
                                                                                                         p_queryParams,
                                                                                                         p_objectQueryParams,
                                                                                                         false)};
            physx::PxSceneQueryFilterData   queryFilterData     {filterData, 
                                                                 p_queryParams.GetPhysXQueryMobilityFlags() | 
                                                                 physx::PxQueryFlag::ePREFILTER             |
                                                                 physx::PxQueryFlag::eANY_HIT};

            physx::PxHitFlags               hitFlags            {physx::PxHitFlags()};

            PhysicsQueryFilterCallback      queryFilterCallback {p_queryParams};

            m_physXScene->raycast(PhysXTypeConverter::TVector3(p_traceStart),
                                  direction, 
                                  deltaMagnitude, 
                                  raycastBuffer, 
                                  hitFlags, 
                                  queryFilterData, 
                                  &queryFilterCallback);
            
            hasBlockingHit = raycastBuffer.hasBlock;
        }
    }

    return hasBlockingHit;
}


// ==============================[Private Local Methods]============================== //

physx::PxGeometry*  PhysicsSceneHandle::CreateGeometryPtr       (CollisionShape const& p_collisionShape)    const
{
    physx::PxGeometry* result = nullptr;

    switch (p_collisionShape.m_shapeType)
    {
        case ECollisionShapeType::Box       : result = new physx::PxBoxGeometry(PhysXTypeConverter::TVector3(p_collisionShape.GetBoxHalfExtent()));  break;
        case ECollisionShapeType::Capsule   : result = new physx::PxCapsuleGeometry(p_collisionShape.GetCapsuleRadius(), p_collisionShape.GetCapsuleHalfHeight()); break;
        case ECollisionShapeType::Sphere    : result = new physx::PxSphereGeometry(p_collisionShape.GetSphereRadius()); break;
    }

    return result;
}

void                PhysicsSceneHandle::UpdateBodyTransforms    ()
{
    uint32              activeActorsCount   {0u};
    physx::PxActor**    activeActors        {m_physXScene->getActiveActors(activeActorsCount)};

    for (uint32 index {0u} ; index < activeActorsCount ; ++index)
    {
        physx::PxRigidActor* actor   {static_cast<physx::PxRigidActor*>(activeActors[index])};
        if (actor != nullptr)
        {
            BodyInstance* bodyInstance  {static_cast<BodyInstance*>(actor->userData)};
            if (bodyInstance != nullptr)
            {
                PrimitiveComponent* component   {bodyInstance->m_ownerComponent};
                if (component)
                {
                    Transform const transform {PhysXTypeConverter::TTransform(actor->getGlobalPose())};
                    component->SetWorldLocationAndRotation(transform.m_translation, transform.m_rotation);
                }
            }
        }
    }
}
