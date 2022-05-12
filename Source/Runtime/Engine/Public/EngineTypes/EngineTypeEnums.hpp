#ifndef __ENGINE_TYPE_ENUMS_HPP__
#define __ENGINE_TYPE_ENUMS_HPP__

#include "CoreMinimal.hpp"

enum class ECollisionChannel : uint8
{
        WorldStatic,  
        WorldDynamic, 
        
        GameTraceChannel1,
        GameTraceChannel2,
        GameTraceChannel3,
        GameTraceChannel4,
        GameTraceChannel5,
        GameTraceChannel6,
        GameTraceChannel7,
        GameTraceChannel8,
        GameTraceChannel9,
        GameTraceChannel10,
        GameTraceChannel11,
        GameTraceChannel12,
        GameTraceChannel13,
        GameTraceChannel14,
        GameTraceChannel15,
        GameTraceChannel16,
        GameTraceChannel17,
        GameTraceChannel18,
        GameTraceChannel19,
        GameTraceChannel20,
        GameTraceChannel21,
        GameTraceChannel22,
        GameTraceChannel23,
        GameTraceChannel24,
        GameTraceChannel25,
        GameTraceChannel26,
        GameTraceChannel27,
        GameTraceChannel28,
        GameTraceChannel29,
        GameTraceChannel30

}; // !enum class ECollisionChannel;

/**
 * Enum indicatinng how each type should respond.
 */
enum class ECollisionResponse : uint8
{
    Ignore,
    Overlap,
    Block

}; // !enum class ECollisionResponse

/**
 * Enum used to describe what type of collision is enabled on a body.
 */
enum class ECollisionEnabled : uint8
{
    /** ... */
    NoCollision,

    /** Only used for spatial queries (Raycast, sweeps and overlaps). Cannot be used for simulation (Rigid Body) */
    QueryOnly,

    /** Only used for physics simulation (Rigid Body). Cannot be used for spatial queries (Raycast, sweeps and overlaps) */
    PhysicsOnly,

    /** Can be used for both spatial queries (Raycast, sweeps and overlaps) and physics simulation (Rigid Body) */
    QueryAndPhysics

}; // class enum ECollisionType

/**
 * Describes how often this component is allow to move.
 */
enum class EComponentMobility : uint8
{
    Static,
    Movable

}; // !enum class EComponentMobility

enum class EGeometryType : uint8
{
    Box,
    Capsule,
    Sphere,
    Invalid
};

/** 
 * Enum for controlling the falloff of strength of a radial impulse as a function of distance from Origin. 
 */
enum class ERadialImpulseFalloff : uint8
{
    /** Impulse is constant strength, up to the limit of its range */
    Constant,

    /** Impulse should get linearly weaker the further from origin */
    Linear

}; // !enum class ERadialImpulseFalloff

/** Whether to teleport physics body or not */
enum class ETeleportType : uint8
{
    /** Do not teleport physics body. This means velocity will reflect the movement between initial and final position, and collisions along the way will occur */
    None,

    /** Teleport physics body so that velocity remains the same and no collision occurs */
    TeleportPhysics,

    /** Teleport physics body and reset physics state completely */
    ResetPhysics,
};

/** Information about how to update transform when something is moved */
enum class EUpdateTransformFlags
{
    /** Default options */
    None = 0x0,

    /** Don't update the underlying physics */
    SkipPhysicsUpdate = 0x1,
    
    /** The update is coming as a result of the parent updating (i.e. not called directly) */
    PropagateFromParent = 0x2,
};

CONSTEXPR inline EUpdateTransformFlags  operator|   (EUpdateTransformFlags const    p_left, 
                                                     EUpdateTransformFlags const    p_right)
{
	return static_cast<EUpdateTransformFlags>(static_cast<int32>(p_left) | static_cast<int32> (p_right));
}

CONSTEXPR inline EUpdateTransformFlags  operator&   (EUpdateTransformFlags const    p_left, 
                                                     EUpdateTransformFlags const    p_right)
{
	return static_cast<EUpdateTransformFlags>(static_cast<int32>(p_left) & static_cast<int32> (p_right));
}

CONSTEXPR inline bool                   operator!   (EUpdateTransformFlags const    p_value)
{
	return p_value == EUpdateTransformFlags::None;
}

CONSTEXPR inline EUpdateTransformFlags  operator~   (EUpdateTransformFlags const    p_value)
{
	return static_cast<EUpdateTransformFlags>(~static_cast<int32>(p_value));
}


// ==============================[Globale Methods]============================== //

FORCEINLINE bool CollisionEnabledHasPhysics (ECollisionEnabled const    p_collisionEnabled);

FORCEINLINE bool CollisionEnabledHasQuery   (ECollisionEnabled const    p_collisionEnabled);

#include "EngineTypeEnums.inl"

#endif // !__ENGINE_TYPE_ENUMS_HPP__
