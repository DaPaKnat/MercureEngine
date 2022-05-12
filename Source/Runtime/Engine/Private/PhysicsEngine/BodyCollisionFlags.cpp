#include "PCH.hpp"

#include "PhysicsEngine/BodyCollisionFlags.hpp"

// ==============================[Public Constructor + Destructor]============================== //

/**
 * Default Constructor. All properties are initializes to false.
 */
BodyCollisionFlags::BodyCollisionFlags  ()
:   m_enableSimulationCollisionSimple   {false},
    m_enableSimulationCollisionComplex  {false},
    m_enableQueryCollision              {false}
{}

/**
 * Constructor using initial values.
 *
 * @param p_enableSimulationCollisionSimple     TODO
 * @param p_enableSimulationCollisionComplex    TODO
 * @param p_enableQueryCollision                TODO
 */
BodyCollisionFlags::BodyCollisionFlags  (bool const                 p_enableSimulationCollisionSimple,
                                         bool const                 p_enableSimulationCollisionComplex,
                                         bool const                 p_enableQueryCollision)
:   m_enableSimulationCollisionSimple   {p_enableSimulationCollisionSimple},
    m_enableSimulationCollisionComplex  {p_enableSimulationCollisionComplex},
    m_enableQueryCollision              {p_enableQueryCollision}
{}

/**
 * Copy Constructor.
 *
 * @param p_copy    The body collision to copy .
 */
BodyCollisionFlags::BodyCollisionFlags  (BodyCollisionFlags const&  p_copy)
:   m_enableSimulationCollisionSimple   {p_copy.m_enableSimulationCollisionSimple},
    m_enableSimulationCollisionComplex  {p_copy.m_enableSimulationCollisionComplex},
    m_enableQueryCollision              {p_copy.m_enableQueryCollision}
{}

/**
 * Move Constructor.
 *
 * @param p_copy    The body collision to move.
 */
BodyCollisionFlags::BodyCollisionFlags  (BodyCollisionFlags&&       p_move)
:   m_enableSimulationCollisionSimple   {std::move(p_move.m_enableSimulationCollisionSimple)},
    m_enableSimulationCollisionComplex  {std::move(p_move.m_enableSimulationCollisionComplex)},
    m_enableQueryCollision              {std::move(p_move.m_enableQueryCollision)}
{}

// ==============================[Public Local Operators]============================== //

/**
 * TODO
 */
bool                BodyCollisionFlags::operator==  (BodyCollisionFlags const&  p_other)    const
{
    return  m_enableSimulationCollisionSimple   == p_other.m_enableSimulationCollisionSimple    &&
            m_enableSimulationCollisionComplex  == p_other.m_enableSimulationCollisionComplex   &&
            m_enableQueryCollision              == p_other.m_enableQueryCollision;
}

/**
 * TODO
 */
bool                BodyCollisionFlags::operator!=  (BodyCollisionFlags const&  p_other)    const
{
    return  m_enableSimulationCollisionSimple   != p_other.m_enableSimulationCollisionSimple    ||
            m_enableSimulationCollisionComplex  != p_other.m_enableSimulationCollisionComplex   ||
            m_enableQueryCollision              != p_other.m_enableQueryCollision;
}

/**
 * TODO
 */
BodyCollisionFlags& BodyCollisionFlags::operator=   (BodyCollisionFlags const&  p_copy)
{

    m_enableSimulationCollisionSimple   = p_copy.m_enableSimulationCollisionSimple;
    m_enableSimulationCollisionComplex  = p_copy.m_enableSimulationCollisionComplex;
    m_enableQueryCollision              = p_copy.m_enableQueryCollision;

    return *this;
}   

/**
 * TODO
 */
BodyCollisionFlags& BodyCollisionFlags::operator=   (BodyCollisionFlags&&       p_move)
{

    m_enableSimulationCollisionSimple   = std::move(p_move.m_enableSimulationCollisionSimple);
    m_enableSimulationCollisionComplex  = std::move(p_move.m_enableSimulationCollisionComplex);
    m_enableQueryCollision              = std::move(p_move.m_enableQueryCollision);

    return *this;
}   