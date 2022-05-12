#ifndef __ASSET_MANAGER_HPP__
#define __ASSET_MANAGER_HPP__

#include "EngineModule.hpp"

#include "RHIAsset.hpp"

#ifndef EDITOR
    #define ASSET_DIRECTORY  "Content/"
#else
    #define ASSET_DIRECTORY  "../../../Content/"
    #define SOURCE_DIRECTORY "../../../Source/"
#endif

enum class ELoadingMode : uint8
{
    ASYNCHRONOUS,
    BLOCKING

};    // !enum class ELoadingMode

class ENGINE_API AssetManager : public EngineModule
{
    public:

    // ============================== [Public Static Methods] ============================== //

        static INLINE AssetManager& Get() noexcept
        {
            return GEngine->GetModuleManager()->Get<AssetManager>();
        }

    // ============================== [Public Constructor and Destructor] ============================== //

        AssetManager    () = default;

        ~AssetManager   () = default;

    // ============================== [Module Public Local Methods] ============================== //
        
        /**
         * Checks the content of the asset directory and adds the discovered assets to the registry. 
         */
        void    Initialize  (EngineKey const& p_passkey)   noexcept final override;

        /**
         * Checks if there is any unused asset to flush.
         */
        void    Update      (EngineKey const& p_passkey)   noexcept final override;

        /**
         * Flushes the whole asset registry.
         */
        void    Shutdown    (EngineKey const& p_passkey)   noexcept final override;

    // ============================== [Public Local Methods] ============================== //

        /**
         * Adds a new asset to the registry and modifies its name if it's already used.
         *
         * @return Whether or not the asset could be added.
         */
        bool    Add         (std::shared_ptr<Asset> p_asset)        noexcept;

        /**
         * Creates a new asset from a file and adds a new asset to the registry.
         *
         * @param p_srcPath Absolute path to the file.
         * @param p_dstPath Relative path in the asset directory.
         *
         * @thread_safety   This function may be called from any thread.
         */
        bool    Add         (ANSICHAR const*        p_srcPath,
                             ANSICHAR const*        p_dstPath = "") noexcept;

        /**
         * Creates a new asset from a file and adds a new asset to the registry if it it could not be found.
         *
         * @param p_srcPath Absolute path to the file.
         * @param p_dstPath Relative path in the asset directory.
         * 
         * @return          Whether or not a new asset was added.
         *
         * @thread_safety   This function may be called from any thread.
         */
        bool    FindOrAdd   (ANSICHAR const*        p_srcPath,
                             ANSICHAR const*        p_dstPath = "") noexcept;

    // ==================================================================================== //

        template<typename T>
        std::shared_ptr<T> Get(ANSICHAR const*  p_name,
                               ELoadingMode     p_loadingMode = ELoadingMode::ASYNCHRONOUS);

    private:

    // ============================== [Private Local Properties] ============================== //

        std::shared_mutex                                       m_mutex;

        std::unordered_map<std::string, std::shared_ptr<Asset>> m_assets;

    // ============================== [Private Local Methods] ============================== //

        /**
         * Removes unused assets from the map.
         *
         * @thread_safety This function must only be called from the main thread.
         */
        void    RemoveUnused    ()                              noexcept;

        /**
         * Flushes unused assets.
         *
         * @thread_safety This function must only be called from the main thread.
         */
        void    FlushUnused     ()                              noexcept;

        /**
         * Flushes unused assets and waits for the serialization to finish.
         *
         * @warning         Every asset should be released before calling this function.
         *
         * @thread_safety   This function must only be called from the main thread.
         */
        void    FlushAll        ()                              noexcept;

    // ===================================================================================== //

        template<typename T>
        std::shared_ptr<T>  AsynchronousGet (std::string const& p_name);

        template<typename T>
        std::shared_ptr<T>  BlockingGet     (std::string const& p_name);

};  // !class AssetManager

#include "AssetManager.inl"

#endif // !__ASSET_MANAGER_HPP__