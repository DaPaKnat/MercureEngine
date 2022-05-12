#include "EngineModuleManager.hpp"

// ============================== [Public Local Methods] ============================== //

/**
 * Registers an Engine module to the module manager.
 */
template<typename T>
void    EngineModuleManager::Add         () noexcept
{
    uint64 hashCode = static_cast<uint64>(typeid(T).hash_code());

    if (m_modules.find(hashCode) == m_modules.end())
    {
        m_modules[hashCode] = std::make_unique<T>();

        LOG(LogEngine, Warning, "Module added : %s", typeid(T).name());
    }

    else
        LOG(LogEngine, Error, "Module already added : %s", typeid(T).name());

    return;
}

/**
 * @return  Whether or not the specified Engine module is registered.
 */
template<typename T>
bool    EngineModuleManager::Contains    () const noexcept
{
    return m_modules.find(static_cast<uint64>(typeid(T).hash_code())) != m_modules.end();
}

/**
 * 
 *
 * @return  A pointer to the requested Engine module.
 */
template<typename T>
T&      EngineModuleManager::Get         () const noexcept
{
    uint64 hashCode = static_cast<uint64>(typeid(T).hash_code());

    if (m_modules.find(hashCode) == m_modules.end())
    {
        LOG(LogEngine, Fatal, "Tried to retrieve an unregistered module : %s", typeid(T).name());
    }

    return *dynamic_cast<T*>(m_modules.at(hashCode).get());
}

/**
 * Removes an Engine module from the module manager.
 */
template<typename T>
void    EngineModuleManager::Remove      () noexcept
{
    uint64 hashCode = static_cast<uint64>(typeid(T).hash_code());

    if (m_modules.find(hashCode) != m_modules.end())
    {
        m_modules.erase(hashCode);

        LOG(LogEngine, Warning, "Module removed : %s.", typeid(T).name());
    }

    else
        LOG(LogEngine, Error, "Tried to remove a non registered module : %s.", typeid(T).name());

    return;
}