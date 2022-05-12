#include "PCH.hpp"

#include "PhysicsEngine/BodyCollisionFilterData.hpp"

// ==============================[Public Constructor + Destructor]============================== //

BodyCollisionFilterData::BodyCollisionFilterData    (CollisionFilterData const&     p_simulationFilter,
                                                     CollisionFilterData const&     p_querySimplefilter,
                                                     CollisionFilterData const&     p_queryComplexFilter)
:   m_simulationFilter      {p_simulationFilter},
    m_querySimpleFilter     {p_querySimplefilter},
    m_queryComplexFilter    {p_queryComplexFilter}
{}

BodyCollisionFilterData::BodyCollisionFilterData    (BodyCollisionFilterData const& p_copy)
:   m_simulationFilter      {p_copy.m_simulationFilter},
    m_querySimpleFilter     {p_copy.m_querySimpleFilter},
    m_queryComplexFilter    {p_copy.m_queryComplexFilter}
{}

BodyCollisionFilterData::BodyCollisionFilterData    (BodyCollisionFilterData&&      p_move)
:   m_simulationFilter      {std::move(p_move.m_simulationFilter)},
    m_querySimpleFilter     {std::move(p_move.m_querySimpleFilter)},
    m_queryComplexFilter    {std::move(p_move.m_queryComplexFilter)}
{}

// ==============================[Public Local Operators]============================== //

bool                        BodyCollisionFilterData::operator== (BodyCollisionFilterData const& p_other)    const   
{
    return  m_simulationFilter      ==  p_other.m_simulationFilter      &&
            m_querySimpleFilter     ==  p_other.m_querySimpleFilter     &&
            m_queryComplexFilter    ==  p_other.m_queryComplexFilter;
}

bool                        BodyCollisionFilterData::operator!= (BodyCollisionFilterData const& p_other)    const
{
    return  m_simulationFilter      !=  p_other.m_simulationFilter      ||
            m_querySimpleFilter     !=  p_other.m_querySimpleFilter     ||
            m_queryComplexFilter    !=  p_other.m_queryComplexFilter;
}

BodyCollisionFilterData&    BodyCollisionFilterData::operator=  (BodyCollisionFilterData const& p_copy)
{
    m_simulationFilter      =   p_copy.m_simulationFilter;
    m_querySimpleFilter     =   p_copy.m_querySimpleFilter;
    m_queryComplexFilter    =   p_copy.m_queryComplexFilter;

    return *this;
}

BodyCollisionFilterData&    BodyCollisionFilterData::operator=  (BodyCollisionFilterData&&      p_move)
{
    m_simulationFilter      =   std::move(p_move.m_simulationFilter);
    m_querySimpleFilter     =   std::move(p_move.m_querySimpleFilter);
    m_queryComplexFilter    =   std::move(p_move.m_queryComplexFilter);

    return *this;
}