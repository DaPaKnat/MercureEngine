#include "PCH.hpp"

#include "PhysicsEngine/CollisionResponseParams.hpp"

// ==============================[Public Static Properties]============================== //

CollisionResponseParams CollisionResponseParams::DefaultResponseParams   {ECollisionResponse::Block};


// ==============================[Public Local Constructor + Destructor ]============================== //

CollisionResponseParams::CollisionResponseParams    (ECollisionResponse const           p_defaultResponse /** = ECollisionResponse::Block*/)
:   m_collisionResponseContainer    {p_defaultResponse}
{}

CollisionResponseParams::CollisionResponseParams    (CollisionResponseContainer const&  p_responseContainer)
:   m_collisionResponseContainer    {p_responseContainer}
{}

CollisionResponseParams::CollisionResponseParams    (CollisionResponseParams const&     p_copy)
:   m_collisionResponseContainer    {p_copy.m_collisionResponseContainer}
{}

CollisionResponseParams::CollisionResponseParams    (CollisionResponseParams&&          p_move)
:   m_collisionResponseContainer    {std::move(p_move.m_collisionResponseContainer)}
{}

// ==============================[Public Local Operators]============================== //

bool                        CollisionResponseParams::operator== (CollisionResponseParams const& p_other)    const
{
    return m_collisionResponseContainer == p_other.m_collisionResponseContainer;
}

bool                        CollisionResponseParams::operator!= (CollisionResponseParams const& p_other)    const
{
    return m_collisionResponseContainer != p_other.m_collisionResponseContainer;
}

CollisionResponseParams&    CollisionResponseParams::operator=  (CollisionResponseParams const& p_copy)
{
    m_collisionResponseContainer != p_copy.m_collisionResponseContainer;
    return *this;
}

CollisionResponseParams&    CollisionResponseParams::operator=  (CollisionResponseParams&&      p_move)
{
    m_collisionResponseContainer != std::move(p_move.m_collisionResponseContainer);
    return *this;
}