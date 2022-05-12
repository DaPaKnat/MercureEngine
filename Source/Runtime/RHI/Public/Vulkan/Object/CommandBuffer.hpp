#ifndef __VULKAN_COMMAND_BUFFER_HPP__
#define __VULKAN_COMMAND_BUFFER_HPP__

#include "Vulkan/Vulkan.hpp"

// ============================== [Forward Declarations] ============================== //

struct Buffer;
struct Image;

// ==================================================================================== //

class ENGINE_API CommandBuffer
{
    public:

    // ============================== [Public Constructors and Destructor] ============================== //

        CommandBuffer           ()                                  = default;

        explicit CommandBuffer  (VkCommandBuffer        p_handle);

        CommandBuffer           (CommandBuffer const&   p_other)    = default;

        CommandBuffer           (CommandBuffer&&        p_other)    = default;

        ~CommandBuffer          ()                                  = default;

    // ============================== [Public Local Methods] ============================== //

        INLINE VkCommandBuffer const GetHandle() const noexcept { return m_handle; }

    // ==================================================================================== //

        /**
         * @thread_safety This function must only be called from the thread owning this object.
         */
        void    Begin               (VkCommandBufferUsageFlags              p_cmdbufferUsage,
                                     VkCommandBufferInheritanceInfo const*  p_inheritanceInfo = nullptr)    const noexcept;

        /**
         * @thread_safety This function must only be called from the thread owning this object.
         */
        void    End                 ()                                                                      const noexcept;

        /**
         * @thread_safety This function must only be called from the thread owning this object.
         */
        void    Reset               (bool                                   p_releaseResources = false)     const noexcept;

        /**
         * @thread_safety This function must only be called from the thread owning this object.
         */
        void    InsertMemoryBarrier (VkPipelineStageFlags                   p_srcStage,
                                     VkPipelineStageFlags                   p_dstStage,
                                     VkMemoryBarrier const&                 p_memoryBarrier)                const noexcept;

        /**
         * @thread_safety This function must only be called from the thread owning this object.
         */
        void    InsertMemoryBarrier (VkPipelineStageFlags                   p_srcStage,
                                     VkPipelineStageFlags                   p_dstStage,
                                     VkBufferMemoryBarrier const&           p_bufferBarrier)                const noexcept;

        /**
         * @thread_safety This function must only be called from the thread owning this object.
         */
        void    InsertMemoryBarrier (VkPipelineStageFlags                   p_srcStage,
                                     VkPipelineStageFlags                   p_dstStage,
                                     VkImageMemoryBarrier const&            p_imageBarrier)                 const noexcept;

        /**
         * @thread_safety This function must only be called from the thread owning this object.
         */
        void    CopyBufferToBuffer  (Buffer const&                          p_srcBuffer,
                                     Buffer const&                          p_dstBuffer)                    const noexcept;

        /**
         * @thread_safety This function must only be called from the thread owning this object.
         */
        void    CopyBufferToImage   (VkImageSubresourceLayers const&        p_subresource,
                                     Buffer                   const&        p_srcBuffer,
                                     Image                    const&        p_dstImage)                     const noexcept;

        /**
         * @thread_safety This function must only be called from the thread owning this object.
         */
        void    CopyImageToBuffer   (VkImageSubresourceLayers const&        p_subresource,
                                     Image                    const&        p_srcImage,
                                     Buffer                   const&        p_dstBuffer)                    const noexcept;

    // ============================== [Public Local Operators] ============================== //

        CommandBuffer& operator=(CommandBuffer const&   p_other) = default;

        CommandBuffer& operator=(CommandBuffer&&        p_other) = default;

    private:

    // ============================== [Private Local Properties] ============================== //

        VkCommandBuffer m_handle;

};  // !class CommandBuffer

#endif // !__VULKAN_COMMAND_BUFFER_HPP__