#ifndef __MATH_UTILITY_INL__
#define __MATH_UTILITY_INL__

//==============================[Public Static Methods]==============================//

/**
 * Compute the absolute value.
 *
 * @param p_value   Floating point/Integral value to convert.
 *
 * @return          Return  p_value  (If p_value >= 0)
 * @return          Return -p_value  (If p_value < 0)
 */
template <typename Type>
CONSTEXPR   Type        Math::Abs                   (Type const         p_value)
{
    static_assert(std::is_arithmetic<Type>::value, "Type must be an arithmetic type !");
    return  (p_value < 0) ? -p_value : p_value;
}

/**
 * Compute the absolute value.
 *
 * @param p_value   Floating point value to convert.
 *
 * @return          Return  p_value  (If p_value >= 0)
 * @return          Return -p_value  (If p_value < 0) 
 */
FORCEINLINE double      Math::Abs                   (double const       p_value)
{ 
    return fabs(p_value);
}

/**
 * Compute the absolute value.
 *
 * @param p_value   Floating point value to convert.
 *
 * @return          Return  p_value  (If p_value >= 0)
 * @return          Return -p_value  (If p_value < 0)
 */
FORCEINLINE float       Math::Abs                   (float const        p_value)
{ 
    return fabsf(p_value);
}

/**
 * Compute the absolute value.
 *
 * @param p_value   Floating point value to convert.
 *
 * @return          Return  p_value  (If p_value >= 0)
 * @return          Return -p_value  (If p_value < 0)
 */
FORCEINLINE long double Math::Abs                   (long double const  p_value)
{ 
    return fabsl(p_value);
}

/**
 * Compute the arc cosine with the value given
 *
 * @param p_value   Floating point/Integral value whose arc cosine is computed, in the interval [-1,1]. If the argument is out of this interval, a domain error occurs.
 *
 * @return          The result of the arc cosine of p_value, in the interval [0,pi] radians.
 */
template <typename ReturnType, typename Type>
FORCEINLINE ReturnType  Math::Acos                  (Type const         p_value)
{    
    static_assert(std::is_arithmetic<ReturnType>::value, "ReturnType must be an arithmetic type !");
    static_assert(std::is_arithmetic<Type>::value, "Type must be an arithmetic type !");
    return  (ReturnType)acos((double)p_value);
}

/**
 * Compute the arc cosine with the value given
 *
 * @param p_value   Floating point value whose arc cosine is computed, in the interval [-1,1]. If the argument is out of this interval, a domain error occurs.
 *
 * @return          The result of the arc cosine of p_value, in the interval [0,pi] radians.
 */
FORCEINLINE double      Math::Acos                  (double const       p_value)
{
    return acos(p_value);
}

/**
 * Compute the arc cosine with the value given
 *
 * @param p_value   Floating point value whose arc cosine is computed, in the interval [-1,1]. If the argument is out of this interval, a domain error occurs.
 *
 * @return          The result of the arc cosine of p_value, in the interval [0,pi] radians.
 */
FORCEINLINE float       Math::Acos                  (float const        p_value)
{
    return acosf(p_value);
}

/**
 * Compute the arc cosine with the value given
 *
 * @param p_value   Floating point value whose arc cosine is computed, in the interval [-1,1]. If the argument is out of this interval, a domain error occurs.
 *
 * @return          The result of the arc cosine of p_value, in the interval [0,pi] radians.
 */
FORCEINLINE long double Math::Acos                  (long double const  p_value)
{
    return acosl(p_value);
}

/**
 * Compute the area hyperbolic cosine with the value given.
 * The area hyperbolic cosine is the inverse operation of the hyperbolic cosine.
 *
 * @param p_value   Floating point/Integral value whose area hyperbolic cosine is computed. If the argument is less than 1, a domain error occurs.
 *
 * @return          Nonnegative area hyperbolic cosine of p_value, in the interval [0,+INFINITY]. Note that the negative of this value is also a valid area hyperbolic cosine of p_value
 */
template <typename ReturnType, typename Type>
FORCEINLINE ReturnType  Math::AcosH                 (Type const         p_value)
{
    static_assert(std::is_arithmetic<ReturnType>::value, "ReturnType must be an arithmetic type !");
    static_assert(std::is_arithmetic<Type>::value, "Type must be an arithmetic type !");
    return (ReturnType)acosh((double)p_value);
}

/**
 * Compute the area hyperbolic cosine with the value given.
 * The area hyperbolic cosine is the inverse operation of the hyperbolic cosine.
 *
 * @param p_value   Floating point value whose area hyperbolic cosine is computed. If the argument is less than 1, a domain error occurs.
 *
 * @return          Nonnegative area hyperbolic cosine of p_value, in the interval [0,+INFINITY]. Note that the negative of this value is also a valid area hyperbolic cosine of p_value
 */
FORCEINLINE double      Math::AcosH                 (double const       p_value)
{
    return acosh(p_value);
}

/**
 * Compute the area hyperbolic cosine with the value given.
 * The area hyperbolic cosine is the inverse operation of the hyperbolic cosine.
 *
 * @param p_value   Floating point value whose area hyperbolic cosine is computed. If the argument is less than 1, a domain error occurs.
 *
 * @return          Nonnegative area hyperbolic cosine of p_value, in the interval [0,+INFINITY]. Note that the negative of this value is also a valid area hyperbolic cosine of p_value
 */
FORCEINLINE float       Math::AcosH                 (float const        p_value)
{
    return acoshf(p_value);
}

/**
 * Compute the area hyperbolic cosine with the value given.
 * The area hyperbolic cosine is the inverse operation of the hyperbolic cosine.
 *
 * @param p_value   Floating point value whose area hyperbolic cosine is computed. If the argument is less than 1, a domain error occurs.
 *
 * @return          Nonnegative area hyperbolic cosine of p_value, in the interval [0,+INFINITY]. Note that the negative of this value is also a valid area hyperbolic cosine of p_value
 */
FORCEINLINE long double Math::AcosH                 (long double const  p_value)
{
    return acoshl(p_value);
}

/**
 * Compute the arc sine with the value given
 *
 * @param p_value   Floating point/Integral value whose arc sine is computed, in the interval [-1,1]. If the argument is out of this interval, a domain error occurs.
 *
 * @return          The result of the arc sine of p_value, in the interval [-pi/2,+pi/2] radians.
 */
template <typename ReturnType, typename Type>
FORCEINLINE ReturnType  Math::Asin                  (Type const         p_value)
{
    static_assert(std::is_arithmetic<ReturnType>::value, "ReturnType must be an arithmetic type !");
    static_assert(std::is_arithmetic<Type>::value, "Type must be an arithmetic type !");
    return (ReturnType)asin((double)p_value);
}

/**
 * Compute the arc sine with the value given
 *
 * @param p_value   Floating point value whose arc sine is computed, in the interval [-1,1]. If the argument is out of this interval, a domain error occurs.
 *
 * @return          The result of the arc sine of p_value, in the interval [-pi/2,+pi/2] radians.
 */
FORCEINLINE double      Math::Asin                  (double const       p_value)
{
    return asin(p_value);
}

/**
 * Compute the arc sine with the value given
 *
 * @param p_value   Floating point value whose arc sine is computed, in the interval [-1,1]. If the argument is out of this interval, a domain error occurs.
 *
 * @return          The result of the arc sine of p_value, in the interval [-pi/2,+pi/2] radians.
 */
FORCEINLINE float       Math::Asin                  (float const        p_value)
{
    return asinf(p_value);
}

/**
 * Compute the arc sine with the value given
 *
 * @param p_value   Floating point value whose arc sine is computed, in the interval [-1,1]. If the argument is out of this interval, a domain error occurs.
 *
 * @return          The result of the arc sine of p_value, in the interval [-pi/2,+pi/2] radians.
 */
FORCEINLINE long double Math::Asin                  (long double const  p_value)
{
    return asinl(p_value);
}

/**
 * Compute the area hyperbolic sine with the value given.
 * The area hyperbolic sine is the inverse operation of the hyperbolic sine.
 *
 * @param p_value   Floating point/Integral value whose area hyperbolic sine is computed.
 *
 * @return          The hyperbolic sine of p_value.
 */
template <typename ReturnType, typename Type>
FORCEINLINE ReturnType  Math::AsinH                 (Type const         p_value)
{
    static_assert(std::is_arithmetic<ReturnType>::value, "ReturnType must be an arithmetic type !");
    static_assert(std::is_arithmetic<Type>::value, "Type must be an arithmetic type !");
    return (ReturnType)asin((double)p_value);
}

/**
 * Compute the area hyperbolic sine with the value given.
 * The area hyperbolic sine is the inverse operation of the hyperbolic sine.
 *
 * @param p_value   Floating point value whose area hyperbolic sine is computed.
 *
 * @return          The hyperbolic sine of p_value.
 */
FORCEINLINE double      Math::AsinH                 (double const       p_value)
{
    return asinh(p_value);
}

/**
 * Compute the area hyperbolic sine with the value given.
 * The area hyperbolic sine is the inverse operation of the hyperbolic sine.
 *
 * @param p_value   Floating point value whose area hyperbolic sine is computed.
 *
 * @return          The hyperbolic sine of p_value.
 */
FORCEINLINE float       Math::AsinH                 (float const        p_value)
{
    return asinhf(p_value);
}

/**
 * Compute the area hyperbolic sine with the value given.
 * The area hyperbolic sine is the inverse operation of the hyperbolic sine.
 *
 * @param p_value   Floating point value whose area hyperbolic sine is computed.
 *
 * @return          The hyperbolic sine of p_value.
 */
FORCEINLINE long double Math::AsinH                 (long double const  p_value)
{
    return asinhl(p_value);
}

/**
 * Compute the arc tangent with the value given.
 * Notice that because of the sign ambiguity, the function cannot determine with certainty in which quadrant
 * the angle falls only by its tangent value. See atan2 for an alternative that takes a fractional argument instead.
 *
 * @param p_value   Floating point/Integral value whose arc tangent is computed.
 *
 * @return          The arc tangent of p_value, in the interval [-pi/2,+pi/2] radians.
 */
template <typename ReturnType, typename Type>
FORCEINLINE ReturnType  Math::Atan                  (Type const         p_value)
{
    static_assert(std::is_arithmetic<ReturnType>::value, "ReturnType must be an arithmetic type !");
    static_assert(std::is_arithmetic<Type>::value, "Type must be an arithmetic type !");
    return (ReturnType)atan((double)p_value);
}

/**
 * Compute the arc tangent with the value given.
 * Notice that because of the sign ambiguity, the function cannot determine with certainty in which quadrant 
 * the angle falls only by its tangent value. See atan2 for an alternative that takes a fractional argument instead.
 *
 * @param p_value   Floating point value whose arc tangent is computed.
 *
 * @return          The arc tangent of p_value, in the interval [-pi/2,+pi/2] radians.
 */
FORCEINLINE double      Math::Atan                  (double const       p_value)
{
    return atan(p_value);
}

/**
 * Compute the arc tangent with the value given.
 * Notice that because of the sign ambiguity, the function cannot determine with certainty in which quadrant
 * the angle falls only by its tangent value. See atan2 for an alternative that takes a fractional argument instead.
 *
 * @param p_value   Floating point value whose arc tangent is computed.
 *
 * @return          The arc tangent of p_value, in the interval [-pi/2,+pi/2] radians.
 */
FORCEINLINE float       Math::Atan                  (float const        p_value)
{
    return atanf(p_value);
}

/**
 * Compute the arc tangent with the value given.
 * Notice that because of the sign ambiguity, the function cannot determine with certainty in which quadrant
 * the angle falls only by its tangent value. See atan2 for an alternative that takes a fractional argument instead.
 *
 * @param p_value   Floating point value whose arc tangent is computed.
 *
 * @return          The arc tangent of p_value, in the interval [-pi/2,+pi/2] radians.
 */
FORCEINLINE long double Math::Atan                  (long double const  p_value)
{
    return atanl(p_value);
}

/**
 * Compute the arc tangent with two parameters given
 * To compute the result, the function takes into account the sign of both arguments in order to determine the quadrant.
 *
 * @param p_y       Floating point/Integral value representing the proportion of the y-coordinate.
 * @param p_x       Floating point/Integral value representing the proportion of the x-coordinate.
 *
 * @return          The principal value of the arc tangent of p_y/p_x, expressed in radians.
 */
template <typename ReturnType, typename Type>
FORCEINLINE ReturnType  Math::Atan2                 (Type const         p_y,
                                                     Type const         p_x)
{
    static_assert(std::is_arithmetic<ReturnType>::value, "ReturnType must be an arithmetic type !");
    static_assert(std::is_arithmetic<Type>::value, "Type must be an arithmetic type !");
    return (ReturnType)atan2((double)p_y, (double)p_x);
}

/**
 * Compute the arc tangent with two parameters given
 * To compute the result, the function takes into account the sign of both arguments in order to determine the quadrant.
 *
 * @param p_y       Floating point value representing the proportion of the y-coordinate.
 * @param p_x       Floating point value representing the proportion of the x-coordinate.
 *
 * @return          The principal value of the arc tangent of p_y/p_x, expressed in radians.
 */
FORCEINLINE double      Math::Atan2                 (double const       p_y,
                                                     double const       p_x)
{
    return atan2(p_y,p_x);
}

/**
 * Compute the arc tangent with two parameters given
 * To compute the result, the function takes into account the sign of both arguments in order to determine the quadrant.
 *
 * @param p_y       Floating point value representing the proportion of the y-coordinate.
 * @param p_x       Floating point value representing the proportion of the x-coordinate.
 *
 * @return          The principal value of the arc tangent of p_y/p_x, expressed in radians.
 */
FORCEINLINE float       Math::Atan2                 (float const        p_y,
                                                     float const        p_x)
{
    return atan2f(p_y,p_x);
}

/**
 * Compute the arc tangent with two parameters given
 * To compute the result, the function takes into account the sign of both arguments in order to determine the quadrant.
 *
 * @param p_y       Floating point value representing the proportion of the y-coordinate.
 * @param p_x       Floating point value representing the proportion of the x-coordinate.
 *
 * @return          The principal value of the arc tangent of p_y/p_x, expressed in radians.
 */
FORCEINLINE long double Math::Atan2                 (long double const  p_y,
                                                     long double const  p_x)
{
    return atan2l(p_y,p_x);
}

/**
 * Compute the area hyperbolic tangent with the value given.
 * The area hyperbolic tangent is the inverse operation of the hyperbolic tangent.
 *
 * @param p_value   Floating point/Integral value whose area hyperbolic tangent is computed, in the interval [-1,+1].
 *
 * @return          The area hyperbolic tangent of p_value.
 */
template <typename ReturnType, typename Type>
FORCEINLINE ReturnType  Math::AtanH                 (Type const         p_value)
{
    static_assert(std::is_arithmetic<ReturnType>::value, "ReturnType must be an arithmetic type !");
    static_assert(std::is_arithmetic<Type>::value, "Type must be an arithmetic type !");
    return (ReturnType)atanh((double)p_value);
}

/**
 * Compute the area hyperbolic tangent with the value given.
 * The area hyperbolic tangent is the inverse operation of the hyperbolic tangent.
 *
 * @param p_value   Floating point value whose area hyperbolic tangent is computed, in the interval [-1,+1]. 
 *
 * @return          The area hyperbolic tangent of p_value.
 */
FORCEINLINE double      Math::AtanH                 (double const       p_value)
{
    return atanh(p_value);
}

/**
 * Compute the area hyperbolic tangent with the value given.
 * The area hyperbolic tangent is the inverse operation of the hyperbolic tangent.
 *
 * @param p_value   Floating point value whose area hyperbolic tangent is computed, in the interval [-1,+1].
 *
 * @return          The area hyperbolic tangent of p_value.
 */
FORCEINLINE float       Math::AtanH                 (float const        p_value)
{
    return atanhf(p_value);
}

/**
 * Compute the area hyperbolic tangent with the value given.
 * The area hyperbolic tangent is the inverse operation of the hyperbolic tangent.
 *
 * @param p_value   Floating point value whose area hyperbolic tangent is computed, in the interval [-1,+1].
 *
 * @return          The area hyperbolic tangent of p_value.
 */
FORCEINLINE long double Math::AtanH                 (long double const  p_value)
{
    return atanhl(p_value);
}


/**
 * Compute the cubic root of the value given
 *
 * @param p_value   Floating point/Integral value whose cubic root is computed
 *
 * @return          The cubic root of p_value.
 */
template <typename ReturnType, typename Type>
FORCEINLINE ReturnType  Math::Cbrt                  (Type const         p_value)
{
    static_assert(std::is_arithmetic<ReturnType>::value, "ReturnType must be an arithmetic type !");
    static_assert(std::is_arithmetic<Type>::value, "Type must be an arithmetic type !");
    return (ReturnType)cbrt((double)p_value);
}

/**
 * Compute the cubic root of the value given
 *
 * @param p_value   Floating point value whose cubic root is computed
 *
 * @return          The cubic root of p_value.
 */
FORCEINLINE double      Math::Cbrt                  (double const       p_value)
{
    return cbrt(p_value);
}

/**
 * Compute the cubic root of the value given
 *
 * @param p_value   Floating point value whose cubic root is computed
 *
 * @return          The cubic root of p_value.
 */
FORCEINLINE float       Math::Cbrt                  (float const        p_value)
{
    return cbrtf(p_value);
}

/**
 * Compute the cubic root of the value given
 *
 * @param p_value   Floating point value whose cubic root is computed
 *
 * @return          The cubic root of p_value.
 */
FORCEINLINE long double Math::Cbrt                  (long double const  p_value)
{
    return cbrtl(p_value);
}

/**
 * Converts a value given to the nearest greater or equal integer.
 *
 * @param p_value   Floating point/Integral value to convert
 *
 * @return          An integer greater or equal to 'p_value'.
 */
template <typename ReturnType, typename Type>
FORCEINLINE ReturnType  Math::Ceil                  (Type const         p_value)
{
    static_assert(std::is_arithmetic<ReturnType>::value, "ReturnType must be an arithmetic type !");
    static_assert(std::is_arithmetic<Type>::value, "Type must be an arithmetic type !");
    return (ReturnType)ceil((double)p_value);
}

/**
 * Converts a value given to the nearest greater or equal integer.
 *
 * @param p_value   Floating point value to convert
 *
 * @return          An integer greater or equal to 'p_value'.
 */
FORCEINLINE double      Math::Ceil                  (double const       p_value)
{
    return ceil(p_value);
}

/**
 * Converts a value given to the nearest greater or equal integer.
 *
 * @param p_value   Floating point value to convert
 *
 * @return          An integer greater or equal to 'p_value'.
 */
FORCEINLINE float       Math::Ceil                  (float const        p_value)
{
    return ceilf(p_value);
}

/**
 * Converts a value given to the nearest greater or equal integer.
 *
 * @param p_value   Floating point value to convert
 *
 * @return          An integer greater or equal to 'p_value'.
 */
FORCEINLINE long double Math::Ceil                  (long double const  p_value)
{
    return ceill(p_value);
}

/**
 * Clamps the value given between two value given min and max included
 *
 * @param p_value   The value to clamp
 * @param p_min     The minimum limit
 * @param p_max     The maximum limit
 *
 * @return          Return p_min    if (p_value < p_min)
 * @return          Return p_value  if (p_min <= p_value && p_value <= p_max)
 * @return          Return p_max    if (p_value > p_max)
 */
template <typename Type>
CONSTEXPR   Type        Math::Clamp                 (Type const         p_value,
                                                     Type const         p_min,
                                                     Type const         p_max)
{

    static_assert(std::is_arithmetic<Type>::value, "Type must be an arithmetic type !");
    return (p_value < p_min) ? p_min : (p_value > p_max) ? p_max : p_value;
}

/**
 * Clamps the value given between 0 and 1 included.
 *
 * @param p_value   Integral value to clamp.
 *
 * @return          Return 0        if (p_value < 0)
 * @return          Return p_value  if (0 <= p_value && p_value <= 1)
 * @return          Return 1        if (p_value > 1)
 */
template <typename Type>
CONSTEXPR   Type        Math::Clamp01               (Type const         p_value)
{
    static_assert(std::is_integral<Type>::value, "p_value must be an integral value !");
    return (p_value < 0) ? 0 : (p_value > 1) ? 1 : p_value;
}

/**
 * Clamps the value given between 0.0 and 1.0 included.
 *
 * @param p_value   Floating point value to clamp.
 *
 * @return          Return 0.0      if (p_value < 0.0)
 * @return          Return p_value  if (0.0 <= p_value && p_value <= 1.0)
 * @return          Return 1.0      if (p_value > 1.0)
 */
CONSTEXPR   double      Math::Clamp01               (double const       p_value)
{
    return (p_value < 0.0) ? 0.0 : (p_value > 1.0) ? 1.0 : p_value;
}

/**
 * Clamps the value given between 0.0f and 1.0f included.
 *
 * @param p_value   Floating point value to clamp.
 *
 * @return          0.0f        if (p_value < 0.0f)
 * @return          p_value     if (0.0f <= p_value && p_value <= 1.0f)
 * @return          1.0f        if (p_value > 1.0f)
 */
CONSTEXPR   float       Math::Clamp01               (float const        p_value)
{
    return (p_value < 0.0f) ? 0.0f : (p_value > 1.0f) ? 1.0f : p_value;
}

/**
 * Clamps the value given between 0.0l and 1.0l included.
 *
 * @param p_value   Floating point value to clamp.
 *
 * @return          0.0l        if (p_value < 0.0l)
 * @return          p_value     if (0.0l <= p_value && p_value <= 1.0l)
 * @return          1.0l        if (p_value > 1.0l)
 */
CONSTEXPR   long double Math::Clamp01               (long double const  p_value)
{
    return (p_value < 0.0l) ? 0.0l : (p_value > 1.0l) ? 1.0l : p_value;
}

/**
 * Compute a value with the magnitude of first argument with the sign of the second.
 *
 * @param p_value   Floating point value with the magnitude of the resulting value.
 * @param p_sign        Floating point value with the sign of the resulting value.
 *
 * @return          The value with the magnitude of p_value with the sign of p_sign.
 */
FORCEINLINE double      Math::CopySign              (double const       p_value,
                                                     double const       p_sign)
{
    return copysign(p_value, p_sign);
}

/**
 * Compute a value with the magnitude of first argument with the sign of the second.
 *
 * @param p_value   Floating point value with the magnitude of the resulting value.
 * @param p_sign        Floating point value with the sign of the resulting value.
 *
 * @return          The value with the magnitude of p_value with the sign of p_sign.
 */
FORCEINLINE float       Math::CopySign              (float const        p_value,
                                                     float const        p_sign)
{
    return copysignf(p_value, p_sign);
}

/**
 * Compute a value with the magnitude of first argument with the sign of the second.
 *
 * @param p_value   Floating point value with the magnitude of the resulting value.
 * @param p_sign        Floating point value with the sign of the resulting value.
 *
 * @return          The value with the magnitude of p_value with the sign of p_sign.
 */
FORCEINLINE long double Math::CopySign              (long double const  p_value,
                                                     long double const  p_sign)
{
    return copysignl(p_value, p_sign);
}

/**
 * Compute the cosine of an angle given in radians
 *
 * @param p_radAngle    Floating point/Integral value representing an angle expressed in radians.
 *
 * @return              The cosine of p_radAngle
 */
template <typename ReturnType, typename Type>
FORCEINLINE ReturnType  Math::Cos                   (Type const         p_radAngle)
{
    static_assert(std::is_arithmetic<ReturnType>::value, "ReturnType must be an arithmetic type !");
    static_assert(std::is_arithmetic<Type>::value, "Type must be an arithmetic type !");
    return (ReturnType)cos((double)p_radAngle);
}

/**
 * Compute the cosine of an angle given in radians
 *
 * @param p_radAngle    Floating point value representing an angle expressed in radians.
 *
 * @return              The cosine of p_radAngle
 */
FORCEINLINE double      Math::Cos                   (double const       p_radAngle)
{
    return cos(p_radAngle);
}

/**
 * Compute the cosine of an angle given in radians
 *
 * @param p_radAngle    Floating point value representing an angle expressed in radians.
 *
 * @return              The cosine of p_radAngle
 */
FORCEINLINE float       Math::Cos                   (float const        p_radAngle)
{
    return cosf(p_radAngle);
}

/**
 * Compute the cosine of an angle given in radians
 *
 * @param p_radAngle    Floating point value representing an angle expressed in radians.
 *
 * @return              The cosine of p_radAngle
 */
FORCEINLINE long double Math::Cos                   (long double const  p_radAngle)
{
    return cosl(p_radAngle);
}

/**
 * Compute the hyperbolic cosine of an angle given in radians
 *
 * @param p_value       Floating point/Integral value representing a hyperbolic angle.
 *
 * @return              The hyperbolic cosine of p_value
 */
template <typename ReturnType, typename Type>
FORCEINLINE ReturnType  Math::CosH                  (Type const         p_value)
{
    static_assert(std::is_arithmetic<ReturnType>::value, "ReturnType must be an arithmetic type !");
    static_assert(std::is_arithmetic<Type>::value, "Type must be an arithmetic type !");
    return (ReturnType)cosh((double)p_value);
}

/**
 * Compute the hyperbolic cosine of an angle given in radians
 *
 * @param p_value       Floating point value representing a hyperbolic angle.
 *
 * @return              The hyperbolic cosine of p_value
 */
FORCEINLINE double      Math::CosH                  (double const       p_value)
{
    return cosh(p_value);
}

/**
 * Compute the hyperbolic cosine of an angle given in radians
 *
 * @param p_value       Floating point value representing a hyperbolic angle.
 *
 * @return              The hyperbolic cosine of p_value
 */
FORCEINLINE float       Math::CosH                  (float const        p_value)
{
    return coshf(p_value);
}

/**
 * Compute the hyperbolic cosine of an angle given in radians
 *
 * @param p_value       Floating point value representing a hyperbolic angle.
 *
 * @return              The hyperbolic cosine of p_value
 */
FORCEINLINE long double Math::CosH                  (long double const  p_value)
{
    return coshl(p_value);
}

/**
 * Compute the cube of the value.
 *
 * @param p_value   Floating point value whose the cube is computed
 *
 * @return          the cube of p_value: p_value*p_value*p_value = p_value^3.
 */
template <typename Type>
CONSTEXPR   Type        Math::Cube                  (Type const         p_value)
{
    static_assert(std::is_arithmetic<Type>::value, "Type must be an arithmetic type !");
    return p_value*p_value*p_value;
}

/**
 * Convert the given angle in degrees to angle in radians
 *
 * @param p_radValue    Floating point angle in degrees to convert
 *
 * @return              The angle converted in radians.
 */
template <typename ReturnType, typename Type>
CONSTEXPR   ReturnType  Math::DegToRad              (Type const         p_degAngle)
{
    static_assert(std::is_arithmetic<ReturnType>::value, "ReturnType must be an arithmetic type !");
    static_assert(std::is_arithmetic<Type>::value, "Type must be an arithmetic type !");
    return (ReturnType)(p_degAngle * MATH_DEG_2_RAD);
}

/**
 * Convert the given angle in degrees to angle in radians
 *
 * @param p_radValue    Floating point angle in degrees to convert
 *
 * @return              The angle converted in radians.
 */
CONSTEXPR   double      Math::DegToRad              (double const       p_degAngle)
{
    return p_degAngle * MATH_DEG_2_RAD;
}

/**
 * Convert the given angle in degrees to angle in radians
 *
 * @param p_radValue    Floating point angle in degrees to convert
 *
 * @return              The angle converted in radians.
 */
CONSTEXPR   float       Math::DegToRad              (float const        p_degAngle)
{
    return p_degAngle * MATH_DEG_2_RAD_F;
}

/**
 * Convert the given angle in degrees to angle in radians
 *
 * @param p_radValue    Floating point angle in degrees to convert
 *
 * @return              The angle converted in radians.
 */
CONSTEXPR   long double Math::DegToRad              (long double const  p_degAngle)
{
    return p_degAngle * MATH_DEG_2_RAD_L;
}

/**
 * Compute the shortest difference in ]-180,180] range between two given angles given in degrees .
 *
 * @param p_current Floating point start angle in Degree
 * @param p_target  Floating point end angle in Degree
 *
 * @return  The smallest angle beetween p_current to p_target in ]-180,180] range in degree
 */
FORCEINLINE double      Math::DeltaAngleDeg         (double const       p_current,
                                                     double const       p_target)
{
    double delta {Math::Repeat(p_target - p_current, 360.0)};
    
    if (delta > 180.0)
        delta -= 360.0;
    
    return delta;
}

/**
 * Compute the shortest difference in ]-180,180] range between two given angles given in degrees .
 *
 * @param p_current Floating point start angle in Degree
 * @param p_target  Floating point end angle in Degree
 *
 * @return  The smallest angle beetween p_current to p_target in ]-180,180] range in degree
 */
FORCEINLINE float       Math::DeltaAngleDeg         (float const        p_current,
                                                     float const        p_target)
{
    float delta {Math::Repeat(p_target - p_current, 360.0f)};
    
    if (delta > 180.0f)
        delta -= 360.0f;
    
    return delta;
}

/**
 * Compute the shortest difference in [-180,180] range between two given angles given in degrees .
 *
 * @param p_current Floating point start angle in Degree
 * @param p_target  Floating point end angle in Degree
 *
 * @return  The smallest angle beetween p_current to p_target in [-180,180] range in degree
 */
FORCEINLINE long double Math::DeltaAngleDeg         (long double const  p_current,
                                                     long double const  p_target)
{
    long double delta {Math::Repeat(p_target - p_current, 360.0l)};
    
    if (delta > 180.0l)
        delta -= 360.0l;
    
    return delta;
}


/**
 * Compute the shortest difference in ]-PI,PI] range between two given angles given in Radian .
 *
 * @param p_current Floating point start angle in Radian
 * @param p_target  Floating point end angle in Radian
 *
 * @return  The smallest angle beetween p_current to p_target in ]-PI,PI] range in Radian
 */
FORCEINLINE double      Math::DeltaAngleRad         (double const       p_current,
                                                     double const       p_target)
{
    double delta {Math::Repeat(p_target - p_current, MATH_TAU)};
    
    if (delta > MATH_PI)
        delta -= MATH_TAU;
    
    return delta;
}

/**
 * Compute the shortest difference in ]-PI,PI] range between two given angles given in Radian .
 *
 * @param p_current Floating point start angle in Radian
 * @param p_target  Floating point end angle in Radian
 *
 * @return  The smallest angle beetween p_current to p_target in ]-PI,PI] range in Radian
 */
FORCEINLINE float       Math::DeltaAngleRad         (float const        p_current,
                                                     float const        p_target)
{
    float delta {Math::Repeat(p_target - p_current, MATH_TAU_F)};
    
    if (delta > MATH_PI_F)
        delta -= MATH_TAU_F;
    
    return delta;
}

/**
 * Compute the shortest difference in ]-PI,PI] range between two given angles given in Radian .
 *
 * @param p_current Floating point start angle in Radian
 * @param p_target  Floating point end angle in Radian
 *
 * @return  The smallest angle beetween p_current to p_target in ]-PI,PI] range in Radian
 */
FORCEINLINE long double Math::DeltaAngleRad         (long double const  p_current,
                                                     long double const  p_target)
{
    long double delta {Math::Repeat(p_target - p_current, MATH_TAU_L)};
    
    if (delta > MATH_PI_L)
        delta -= MATH_TAU_L;
    
    return delta;
}

/**
 * Computes the exponential raised to the specified power
 * 
 * @param p_value   Floating point/Integral value of the exponent
 *
 * @return          The exponential value of p_value (e^p_value).
 */
template <typename ReturnType, typename Type>
FORCEINLINE ReturnType  Math::Exp                   (Type const         p_value)
{
    static_assert(std::is_arithmetic<ReturnType>::value, "ReturnType must be an arithmetic type !");
    static_assert(std::is_arithmetic<Type>::value, "Type must be an arithmetic type !");
    return (ReturnType)exp((double)p_value);
}

/**
 * Computes the exponential raised to the specified power
 * 
 * @param p_value   Floating point value of the exponent
 *
 * @return          The exponential value of p_value (e^p_value).
 */
FORCEINLINE double      Math::Exp                   (double const       p_value)
{
    return exp(p_value);
}

/**
 * Computes the exponential raised to the specified power
 *
 * @param p_value   Floating point value of the exponent
 *
 * @return          The exponential value of p_value (e^p_value).
 */
FORCEINLINE float       Math::Exp                   (float const        p_value)
{
    return expf(p_value);
}

/**
 * Computes the exponential raised to the specified power
 *
 * @param p_value   Floating point value of the exponent
 *
 * @return          The exponential value of p_value (e^p_value).
 */
FORCEINLINE long double Math::Exp                   (long double const  p_value)
{
    return expl(p_value);
}

/**
 * Compute the base 2 raised to the power given
 *
 * @param p_value   Floating point/Integral value of the exponent
 *
 * @return          The result of two raised to the power of p_value.
 */
template <typename ReturnType, typename Type>
FORCEINLINE ReturnType  Math::Exp2                  (Type const         p_value)
{
    static_assert(std::is_arithmetic<ReturnType>::value, "ReturnType must be an arithmetic type !");
    static_assert(std::is_arithmetic<Type>::value, "Type must be an arithmetic type !");
    return (ReturnType)exp2((double)p_value);
}

/**
 * Compute the base 2 raised to the power given
 *
 * @param p_value   Floating point value of the exponent
 *
 * @return          The result of two raised to the power of p_value.
 */
FORCEINLINE double      Math::Exp2                  (double const       p_value)
{
    return exp2(p_value);
}

/**
 * Compute the base 2 raised to the power given
 *
 * @param p_value   Floating point value of the exponent
 *
 * @return          The result of two raised to the power of p_value.
 */
FORCEINLINE float       Math::Exp2                  (float const        p_value)
{
    return exp2f(p_value);
}

/**
 * Compute the base 2 raised to the power given
 *
 * @param p_value   Floating point value of the exponent
 *
 * @return          The result of two raised to the power of p_value.
 */
FORCEINLINE long double Math::Exp2                  (long double const  p_value)
{
    return exp2l(p_value);
}

/**
 * Returns value based on comparand. The main purpose of this function is to avoid
 * branching based on floating point comparison which can be avoided via compiler
 * intrinsics.
 *
 * Please note that we don't define what happens in the case of NaNs as there might
 * be platform specific differences.
 *
 * @param	p_comparand		    Comparand the results are based on
 * @param	p_valueGEZero		Return value if Comparand >= 0
 * @param	p_valueLTZero		Return value if Comparand < 0
 *
 * @return	                    ValueGEZero if Comparand >= 0, ValueLTZero otherwise
 */
CONSTEXPR   double      Math::FloatSelect           (double const       p_comparand,
                                                     double const       p_valueGEZero,
                                                     double const       p_valueLTZero)
{
    return p_comparand >= 0.0 ? p_valueGEZero : p_valueLTZero;
}

/**
 * Returns value based on comparand. The main purpose of this function is to avoid
 * branching based on floating point comparison which can be avoided via compiler
 * intrinsics.
 *
 * Please note that we don't define what happens in the case of NaNs as there might
 * be platform specific differences.
 *
 * @param	p_comparand		    Comparand the results are based on
 * @param	p_valueGEZero		Return value if Comparand >= 0
 * @param	p_valueLTZero		Return value if Comparand < 0
 *
 * @return	                    ValueGEZero if Comparand >= 0, ValueLTZero otherwise
 */
CONSTEXPR   float       Math::FloatSelect           (float const        p_comparand,
                                                     float const        p_valueGEZero,
                                                     float const        p_valueLTZero)
{
    return p_comparand >= 0.0f ? p_valueGEZero : p_valueLTZero;
}

/**
 * Returns value based on comparand. The main purpose of this function is to avoid
 * branching based on floating point comparison which can be avoided via compiler
 * intrinsics.
 *
 * Please note that we don't define what happens in the case of NaNs as there might
 * be platform specific differences.
 *
 * @param	p_comparand		    Comparand the results are based on
 * @param	p_valueGEZero		Return value if Comparand >= 0
 * @param	p_valueLTZero		Return value if Comparand < 0
 *
 * @return	                    ValueGEZero if Comparand >= 0, ValueLTZero otherwise
 */
CONSTEXPR   long double Math::FloatSelect           (long double const  p_comparand,
                                                     long double const  p_valueGEZero,
                                                     long double const  p_valueLTZero)
{
    return p_comparand >= 0.0f ? p_valueGEZero : p_valueLTZero;
}

/**
 * Converts a value given to the nearest less or equal integer.
 *
 * @param p_value   Floating point/Integral value to convert
 *
 * @return          An integer less or equal to 'p_value'.
 */
template <typename ReturnType, typename Type>
FORCEINLINE ReturnType  Math::Floor                 (Type const         p_value)
{
    static_assert(std::is_arithmetic<ReturnType>::value, "ReturnType must be an arithmetic type !");
    static_assert(std::is_arithmetic<Type>::value, "Type must be an arithmetic type !");
    return (ReturnType)floor((double)p_value);
}

/**
 * Converts a value given to the nearest less or equal integer.
 *
 * @param p_value   Floating point value to convert
 *
 * @return          An integer less or equal to 'p_value'.
 */
FORCEINLINE double      Math::Floor                 (double const       p_value)
{
    return floor(p_value);
}

/**
 * Converts a value given to the nearest less or equal integer.
 *
 * @param p_value   Floating point value to convert
 *
 * @return          An integer less or equal to 'p_value'.
 */
FORCEINLINE float       Math::Floor                 (float const        p_value)
{
    return floorf(p_value);
}

/**
 * Converts a value given to the nearest less or equal integer.
 *
 * @param p_value   Floating point value to convert
 *
 * @return          An integer less or equal to 'p_value'.
 */
FORCEINLINE long double Math::Floor                 (long double const  p_value)
{
    return floorl(p_value);
}

/**
 * Compute the modulo of p_value by p_modulo
 * Warning: Always returns remainder toward 0, not toward the smaller multiple of p_modulo.
 *
 * @param p_value   Floating point value whose compute the modulo
 * @param p_value   Floating point value of the modulo
 *
 * @return          The floating-point remainder of p_value % p_modulo.
 */
FORCEINLINE double      Math::Fmod                  (double const       p_value,
                                                     double const       p_modulo)
{
    return fmod(p_value, p_modulo);
}

/**
 * Compute the modulo of p_value by p_modulo
 * Warning: Always returns remainder toward 0, not toward the smaller multiple of p_modulo.
 *
 * @param p_value   Floating point value whose compute the modulo
 * @param p_value   Floating point value of the modulo
 *
 * @return          The floating-point remainder of p_value % p_modulo.
 */
FORCEINLINE float       Math::Fmod                  (float const        p_value,
                                                     float const        p_modulo)
{
    return fmodf(p_value, p_modulo);
}

/**
 * Compute the modulo of p_value by p_modulo
 * Warning: Always returns remainder toward 0, not toward the smaller multiple of p_modulo.
 *
 * @param p_value   Floating point value whose compute the modulo
 * @param p_value   Floating point value of the modulo
 *
 * @return          The floating-point remainder of p_value % p_modulo.
 */
FORCEINLINE long double Math::Fmod                  (long double const  p_value,
                                                     long double const  p_modulo)
{
    return fmodl(p_value, p_modulo);
}
/**
 * Returns the fractional part of the value given.
 *
 * @param Value Floating point/Integral value to convert
 *
 * @return      The fractional part of p_value. It's a value in the range [0,1[.
 */
template <typename ReturnType, typename Type>
FORCEINLINE ReturnType  Math::Frac                  (Type const         p_value)
{
    return (ReturnType)(p_value - Math::Floor((double)p_value));
}

/**
 * Returns the fractional part of the value given.
 *
 * @param Value Floating point value to convert
 *
 * @return      The fractional part of p_value. It's a value in the range [0,1[.
 */
FORCEINLINE double      Math::Frac                  (double const       p_value)
{
    return p_value - Math::Floor(p_value);
}

/**
 * Returns the fractional part of the value given.
 *
 * @param Value Floating point value to convert
 *
 * @return      The fractional part of p_value. It's a value in the range [0,1[.
 */
FORCEINLINE float       Math::Frac                  (float const        p_value)
{
    return p_value - Math::Floor(p_value);
}

/**
 * Returns the fractional part of the value given.
 *
 * @param Value Floating point value to convert
 *
 * @return      The fractional part of p_value. It's a value in the range [0,1[.
 */
FORCEINLINE long double Math::Frac                  (long double const  p_value)
{
    return p_value - Math::Floor(p_value);
}

/**
 * Returns signed fractional part of a double.
 *
 * @param p_value   Floating point/Integral value to convert
 *
 * @return          A double between >=0 and < 1 for nonnegative input. A double between >= -1 and < 0 for negative input.
 */
template <typename ReturnType, typename Type>
FORCEINLINE ReturnType  Math::Fractional            (Type const         p_value)
{
    return (ReturnType)(p_value - Math::Trunc((double)p_value));
}

/**
 * Returns signed fractional part of a double.
 *
 * @param p_value   Floating point value to convert
 *
 * @return          A double between >=0 and < 1 for nonnegative input. A double between >= -1 and < 0 for negative input.
 */
FORCEINLINE double      Math::Fractional            (double const       p_value)
{
    return p_value - Math::Trunc(p_value);
}

/**
 * Returns signed fractional part of a double.
 *
 * @param p_value   Floating point value to convert
 *
 * @return          A double between >=0 and < 1 for nonnegative input. A double between >= -1 and < 0 for negative input.
 */
FORCEINLINE float       Math::Fractional            (float const        p_value)
{
    return p_value - Math::Trunc(p_value);
}

/**
 * Returns signed fractional part of a double.
 *
 * @param p_value   Floating point value to convert
 *
 * @return          A double between >=0 and < 1 for nonnegative input. A double between >= -1 and < 0 for negative input.
 */
FORCEINLINE long double Math::Fractional            (long double const  p_value)
{
    return p_value - Math::Trunc(p_value);
}

/**
 *  Compute the percentage along a line from a min value to a max that value wtih a value given.
 *
 *  @param p_a      The start value of the line.
 *  @param p_b      The end value of the line.
 *  @param p_value  The value to compute the percentage with.
 *
 *  @return         The percentage of p_value between p_start and p_end;
 */
template <typename ReturnType, typename Type>
FORCEINLINE ReturnType  Math::InverseLerp           (Type const         p_a,
                                                     Type const         p_b,
                                                     double const       p_value)
{
    static_assert(std::is_arithmetic<ReturnType>::value, "ReturnType must be an arithmetic type !");
    static_assert(std::is_arithmetic<Type>::value, "Type must be an arithmetic type !");
    return  (ReturnType)((p_value - p_a) / (p_b - p_a));
}

/**
 *  Compute the percentage along a line from a min value to a max that value wtih a value given.
 *
 *  @param p_a      The start value of the line.
 *  @param p_b      The end value of the line.
 *  @param p_value  The value to compute the percentage with.
 *
 *  @return         The percentage of p_value between p_start and p_end;
 */
template <typename ReturnType, typename Type>
FORCEINLINE ReturnType  Math::InverseLerp           (Type const         p_a,
                                                     Type const         p_b,
                                                     float const        p_value)
{
    static_assert(std::is_arithmetic<ReturnType>::value, "ReturnType must be an arithmetic type !");
    static_assert(std::is_arithmetic<Type>::value, "Type must be an arithmetic type !");
    return  (ReturnType)((p_value - p_a) / (p_b - p_a));
}

/**
 *  Compute the percentage along a line from a min value to a max that value wtih a value given.
 *
 *  @param p_a      The start value of the line.
 *  @param p_b      The end value of the line.
 *  @param p_value  The value to compute the percentage with.
 *
 *  @return         The percentage of p_value between p_start and p_end;
 */
template <typename ReturnType, typename Type>
FORCEINLINE ReturnType  Math::InverseLerp           (Type const         p_a,
                                                     Type const         p_b,
                                                     long double const  p_value)
{
    static_assert(std::is_arithmetic<ReturnType>::value, "ReturnType must be an arithmetic type !");
    static_assert(std::is_arithmetic<Type>::value, "Type must be an arithmetic type !");
    return  (ReturnType)((p_value - p_a) / (p_b - p_a));
}

/**
 * Compute the inverse square root of the value given
 *
 * @param p_value   Floating point/Integral value whose compute the inverse square root
 *
 * @return          The inverse square root of p_value.
 */
template <typename ReturnType, typename Type>
FORCEINLINE ReturnType  Math::InvSqrt               (Type const         p_value)
{
    static_assert(std::is_arithmetic<ReturnType>::value, "ReturnType must be an arithmetic type !");
    static_assert(std::is_arithmetic<Type>::value, "Type must be an arithmetic type !");
    return (ReturnType)(1.0 / sqrt((double)p_value));
}

/**
 * Compute the inverse square root of the value given
 *
 * @param p_value   Floating point value whose compute the inverse square root
 *
 * @return          The inverse square root of p_value.
 */
FORCEINLINE double      Math::InvSqrt               (double const       p_value)
{
    return 1.0/sqrt(p_value);
}

/**
 * Compute the inverse square root of the value given
 *
 * @param p_value   Floating point value whose compute the inverse square root
 *
 * @return          The inverse square root of p_value.
 */
FORCEINLINE float       Math::InvSqrt               (float const        p_value)
{
    return 1.0f / sqrtf(p_value);
}

/**
 * Compute the inverse square root of the value given
 *
 * @param p_value   Floating point value whose compute the inverse square root
 *
 * @return          The inverse square root of p_value.
 */
FORCEINLINE long double Math::InvSqrt               (long double const  p_value)
{
    return 1.0l / sqrtl(p_value);
}

/**
 * Check if the value given is a finite value.
 * A finite value is any floating-point value that is neither infinite nor NaN (Not-A-Number).
 *
 * @param p_value   Floating point value to check
 *
 * @return          True  if p_value is finite.
 * @return          false if p_value isn't finite.
 */
FORCEINLINE double      Math::IsFinite              (double const       p_value)
{
    return isfinite(p_value);
}

/**
 * Check if the value given is a finite value.
 * A finite value is any floating-point value that is neither infinite nor NaN (Not-A-Number).
 *
 * @param p_value   Floating point value to check
 *
 * @return          True  if p_value is finite.
 * @return          false if p_value isn't finite. 
 */
FORCEINLINE float       Math::IsFinite              (float const        p_value)
{
    return isfinite(p_value);
}

/**
 * Check if the value given is a finite value.
 * A finite value is any floating-point value that is neither infinite nor NaN (Not-A-Number).
 *
 * @param p_value   Floating point value to check
 *
 * @return          True  if p_value is finite.
 * @return          false if p_value isn't finite.
 */
FORCEINLINE long double Math::IsFinite              (long double const  p_value)
{
    return isfinite(p_value);
}

/**
 * Check if the value given is a infinite value.
 *
 * @param p_value   Floating point value to check
 *
 * @return          True  if p_value is infinite.
 * @return          false if p_value isn't infinite.
 */
FORCEINLINE double      Math::IsInfinite            (double const       p_value)
{
    return isinf(p_value);
}

/**
 * Check if the value given is a infinite value.
 *
 * @param p_value   Floating point value to check
 *
 * @return          True  if p_value is infinite.
 * @return          false if p_value isn't infinite.
 */
FORCEINLINE float       Math::IsInfinite            (float const        p_value)
{
    return isinf(p_value);
}

/**
 * Check if the value given is a infinite value.
 *
 * @param p_value   Floating point value to check
 *
 * @return          True  if p_value is infinite.
 * @return          false if p_value isn't infinite.
 */
FORCEINLINE long double Math::IsInfinite            (long double const  p_value)
{
    return isinf(p_value);
}

/**
 * Check if the value given is not a number (NaN)
 *
 * @param p_value   Floating point value to check
 *
 * @return          True  if p_value is not a number.
 * @return          false if p_value is a number or a infinite value.
 */
FORCEINLINE double      Math::IsNaN                 (double const       p_value)
{
    return isnan(p_value);
}

/**
 * Check if the value given is not a number (NaN)
 *
 * @param p_value   Floating point value to check
 *
 * @return          True  if p_value is not a number.
 * @return          false if p_value is a number or a infinite value.
 */
FORCEINLINE float       Math::IsNaN                 (float const        p_value)
{
    return isnan(p_value);
}

/**
 * Check if the value given is not a number (NaN)
 *
 * @param p_value   Floating point value to check
 *
 * @return          True  if p_value is not a number.
 * @return          false if p_value is a number or a infinite value.
 */
FORCEINLINE long double Math::IsNaN                 (long double const  p_value)
{
    return isnan(p_value);
}

/**
 *  Checks if two floating point numbers are nearly equal.
 *
 *  @param p_a              First number to compare
 *  @param p_b              Second number to compare
 *  @param p_errorTolerance Maximum allowed difference for considering them as 'nearly equal'
 *
 *  @return                 True  if A and B are nearly equal
 *  @return                 False if A and B aren't nearly equal
 */
FORCEINLINE bool        Math::IsNearlyEqual         (double const       p_a, 
                                                     double const       p_b, 
                                                     double const       p_errorTolerance)
{
    return Math::Abs(p_a-p_b) <= p_errorTolerance;
}

/**
 *  Checks if two floating point numbers are nearly equal.
 *
 *  @param p_a              First number to compare
 *  @param p_b              Second number to compare
 *  @param p_errorTolerance Maximum allowed difference for considering them as 'nearly equal'
 *
 *  @return                 True  if A and B are nearly equal
 *  @return                 False if A and B aren't nearly equal
 */
FORCEINLINE bool        Math::IsNearlyEqual         (float const        p_a,
                                                     float const        p_b, 
                                                     float const        p_errorTolerance)
{
    return Math::Abs(p_a-p_b) <= p_errorTolerance;
}

/**
 *  Checks if two floating point numbers are nearly equal.
 *
 *  @param p_a              First number to compare
 *  @param p_b              Second number to compare
 *  @param p_errorTolerance Maximum allowed difference for considering them as 'nearly equal'
 *
 *  @return                 True  if A and B are nearly equal
 *  @return                 False if A and B aren't nearly equal
 */
FORCEINLINE bool        Math::IsNearlyEqual         (long double const  p_a,
                                                     long double const  p_b, 
                                                     long double const  p_errorTolerance)
{
    return Math::Abs(p_a-p_b) <= p_errorTolerance;
}

/**
 *  Checks if a floating point number is nearly zero.
 *
 *  @param p_value          Number to compare
 *  @param p_errorTolerance Maximum allowed difference for considering Value as 'nearly zero'
 *
 *  @return                 True  if p_value is nearly zero
 *  @return                 False if p_value isn't nearly zero
 */
FORCEINLINE bool        Math::IsNearlyZero          (double const       p_value,
                                                     double const       p_errorTolerance)
{
    return Math::Abs(p_value) <= p_errorTolerance;
}

/**
 *  Checks if a floating point number is nearly zero.
 *
 *  @param p_value          Number to compare
 *  @param p_errorTolerance Maximum allowed difference for considering Value as 'nearly zero'
 *
 *  @return                 True  if p_value is nearly zero
 *  @return                 False if p_value isn't nearly zero
 */
FORCEINLINE bool        Math::IsNearlyZero          (float const        p_value,
                                                     float const        p_errorTolerance)
{
    return Math::Abs(p_value) <= p_errorTolerance;
}

/**
 *  Checks if a floating point number is nearly zero.
 *
 *  @param p_value          Number to compare
 *  @param p_errorTolerance Maximum allowed difference for considering Value as 'nearly zero'
 *
 *  @return                 True  if p_value is nearly zero
 *  @return                 False if p_value isn't nearly zero
 */
FORCEINLINE bool        Math::IsNearlyZero          (long double const  p_value,
                                                     long double const  p_errorTolerance)
{
    return Math::Abs(p_value) <= p_errorTolerance;
}

/**
 * Check if the value given is negative
 *
 * @param p_value   Floating point value to check
 *
 * @return          True    if p_value is negative
 * @return          False   if p_value is positive
 */
FORCEINLINE bool        Math::IsNegative            (double const       p_value)
{
    return (*(uint64*)&p_value) >= 0x8000000000000000;
                                                             
}                                               

/**
 * Check if the value given is negative
 *
 * @param p_value   Floating point value to check
 *
 * @return          True    if p_value is negative
 * @return          False   if p_value is positive
 */
FORCEINLINE bool        Math::IsNegative            (float const        p_value)
{
    return (*(uint32*)&p_value) >= 0x80000000;                                  
}                                       

/**
 * Check if the value given is negative
 *
 * @param p_value   Floating point value to check
 *
 * @return          True    if p_value is negative
 * @return          False   if p_value is positive
 */
FORCEINLINE bool        Math::IsNegative            (long double const  p_value)
{
    return (*(uint64*)&p_value) >= 0x8000000000000000;
}

/**
 *  Checks if the value given is outside the range [p_min,p_max]
 *
 *  @param p_value              Value to Check
 *  @param p_minInclusiveValue  Min range value (Inclusive)
 *  @param p_maxInclusiveValue  Max range value (Inclusive)
 *
 *  @return                     True  if (p_value <= p_minInclusiveValue || p_maxInclusiveValue <= p_value)
 *  @return                     False if (p_minInclusiveValue < p_value && p_value < p_maxInclusiveValue)
 */
template <typename Type>
FORCEINLINE bool        Math::IsOutsideIn           (Type const         p_value,
                                                     Type const         p_minInclusiveValue, 
                                                     Type const         p_maxInclusiveValue)
{
    static_assert(std::is_arithmetic<Type>::value, "Type must be an arithmetic type !");
    return (p_value <= p_minInclusiveValue || p_maxInclusiveValue <= p_value);
}

/**
 *  Checks if the value given is outside the range [p_min,p_max[
 *
 *  @param p_value              Value to Check
 *  @param p_minInclusiveValue  Min range value (Inclusive)
 *  @param p_maxExclusiveValue  Max range value (Exclusive)
 *
 *  @return                     True  if (p_value <= p_minInclusiveValue || p_maxExclusiveValue < p_value)
 *  @return                     False if (p_minInclusiveValue < p_value && p_value <= p_maxExclusiveValue)
 */
template <typename Type>
FORCEINLINE bool        Math::IsOutsideInEx         (Type const         p_value,
                                                     Type const         p_minInclusiveValue, 
                                                     Type const         p_maxExclusiveValue)
{
    static_assert(std::is_arithmetic<Type>::value, "Type must be an arithmetic type !");
    return (p_value <= p_minInclusiveValue || p_maxExclusiveValue < p_value);
}

/**
*   Checks if the value given is outside the range ]p_min,p_max[
*
*   @param p_value              Value to Check
*   @param p_minExclusiveValue  Min range value (Exclusive)
*   @param p_maxExclusiveValue  Max range value (Exclusive)
*
*   @return                     True  if (p_value < p_minExclusiveValue || p_maxExclusiveValue < p_value)
*   @return                     False if (p_minExclusiveValue < p_value && p_value <= p_maxExclusiveValue)
*/
template <typename Type>
FORCEINLINE bool        Math::IsOutsideEx           (Type const         p_value,
                                                     Type const         p_minExclusiveValue,
                                                     Type const         p_maxExclusiveValue)
{
    static_assert(std::is_arithmetic<Type>::value, "Type must be an arithmetic type !");
    return (p_value < p_minExclusiveValue || p_maxExclusiveValue < p_value);
}

/**
 *  Checks if the value given is outside the range ]p_min,p_max]
 *
 *  @param p_value              Value to Check
 *  @param p_minExclusiveValue  Min range value (Exclusive)
 *  @param p_maxInclusiveValue  Max range value (Inclusive)
 *
 *  @return                     True  if (p_value < p_minExclusiveValue || p_maxInclusiveValue < p_value)
 *  @return                     False if (p_minExclusiveValue < p_value && p_value <= p_maxInclusiveValue)
 */
template <typename Type>
FORCEINLINE bool        Math::IsOutsideExIn         (Type const         p_value,
                                                     Type const         p_minExclusiveValue, 
                                                     Type const         p_maxInclusiveValue)
{
    static_assert(std::is_arithmetic<Type>::value, "Type must be an arithmetic type !");
    return (p_value < p_minExclusiveValue || p_maxInclusiveValue <= p_value);
}

/**
 * Check if the value given is positive
 *
 * @param p_value   Floating point value to check
 *
 * @return          True    if p_value is positive
 * @return          False   if p_value is negative
 */
FORCEINLINE bool        Math::IsPositive            (double const       p_value)
{
    return (*(uint64*)&p_value) < 0x8000000000000000;
                                                             
}                                               

/**
 * Check if the value given is positive
 *
 * @param p_value   Floating point value to check
 *
 * @return          True    if p_value is positive
 * @return          False   if p_value is negative
 */
FORCEINLINE bool        Math::IsPositive            (float const        p_value)
{
    return (*(uint32*)&p_value) < 0x80000000;                                   
}                                       

/**
 * Check if the value given is positive
 *
 * @param p_value   Floating point value to check
 *
 * @return          True    if p_value is positive
 * @return          False   if p_value is negative
 */
FORCEINLINE bool        Math::IsPositive            (long double const  p_value)
{
    return (*(uint64*)&p_value) < 0x8000000000000000;
}

/**
 *  Checks whether a number is a power of two.
 *
 *  @param p_value  The value to be checked.
 *
 *  @return         True  if p_value is a power of two.
 *  @return         False if p_value is not a power of two.
 */
template <typename Type>
FORCEINLINE bool        Math::IsPowerOfTwo          (Type const         p_value)
{
    static_assert(std::is_arithmetic<Type>::value, "Type must be an arithmetic type !");
    return (p_value & (p_value-1) == 0);
}

/**
 *  Checks if the value given is within range [p_min,p_max]
 *
 *  @param p_value              Value to Check
 *  @param p_minInclusiveValue  Min range value (Inclusive)
 *  @param p_maxInclusiveValue  Max range value (Inclusive)

 *  @return                     True  if (p_minInclusiveValue <= p_value && p_value <= p_maxInclusiveValue)
 *  @return                     False if (p_value < p_minInclusiveValue || p_maxInclusiveValue < p_value)
 */
template <typename Type>
FORCEINLINE bool        Math::IsWithinIn            (Type const         p_value,
                                                     Type const         p_minInclusiveValue, 
                                                     Type const         p_maxInclusiveValue)
{
    static_assert(std::is_arithmetic<Type>::value, "Type must be an arithmetic type !");
    return (p_minInclusiveValue <= p_value) && (p_value <= p_maxInclusiveValue);
}

/**
 *  Checks if the value given is within range [p_min,p_max[
 *
 *  @param p_value              Value to Check
 *  @param p_minInclusiveValue  Min range value (Inclusive)
 *  @param p_maxExclusiveValue  Max range value (Exclusive)

 *  @return                     True  if (p_minInclusiveValue <= p_value && p_value < p_maxExclusiveValue)
 *  @return                     False if (p_value < p_minInclusiveValue || p_maxExclusiveValue <= p_value)
 */
template <typename Type>
FORCEINLINE bool        Math::IsWithinInEx          (Type const         p_value,
                                                     Type const         p_minInclusiveValue, 
                                                     Type const         p_maxExclusiveValue)
{
    static_assert(std::is_arithmetic<Type>::value, "Type must be an arithmetic type !");
    return (p_minInclusiveValue <= p_value) && (p_value < p_maxExclusiveValue);
}

/**
 *  Checks if the value given is within range ]p_min,p_max[
 *
 *  @param p_value              Value to Check
 *  @param p_minExclusiveValue  Min range value (Exclusive)
 *  @param p_maxExclusiveValue  Max range value (Exclusive)

 *  @return                     True  if (p_minExclusiveValue < p_value && p_value < p_maxExclusiveValue)
 *  @return                     False if (p_value <= p_minExclusiveValue || p_maxExclusiveValue <= p_value)
 */
template <typename Type>
FORCEINLINE bool        Math::IsWithinEx            (Type const         p_value,
                                                     Type const         p_minExclusiveValue,
                                                     Type const         p_maxExclusiveValue)
{
    static_assert(std::is_arithmetic<Type>::value, "Type must be an arithmetic type !");
    return (p_minExclusiveValue < p_value) && (p_value < p_maxExclusiveValue);
}

/**
 *  Checks if the value given is within range ]p_min,p_max]
 *
 *  @param p_value              Value to Check
 *  @param p_minExclusiveValue  Min range value (Exclusive)
 *  @param p_maxInclusiveValue  Max range value (Inclusive)

 *  @return                     True  if (p_minExclusiveValue < p_value && p_value <= p_maxInclusiveValue)
 *  @return                     False if (p_value <= p_minExclusiveValue || p_maxInclusiveValue < p_value)
 */
template <typename Type>
FORCEINLINE bool        Math::IsWithinExIn          (Type const         p_value,
                                                     Type const         p_minExclusiveValue, 
                                                     Type const         p_maxInclusiveValue)
{
    static_assert(std::is_arithmetic<Type>::value, "Type must be an arithmetic type !");
    return (p_minExclusiveValue < p_value) && (p_value <= p_maxInclusiveValue);
}

/**
 *  Computes a linerar interpolation between two value
 *
 *  @param p_a      The start value of the interpolation
 *  @param p_b      The end value of the interpolation
 *  @param p_alpha  The interpolation value
 *
 *  @return         The interpolation of p_a and p_b by p_alpha
 */
template <typename ReturnType, typename Type>
FORCEINLINE ReturnType  Math::Lerp                  (Type const         p_a,
                                                     Type const         p_b,
                                                     double const       p_alpha)
{
    static_assert(std::is_arithmetic<ReturnType>::value, "ReturnType must be an arithmetic type !");
    static_assert(std::is_arithmetic<Type>::value, "Type must be an arithmetic type !");
    return (ReturnType)(p_a + p_alpha * (p_b - p_a));
}

/** 
 *  Computes a linerar interpolation between two value
 *
 *  @param p_a      The start value of the interpolation
 *  @param p_b      The end value of the interpolation
 *  @param p_alpha  The interpolation value
 *
 *  @return         The interpolation of p_a and p_b by p_alpha
 */
template <typename ReturnType, typename Type>
FORCEINLINE ReturnType  Math::Lerp                  (Type const         p_a,
                                                     Type const         p_b,
                                                     float const        p_alpha)
{
    static_assert(std::is_arithmetic<ReturnType>::value, "ReturnType must be an arithmetic type !");
    static_assert(std::is_arithmetic<Type>::value, "Type must be an arithmetic type !");
    return (ReturnType)(p_a + p_alpha * (p_b - p_a));
}

/**
 *  Computes a linerar interpolation between two value
 *
 *  @param p_a      The start value of the interpolation
 *  @param p_b      The end value of the interpolation
 *  @param p_alpha  The interpolation value
 *
 *  @return         The interpolation of p_a and p_b by p_alpha
 */
template <typename ReturnType, typename Type>
FORCEINLINE ReturnType  Math::Lerp                  (Type const         p_a,
                                                     Type const         p_b,
                                                     long double const  p_alpha)
{
    static_assert(std::is_arithmetic<ReturnType>::value, "ReturnType must be an arithmetic type !");
    static_assert(std::is_arithmetic<Type>::value, "Type must be an arithmetic type !");
    return (ReturnType)(p_a + p_alpha * (p_b - p_a));
}
/**
 *  Computes a linerar interpolation between two angle in degree.
 *  The resulting angle is wrapped around 360 degrees.
 *
 *  @param p_a      The start angle of the interpolation
 *  @param p_b      The end angle of the interpolation
 *  @param p_alpha  The interpolation value
 *
 *  @return         The interpolation of p_a and p_b by p_alpha
 */
template <typename ReturnType, typename Type>
FORCEINLINE ReturnType  Math::LerpAngleDeg          (Type const         p_a,
                                                     Type const         p_b,
                                                     double const       p_alpha)
{
    static_assert(std::is_arithmetic<ReturnType>::value, "ReturnType must be an arithmetic type !");
    static_assert(std::is_arithmetic<Type>::value, "Type must be an arithmetic type !");
    
    float delta   {Repeat((double)(p_b-p_a), 360.0)};
    
    if (delta > 180.0)
        delta -= 360.0;

    return (ReturnType)(p_a + delta * p_alpha);
}

/**
 *  Computes a linerar interpolation between two angle in degree.
 *  The resulting angle is wrapped around 360 degrees.
 *
 *  @param p_a      The start angle of the interpolation
 *  @param p_b      The end angle of the interpolation
 *  @param p_alpha  The interpolation value
 *
 *  @return         The interpolation of p_a and p_b by p_alpha
 */
template <typename ReturnType, typename Type>
FORCEINLINE ReturnType  Math::LerpAngleDeg          (Type const         p_a,
                                                     Type const         p_b,
                                                     float const        p_alpha)
{
    static_assert(std::is_arithmetic<ReturnType>::value, "ReturnType must be an arithmetic type !");
    static_assert(std::is_arithmetic<Type>::value, "Type must be an arithmetic type !");
    
    float delta   {Repeat((float)(p_b-p_a), 360.0f)};
    
    if (delta > 180.0f)
        delta -= 360.0f;

    return (ReturnType)(p_a + delta * p_alpha);
}

/**
 *  Computes a linerar interpolation between two angle in degree.
 *  The resulting angle is wrapped around 360 degrees.
 *
 *  @param p_a      The start angle of the interpolation
 *  @param p_b      The end angle of the interpolation
 *  @param p_alpha  The interpolation value
 *
 *  @return         The interpolation of p_a and p_b by p_alpha
 */
template <typename ReturnType, typename Type>
FORCEINLINE ReturnType  Math::LerpAngleDeg          (Type const         p_a,
                                                     Type const         p_b,
                                                     long double const  p_alpha)
{
    static_assert(std::is_arithmetic<ReturnType>::value, "ReturnType must be an arithmetic type !");
    static_assert(std::is_arithmetic<Type>::value, "Type must be an arithmetic type !");
    
    float delta   {Repeat((long double)(p_b-p_a), 360.0l)};
    
    if (delta > 180.0l)
        delta -= 360.0l;

    return (ReturnType)(p_a + delta * p_alpha);
}

/**
 *  Computes a linerar interpolation between two angle in radian.
 *  The resulting angle is wrapped around 2PI degrees.
 *
 *  @param p_a      The start angle of the interpolation
 *  @param p_b      The end angle of the interpolation
 *  @param p_alpha  The interpolation value
 *
 *  @return         The interpolation of p_a and p_b by p_alpha
 */
template <typename ReturnType, typename Type>
FORCEINLINE ReturnType  Math::LerpAngleRad          (Type const         p_a,
                                                     Type const         p_b,
                                                     double const       p_alpha)
{
    static_assert(std::is_arithmetic<ReturnType>::value, "ReturnType must be an arithmetic type !");
    static_assert(std::is_arithmetic<Type>::value, "Type must be an arithmetic type !");
    
    float delta   {Repeat((double)(p_b-p_a), MATH_TAU)};
    
    if (delta > MATH_PI)
        delta -= MATH_TAU;

    return (ReturnType)(p_a + delta * p_alpha);
}

/**
 *  Computes a linerar interpolation between two angle in radian.
 *  The resulting angle is wrapped around 2PI degrees.
 *
 *  @param p_a      The start angle of the interpolation
 *  @param p_b      The end angle of the interpolation
 *  @param p_alpha  The interpolation value
 *
 *  @return         The interpolation of p_a and p_b by p_alpha
 */
template <typename ReturnType, typename Type>
FORCEINLINE ReturnType  Math::LerpAngleRad          (Type const         p_a,
                                                     Type const         p_b,
                                                     float const        p_alpha)
{
    static_assert(std::is_arithmetic<ReturnType>::value, "ReturnType must be an arithmetic type !");
    static_assert(std::is_arithmetic<Type>::value, "Type must be an arithmetic type !");
    
    float delta   {Repeat((double)(p_b-p_a), MATH_TAU_F)};
    
    if (delta > MATH_PI_F)
        delta -= MATH_TAU_F;

    return (ReturnType)(p_a + delta * p_alpha);
}

/**
 *  Computes a linerar interpolation between two angle in radian.
 *  The resulting angle is wrapped around 2PI degrees.
 *
 *  @param p_a      The start angle of the interpolation
 *  @param p_b      The end angle of the interpolation
 *  @param p_alpha  The interpolation value
 *
 *  @return         The interpolation of p_a and p_b by p_alpha
 */
template <typename ReturnType, typename Type>
FORCEINLINE ReturnType  Math::LerpAngleRad          (Type const         p_a,
                                                     Type const         p_b,
                                                     long double const  p_alpha)
{
    static_assert(std::is_arithmetic<ReturnType>::value, "ReturnType must be an arithmetic type !");
    static_assert(std::is_arithmetic<Type>::value, "Type must be an arithmetic type !");
    
    float delta   {Repeat((double)(p_b-p_a), MATH_TAU_L)};
    
    if (delta > MATH_PI_L)
        delta -= MATH_TAU_L;

    return (ReturnType)(p_a + delta * p_alpha);
}

/**
 * Get the base 2 logarithm of the value given
 *
 * @param p_value   Floating point value
 *
 * @return          Log2(p_value)
 */
template <typename ReturnType, typename Type>
FORCEINLINE ReturnType  Math::Log2                  (Type const         p_value)
{
    static_assert(std::is_arithmetic<ReturnType>::value, "ReturnType must be an arithmetic type !");
    static_assert(std::is_arithmetic<Type>::value, "Type must be an arithmetic type !");
    return (ReturnType)log2((double)p_value);
}

/**
 * Get the base 2 logarithm of the value given
 *
 * @param p_value   Floating point value
 *
 * @return          Log2(p_value)
 */
FORCEINLINE double      Math::Log2                  (double const       p_value)
{
    return log2(p_value);
}

/**
 * Get the base 2 logarithm of the value given
 *
 * @param p_value   Floating point value
 *
 * @return          Log2(p_value)
 */
FORCEINLINE float       Math::Log2                  (float const        p_value)
{
    return log2f(p_value);
}

/**
 * Get the base 2 logarithm of the value given
 *
 * @param p_value   Floating point value
 *
 * @return          Log2(p_value)
 */
FORCEINLINE long double Math::Log2                  (long double const  p_value)
{
    return log2l(p_value);
}

/**
 * Get the base 10 logarithm of the value given
 *
 * @param p_value   Floating point value
 *
 * @return          Log10(p_value)
 */
template <typename ReturnType, typename Type>
FORCEINLINE ReturnType  Math::Log10                 (Type const         p_value)
{
    static_assert(std::is_arithmetic<ReturnType>::value, "ReturnType must be an arithmetic type !");
    static_assert(std::is_arithmetic<Type>::value, "Type must be an arithmetic type !");
    return (ReturnType)log10((double)p_value);
}

/**
 * Get the base 10 logarithm of the value given
 *
 * @param p_value   Floating point value
 *
 * @return          Log10(p_value)
 */
FORCEINLINE double      Math::Log10                 (double const       p_value)
{
    return log10(p_value);
}

/**
 * Get the base 10 logarithm of the value given
 *
 * @param p_value   Floating point value
 *
 * @return          Log10(p_value)
 */
FORCEINLINE float       Math::Log10                 (float const        p_value)
{
    return log10f(p_value);
}

/**
 * Get the base 10 logarithm of the value given
 *
 * @param p_value   Floating point value
 *
 * @return          Log10(p_value)
 */
FORCEINLINE long double Math::Log10                 (long double const  p_value)
{
    return log10l(p_value);
}

/**
 * Get the natural logarithm of the value given
 *
 * @param p_value   Floating point value
 *
 * @return          Log(p_value) = logExp(p_value).
 */
template <typename ReturnType, typename Type>
FORCEINLINE ReturnType  Math::LogE                  (Type const         p_value)
{
    static_assert(std::is_arithmetic<ReturnType>::value, "ReturnType must be an arithmetic type !");
    static_assert(std::is_arithmetic<Type>::value, "Type must be an arithmetic type !");
    return (ReturnType)log((double)p_value);
}

/**
 * Get the natural logarithm of the value given
 *
 * @param p_value   Floating point value
 *
 * @return          Log(p_value) = logExp(p_value).
 */
FORCEINLINE double      Math::LogE                  (double const       p_value)
{
    return log(p_value);
}

/**
 * Get the natural logarithm of the value given
 *
 * @param p_value   Floating point value
 *
 * @return          Log(p_value) = logExp(p_value).
 */
FORCEINLINE float       Math::LogE                  (float const        p_value)
{
    return logf(p_value);
}

/**
 * Get the natural logarithm of the value given
 *
 * @param p_value   Floating point value
 *
 * @return          Log(p_value) = logExp(p_value).
 */
FORCEINLINE long double Math::LogE                  (long double const  p_value)
{
    return logl(p_value);
}

/**
 * Get the logarithm with a base and value given
 *
 * @param p_value   Floating point value
 * @param p_base    Floating point base of the logarithm
 *
 * @return          LogBase(p_value).
 */
FORCEINLINE double      Math::LogX                  (double const       p_value,
                                                     double const       p_base)
{
    return log(p_value)/ log(p_base);
}

/**
 * Get the logarithm with a base and value given
 *
 * @param p_value   Floating point value
 * @param p_base    Floating point base of the logarithm
 *
 * @return          LogBase(p_value).
 */
FORCEINLINE float       Math::LogX                  (float const        p_value,
                                                     float const        p_base)
{
    return logf(p_value)/ logf(p_base);
}

/**
 * Get the logarithm with a base and value given
 *
 * @param p_value   Floating point value
 * @param p_base    Floating point base of the logarithm
 *
 * @return          LogBase(p_value).
 */
FORCEINLINE long double Math::LogX                  (long double const  p_value,
                                                     long double const  p_base)
{
    return logl(p_value)/ logl(p_base);
}

/**
 * Give the higher value between two value given.
 *
 * @param p_a       First value to compare
 * @param p_b       Second value to compare
 *
 * @return          Return p_a if (p_a >= p_b)
 * @return          Return p_b if (p_a < p_b)
 */
template <typename Type>
CONSTEXPR   Type        Math::Max                   (Type const         p_a,
                                                     Type const         p_b)
{
    static_assert(std::is_arithmetic<Type>::value, "Type must be an arithmetic type !");
    return (p_a < p_b) ? p_b : p_a;
}

/**
 * Gives the highest value between three given values
 *
 * @param p_a       First value to compare
 * @param p_b       Second value to compare
 * @param p_c       Thrid value to compare
 *
 * @return          Return p_a if (p_a > p_b && p_a > p_c)
 * @return          Return p_b if (p_b > p_a && p_b > p_c)
 * @return          Return p_c if (p_c > p_a && p_c > p_b)
 */
template <typename Type>
CONSTEXPR   Type        Math::Max3                  (Type const         p_a,
                                                     Type const         p_b,
                                                     Type const         p_c)
{
    static_assert(std::is_arithmetic<Type>::value, "Type must be an arithmetic type !");

    Type const  max2 {(p_a < p_b) ? p_b : p_a};
    return (p_c < max2) ? max2 : p_c;
}

/**
 * Gives the lower value between two given values
 *
 * @param p_a       First value to compare
 * @param p_b       Second value to compare
 *
 * @return          Return p_a if (p_a <= p_b)
 * @return          Return p_b if (p_b < p_a)
 */
template <typename Type>
CONSTEXPR   Type        Math::Min                   (Type const         p_a,
                                                     Type const         p_b)
{
    return (p_a > p_b) ? p_b : p_a;
}

/**
 * Gives the lowest value between three given values.
 *
 * @param p_a       First value to compare
 * @param p_b       Second value to compare
 * @param p_c       Thrid value to compare
 *
 * @return          Return p_a if (p_a < p_b && p_a < p_c)
 * @return          Return p_b if (p_b < p_a && p_b < p_c)
 * @return          Return p_c if (p_c < p_a && p_c < p_b)
 */
template <typename Type>
CONSTEXPR   Type        Math::Min3                  (Type const         p_a,
                                                     Type const         p_b,
                                                     Type const         p_c)
{
    static_assert(std::is_arithmetic<Type>::value, "Type must be an arithmetic type !");

    Type const  min2 {(p_a > p_b) ? p_b : p_a};
    return (p_c > min2) ? min2 : p_c;
}

/**
 * Breaks the given value into an integral and a fractional part.
 *
 * @param p_value   Floating point value to convert
 * @param p_intPart Floating point value that receives the integral part of the number.
 *
 * @return          The fractional part of the number.
 */
FORCEINLINE double      Math::Modf                  (double const       p_value,
                                                     double*            p_intPart)
{
    return modf(p_value,p_intPart);
}

/**
 * Breaks the given value into an integral and a fractional part.
 *
 * @param p_value   Floating point value to convert
 * @param p_intPart Floating point value that receives the integral part of the number.
 *
 * @return          The fractional part of the number.
 */
FORCEINLINE float       Math::Modf                  (float const        p_value,
                                                     float*             p_intPart)
{
    return modff(p_value,p_intPart);
}

/**
 * Breaks the given value into an integral and a fractional part.
 *
 * @param p_value   Floating point value to convert
 * @param p_intPart Floating point value that receives the integral part of the number.
 *
 * @return          The fractional part of the number.
 */
FORCEINLINE long double Math::Modf                  (long double const  p_value,
                                                     long double*       p_intPart)
{
    return modfl(p_value,p_intPart);
}

/**
 * Moves the current value given towards target.
 * This is essentially the same as Math::Lerp but instead the function will ensure that the speed never exceeds maxDelta.
 * Negative values of maxDelta pushes the value away from target
 *
 * @param   p_current   The current value
 * @param   p_target    The value to move towards
 * @param   p_maxDelta  The maximum change that should be applied to the value.
 *
 * @return              The value returned is p_current with the p_maxDelta change applied
 */
FORCEINLINE double      Math::MoveTowards           (double const       p_current,
                                                     double const       p_target,
                                                     double const       p_maxDelta)
{
    if (Math::Abs(p_target - p_current) <= p_maxDelta)
        return p_target;

    return p_current + Math::FloatSelect(p_target - p_current, 1.0, -1.0) * p_maxDelta;
}

/**
 * Moves the current value given towards target.
 * This is essentially the same as Math::Lerp but instead the function will ensure that the speed never exceeds maxDelta.
 * Negative values of maxDelta pushes the value away from target
 *
 * @param   p_current   The current value
 * @param   p_target    The value to move towards
 * @param   p_maxDelta  The maximum change that should be applied to the value.
 *
 * @return              The value returned is p_current with the p_maxDelta change applied
 */
FORCEINLINE float       Math::MoveTowards           (float const        p_current,
                                                     float const        p_target,
                                                     float const        p_maxDelta)
{
    if (Math::Abs(p_target - p_current) <= p_maxDelta)
        return p_target;

    return p_current + Math::FloatSelect(p_target - p_current, 1.0f, -1.0f) * p_maxDelta;
}

/**
 * Moves the current value given towards target.
 * This is essentially the same as Math::Lerp but instead the function will ensure that the speed never exceeds maxDelta.
 * Negative values of maxDelta pushes the value away from target
 *
 * @param   p_current   The current value
 * @param   p_target    The value to move towards
 * @param   p_maxDelta  The maximum change that should be applied to the value.
 *
 * @return              The value returned is p_current with the p_maxDelta change applied
 */
FORCEINLINE long double Math::MoveTowards           (long double const  p_current,
                                                     long double const  p_target,
                                                     long double const  p_maxDelta)
{
    if (Math::Abs(p_target-p_current) <= p_maxDelta)
        return p_target;
    
    return p_current + Math::FloatSelect(p_target-p_current, 1.0l, -1.0l) * p_maxDelta;
}

/**
 * Same as MoveTowards but makes sure the values interpolate correctly when they wrap around 360 degrees
 * For optimization reasons, negative values of maxDelta are not supported and may cause oscillation. 
 * To push p_current away from a target angle, add 180 to that angle instead.
 *
 * @param   p_current   The current value in degree (in degree)
 * @param   p_target    The value to move towards (in degree)
 * @param   p_maxDelta  The maximum change that should be applied to the value (in degree).
 *
 * @return              The value returned is p_current with the p_maxDelta change applied.
 */
FORCEINLINE double      Math::MoveTowardsAngleDeg   (double const       p_current,
                                                     double const       p_target,
                                                     double const       p_maxDelta)
{
    double const deltaAngle  {DeltaAngleDeg(p_current, p_target)};
   
    if (IsWithinEx(deltaAngle, -p_maxDelta, p_maxDelta))
        return p_target;
   
    return MoveTowards(p_current, p_current + deltaAngle, p_maxDelta);
}

/**
 * Same as MoveTowards but makes sure the values interpolate correctly when they wrap around 360 degrees
 * For optimization reasons, negative values of maxDelta are not supported and may cause oscillation.
 * To push p_current away from a target angle, add 180 to that angle instead.
 *
 * @param   p_current   The current value in degree (in degree)
 * @param   p_target    The value to move towards (in degree)
 * @param   p_maxDelta  The maximum change that should be applied to the value (in degree).
 *
 * @return              The value returned is p_current with the p_maxDelta change applied.
 */
FORCEINLINE float       Math::MoveTowardsAngleDeg   (float const        p_current,
                                                     float const        p_target,
                                                     float const        p_maxDelta)
{
    float const deltaAngle  {DeltaAngleDeg(p_current, p_target)};
   
    if (IsWithinEx(deltaAngle, -p_maxDelta, p_maxDelta))
        return p_target;
   
    return MoveTowards(p_current, p_current + deltaAngle, p_maxDelta);
}

/**
 * Same as MoveTowards but makes sure the values interpolate correctly when they wrap around 360 degrees
 * For optimization reasons, negative values of maxDelta are not supported and may cause oscillation.
 * To push p_current away from a target angle, add 180 to that angle instead.
 *
 * @param   p_current   The current value in degree (in degree)
 * @param   p_target    The value to move towards (in degree)
 * @param   p_maxDelta  The maximum change that should be applied to the value (in degree).
 *
 * @return              The value returned is p_current with the p_maxDelta change applied.
 */
FORCEINLINE long double Math::MoveTowardsAngleDeg   (long double const  p_current,
                                                     long double const  p_target,
                                                     long double const  p_maxDelta)
{
    long double const deltaAngle  {DeltaAngleDeg(p_current, p_target)};
   
    if (IsWithinEx(deltaAngle, -p_maxDelta, p_maxDelta))
        return p_target;
   
    return MoveTowards(p_current, p_current + deltaAngle, p_maxDelta);
}

/**
 * Same as MoveTowards but makes sure the values interpolate correctly when they wrap around 2PI degrees
 * For optimization reasons, negative values of maxDelta are not supported and may cause oscillation.
 * To push p_current away from a target angle, add PI to that angle instead.
 *
 * @param   p_current   The current value in degree (in radian)
 * @param   p_target    The value to move towards (in radian)
 * @param   p_maxDelta  The maximum change that should be applied to the value (in radian).
 *
 * @return              The value returned is p_current with the p_maxDelta change applied.
 */
FORCEINLINE double      Math::MoveTowardsAngleRad   (double const       p_current,
                                                     double const       p_target,
                                                     double const       p_maxDelta)
{
    double const deltaAngle  {DeltaAngleRad(p_current, p_target)};
   
    if (IsWithinEx(deltaAngle, -p_maxDelta, p_maxDelta))
        return p_target;
   
    return MoveTowards(p_current, p_current + deltaAngle, p_maxDelta);
}

/**
 * Same as MoveTowards but makes sure the values interpolate correctly when they wrap around 2PI degrees
 * For optimization reasons, negative values of maxDelta are not supported and may cause oscillation.
 * To push p_current away from a target angle, add PI to that angle instead.
 *
 * @param   p_current   The current value in degree (in radian)
 * @param   p_target    The value to move towards (in radian)
 * @param   p_maxDelta  The maximum change that should be applied to the value (in radian).
 *
 * @return              The value returned is p_current with the p_maxDelta change applied.
 */
FORCEINLINE float       Math::MoveTowardsAngleRad   (float const        p_current,
                                                     float const        p_target,
                                                     float const        p_maxDelta)
{
    float const deltaAngle  {DeltaAngleRad(p_current, p_target)};
   
    if (IsWithinEx(deltaAngle, -p_maxDelta, p_maxDelta))
        return p_target;
   
    return MoveTowards(p_current, p_current + deltaAngle, p_maxDelta);
}

/**
 * Same as MoveTowards but makes sure the values interpolate correctly when they wrap around 2PI degrees
 * For optimization reasons, negative values of maxDelta are not supported and may cause oscillation.
 * To push p_current away from a target angle, add PI to that angle instead.
 *
 * @param   p_current   The current value in degree (in radian)
 * @param   p_target    The value to move towards (in radian)
 * @param   p_maxDelta  The maximum change that should be applied to the value (in radian).
 *
 * @return              The value returned is p_current with the p_maxDelta change applied.
 */
FORCEINLINE long double Math::MoveTowardsAngleRad   (long double const  p_current,
                                                     long double const  p_target,
                                                     long double const  p_maxDelta)
{
    long double const deltaAngle  {DeltaAngleRad(p_current, p_target)};
   
    if (IsWithinEx(deltaAngle, -p_maxDelta, p_maxDelta))
        return p_target;
   
    return MoveTowards(p_current, p_current + deltaAngle, p_maxDelta);
}

/**
 * Compute the next power of two such as it's superior of the value given;
 *
 * @param p_value   Floating point value to convert
 *
 * @return          Next power of two
 */
template <typename Type>
            Type        Math::NextPowerOfTwo        (Type const         p_value)
{
    static_assert(std::is_integral<Type>::value, "Type must be an integral type !");
    Type powerOfTwo {1};
    while (powerOfTwo < p_value)
        powerOfTwo = powerOfTwo << 1;
    return powerOfTwo;
}

/**
 * Compute the Nth root:  NRoot(value) = Pow(value, 1/nthRoot)
 *
 * @param p_value   Floating point value to compute with
 * @param p_nthRoot Floating point value corresponding to the nth root.
 *
 * @return          The NthRoot of p_value: Pow(p_value, 1/p_nthRoot)
 */
template <typename ReturnType, typename Type1, typename Type2>
FORCEINLINE ReturnType  Math::NthRoot               (Type1 const        p_value,
                                                     Type2 const        p_nthRoot)
{
    static_assert(std::is_arithmetic<ReturnType>::value, "ReturnType must be an arithmetic type !");
    static_assert(std::is_arithmetic<Type1>::value, "'Type1' must be an arithmetic type !");
    static_assert(std::is_arithmetic<Type2>::value, "'Type2' must be an arithmetic type !");
    return (ReturnType)Math::Pow((double)p_value, 1/(double)p_nthRoot);
}

/**
 * Compute the Nth root:  NRoot(value) = Pow(value, 1/nthRoot)
 *
 * @param p_value   Floating point value to compute with
 * @param p_nthRoot Floating point value corresponding to the nth root.
 *
 * @return          The NthRoot of p_value: Pow(p_value, 1/p_nthRoot)
 */
FORCEINLINE double      Math::NthRoot               (double const       p_value,
                                                     double const       p_nthRoot)
{
    return Math::Pow(p_value, 1/p_nthRoot);
}

/**
 * Compute the Nth root:  NRoot(value) = Pow(value, 1/nthRoot)
 *
 * @param p_value   Floating point value to compute with
 * @param p_nthRoot Floating point value corresponding to the nth root.
 *
 * @return          The NthRoot of p_value: Pow(p_value, 1/p_nthRoot)
 */
FORCEINLINE float       Math::NthRoot               (float const        p_value,
                                                     float const        p_nthRoot)
{
    return Math::Pow(p_value, 1/p_nthRoot);
}

/**
 * Compute the Nth root:  NRoot(value) = Pow(value, 1/nthRoot)
 *
 * @param p_value   Floating point value to compute with
 * @param p_nthRoot Floating point value corresponding to the nth root.
 *
 * @return          The NthRoot of p_value: Pow(p_value, 1/p_nthRoot)
 */
FORCEINLINE long double Math::NthRoot               (long double const  p_value,
                                                     long double const  p_nthRoot)
{
    return Math::Pow(p_value, 1/p_nthRoot);
}

/**
 * PingPongs e p_value, so that it is never larger than p_length and never smaller than 0.0.
 * The returned value will move back and forth between 0 and p_length.
 */
FORCEINLINE double      Math::PingPong              (double const       p_value,
                                                     double const       p_length)
{
    return p_length - Abs(Repeat(p_value, 2.0*p_length)- p_length);
}

/**
 * PingPongs e p_value, so that it is never larger than p_length and never smaller than 0.0.
 * The returned value will move back and forth between 0 and p_length.
 */
FORCEINLINE float       Math::PingPong              (float const        p_value,
                                                     float const        p_length)
{
    return p_length - Abs(Repeat(p_value, 2.0f*p_length)- p_length);
}

/**
 * PingPongs e p_value, so that it is never larger than p_length and never smaller than 0.0.
 * The returned value will move back and forth between 0 and p_length.
 */
FORCEINLINE long double Math::PingPong              (long double const  p_value,
                                                     long double const  p_length)
{
    return p_length - Abs(Repeat(p_value, 2.0l*p_length)- p_length);
}

/**
 * Coupute the base given raised to the power given
 *
 * @param p_base        Floating point base value
 * @param p_exponent    Floating point exponent value.
 *
 * @return              The result of p_base^(p_exponent).
 */
template <typename ReturnType, typename Type1, typename Type2>
FORCEINLINE ReturnType  Math::Pow                   (Type1 const        p_base,
                                                     Type2 const        p_exponent)
{
    static_assert(std::is_arithmetic<ReturnType>::value, "ReturnType must be an arithmetic type !");
    static_assert(std::is_arithmetic<Type1>::value, "'Type1' must be an arithmetic type !");
    static_assert(std::is_arithmetic<Type2>::value, "'Type2' must be an arithmetic type !");
    return (ReturnType)pow((double)p_base,(double)p_exponent);
}

/**
 * Coupute the base given raised to the power given
 *
 * @param p_base        Floating point base value
 * @param p_exponent    Floating point exponent value.
 *
 * @return              The result of p_base^(p_exponent).
 */
FORCEINLINE double      Math::Pow                   (double const       p_base,
                                                     double const       p_exponent)
{
    return pow(p_base,p_exponent);
}

/**
 * Coupute the base given raised to the power given
 *
 * @param p_base        Floating point base value
 * @param p_exponent    Floating point exponent value.
 *
 * @return              The result of p_base^(p_exponent).
 */
FORCEINLINE float       Math::Pow                   (float const        p_base,
                                                     float const        p_exponent)
{
    return powf(p_base, p_exponent);
}

/**
 * Coupute the base given raised to the power given
 *
 * @param p_base        Floating point base value
 * @param p_exponent    Floating point exponent value.
 *
 * @return              The result of p_base^(p_exponent).
 */
FORCEINLINE long double Math::Pow                   (long double const  p_base,
                                                     long double const  p_exponent)
{
    return powl(p_base, p_exponent);
}

/**
 * Convert the given angle in radian to angle in degrees
 *
 * @param p_radValue    Floating point angle in radian to convert
 *
 * @return              The angle converted in degrees.
 */
template <typename ReturnType, typename Type>
CONSTEXPR   ReturnType  Math::RadToDeg              (Type const         p_radAngle)
{
    static_assert(std::is_arithmetic<ReturnType>::value, "ReturnType must be an arithmetic type !");
    static_assert(std::is_arithmetic<Type>::value, "Type must be an arithmetic type !");
    return (ReturnType)(p_radAngle * MATH_RAD_2_DEG);
}

/**
 * Convert the given angle in radian to angle in degrees
 *
 * @param p_radValue    Floating point angle in radian to convert
 *
 * @return              The angle converted in degrees.
 */
CONSTEXPR   double      Math::RadToDeg              (double const       p_radAngle)
{
    return p_radAngle * MATH_RAD_2_DEG;
}

/**
 * Convert the given angle in radian to angle in degrees
 *
 * @param p_radValue    Floating point angle in radian to convert
 *
 * @return              The angle converted in degrees.
 */
CONSTEXPR   float       Math::RadToDeg              (float const        p_radAngle)
{
    return p_radAngle * MATH_RAD_2_DEG_F;
}

/**
 * Convert the given angle in radian to angle in degrees
 *
 * @param p_radValue    Floating point angle in radian to convert
 *
 * @return              The angle converted in degrees.
 */
CONSTEXPR   long double Math::RadToDeg              (long double const  p_radAngle)
{
    return p_radAngle * MATH_RAD_2_DEG_L;
}

/**
 * Loops the value given, so that it is never larger than length given and never smaller than 0.
 * This is similar to the modulo operator but it works with floating point numbers. 
 * For example, using 3.0 for t and 2.5 for length, the result would be 0.5. With t = 5 and length = 2.5, the result would be 0.0. 
 * Note, however, that the behaviour is not defined for negative numbers as it is for the modulo operator.
 *
 * @param p_value       The value to loop.
 * @param p_length      The length of the loop
 *
 * @return              The looped value.
 */
FORCEINLINE double      Math::Repeat                (double const       p_value,
                                                     double const       p_length)
{
    return Clamp(p_value - Math::Floor(p_value/p_length) * p_length, 0.0, p_length);
}

/**
 * Loops the value given, so that it is never larger than length given and never smaller than 0.
 * This is similar to the modulo operator but it works with floating point numbers.
 * For example, using 3.0 for t and 2.5 for length, the result would be 0.5. With t = 5 and length = 2.5, the result would be 0.0.
 * Note, however, that the behaviour is not defined for negative numbers as it is for the modulo operator.
 *
 * @param p_value       The value to loop.
 * @param p_length      The length of the loop
 *
 * @return              The looped value.
 */
FORCEINLINE float       Math::Repeat                (float const        p_value,
                                                     float const        p_length)
{
    return Clamp(p_value - Math::Floor(p_value/p_length) * p_length, 0.0f, p_length);
}

/**
 * Loops the value given, so that it is never larger than length given and never smaller than 0.
 * This is similar to the modulo operator but it works with floating point numbers.
 * For example, using 3.0 for t and 2.5 for length, the result would be 0.5. With t = 5 and length = 2.5, the result would be 0.0.
 * Note, however, that the behaviour is not defined for negative numbers as it is for the modulo operator.
 *
 * @param p_value       The value to loop.
 * @param p_length      The length of the loop
 *
 * @return              The looped value.
 */
FORCEINLINE long double Math::Repeat                (long double const  p_value,
                                                     long double const  p_length)
{
    return Clamp(p_value - Math::Floor(p_value/p_length) * p_length, 0.0l, p_length);
}

/**
 * Converts a value to the nearest integer. Rounds up when the fraction is .5
 *
 * @param p_value   Floating point/Integral value to convert
 *
 * @return          The nearest integer to 'p_value'.
 */
template <typename ReturnType, typename Type>
FORCEINLINE ReturnType  Math::Round                 (Type const         p_value)
{ 
    static_assert(std::is_arithmetic<ReturnType>::value, "ReturnType must be an arithmetic type !");
    static_assert(std::is_arithmetic<Type>::value, "Type must be an arithmetic type !");
    return (ReturnType)round((double)p_value);
}

/**
 * Converts a double to the nearest integer. Rounds up when the fraction is .5
 *
 * @param p_value   Floating point value to convert
 *
 * @return          The nearest integer to 'p_value'.
 */
FORCEINLINE double      Math::Round                 (double const       p_value)
{ 
    return round(p_value);
}

/**
 * Converts a float to the nearest integer. Rounds up when the fraction is .5
 *
 * @param p_value   Floating point value to convert
 *
 * @return          The nearest integer to 'p_value'.
 */
FORCEINLINE float       Math::Round                 (float const        p_value)
{ 
    return roundf(p_value);
}

/**
 * Converts a long double to the nearest integer. Rounds up when the fraction is .5
 *
 * @param p_value   Floating point value to convert
 *
 * @return          The nearest integer to 'p_value'.
 */
FORCEINLINE long double Math::Round                 (long double const  p_value)
{ 
    return roundl(p_value);
}

/**
 * Compute the sign of the value given
 *
 * @param p_value   Integral or Floating point value to check
 *
 * @return          -1 If p_value <  0.
 * @return           0 If p_value == 0.
 * @return           1 If p_value >  0.
 */
template <typename Type>
CONSTEXPR   Type        Math::Sign                  (Type const         p_value)
{
    static_assert(std::is_arithmetic<Type>::value, "'Type' must be an arithmetic type !");
    return (0 < p_value) - (p_value < 0);
}

/**
 * Compute the sine of an angle given in radians
 *
 * @param p_radAngle    Floating point/Integral value representing an angle expressed in radians.
 *
 * @return              The sine of p_radAngle
 */
template <typename ReturnType, typename Type>
FORCEINLINE ReturnType  Math::Sin                   (Type const         p_radAngle)
{
    static_assert(std::is_arithmetic<ReturnType>::value, "ReturnType must be an arithmetic type !");
    static_assert(std::is_arithmetic<Type>::value, "Type must be an arithmetic type !");
    return (ReturnType)sin((double)p_radAngle);
}

/**
 * Compute the sine of an angle given in radians
 *
 * @param p_radAngle    Floating point value representing an angle expressed in radians.
 *
 * @return              The sine of p_radAngle
 */
FORCEINLINE double      Math::Sin                   (double const       p_radAngle)
{
    return sin(p_radAngle);
}

/**
 * Compute the sine of an angle given in radians
 *
 * @param p_radAngle    Floating point value representing an angle expressed in radians.
 *
 * @return              The sine of p_radAngle
 */
FORCEINLINE float       Math::Sin                   (float const        p_radAngle)
{
    return sinf(p_radAngle);
}

/**
 * Compute the sine of an angle given in radians
 *
 * @param p_radAngle    Floating point value representing an angle expressed in radians.
 *
 * @return              The sine of p_radAngle
 */
FORCEINLINE long double Math::Sin                   (long double const  p_radAngle)
{
    return sinl(p_radAngle);
}

 /**
 * Compute the hyperbolic sine of an angle given in radians
 *
 * @param p_value       Floating point/Integral value representing a hyperbolic angle.
 *
 * @return              The hyperbolic sine of p_value
 */
template <typename ReturnType, typename Type>
FORCEINLINE ReturnType  Math::SinH                  (Type const         p_value)
{
    static_assert(std::is_arithmetic<ReturnType>::value, "ReturnType must be an arithmetic type !");
    static_assert(std::is_arithmetic<Type>::value, "Type must be an arithmetic type !");
    return (ReturnType)sinh((double)p_value);
}

/**
 * Compute the hyperbolic sine of an angle given in radians
 *
 * @param p_value       Floating point value representing a hyperbolic angle.
 *
 * @return              The hyperbolic sine of p_value
 */
FORCEINLINE double      Math::SinH                  (double const       p_value)
{
    return sinh(p_value);
}

/**
 * Compute the hyperbolic sine of an angle given in radians
 *
 * @param p_value       Floating point value representing a hyperbolic angle.
 *
 * @return              The hyperbolic sine of p_value
 */
FORCEINLINE float       Math::SinH                  (float const        p_value)
{
    return sinhf(p_value);
}

/**
 * Compute the hyperbolic sine of an angle given in radians
 *
 * @param p_value       Floating point value representing a hyperbolic angle.
 *
 * @return              The hyperbolic sine of p_value
 */
FORCEINLINE long double Math::SinH                  (long double const  p_value)
{
    return sinhl(p_value);
}

/**
 * Compute the square of the value
 *
 * @param p_value   Floating point value whose square is calculated with.
 *
 * @return          p_value*p_value = p_value^2.
 */
template <typename Type>
CONSTEXPR   Type        Math::Square                (Type const         p_value)
{
    static_assert(std::is_arithmetic<Type>::value, "Type must be an arithmetic type !");
    return p_value*p_value;
}

/**
 * Compute the square root of the value given
 *
 * @param p_value   Floating point/Integral value whose square root is computed. If the argument is negative, a domain error occurs.
 *
 * @return          The square root of p_value.
 */
template <typename ReturnType, typename Type>
FORCEINLINE ReturnType  Math::Sqrt                  (Type const         p_value)
{
    static_assert(std::is_arithmetic<ReturnType>::value, "ReturnType must be an arithmetic type !");
    static_assert(std::is_arithmetic<Type>::value, "Type must be an arithmetic type !");
    return (ReturnType)sqrt((double)p_value);
}

/**
 * Compute the square root of the value given
 *
 * @param p_value   Floating point value whose square root is computed. If the argument is negative, a domain error occurs.
 *
 * @return          The square root of p_value.
 */
FORCEINLINE double      Math::Sqrt                  (double const       p_value)
{
    return sqrt(p_value);
}

/**
 * Compute the square root of the value given
 *
 * @param p_value   Floating point value whose square root is computed. If the argument is negative, a domain error occurs.
 *
 * @return          The square root of p_value.
 */
FORCEINLINE float       Math::Sqrt                  (float const        p_value)
{
    return sqrtf(p_value);
}

/**
 * Compute the square root of the value given
 *
 * @param p_value   Floating point value whose square root is computed. If the argument is negative, a domain error occurs.
 *
 * @return          The square root of p_value.
 */
FORCEINLINE long double Math::Sqrt                  (long double const  p_value)
{
    return sqrtl(p_value);
}

/**
 * Compute the tangent of an angle given in radians
 *
 * @param p_radAngle    Floating point/Integral value representing an angle expressed in radians.
 *
 * @return              The tangent of p_radAngle
 */
template <typename ReturnType, typename Type>
FORCEINLINE ReturnType  Math::Tan                   (Type const         p_radAngle)
{
    static_assert(std::is_arithmetic<ReturnType>::value, "ReturnType must be an arithmetic type !");
    static_assert(std::is_arithmetic<Type>::value, "Type must be an arithmetic type !");
    return (ReturnType)tan((double)p_radAngle);
}

/**
 * Compute the tangent of an angle given in radians
 *
 * @param p_radAngle    Floating point value representing an angle expressed in radians.
 *
 * @return              The tangent of p_radAngle
 */
FORCEINLINE double      Math::Tan                   (double const       p_radAngle)
{
    return tan(p_radAngle);
}

/**
 * Compute the tangent of an angle given in radians
 *
 * @param p_radAngle    Floating point value representing an angle expressed in radians.
 *
 * @return              The tangent of p_radAngle
 */
FORCEINLINE float       Math::Tan                   (float const        p_radAngle)
{
    return tanf(p_radAngle);
}

/**
 * Compute the tangent of an angle given in radians
 *
 * @param p_radAngle    Floating point value representing an angle expressed in radians.
 *
 * @return              The tangent of p_radAngle
 */
FORCEINLINE long double Math::Tan                   (long double const  p_radAngle)
{
    return tanl(p_radAngle);
}

 /**
 * Compute the hyperbolic tangent of an angle given in radians
 *
 * @param p_value       Floating point/Integral value representing a hyperbolic angle.
 *
 * @return              The hyperbolic tangent of p_value
 */
template <typename ReturnType, typename Type>
FORCEINLINE ReturnType  Math::TanH                  (Type const         p_value)
{
    static_assert(std::is_arithmetic<ReturnType>::value, "ReturnType must be an arithmetic type !");
    static_assert(std::is_arithmetic<Type>::value, "Type must be an arithmetic type !");
    return (ReturnType)tanh((double)p_value);
}

/**
* Compute the hyperbolic tangent of an angle given in radians
*
* @param p_value       Floating point value representing a hyperbolic angle.
*
* @return              The hyperbolic tangent of p_value
*/
FORCEINLINE double      Math::TanH                  (double const       p_value)
{
    return tanh(p_value);
}

/**
* Compute the hyperbolic tangent of an angle given in radians
*
* @param p_value       Floating point value representing a hyperbolic angle.
*
* @return              The hyperbolic tangent of p_value
*/
FORCEINLINE float       Math::TanH                  (float const        p_value)
{
    return tanhf(p_value);
}

/**
* Compute the hyperbolic tangent of an angle given in radians
*
* @param p_value       Floating point value representing a hyperbolic angle.
*
* @return              The hyperbolic tangent of p_value
*/
FORCEINLINE long double Math::TanH                  (long double const  p_value)
{
    return tanhl(p_value);
}

/**
 * Converts a value to an integer with truncation towards zero.
 *
 * @param p_value   The value to truncate.
 *
 * @return          The nearest integral value that is not larger in magnitude than p_value.
 */
template <typename ReturnType, typename Type>
FORCEINLINE ReturnType  Math::Trunc                 (Type const         p_value)
{
    static_assert(std::is_arithmetic<ReturnType>::value, "ReturnType must be an arithmetic type !");
    static_assert(std::is_arithmetic<Type>::value, "Type must be an arithmetic type !");
    return  (ReturnType)trunc((double)p_value);
}

/**
 * Converts a value to an integer with truncation towards zero.
 *
 * @param p_value   The value to truncate.
 *
 * @return          The nearest integral value that is not larger in magnitude than p_value.
 */
FORCEINLINE double      Math::Trunc                 (double const       p_value)
{
    return  trunc(p_value);
}

/**
 * Converts a value to an integer with truncation towards zero.
 *
 * @param p_value   The value to truncate.
 *
 * @return          The nearest integral value that is not larger in magnitude than p_value.
 */
FORCEINLINE float       Math::Trunc                 (float const        p_value)
{
    return  truncf(p_value);
}

/**
 * Converts a value to an integer with truncation towards zero.
 *
 * @param p_value   The value to truncate.
 *
 * @return          The nearest integral value that is not larger in magnitude than p_value.
 */
FORCEINLINE long double Math::Trunc                 (long double const  p_value)
{
    return  truncl(p_value);
}

#endif //! __MATH_UTILITY_INL_