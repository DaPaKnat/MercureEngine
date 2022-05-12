#include "PCH.hpp"

#include "PhysicsEngine/CollisionObjectQueryParams.hpp"

// ==============================[Public Static Properties]============================== //

CollisionObjectQueryParams  CollisionObjectQueryParams::DefaultObjectQueryParams;

// ==============================[Public Local Constructor + Destructor]============================== //

CollisionObjectQueryParams::CollisionObjectQueryParams  ()
:   m_objectTypeToQuery {0}
{}

CollisionObjectQueryParams::CollisionObjectQueryParams      (ECollisionChannel const                p_queryChannel)
:   m_objectTypeToQuery {1<<static_cast<int32>(p_queryChannel)}
{}

CollisionObjectQueryParams::CollisionObjectQueryParams      (std::vector<ECollisionChannel> const&  p_objectTypes)
:   m_objectTypeToQuery {0}
{
    for (auto it {p_objectTypes.begin()}; it != p_objectTypes.end(); ++it)
    {
        AddObjectTypeToQuery(*it);
    }
}

// ==============================[Public Local Methods]============================== //

void    CollisionObjectQueryParams::AddObjectTypeToQuery    (ECollisionChannel const    p_objectTypeToQuery)
{
    m_objectTypeToQuery |= (1 << static_cast<int32>(p_objectTypeToQuery));
}

int32   CollisionObjectQueryParams::GetQueryBitfield        ()  const
{
    return m_objectTypeToQuery;
}

bool    CollisionObjectQueryParams::IsValid                 ()  const
{
    return m_objectTypeToQuery != 0;
}

void    CollisionObjectQueryParams::RemoveObjectTypeToQuery (ECollisionChannel const    p_objectTypeToQuery)
{
    m_objectTypeToQuery &= ~(1 << static_cast<int32>(p_objectTypeToQuery));
}

void    CollisionObjectQueryParams::SetQueryBitfield        (int32 const                p_newQueryBitfield)
{
    m_objectTypeToQuery = p_newQueryBitfield;
}

// ==============================[Public Local Operators]============================== //

bool                        CollisionObjectQueryParams::operator==  (CollisionObjectQueryParams const&  p_other)  const
{
    return m_objectTypeToQuery == p_other.m_objectTypeToQuery;
}

bool                        CollisionObjectQueryParams::operator!=  (CollisionObjectQueryParams const&  p_other)  const
{
    return m_objectTypeToQuery != p_other.m_objectTypeToQuery;
}

CollisionObjectQueryParams& CollisionObjectQueryParams::operator=   (CollisionObjectQueryParams const&  p_copy)
{
    m_objectTypeToQuery = p_copy.m_objectTypeToQuery;

    return *this;
}

CollisionObjectQueryParams& CollisionObjectQueryParams::operator=   (CollisionObjectQueryParams&&       p_move)
{
    m_objectTypeToQuery = std::move(p_move.m_objectTypeToQuery);

    return *this;
}