#ifndef __GAME_USER_SETTINGS_HPP__
#define __GAME_USER_SETTINGS_HPP__

#include "Engine.hpp"

class ENGINE_API GameUserSettings : public UniqueObject
{
    public:

    // ============================== [Public Constructor and Destructor] ============================== //
        
        GameUserSettings    () = default;

        ~GameUserSettings   () = default;

    // ============================== [Public Local Methods] ============================== //

        /**
         * Saves the settings in a JSON file or creates a new one if it doesn't exist.
         */
        void    Save    (EngineKey const& p_passkey);

        /**
         * Loads the settings in a JSON file or uses the default values if it doesn't exist.
         */
        void    Load    (EngineKey const& p_passkey);

    // ==================================================================================== //

        /**
         * @thread_safety   This function must only be called from the main thread.
         */
        INLINE bool     IsVSyncEnabled      ()  const noexcept  { return m_isVSyncEnabled; }

        /**
         * @thread_safety   This function must only be called from the main thread.
         */
        INLINE uint32   GetResolutionWidth  ()  const noexcept  { return m_resolutionWidth; }

        /**
         * @thread_safety   This function must only be called from the main thread.
         */
        INLINE uint32   GetResolutionHeight ()  const noexcept  { return m_resolutionHeight; }

        /**
         * @thread_safety   This function must only be called from the main thread.
         */
        INLINE uint32   GetMultisampleCount ()  const noexcept  { return m_multisampleCount; }

        /**
         * @thread_safety   This function must only be called from the main thread.
         */
        INLINE float    GetAnisotropy       ()  const noexcept  { return m_anisotropy; }

    private:

    // ============================== [Private Local Properties] ============================== //

        Json m_json;

    // ======================================================================================== //

        bool    m_isVSyncEnabled    = false;

        uint32  m_resolutionWidth   = 0u;

        uint32  m_resolutionHeight  = 0u;

        uint32  m_multisampleCount  = 1u;

        float   m_anisotropy        = 1.0f;

};  // !class GameUserSettings

#endif // !__GAME_USER_SETTINGS_HPP__