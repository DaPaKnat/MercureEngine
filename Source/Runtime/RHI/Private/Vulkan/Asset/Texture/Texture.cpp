#include "PCH.hpp"
#include "RHI.hpp"

#include <stb_image.h>

#include "Vulkan/Object/Fence.hpp"

#include "Vulkan/Asset/Texture/Texture.hpp"

// ============================== [Public Constructor] ============================== //

Texture::Texture    (std::string const& p_name,
                     void*              p_pixels,
                     uint32             p_width,
                     uint32             p_height) : Asset(p_name)
{
    m_isPending.store(true, std::memory_order_relaxed);
    
    auto const& device    = RHI::Get().GetDevice   ();
    auto const& allocator = RHI::Get().GetAllocator();

    // Staging buffer.
    Buffer             buffer   = {};
    VkBufferCreateInfo bufferCI = { VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO };

    bufferCI.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
    bufferCI.size  = p_width * p_height * 4;

    allocator->CreateBuffer(buffer, bufferCI, VMA_ALLOCATION_CREATE_MAPPED_BIT, VMA_MEMORY_USAGE_CPU_ONLY, 0u);

    memcpy(buffer.allocationInfo.pMappedData, p_pixels, p_width * p_height * 4);

    // Actual image.
    VkImageCreateInfo imageCI = { VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO };

    imageCI.imageType     = VK_IMAGE_TYPE_2D;
    imageCI.format        = VK_FORMAT_R8G8B8A8_UNORM;
    imageCI.extent.width  = p_width;
    imageCI.extent.height = p_height;
    imageCI.extent.depth  = 1u;
    imageCI.mipLevels     = 1u;
    imageCI.arrayLayers   = 1u;
    imageCI.samples       = VK_SAMPLE_COUNT_1_BIT;
    imageCI.tiling        = VK_IMAGE_TILING_OPTIMAL;
    imageCI.usage         = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;

    allocator->CreateImage(m_image, imageCI, 0u, VMA_MEMORY_USAGE_GPU_ONLY, 0u);

    // Copies data from staging buffer to actual image.
    Fence         fence;
    CommandBuffer cmdBuffer(device->GetGraphicsCommandPool()->AllocateCommandBuffer(VK_COMMAND_BUFFER_LEVEL_PRIMARY));

    cmdBuffer.Begin(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);

    VkImageMemoryBarrier barrier = { VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER };

    barrier.srcAccessMask               = 0u;
    barrier.dstAccessMask               = VK_ACCESS_TRANSFER_WRITE_BIT;
    barrier.oldLayout                   = VK_IMAGE_LAYOUT_UNDEFINED;
    barrier.newLayout                   = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
    barrier.srcQueueFamilyIndex         = VK_QUEUE_FAMILY_IGNORED;
    barrier.dstQueueFamilyIndex         = VK_QUEUE_FAMILY_IGNORED;
    barrier.image                       = m_image.handle;
    barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    barrier.subresourceRange.levelCount = 1u;
    barrier.subresourceRange.layerCount = 1u;

    cmdBuffer.InsertMemoryBarrier(VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, barrier);

    VkImageSubresourceLayers layers = {};

    layers.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    layers.layerCount = 1u;

    cmdBuffer.CopyBufferToImage(layers, buffer, m_image);

    barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
    barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
    barrier.oldLayout     = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
    barrier.newLayout     = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

    cmdBuffer.InsertMemoryBarrier(VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, barrier);

    cmdBuffer.End();

    device->GetGraphicsQueue()->Submit(cmdBuffer.GetHandle(), fence.GetHandle());

    // Waits for the transfer operation to complete before destroying the staging buffer.
    fence.Wait ();
    fence.Reset();

    // Staging buffer is temporary so it needs to be destroyed after usage.
    allocator->DestroyBuffer(buffer);

    // The command buffer can be freed after the transfer operation has been completed.
    device->GetGraphicsCommandPool()->FreeCommandBuffer(cmdBuffer);

    // Actual image's view.
    VkImageViewCreateInfo imageViewCI = { VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO };

    imageViewCI.image                       = m_image.handle;
    imageViewCI.viewType                    = VK_IMAGE_VIEW_TYPE_2D;
    imageViewCI.format                      = VK_FORMAT_R8G8B8A8_UNORM;
    imageViewCI.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    imageViewCI.subresourceRange.levelCount = 1u;
    imageViewCI.subresourceRange.layerCount = 1u;

    VK_CHECK_RESULT(vkCreateImageView(device->GetLogicalDevice(), &imageViewCI, nullptr, &m_image.imageView));

    // Setups debug info.
    Debug::SetImageName    (device->GetLogicalDevice(), m_image.handle,    (m_name + "_Image")    .c_str());
    Debug::SetImageViewName(device->GetLogicalDevice(), m_image.imageView, (m_name + "_ImageView").c_str());

    m_isLoaded .store(true,  std::memory_order_release);
    m_isPending.store(false, std::memory_order_release);
}

// ============================== [Interface Private Local Methods] ============================== //

void    Texture::Deserialize    (std::string const& p_path) noexcept
{
    int32 width, height, channels;

    if (stbi_uc* pixels = stbi_load(p_path.c_str(),
                                    &width,
                                    &height,
                                    &channels,
                                    STBI_rgb_alpha))
    {
        auto const& device    = RHI::Get().GetDevice   ();
        auto const& allocator = RHI::Get().GetAllocator();

        // Staging buffer.
        Buffer             buffer   = {};
        VkBufferCreateInfo bufferCI = { VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO };

        bufferCI.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
        bufferCI.size  = width * height * 4;

        allocator->CreateBuffer(buffer, bufferCI, VMA_ALLOCATION_CREATE_MAPPED_BIT, VMA_MEMORY_USAGE_CPU_ONLY, 0u);

        memcpy(buffer.allocationInfo.pMappedData, pixels, bufferCI.size);

        // Actual image.
        VkImageCreateInfo imageCI = { VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO };

        imageCI.imageType     = VK_IMAGE_TYPE_2D;
        imageCI.format        = VK_FORMAT_R8G8B8A8_UNORM;
        imageCI.extent.width  = static_cast<uint32>(width);
        imageCI.extent.height = static_cast<uint32>(height);
        imageCI.extent.depth  = 1u;
        imageCI.mipLevels     = 1u;
        imageCI.arrayLayers   = 1u;
        imageCI.samples       = VK_SAMPLE_COUNT_1_BIT;
        imageCI.tiling        = VK_IMAGE_TILING_OPTIMAL;
        imageCI.usage         = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;

        allocator->CreateImage(m_image, imageCI, 0u, VMA_MEMORY_USAGE_GPU_ONLY, 0u);

        // Copies data from staging buffer to actual image.
        Fence         fence;
        CommandBuffer cmdBuffer(device->GetGraphicsCommandPool()->AllocateCommandBuffer(VK_COMMAND_BUFFER_LEVEL_PRIMARY));

        cmdBuffer.Begin(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);

        VkImageMemoryBarrier barrier = { VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER };

        barrier.srcAccessMask               = 0u;
        barrier.dstAccessMask               = VK_ACCESS_TRANSFER_WRITE_BIT;
        barrier.oldLayout                   = VK_IMAGE_LAYOUT_UNDEFINED;
        barrier.newLayout                   = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
        barrier.srcQueueFamilyIndex         = VK_QUEUE_FAMILY_IGNORED;
        barrier.dstQueueFamilyIndex         = VK_QUEUE_FAMILY_IGNORED;
        barrier.image                       = m_image.handle;
        barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        barrier.subresourceRange.levelCount = 1u;
        barrier.subresourceRange.layerCount = 1u;

        cmdBuffer.InsertMemoryBarrier(VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, barrier);

        VkImageSubresourceLayers layers = {};

        layers.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        layers.layerCount = 1u;

        cmdBuffer.CopyBufferToImage(layers, buffer, m_image);

        barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
        barrier.oldLayout     = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
        barrier.newLayout     = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

        cmdBuffer.InsertMemoryBarrier(VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, barrier);

        cmdBuffer.End();

        device->GetGraphicsQueue()->Submit(cmdBuffer.GetHandle(), fence.GetHandle());

        // Waits for the transfer operation to complete before destroying the staging buffer.
        fence.Wait ();
        fence.Reset();

        // Staging buffer is temporary so it needs to be destroyed after usage.
        allocator->DestroyBuffer(buffer);

        // The command buffer can be freed after the transfer operation has been completed.
        device->GetGraphicsCommandPool()->FreeCommandBuffer(cmdBuffer);

        // Actual image's view.
        VkImageViewCreateInfo imageViewCI = { VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO };

        imageViewCI.image                       = m_image.handle;
        imageViewCI.viewType                    = VK_IMAGE_VIEW_TYPE_2D;
        imageViewCI.format                      = VK_FORMAT_R8G8B8A8_UNORM;
        imageViewCI.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        imageViewCI.subresourceRange.levelCount = 1u;
        imageViewCI.subresourceRange.layerCount = 1u;

        VK_CHECK_RESULT(vkCreateImageView(device->GetLogicalDevice(), &imageViewCI, nullptr, &m_image.imageView));

        // Setups debug info.
        Debug::SetImageName    (device->GetLogicalDevice(), m_image.handle,    (m_name + "_Image")    .c_str());
        Debug::SetImageViewName(device->GetLogicalDevice(), m_image.imageView, (m_name + "_ImageView").c_str());

        stbi_image_free(pixels);

        m_isLoaded.store(true,  std::memory_order_release);
    }

    else
        LOG(LogAssetManager, Error, "Failed to open %s for deserialization", p_path.c_str());

    m_isPending.store(false, std::memory_order_release);
}

void    Texture::Serialize      (std::string const& p_path) noexcept
{
    RHI::Get().GetAllocator()->DestroyImage(m_image);

    vkDestroyImageView(RHI::Get().GetDevice()->GetLogicalDevice(), m_image.imageView, nullptr);

    m_isLoaded .store(false, std::memory_order_release);
    m_isPending.store(false, std::memory_order_release);
}