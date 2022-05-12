#ifndef __PLATFORM_HPP__
#define __PLATFORM_HPP__

// ============================== [Include Current Platform] ============================== //

#if defined(_WIN64) || defined(_WIN32)
    #include "Windows/WindowsPlatform.hpp"
#endif

// ============================== [Final Defines Setup] ============================== //

#ifndef CDECL
    #define CDECL
#endif

#ifndef STDCALL
    #define STDCALL
#endif

#ifndef INLINE
    #define INLINE
#endif

#ifndef FORCEINLINE
    #define FORCEINLINE
#endif

#ifndef FORCENOINLINE
    #define FORCENOINLINE
#endif

#ifndef CONSTEXPR
    #define CONSTEXPR
#endif

#ifndef MS_ALIGN
    #define MS_ALIGN(n)
#endif

#ifndef BASE
    #define BASE
#endif

#ifndef INTERFACE
    #define INTERFACE class
#endif

#ifndef DLLIMPORT
    #define DLLIMPORT
#endif

#ifndef DLLEXPORT
    #define DLLEXPORT
#endif

#ifdef ENGINE_EXPORT
    #define ENGINE_API DLLEXPORT
#else
    #define ENGINE_API DLLIMPORT
#endif

#define MAKE_VERSION(major, minor, patch) (((major) << 22) | ((minor) << 12) | (patch))

#define VERSION_MAJOR(version) ((uint32)(version) >> 22)
#define VERSION_MINOR(version) (((uint32)(version) >> 12) & 0x3ff)
#define VERSION_PATCH(version) ((uint32)(version) & 0xfff)

// ============================== [Unsigned Base Types] ============================== //

/** 8-bit unsigned integer. */
typedef PlatformTypes::uint8    uint8;

/** 16-bit unsigned integer. */
typedef PlatformTypes::uint16   uint16;

/** 32-bit unsigned integer. */
typedef PlatformTypes::uint32   uint32;

/** 64-bit unsigned integer. */
typedef PlatformTypes::uint64   uint64;

// ============================== [Signed Base Types] ============================== //

/** 8-bit signed integer. */
typedef PlatformTypes::int8     int8;

/** 16-bit signed integer. */
typedef PlatformTypes::int16    int16;

/** 32-bit signed integer. */
typedef PlatformTypes::int32    int32;

/** 64-bit signed integer. */
typedef PlatformTypes::int64    int64;

// ============================== [Character Types] ============================== //

/** ANSI character. */
typedef PlatformTypes::ANSICHAR ANSICHAR;

/** Wide character. */
typedef PlatformTypes::WIDECHAR WIDECHAR;

/** 8-bit character (Unicode, 8-bit, variable-width). */
typedef PlatformTypes::CHAR8    UTF8CHAR;

/** 16-bit character (Unicode, 16-bit, variable-width). */
typedef PlatformTypes::CHAR16   UTF16CHAR;

/** 32-bit character (Unicode, 32-bit, fixed-width). */
typedef PlatformTypes::CHAR32   UTF32CHAR;

// ============================== [Pointer Size] ============================== //

/** Size of a pointer as an unsigned integer. */
typedef PlatformTypes::SIZE_PTR     SIZE_PTR;

/** Size of a pointer as a signed integer. */
typedef PlatformTypes::SSIZE_PTR    SSIZE_PTR;

#endif // !__PLATFORM_HPP__