#include "PCH.hpp"

#include "PhysicsEngine/BodyCollisionData.hpp"

// ==============================[Public Local Constructor + Destructorc]============================== //

BodyCollisionData::BodyCollisionData    (BodyCollisionFlags const&      p_collisionFlags,
                                         BodyCollisionFilterData const& p_collisionFilterData)
:   m_collisionFlags        {p_collisionFlags},
    m_collisionFilterData   {p_collisionFilterData}
{}

BodyCollisionData::BodyCollisionData    (BodyCollisionData const&       p_copy)
:   m_collisionFlags        {p_copy.m_collisionFlags},
    m_collisionFilterData   {p_copy.m_collisionFilterData}
{}

BodyCollisionData::BodyCollisionData    (BodyCollisionData&&            p_move)
:   m_collisionFlags        {std::move(p_move.m_collisionFlags)},
    m_collisionFilterData   {std::move(p_move.m_collisionFilterData)}
{}

// ==============================[Public Local Operators]============================== //

bool                BodyCollisionData::operator==   (BodyCollisionData const&   p_other)    const
{
    return  m_collisionFlags        ==  p_other.m_collisionFlags        &&
            m_collisionFilterData   ==  p_other.m_collisionFilterData;
}

bool                BodyCollisionData::operator!=   (BodyCollisionData const&   p_other)    const
{
    return  m_collisionFlags        !=  p_other.m_collisionFlags        ||
            m_collisionFilterData   !=  p_other.m_collisionFilterData;
}

BodyCollisionData&  BodyCollisionData::operator=    (BodyCollisionData const&   p_copy)
{
    m_collisionFlags        = p_copy.m_collisionFlags;
    m_collisionFilterData   = p_copy.m_collisionFilterData;

    return *this;
}

BodyCollisionData&  BodyCollisionData::operator=    (BodyCollisionData&&        p_move)
{
    m_collisionFlags        = std::move(p_move.m_collisionFlags);
    m_collisionFilterData   = std::move(p_move.m_collisionFilterData);

    return *this;
}