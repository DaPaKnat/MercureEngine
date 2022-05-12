#include "PCH.hpp"

#include "Mathematic/Utility.hpp"

//==============================[Public Static Methods]==============================//

/**
 * Clamps an arbitrary angle to be between the given angles.  Will clamp to nearest boundary.
 *
 * @param p_angleDegrees    Floating point value to corresponding to the angle in Degree to convert
 * @param p_minAngleDegrees "from" angle that defines the beginning of the range of valid angles (sweeping clockwise)
 * @param p_maxAngleDegrees "to" angle that defines the end of the range of valid angles
 *
 * @return Returns clamped angle in the range -180..180.
 */
double      Math::ClampAngleDeg (double const       p_angleDegrees,
                                 double const       p_minAngleDegrees,
                                 double const       p_maxAngleDegrees)
{
    double maxDelta         {Math::Fmod(p_maxAngleDegrees - p_minAngleDegrees, 360.0) * 0.5};
    if (maxDelta < 0.0)
        maxDelta += 180.0;

    double rangeCenter      {Math::Fmod(p_minAngleDegrees + maxDelta, 360.0)};
    if (rangeCenter < 0.0)
        rangeCenter += 360.0;

    double deltaFromCenter  {Math::UnwindDeg(p_angleDegrees - rangeCenter)};

    if (deltaFromCenter > maxDelta)
    {
        return Math::UnwindDeg(rangeCenter + maxDelta);
    }
    else if (deltaFromCenter < -maxDelta)
    {
        return Math::UnwindDeg(rangeCenter - maxDelta);
    }

    return Math::UnwindDeg(p_angleDegrees);
}

/**
 * Clamps an arbitrary angle to be between the given angles.  Will clamp to nearest boundary.
 *
 * @param p_angleDegrees    Floating point value to corresponding to the angle in Degree to convert
 * @param p_minAngleDegrees "from" angle that defines the beginning of the range of valid angles (sweeping clockwise)
 * @param p_maxAngleDegrees "to" angle that defines the end of the range of valid angles
 *
 * @return Returns clamped angle in the range -180..180.
 */
float       Math::ClampAngleDeg (float const        p_angleDegrees,
                                 float const        p_minAngleDegrees,
                                 float const        p_maxAngleDegrees)
{
    float maxDelta          {Math::Fmod(p_maxAngleDegrees - p_minAngleDegrees, 360.0f) * 0.5f};
    if (maxDelta < 0.0f)
        maxDelta += 180.0f;

    float rangeCenter       {Math::Fmod(p_minAngleDegrees + maxDelta, 360.0f)};
    if (rangeCenter < 0.0f)
        rangeCenter += 360.0f;

    float deltaFromCenter   {Math::UnwindDeg(p_angleDegrees - rangeCenter)};

    if (deltaFromCenter > maxDelta)
    {
        return Math::UnwindDeg(rangeCenter + maxDelta);
    }
    else if (deltaFromCenter < -maxDelta)
    {
        return Math::UnwindDeg(rangeCenter - maxDelta);
    }

    return Math::UnwindDeg(p_angleDegrees);
}

/**
 * Clamps an arbitrary angle to be between the given angles.  Will clamp to nearest boundary.
 *
 * @param p_angleDegrees    Floating point value to corresponding to the angle in Degree to convert
 * @param p_minAngleDegrees "from" angle that defines the beginning of the range of valid angles (sweeping clockwise)
 * @param p_maxAngleDegrees "to" angle that defines the end of the range of valid angles
 *
 * @return Returns clamped angle in the range -180..180.
 */
long double Math::ClampAngleDeg (long double const  p_angleDegrees,
                                 long double const  p_minAngleDegrees,
                                 long double const  p_maxAngleDegrees)
{
    long double maxDelta        {Math::Fmod(p_maxAngleDegrees - p_minAngleDegrees, 360.0l) * 0.5l};
    if (maxDelta < 0.0l)
        maxDelta += 180.0l;

    long double rangeCenter     {Math::Fmod(p_minAngleDegrees + maxDelta, 360.0l)};
    if (rangeCenter < 0.0l)
        rangeCenter += 360.0l;

    long double deltaFromCenter {Math::UnwindDeg(p_angleDegrees - rangeCenter)};

    if (deltaFromCenter > maxDelta)
    {
        return Math::UnwindDeg(rangeCenter + maxDelta);
    }
    else if (deltaFromCenter < -maxDelta)
    {
        return Math::UnwindDeg(rangeCenter - maxDelta);
    }

    return Math::UnwindDeg(p_angleDegrees);
}

/**
 * Clamps an arbitrary angle to be between the given angles.  Will clamp to nearest boundary.
 *
 * @param p_angleDegrees    Floating point value to corresponding to the angle in Degree to convert
 * @param p_minAngleDegrees "from" angle that defines the beginning of the range of valid angles (sweeping clockwise)
 * @param p_maxAngleDegrees "to" angle that defines the end of the range of valid angles
 *
 * @return Returns clamped angle in the range -PI..PI.
 */
double      Math::ClampAngleRad (double const       p_angleRadians,
                                 double const       p_minAngleRadians,
                                 double const       p_maxAngleRadians)
{
    double maxDelta         {Math::Fmod(p_maxAngleRadians - p_minAngleRadians, MATH_TAU) * 0.5};
    if (maxDelta < 0.0)
        maxDelta += MATH_PI;

    double rangeCenter      {Math::Fmod(p_minAngleRadians + maxDelta, MATH_TAU)};
    if (rangeCenter < 0.0)
        rangeCenter += MATH_TAU;

    double deltaFromCenter  {Math::UnwindRad(p_angleRadians - rangeCenter)};

    if (deltaFromCenter > maxDelta)
    {
        return Math::UnwindRad(rangeCenter + maxDelta);
    }
    else if (deltaFromCenter < -maxDelta)
    {
        return Math::UnwindRad(rangeCenter - maxDelta);
    }

    return Math::UnwindRad(p_angleRadians);
}

/**
 * Clamps an arbitrary angle to be between the given angles.  Will clamp to nearest boundary.
 *
 * @param p_angleDegrees    Floating point value to corresponding to the angle in Degree to convert
 * @param p_minAngleDegrees "from" angle that defines the beginning of the range of valid angles (sweeping clockwise)
 * @param p_maxAngleDegrees "to" angle that defines the end of the range of valid angles
 *
 * @return Returns clamped angle in the range -PI..PI.
 */
float       Math::ClampAngleRad (float const        p_angleRadians,
                                 float const        p_minAngleRadians,
                                 float const        p_maxAngleRadians)
{
    float maxDelta          {Math::Fmod(p_maxAngleRadians - p_minAngleRadians, MATH_TAU_F) * 0.5f};
    if (maxDelta < 0.0f)
        maxDelta += MATH_PI_F;

    float rangeCenter       {Math::Fmod(p_minAngleRadians + maxDelta, MATH_TAU_F)};
    if (rangeCenter < 0.0f)
        rangeCenter += MATH_TAU_F;

    float deltaFromCenter   {Math::UnwindRad(p_angleRadians - rangeCenter)};

    if (deltaFromCenter > maxDelta)
    {
        return Math::UnwindRad(rangeCenter + maxDelta);
    }
    else if (deltaFromCenter < -maxDelta)
    {
        return Math::UnwindRad(rangeCenter - maxDelta);
    }

    return Math::UnwindRad(p_angleRadians);
}

/**
 * Clamps an arbitrary angle to be between the given angles.  Will clamp to nearest boundary.
 *
 * @param p_angleDegrees    Floating point value to corresponding to the angle in Degree to convert
 * @param p_minAngleDegrees "from" angle that defines the beginning of the range of valid angles (sweeping clockwise)
 * @param p_maxAngleDegrees "to" angle that defines the end of the range of valid angles
 *
 * @return Returns clamped angle in the range -PI..PI.
 */
long double Math::ClampAngleRad (long double const  p_angleRadians,
                                 long double const  p_minAngleRadians,
                                 long double const  p_maxAngleRadians)
{
    long double maxDelta        {Math::Fmod(p_maxAngleRadians - p_minAngleRadians, MATH_TAU_L) * 0.5l};
    if (maxDelta < 0.0l)
        maxDelta += MATH_PI_L;

    long double rangeCenter     {Math::Fmod(p_minAngleRadians + maxDelta, MATH_TAU_L)};
    if (rangeCenter < 0.0l)
        rangeCenter += MATH_TAU_L;

    long double deltaFromCenter {Math::UnwindRad(p_angleRadians - rangeCenter)};

    if (deltaFromCenter > maxDelta)
    {
        return Math::UnwindRad(rangeCenter + maxDelta);
    }
    else if (deltaFromCenter < -maxDelta)
    {
        return Math::UnwindRad(rangeCenter - maxDelta);
    }

    return Math::UnwindRad(p_angleRadians);
}

/**
 * Convert the given angle to an angle between [-180,180] degrees.
 *
 * @param p_angleDegree Floating point/Integral angle in degrees to convert.
 *
 * @return              The converted angle in [-180,180] range.
 */
template <typename ReturnType, typename Type>
ReturnType  Math::UnwindDeg     (Type const         p_angleDegree)
{
    static_assert(std::is_arithmetic<ReturnType>::value, "ReturnType must be an arithmetic type !");
    static_assert(std::is_arithmetic<Type>::value, "Type must be an arithmetic type !");

    double angleDegree  {(double)p_angleDegree};
    while (angleDegree > 180.0)
        angleDegree -= 360.0;
    while (angleDegree < -180.0)
        angleDegree += 360.0;
    return (ReturnType)angleDegree;
}

/**
 * Convert the given angle to an angle between [-180,180] degrees.
 *
 * @param p_angleDegree Floating point angle in degrees to convert.
 *
 * @return              The converted angle in [-180,180] range.
 */
double      Math::UnwindDeg     (double             p_angleDegree)
{
    while (p_angleDegree > 180.0)
        p_angleDegree -= 360.0;
    while (p_angleDegree < -180.0)
        p_angleDegree += 360.0;
    return p_angleDegree;
}

/**
 * Convert the given angle to an angle between [-180,180] degrees.
 *
 * @param p_angleDegree Floating point angle in degrees to convert.
 *
 * @return              The converted angle in [-180,180] range.
 */
float       Math::UnwindDeg     (float              p_angleDegree)
{
    while (p_angleDegree > 180.0f)
        p_angleDegree -= 360.0f;
    while (p_angleDegree < -180.0f)
        p_angleDegree += 360.0f;
    return p_angleDegree;
}

/**
 * Convert the given angle to an angle between [-180,180] degrees.
 *
 * @param p_angleDegree Floating point angle in degrees to convert.
 *
 * @return              The converted angle in [-180,180] range.
 */
long double Math::UnwindDeg     (long double        p_angleDegree)
{
    while (p_angleDegree > 180.0l)
        p_angleDegree -= 360.0l;
    while (p_angleDegree < -180.0l)
        p_angleDegree += 360.0l;
    return p_angleDegree;
}

/**
 * Convert the given angle to an angle between [-PI,PI] radians.
 *
 * @param p_angleRadian Floating point/Integral angle in radians to convert.
 *
 * @return              The converted angle in [-PI,PI] range.
 */
template <typename ReturnType, typename Type>
ReturnType  Math::UnwindRad     (Type const         p_angleRadian)
{
    static_assert(std::is_arithmetic<ReturnType>::value, "ReturnType must be an arithmetic type !");
    static_assert(std::is_arithmetic<Type>::value, "Type must be an arithmetic type !");

    double angleRadian {(double)p_angleRadian};
    while (angleRadian > MATH_PI)
        angleRadian -= MATH_TAU;
    while (angleRadian < -MATH_PI)
        angleRadian += MATH_TAU;
    return (ReturnType)angleRadian;
}

/**
 * Convert the given angle to an angle between [-PI,PI] radians.
 *
 * @param p_angleRadian Floating point angle in radians to convert.
 *
 * @return              The converted angle in [-PI,PI] range.
 */
double      Math::UnwindRad     (double             p_angleRadian)
{
    while (p_angleRadian > MATH_PI)
        p_angleRadian -= MATH_TAU;
    while (p_angleRadian < -MATH_PI)
        p_angleRadian += MATH_TAU;
    return p_angleRadian;
}

/**
 * Convert the given angle to an angle between [-PI,PI] radians.
 *
 * @param p_angleRadian Floating point angle in radians to convert.
 *
 * @return              The converted angle in [-PI,PI] range.
 */
float       Math::UnwindRad     (float              p_angleRadian)
{
    while (p_angleRadian > MATH_PI_F)
        p_angleRadian -= MATH_TAU_F;
    while (p_angleRadian < -MATH_PI_F)
        p_angleRadian += MATH_TAU_F;
    return p_angleRadian;
}

/**
 * Convert the given angle to an angle between [-PI,PI] radians.
 *
 * @param p_angleRadian Floating point angle in radians to convert.
 *
 * @return              The converted angle in [-PI,PI] range.
 */
long double Math::UnwindRad     (long double        p_angleRadian)
{
    while (p_angleRadian > MATH_PI_L)
        p_angleRadian -= MATH_TAU_L;
    while (p_angleRadian < -MATH_PI_L)
        p_angleRadian += MATH_TAU_L;
    return p_angleRadian;
}