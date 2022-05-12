#ifndef __WINDOWS_PLATFORM_HPP__
#define __WINDOWS_PLATFORM_HPP__

#include "GenericPlatform\GenericPlatform.hpp"

// ============================== [Microsoft Visual Macros] ============================== //

#ifdef _MSC_VER
    
    // Function macros
    #define CDECL           __cdecl
    #define STDCALL         __stdcall
    #define INLINE          __inline
    #define FORCEINLINE     __forceinline
    #define FORCENOINLINE   __declspec(noinline)
    
    #if _MSC_VER >= 1900
        #define CONSTEXPR constexpr
    #endif

    // Data macros
    #define MS_ALIGN(n) __declspec(align(n))

    // Class attributes
    #define DLLIMPORT   __declspec(dllimport)
    #define DLLEXPORT   __declspec(dllexport)
    #define INTERFACE   __interface
    #define BASE        __declspec(novtable)
    
#endif

// ============================== [Windows Types] ============================== //

/**
 * Windows specific types.
 */
struct WindowsPlatformTypes : public GenericPlatformTypes
{
    #ifndef WIN32
        /** Size of a pointer as an unsigned integer for windows 64 bits. */
        typedef unsigned __int64    SIZE_PTR;

        /** Size of a pointer as a signed integer for windows 64 bits. */
        typedef __int64             SSIZE_PTR;
    #else
        /** Size of a pointer as an unsigned integer for windows 32 bits. */
        typedef unsigned __int32    SIZE_PTR;

        /** Size of a pointer as a signed integer for windows 32 bits. */
        typedef __int32             SSIZE_PTR;
    #endif

}; // !struct WindowsPlatformTypes

typedef WindowsPlatformTypes PlatformTypes;

#endif // !__WINDOWS_PLATFORM_HPP__