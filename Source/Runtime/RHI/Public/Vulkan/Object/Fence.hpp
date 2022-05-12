#ifndef __VULKAN_FENCE_HPP__
#define __VULKAN_FENCE_HPP__

#include "Vulkan/Vulkan.hpp"

class ENGINE_API Fence
{
    public:

    // ============================== [Public Constructor and Destructor] ============================== //

        Fence   (VkFenceCreateFlags p_flags = 0u);

        Fence   (Fence const&       p_other) = default;

        Fence   (Fence&&            p_other) = default;

        ~Fence  ();

    // ============================== [Public Local Methods] ============================== //

        void    Reset   ()  const noexcept;

        void    Wait    ()  const noexcept;

    // ==================================================================================== //

        INLINE VkFence const GetHandle() const noexcept { return m_handle; }

    // ============================== [Public Local Operators] ============================== //

        Fence& operator=(Fence const&   p_other) = delete;

        Fence& operator=(Fence&&        p_other) = delete;

    private:

    // ============================== [Private Local Properties] ============================== //

        VkFence m_handle;

};  // !class Fence

#endif // !__VULKAN_FENCE_HPP__