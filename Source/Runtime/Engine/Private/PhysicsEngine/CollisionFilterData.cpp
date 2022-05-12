#include "PCH.hpp"

#include "PhysicsEngine/CollisionFilterData.hpp"

// ==============================[Public Local Constructor + Destructor]============================== //

CollisionFilterData::CollisionFilterData    ()
:   m_word0 {0u},
    m_word1 {0u},
    m_word2 {0u},
    m_word3 {0u}
{}


CollisionFilterData::CollisionFilterData    (uint32 const               p_word0,
                                             uint32 const               p_word1,
                                             uint32 const               p_word2,
                                             uint32 const               p_word3)
:   m_word0 {p_word0},
    m_word1 {p_word1},
    m_word2 {p_word2},
    m_word3 {p_word3}
{}


CollisionFilterData::CollisionFilterData    (CollisionFilterData const& p_copy)

:   m_word0 {p_copy.m_word0},
    m_word1 {p_copy.m_word1},
    m_word2 {p_copy.m_word2},
    m_word3 {p_copy.m_word3}
{}


CollisionFilterData::CollisionFilterData    (CollisionFilterData&&      p_move)

:   m_word0 {std::move(p_move.m_word0)},
    m_word1 {std::move(p_move.m_word1)},
    m_word2 {std::move(p_move.m_word2)},
    m_word3 {std::move(p_move.m_word3)}
{}

// ==============================[Public Local Operators]============================== //

bool                    CollisionFilterData::operator==  (CollisionFilterData const&  p_other)    const
{
    return  m_word0 == p_other.m_word0  &&
            m_word1 == p_other.m_word1  &&
            m_word2 == p_other.m_word2  &&
            m_word3 == p_other.m_word3;
}


bool                    CollisionFilterData::operator!=  (CollisionFilterData const&  p_other)    const
{
    return  m_word0 != p_other.m_word0  ||
            m_word1 != p_other.m_word1  ||
            m_word2 != p_other.m_word2  ||
            m_word3 != p_other.m_word3;
}

CollisionFilterData&    CollisionFilterData::operator=   (CollisionFilterData const&  p_copy)
{

    m_word0 = p_copy.m_word0;
    m_word1 = p_copy.m_word1;
    m_word2 = p_copy.m_word2;
    m_word3 = p_copy.m_word3;

    return *this;
}   

CollisionFilterData&    CollisionFilterData::operator=   (CollisionFilterData&&       p_move)
{

    m_word0  = std::move(p_move.m_word0);
    m_word1  = std::move(p_move.m_word1);
    m_word2  = std::move(p_move.m_word2);
    m_word3  = std::move(p_move.m_word3);

    return *this;
}   