#ifndef __VULKAN_DEVICE_ALLOCATOR_HPP__
#define __VULKAN_DEVICE_ALLOCATOR_HPP__

#include <vk_mem_alloc.h>

#include "Vulkan/Vulkan.hpp"

// ============================== [Data Structures] ============================== //

struct ENGINE_API Buffer
{
    VkBuffer          handle         = VK_NULL_HANDLE;
    VkDeviceSize      size           = 0;
    VmaAllocation     allocation     = VK_NULL_HANDLE;
    VmaAllocationInfo allocationInfo = {};

};  // !struct Buffer

struct ENGINE_API Image
{
    VkImage           handle         = VK_NULL_HANDLE;
    VkImageView       imageView      = VK_NULL_HANDLE;
    uint32            width          = 0u;
    uint32            height         = 0u;
    uint32            depth          = 1u;
    VmaAllocation     allocation     = VK_NULL_HANDLE;
    VmaAllocationInfo allocationInfo = {};

};  // !struct Image

// =============================================================================== //

class ENGINE_API DeviceAllocator : public UniqueObject
{
    public:

    // ============================== [Public Constructor and Destructor] ============================== //

        DeviceAllocator () = delete;

        DeviceAllocator (uint32 p_frameCount);

        ~DeviceAllocator();

    // ============================== [Public Local Methods] ============================== //

        /**
         * @thread_safety This function may be called from any thread.
         */
        void    CreateBuffer    (Buffer&                    p_buffer,
                                 VkBufferCreateInfo const&  p_bufferCI,
                                 VmaAllocationCreateFlags   p_flags,
                                 VmaMemoryUsage             p_usage,
                                 VkMemoryPropertyFlags      p_requiredFlags)    const noexcept;

        /**
         * @thread_safety This function may be called from any thread.
         */
        void    CreateImage     (Image&                     p_image,
                                 VkImageCreateInfo const&   p_imageCI,
                                 VmaAllocationCreateFlags   p_flags,
                                 VmaMemoryUsage             p_usage,
                                 VkMemoryPropertyFlags      p_requiredFlags)    const noexcept;

        /**
         * @thread_safety This function may be called from any thread.
         */
        void    Map             (Buffer&                    p_buffer)           const noexcept;

        /**
         * @thread_safety This function may be called from any thread.
         */
        void    Unmap           (Buffer&                    p_buffer)           const noexcept;

        /**
         * @thread_safety This function may be called from any thread.
         */
        void    DestroyBuffer   (Buffer const&              p_buffer)       const noexcept;

        /**
         * @thread_safety This function may be called from any thread.
         */
        void    DestroyImage    (Image  const&              p_image)        const noexcept;

    private:

    // ============================== [Private Local Properties] ============================== //

        VmaAllocator m_allocator;

};  // !class Allocator

#endif // !__VULKAN_DEVICE_ALLOCATOR_HPP__