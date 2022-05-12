#ifndef __PHYSICS_ENUMS_HPP__
#define __PHYSICS_ENUMS_HPP__

#include "CoreMinimal.hpp"

#define TRACE_SINGLE 0
#define TRACE_MULTI 1

/**
 * Enumerations that determines the way in wich two material properties will be be combine yield a friction or restitution for a collision/
 */
enum class EPhysicsCombineMode : uint8
{
    /** Uses the average value of the materials touching: (a+b)/2 */
    Average = 0,

    /** Uses the minimum value of the materials touching: Min(a,b) */
    Min = 1,

    /** Uses the product of the values of the materials touching: a*b */
    Multiply = 2,

    /** Uses the maximum value of materials touching: Max(a,b) */
    Max = 3,

}; // !enum class EPhysicCombineMode

enum class ECollisionQueryType
{
    ObjectQuery = 0,
    TraceQuery = 1,

}; // !enum classECollisionQueryType

#endif // !__PHYSICS_ENUMS_HPP__