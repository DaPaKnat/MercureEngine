#ifndef __ASSET_MANAGER_INL__
#define __ASSET_MANAGER_INL__

#include "ThreadPool.hpp"
#include "AssetManager.hpp"

// ============================== [Public Local Methods] ============================== //

/**
 * Finds an asset in the registry.
 *
 * If the asset was found and has not been loaded yet, loads it in the desired mode.
 *
 * @warning         Before using any asset, its validity must be checked.
 *
 * @thread_safety   This function may be called from any thread.
 */
template<typename T>
std::shared_ptr<T>  AssetManager::Get   (ANSICHAR const* p_name,
                                         ELoadingMode    p_loadingMode)
{
    static_assert(std::is_base_of_v<Asset, T>, "The specified asset must derive from Asset");

    switch (p_loadingMode)
    {
        case ELoadingMode::ASYNCHRONOUS:
            return AsynchronousGet<T>(p_name);

        case ELoadingMode::BLOCKING:
            return BlockingGet<T>(p_name);

        default:
            break;
    }

    return nullptr;
}

// ============================== [Private Local Methods] ============================== //

/**
 * Finds an asset in the registry and loads it asynchronously.
 *
 * @warning         This function returns before the asset is completely loaded.
 *
 * @thread_safety   This function may be called from any thread.
 */
template<typename T>
std::shared_ptr<T>  AssetManager::AsynchronousGet   (std::string const& p_name)
{
    std::shared_ptr<Asset> asset;

    {
        std::shared_lock sharedLock(m_mutex);

        if (m_assets.find(p_name) == m_assets.end())
        {
            sharedLock.unlock();
            
            if (!std::filesystem::exists(ASSET_DIRECTORY + p_name + ".asset"))
                return nullptr;

            std::unique_lock uniqueLock(m_mutex);

            m_assets[p_name] = std::make_shared<T>(p_name);

            asset = m_assets.at(p_name);
        }

        else
            asset = m_assets.at(p_name);
    }

    if (!asset->m_isLoaded .load(std::memory_order_relaxed) &&
        !asset->m_isPending.load(std::memory_order_relaxed) )
    {
        asset->m_isPending.store(true, std::memory_order_relaxed);

        ThreadPool::Get().SubmitTask([=] { asset->Deserialize(ASSET_DIRECTORY + p_name + ".asset"); });
    }

    return std::dynamic_pointer_cast<T>(asset);
}

/**
 * Finds an asset in the registry and loads it.
 *
 * @warning         This function doesn't return until the asset is completely loaded.
 *
 * @thread_safety   This function may be called from any thread.
 */
template<typename T>
std::shared_ptr<T>  AssetManager::BlockingGet       (std::string const& p_name)
{
    std::shared_ptr<Asset> asset;

    {
        std::shared_lock sharedLock(m_mutex);

        if (m_assets.find(p_name) == m_assets.end())
        {
            sharedLock.unlock();
            
            if (!std::filesystem::exists(ASSET_DIRECTORY + p_name + ".asset"))
                return nullptr;

            std::unique_lock uniqueLock(m_mutex);

            m_assets[p_name] = std::make_shared<T>(p_name);

            asset = m_assets.at(p_name);
        }

        else
            asset = m_assets.at(p_name);
    }

    if (!asset->m_isLoaded .load(std::memory_order_relaxed) &&
        !asset->m_isPending.load(std::memory_order_relaxed) )
    {
        asset->m_isPending.store(true, std::memory_order_relaxed);

        asset->Deserialize(ASSET_DIRECTORY + p_name + ".asset");
    }

    return std::dynamic_pointer_cast<T>(asset);
}

#endif // !__ASSET_MANAGER_INL__