#include "PCH.hpp"

#include "PhysicsEngine/OverlapInfo.hpp"
#include "Components/PrimitiveComponent.hpp"

// ==============================[Public Constructor + Destructor]============================== //

OverlapInfo::OverlapInfo    (HitResult const&           p_hitResult,
                             bool const                 p_fromSweep)
 :   m_overlapInfo   {p_hitResult},
     m_fromSweep     {p_fromSweep}
 {}

OverlapInfo::OverlapInfo    (PrimitiveComponent* const  p_component,
                             bool const                 p_fromSweep /** = false */)
 :   m_fromSweep     {p_fromSweep}
 {
     m_overlapInfo.m_entity = p_component ? p_component->GetOwner() : nullptr;
     m_overlapInfo.m_primitiveComponent = p_component;
 }

OverlapInfo::OverlapInfo    (OverlapInfo const&         p_copy)
 :   m_overlapInfo   {p_copy.m_overlapInfo},
     m_fromSweep     {p_copy.m_fromSweep}
 {}

OverlapInfo::OverlapInfo    (OverlapInfo&&              p_move)
 :   m_overlapInfo   {std::move(p_move.m_overlapInfo)},
     m_fromSweep     {std::move(p_move.m_fromSweep)}
 {}

// ==============================[Public Local Operators]============================== //

bool            OverlapInfo::operator== (OverlapInfo const& p_other)    const
{
    return  m_overlapInfo.m_entity == p_other.m_overlapInfo.m_entity &&
            m_overlapInfo.m_primitiveComponent == p_other.m_overlapInfo.m_primitiveComponent;
}

bool            OverlapInfo::operator!= (OverlapInfo const& p_other)    const
{
    return !operator==(p_other);
}

OverlapInfo&    OverlapInfo::operator=  (OverlapInfo const& p_copy)
{
    m_overlapInfo   = p_copy.m_overlapInfo;
    m_fromSweep     = p_copy.m_fromSweep;

    return *this;
}

OverlapInfo&    OverlapInfo::operator=  (OverlapInfo&&      p_move)
{
    m_overlapInfo   = std::move(p_move.m_overlapInfo);
    m_fromSweep     = std::move(p_move.m_fromSweep);

    return *this;
}