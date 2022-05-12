#ifndef __VULKAN_LOADER_HPP__
#define __VULKAN_LOADER_HPP__

#include "CoreMinimal.hpp"

/**
 * This is the class managing the API's library.
 */
class ENGINE_API Loader : public UniqueObject
{
    private:

    // ============================== [Private Static Methods] ============================== //

        /**
         * Frees the Vulkan library.
         */
        static void Free                    ()  noexcept;

        /**
         * Returns whether or not the Vulkan library is loaded.
         */
        static bool IsLoaded                ()  noexcept;

        /**
         * Returns whether or not the Vulkan library could be loaded.
         */
        static bool Load                    ()  noexcept;

        /**
         * Returns whether or not the loaded version and the required version are compatible.
         */
        static bool Check                   ()  noexcept;

        /**
         * Returns whether or not the Vulkan library could be installed.
         */
        static bool Install                 ()  noexcept;

    // ============================== [Private Constructor and Destructor] ============================== //

        Loader  () = default;

        ~Loader () = default;

    // ============================== [Friend Class] ============================== //

        friend class RHI;

};  // !class Loader

#endif // !__VULKAN_LOADER_HPP__