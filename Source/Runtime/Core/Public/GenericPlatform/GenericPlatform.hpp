#ifndef __GENERIC_PLATFORM_HPP__
#define __GENERIC_PLATFORM_HPP__

/**
 * Generic types for almost all platforms.
 */
struct GenericPlatformTypes
{
    // ============================== [Unsigned Base Types] ============================== //

    /** 8-bit unsigned integer. */
    typedef unsigned char       uint8;

    /** 16-bit unsigned integer. */
    typedef unsigned short      uint16;

    /** 32-bit unsigned integer. */
    typedef unsigned int        uint32;

    /** 64-bit unsigned integer. */
    typedef unsigned long long  uint64;
    
    // ============================== [Signed Base Types] ============================== //

    /** 8-bit signed integer. */
    typedef signed char         int8;

    /** 16-bit signed integer. */
    typedef signed short        int16;

    /** 32-bit signed integer. */
    typedef signed int          int32;

    /** 64-bit signed integer. */
    typedef signed long long    int64;

    // ============================== [Character Types] ============================== //

    /** ANSI character (8-bit fixed width representation). */
    typedef char    ANSICHAR;

    /** Wide character (the largest supported character set). */
    typedef wchar_t WIDECHAR;

    /** 8-bit character type. */
    typedef uint8   CHAR8;

    /** 16-bit character type. */
    typedef uint16  CHAR16;

    /** 32-bit character type. */
    typedef uint32  CHAR32;

}; // !struct GenericPlatformTypes

#endif // !__GENERIC_PLATFORM_HPP__