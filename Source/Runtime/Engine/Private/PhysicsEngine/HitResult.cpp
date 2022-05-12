#include "PCH.hpp"

#include "PhysicsEngine/HitResult.hpp"

// ==============================[Public Constructor + Destructor]============================== //

/**
 * Default Constructor.
 */
HitResult::HitResult    ()
:   m_entity                {nullptr},
    m_distance              {0.0f},
    m_impactLocation        {Vector3::Zero},
    m_impactNormal          {Vector3::Zero},
    m_isBlockingHit         {false},
    m_location              {Vector3::Zero},
    m_normal                {Vector3::Zero},
    m_penetrationDepth      {0.0f},
    m_primitiveComponent    {nullptr},
    m_traceEnd              {Vector3::Zero},
    m_traceStart            {Vector3::Zero},
    m_startPenetrating      {false},
    m_hitTime               {0.0f}
{}

/**
 * Contructor where we initialize start and end trace location.
 *
 * @param p_startTrace  The start location of the trace.
 * @param p_endTrace    The end location of the trace.
 */
HitResult::HitResult    (Vector3 const&     p_startTrace,
                         Vector3 const&     p_endTrace)
:   m_entity                {nullptr},
    m_distance              {0.0f},
    m_impactLocation        {Vector3::Zero},
    m_impactNormal          {Vector3::Zero},
    m_isBlockingHit         {false},
    m_location              {Vector3::Zero},
    m_normal                {Vector3::Zero},
    m_penetrationDepth      {0.0f},
    m_primitiveComponent    {nullptr},
    m_traceEnd              {p_startTrace},
    m_traceStart            {p_endTrace},
    m_startPenetrating      {false},
    m_hitTime               {0.0f}
{}

/**
 * Copy Constructor.
 */
HitResult::HitResult    (HitResult const&   p_copy)
:   m_entity                {p_copy.m_entity},
    m_distance              {p_copy.m_distance},
    m_impactLocation        {p_copy.m_impactLocation},
    m_impactNormal          {p_copy.m_impactNormal},
    m_isBlockingHit         {p_copy.m_isBlockingHit},
    m_location              {p_copy.m_location},
    m_normal                {p_copy.m_normal},
    m_penetrationDepth      {p_copy.m_penetrationDepth},
    m_primitiveComponent    {p_copy.m_primitiveComponent},
    m_traceEnd              {p_copy.m_traceEnd},
    m_traceStart            {p_copy.m_traceStart},
    m_startPenetrating      {p_copy.m_startPenetrating},
    m_hitTime               {p_copy.m_hitTime}
{}

/**
 * Move Constructor.
 */
HitResult::HitResult    (HitResult&&        p_move)
:   m_entity                {std::move(p_move.m_entity)},
    m_distance              {std::move(p_move.m_distance)},
    m_impactLocation        {std::move(p_move.m_impactLocation)},
    m_impactNormal          {std::move(p_move.m_impactNormal)},
    m_isBlockingHit         {std::move(p_move.m_isBlockingHit)},
    m_location              {std::move(p_move.m_location)},
    m_normal                {std::move(p_move.m_normal)},
    m_penetrationDepth      {std::move(p_move.m_penetrationDepth)},
    m_primitiveComponent    {std::move(p_move.m_primitiveComponent)},
    m_traceEnd              {std::move(p_move.m_traceEnd)},
    m_traceStart            {std::move(p_move.m_traceStart)},
    m_startPenetrating      {std::move(p_move.m_startPenetrating)},
    m_hitTime               {std::move(p_move.m_hitTime)}
{}

// ==============================[Public Local Operators]============================== //


HitResult&  HitResult::operator=    (HitResult const&   p_copy)
{
    m_entity                = p_copy.m_entity;
    m_distance              = p_copy.m_distance;
    m_impactLocation        = p_copy.m_impactLocation;
    m_impactNormal          = p_copy.m_impactNormal;
    m_isBlockingHit         = p_copy.m_isBlockingHit;
    m_location              = p_copy.m_location;
    m_normal                = p_copy.m_normal;
    m_penetrationDepth      = p_copy.m_penetrationDepth;
    m_primitiveComponent    = p_copy.m_primitiveComponent;
    m_traceEnd              = p_copy.m_traceEnd;
    m_traceStart            = p_copy.m_traceStart;
    m_startPenetrating      = p_copy.m_startPenetrating;
    m_hitTime               = p_copy.m_hitTime;

    return *this;
}

HitResult&  HitResult::operator=    (HitResult&&        p_move)
{
    m_entity                = std::move(p_move.m_entity);
    m_distance              = std::move(p_move.m_distance);
    m_impactLocation        = std::move(p_move.m_impactLocation);
    m_impactNormal          = std::move(p_move.m_impactNormal);
    m_isBlockingHit         = std::move(p_move.m_isBlockingHit);
    m_location              = std::move(p_move.m_location);
    m_normal                = std::move(p_move.m_normal);
    m_penetrationDepth      = std::move(p_move.m_penetrationDepth);
    m_primitiveComponent    = std::move(p_move.m_primitiveComponent);
    m_traceEnd              = std::move(p_move.m_traceEnd);
    m_traceStart            = std::move(p_move.m_traceStart);
    m_startPenetrating      = std::move(p_move.m_startPenetrating);
    m_hitTime               = std::move(p_move.m_hitTime);

    return *this;
}