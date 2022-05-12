#ifndef __PHYSICS_SCENE_HANDLE_INL__
#define __PHYSICS_SCENE_HANDLE_INL__

// ==============================[Public Local Methods]============================== //

FORCEINLINE World*  PhysicsSceneHandle::GetOwningWorld  ()  const
{
    return m_owningWorld;
}

FORCEINLINE void    PhysicsSceneHandle::SetOwningWorld  (World* p_newOwningWorld)
{
    m_owningWorld = p_newOwningWorld;
}

// ==============================[Public Local Methods]============================== //

template <bool t_isBlocking>
bool    PhysicsSceneHandle::GeometryOverlapTestImp  (physx::PxGeometry const&           p_geometry,
                                                     physx::PxTransform const&          p_geometryTransform, 
                                                     ECollisionChannel                  p_traceChannel, 
                                                     CollisionQueryParams const&        p_queryParams, 
                                                     CollisionResponseParams const&     p_responseParams, 
                                                     CollisionObjectQueryParams const&  p_objectQueryParams)    const
{
    bool hasBlockingHit {false};

            
    physx::PxOverlapBuffer          overlapBuffer;
    physx::PxFilterData             filterData          {PhysicsInterface::CreateQueryFilterData(p_traceChannel, 
                                                                                                 p_queryParams.m_traceComplex, 
                                                                                                 p_responseParams.m_collisionResponseContainer,
                                                                                                 p_queryParams,
                                                                                                 p_objectQueryParams,
                                                                                                 t_isBlocking)};
    physx::PxSceneQueryFilterData   queryFilterData     {filterData, 
                                                         p_queryParams.GetPhysXQueryMobilityFlags() | 
                                                         physx::PxQueryFlag::ePREFILTER             |
                                                         physx::PxQueryFlag::eANY_HIT};

    physx::PxHitFlags               hitFlags            {physx::PxHitFlags()};

    PhysicsQueryFilterCallback      queryFilterCallback {p_queryParams};
    queryFilterCallback.m_ignoreTouches  = t_isBlocking;
    queryFilterCallback.m_isOverlapQuery = true;


    m_physXScene->overlap(p_geometry, p_geometryTransform, overlapBuffer, queryFilterData, &queryFilterCallback);
    hasBlockingHit = overlapBuffer.hasBlock;

    return hasBlockingHit;
}

#endif // !__PHYSICS_SCENE_HANDLE_INL__