#include "PCH.hpp"

#include "GameUserSettings.hpp"

#ifndef EDITOR
    #define GAME_USER_SETTINGS_PATH "Config/GameUserSettings.json"
#else
    #define GAME_USER_SETTINGS_PATH "../../../Config/GameUserSettings.json" 
#endif

// ============================== [Public Local Methods] ============================== //

void    GameUserSettings::Save  (EngineKey const& p_passkey)
{
    std::ofstream file(GAME_USER_SETTINGS_PATH);

    if (file.is_open())
    {
        m_json["EnableVSync"]      = m_isVSyncEnabled;
        m_json["ResolutionWidth"]  = m_resolutionWidth;
        m_json["ResolutionHeight"] = m_resolutionHeight;
        m_json["MultisampleCount"] = m_multisampleCount;
        m_json["Anisotropy"]       = m_anisotropy;

        file << m_json.dump(4);
    }

    else
        LOG(LogEngine, Error, "Failed to open %s", GAME_USER_SETTINGS_PATH);

    file.close();
}

void    GameUserSettings::Load  (EngineKey const& p_passkey)
{
    std::ifstream file(GAME_USER_SETTINGS_PATH);

    if (file.is_open())
    {
        file >> m_json;

        m_isVSyncEnabled   = m_json.value("EnableVSync",      m_isVSyncEnabled);
        m_resolutionWidth  = m_json.value("ResolutionWidth",  m_resolutionWidth);
        m_resolutionHeight = m_json.value("ResolutionHeight", m_resolutionHeight);
        m_multisampleCount = m_json.value("MultisampleCount", m_multisampleCount);
        m_anisotropy       = m_json.value("Anisotropy",       m_anisotropy);
    }

    else
    {
        LOG(LogEngine, Warning, "Could not find config file, trying to generate one by default");

        Save(p_passkey);
    }

    file.close();
}