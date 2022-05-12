#include "PCH.hpp"
#include "RHI.hpp"
#include "Renderer.hpp"
#include "AssetManager.hpp"

#include "Vulkan/RenderPasses/ShadowPass.hpp"

// ============================== [Public Constructor and Destructor] ============================== //

ShadowPass::ShadowPass  (std::vector<Frame> const& p_frames) noexcept : RenderPass()
{
    #if EDITOR

    // Loads default shaders.
    AssetManager& assetManager = AssetManager::Get();

    assetManager.FindOrAdd((std::string(SOURCE_DIRECTORY) + "Default/Shaders/shadow.vert.glsl")    .c_str(), "Default/Shaders/");
    assetManager.FindOrAdd((std::string(SOURCE_DIRECTORY) + "Default/Shaders/shadow.geom.glsl")    .c_str(), "Default/Shaders/");
    assetManager.FindOrAdd((std::string(SOURCE_DIRECTORY) + "Default/Shaders/shadowomni.vert.glsl").c_str(), "Default/Shaders/");
    assetManager.FindOrAdd((std::string(SOURCE_DIRECTORY) + "Default/Shaders/shadowomni.geom.glsl").c_str(), "Default/Shaders/");
    assetManager.FindOrAdd((std::string(SOURCE_DIRECTORY) + "Default/Shaders/shadowomni.frag.glsl").c_str(), "Default/Shaders/");

    #endif

    SetupRenderPass  (p_frames);
    SetupFramebuffers(p_frames);
    SetupPipelines   (p_frames);
}

ShadowPass::~ShadowPass ()
{
    auto const& device    = RHI::Get().GetDevice   ();
    auto const& allocator = RHI::Get().GetAllocator();

    for (auto const& attachment : m_attachments)
    {
        allocator->DestroyImage(attachment.shadow2D);
        allocator->DestroyImage(attachment.shadowCube);

        vkDestroyImageView(device->GetLogicalDevice(), attachment.shadow2D  .imageView, nullptr);
        vkDestroyImageView(device->GetLogicalDevice(), attachment.shadowCube.imageView, nullptr);

        vkDestroyFramebuffer(device->GetLogicalDevice(), attachment.shadow2DFramebuffer,   nullptr);
        vkDestroyFramebuffer(device->GetLogicalDevice(), attachment.shadowCubeFramebuffer, nullptr);
    }

    vkDestroySampler            (device->GetLogicalDevice(), m_sampler,             nullptr);
    vkDestroyDescriptorPool     (device->GetLogicalDevice(), m_descriptorPool,      nullptr);
    vkDestroyDescriptorSetLayout(device->GetLogicalDevice(), m_descriptorSetLayout, nullptr);
    vkDestroyPipelineLayout     (device->GetLogicalDevice(), m_pipelineLayout,      nullptr);
    vkDestroyPipeline           (device->GetLogicalDevice(), m_shadow2DPipeline,    nullptr);
    vkDestroyPipeline           (device->GetLogicalDevice(), m_shadowCubePipeline,  nullptr);
}

// ============================== [Public Local Methods] ============================== //

void    ShadowPass::Rebuild (std::vector<Frame> const& p_frames) noexcept
{

}

void    ShadowPass::Draw    (Frame const& p_frame) noexcept
{
    VkViewport viewport = {
        0.0f,       // x
        0.0f,       // y
        1024.0f,    // width
        1024.0f,    // height
        0.0f,       // minDepth
        1.0f        // maxDepth
    };

    VkRect2D scissor = {
        0,      // offset.x
        0,      // offset.y
        1024u,  // extent.width
        1024u   // extent.height
    };

    vkCmdSetViewport(p_frame.commandBuffer.GetHandle(), 0u, 1u, &viewport);
    vkCmdSetScissor (p_frame.commandBuffer.GetHandle(), 0u, 1u, &scissor);

    vkCmdBindDescriptorSets(p_frame.commandBuffer.GetHandle(),
                            VK_PIPELINE_BIND_POINT_GRAPHICS,
                            m_pipelineLayout,
                            0u,
                            1u,
                            &m_attachments[p_frame.index].descriptorSet,
                            0u,
                            nullptr);

    VkClearValue const clearValue = { 1.0f, 0u };

    VkRenderPassBeginInfo renderPassBI = { VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO };

    renderPassBI.renderPass               = m_handle;
    renderPassBI.framebuffer              = m_attachments[p_frame.index].shadow2DFramebuffer;
    renderPassBI.renderArea.extent.width  = 1024u;
    renderPassBI.renderArea.extent.height = 1024u;
    renderPassBI.clearValueCount          = 1u;
    renderPassBI.pClearValues             = &clearValue;

    Debug::BeginCmdBufferLabelRegion(p_frame.commandBuffer.GetHandle(), "ShadowPass", Color::Grey);

    // 2D Shadows.
    Debug::BeginCmdBufferLabelRegion(p_frame.commandBuffer.GetHandle(), "2DShadows", Color::Red);
    
    vkCmdBeginRenderPass(p_frame.commandBuffer.GetHandle(), &renderPassBI, VK_SUBPASS_CONTENTS_INLINE);

    if (!p_frame.renderList->spotLights       .empty() ||
        !p_frame.renderList->directionalLights.empty() )
    {
        vkCmdBindPipeline(p_frame.commandBuffer.GetHandle(), VK_PIPELINE_BIND_POINT_GRAPHICS, m_shadow2DPipeline);

        VkDeviceSize offset = 0;

        for (auto const& mesh : p_frame.renderList->opaqueMeshes)
        {
            vkCmdPushConstants(p_frame.commandBuffer.GetHandle(),
                               m_pipelineLayout,
                               VK_SHADER_STAGE_VERTEX_BIT,
                               0u,
                               sizeof(Matrix4x4),
                               &std::get<1>(mesh));

            vkCmdBindVertexBuffers(p_frame.commandBuffer.GetHandle(), 0u, 1u, &std::get<3>(mesh)->vertexBuffer.handle, &offset);

            vkCmdBindIndexBuffer(p_frame.commandBuffer.GetHandle(), std::get<3>(mesh)->indexBuffer.handle, 0u, VK_INDEX_TYPE_UINT32);

            vkCmdDrawIndexed(p_frame.commandBuffer.GetHandle(), std::get<3>(mesh)->indexCount, 1u, 0u, 0u, 0u);
        }
    }

    vkCmdEndRenderPass(p_frame.commandBuffer.GetHandle());

    Debug::EndCmdBufferLabelRegion(p_frame.commandBuffer.GetHandle());

    // Cube Shadows.
    Debug::BeginCmdBufferLabelRegion(p_frame.commandBuffer.GetHandle(), "CubeShadows", Color::Green);

    renderPassBI.framebuffer = m_attachments[p_frame.index].shadowCubeFramebuffer;

    vkCmdBeginRenderPass(p_frame.commandBuffer.GetHandle(), &renderPassBI, VK_SUBPASS_CONTENTS_INLINE);

    if (!p_frame.renderList->pointLights.empty())
    {
        vkCmdBindPipeline(p_frame.commandBuffer.GetHandle(), VK_PIPELINE_BIND_POINT_GRAPHICS, m_shadowCubePipeline);

        VkDeviceSize offset = 0;

        for (auto const& mesh : p_frame.renderList->opaqueMeshes)
        {
            vkCmdPushConstants(p_frame.commandBuffer.GetHandle(),
                               m_pipelineLayout,
                               VK_SHADER_STAGE_VERTEX_BIT,
                               0u,
                               sizeof(Matrix4x4),
                               &std::get<1>(mesh));

            vkCmdBindVertexBuffers(p_frame.commandBuffer.GetHandle(), 0u, 1u, &std::get<3>(mesh)->vertexBuffer.handle, &offset);

            vkCmdBindIndexBuffer(p_frame.commandBuffer.GetHandle(), std::get<3>(mesh)->indexBuffer.handle, 0u, VK_INDEX_TYPE_UINT32);

            vkCmdDrawIndexed(p_frame.commandBuffer.GetHandle(), std::get<3>(mesh)->indexCount, 1u, 0u, 0u, 0u);
        }
    }

    Debug::EndCmdBufferLabelRegion(p_frame.commandBuffer.GetHandle());

    vkCmdEndRenderPass(p_frame.commandBuffer.GetHandle());

    Debug::EndCmdBufferLabelRegion(p_frame.commandBuffer.GetHandle());
}

// ============================== [Protected Local Methods] ============================== //

void    ShadowPass::SetupRenderPass     (std::vector<Frame> const& p_frames) noexcept
{
    auto const& device = RHI::Get().GetDevice();

    std::vector<VkFormat> const formats = {
        VK_FORMAT_D32_SFLOAT,
        VK_FORMAT_D32_SFLOAT_S8_UINT,
        VK_FORMAT_D24_UNORM_S8_UINT,
        VK_FORMAT_D16_UNORM,
        VK_FORMAT_D16_UNORM_S8_UINT
    };

    VkFormat const depthFormat = device->FindSupportedFormat(formats,
                                                             VK_IMAGE_TILING_OPTIMAL,
                                                             VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);

    VkAttachmentDescription attachment = {};

    attachment.format         = depthFormat;
    attachment.samples        = VK_SAMPLE_COUNT_1_BIT;
    attachment.loadOp         = VK_ATTACHMENT_LOAD_OP_CLEAR;
    attachment.storeOp        = VK_ATTACHMENT_STORE_OP_STORE;
    attachment.stencilLoadOp  = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    attachment.initialLayout  = VK_IMAGE_LAYOUT_UNDEFINED;
    attachment.finalLayout    = VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL;

    VkAttachmentReference depthReference = {};

    depthReference.attachment = 0u;
    depthReference.layout     = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpass = {};

    subpass.pipelineBindPoint       = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.pDepthStencilAttachment = &depthReference;

    std::array<VkSubpassDependency, 2> dependencies = {};

    dependencies[0].srcSubpass      = VK_SUBPASS_EXTERNAL;
    dependencies[0].dstSubpass      = 0u;
    dependencies[0].srcStageMask    = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
    dependencies[0].dstStageMask    = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependencies[0].srcAccessMask   = VK_ACCESS_MEMORY_READ_BIT;
    dependencies[0].dstAccessMask   = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
    dependencies[0].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

    dependencies[1].srcSubpass      = 0u;
    dependencies[1].dstSubpass      = VK_SUBPASS_EXTERNAL;
    dependencies[1].srcStageMask    = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependencies[1].dstStageMask    = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
    dependencies[1].srcAccessMask   = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
    dependencies[1].dstAccessMask   = VK_ACCESS_MEMORY_READ_BIT;
    dependencies[1].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

    VkRenderPassCreateInfo renderPassCI = { VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO };

    renderPassCI.attachmentCount = 1u;
    renderPassCI.pAttachments    = &attachment;
    renderPassCI.subpassCount    = 1u;
    renderPassCI.pSubpasses      = &subpass;
    renderPassCI.dependencyCount = static_cast<uint32>(dependencies.size());
    renderPassCI.pDependencies   = dependencies.data();

    VK_CHECK_RESULT(vkCreateRenderPass(device->GetLogicalDevice(), &renderPassCI, nullptr, &m_handle));

    Debug::SetRenderPassName(device->GetLogicalDevice(), m_handle, "ShadowPass");
}

void    ShadowPass::SetupFramebuffers   (std::vector<Frame> const& p_frames) noexcept
{
    auto const& device    = RHI::Get().GetDevice   ();
    auto const& allocator = RHI::Get().GetAllocator();

    VkSamplerCreateInfo samplerCI = { VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO };

    samplerCI.magFilter        = VK_FILTER_LINEAR;
    samplerCI.minFilter        = VK_FILTER_LINEAR;
    samplerCI.mipmapMode       = VK_SAMPLER_MIPMAP_MODE_LINEAR;
    samplerCI.addressModeU     = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
    samplerCI.addressModeV     = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
    samplerCI.addressModeW     = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
    samplerCI.anisotropyEnable = device->GetFeatures  ().samplerAnisotropy;
    samplerCI.maxAnisotropy    = device->GetProperties().limits.maxSamplerAnisotropy;
    samplerCI.compareEnable    = VK_TRUE;
    samplerCI.compareOp        = VK_COMPARE_OP_LESS_OR_EQUAL;
    samplerCI.borderColor      = VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE;

    VK_CHECK_RESULT(vkCreateSampler(device->GetLogicalDevice(), &samplerCI, nullptr, &m_sampler));

    Debug::SetSamplerName(device->GetLogicalDevice(), m_sampler, "Shadow_Sampler");

    std::vector<VkFormat> const formats = {
        VK_FORMAT_D32_SFLOAT,
        VK_FORMAT_D32_SFLOAT_S8_UINT,
        VK_FORMAT_D24_UNORM_S8_UINT,
        VK_FORMAT_D16_UNORM,
        VK_FORMAT_D16_UNORM_S8_UINT
    };

    VkFormat const depthFormat = device->FindSupportedFormat(formats,
                                                             VK_IMAGE_TILING_OPTIMAL,
                                                             VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);

    uint32 const layerCount = (Math::Min(device->GetProperties().limits.maxGeometryOutputVertices,
                                         device->GetProperties().limits.maxGeometryTotalOutputComponents / 6u) / 18u) * 6u;

    m_attachments.resize(p_frames.size());

    for (size_t i = 0; i < m_attachments.size() && i < p_frames.size(); ++i)
    {
        VkImageCreateInfo imageCI = { VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO };

        imageCI.imageType     = VK_IMAGE_TYPE_2D;
        imageCI.format        = depthFormat;
        imageCI.extent.width  = 1024u;
        imageCI.extent.height = 1024u;
        imageCI.extent.depth  = 1u;
        imageCI.mipLevels     = 1u;
        imageCI.arrayLayers   = layerCount;
        imageCI.samples       = VK_SAMPLE_COUNT_1_BIT;
        imageCI.tiling        = VK_IMAGE_TILING_OPTIMAL;
        imageCI.usage         = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;

        allocator->CreateImage(m_attachments[i].shadow2D, imageCI, VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT, VMA_MEMORY_USAGE_GPU_ONLY, 0u);

        imageCI.flags = VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT;

        allocator->CreateImage(m_attachments[i].shadowCube, imageCI, VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT, VMA_MEMORY_USAGE_GPU_ONLY, 0u);

        VkImageViewCreateInfo imageViewCI = { VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO };

        imageViewCI.image                       = m_attachments[i].shadow2D.handle;
        imageViewCI.viewType                    = VK_IMAGE_VIEW_TYPE_2D_ARRAY;
        imageViewCI.format                      = depthFormat;
        imageViewCI.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
        imageViewCI.subresourceRange.levelCount = imageCI.mipLevels;
        imageViewCI.subresourceRange.layerCount = imageCI.arrayLayers;

        VK_CHECK_RESULT(vkCreateImageView(device->GetLogicalDevice(), &imageViewCI, nullptr, &m_attachments[i].shadow2D.imageView));

        imageViewCI.image    = m_attachments[i].shadowCube.handle;
        imageViewCI.viewType = VK_IMAGE_VIEW_TYPE_CUBE_ARRAY;

        VK_CHECK_RESULT(vkCreateImageView(device->GetLogicalDevice(), &imageViewCI, nullptr, &m_attachments[i].shadowCube.imageView));

        Debug::SetImageName    (device->GetLogicalDevice(), m_attachments[i].shadow2D  .handle,    ("Shadow2D_Image_"       + std::to_string(i)).c_str());
        Debug::SetImageName    (device->GetLogicalDevice(), m_attachments[i].shadowCube.handle,    ("ShadowCube_Image_"     + std::to_string(i)).c_str());
        Debug::SetImageViewName(device->GetLogicalDevice(), m_attachments[i].shadow2D  .imageView, ("Shadow2D_ImageView_"   + std::to_string(i)).c_str());
        Debug::SetImageViewName(device->GetLogicalDevice(), m_attachments[i].shadowCube.imageView, ("ShadowCube_ImageView_" + std::to_string(i)).c_str());

        VkFramebufferCreateInfo framebufferCI = { VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO };

        framebufferCI.renderPass      = m_handle;
        framebufferCI.attachmentCount = 1u;
        framebufferCI.pAttachments    = &m_attachments[i].shadow2D.imageView;
        framebufferCI.width           = 1024u;
        framebufferCI.height          = 1024u;
        framebufferCI.layers          = layerCount;

        VK_CHECK_RESULT(vkCreateFramebuffer(device->GetLogicalDevice(), &framebufferCI, nullptr, &m_attachments[i].shadow2DFramebuffer));

        framebufferCI.pAttachments = &m_attachments[i].shadowCube.imageView;

        VK_CHECK_RESULT(vkCreateFramebuffer(device->GetLogicalDevice(), &framebufferCI, nullptr, &m_attachments[i].shadowCubeFramebuffer));

        Debug::SetFramebufferName(device->GetLogicalDevice(), m_attachments[i].shadow2DFramebuffer,   ("Shadow2D_Framebuffer_"   + std::to_string(i)).c_str());
        Debug::SetFramebufferName(device->GetLogicalDevice(), m_attachments[i].shadowCubeFramebuffer, ("ShadowCube_Framebuffer_" + std::to_string(i)).c_str());
    }
}

void    ShadowPass::SetupPipelines      (std::vector<Frame> const& p_frames) noexcept
{
    SetupDescriptorPool     (p_frames);
    SetupDescriptorSetLayout(p_frames);
    SetupDescriptorSets     (p_frames);
    SetupPipelineLayout     (p_frames);
    Setup2DShadowPipeline   (p_frames);
    SetupCubeShadowPipeline (p_frames);
}

// ======================================================================================= //

void    ShadowPass::SetupDescriptorPool                 (std::vector<Frame> const& p_frames) noexcept
{
    VkDevice const device = RHI::Get().GetDevice()->GetLogicalDevice();

    VkDescriptorPoolSize descriptorPoolSize = {};

    descriptorPoolSize.type            = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    descriptorPoolSize.descriptorCount = static_cast<uint32>(3 * p_frames.size());

    VkDescriptorPoolCreateInfo descriptorPoolCI = { VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO };

    descriptorPoolCI.maxSets       = static_cast<uint32>(p_frames.size());
    descriptorPoolCI.poolSizeCount = 1u;
    descriptorPoolCI.pPoolSizes    = &descriptorPoolSize;

    VK_CHECK_RESULT(vkCreateDescriptorPool(device, &descriptorPoolCI, nullptr, &m_descriptorPool));

    Debug::SetDescriptorPoolName(device, m_descriptorPool, "Shadow_DescriptorPool");
}

void    ShadowPass::SetupDescriptorSetLayout            (std::vector<Frame> const& p_frames) noexcept
{
    VkDevice const device = RHI::Get().GetDevice()->GetLogicalDevice();

    std::array<VkDescriptorSetLayoutBinding, 3> descriptorSetLayoutBindings = {};

    descriptorSetLayoutBindings[0].binding         = 0u;
    descriptorSetLayoutBindings[0].descriptorType  = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    descriptorSetLayoutBindings[0].descriptorCount = 1u;
    descriptorSetLayoutBindings[0].stageFlags      = VK_SHADER_STAGE_GEOMETRY_BIT;

    descriptorSetLayoutBindings[1].binding         = 1u;
    descriptorSetLayoutBindings[1].descriptorType  = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    descriptorSetLayoutBindings[1].descriptorCount = 1u;
    descriptorSetLayoutBindings[1].stageFlags      = VK_SHADER_STAGE_GEOMETRY_BIT;

    descriptorSetLayoutBindings[2].binding         = 2u;
    descriptorSetLayoutBindings[2].descriptorType  = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    descriptorSetLayoutBindings[2].descriptorCount = 1u;
    descriptorSetLayoutBindings[2].stageFlags      = VK_SHADER_STAGE_GEOMETRY_BIT;

    VkDescriptorSetLayoutCreateInfo descriptorSetLayoutCI = { VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO };

    descriptorSetLayoutCI.bindingCount = static_cast<uint32>(descriptorSetLayoutBindings.size());
    descriptorSetLayoutCI.pBindings    = descriptorSetLayoutBindings.data();

    VK_CHECK_RESULT(vkCreateDescriptorSetLayout(device, &descriptorSetLayoutCI, nullptr, &m_descriptorSetLayout));

    Debug::SetDescriptorSetLayoutName(device, m_descriptorSetLayout, "Shadow_DescriptorSetLayout");
}

void    ShadowPass::SetupDescriptorSets        (std::vector<Frame> const& p_frames) noexcept
{
    VkDevice const device = RHI::Get().GetDevice()->GetLogicalDevice();

    for (size_t i = 0; i < p_frames.size(); ++i)
    {
        VkDescriptorSetAllocateInfo decriptorSetAI = { VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO };

        decriptorSetAI.descriptorPool     = m_descriptorPool;
        decriptorSetAI.descriptorSetCount = 1u;
        decriptorSetAI.pSetLayouts        = &m_descriptorSetLayout;

        VK_CHECK_RESULT(vkAllocateDescriptorSets(device, &decriptorSetAI, &m_attachments[i].descriptorSet));

        Debug::SetDescriptorSetName(device, m_attachments[i].descriptorSet, ("Shadow_DescriptorSet_" + std::to_string(i)).c_str());

        std::array<VkDescriptorBufferInfo, 3> bufferInfos = {};

        bufferInfos[0].buffer = p_frames[i].ubos.spotLight.handle;
        bufferInfos[0].range  = p_frames[i].ubos.spotLight.size;

        bufferInfos[1].buffer = p_frames[i].ubos.pointLight.handle;
        bufferInfos[1].range  = p_frames[i].ubos.pointLight.size;

        bufferInfos[2].buffer = p_frames[i].ubos.directionalLight.handle;
        bufferInfos[2].range  = p_frames[i].ubos.directionalLight.size;

        std::array<VkWriteDescriptorSet, 3> writeSets = {};

        writeSets[0].sType           = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        writeSets[0].dstSet          = m_attachments[i].descriptorSet;
        writeSets[0].dstBinding      = 0u;
        writeSets[0].descriptorCount = 1u;
        writeSets[0].descriptorType  = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        writeSets[0].pBufferInfo     = &bufferInfos[0];

        writeSets[1].sType           = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        writeSets[1].dstSet          = m_attachments[i].descriptorSet;
        writeSets[1].dstBinding      = 1u;
        writeSets[1].descriptorCount = 1u;
        writeSets[1].descriptorType  = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        writeSets[1].pBufferInfo     = &bufferInfos[1];

        writeSets[2].sType           = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        writeSets[2].dstSet          = m_attachments[i].descriptorSet;
        writeSets[2].dstBinding      = 2u;
        writeSets[2].descriptorCount = 1u;
        writeSets[2].descriptorType  = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        writeSets[2].pBufferInfo     = &bufferInfos[2];

        vkUpdateDescriptorSets(device, static_cast<uint32>(writeSets.size()), writeSets.data(), 0u, nullptr);
    }
}

void    ShadowPass::SetupPipelineLayout                 (std::vector<Frame> const& p_frames) noexcept
{
    VkDevice const device = RHI::Get().GetDevice()->GetLogicalDevice();

    VkPushConstantRange pushConstant = {};

    pushConstant.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
    pushConstant.offset     = 0u;
    pushConstant.size       = sizeof(Matrix4x4);

    VkPipelineLayoutCreateInfo pipelineLayoutCI = { VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO };

    pipelineLayoutCI.setLayoutCount         = 1u;
    pipelineLayoutCI.pSetLayouts            = &m_descriptorSetLayout;
    pipelineLayoutCI.pushConstantRangeCount = 1u;
    pipelineLayoutCI.pPushConstantRanges    = &pushConstant;

    VK_CHECK_RESULT(vkCreatePipelineLayout(device, &pipelineLayoutCI, nullptr, &m_pipelineLayout));

    Debug::SetPipelineLayoutName(device, m_pipelineLayout, "Shadow_PipelineLayout");
}

void    ShadowPass::Setup2DShadowPipeline               (std::vector<Frame> const& p_frames) noexcept
{
    auto const& device = RHI::Get().GetDevice       ();
    auto const& cache  = RHI::Get().GetPipelineCache();

    std::array<VkPipelineShaderStageCreateInfo, 2> shaderStages = {};

    shaderStages[0].sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    shaderStages[0].stage  = VK_SHADER_STAGE_VERTEX_BIT;
    shaderStages[0].module = AssetManager::Get().Get<Shader>("Default/Shaders/shadow.vert", ELoadingMode::BLOCKING)->GetModule();
    shaderStages[0].pName  = "main";

    shaderStages[1].sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    shaderStages[1].stage  = VK_SHADER_STAGE_GEOMETRY_BIT;
    shaderStages[1].module = AssetManager::Get().Get<Shader>("Default/Shaders/shadow.geom", ELoadingMode::BLOCKING)->GetModule();
    shaderStages[1].pName  = "main";

    VkVertexInputBindingDescription                  vertexInputBinding    = Vertex::GetBindingDescription   ();
    std::array<VkVertexInputAttributeDescription, 4> vertexInputAttributes = Vertex::GetAttributeDescriptions();

    VkPipelineVertexInputStateCreateInfo vertexInputStateCI = { VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO };

    vertexInputStateCI.vertexBindingDescriptionCount   = 1u;
    vertexInputStateCI.pVertexBindingDescriptions      = &vertexInputBinding;
    vertexInputStateCI.vertexAttributeDescriptionCount = static_cast<uint32>(vertexInputAttributes.size());
    vertexInputStateCI.pVertexAttributeDescriptions    = vertexInputAttributes.data();

    VkPipelineInputAssemblyStateCreateInfo inputAssemblyStateCI = { VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO };

    inputAssemblyStateCI.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;

    VkPipelineViewportStateCreateInfo viewportStateCI = { VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO };

    viewportStateCI.viewportCount = 1u;
    viewportStateCI.scissorCount  = 1u;

    VkPipelineRasterizationStateCreateInfo rasterizationStateCI = { VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO };

    rasterizationStateCI.polygonMode = VK_POLYGON_MODE_FILL;
    rasterizationStateCI.cullMode    = VK_CULL_MODE_FRONT_BIT;
    rasterizationStateCI.frontFace   = VK_FRONT_FACE_CLOCKWISE;
    rasterizationStateCI.lineWidth   = 1.0f;

    VkPipelineMultisampleStateCreateInfo multisampleStateCI = { VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO };

    multisampleStateCI.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

    VkPipelineDepthStencilStateCreateInfo depthStencilStateCI = { VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO };

    depthStencilStateCI.depthTestEnable  = VK_TRUE;
    depthStencilStateCI.depthWriteEnable = VK_TRUE;
    depthStencilStateCI.depthCompareOp   = VK_COMPARE_OP_LESS_OR_EQUAL;

    std::array<VkDynamicState, 2> dynamicStates = {
        VK_DYNAMIC_STATE_VIEWPORT,
        VK_DYNAMIC_STATE_SCISSOR,
    };

    VkPipelineDynamicStateCreateInfo dynamicStateCI = { VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO };

    dynamicStateCI.dynamicStateCount = static_cast<uint32>(dynamicStates.size());
    dynamicStateCI.pDynamicStates    = dynamicStates.data();

    VkGraphicsPipelineCreateInfo pipelineCI = { VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO };

    pipelineCI.stageCount          = static_cast<uint32>(shaderStages.size());
    pipelineCI.pStages             = shaderStages.data();
    pipelineCI.pVertexInputState   = &vertexInputStateCI;
    pipelineCI.pInputAssemblyState = &inputAssemblyStateCI;
    pipelineCI.pTessellationState  = nullptr;
    pipelineCI.pViewportState      = &viewportStateCI;
    pipelineCI.pRasterizationState = &rasterizationStateCI;
    pipelineCI.pMultisampleState   = &multisampleStateCI;
    pipelineCI.pDepthStencilState  = &depthStencilStateCI;
    pipelineCI.pColorBlendState    = nullptr;
    pipelineCI.pDynamicState       = &dynamicStateCI;
    pipelineCI.layout              = m_pipelineLayout;
    pipelineCI.renderPass          = m_handle;
    pipelineCI.subpass             = 0u;

    uint32 const maxTriangles = (Math::Min(device->GetProperties().limits.maxGeometryOutputVertices,
                                           device->GetProperties().limits.maxGeometryTotalOutputComponents / 6u) / 18u) * 6u;

    std::array<VkSpecializationMapEntry, 2> entries = {};

    entries[0].constantID = 0u;
    entries[1].constantID = 1u;

    entries[0].offset = 0u * sizeof(uint32);
    entries[1].offset = 1u * sizeof(uint32);

    entries[0].size = sizeof(uint32);
    entries[1].size = sizeof(uint32);

    std::array<uint32, 2> lightCounts = {
        maxTriangles / 2u,
        maxTriangles / 2u
    };

    VkSpecializationInfo info = {};

    info.mapEntryCount = static_cast<uint32>(entries.size());
    info.pMapEntries   = entries.data();
    info.dataSize      = sizeof(lightCounts);
    info.pData         = lightCounts.data();

    shaderStages[1].pSpecializationInfo = &info;

    VK_CHECK_RESULT(vkCreateGraphicsPipelines(device->GetLogicalDevice(),
                                              cache ->GetHandle       (),
                                              1u,
                                              &pipelineCI,
                                              nullptr,
                                              &m_shadow2DPipeline));

    Debug::SetPipelineName(device->GetLogicalDevice(), m_shadow2DPipeline, "Shadow2D_Pipeline");
}

void    ShadowPass::SetupCubeShadowPipeline            (std::vector<Frame> const& p_frames) noexcept
{
    auto const& device = RHI::Get().GetDevice       ();
    auto const& cache  = RHI::Get().GetPipelineCache();

    std::array<VkPipelineShaderStageCreateInfo, 3> shaderStages = {};

    shaderStages[0].sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    shaderStages[0].stage  = VK_SHADER_STAGE_VERTEX_BIT;
    shaderStages[0].module = AssetManager::Get().Get<Shader>("Default/Shaders/shadowomni.vert", ELoadingMode::BLOCKING)->GetModule();
    shaderStages[0].pName  = "main";

    shaderStages[1].sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    shaderStages[1].stage  = VK_SHADER_STAGE_GEOMETRY_BIT;
    shaderStages[1].module = AssetManager::Get().Get<Shader>("Default/Shaders/shadowomni.geom", ELoadingMode::BLOCKING)->GetModule();
    shaderStages[1].pName  = "main";

    shaderStages[2].sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    shaderStages[2].stage  = VK_SHADER_STAGE_FRAGMENT_BIT;
    shaderStages[2].module = AssetManager::Get().Get<Shader>("Default/Shaders/shadowomni.frag", ELoadingMode::BLOCKING)->GetModule();
    shaderStages[2].pName  = "main";

    VkVertexInputBindingDescription                  vertexInputBinding    = Vertex::GetBindingDescription   ();
    std::array<VkVertexInputAttributeDescription, 4> vertexInputAttributes = Vertex::GetAttributeDescriptions();

    VkPipelineVertexInputStateCreateInfo vertexInputStateCI = { VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO };

    vertexInputStateCI.vertexBindingDescriptionCount   = 1u;
    vertexInputStateCI.pVertexBindingDescriptions      = &vertexInputBinding;
    vertexInputStateCI.vertexAttributeDescriptionCount = static_cast<uint32>(vertexInputAttributes.size());
    vertexInputStateCI.pVertexAttributeDescriptions    = vertexInputAttributes.data();

    VkPipelineInputAssemblyStateCreateInfo inputAssemblyStateCI = { VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO };

    inputAssemblyStateCI.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;

    VkPipelineViewportStateCreateInfo viewportStateCI = { VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO };

    viewportStateCI.viewportCount = 1u;
    viewportStateCI.scissorCount  = 1u;

    VkPipelineRasterizationStateCreateInfo rasterizationStateCI = { VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO };

    rasterizationStateCI.polygonMode = VK_POLYGON_MODE_FILL;
    rasterizationStateCI.cullMode    = VK_CULL_MODE_FRONT_BIT;
    rasterizationStateCI.frontFace   = VK_FRONT_FACE_CLOCKWISE;
    rasterizationStateCI.lineWidth   = 1.0f;

    VkPipelineMultisampleStateCreateInfo multisampleStateCI = { VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO };

    multisampleStateCI.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

    VkPipelineDepthStencilStateCreateInfo depthStencilStateCI = { VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO };

    depthStencilStateCI.depthTestEnable  = VK_TRUE;
    depthStencilStateCI.depthWriteEnable = VK_TRUE;
    depthStencilStateCI.depthCompareOp   = VK_COMPARE_OP_LESS_OR_EQUAL;

    std::array<VkDynamicState, 2> dynamicStates = {
        VK_DYNAMIC_STATE_VIEWPORT,
        VK_DYNAMIC_STATE_SCISSOR,
    };

    VkPipelineDynamicStateCreateInfo dynamicStateCI = { VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO };

    dynamicStateCI.dynamicStateCount = static_cast<uint32>(dynamicStates.size());
    dynamicStateCI.pDynamicStates    = dynamicStates.data();

    VkGraphicsPipelineCreateInfo pipelineCI = { VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO };

    pipelineCI.stageCount          = static_cast<uint32>(shaderStages.size());
    pipelineCI.pStages             = shaderStages.data();
    pipelineCI.pVertexInputState   = &vertexInputStateCI;
    pipelineCI.pInputAssemblyState = &inputAssemblyStateCI;
    pipelineCI.pTessellationState  = nullptr;
    pipelineCI.pViewportState      = &viewportStateCI;
    pipelineCI.pRasterizationState = &rasterizationStateCI;
    pipelineCI.pMultisampleState   = &multisampleStateCI;
    pipelineCI.pDepthStencilState  = &depthStencilStateCI;
    pipelineCI.pColorBlendState    = nullptr;
    pipelineCI.pDynamicState       = &dynamicStateCI;
    pipelineCI.layout              = m_pipelineLayout;
    pipelineCI.renderPass          = m_handle;
    pipelineCI.subpass             = 0u;

    uint32 const maxTriangles = Math::Min(device->GetProperties().limits.maxGeometryOutputVertices,
                                          device->GetProperties().limits.maxGeometryTotalOutputComponents / 6u) / 18u;

    VkSpecializationMapEntry entry = {};

    entry.constantID = 0u;
    entry.offset     = 0u;
    entry.size       = sizeof(uint32);

    VkSpecializationInfo info = {};

    info.mapEntryCount = 1u;
    info.pMapEntries   = &entry;
    info.dataSize      = sizeof(maxTriangles);
    info.pData         = &maxTriangles;

    shaderStages[1].pSpecializationInfo = &info;

    VK_CHECK_RESULT(vkCreateGraphicsPipelines(device->GetLogicalDevice(),
                                              cache ->GetHandle       (),
                                              1u,
                                              &pipelineCI,
                                              nullptr,
                                              &m_shadowCubePipeline));

    Debug::SetPipelineName(device->GetLogicalDevice(), m_shadowCubePipeline, "ShadowCube_Pipeline");
}