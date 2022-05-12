#ifndef __ASSET_HPP__
#define __ASSET_HPP__

#include "CoreMinimal.hpp"

// ============================== [Global Enum] ============================== //

enum class EAssetType : uint8
{
    MODEL,
    SHADER,
    MATERIAL,
    MATERIAL_INSTANCE,
    EMPTY

};  // !enum class EAssetType

// =========================================================================== //

class ENGINE_API BASE Asset : public UniqueObject
{
    REFLECT(Asset)

    public:

    // ============================== [Public Constructor and Destructor] ============================== //
        
        Asset           () = delete;

        Asset           (std::string const& p_name) : m_name { p_name } {};

        virtual ~Asset  () = default;

    // ============================== [Public Local Methods] ============================== //

        /**
         * @thread_safety This function must only be called from the main thread.
         */
        INLINE std::string const&   GetName ()  const noexcept  { return m_name; }

        /**
         * @thread_safety This function may be called from any thread.
         */
        INLINE bool                 IsValid ()  const noexcept  { return m_isLoaded .load(std::memory_order_acquire)  &&
                                                                        !m_isPending.load(std::memory_order_acquire); }
    protected:

    // ============================== [Protected Local Properties] ============================== //

        PROPERTY()
        std::string         m_name;

        std::atomic_bool    m_isPending = false;

        std::atomic_bool    m_isLoaded  = false;

    // ============================== [Virtual Protected Local Methods] ============================== //

        virtual void    Deserialize (std::string const& p_path) = 0;

        virtual void    Serialize   (std::string const& p_path) = 0;

    // ============================== [Friend Class] ============================== //

        friend class AssetManager;

};  // !class Asset

#include "Asset.generated.hpp"

#endif // !__ASSET_HPP__