#ifndef __PLAYER_MOVEMENT_COMPONENT_HPP__
#define __PLAYER_MOVEMENT_COMPONENT_HPP__

#include "Components/EntityComponent.hpp"
#include "StepDownResult.hpp"

#include "Player.hpp"

enum class EMovementMode
{
    Walking,
    Falling,
    None
};

DECLARE_LOG_CATEGORY(LogPlayerMovement);

// ==============================[Forward Declaration]============================== //

class CollisionQueryParams;
class CollisionResponseParams;

// ================================================================================= //


class ENGINE_API PlayerMovementComponent : public EntityComponent
{
    REFLECT(PlayerMovementComponent)

    typedef EntityComponent Super;

    public: 

    // ==============================[Public Static Properties]============================== //

        static CONSTEXPR float  MaxFloorDistance        = 0.024f;

        static CONSTEXPR float  MinFloorDistance        = 0.019f;

        static CONSTEXPR float  SweepEdgeRejectDistance = 0.0015f;

    // ==============================[Public Local Properties]============================== //

        PROPERTY()
        float               m_gravityScale                      = 1.0f;

        PROPERTY()
        float               m_jumpZForce                        = 6.0f; 

        PROPERTY()
        float               m_maxAcceleration                   = 2.0f; 

        PROPERTY()
        float               m_mass                              = 1.0f;

        PROPERTY()
        float               m_maxStepHeight                     = 0.45f;

        PROPERTY()
        float               m_maxWalkSpeed                      = 6.0f;

        EMovementMode       m_movementMode                      = EMovementMode::Falling;

        PROPERTY()
        float               m_airControl                        = 0.05f;

        PROPERTY()
        float               m_airControlBoostMultiplier         = 2.0f;

        PROPERTY()
        float               m_airControlVelocityTreshold        = 0.25f;

        PROPERTY()
        float               m_brakingSubStepMaxTime             = 1.0f / 33.0f ;

        PROPERTY()
        float               m_brakingFrictionFactor             = 2.0f;

        PROPERTY()
        float               m_groundFriction                    = 8.0f;

        PROPERTY()
        float               m_breakingDecelerationWalking       = m_maxAcceleration;

        PROPERTY()
        float               m_breakingDecelerationFalling       = 0.0f;

        PROPERTY()
        bool                m_applyGravityWhileJumping          = true;

        PROPERTY()
        float               m_fallingLateralFriction            = 0.0f;

        PROPERTY()
        bool                m_forceMaxAcceleration              = false;

        PROPERTY()
        bool                m_useSeperateBrakingFriction        = false;
        
        PROPERTY()
        float               m_seperateBrakingFriction           = 0.0f;

        PROPERTY()
        bool                m_maintainHorizontalGroundVelocity  = true;

        PROPERTY()
        bool                m_alwayCheckFloor                   = true;

        PROPERTY()
        float               m_maxSimulationTimeStep             = 0.05f;

        PROPERTY()
        int32              m_maxSimulationIterations            = 8;

    // ==============================[Public Constructor + Destructor]============================== //

        PlayerMovementComponent     () = default;

        PlayerMovementComponent     (PlayerMovementComponent const& p_copy) = default;

        PlayerMovementComponent     (PlayerMovementComponent&&      p_move) = default;

        ~PlayerMovementComponent    () = default;

    // ==============================[Virtual Public Local Methods]============================== //

        virtual void BeginPlay      ()  override;

		virtual void Tick           (float const    p_deltaTime)    override;

        virtual void Serialize      (Json&          p_serialize)    const override;
        
        virtual void Deserialize    (Json const&    p_deserialize,
                                     Json const&    p_components)   override;

    // ==============================[Public Local Methods]============================== //

        void                    ApplyVelocityBraking            (float                          p_deltaTime,
                                                                 float                          p_friction,
                                                                 float                          p_brakingDeceleration);

        bool                    CanJump                         ()  const;

        void                    ClearPendingForces              ();

        Vector3                 ConsumePlayerMovementInput      ();

        void                    ComputeFloorDistance            (Vector3 const&                 p_capsuleLocation,
                                                                 float const                    p_lineDistance,
                                                                 float const                    p_sweepDistance,
                                                                 FindFloorResult&               p_outFloorResult,
                                                                 float const                    p_sweepRadius,
                                                                 HitResult const*               p_downwardSweepResult = nullptr)    const;

        void                    ComputeVelocity                 (float                          p_deltaTime,
                                                                 float                          p_friction,
                                                                 float                          p_brakingDeceleration);

        bool                    DoJump                          ();                

        //Player*                 GetPlayer                     ()  const;

        //INLINE EMovementMode    GetMovementMode               ()  const;

        void                    FindFloor                       (Vector3 const&                 p_capsuleLocation,
                                                                 FindFloorResult&               p_outFloorResult,
                                                                 bool                           p_canuseCachedLocation,
                                                                 HitResult const*               p_downwardSweepResult = nullptr) const;

        bool                    FloorSweepTest                  (HitResult&                     p_outHit,
                                                                 Vector3 const&                 p_start,
                                                                 Vector3 const&                 p_end,
                                                                 ECollisionChannel const        p_collisionChannel,
                                                                 CollisionShape const&          p_collisionShape,
                                                                 CollisionQueryParams const&    p_queryParams,
                                                                 CollisionResponseParams const& p_responseParams)   const;


        float                   GetJumpMaxHoldTime              ()  const;

        Vector3                 GetAirControl                   (float                          p_deltaTime,
                                                                 float                          p_tickAirControl,
                                                                 Vector3 const&                 p_fallAcceleration) const;

        Vector3                 GetFallingLateralAcceleration   (float const                    p_deltaTime)  const;

        float                   GetMaxAcceleration              ()  const;

        float                   GetMaxBrakingDeceleration       ()  const;

        float                   GetMaxSpeed                     ()  const;

        std::string             GetMovementName                 ()  const;

        float                   GetSimulationTimeStep           (float                          p_remainingTime,
                                                                 uint32 const                   p_iterations)   const;

        bool                    HasValidData                    ()  const;

        void                    InitCollisionParams             (CollisionQueryParams&          p_outQueryParams,
                                                                 CollisionResponseParams&       p_outResponseParams)    const;

        bool                    IsExceedingMaxSpeed             (float                          p_maxSpeed) const;

        bool                    IsFalling                       ()  const;

        bool                    IsMovingOnGround                ()  const;

        bool                    IsValidLandingSpot              (Vector3 const                  p_capsuleLocation,
                                                                 HitResult const&               p_hit)  const;

        bool                    IsWithinEdgeTolerance           (Vector3 const&                 p_capsuleLocation,
                                                                 Vector3 const&                 p_testImpactPoint,
                                                                 float const                    p_capsuleRadius)    const;

        void                    Jump                            ();

        Vector3                 NewFallVelocity                 (Vector3 const&                 p_initialVelocity,
                                                                 Vector3 const&                 p_gravity,
                                                                 float const                    p_deltaTime)    const;
        
        bool                    MoveUpdatedPrimitive            (Vector3 const&                 p_delta,
                                                                 Quaternion const&              p_rotation,
                                                                 bool const                     p_sweep,
                                                                 HitResult&                     p_hitResult);
        
        void                    ResetJumpState                  ();

        void                    SetMovementMode                 (EMovementMode                  p_newMovementMode);

        bool                    ShouldSkipUpdate                (float const                    p_deltaTime)    const;

        void                    StopJumping                     ();

    // ==============================[Public Local Methods]============================== //

        PlayerMovementComponent&    operator=   (PlayerMovementComponent const& p_copy) = default;
        PlayerMovementComponent&    operator=   (PlayerMovementComponent&&      p_move) = default;

    protected:

    // ==============================[Protected Local Properties]============================== //

        Vector3 m_acceleration              = Vector3::Zero;

        Vector3 m_velocity                  = Vector3::Zero;

        Vector3 m_pendingForceToApply       = Vector3::Zero;

        Vector3 m_pendingImpulseToApply     = Vector3::Zero;

        Vector3 m_movementInput             = Vector3::Zero;

        Vector3 m_lastMovementInput         = Vector3::Zero;

        bool    m_jumpPressed               = false;

        bool    m_wasJumping                = false;

        float   m_jumpForceTimeRemaining    = 0.0f;

        float   m_jumpCurrentHoldTime       = 0.0f;
        
        PROPERTY()
        float   m_jumpMaxHoldTime           = 0.0f;

        uint32  m_jumpCurrentCount          = 0u;

        PROPERTY()
        int32  m_jumpMaxCount              = 1u;

        bool    m_forceNextFloorCheck       = true;

        Vector3 m_lastUpdatedLocation       = Vector3::Zero;

        Vector3 m_lastUpdatedVelocity       = Vector3::Zero;

    // ==============================[Protected Local Methods]============================== //

        float   BoostAirControl                     (float              p_deltaTime,
                                                     float              p_tickAirControl,
                                                     Vector3 const&     p_fallAcceleration) const;
              
        void    CheckJumpInput                      (float const        p_deltaTime);

        Vector3 ConstrainInputAcceleration          (Vector3 const&     p_iputAcceleration) const;

        Vector3 ComputeGroundMovementDelta          (Vector3 const&     p_delta,
                                                     HitResult const&   p_rampHit,
                                                     bool const         p_hitFronLineTrace) const;

        Vector3 ComputeSlideVector                  (Vector3 const&     p_delta,
                                                     float const        p_time,
                                                     Vector3 const&     p_normal) const;

        bool    IsWalkable                          (HitResult const&   p_hit)  const;

        void    MaintainHorizontalGroundVelocity    ();

        void    MoveAlongFloor                      (Vector3 const&     p_velocity,
                                                     float const        p_deltatime,
                                                     StepDownResult*    p_outStepDownResult = nullptr);

        void    OnMovementModeChanged               (EMovementMode      p_previousMovementMode);

        void    PerformMovement                     (float const        p_deltaTime);

        void    ProcessLanded                       (HitResult const&   p_hit,
                                                     float const        remainingTime,
                                                     int32              p_iterations);

        Vector3 ScaleInputAcceleration              (Vector3 const&     p_inputAcceleration)    const;

        void    SimulateFalling                     (float              p_deltaTime,
                                                     uint32             p_iterations);

        void    SimulateMovement                    (float              p_deltaTime,
                                                     uint32             p_iterations);

        void    SimulateWalking                     (float              p_deltaTime,
                                                     uint32             p_iterations);

        float   SlideAlongSurface                   (Vector3 const&     p_delta,
                                                     float              p_deltaTime,
                                                     Vector3 const&     p_normal,
                                                     HitResult&         p_hit);

        float   SlideAlongSurfaceGl                 (Vector3 const&     p_delta,
                                                     float              p_deltaTime,
                                                     Vector3 const&     p_normal,
                                                     HitResult&         p_hit);

        void    TwoWallAdjust                       (Vector3&           p_outDelta,
                                                     HitResult const&   p_hit,
                                                     Vector3 const&     p_oldHitNormal) const;

        void    UpdateJumpState                     (float const        p_deltaTimes);

    private:
    
    // ==============================[Private Local Properties]============================== //
        
        PROPERTY()
        float               m_walkableFloorAngle    = 45.0f;

        float               m_walkableFloorY        = 0.0f;

        Player*             m_playerOwner           = nullptr;

        PrimitiveComponent* m_updatedPrimitive      = nullptr;

        FindFloorResult     m_currentFloor;

    // ==============================[Private Local Methods]============================== //

        bool    SetupPlayerOwner        ();

        bool    SetupUpdatedPrimitive   ();

        void    UpdateWalkableFloorY    ();

}; // !class PlayerMovementComponent

#include "PlayerMovementComponent.generated.hpp"

#endif // !__PLAYER_MOVEMENT_COMPONENT_HPP__
