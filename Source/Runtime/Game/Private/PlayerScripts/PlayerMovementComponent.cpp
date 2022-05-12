#include "PCH.hpp"

#include "PlayerScripts/PlayerMovementComponent.hpp"

#include "PhysicsEngine/CollisionQueryParams.hpp"
#include "PhysicsEngine/CollisionResponseParams.hpp"

#include "World.hpp"

DEFINE_LOG_CATEGORY(LogPlayerMovement);

// =================================[Virtual Public Local Methods]================================= //

void    PlayerMovementComponent::BeginPlay      ()
{
    Super::BeginPlay();

    SetupPlayerOwner();
    SetupUpdatedPrimitive();
    UpdateWalkableFloorY();
}

void    PlayerMovementComponent::Tick           (float const    p_deltaTime)
{
    Super::Tick(p_deltaTime);

    Vector3 const movementInput {ConsumePlayerMovementInput()};

    if (!ShouldSkipUpdate(p_deltaTime))
    {
        m_acceleration = ScaleInputAcceleration(ConstrainInputAcceleration(movementInput));

        PerformMovement(p_deltaTime);
    }
}

void    PlayerMovementComponent::Serialize      (Json&          p_serialize)    const
{
    p_serialize = *this;
}

void    PlayerMovementComponent::Deserialize    (Json const&    p_deserialize,
                                                 Json const&)
{
    *this = PlayerMovementComponent();
    *this = p_deserialize;

}

// ==============================[Public Local Methods]============================== //

void        PlayerMovementComponent::ApplyVelocityBraking           (float                          p_deltaTime,
                                                                     float                          p_friction,
                                                                     float                          p_brakingDeceleration)
{
    if (m_velocity.IsZero() || !HasValidData() || p_deltaTime < MATH_EPSILON_F)
    {
        return;
    }

    float const frictionFactor  {Math::Max(0.f, m_brakingFrictionFactor)};
    p_friction = Math::Max(0.0f, p_friction * frictionFactor);
    p_brakingDeceleration = Math::Max(0.0f, p_brakingDeceleration);
    bool const zeroFriction {p_friction == 0.f};
    bool const zeroBraking  {p_brakingDeceleration == 0.f};

    if (zeroFriction && zeroBraking)
    {
        return;
    }

    Vector3 const oldVelocity   {m_velocity};

    float remainingTime {p_deltaTime};
    float const maxTimeStep {Math::Clamp(m_brakingSubStepMaxTime, 1.0f / 75.0f, 1.0f / 20.0f)};

    Vector3 const revAcceleration {zeroBraking ? Vector3::Zero : (-p_brakingDeceleration * m_velocity.GetSafeNormalized())};

    while (remainingTime >= MATH_EPSILON_F)
    {
        float const dt {(remainingTime > maxTimeStep && !zeroFriction) ? Math::Min(maxTimeStep, remainingTime * 0.5f) : remainingTime};
        remainingTime -= dt;

        m_velocity = m_velocity + ((-p_friction) * m_velocity + revAcceleration) * dt;

        if ((m_velocity | oldVelocity) <= 0.0f)
        {
            m_velocity = Vector3::Zero;
            return;
        }
    }

    float const velocityMagnitudeSquared {m_velocity.GetMagnitudeSquared()};
    if (velocityMagnitudeSquared < Math::Square(MATH_EPSILON_F) || (!zeroBraking && velocityMagnitudeSquared <= Math::Square(0.1f)))
    {
        m_velocity = Vector3::Zero;
    }
}

bool        PlayerMovementComponent::CanJump                        ()  const
{
    bool canJump    {IsMovingOnGround() || IsFalling()};

    if (canJump)
    {
        if (!m_wasJumping || GetJumpMaxHoldTime() <= 0.0f)
        {
            if (m_jumpCurrentCount == 0 && IsFalling())
            {
                canJump = m_jumpCurrentCount + 1 < m_jumpMaxCount;
            }

            else
            {
                canJump = m_jumpCurrentCount < m_jumpMaxCount;
            }
        }

        else
        {
            bool const jumpHeld = (m_jumpPressed && m_jumpCurrentHoldTime < GetJumpMaxHoldTime());
            canJump = jumpHeld && (m_jumpCurrentCount < m_jumpMaxCount || (m_wasJumping && m_jumpCurrentCount == m_jumpMaxCount));
        }
    }

    return canJump;
}

void        PlayerMovementComponent::ClearPendingForces             ()
{
    m_pendingForceToApply   = Vector3::Zero;
    m_pendingImpulseToApply = Vector3::Zero;
}

Vector3     PlayerMovementComponent::ConsumePlayerMovementInput     ()
{
    return m_playerOwner ? m_playerOwner->ConsumeMovementInput() : Vector3::Zero;
}

void        PlayerMovementComponent::ComputeFloorDistance           (Vector3 const&                 p_capsuleLocation,
                                                                     float const                    p_lineDistance,
                                                                     float const                    p_sweepDistance,
                                                                     FindFloorResult&               p_outFloorResult,
                                                                     float const                    p_sweepRadius,
                                                                     HitResult const*               p_downwardSweepResult /**= nullptr*/)    const
{
    p_outFloorResult.Clear();

    CapsuleComponent*   playerCapsuleCollider   {m_playerOwner->GetCapsuleCollider()};

    float const         playerRadius            {playerCapsuleCollider->GetScaledCapsuleRadius()};
    float const         playerHalfHeight        {playerCapsuleCollider->GetScaledCapsuleHalfHeight()};

    bool                skipSweep               {false};

    if (p_downwardSweepResult != nullptr && p_downwardSweepResult->IsValidBlockingHit())
    {
        Vector3 p_downwardSweepResultXZ {p_downwardSweepResult->m_traceStart - p_downwardSweepResult->m_traceEnd};
        p_downwardSweepResultXZ.m_y = 0.0f;

        if (p_downwardSweepResult->m_traceStart.m_y > p_downwardSweepResult->m_traceEnd.m_y &&
            p_downwardSweepResultXZ.GetMagnitudeSquared() <= MATH_EPSILON_F)
        {
            if (IsWithinEdgeTolerance(p_downwardSweepResult->m_location, p_downwardSweepResult->m_impactLocation, playerRadius))
            {
                skipSweep = true;

                bool const  isWalkable      {IsWalkable(*p_downwardSweepResult)};
                float const floorDistance   {p_capsuleLocation.m_y - p_downwardSweepResult->m_location.m_y};
                p_outFloorResult.SetFromSweep(*p_downwardSweepResult, floorDistance, isWalkable);

                if (isWalkable)
                {
                    return;
                }
            }
        }
    }

    if (p_sweepDistance < p_lineDistance)
        return;

    bool isBlockingHit  {false};
    CollisionQueryParams    queryParams;
    CollisionResponseParams responseParams;
    InitCollisionParams(queryParams, responseParams);
    ECollisionChannel const collisionChannel    {m_updatedPrimitive->GetCollisionObjectType()};

    if (!skipSweep && p_sweepDistance > 0.0f && p_sweepRadius > 0.0f)
    {
        float const             shrinkScale         {0.9f};
        float const             shrinkScaleOverlap  {0.1f};
        float                   shrinkHeight        {(playerHalfHeight - playerRadius) * (1-.0f - shrinkScale)};
        float                   traceDistance       {p_sweepDistance + shrinkHeight};
        CollisionShape          capsuleShape        {CollisionShape::MakeCapsule(p_sweepRadius, playerHalfHeight)};
        HitResult               hit;

        isBlockingHit = FloorSweepTest(hit, 
                                       p_capsuleLocation, 
                                       p_capsuleLocation + Vector3(0.0f, -traceDistance, 0.0f),
                                       collisionChannel, 
                                       capsuleShape,
                                       queryParams,
                                       responseParams);

        if (isBlockingHit)
        {
            if (hit.m_startPenetrating  || !IsWithinEdgeTolerance(p_capsuleLocation, hit.m_impactLocation, capsuleShape.Capsule.m_radius))
            {
                capsuleShape.Capsule.m_radius = Math::Max(0.0f, capsuleShape.Capsule.m_radius - MATH_EPSILON_F - SweepEdgeRejectDistance);

                if (!capsuleShape.IsNearlyZero())
                {
                    shrinkHeight                        = (playerHalfHeight - playerRadius) * (1 - shrinkScaleOverlap);
                    traceDistance                       = p_sweepDistance + shrinkHeight;
                    capsuleShape.Capsule.m_halfHeight   = Math::Max(playerHalfHeight - shrinkHeight, capsuleShape.Capsule.m_radius);
                    hit = HitResult();

                    isBlockingHit = FloorSweepTest(hit, 
                                                   p_capsuleLocation, 
                                                   p_capsuleLocation + Vector3(0.0f, -traceDistance, 0.0f),
                                                   collisionChannel, 
                                                   capsuleShape,
                                                   queryParams,
                                                   responseParams);
                }
            }

            float const maxPenetrationAdjust    {Math::Max(MaxFloorDistance, playerRadius)};
            float const sweepResult             {Math::Max(-maxPenetrationAdjust, hit.m_hitTime * traceDistance - shrinkHeight)};

            p_outFloorResult.SetFromSweep(hit, sweepResult, false);
            if (hit.IsValidBlockingHit() && IsWalkable(hit))
            {
                if (sweepResult <= p_sweepDistance)
                {
                    p_outFloorResult.m_isWalkableFloor = true;
                    return;
                }
            }
        }
    }

    if (!p_outFloorResult.m_isBlockingHit && !p_outFloorResult.m_hitResult.m_startPenetrating)
    {
        p_outFloorResult.m_floorDistance = p_sweepDistance;
        return;
    }

    if (p_lineDistance > 0.0f)
    {
        float const     shrinkHeight    {playerHalfHeight};
        Vector3 const   lineTraceStart  {p_capsuleLocation};
        float const     traceDistance   {p_lineDistance + shrinkHeight};
        Vector3 const   down            {0.0f, -traceDistance, 0.0f};
        HitResult       hit;

        isBlockingHit = GetWorld()->LineTraceSingleByChannel(hit, 
                                                             lineTraceStart, 
                                                             lineTraceStart + down, 
                                                             collisionChannel,
                                                             queryParams,
                                                             responseParams);
        if (isBlockingHit)
        {
            if (hit.m_hitTime > 0.0f)
            {
                float const maxPenetrationAdjust    {Math::Max(MaxFloorDistance, playerRadius)};
                float const lineResult              {Math::Max(-maxPenetrationAdjust, hit.m_hitTime * traceDistance - shrinkHeight)};

                p_outFloorResult.m_isBlockingHit = true;

                if (lineResult <= p_lineDistance && IsWalkable(hit))
                {
                    p_outFloorResult.SetFromLineTrace(hit, p_outFloorResult.m_floorDistance, lineResult, true);
                }
            }
        }
    }

    p_outFloorResult.m_isWalkableFloor  = false;
    p_outFloorResult.m_floorDistance    = p_sweepDistance;

}

void        PlayerMovementComponent::ComputeVelocity                (float                          p_deltaTime,
                                                                     float                          p_friction,
                                                                     float                          p_brakingDeceleration)
{
    if (!HasValidData() || MATH_EPSILON_F <= 0.0f)
    {
        return;
    }

    p_friction = Math::Max(0.0f, p_friction);

    float const maxAcceleration             {GetMaxAcceleration()};
    float       maxSpeed                    {GetMaxSpeed()};

    if (m_forceMaxAcceleration)
    {
        if (m_acceleration.GetMagnitudeSquared() > MATH_EPSILON_F)
        {
            m_acceleration = m_acceleration.GetSafeNormalized() * m_maxAcceleration;
        }

        else
        {
            m_acceleration = m_maxAcceleration * (m_velocity.GetMagnitudeSquared() < MATH_EPSILON_F ? m_updatedPrimitive->GetForward() : m_velocity.GetSafeNormalized());
        }
    }

    // Apply Braking or Deceleration //
    bool const  zeroAcceleration    {m_acceleration.IsZero()};
    bool const  velocityOverMax     {IsExceedingMaxSpeed(maxSpeed)};

    // Only Apply Braking if there in no acceleration, or we are over our max speed ans need to slow down to it..
    if (zeroAcceleration || velocityOverMax)
    {
        Vector3 const oldVelocity           {m_velocity};
        
        float const   actualBrakingFriction {m_useSeperateBrakingFriction ? m_seperateBrakingFriction : p_friction};
        ApplyVelocityBraking(p_deltaTime, actualBrakingFriction, p_brakingDeceleration);

        if (velocityOverMax && m_velocity.GetMagnitudeSquared() < Math::Square(maxSpeed) && (m_acceleration | oldVelocity) > 0.0f)
        {
            m_velocity = oldVelocity.GetSafeNormalized() * maxSpeed;
        }

    }

    else if (!zeroAcceleration)
    {
        Vector3 const   accelerationDirection   {m_acceleration.GetSafeNormalized()};
        float const     velocityMagnitude       {m_velocity.GetMagnitude()};

        m_velocity = m_velocity - (m_velocity - accelerationDirection * velocityMagnitude) * Math::Min(p_deltaTime * p_friction, 1.0f);

    }

    if (!zeroAcceleration)
    {
        float const newMaxSpeed {IsExceedingMaxSpeed(maxSpeed) ? m_velocity.GetMagnitude() : maxSpeed};
        m_velocity += m_acceleration * p_deltaTime;
        m_velocity = Vector3::ClampMaxMagnitude(m_velocity, newMaxSpeed);
    }
}

bool        PlayerMovementComponent::DoJump                         ()
{
    if (CanJump())
    {
        m_velocity.m_y = Math::Max(m_velocity.m_y, m_jumpZForce);
        SetMovementMode(EMovementMode::Falling);
        return true;
    }

    return false;
}

void        PlayerMovementComponent::FindFloor                      (Vector3 const&                 p_capsuleLocation,
                                                                     FindFloorResult&               p_outFloorResult,
                                                                     bool                           p_canUseCachedLocation,
                                                                     HitResult const*               p_downwardSweepResult /** = nullptr */) const
{
    if (!HasValidData() || !m_updatedPrimitive->IsQueryCollisionEnabled())
    {
        p_outFloorResult.Clear();
        return;
    }

    float const heightCheckAdjust   {IsMovingOnGround() ? MaxFloorDistance : -MaxFloorDistance};

    float floorSweepTraceDistance   {Math::Max(MaxFloorDistance + MATH_EPSILON_F, m_maxStepHeight + heightCheckAdjust)};
    float floorLineTraceDistance    {floorSweepTraceDistance};
    bool  needToValidateFloor       {true};

    if (floorLineTraceDistance > 0.0f || floorSweepTraceDistance > 0.0f)
    {
        PlayerMovementComponent* mutableThis {const_cast<PlayerMovementComponent*>(this)};

        if (m_alwayCheckFloor || !p_canUseCachedLocation || m_forceNextFloorCheck)
        {
            mutableThis->m_forceNextFloorCheck = false;
            ComputeFloorDistance(p_capsuleLocation, floorLineTraceDistance, floorSweepTraceDistance, p_outFloorResult, m_playerOwner->GetCapsuleCollider()->GetScaledCapsuleRadius(), p_downwardSweepResult);
        }

        else
        {
            PrimitiveComponent*     movementBase        {m_updatedPrimitive};
            Entity const*           baseEntity          {movementBase->GetOwner()};
            ECollisionChannel const collisionChannel    {m_updatedPrimitive->GetCollisionObjectType()};

            if (movementBase != nullptr)
            {
                mutableThis->m_forceNextFloorCheck = !movementBase->IsQueryCollisionEnabled() ||
                                                      movementBase->GetCollisionResponseToChannel(collisionChannel) != ECollisionResponse::Block ||
                                                      movementBase->GetMobility() == EComponentMobility::Movable;
            }

            bool const isEntityBasePendingKill  {baseEntity && baseEntity->IsPendingKill()};

            if (!m_forceNextFloorCheck && !isEntityBasePendingKill && movementBase)
            {
                p_outFloorResult = m_currentFloor;
                needToValidateFloor = false;
            }

            else
            {
                mutableThis->m_forceNextFloorCheck = false;
                ComputeFloorDistance(p_capsuleLocation, floorLineTraceDistance, floorSweepTraceDistance, p_outFloorResult, m_playerOwner->GetCapsuleCollider()->GetScaledCapsuleRadius(), p_downwardSweepResult);
            }
        }
    }
}

bool       PlayerMovementComponent::FloorSweepTest                  (HitResult&                      p_outHit,
                                                                     Vector3 const&                 p_start,
                                                                     Vector3 const&                 p_end,
                                                                     ECollisionChannel const        p_collisionChannel,
                                                                     CollisionShape const&          p_collisionShape,
                                                                     CollisionQueryParams const&    p_queryParams,
                                                                     CollisionResponseParams const& p_responseParams)   const
{
    bool isBlockingHit  {false};

    isBlockingHit = GetWorld()->SweepSingleByChannel(p_outHit,
                                                     p_start,
                                                     p_end,
                                                     Quaternion::Identity,
                                                     p_collisionChannel,
                                                     p_collisionShape,
                                                     p_queryParams,
                                                     p_responseParams);

    return isBlockingHit;
}


Vector3     PlayerMovementComponent::GetAirControl                  (float                          p_deltaTime,
                                                                     float                          p_tickAirControl,
                                                                     Vector3 const&                 p_fallAcceleration) const
{
    if (p_tickAirControl != 0.0f)
    {
        p_tickAirControl = BoostAirControl(p_deltaTime, p_tickAirControl, p_fallAcceleration);
    }

    return p_tickAirControl * p_fallAcceleration;
}

Vector3     PlayerMovementComponent::GetFallingLateralAcceleration  (float const                    p_deltaTime)  const
{
    Vector3 fallAcceleration    {m_acceleration.m_x, 0.0f, m_acceleration.m_z};

    if (fallAcceleration.GetMagnitudeSquared() > 0.0f)
    {
        fallAcceleration = GetAirControl(p_deltaTime, m_airControl, fallAcceleration);
        fallAcceleration = Vector3::ClampMaxMagnitude(fallAcceleration, GetMaxAcceleration());
    }

    return fallAcceleration;
}

float       PlayerMovementComponent::GetJumpMaxHoldTime             ()  const
{
    return m_jumpMaxHoldTime;
}

float       PlayerMovementComponent::GetMaxAcceleration             ()  const
{
    return m_maxAcceleration;
}

float       PlayerMovementComponent::GetMaxBrakingDeceleration      ()  const
{
    switch (m_movementMode)
    {
        case EMovementMode::Falling : return m_breakingDecelerationWalking;
        case EMovementMode::Walking : return m_breakingDecelerationWalking;
        default                     : return 0.0f;
    }
}

float       PlayerMovementComponent::GetMaxSpeed                    ()  const
{
    switch (m_movementMode)
    {
        case EMovementMode::Falling:    return m_maxWalkSpeed;
        case EMovementMode::Walking:    return m_maxWalkSpeed;

        default:                        return 0.0f;
    }
}

std::string PlayerMovementComponent::GetMovementName                ()  const
{
    if (m_updatedPrimitive->IsSimulatingPhysics())
    {
        return "Rigid Body";
    }

    switch (m_movementMode)
    {
        case EMovementMode::Falling :   return "Falling";   
        case EMovementMode::Walking :   return "Walking";
        case EMovementMode::None    :   return "None";
        default                     :   return "Unknown";
    }
}

float       PlayerMovementComponent::GetSimulationTimeStep          (float                          p_remainingTime,
                                                                     uint32 const                   p_iterations)   const
{
    static uint32 s_warningCount  {0};

    if (p_remainingTime > m_maxSimulationTimeStep)
    {
        if (p_iterations < m_maxSimulationIterations)
        {
            p_remainingTime = Math::Min(m_maxSimulationTimeStep, p_remainingTime * 0.5f);
        }

        else
        {
            if (s_warningCount++ < 100)
            {
                LOG(LogPlayerMovement, 
                    Warning, 
                    "GetSimulationTimeStep() - Max iterations %d hit while remaining time %.6f > MaxSimulationTimeStep (%.3f) for '%s', movement '%s'", 
                    m_maxSimulationIterations, 
                    p_remainingTime, 
                    m_maxSimulationTimeStep, 
                    m_playerOwner->GetName(),
                    GetMovementName());
            }
        }
    }

    return Math::Max(MATH_EPSILON_F, p_remainingTime);
}

bool        PlayerMovementComponent::HasValidData                   ()  const
{
    return m_updatedPrimitive && m_playerOwner && !m_playerOwner->IsPendingKill();
}

bool        PlayerMovementComponent::IsExceedingMaxSpeed            (float                          p_maxSpeed) const
{
    p_maxSpeed = Math::Max(0.0f, p_maxSpeed);

    float const maxSpeedSquared     {Math::Square(p_maxSpeed)};
    float const overVelocityPercent {1.01f};

    return (m_velocity.GetMagnitudeSquared() > maxSpeedSquared * overVelocityPercent);
}

bool        PlayerMovementComponent::IsFalling                      ()  const
{
    return m_updatedPrimitive && m_movementMode == EMovementMode::Falling;
}

void        PlayerMovementComponent::InitCollisionParams            (CollisionQueryParams&          p_outQueryParams,
                                                                     CollisionResponseParams&       p_outResponseParams)    const
{
    if (m_updatedPrimitive)
    {
        m_updatedPrimitive->InitSweepCollisionParams(p_outQueryParams, p_outResponseParams);
    }
}
    
bool        PlayerMovementComponent::IsMovingOnGround               ()  const
{
    return m_updatedPrimitive && m_movementMode == EMovementMode::Walking;
}

bool        PlayerMovementComponent::IsValidLandingSpot             (Vector3 const                  p_capsuleLocation,
                                                                     HitResult const&               p_hit)  const
{
    if (!p_hit.m_isBlockingHit)
    {
        return false;
    }

    if (!p_hit.m_startPenetrating)
    {
        if (!IsWalkable(p_hit))
        {
            return false;
        }

        float playerRadius      {m_playerOwner->GetCapsuleCollider()->GetScaledCapsuleRadius()};
        float playerhalfHeight  {m_playerOwner->GetCapsuleCollider()->GetScaledCapsuleHalfHeight()};

        float lowerHemisphereY  {p_hit.m_location.m_y - playerhalfHeight + playerRadius};

        if (p_hit.m_impactLocation.m_y >= lowerHemisphereY)
        {
            return false;
        }

        if (!IsWithinEdgeTolerance(p_hit.m_location, p_hit.m_impactLocation, playerRadius))
        {
            return false;
        }
    }

    else
    {
        if (p_hit.m_normal.m_y < MATH_EPSILON_F)
        {
            return false;
        }
    }

    FindFloorResult floorResult;
    FindFloor(p_capsuleLocation, floorResult, false, &p_hit);

    if (!floorResult.IsWalkableFloor())
    {
        return false;
    }

    return true;
}

bool        PlayerMovementComponent::IsWithinEdgeTolerance          (Vector3 const&                 p_capsuleLocation,
                                                                     Vector3 const&                 p_testImpactPoint,
                                                                     float const                    p_capsuleRadius)    const
{
    Vector3 vec {p_testImpactPoint - p_capsuleLocation};
    vec.m_y = 0.0f;

    float const distanceFromCenterSquared   {vec.GetMagnitudeSquared()};
    float const reducedRadiusSquared       {Math::Square(Math::Max(SweepEdgeRejectDistance + MATH_EPSILON_F, p_capsuleRadius - SweepEdgeRejectDistance))};

    return distanceFromCenterSquared  < reducedRadiusSquared;
}

void        PlayerMovementComponent::Jump                           ()
{
    m_jumpPressed           = true;
    m_jumpCurrentHoldTime   = 0.0f;
}

bool        PlayerMovementComponent::MoveUpdatedPrimitive           (Vector3 const&                 p_delta,
                                                                     Quaternion const&              p_rotation,
                                                                     bool const                     p_sweep,
                                                                     HitResult&                     p_hitResult)
{
    if (m_updatedPrimitive)
    {
        return m_updatedPrimitive->MoveComponent(p_delta, p_rotation, p_sweep, &p_hitResult);
    }

    return false;
}

Vector3     PlayerMovementComponent::NewFallVelocity                (Vector3 const&                 p_initialVelocity,
                                                                     Vector3 const&                 p_gravity,
                                                                     float const                    p_deltaTime)    const
{
    Vector3 result {p_initialVelocity};

    if (p_deltaTime > 0.0f)
    {
        result += p_gravity * p_deltaTime;
    }

    return result;
}

void        PlayerMovementComponent::ResetJumpState                 ()
{
    m_jumpPressed               = false;
    m_wasJumping                = false;
    m_jumpCurrentHoldTime       = 0.0f;
    m_jumpForceTimeRemaining    = 0.0f;

    if (!IsFalling())
    {
        m_jumpCurrentCount = 0u;
    }
}

void        PlayerMovementComponent::SetMovementMode                (EMovementMode                  p_newMovementMode)
{
    if (p_newMovementMode != m_movementMode)
    {
        EMovementMode const previousMovementMode    {m_movementMode};
        m_movementMode = p_newMovementMode;

        OnMovementModeChanged(previousMovementMode);
    }
}

bool        PlayerMovementComponent::ShouldSkipUpdate               (float const                    p_deltaTime)    const
{
    return  m_updatedPrimitive == nullptr ||
            m_updatedPrimitive->GetMobility() != EComponentMobility::Movable ||
            !HasValidData() ||
            m_updatedPrimitive->IsSimulatingPhysics() ||
            p_deltaTime == 0.0f;
}

void        PlayerMovementComponent::StopJumping                    ()
{
    m_jumpPressed = false;
    ResetJumpState();
}

// ==============================[Protected Local Methods]============================== //

float   PlayerMovementComponent::BoostAirControl                    (float              p_deltaTime,
                                                                     float              p_tickAirControl,
                                                                     Vector3 const&     p_fallAcceleration) const
{
    if (m_airControlBoostMultiplier > 0.0f &&
        Vector2(m_velocity.m_x, m_velocity.m_z).GetMagnitudeSquared() < Math::Square(m_airControlVelocityTreshold))
    {
        p_tickAirControl = Math::Min(1.0f, m_airControlBoostMultiplier * p_tickAirControl);
    }

    return p_tickAirControl;
}

void    PlayerMovementComponent::CheckJumpInput                     (float const        p_deltaTime)
{
    if (m_jumpPressed)
    {
        bool const isfirstJump  {m_jumpCurrentCount == 0};
        if (isfirstJump && IsFalling())
        {
            m_jumpCurrentCount++;
        }

        bool const didJump  {DoJump()};

        if (didJump)
        {
            if (!m_wasJumping)
            {
                m_jumpCurrentCount++;
                m_jumpForceTimeRemaining = GetJumpMaxHoldTime();
            }
        }

        m_wasJumping = didJump;
    }
}

Vector3 PlayerMovementComponent::ConstrainInputAcceleration         (Vector3 const&     p_inputAcceleration)    const
{
    if (p_inputAcceleration.m_y != 0.0f && (IsMovingOnGround() || IsFalling()))
    {
        return Vector3(p_inputAcceleration.m_x, 0.0f, p_inputAcceleration.m_z);
    }

    return p_inputAcceleration;
}

Vector3 PlayerMovementComponent::ComputeGroundMovementDelta         (Vector3 const&     p_delta,
                                                                     HitResult const&   p_rampHit,
                                                                     bool const         p_hitFronLineTrace) const
{
    Vector3 const floorNormal   {p_rampHit.m_impactNormal};
    Vector3 const contactNormal {p_rampHit.m_normal};

    if (floorNormal.m_y < (1.0f - MATH_EPSILON_F) && floorNormal.m_y > MATH_EPSILON_F && contactNormal.m_y > MATH_EPSILON_F && !p_hitFronLineTrace && IsWalkable(p_rampHit))
    {
        float const floorDotDelta   {floorNormal | p_delta};
        Vector3 rampMovement        {p_delta.m_x, -floorDotDelta / floorDotDelta , p_delta.m_z};

        if (m_maintainHorizontalGroundVelocity)
        {
            return rampMovement;
        }

        else
        {
            return rampMovement.GetSafeNormalized() * p_delta.GetMagnitude();
        }
    }

    return p_delta;
}

Vector3 PlayerMovementComponent::ComputeSlideVector                 (Vector3 const&     p_delta,
                                                                     float const        p_time,
                                                                     Vector3 const&     p_normal) const
{
    return Vector3::ProjectOnPlane(p_delta, p_normal) * p_time;
}

bool    PlayerMovementComponent::IsWalkable                         (HitResult const&   p_hit)  const
{
    if (!p_hit.IsValidBlockingHit() || p_hit.m_impactNormal.m_y < MATH_EPSILON_F || p_hit.m_impactNormal.m_y < m_walkableFloorY)
    {
        return false;
    }

    return true;
}

void    PlayerMovementComponent::MaintainHorizontalGroundVelocity   ()
{
    if (m_velocity.m_y)
    {
        if (m_maintainHorizontalGroundVelocity)
        {
            m_velocity.m_y = 0.f;
        }

        else
        {
            m_velocity = Vector3(m_velocity.m_x, 0.0f, m_velocity.m_z).GetSafeNormalized() * m_velocity.GetMagnitude();
        }
    }
}

void    PlayerMovementComponent::MoveAlongFloor                     (Vector3 const&     p_velocity,
                                                                     float const        p_deltaTime,
                                                                     StepDownResult*    p_outStepDownResult)
{
    if (!m_currentFloor.IsWalkableFloor())
    {
        return;
    }

    Vector3 const delta {Vector3(p_velocity.m_x, 0.0f, p_velocity.m_z) * p_deltaTime};
    HitResult hit;
    Vector3 rampVector  {ComputeGroundMovementDelta(delta, m_currentFloor.m_hitResult, m_currentFloor.m_lineTrace)};
    MoveUpdatedPrimitive(delta, m_updatedPrimitive->GetWorldRotation(), true, hit);
    float lastMoveTimeSlice {p_deltaTime};

    LOG(LogPlayerMovement, Display, "(%f, %f, %f)", rampVector.m_x, rampVector.m_y, rampVector.m_z);


    if (hit.m_startPenetrating)
    {
        LOG(LogPlayerMovement, Warning, "MoveAlongFloor() : Start Penetrating not implemented !");
        SlideAlongSurface(delta, 1.0f, hit.m_normal, hit);
    }
    
    else if (hit.IsValidBlockingHit())
    {
        float percentTimeApplied    {hit.m_hitTime};

        if (hit.m_hitTime > 0.0f && hit.m_normal.m_y > MATH_EPSILON_F /* &&  IsWalkable(hit)*/)
        {
            float const intitialPercentRemaining    {1.0f- percentTimeApplied};
            rampVector = ComputeGroundMovementDelta(delta * intitialPercentRemaining, hit, false);
            lastMoveTimeSlice = intitialPercentRemaining * lastMoveTimeSlice;
            MoveUpdatedPrimitive(rampVector, m_updatedPrimitive->GetWorldRotation(), true, hit);

            float const secondHitPercent    {hit.m_hitTime * intitialPercentRemaining};
            percentTimeApplied = Math::Clamp01(percentTimeApplied + secondHitPercent);
        }

        if (hit.IsValidBlockingHit())
        {
            // Add possibility step up a barrier if I have the Time
        }
    }
}

void    PlayerMovementComponent::OnMovementModeChanged              (EMovementMode      p_previousMovementMode)
{
    LOG(LogPlayerMovement, Warning, "PlayerMovementComponent::OnMovementModeChanged is not finished yet.");


    if (HasValidData())
    {
        if (m_movementMode == EMovementMode::Walking)
        {
            m_velocity.m_y = 0.0f;
            FindFloor(m_updatedPrimitive->GetWorldLocation(), m_currentFloor, false);
            // AdujstFloorHeight();
            // SetBaseFromFloor();
        }

        else
        {
            m_currentFloor.Clear();

            if (m_movementMode == EMovementMode::None)
            {
                ResetJumpState();
                ClearPendingForces();
            }
        }

        if (!m_jumpPressed || !IsFalling())
        {
            ResetJumpState();
        }
    }
}

void    PlayerMovementComponent::PerformMovement                    (float const        p_deltaTime)
{
    if (!HasValidData())
    {
        return;
    }

    if (m_movementMode == EMovementMode::None || m_updatedPrimitive->GetMobility() != EComponentMobility::Movable || m_updatedPrimitive->IsSimulatingPhysics())
    {
        ClearPendingForces();
    }

    m_forceNextFloorCheck |= (IsMovingOnGround() && m_updatedPrimitive->GetWorldLocation() != m_lastUpdatedLocation);

    // Add Pending force if have time to implement it.

    CheckJumpInput(p_deltaTime);
    UpdateJumpState(p_deltaTime);

    SimulateMovement(p_deltaTime, 0u);
    

    Vector3 const newLocation   {m_updatedPrimitive ? m_updatedPrimitive->GetWorldLocation() : Vector3::Zero};

    m_lastUpdatedLocation = newLocation;
    m_lastUpdatedVelocity = m_velocity;
}

void    PlayerMovementComponent::ProcessLanded                      (HitResult const&   p_hit,
                                                                     float const        p_remainingTime,
                                                                     int32              p_iterations)
{
    if (IsFalling())
    {
        SetMovementMode(EMovementMode::Walking);
        // Apply physics force if we have time to implement this one.
    }

    SimulateMovement(p_remainingTime, p_iterations);
}

Vector3 PlayerMovementComponent::ScaleInputAcceleration             (Vector3 const&     p_inputAcceleration)    const
{
    return GetMaxAcceleration() * Vector3::ClampMaxMagnitude(p_inputAcceleration, 1.0f);
}

void    PlayerMovementComponent::SimulateFalling                    (float const        p_deltaTime,
                                                                     uint32             p_iteration)
{
    if (p_deltaTime <= MATH_EPSILON_F)
    {
        return;
    }

    Vector3             fallingLateralAcceleration  {GetFallingLateralAcceleration(p_deltaTime)};
    fallingLateralAcceleration.m_y = 0.0f;
    bool const          hasAirControl               {fallingLateralAcceleration.GetMagnitudeSquared() > 0.0f};
   


    float               remainingTime               {p_deltaTime};
    while (remainingTime >= MATH_EPSILON_F && p_iteration < m_maxSimulationIterations)
    {
        p_iteration++;
        float const timeTick    {GetSimulationTimeStep(remainingTime, p_iteration)};
        remainingTime -= timeTick;

        Vector3 const       oldLocation             {m_updatedPrimitive->GetWorldLocation()};
        Quaternion const    playerRotation          {m_updatedPrimitive->GetWorldRotation()};

        Vector3             oldVelocity             {m_velocity};
        Vector3             velocityNoAirControl    {m_velocity};

        float const maxDeceleration {GetMaxBrakingDeceleration()};

        if (hasAirControl)
        {
            GuardValue<Vector3> restoreAcceleration (m_acceleration, Vector3::Zero);
            GuardValue<Vector3> restoreVelocity     (m_velocity, m_velocity);
            m_velocity.m_y = 0.0f;

            ComputeVelocity(timeTick, m_fallingLateralFriction, maxDeceleration);
            velocityNoAirControl.Set(m_velocity.m_x, oldVelocity.m_y, m_velocity.m_z); 
        }

        {
            GuardValue<Vector3> restoreAcceleration {m_acceleration, fallingLateralAcceleration};
            m_velocity.m_y = 0.0f;
            ComputeVelocity(timeTick, m_fallingLateralFriction, maxDeceleration);
            m_velocity.m_y = oldVelocity.m_y;
        }

        if  (!hasAirControl)
        {
            velocityNoAirControl = m_velocity;
        }


        Vector3 gravity     {0.0f,-9.81f, 0.0f};
        float   gravityTime {timeTick};
        
        if (m_jumpForceTimeRemaining > 0.0f)
        {
            float const jumpForceTime   {Math::Min(m_jumpForceTimeRemaining, timeTick)};
            gravityTime = m_applyGravityWhileJumping ? timeTick : Math::Max(0.0f, timeTick - jumpForceTime);

            m_jumpForceTimeRemaining -= jumpForceTime;
            if (m_jumpForceTimeRemaining <= 0.0f)
            {
                ResetJumpState();
            }
        }

        m_velocity = NewFallVelocity(m_velocity, gravity, gravityTime);
        velocityNoAirControl = hasAirControl ? NewFallVelocity(velocityNoAirControl, gravity, gravityTime) : m_velocity;
        Vector3 const airControlAcceleration {(m_velocity - velocityNoAirControl) / timeTick};

        HitResult hit;
        Vector3 adjusted    {0.5f * (oldVelocity + m_velocity) * timeTick};
        MoveUpdatedPrimitive(adjusted, playerRotation, true, hit);

        if (!HasValidData())
        {
            return;
        }

        float subTimetickRemaining  {timeTick * (1.0f - hit.m_hitTime)};

        // Check move Collision //
        if (hit.m_isBlockingHit)
        {
            if (IsValidLandingSpot(m_updatedPrimitive->GetWorldLocation(), hit))
            {
                remainingTime += subTimetickRemaining;
                ProcessLanded(hit, remainingTime, p_iteration);
            }
   
            else
            {
                LOG(LogPlayerMovement, Display, "Other Procces");
            }

        }


        if (Vector3(m_velocity.m_x, 0.0f, m_velocity.m_z).GetMagnitudeSquared() < MATH_EPSILON_F)
        {
            m_velocity.m_x = 0.0f;
            m_velocity.m_z = 0.0f;
        }
    }
}

void    PlayerMovementComponent::SimulateMovement                   (float const        p_deltaTime,
                                                                     uint32             p_iterations)
{
    if (p_deltaTime < MATH_EPSILON_F || p_iterations >= m_maxSimulationIterations || !HasValidData())
    {
        return;
    }

    if (m_updatedPrimitive->IsSimulatingPhysics())
    {
        LOG(LogPlayerMovement, Warning, "%s is silmulating physics !", m_updatedPrimitive->GetName());
        return;
    }

    switch (m_movementMode)
    {
        case EMovementMode::Walking:    SimulateWalking(p_deltaTime, p_iterations); break;
        case EMovementMode::Falling:    SimulateFalling(p_deltaTime, p_iterations); break;
        case EMovementMode::None:                                                   break;

        default:
        {
            LOG(LogPlayerMovement, Warning, "%s has unsupported movement mode !", m_playerOwner->GetName());
            m_movementMode = EMovementMode::None;
        }
    }
}

void    PlayerMovementComponent::SimulateWalking                    (float const        p_deltaTime,
                                                                     uint32             p_iterations)
{
    if (p_deltaTime < MATH_EPSILON_F)
    {
        return;
    }

    if (!m_updatedPrimitive->IsQueryCollisionEnabled())
    {
        SetMovementMode(EMovementMode::Walking);
        return;
    }

    float remainingTime {p_deltaTime};

    while (remainingTime >= MATH_EPSILON_F && p_iterations < m_maxSimulationIterations)
    {
        p_iterations++;
        float const timeTick    {GetSimulationTimeStep(remainingTime, p_iterations)};
        remainingTime -= timeTick;

        Vector3 const oldLocation = m_updatedPrimitive->GetWorldLocation();
        FindFloorResult oldFloor {m_currentFloor};

        MaintainHorizontalGroundVelocity();
        Vector3 const oldVelocity   {m_velocity};
        m_velocity.m_y = 0.0f;

        ComputeVelocity(timeTick, m_groundFriction, GetMaxBrakingDeceleration());

        Vector3 const   moveVelocity    {m_velocity};
        Vector3 const   delta           {timeTick * moveVelocity};
        bool const      zeroDelta       {delta.IsZero()};
        StepDownResult  stepDownResult;

        if (zeroDelta)
        {
            remainingTime = 0.0f;
        }

        else
        {
            MoveAlongFloor(moveVelocity, timeTick, &stepDownResult);

            if (IsFalling())
            {
                float const desiredDist = delta.GetMagnitude();

                if (desiredDist > MATH_EPSILON_F)
                {
                    Vector3 const   vec             {m_updatedPrimitive->GetWorldLocation() - oldLocation};
                    float const     actualDistance  {Vector3(vec.m_x, 0.0f, vec.m_z).GetMagnitude()};
                    remainingTime += timeTick * (1.0f - Math::Min(1.0f, actualDistance / desiredDist));
                }

                SimulateMovement(remainingTime, p_iterations);
                return;
            }
        }

        if (stepDownResult.m_computedFloor)
        {
            m_currentFloor = stepDownResult.m_floorResult;
        }

        else
        {
            FindFloor(m_updatedPrimitive->GetWorldLocation(), m_currentFloor, zeroDelta, nullptr);
        }

        if (IsMovingOnGround() && timeTick >= MATH_EPSILON_F)
        {
            m_velocity = (m_updatedPrimitive->GetWorldLocation() - oldLocation) / timeTick;
        }

        if (m_updatedPrimitive->GetWorldLocation() == oldLocation)
        {
            remainingTime = 0.0f;
            break;
        }
    }

    if (IsMovingOnGround())
    {
        MaintainHorizontalGroundVelocity();
    }
}

float   PlayerMovementComponent::SlideAlongSurface                  (Vector3 const&     p_delta,
                                                                     float              p_deltaTime,
                                                                     Vector3 const&     p_normal,
                                                                     HitResult&         p_hit)
{
    if (!p_hit.m_isBlockingHit)
    {
        return 0.0f;
    }

    Vector3 normal  {p_normal};
    if (IsMovingOnGround())
    {
        if (normal.m_y > 0.0f)
        {
            if (!IsWalkable(p_hit))
            {
                normal = Vector3(normal.m_x, 0.0f, normal.m_z).GetSafeNormalized();
            }
        }

        else if (normal.m_z < - MATH_EPSILON_F)
        {
            if (m_currentFloor.m_floorDistance < MinFloorDistance && m_currentFloor.m_isBlockingHit)
            {
                Vector3 const floorNormal = m_currentFloor.m_hitResult.m_normal;
                bool const floorOpposedToMovement   {(p_delta | floorNormal) < 0.0f && floorNormal.m_z < 1.0f - 0.00001f};

                if (floorOpposedToMovement)
                {
                    normal = floorNormal;
                }

                normal = Vector3(normal.m_x, 0.0f, normal.m_z).GetSafeNormalized();
            }
        }
    }

    return SlideAlongSurfaceGl(p_delta, p_deltaTime, normal, p_hit);
}

float   PlayerMovementComponent::SlideAlongSurfaceGl                (Vector3 const&     p_delta,
                                                                     float              p_deltaTime,
                                                                     Vector3 const&     p_normal,
                                                                     HitResult&         p_hit)
{
    if (!p_hit.m_isBlockingHit)
    {
        return 0.0f;
    }

    float percentTimeApplied    {0.0f};
    Vector3 const oldHitNormal  {p_normal};

    Vector3 slideDelta          {ComputeSlideVector(p_delta, p_deltaTime, p_normal)};

    if ((slideDelta | p_delta) > 0.f)
    {
        Quaternion const rotation   {m_updatedPrimitive->GetWorldRotation()};
        MoveUpdatedPrimitive(slideDelta, rotation, true, p_hit);

        float const firstHitPercent {p_hit.m_hitTime};
        percentTimeApplied = firstHitPercent;

        if (p_hit.IsValidBlockingHit())
        {
            TwoWallAdjust(slideDelta, p_hit, oldHitNormal);

            if (!slideDelta.IsNearlyZero(1e-3f) && (slideDelta | p_delta) > 0.0f)
            {
                MoveUpdatedPrimitive(slideDelta, rotation, true, p_hit);
                float const secondHitPercent {p_hit.m_hitTime * (1.0f - firstHitPercent)};

                percentTimeApplied += secondHitPercent;

            }
        }

        return Math::Clamp01(percentTimeApplied);
    }

    return 0.0f;
}

void    PlayerMovementComponent::TwoWallAdjust                      (Vector3&           p_outDelta,
                                                                     HitResult const&   p_hit,
                                                                     Vector3 const&     p_oldHitNormal) const
{
    Vector3         delta       {p_outDelta};
    Vector3 const&  hitNormal   {p_hit.m_normal};

    if ((p_oldHitNormal | hitNormal) <= 0.0f)
    {
        Vector3 const desiredDir    {delta};
        Vector3 newDir              {hitNormal ^ p_oldHitNormal};

        newDir = newDir.GetSafeNormalized();
        delta = (delta | newDir) * (1.0f - p_hit.m_hitTime) * newDir;
        if ((desiredDir | delta) < 0.0f)
        {
            delta = -1.0f * delta;
        }
    }

    else
    {
        Vector3 desiredDir  {delta};
        delta = ComputeSlideVector(delta, 1.0f - p_hit.m_hitTime, hitNormal);

        if ((delta | desiredDir) <= 0.0f)
        {
            delta += hitNormal * 0.01f;
        }
    }

    p_outDelta = delta;
}

void    PlayerMovementComponent::UpdateJumpState                    (float const    p_deltaTine)
{
    if (m_jumpPressed)
    {
        m_jumpCurrentHoldTime += p_deltaTine;
        if (m_jumpCurrentHoldTime >= GetJumpMaxHoldTime())
        {
            m_jumpPressed = false;
        }
    }

    else
    {
        m_jumpForceTimeRemaining = 0.0f;
        m_wasJumping = false;
    }
}

// ==============================[Private Local Methods]============================== //

bool    PlayerMovementComponent::SetupPlayerOwner       ()
{
    m_playerOwner = dynamic_cast<Player*>(GetOwner());
    if (m_playerOwner == nullptr)
    {
        LOG(LogPlayerMovement, Error, "Trying to setup a nullptr player owner.");
        return false;
    }

    return true;
}

bool    PlayerMovementComponent::SetupUpdatedPrimitive  ()
{
    m_updatedPrimitive = dynamic_cast<PrimitiveComponent*>(m_playerOwner->GetRootComponent());
    if (m_updatedPrimitive == nullptr)
    {
        LOG(LogPlayerMovement, Error, "Trying to setup a nullptr updated primitive component.");
        return false;
    }

    return true;
}

void    PlayerMovementComponent::UpdateWalkableFloorY   ()
{
    m_walkableFloorAngle = Math::Clamp(m_walkableFloorAngle,0.0f, 90.0f);
    m_walkableFloorY = Math::Cos(Math::DegToRad(m_walkableFloorAngle));
}