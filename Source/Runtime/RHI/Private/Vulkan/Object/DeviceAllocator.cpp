#include "PCH.hpp"

#define VMA_IMPLEMENTATION

#include "RHI.hpp"

// ============================== [Public Constructor and Destructor] ============================== //

DeviceAllocator::DeviceAllocator    (uint32 p_frameCount)
{
    VmaAllocatorCreateInfo allocatorCI = {};

    allocatorCI.physicalDevice  = RHI::Get().GetDevice()->GetPhysicalDevice();
    allocatorCI.device          = RHI::Get().GetDevice()->GetLogicalDevice ();
    allocatorCI.frameInUseCount = p_frameCount - 1u;

    VK_CHECK_RESULT(vmaCreateAllocator(&allocatorCI, &m_allocator));

    LOG(LogRHI, Display, "Created : DeviceAllocator");
}

DeviceAllocator::~DeviceAllocator   ()
{
    vmaDestroyAllocator(m_allocator);

    LOG(LogRHI, Display, "Destroyed : DeviceAllocator");
}

// ============================== [Public Local Methods] ============================== //

void    DeviceAllocator::CreateBuffer   (Buffer&                   p_buffer,
                                         VkBufferCreateInfo const& p_bufferCI,
                                         VmaAllocationCreateFlags  p_flags,
                                         VmaMemoryUsage            p_usage,
                                         VkMemoryPropertyFlags     p_requiredFlags) const noexcept
{
    VmaAllocationCreateInfo allocationCI = {};

    allocationCI.flags         = p_flags;
    allocationCI.usage         = p_usage;
    allocationCI.requiredFlags = p_requiredFlags;

    VK_CHECK_RESULT(vmaCreateBuffer(m_allocator,
                                    &p_bufferCI,
                                    &allocationCI,
                                    &p_buffer.handle,
                                    &p_buffer.allocation,
                                    &p_buffer.allocationInfo));

    p_buffer.size = p_bufferCI.size;
}

void    DeviceAllocator::CreateImage    (Image&                   p_image,
                                         VkImageCreateInfo const& p_imageCI,
                                         VmaAllocationCreateFlags p_flags,
                                         VmaMemoryUsage           p_usage,
                                         VkMemoryPropertyFlags    p_requiredFlags) const noexcept
{
    VmaAllocationCreateInfo allocationCI = {};

    allocationCI.flags         = p_flags;
    allocationCI.usage         = p_usage;
    allocationCI.requiredFlags = p_requiredFlags;

    VK_CHECK_RESULT(vmaCreateImage(m_allocator,
                                   &p_imageCI,
                                   &allocationCI,
                                   &p_image.handle,
                                   &p_image.allocation,
                                   &p_image.allocationInfo));

    p_image.width  = p_imageCI.extent.width;
    p_image.height = p_imageCI.extent.height;
    p_image.depth  = p_imageCI.extent.depth;
}

void    DeviceAllocator::Map            (Buffer& p_buffer) const noexcept
{
    vmaMapMemory(m_allocator, p_buffer.allocation, &p_buffer.allocationInfo.pMappedData);
}

void    DeviceAllocator::Unmap          (Buffer& p_buffer) const noexcept
{
    vmaUnmapMemory(m_allocator, p_buffer.allocation);
}

void    DeviceAllocator::DestroyBuffer  (Buffer const& p_buffer) const noexcept
{
    vmaDestroyBuffer(m_allocator, p_buffer.handle, p_buffer.allocation);
}

void    DeviceAllocator::DestroyImage   (Image const& p_image) const noexcept
{
    vmaDestroyImage(m_allocator, p_image.handle, p_image.allocation);
}