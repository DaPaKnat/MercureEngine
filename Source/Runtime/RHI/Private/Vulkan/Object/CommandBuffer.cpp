#include "PCH.hpp"
#include "RHI.hpp"

#include "Vulkan/Object/CommandPool.hpp"
#include "Vulkan/Object/CommandBuffer.hpp"

// ============================== [Public Constructor] ============================== //

CommandBuffer::CommandBuffer    (VkCommandBuffer p_handle) :
    m_handle { p_handle }
{

}

// ============================== [Public Local Methods] ============================== //

void    CommandBuffer::Begin                (VkCommandBufferUsageFlags             p_cmdbufferUsage,
                                             VkCommandBufferInheritanceInfo const* p_inheritanceInfo) const noexcept
{
    VkCommandBufferBeginInfo cmdBufferBI = { VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO };

    cmdBufferBI.flags            = p_cmdbufferUsage;
    cmdBufferBI.pInheritanceInfo = p_inheritanceInfo;

    VK_CHECK_RESULT(vkBeginCommandBuffer(m_handle, &cmdBufferBI));
}

void    CommandBuffer::End                  () const noexcept
{
    VK_CHECK_RESULT(vkEndCommandBuffer(m_handle));
}

void    CommandBuffer::Reset                (bool p_releaseResources) const noexcept
{
    VK_CHECK_RESULT(vkResetCommandBuffer(m_handle, p_releaseResources ? VK_COMMAND_BUFFER_RESET_RELEASE_RESOURCES_BIT : 0u));
}

void    CommandBuffer::InsertMemoryBarrier  (VkPipelineStageFlags   p_srcStage,
                                             VkPipelineStageFlags   p_dstStage,
                                             VkMemoryBarrier const& p_memoryBarrier) const noexcept
{
    vkCmdPipelineBarrier(
        m_handle,
        p_srcStage,
        p_dstStage,
        0u,
        1u,
        &p_memoryBarrier,
        0u,
        nullptr,
        0u,
        nullptr
    );
}

void    CommandBuffer::InsertMemoryBarrier  (VkPipelineStageFlags         p_srcStage,
                                             VkPipelineStageFlags         p_dstStage,
                                             VkBufferMemoryBarrier const& p_bufferBarrier) const noexcept
{
    vkCmdPipelineBarrier(
        m_handle,
        p_srcStage,
        p_dstStage,
        0u,
        0u,
        nullptr,
        1u,
        &p_bufferBarrier,
        0u,
        nullptr
    );
}

void    CommandBuffer::InsertMemoryBarrier  (VkPipelineStageFlags        p_srcStage,
                                             VkPipelineStageFlags        p_dstStage,
                                             VkImageMemoryBarrier const& p_imageBarrier) const noexcept
{
    vkCmdPipelineBarrier(
        m_handle,
        p_srcStage,
        p_dstStage,
        0u,
        0u,
        nullptr,
        0u,
        nullptr,
        1u,
        &p_imageBarrier
    );
}

void    CommandBuffer::CopyBufferToBuffer   (Buffer const& p_srcBuffer,
                                             Buffer const& p_dstBuffer) const noexcept
{
    // Structure specifying a buffer copy operation.
    VkBufferCopy region = {};

    region.size = p_dstBuffer.size;

    vkCmdCopyBuffer(m_handle, p_srcBuffer.handle, p_dstBuffer.handle, 1u, &region);
}

void    CommandBuffer::CopyBufferToImage    (VkImageSubresourceLayers const& p_subresource,
                                             Buffer                   const& p_srcBuffer,
                                             Image                    const& p_dstImage) const noexcept
{
    VkBufferImageCopy region = {};

    region.imageSubresource   = p_subresource;
    region.imageExtent.width  = p_dstImage.width;
    region.imageExtent.height = p_dstImage.height;
    region.imageExtent.depth  = p_dstImage.depth;

    vkCmdCopyBufferToImage(m_handle, p_srcBuffer.handle, p_dstImage.handle, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1u, &region);
}

void    CommandBuffer::CopyImageToBuffer    (VkImageSubresourceLayers const& p_subresource,
                                             Image                    const& p_srcImage,
                                             Buffer                   const& p_dstBuffer) const noexcept
{
    VkBufferImageCopy region = {};

    region.imageSubresource   = p_subresource;
    region.imageExtent.width  = p_srcImage.width;
    region.imageExtent.height = p_srcImage.height;
    region.imageExtent.depth  = p_srcImage.depth;

    vkCmdCopyImageToBuffer(m_handle, p_srcImage.handle, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, p_dstBuffer.handle, 1u, &region);
}