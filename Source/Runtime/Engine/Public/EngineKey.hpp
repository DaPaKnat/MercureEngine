#ifndef __ENGINE_KEY_HPP__
#define __ENGINE_KEY_HPP__

#include "CoreMinimal.hpp"

/**
 * This class is used to secure acesses to critical functions of the global Engine modules.
 */
class ENGINE_API EngineKey : public UniqueObject
{
    private:

    // ============================== [Private Constructor and Destructor] ============================== //

        EngineKey   () = default;

        ~EngineKey  () = default;

    // ============================== [Friend Class] ============================== //

        friend class Engine;

};  // !class EngineKey

#endif // !__ENGINE_KEY_HPP__