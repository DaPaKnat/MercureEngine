#ifndef __VULKAN_COMMAND_POOL_HPP__
#define __VULKAN_COMMAND_POOL_HPP__

#include "CommandBuffer.hpp"

class ENGINE_API CommandPool : public UniqueObject
{
    public:

    // ============================== [Public Constructors and Destructor] ============================== //

        CommandPool         () = delete;

        explicit CommandPool(uint32         p_queueFamily,
                             VkCommandPool  p_handle);

        ~CommandPool        () = default;

    // ============================== [Public Local Methods] ============================== //

        /**
         * @thread_safety This function may be called from any thread.
         */
        CommandBuffer   AllocateCommandBuffer   (VkCommandBufferLevel   p_cmdBufferLevel)   const noexcept;

        /**
         * @thread_safety This function may be called from any thread.
         */
        void            FreeCommandBuffer       (CommandBuffer const&   p_cmdBuffer)        const noexcept;

    private:

    // ============================== [Private Local Properties] ============================== //

        uint32          m_queueFamily;

        VkCommandPool   m_handle;

    // ============================== [Friend Class] ============================== //

        friend class Device;

};  // !class CommandPool

#endif // !__VULKAN_COMMAND_POOL_HPP__