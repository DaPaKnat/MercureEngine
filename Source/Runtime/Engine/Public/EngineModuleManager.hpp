#ifndef __ENGINE_MODULE_MANAGER_HPP__
#define __ENGINE_MODULE_MANAGER_HPP__

#include "CoreMinimal.hpp"

// ============================== [Forward Declarations] ============================== //

class EngineKey;
class EngineModule;

// ==================================================================================== //

class ENGINE_API EngineModuleManager : public UniqueObject
{
    public:

    // ============================== [Public Constructor and Destructor] ============================== //

        EngineModuleManager     ();

        ~EngineModuleManager    ();

    // ============================== [Public Local Methods] ============================== //

        void    Initialize  (EngineKey const& p_passkey) noexcept;

        void    Shutdown    (EngineKey const& p_passkey) noexcept;

    // ==================================================================================== //

        template<typename T>
        void    Add         ()  noexcept;

        template<typename T>
        bool    Contains    ()  const noexcept;

        template<typename T>
        T&      Get         ()  const noexcept;

        template<typename T>
        void    Remove      ()  noexcept;

    private:

    // ============================== [Private Local Properties] ============================== //

        /** Map containing the Engine modules as values and their hash codes as keys. */
        std::unordered_map<uint64, std::unique_ptr<EngineModule>> m_modules;

};  // !class EngineModuleManager

#include "EngineModuleManager.inl"

#endif // !__ENGINE_MODULE_MANAGER_HPP__