#ifndef __NUMERIC_LIMITS_HPP__
#define __NUMERIC_LIMITS_HPP__

#include "HAL/Platform.hpp"

#define MIN_UINT_8      ((uint8)    0x00)
#define MIN_UINT_16     ((uint16)   0x0000)
#define MIN_UINT_32     ((uint32)   0x00000000)
#define MIN_UINT_64     ((uint64)   0x0000000000000000)

#define MIN_INT_8       ((int8)     0x80)
#define MIN_INT_16      ((int16)    0x8000)
#define MIN_INT_32      ((int32)    0x80000000)
#define MIN_INT_64      ((int64)    0x8000000000000000)

#define MIN_FLOAT       (1.175494351e-38F)
#define MIN_DOUBLE      (2.2250738585072014e-308)
#define MIN_LONG_DOUBLE ((long double)MIN_DOUBLE)

#define MAX_UINT_8      ((uint8)    0xFF)
#define MAX_UINT_16     ((uint16)   0xFFFF)
#define MAX_UINT_32     ((uint32)   0xFFFFFFFF)
#define MAX_UINT_64     ((uint64)   0xFFFFFFFFFFFFFFFF)

#define MAX_INT_8       ((int8)     0x7F)
#define MAX_INT_16      ((int16)    0x7FFF)
#define MAX_INT_32      ((int32)    0x7FFFFFFF)
#define MAX_INT_64      ((int64)    0x7FFFFFFFFFFFFFFF)

#define MAX_FLOAT       (3.402823466e+38F)
#define MAX_DOUBLE      (1.7976931348623158e+308)
#define MAX_LONG_DOUBLE ((long double)MAX_DOUBLE)

template <typename Type>
struct ENGINE_API NumericLimits;

template <typename Type>
struct ENGINE_API NumericLimits<Type const> : public NumericLimits<Type>
{};

template <typename Type>
struct ENGINE_API NumericLimits<volatile Type> : public NumericLimits<Type>
{};

template <typename Type>
struct ENGINE_API NumericLimits<volatile Type const> : public NumericLimits<Type>
{};

template<>
struct ENGINE_API NumericLimits<uint8>
{
    static CONSTEXPR uint8  Min ()
    {
        return MIN_UINT_8;
    }

    static CONSTEXPR uint8  Max ()
    {
        return MAX_UINT_8;
    }
};

template<>
struct ENGINE_API NumericLimits<uint16>
{
    static CONSTEXPR uint16 Min ()
    {
        return MIN_UINT_16;
    }

    static CONSTEXPR uint16 Max ()
    {
        return MAX_UINT_16;
    }
};

template<>
struct ENGINE_API NumericLimits<uint32>
{
    static CONSTEXPR uint32 Min ()
    {
        return MIN_UINT_32;
    }

    static CONSTEXPR uint32 Max ()
    {
        return MAX_UINT_32;
    }
};

template<>
struct ENGINE_API NumericLimits<uint64>
{
    static CONSTEXPR uint64 Min ()
    {
        return MIN_UINT_64;
    }

    static CONSTEXPR uint64 Max ()
    {
        return MAX_UINT_64;
    }
};

template<>
struct ENGINE_API NumericLimits<int8>
{
    static CONSTEXPR int8   Min ()
    {
        return MIN_INT_8;
    }

    static CONSTEXPR int8   Max ()
    {
        return MAX_INT_8;
    }
};

template<>
struct ENGINE_API NumericLimits<int16>
{
    static CONSTEXPR int16  Min ()
    {
        return MIN_INT_16;
    }

    static CONSTEXPR int16  Max ()
    {
        return MAX_INT_16;
    }
};

template<>
struct ENGINE_API NumericLimits<int32>
{
    static CONSTEXPR int32 Min  ()
    {
        return MIN_INT_32;
    }

    static CONSTEXPR int32 Max  ()
    {
        return MAX_INT_32;
    }
};

template<>
struct ENGINE_API NumericLimits<signed long long>
{
    static CONSTEXPR int64  Min ()
    {
        return MIN_INT_64;
    }

    static CONSTEXPR int64  Max ()
    {
        return MAX_INT_64;
    }
};

template<>
struct ENGINE_API NumericLimits<float>
{
    static CONSTEXPR float  Min ()
    {
        return MIN_FLOAT;
    }

    static CONSTEXPR float  Max ()
    {
        return MAX_FLOAT;
    }
};

template<>
struct ENGINE_API NumericLimits<double>
{
    static CONSTEXPR double Min ()
    {
        return MIN_DOUBLE;
    }

    static CONSTEXPR double Max ()
    {
        return MAX_DOUBLE;
    }
};

template<>
struct ENGINE_API NumericLimits<long double>
{
    static CONSTEXPR long double    Min ()
    {
        return MIN_LONG_DOUBLE;
    }

    static CONSTEXPR long double    Max ()
    {
        return MAX_LONG_DOUBLE;
    }
};

#endif // !__NUMERIC_LIMITS_HPP__
