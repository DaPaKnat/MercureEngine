#include "PCH.hpp"

#include "PhysicsEngine/PhysicsQueryFilterCallbackSweep.hpp"

// ==============================[Public Local Properties]============================== //

PhysicsQueryFilterCallbackSweep::PhysicsQueryFilterCallbackSweep    (CollisionQueryParams const& p_queryParams)
    : Base(p_queryParams)
{
    m_discardInitialOverlaps = !p_queryParams.m_findInitialOverlaps;
}

// ==============================[Public Local Methods]============================== //

physx::PxQueryHitType::Enum PhysicsQueryFilterCallbackSweep::postFilter (physx::PxFilterData const& p_filterData,
                                                                         physx::PxQueryHit const&   p_queryHit)
{
    physx::PxSweepHit& sweepHit     {(physx::PxSweepHit&)p_queryHit};
    bool const hadInitialOverlap    {sweepHit.hadInitialOverlap()};

    if (hadInitialOverlap && m_discardInitialOverlaps && sweepHit.distance != 0.0f)
    {
        return physx::PxQueryHitType::eNONE;
    }

    else
    {
        if (hadInitialOverlap && m_preFilterReturnValue == physx::PxQueryHitType::eBLOCK && sweepHit.distance != 0.0f)
        {
            return physx::PxQueryHitType::eTOUCH;
        }

        return m_preFilterReturnValue;
    }
}