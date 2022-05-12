#ifndef __ENGINE_MODULE_HPP__
#define __ENGINE_MODULE_HPP__

#include "Engine.hpp"
#include "EngineModuleManager.hpp"

/**
 * This is the base class for an Engine module.
 */
class ENGINE_API BASE EngineModule : public UniqueObject
{
    public:

    // ============================== [Virtual Protected Local Methods] ============================== //

        /** Initializes this Engine module. */
        virtual void    Initialize  (EngineKey const& p_passkey)   = 0;

        /** Updates this Engine module. */
        virtual void    Update      (EngineKey const& p_passkey)   = 0;

        /** Shutdowns this Engine module. */
        virtual void    Shutdown    (EngineKey const& p_passkey)   = 0;

    protected:

    // ============================== [Virtual Protected Local Properties] ============================== //

        /** Whether or not this Engine module has been initialized. */
        bool m_initialized = false;

};  // !class EngineModule

#endif // !__ENGINE_MODULE_HPP__