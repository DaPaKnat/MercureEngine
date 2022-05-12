#ifndef __MATH_UTILITY_HPP__
#define __MATH_UTILITY_HPP__

#include <corecrt_math.h>
#include <type_traits>

#include "Reflection/Reflection.hpp"

#include "MathMacro.hpp"
#include "NumericLimits.hpp"

struct ENGINE_API Math
{
    template <typename Type>
    static  CONSTEXPR   Type        Abs                 (Type const         p_value);
    static  FORCEINLINE double      Abs                 (double const       p_value);
    static  FORCEINLINE float       Abs                 (float const        p_value);
    static  FORCEINLINE long double Abs                 (long double const  p_value);

    template <typename ReturnType = float, typename Type>
    static  FORCEINLINE ReturnType  Acos                (Type const         p_value);
    static  FORCEINLINE double      Acos                (double const       p_value);
    static  FORCEINLINE float       Acos                (float const        p_value);
    static  FORCEINLINE long double Acos                (long double const  p_value);

    template <typename ReturnType = float, typename Type>
    static  FORCEINLINE ReturnType  AcosH               (Type const         p_value);
    static  FORCEINLINE double      AcosH               (double const       p_value);
    static  FORCEINLINE float       AcosH               (float const        p_value);
    static  FORCEINLINE long double AcosH               (long double const  p_value);
    
    template <typename ReturnType = float, typename Type>
    static  FORCEINLINE ReturnType  Asin                (Type const         p_value);
    static  FORCEINLINE double      Asin                (double const       p_value);
    static  FORCEINLINE float       Asin                (float const        p_value);
    static  FORCEINLINE long double Asin                (long double const  p_value);

    template <typename ReturnType = float, typename Type>
    static  FORCEINLINE ReturnType  AsinH               (Type const         p_value);
    static  FORCEINLINE double      AsinH               (double const       p_value);
    static  FORCEINLINE float       AsinH               (float const        p_value);
    static  FORCEINLINE long double AsinH               (long double const  p_value);

    template <typename ReturnType = float, typename Type>
    static  FORCEINLINE ReturnType  Atan                (Type const         p_value);
    static  FORCEINLINE double      Atan                (double const       p_value);
    static  FORCEINLINE float       Atan                (float const        p_value);
    static  FORCEINLINE long double Atan                (long double const  p_value);

    template <typename ReturnType = float, typename Type>
    static  FORCEINLINE ReturnType  Atan2               (Type const         p_y,
                                                         Type const         p_x);
    static  FORCEINLINE double      Atan2               (double const       p_y,
                                                         double const       p_x);
    static  FORCEINLINE float       Atan2               (float const        p_y,
                                                         float const        p_x);
    static  FORCEINLINE long double Atan2               (long double const  p_y,
                                                         long double const  p_x);

    template <typename ReturnType = float, typename Type>
    static  FORCEINLINE ReturnType  AtanH               (Type const         p_value);
    static  FORCEINLINE double      AtanH               (double const       p_value);
    static  FORCEINLINE float       AtanH               (float const        p_value);
    static  FORCEINLINE long double AtanH               (long double const  p_value);

    template <typename ReturnType = float, typename Type>
    static  FORCEINLINE ReturnType  Cbrt                (Type const         p_value);
    static  FORCEINLINE double      Cbrt                (double const       p_value);
    static  FORCEINLINE float       Cbrt                (float const        p_value);
    static  FORCEINLINE long double Cbrt                (long double const  p_value);

    template <typename ReturnType = float, typename Type>
    static  FORCEINLINE ReturnType  Ceil                (Type const         p_value);
    static  FORCEINLINE double      Ceil                (double const       p_value);
    static  FORCEINLINE float       Ceil                (float const        p_value);
    static  FORCEINLINE long double Ceil                (long double const  p_value);

    template <typename Type>
    static  CONSTEXPR   Type        Clamp               (Type const         p_value,
                                                         Type const         p_min,
                                                         Type const         p_max);

    template <typename Type>
    static  CONSTEXPR   Type        Clamp01             (Type const         p_value);
    static  CONSTEXPR   double      Clamp01             (double const       p_value);
    static  CONSTEXPR   float       Clamp01             (float const        p_value);
    static  CONSTEXPR   long double Clamp01             (long double const  p_value);

    static              double      ClampAngleDeg       (double const       p_angleDegrees,
                                                         double const       p_minAngleDegrees,
                                                         double const       p_maxAngleDegrees);
    static              float       ClampAngleDeg       (float const        p_angleDegrees,
                                                         float const        p_minAngleDegrees,
                                                         float const        p_maxAngleDegrees);
    static              long double ClampAngleDeg       (long double const  p_angleDegrees,
                                                         long double const  p_minAngleDegrees,
                                                         long double const  p_maxAngleDegrees);


    static              double      ClampAngleRad       (double const       p_angleRadians,
                                                         double const       p_minAngleRadians,
                                                         double const       p_maxAngleRadians);
    static              float       ClampAngleRad       (float const        p_angleRadians,
                                                         float const        p_minAngleRadians,
                                                         float const        p_maxAngleRadians);
    static              long double ClampAngleRad       (long double const  p_angleRadians,
                                                         long double const  p_minAngleRadians,
                                                         long double const  p_maxAngleRadians);

    static  FORCEINLINE double      CopySign            (double const       p_value,
                                                         double const       p_sign);
    static  FORCEINLINE float       CopySign            (float const        p_value,
                                                         float const        p_sign);
    static  FORCEINLINE long double CopySign            (long double const  p_value,
                                                         long double const  p_sign);

    template <typename ReturnType = float, typename Type>
    static  FORCEINLINE ReturnType  Cos                 (Type const         p_radAngle);
    static  FORCEINLINE double      Cos                 (double const       p_radAngle);
    static  FORCEINLINE float       Cos                 (float const        p_radAngle);
    static  FORCEINLINE long double Cos                 (long double const  p_radAngle);

    template <typename ReturnType = float, typename Type>
    static  FORCEINLINE ReturnType  CosH                (Type const         p_value);
    static  FORCEINLINE double      CosH                (double const       p_value);
    static  FORCEINLINE float       CosH                (float const        p_value);
    static  FORCEINLINE long double CosH                (long double const  p_value);

    template <typename Type>
    static  CONSTEXPR   Type        Cube                (Type const         p_value);

    template <typename ReturnType = float, typename Type>
    static  CONSTEXPR   ReturnType  DegToRad            (Type const         p_degAngle);
    static  CONSTEXPR   double      DegToRad            (double const       p_degAngle);
    static  CONSTEXPR   float       DegToRad            (float const        p_degAngle);
    static  CONSTEXPR   long double DegToRad            (long double const  p_degAngle);

    static  FORCEINLINE double      DeltaAngleDeg       (double const       p_current,
                                                         double const       p_target);
    static  FORCEINLINE float       DeltaAngleDeg       (float const        p_current,
                                                         float const        p_target);
    static  FORCEINLINE long double DeltaAngleDeg       (long double const  p_current,
                                                         long double const  p_target);

    static  FORCEINLINE double      DeltaAngleRad       (double const       p_current,
                                                         double const       p_target);
    static  FORCEINLINE float       DeltaAngleRad       (float const        p_current,
                                                         float const        p_target);
    static  FORCEINLINE long double DeltaAngleRad       (long double const  p_current,
                                                         long double const  p_target);

    template <typename ReturnType = float, typename Type>
    static  FORCEINLINE ReturnType  Exp                 (Type const         p_value);
    static  FORCEINLINE double      Exp                 (double const       p_value);
    static  FORCEINLINE float       Exp                 (float const        p_value);
    static  FORCEINLINE long double Exp                 (long double const  p_value);

    template <typename ReturnType = float, typename Type>
    static  FORCEINLINE ReturnType  Exp2                (Type const         p_value);
    static  FORCEINLINE double      Exp2                (double const       p_value);
    static  FORCEINLINE float       Exp2                (float const        p_value);
    static  FORCEINLINE long double Exp2                (long double const  p_value);
    
    static  CONSTEXPR   double      FloatSelect         (double const       p_comparand,
                                                         double const       p_valueGEZero,
                                                         double const       p_valueLTZero);

    static  CONSTEXPR   float       FloatSelect         (float const        p_comparand,
                                                         float const        p_valueGEZero,
                                                         float const        p_valueLTZero);

    static  CONSTEXPR   long double FloatSelect         (long double const  p_comparand,
                                                         long double const  p_valueGEZero,
                                                         long double const  p_valueLTZero);

    template <typename ReturnType = float, typename Type>
    static  FORCEINLINE ReturnType  Floor               (Type const         p_value);
    static  FORCEINLINE double      Floor               (double const       p_value);
    static  FORCEINLINE float       Floor               (float const        p_value);
    static  FORCEINLINE long double Floor               (long double const  p_value);

    static  FORCEINLINE double      Fmod                (double const       p_value,
                                                         double const       p_modulo);
    static  FORCEINLINE float       Fmod                (float const        p_value,
                                                         float const        p_modulo);
    static  FORCEINLINE long double Fmod                (long double const  p_value,
                                                         long double const  p_modulo);

    template <typename ReturnType = float, typename Type>
    static  FORCEINLINE ReturnType  Frac                (Type const         p_value);
    static  FORCEINLINE double      Frac                (double const       p_value);
    static  FORCEINLINE float       Frac                (float const        p_value);
    static  FORCEINLINE long double Frac                (long double const  p_value);

    template <typename ReturnType = float, typename Type>
    static  FORCEINLINE ReturnType  Fractional          (Type const         p_value);
    static  FORCEINLINE double      Fractional          (double const       p_value);
    static  FORCEINLINE float       Fractional          (float const        p_value);
    static  FORCEINLINE long double Fractional          (long double const  p_value);

    template <typename ReturnType = double, typename Type>
    static  FORCEINLINE ReturnType  InverseLerp         (Type const         p_a,
                                                         Type const         p_b,
                                                         double const       p_value);

    template <typename ReturnType = float, typename Type>
    static  FORCEINLINE ReturnType  InverseLerp         (Type const         p_a,
                                                         Type const         p_b,
                                                         float const        p_value);

    template <typename ReturnType = long double, typename Type>
    static  FORCEINLINE ReturnType  InverseLerp         (Type const         p_a,
                                                         Type const         p_b,
                                                         long double const  p_value);

    template <typename ReturnType = float, typename Type>                                                        
    static  FORCEINLINE ReturnType  InvSqrt             (Type const         p_value);
    static  FORCEINLINE double      InvSqrt             (double const       p_value);
    static  FORCEINLINE float       InvSqrt             (float const        p_value);
    static  FORCEINLINE long double InvSqrt             (long double const  p_value);

    static  FORCEINLINE double      IsFinite            (double const       p_value);
    static  FORCEINLINE float       IsFinite            (float const        p_value);
    static  FORCEINLINE long double IsFinite            (long double const  p_value);

    static  FORCEINLINE double      IsInfinite          (double const       p_value);
    static  FORCEINLINE float       IsInfinite          (float const        p_value);
    static  FORCEINLINE long double IsInfinite          (long double const  p_value);

    static  FORCEINLINE double      IsNaN               (double const       p_value);
    static  FORCEINLINE float       IsNaN               (float const        p_value);
    static  FORCEINLINE long double IsNaN               (long double const  p_value);

    static  FORCEINLINE bool        IsNearlyEqual       (double const       p_a, 
                                                         double const       p_b, 
                                                         double const       p_errorTolerance = MATH_EPSILON);
    static  FORCEINLINE bool        IsNearlyEqual       (float const        p_a, 
                                                         float const        p_b, 
                                                         float const        p_errorTolerance = MATH_EPSILON_F);
    static  FORCEINLINE bool        IsNearlyEqual       (long double const  p_a, 
                                                         long double const  p_b, 
                                                         long double const  p_errorTolerance = MATH_EPSILON_L);


    static  FORCEINLINE bool        IsNearlyZero        (double const       p_value,
                                                         double const       p_errorTolerance = MATH_EPSILON);
    static  FORCEINLINE bool        IsNearlyZero        (float const        p_value,
                                                         float const        p_errorTolerance = MATH_EPSILON_F);
    static  FORCEINLINE bool        IsNearlyZero        (long double const  p_value,
                                                         long double const  p_errorTolerance = MATH_EPSILON_L);


    static  FORCEINLINE bool        IsNegative          (double const       p_value);
    static  FORCEINLINE bool        IsNegative          (float const        p_value);
    static  FORCEINLINE bool        IsNegative          (long double const  p_value);

    template <typename Type>
    static  FORCEINLINE bool        IsOutsideIn         (Type const         p_value,
                                                         Type const         p_minInclusiveValue, 
                                                         Type const         p_maxInclusiveValue);
    template <typename Type>
    static  FORCEINLINE bool        IsOutsideInEx       (Type const         p_value,
                                                         Type const         p_minInclusiveValue, 
                                                         Type const         p_maxExclusiveValue);
    template <typename Type>
    static  FORCEINLINE bool        IsOutsideEx         (Type const         p_value,
                                                         Type const         p_minExclusiveValue,
                                                         Type const         p_maxExclusiveValue);
    template <typename Type>
    static  FORCEINLINE bool        IsOutsideExIn       (Type const         p_value,
                                                         Type const         p_minExclusiveValue, 
                                                         Type const         p_maxInclusiveValue);

    static  FORCEINLINE bool        IsPositive          (double const       p_value);
    static  FORCEINLINE bool        IsPositive          (float const        p_value);
    static  FORCEINLINE bool        IsPositive          (long double const  p_value);

    template <typename Type>
    static  FORCEINLINE bool        IsPowerOfTwo        (Type const         p_value);

    template <typename Type>
    static  FORCEINLINE bool        IsWithinIn          (Type const         p_value,
                                                         Type const         p_minInclusiveValue, 
                                                         Type const         p_maxInclusiveValue);
    template <typename Type>
    static  FORCEINLINE bool        IsWithinInEx        (Type const         p_value,
                                                         Type const         p_minInclusiveValue, 
                                                         Type const         p_maxExclusiveValue);
    template <typename Type>
    static  FORCEINLINE bool        IsWithinEx          (Type const         p_value,
                                                         Type const         p_minExclusiveValue,
                                                         Type const         p_maxExclusiveValue);
    template <typename Type>
    static  FORCEINLINE bool        IsWithinExIn        (Type const         p_value,
                                                         Type const         p_minExclusiveValue, 
                                                         Type const         p_maxInclusiveValue);

    template <typename ReturnType = double, typename Type>
    static  FORCEINLINE ReturnType  Lerp                (Type const         p_a,
                                                         Type const         p_b,
                                                         double const       p_alpha);
    template <typename ReturnType = float, typename Type>
    static  FORCEINLINE ReturnType  Lerp                (Type const         p_a,
                                                         Type const         p_b,
                                                         float const        p_alpha);
    template <typename ReturnType = long double, typename Type>
    static  FORCEINLINE ReturnType  Lerp                (Type const         p_a,
                                                         Type const         p_b,
                                                         long double const  p_alpha);

    template <typename ReturnType = double, typename Type>
    static  FORCEINLINE ReturnType  LerpAngleDeg        (Type const         p_a,
                                                         Type const         p_b,
                                                         double const       p_alpha);
    template <typename ReturnType = float, typename Type>
    static  FORCEINLINE ReturnType  LerpAngleDeg        (Type const         p_a,
                                                         Type const         p_b,
                                                         float const        p_alpha);
    template <typename ReturnType = long double, typename Type>
    static  FORCEINLINE ReturnType  LerpAngleDeg        (Type const         p_a,
                                                         Type const         p_b,
                                                         long double const  p_alpha);

    template <typename ReturnType = double, typename Type>
    static  FORCEINLINE ReturnType  LerpAngleRad        (Type const         p_a,
                                                         Type const         p_b,
                                                         double const       p_alpha);
    template <typename ReturnType = float, typename Type>
    static  FORCEINLINE ReturnType  LerpAngleRad        (Type const         p_a,
                                                         Type const         p_b,
                                                         float const        p_alpha);
    template <typename ReturnType = long double, typename Type>
    static  FORCEINLINE ReturnType  LerpAngleRad        (Type const         p_a,
                                                         Type const         p_b,
                                                         long double const  p_alpha);

    template <typename ReturnType = float, typename Type>                                                        
    static  FORCEINLINE ReturnType  Log2                (Type const         p_value);
    static  FORCEINLINE double      Log2                (double const       p_value);
    static  FORCEINLINE float       Log2                (float const        p_value);
    static  FORCEINLINE long double Log2                (long double const  p_value);

    template <typename ReturnType = float, typename Type>                                                        
    static  FORCEINLINE ReturnType  Log10               (Type const         p_value);
    static  FORCEINLINE double      Log10               (double const       p_value);
    static  FORCEINLINE float       Log10               (float const        p_value);
    static  FORCEINLINE long double Log10               (long double const  p_value);

    template <typename ReturnType = float, typename Type>                                                        
    static  FORCEINLINE ReturnType  LogE                (Type const         p_value);
    static  FORCEINLINE double      LogE                (double const       p_value);
    static  FORCEINLINE float       LogE                (float const        p_value);
    static  FORCEINLINE long double LogE                (long double const  p_value);

    static  FORCEINLINE double      LogX                (double const       p_value,
                                                         double const       p_base);
    static  FORCEINLINE float       LogX                (float const        p_value,
                                                         float const        p_base);
    static  FORCEINLINE long double LogX                (long double const  p_value,
                                                         long double const  p_base);

    template <typename Type>
    static  CONSTEXPR   Type        Max                 (Type const         p_a,
                                                         Type const         p_b);

    template <typename Type>
    static  CONSTEXPR   Type        Max3                (Type const         p_a,
                                                         Type const         p_b,
                                                         Type const         p_c);

    template <typename Type>
    static  CONSTEXPR   Type        Min                 (Type const         p_a,
                                                         Type const         p_b);

    template <typename Type>
    static  CONSTEXPR   Type        Min3                (Type const         p_a,
                                                         Type const         p_b,
                                                         Type const         p_c);

    static  FORCEINLINE double      Modf                (double const       p_value,
                                                         double*            p_intPart);
    static  FORCEINLINE float       Modf                (float const        p_value,
                                                         float*             p_intPart);
    static  FORCEINLINE long double Modf                (long double const  p_value,
                                                         long double*       p_intPart);

    static  FORCEINLINE double      MoveTowards         (double const       p_current,
                                                         double const       p_target,
                                                         double const       p_maxDelta);
    static  FORCEINLINE float       MoveTowards         (float const        p_current,
                                                         float const        p_target,
                                                         float const        p_maxDelta);
    static  FORCEINLINE long double MoveTowards         (long double const  p_current,
                                                         long double const  p_target,
                                                         long double const  p_maxDelta);

    static  FORCEINLINE double      MoveTowardsAngleDeg (double const       p_current,
                                                         double const       p_target,
                                                         double const       p_maxDelta);
    static  FORCEINLINE float       MoveTowardsAngleDeg (float const        p_current,
                                                         float const        p_target,
                                                         float const        p_maxDelta);
    static  FORCEINLINE long double MoveTowardsAngleDeg (long double const  p_current,
                                                         long double const  p_target,
                                                         long double const  p_maxDelta);

    static  FORCEINLINE double      MoveTowardsAngleRad (double const       p_current,
                                                         double const       p_target,
                                                         double const       p_maxDelta);
    static  FORCEINLINE float       MoveTowardsAngleRad (float const        p_current,
                                                         float const        p_target,
                                                         float const        p_maxDelta);
    static  FORCEINLINE long double MoveTowardsAngleRad (long double const  p_current,
                                                         long double const  p_target,
                                                         long double const  p_maxDelta);

    template <typename Type>
    static              Type        NextPowerOfTwo      (Type const         p_value);

    template <typename ReturnType = float, typename Type1, typename Type2>
    static  FORCEINLINE ReturnType  NthRoot             (Type1 const        p_value,
                                                         Type2 const        p_nthRoot);
    static  FORCEINLINE double      NthRoot             (double const       p_value,
                                                         double const       p_nthRoot);
    static  FORCEINLINE float       NthRoot             (float const        p_value,
                                                         float const        p_nthRoot);
    static  FORCEINLINE long double NthRoot             (long double const  p_value,
                                                         long double const  p_nthRoot);

    static  FORCEINLINE double      PingPong            (double const       p_value,
                                                         double const       p_length);

    static  FORCEINLINE float       PingPong            (float const        p_value,
                                                         float const        p_length);

    static  FORCEINLINE long double PingPong            (long double const  p_value,
                                                         long double const  p_length);

    template <typename ReturnType = float, typename Type1, typename Type2>
    static  FORCEINLINE ReturnType  Pow                 (Type1 const        p_base,
                                                         Type2 const        p_exponent);
    static  FORCEINLINE double      Pow                 (double const       p_base,
                                                         double const       p_exponent);
    static  FORCEINLINE float       Pow                 (float const        p_base,
                                                         float const        p_exponent);
    static  FORCEINLINE long double Pow                 (long double const  p_base,
                                                         long double const  p_exponent);

    template <typename ReturnType = float, typename Type>
    static  CONSTEXPR   ReturnType  RadToDeg            (Type const         p_radAngle);
    static  CONSTEXPR   double      RadToDeg            (double const       p_radAngle);
    static  CONSTEXPR   float       RadToDeg            (float const        p_radAngle);
    static  CONSTEXPR   long double RadToDeg            (long double const  p_radAngle);

    static  FORCEINLINE double      Repeat              (double const       p_value,
                                                         double const       p_length);

    static  FORCEINLINE float       Repeat              (float const        p_value,
                                                         float const        p_length);

    static  FORCEINLINE long double Repeat              (long double const  p_value,
                                                         long double const  p_length);

    template <typename ReturnType = float, typename Type>
    static  FORCEINLINE ReturnType  Round               (Type const         p_value);
    static  FORCEINLINE double      Round               (double const       p_value);
    static  FORCEINLINE float       Round               (float const        p_value);
    static  FORCEINLINE long double Round               (long double const  p_value);

    template <typename Type>
    static  CONSTEXPR   Type        Sign                (Type const         p_value);

    template <typename ReturnType = float, typename Type>
    static  FORCEINLINE ReturnType  Sin                 (Type const         p_radAngle);
    static  FORCEINLINE double      Sin                 (double const       p_radAngle);
    static  FORCEINLINE float       Sin                 (float const        p_radAngle);
    static  FORCEINLINE long double Sin                 (long double const  p_radAngle);

    template <typename ReturnType = float, typename Type>
    static  FORCEINLINE ReturnType  SinH                (Type const         p_value);
    static  FORCEINLINE double      SinH                (double const       p_value);
    static  FORCEINLINE float       SinH                (float const        p_value);
    static  FORCEINLINE long double SinH                (long double const  p_value);

    template <typename Type>
    static  CONSTEXPR   Type        Square              (Type const         p_value);

    template <typename ReturnType = float, typename Type>
    static  FORCEINLINE ReturnType  Sqrt                (Type const         p_value);
    static  FORCEINLINE double      Sqrt                (double const       p_value);
    static  FORCEINLINE float       Sqrt                (float const        p_value);
    static  FORCEINLINE long double Sqrt                (long double const  p_value);

    template <typename ReturnType = float, typename Type>
    static  FORCEINLINE ReturnType  Tan                 (Type const         p_radAngle);
    static  FORCEINLINE double      Tan                 (double const       p_radAngle);
    static  FORCEINLINE float       Tan                 (float const        p_radAngle);
    static  FORCEINLINE long double Tan                 (long double const  p_radAngle);

    template <typename ReturnType = float, typename Type>
    static  FORCEINLINE ReturnType  TanH                (Type const         p_value);
    static  FORCEINLINE double      TanH                (double const       p_value);
    static  FORCEINLINE float       TanH                (float const        p_value);
    static  FORCEINLINE long double TanH                (long double const  p_value);

    template <typename ReturnType = float, typename Type>
    static  FORCEINLINE ReturnType  Trunc               (Type const         p_value);
    static  FORCEINLINE double      Trunc               (double const       p_value);
    static  FORCEINLINE float       Trunc               (float const        p_value);
    static  FORCEINLINE long double Trunc               (long double const  p_value);

    template <typename ReturnType = float, typename Type>
    static              ReturnType  UnwindDeg           (Type const         p_angleDegree);
    static              double      UnwindDeg           (double             p_angleDegree);
    static              float       UnwindDeg           (float              p_angleDegree);
    static              long double UnwindDeg           (long double        p_angleDegree);
    
    template <typename ReturnType = float, typename Type>
    static              ReturnType  UnwindRad           (Type const         p_angleRadians);
    static              double      UnwindRad           (double             p_angleRadians);
    static              float       UnwindRad           (float              p_angleRadians);
    static              long double UnwindRad           (long double        p_angleRadians);
};

#include "Utility.inl"

#endif // !__MATH_UTILITY_HPP__