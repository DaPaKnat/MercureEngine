#include "PCH.hpp"

#include "AssetManager.hpp"

#include "Builder/ModelBuilder.hpp"
#include "Builder/ShaderBuilder.hpp"
#include "Builder/TextureBuilder.hpp"

// ============================== [Module Public Local Methods] ============================== //

void    AssetManager::Initialize    (EngineKey const& p_passkey) noexcept
{
    LOG(LogAssetManager, Warning, "\nInitializing AssetManager...\n");

    if (!std::filesystem::exists(ASSET_DIRECTORY))
    {
        #if EDITOR

        std::error_code error;

        if (!std::filesystem::create_directory(ASSET_DIRECTORY, error))
            LOG(LogAssetManager, Fatal, "Failed to create %s : %s", ASSET_DIRECTORY, error.message());

        #else

        LOG(LogAssetManager, Fatal, "Failed to open %s", ASSET_DIRECTORY);

        #endif
    }

    std::filesystem::create_directory(ASSET_DIRECTORY + std::string("Default"));
    std::filesystem::create_directory(ASSET_DIRECTORY + std::string("Default/Models"));
    std::filesystem::create_directory(ASSET_DIRECTORY + std::string("Default/Shaders"));
    std::filesystem::create_directory(ASSET_DIRECTORY + std::string("Default/Materials"));
    std::filesystem::create_directory(ASSET_DIRECTORY + std::string("Default/MaterialInstances"));

    m_initialized = true;

    LOG(LogAssetManager, Warning, "\nAssetManager initialized\n");
}

void    AssetManager::Update        (EngineKey const& p_passkey) noexcept
{
    if (!m_initialized)
    {
        LOG(LogAssetManager, Error, "AssetManager was not initialized");
        return;
    }

    FlushUnused ();
    RemoveUnused();
}

void    AssetManager::Shutdown      (EngineKey const& p_passkey) noexcept
{
    LOG(LogAssetManager, Warning, "\nShutting down AssetManager...\n");

    if (m_initialized)
        FlushAll();

    m_initialized = false;

    LOG(LogAssetManager, Warning, "\nAssetManager shut down\n");
}

// ============================== [Public Local Methods] ============================== //

bool    AssetManager::Add       (std::shared_ptr<Asset> p_asset) noexcept
{
    if (p_asset == nullptr)
        return false;

    // Checks if the name is already used.
    std::string name(p_asset->m_name);

    {
        std::shared_lock lock(m_mutex);

        int32 i = 1;

        while (m_assets.find(name) != m_assets.end())
            name = p_asset->m_name + std::to_string(++i);
    }

    p_asset->m_name = std::move(name);

    // Adds the new asset to the map.
    {
        std::unique_lock lock(m_mutex);

        m_assets[p_asset->m_name] = p_asset;
    }

    LOG(LogAssetManager, Log, "New asset added : %s", p_asset->m_name.c_str());

    return true;
}

bool    AssetManager::Add       (ANSICHAR const* p_srcPath,
                                 ANSICHAR const* p_dstPath) noexcept
{
    std::filesystem::path srcPath(p_srcPath);

    if (std::filesystem::exists(srcPath) && std::filesystem::is_regular_file(srcPath))
    {
        std::string name     (srcPath.stem     ().string());
        std::string extension(srcPath.extension().string());

        if (ModelBuilder::IsExtensionSupported(extension))
        {
            return Add(ModelBuilder::BuildFromFile(p_dstPath + name, p_srcPath));
        }

        if (TextureBuilder::IsExtensionSupported(extension))
        {
            return Add(TextureBuilder::BuildFromFile(p_dstPath + name, p_srcPath));
        }

        if (ShaderBuilder::IsExtensionSupported(extension))
        {
            return Add(ShaderBuilder::BuildFromFile(p_dstPath + name, p_srcPath));
        }

        LOG(LogAssetManager, Error, "Extension not supported : %s", extension);
    }

    else
        LOG(LogAssetManager, Error, "Invalid file : %s", p_srcPath);

    return false;
}

bool    AssetManager::FindOrAdd (ANSICHAR const* p_srcPath,
                                 ANSICHAR const* p_dstPath) noexcept
{
    {
        std::shared_lock lock(m_mutex);

        if (m_assets.find(p_dstPath + std::filesystem::path(p_srcPath).stem().string()) != m_assets.end())
            return false;
    }

    return Add(p_srcPath, p_dstPath);
}

// ============================== [Private Local Methods] ============================== //

void    AssetManager::RemoveUnused  () noexcept
{
    std::unique_lock lock(m_mutex);

    for (auto it = m_assets.begin(); it != m_assets.end(); )
    {
        if (!it->second->m_isPending.load(std::memory_order_relaxed) &&
            !it->second->m_isLoaded .load(std::memory_order_relaxed))
        {
            it = m_assets.erase(it);
        }

        else
            ++it;
    }
}

void    AssetManager::FlushUnused   () noexcept
{
    std::vector<ThreadPool::Task> tasks;

    {
        std::shared_lock lock(m_mutex);

        for (auto const& it : m_assets)
        {
            // If "use_count" is greater than 1, the asset is still used.
            if (it.second.use_count() > 1)
                continue;

            if ( it.second->m_isLoaded .load(std::memory_order_relaxed) &&
                !it.second->m_isPending.load(std::memory_order_relaxed) )
            {
                it.second->m_isPending.store(true, std::memory_order_relaxed);

                tasks.push_back([it] { it.second->Serialize(ASSET_DIRECTORY + it.first + ".asset"); });
            }
        }
    }
    
    if (!tasks.empty())
        ThreadPool::Get().SubmitTasks(std::move(tasks));
}

void    AssetManager::FlushAll      () noexcept
{
    std::vector<ThreadPool::Task> tasks;

    {
        std::unique_lock lock(m_mutex);

        for (auto const& it : m_assets)
        {
            if ( it.second->m_isLoaded .load(std::memory_order_relaxed) &&
                !it.second->m_isPending.load(std::memory_order_relaxed) )
            {
                it.second->m_isPending.store(true, std::memory_order_relaxed);

                tasks.push_back([it] { it.second->Serialize(ASSET_DIRECTORY + it.first + ".asset"); });
            }
        }
    }
 
    if (!tasks.empty())
        ThreadPool::Get().SubmitTasks(std::move(tasks));
}