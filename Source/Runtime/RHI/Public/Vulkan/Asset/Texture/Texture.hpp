#ifndef __VULKAN_TEXTURE_HPP__
#define __VULKAN_TEXTURE_HPP__

#include "Asset.hpp"

#include "Vulkan/Object/DeviceAllocator.hpp"

class ENGINE_API Texture : public Asset
{
    public:

    // ============================== [Public Constructors and Destructor] ============================== //
        
        Texture () = delete;

        Texture (std::string const& p_name) : Asset(p_name) {};

        Texture (std::string const& p_name,
                 void*              p_pixels,
                 uint32             p_width,
                 uint32             p_height);

        ~Texture() = default;

    // ============================== [Public Local Methods] ============================== //

        INLINE Image const& GetImage() const noexcept { return m_image; }

    private:

    // ============================== [Private Local Properties] ============================== //

        Image m_image;

    // ============================== [Interface Private Local Methods] ============================== //

        void    Deserialize (std::string const& p_path) noexcept final override;

        void    Serialize   (std::string const& p_path) noexcept final override;

};  // !class Texture

#endif // !__VULKAN_TEXTURE_HPP__