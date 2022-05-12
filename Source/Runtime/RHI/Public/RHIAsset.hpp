#ifndef __RHI_ASSET_HPP__
#define __RHI_ASSET_HPP__

#if defined(VULKAN)
    #include "Vulkan/Asset/Model/Model.hpp"
    #include "Vulkan/Asset/Shader/Shader.hpp"
    #include "Vulkan/Asset/Texture/Texture.hpp"
    #include "Vulkan/Asset/Material/Material.hpp"
    #include "Vulkan/Asset/Material/MaterialInstance.hpp"
#else
    #error UNKNOWN GRAPHICS API
#endif

#endif // !__RHI_ASSET_HPP__