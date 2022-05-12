#include "PCH.hpp"

#include "PlayerScripts/FindFloorResult.hpp"

// ==============================[Public Local Methods]============================== //

void    FindFloorResult::Clear              ()
{
    m_isBlockingHit     = false;
    m_isWalkableFloor   = false;
    m_lineTrace         = false;
    m_floorDistance     = 0.f;
    m_lineDistance      = 0.f;

    m_hitResult         = HitResult();
}

float   FindFloorResult::GetDistanceToFloor ()  const
{
    return m_lineTrace ? m_lineDistance : m_floorDistance;
}

bool    FindFloorResult::IsWalkableFloor    ()  const
{
    return m_isBlockingHit && m_isWalkableFloor;
}

void    FindFloorResult::SetFromLineTrace   (HitResult const&   p_inHit,
                                             float const        p_inSweepFloorDistance,
                                             float const        p_inLineDistance,
                                             bool const         p_isWalkableFloor)
{
    if (m_hitResult.m_isBlockingHit && p_inHit.m_isBlockingHit)
    {
        HitResult oldHit    {m_hitResult};
        m_hitResult = p_inHit;

        m_hitResult.m_hitTime           = oldHit.m_hitTime;
        m_hitResult.m_impactLocation    = oldHit.m_impactLocation;
        m_hitResult.m_location          = oldHit.m_location;
        m_hitResult.m_traceStart        = oldHit.m_traceStart;
        m_hitResult.m_traceEnd          = oldHit.m_traceEnd;

        m_lineTrace                     = true;
        m_floorDistance                 = p_inSweepFloorDistance;
        m_lineDistance                  = p_inLineDistance;
        m_isWalkableFloor               = p_isWalkableFloor;
    }
}

void    FindFloorResult::SetFromSweep       (HitResult const&   p_inHit,
                                             float const        p_inSweepFloorDistance,
                                             bool const         p_isWalkableFloor)
{
    m_isBlockingHit     = p_inHit.IsValidBlockingHit();
	m_isWalkableFloor   = p_isWalkableFloor;
	m_lineTrace         = false;
	m_floorDistance     = p_inSweepFloorDistance;
	m_lineDistance      = 0.f;
    m_hitResult         = p_inHit;
}