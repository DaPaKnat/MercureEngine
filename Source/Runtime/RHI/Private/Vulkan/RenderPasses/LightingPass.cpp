#include "PCH.hpp"
#include "RHI.hpp"
#include "Renderer.hpp"
#include "AssetManager.hpp"

#include "Vulkan/RenderPasses/ShadowPass.hpp"
#include "Vulkan/RenderPasses/LightingPass.hpp"

// ============================== [Public Constructor and Destructor] ============================== //

LightingPass::LightingPass  (std::vector<Frame> const& p_frames) : RenderPass()
{
    #if EDITOR

    // Loads default shaders.
    AssetManager& assetManager = AssetManager::Get();

    assetManager.FindOrAdd((std::string(SOURCE_DIRECTORY) + "Default/Shaders/gbuffer.vert.glsl")    .c_str(), "Default/Shaders/");
    assetManager.FindOrAdd((std::string(SOURCE_DIRECTORY) + "Default/Shaders/gbuffer.frag.glsl")    .c_str(), "Default/Shaders/");
    assetManager.FindOrAdd((std::string(SOURCE_DIRECTORY) + "Default/Shaders/fullscreen.vert.glsl") .c_str(), "Default/Shaders/");
    assetManager.FindOrAdd((std::string(SOURCE_DIRECTORY) + "Default/Shaders/composition.frag.glsl").c_str(), "Default/Shaders/");
    assetManager.FindOrAdd((std::string(SOURCE_DIRECTORY) + "Default/Shaders/transparent.vert.glsl").c_str(), "Default/Shaders/");
    assetManager.FindOrAdd((std::string(SOURCE_DIRECTORY) + "Default/Shaders/transparent.frag.glsl").c_str(), "Default/Shaders/");

    #endif

    SetupRenderPass  (p_frames);
    SetupFramebuffers(p_frames);
    SetupPipelines   (p_frames);
}

LightingPass::~LightingPass ()
{
    auto const& device    = RHI::Get().GetDevice   ();
    auto const& allocator = RHI::Get().GetAllocator();

    for (auto const& attachment : m_attachments)
    {
        allocator->DestroyImage(attachment.depth);
        allocator->DestroyImage(attachment.position);
        allocator->DestroyImage(attachment.normal);
        allocator->DestroyImage(attachment.albedo);
        allocator->DestroyImage(attachment.composition);

        vkDestroyImageView(device->GetLogicalDevice(), attachment.depth      .imageView, nullptr);
        vkDestroyImageView(device->GetLogicalDevice(), attachment.position   .imageView, nullptr);
        vkDestroyImageView(device->GetLogicalDevice(), attachment.normal     .imageView, nullptr);
        vkDestroyImageView(device->GetLogicalDevice(), attachment.albedo     .imageView, nullptr);
        vkDestroyImageView(device->GetLogicalDevice(), attachment.composition.imageView, nullptr);

        vkDestroyFramebuffer(device->GetLogicalDevice(), attachment.framebuffer, nullptr);
    }

    vkDestroyDescriptorPool     (device->GetLogicalDevice(), m_descriptorPool,      nullptr);
    vkDestroyDescriptorSetLayout(device->GetLogicalDevice(), m_descriptorSetLayout, nullptr);
    vkDestroyPipelineLayout     (device->GetLogicalDevice(), m_pipelineLayout,      nullptr);
    vkDestroyPipeline           (device->GetLogicalDevice(), m_pipeline,            nullptr);
}

// ============================== [Public Local Methods] ============================== //

void    LightingPass::Rebuild (std::vector<Frame> const& p_frames) noexcept
{
    
}

void    LightingPass::Draw    (Frame const& p_frame) noexcept
{
    VkViewport viewport = {
        0.0f,                                       // x
        0.0f,                                       // y
        static_cast<float>(p_frame.result.width),   // width
        static_cast<float>(p_frame.result.height),  // height
        0.0f,                                       // minDepth
        1.0f                                        // maxDepth
    };

    VkRect2D scissor = {
        0,                      // offset.x
        0,                      // offset.y
        p_frame.result.width,   // extent.width
        p_frame.result.height   // extent.height
    };

    vkCmdSetViewport(p_frame.commandBuffer.GetHandle(), 0u, 1u, &viewport);
    vkCmdSetScissor (p_frame.commandBuffer.GetHandle(), 0u, 1u, &scissor);

    std::array<VkClearValue, 5> const clearValues = {
        VkClearValue { 1.0f, 0u },
        VkClearValue { 0.0f, 0.0f, 0.0f, 0.0f },
        VkClearValue { 0.0f, 0.0f, 0.0f, 0.0f },
        VkClearValue { 0.0f, 0.0f, 0.0f, 0.0f },
        VkClearValue { 0.0f, 0.0f, 0.0f, 0.0f }
    };

    VkRenderPassBeginInfo renderPassBI = { VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO };

    renderPassBI.renderPass               = m_handle;
    renderPassBI.framebuffer              = m_attachments[p_frame.index].framebuffer;
    renderPassBI.renderArea.extent.width  = p_frame.result.width;
    renderPassBI.renderArea.extent.height = p_frame.result.height;
    renderPassBI.clearValueCount          = static_cast<uint32>(clearValues.size());
    renderPassBI.pClearValues             = clearValues.data();

    Debug::BeginCmdBufferLabelRegion(p_frame.commandBuffer.GetHandle(), "LightingPass", Color::Yellow);

    vkCmdBeginRenderPass(p_frame.commandBuffer.GetHandle(), &renderPassBI, VK_SUBPASS_CONTENTS_INLINE);

    vkCmdBindDescriptorSets(p_frame.commandBuffer.GetHandle(),
                            VK_PIPELINE_BIND_POINT_GRAPHICS,
                            m_pipelineLayout,
                            0u,
                            1u,
                            &p_frame.descriptorSets.camera,
                            0u,
                            nullptr);


    vkCmdBindDescriptorSets(p_frame.commandBuffer.GetHandle(),
                            VK_PIPELINE_BIND_POINT_GRAPHICS,
                            m_pipelineLayout,
                            1u,
                            1u,
                            &p_frame.descriptorSets.light,
                            0u,
                            nullptr);

    GBufferPass    (p_frame);
    CompositionPass(p_frame);
    TransparentPass(p_frame);

    vkCmdEndRenderPass(p_frame.commandBuffer.GetHandle());

    Debug::EndCmdBufferLabelRegion(p_frame.commandBuffer.GetHandle());
}

// ============================== [Protected Local Methods] ============================== //

void    LightingPass::SetupRenderPass   (std::vector<Frame> const& p_frames) noexcept
{
    auto const& device = RHI::Get().GetDevice();

    std::vector<VkFormat> const formats = {
        VK_FORMAT_D24_UNORM_S8_UINT,
        VK_FORMAT_D16_UNORM_S8_UINT,
        VK_FORMAT_D16_UNORM
    };

    VkFormat const depthFormat = device->FindSupportedFormat(formats,
                                                             VK_IMAGE_TILING_OPTIMAL,
                                                             VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);

    std::array<VkAttachmentDescription, 5> attachments = {};

    // Depth.
    attachments[0].format         = depthFormat;
    attachments[0].samples        = VK_SAMPLE_COUNT_1_BIT;
    attachments[0].loadOp         = VK_ATTACHMENT_LOAD_OP_CLEAR;
    attachments[0].storeOp        = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    attachments[0].stencilLoadOp  = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    attachments[0].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    attachments[0].initialLayout  = VK_IMAGE_LAYOUT_UNDEFINED;
    attachments[0].finalLayout    = VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL;

    // Position.
    attachments[1].format         = VK_FORMAT_R16G16B16A16_SFLOAT;
    attachments[1].samples        = VK_SAMPLE_COUNT_1_BIT;
    attachments[1].loadOp         = VK_ATTACHMENT_LOAD_OP_CLEAR;
    attachments[1].storeOp        = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    attachments[1].stencilLoadOp  = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    attachments[1].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    attachments[1].initialLayout  = VK_IMAGE_LAYOUT_UNDEFINED;
    attachments[1].finalLayout    = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    // Normal.
    attachments[2].format         = VK_FORMAT_R16G16B16A16_SFLOAT;
    attachments[2].samples        = VK_SAMPLE_COUNT_1_BIT;
    attachments[2].loadOp         = VK_ATTACHMENT_LOAD_OP_CLEAR;
    attachments[2].storeOp        = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    attachments[2].stencilLoadOp  = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    attachments[2].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    attachments[2].initialLayout  = VK_IMAGE_LAYOUT_UNDEFINED;
    attachments[2].finalLayout    = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    // Albedo.
    attachments[3].format         = VK_FORMAT_R8G8B8A8_UNORM;
    attachments[3].samples        = VK_SAMPLE_COUNT_1_BIT;
    attachments[3].loadOp         = VK_ATTACHMENT_LOAD_OP_CLEAR;
    attachments[3].storeOp        = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    attachments[3].stencilLoadOp  = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    attachments[3].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    attachments[3].initialLayout  = VK_IMAGE_LAYOUT_UNDEFINED;
    attachments[3].finalLayout    = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    // Composition.
    attachments[4].format         = VK_FORMAT_R16G16B16A16_SFLOAT;
    attachments[4].samples        = VK_SAMPLE_COUNT_1_BIT;
    attachments[4].loadOp         = VK_ATTACHMENT_LOAD_OP_CLEAR;
    attachments[4].storeOp        = VK_ATTACHMENT_STORE_OP_STORE;
    attachments[4].stencilLoadOp  = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    attachments[4].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    attachments[4].initialLayout  = VK_IMAGE_LAYOUT_UNDEFINED;
    attachments[4].finalLayout    = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

    std::array<VkSubpassDescription, 3> subpasses = {};

    // G-Buffer.
    VkAttachmentReference depthWriteReference = {};

    depthWriteReference.attachment = 0u;
    depthWriteReference.layout     = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    std::array<VkAttachmentReference, 3> colorReferences = {};

    colorReferences[0].attachment = 1u;
    colorReferences[0].layout     = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    colorReferences[1].attachment = 2u;
    colorReferences[1].layout     = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    colorReferences[2].attachment = 3u;
    colorReferences[2].layout     = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    subpasses[0].pipelineBindPoint       = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpasses[0].colorAttachmentCount    = static_cast<uint32>(colorReferences.size());
    subpasses[0].pColorAttachments       = colorReferences.data();
    subpasses[0].pDepthStencilAttachment = &depthWriteReference;

    // Composition.
    std::array<VkAttachmentReference, 3> inputReferences = {};

    inputReferences[0].attachment = 1u;
    inputReferences[0].layout     = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    inputReferences[1].attachment = 2u;
    inputReferences[1].layout     = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    inputReferences[2].attachment = 3u;
    inputReferences[2].layout     = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

    VkAttachmentReference colorReference = {};

    colorReference.attachment = 4u;
    colorReference.layout     = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    subpasses[1].pipelineBindPoint    = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpasses[1].inputAttachmentCount = static_cast<uint32>(inputReferences.size());
    subpasses[1].pInputAttachments    = inputReferences.data();
    subpasses[1].colorAttachmentCount = 1u;
    subpasses[1].pColorAttachments    = &colorReference;

    // Transparency.
    VkAttachmentReference depthReadReference = {};

    depthReadReference.attachment = 0u;
    depthReadReference.layout     = VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL;

    subpasses[2].pipelineBindPoint       = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpasses[2].colorAttachmentCount    = 1u;
    subpasses[2].pColorAttachments       = &colorReference;
    subpasses[2].pDepthStencilAttachment = &depthReadReference;

    std::array<VkSubpassDependency, 4> dependencies = {};

    dependencies[0].srcSubpass      = VK_SUBPASS_EXTERNAL;
    dependencies[0].dstSubpass      = 0u;
    dependencies[0].srcStageMask    = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
    dependencies[0].dstStageMask    = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependencies[0].srcAccessMask   = VK_ACCESS_MEMORY_READ_BIT;
    dependencies[0].dstAccessMask   = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
    dependencies[0].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

    dependencies[1].srcSubpass      = 0u;
    dependencies[1].dstSubpass      = 1u;
    dependencies[1].srcStageMask    = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependencies[1].dstStageMask    = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
    dependencies[1].srcAccessMask   = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
    dependencies[1].dstAccessMask   = VK_ACCESS_SHADER_READ_BIT;
    dependencies[1].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

    dependencies[2].srcSubpass      = 1u;
    dependencies[2].dstSubpass      = 2u;
    dependencies[2].srcStageMask    = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependencies[2].dstStageMask    = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
    dependencies[2].srcAccessMask   = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
    dependencies[2].dstAccessMask   = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT;
    dependencies[2].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

    dependencies[3].srcSubpass      = 2u;
    dependencies[3].dstSubpass      = VK_SUBPASS_EXTERNAL;
    dependencies[3].srcStageMask    = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependencies[3].dstStageMask    = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
    dependencies[3].srcAccessMask   = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
    dependencies[3].dstAccessMask   = VK_ACCESS_MEMORY_READ_BIT;
    dependencies[3].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

    VkRenderPassCreateInfo renderPassCI = { VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO };

    renderPassCI.attachmentCount = static_cast<uint32>(attachments.size());
    renderPassCI.pAttachments    = attachments.data();
    renderPassCI.subpassCount    = static_cast<uint32>(subpasses.size());
    renderPassCI.pSubpasses      = subpasses.data();
    renderPassCI.dependencyCount = static_cast<uint32>(dependencies.size());
    renderPassCI.pDependencies   = dependencies.data();

    VK_CHECK_RESULT(vkCreateRenderPass(device->GetLogicalDevice(), &renderPassCI, nullptr, &m_handle));

    Debug::SetRenderPassName(device->GetLogicalDevice(), m_handle, "LightingPass");
}

void    LightingPass::SetupFramebuffers  (std::vector<Frame> const& p_frames) noexcept
{
    auto const& device    = RHI::Get().GetDevice   ();
    auto const& allocator = RHI::Get().GetAllocator();

    std::vector<VkFormat> const formats = {
        VK_FORMAT_D24_UNORM_S8_UINT,
        VK_FORMAT_D16_UNORM_S8_UINT,
        VK_FORMAT_D16_UNORM
    };

    VkFormat const depthFormat = device->FindSupportedFormat(formats,
                                                             VK_IMAGE_TILING_OPTIMAL,
                                                             VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);

    m_attachments.resize(p_frames.size());

    for (size_t i = 0; i < m_attachments.size() && i < p_frames.size(); ++i)
    {
        // Depth image.
        VkImageCreateInfo imageCI = { VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO };

        imageCI.imageType     = VK_IMAGE_TYPE_2D;
        imageCI.format        = depthFormat;
        imageCI.extent.width  = p_frames[i].result.width;
        imageCI.extent.height = p_frames[i].result.height;
        imageCI.extent.depth  = 1u;
        imageCI.mipLevels     = 1u;
        imageCI.arrayLayers   = 1u;
        imageCI.samples       = VK_SAMPLE_COUNT_1_BIT;
        imageCI.tiling        = VK_IMAGE_TILING_OPTIMAL;
        imageCI.usage         = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;

        allocator->CreateImage(m_attachments[i].depth, imageCI, VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT, VMA_MEMORY_USAGE_GPU_ONLY, 0u);

        // Depth image view.
        VkImageViewCreateInfo imageViewCI = { VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO };

        imageViewCI.image                       = m_attachments[i].depth.handle;
        imageViewCI.viewType                    = VK_IMAGE_VIEW_TYPE_2D;
        imageViewCI.format                      = depthFormat;
        imageViewCI.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
        imageViewCI.subresourceRange.levelCount = imageCI.mipLevels;
        imageViewCI.subresourceRange.layerCount = imageCI.arrayLayers;

        VK_CHECK_RESULT(vkCreateImageView(device->GetLogicalDevice(), &imageViewCI, nullptr, &m_attachments[i].depth.imageView));

        // Position image.
        imageCI.format = VK_FORMAT_R16G16B16A16_SFLOAT;
        imageCI.usage  = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT;

        allocator->CreateImage(m_attachments[i].position, imageCI, VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT, VMA_MEMORY_USAGE_GPU_ONLY, 0u);

        // Position image view.
        imageViewCI.image                       = m_attachments[i].position.handle;
        imageViewCI.format                      = VK_FORMAT_R16G16B16A16_SFLOAT;
        imageViewCI.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;

        VK_CHECK_RESULT(vkCreateImageView(device->GetLogicalDevice(), &imageViewCI, nullptr, &m_attachments[i].position.imageView));

        // Normal image.
        allocator->CreateImage(m_attachments[i].normal, imageCI, VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT, VMA_MEMORY_USAGE_GPU_ONLY, 0u);

        // Normal image view.
        imageViewCI.image = m_attachments[i].normal.handle;

        VK_CHECK_RESULT(vkCreateImageView(device->GetLogicalDevice(), &imageViewCI, nullptr, &m_attachments[i].normal.imageView));

        // Albedo image.
        imageCI.format = VK_FORMAT_R8G8B8A8_UNORM;

        allocator->CreateImage(m_attachments[i].albedo, imageCI, VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT, VMA_MEMORY_USAGE_GPU_ONLY, 0u);

        // Albedo image view.
        imageViewCI.image  = m_attachments[i].albedo.handle;
        imageViewCI.format = VK_FORMAT_R8G8B8A8_UNORM;

        VK_CHECK_RESULT(vkCreateImageView(device->GetLogicalDevice(), &imageViewCI, nullptr, &m_attachments[i].albedo.imageView));

        // Composition image.
        imageCI.format = VK_FORMAT_R16G16B16A16_SFLOAT;
        imageCI.usage  = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;

        allocator->CreateImage(m_attachments[i].composition, imageCI, VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT, VMA_MEMORY_USAGE_GPU_ONLY, 0u);

        // Composition image view.
        imageViewCI.image  = m_attachments[i].composition.handle;
        imageViewCI.format = VK_FORMAT_R16G16B16A16_SFLOAT;

        VK_CHECK_RESULT(vkCreateImageView(device->GetLogicalDevice(), &imageViewCI, nullptr, &m_attachments[i].composition.imageView));

        // Setups debug info.
        Debug::SetImageName    (device->GetLogicalDevice(), m_attachments[i].depth      .handle,    ("Depth_Image_"           + std::to_string(i)).c_str());
        Debug::SetImageName    (device->GetLogicalDevice(), m_attachments[i].position   .handle,    ("Position_Image_"        + std::to_string(i)).c_str());
        Debug::SetImageName    (device->GetLogicalDevice(), m_attachments[i].normal     .handle,    ("Normal_Image_"          + std::to_string(i)).c_str());
        Debug::SetImageName    (device->GetLogicalDevice(), m_attachments[i].albedo     .handle,    ("Albedo_Image_"          + std::to_string(i)).c_str());
        Debug::SetImageName    (device->GetLogicalDevice(), m_attachments[i].composition.handle,    ("Composition_Image_"     + std::to_string(i)).c_str());
        Debug::SetImageViewName(device->GetLogicalDevice(), m_attachments[i].depth      .imageView, ("Depth_ImageView_"       + std::to_string(i)).c_str());
        Debug::SetImageViewName(device->GetLogicalDevice(), m_attachments[i].position   .imageView, ("Position_ImageView_"    + std::to_string(i)).c_str());
        Debug::SetImageViewName(device->GetLogicalDevice(), m_attachments[i].normal     .imageView, ("Normal_ImageView_"      + std::to_string(i)).c_str());
        Debug::SetImageViewName(device->GetLogicalDevice(), m_attachments[i].albedo     .imageView, ("Albedo_ImageView_"      + std::to_string(i)).c_str());
        Debug::SetImageViewName(device->GetLogicalDevice(), m_attachments[i].composition.imageView, ("Composition_ImageView_" + std::to_string(i)).c_str());

        // Framebuffer.
        std::array<VkImageView, 5> attachments = {
            m_attachments[i].depth      .imageView,
            m_attachments[i].position   .imageView,
            m_attachments[i].normal     .imageView,
            m_attachments[i].albedo     .imageView,
            m_attachments[i].composition.imageView,
        };

        VkFramebufferCreateInfo framebufferCI = { VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO };

        framebufferCI.renderPass      = m_handle;
        framebufferCI.attachmentCount = static_cast<uint32>(attachments.size());
        framebufferCI.pAttachments    = attachments.data();
        framebufferCI.width           = p_frames[i].result.width;
        framebufferCI.height          = p_frames[i].result.height;
        framebufferCI.layers          = 1u;

        VK_CHECK_RESULT(vkCreateFramebuffer(device->GetLogicalDevice(), &framebufferCI, nullptr, &m_attachments[i].framebuffer));

        Debug::SetFramebufferName(device->GetLogicalDevice(), m_attachments[i].framebuffer, ("Lighting_Framebuffer_" + std::to_string(i)).c_str());
    }
}

void    LightingPass::SetupPipelines    (std::vector<Frame> const& p_frames) noexcept
{
    SetupCompositionDescriptorPool     (p_frames);
    SetupCompositionDescriptorSetLayout(p_frames);
    SetupCompositionDescriptorSets     (p_frames);
    SetupCompositionPipelineLayout     (p_frames);
    SetupCompositionPipeline           (p_frames);
}

// ======================================================================================= //

void    LightingPass::SetupCompositionDescriptorPool        (std::vector<Frame> const& p_frames) noexcept
{
    VkDevice const device = RHI::Get().GetDevice()->GetLogicalDevice();

    std::array<VkDescriptorPoolSize, 2> descriptorPoolSizes = {};

    descriptorPoolSizes[0].type            = VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT;
    descriptorPoolSizes[0].descriptorCount = static_cast<uint32>(3 * p_frames.size());

    descriptorPoolSizes[1].type            = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    descriptorPoolSizes[1].descriptorCount = static_cast<uint32>(2 * p_frames.size());

    VkDescriptorPoolCreateInfo descriptorPoolCI = { VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO };

    descriptorPoolCI.maxSets       = static_cast<uint32>(p_frames.size());
    descriptorPoolCI.poolSizeCount = static_cast<uint32>(descriptorPoolSizes.size());
    descriptorPoolCI.pPoolSizes    = descriptorPoolSizes.data();

    VK_CHECK_RESULT(vkCreateDescriptorPool(device, &descriptorPoolCI, nullptr, &m_descriptorPool));

    Debug::SetDescriptorPoolName(device, m_descriptorPool, "Composition_DescriptorPool");
}

void    LightingPass::SetupCompositionDescriptorSetLayout   (std::vector<Frame> const& p_frames) noexcept
{
    VkDevice const device = RHI::Get().GetDevice()->GetLogicalDevice();

    std::array<VkDescriptorSetLayoutBinding, 5> descriptorSetLayoutBindings = {};

    descriptorSetLayoutBindings[0].binding         = 0u;
    descriptorSetLayoutBindings[0].descriptorType  = VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT;
    descriptorSetLayoutBindings[0].descriptorCount = 1u;
    descriptorSetLayoutBindings[0].stageFlags      = VK_SHADER_STAGE_FRAGMENT_BIT;

    descriptorSetLayoutBindings[1].binding         = 1u;
    descriptorSetLayoutBindings[1].descriptorType  = VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT;
    descriptorSetLayoutBindings[1].descriptorCount = 1u;
    descriptorSetLayoutBindings[1].stageFlags      = VK_SHADER_STAGE_FRAGMENT_BIT;

    descriptorSetLayoutBindings[2].binding         = 2u;
    descriptorSetLayoutBindings[2].descriptorType  = VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT;
    descriptorSetLayoutBindings[2].descriptorCount = 1u;
    descriptorSetLayoutBindings[2].stageFlags      = VK_SHADER_STAGE_FRAGMENT_BIT;

    descriptorSetLayoutBindings[3].binding         = 3u;
    descriptorSetLayoutBindings[3].descriptorType  = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    descriptorSetLayoutBindings[3].descriptorCount = 1u;
    descriptorSetLayoutBindings[3].stageFlags      = VK_SHADER_STAGE_FRAGMENT_BIT;

    descriptorSetLayoutBindings[4].binding         = 4u;
    descriptorSetLayoutBindings[4].descriptorType  = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    descriptorSetLayoutBindings[4].descriptorCount = 1u;
    descriptorSetLayoutBindings[4].stageFlags      = VK_SHADER_STAGE_FRAGMENT_BIT;

    VkDescriptorSetLayoutCreateInfo descriptorSetLayoutCI = { VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO };

    descriptorSetLayoutCI.bindingCount = static_cast<uint32>(descriptorSetLayoutBindings.size());
    descriptorSetLayoutCI.pBindings    = descriptorSetLayoutBindings.data();

    VK_CHECK_RESULT(vkCreateDescriptorSetLayout(device, &descriptorSetLayoutCI, nullptr, &m_descriptorSetLayout));

    Debug::SetDescriptorSetLayoutName(device, m_descriptorSetLayout, "Composition_DescriptorSetLayout");
}

void    LightingPass::SetupCompositionDescriptorSets        (std::vector<Frame> const& p_frames) noexcept
{
    VkDevice const device = RHI::Get().GetDevice()->GetLogicalDevice();

    for (size_t i = 0; i < p_frames.size(); ++i)
    {
        VkDescriptorSetAllocateInfo decriptorSetAI = { VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO };

        decriptorSetAI.descriptorPool     = m_descriptorPool;
        decriptorSetAI.descriptorSetCount = 1u;
        decriptorSetAI.pSetLayouts        = &m_descriptorSetLayout;

        VK_CHECK_RESULT(vkAllocateDescriptorSets(device, &decriptorSetAI, &m_attachments[i].descriptorSet));

        Debug::SetDescriptorSetName(device, m_attachments[i].descriptorSet, ("Composition_DescriptorSet" + std::to_string(i)).c_str());

        std::array<VkWriteDescriptorSet, 5> writeSets = {};

        VkDescriptorImageInfo positionImageInfo = {};

        positionImageInfo.imageView   = m_attachments[i].position.imageView;
        positionImageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

        writeSets[0].sType           = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        writeSets[0].dstSet          = m_attachments[i].descriptorSet;
        writeSets[0].dstBinding      = 0u;
        writeSets[0].descriptorCount = 1u;
        writeSets[0].descriptorType  = VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT;
        writeSets[0].pImageInfo      = &positionImageInfo;

        VkDescriptorImageInfo normalImageInfo = {};

        normalImageInfo.imageView   = m_attachments[i].normal.imageView;
        normalImageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

        writeSets[1].sType           = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        writeSets[1].dstSet          = m_attachments[i].descriptorSet;
        writeSets[1].dstBinding      = 1u;
        writeSets[1].descriptorCount = 1u;
        writeSets[1].descriptorType  = VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT;
        writeSets[1].pImageInfo      = &normalImageInfo;

        VkDescriptorImageInfo albedoImageInfo = {};

        albedoImageInfo.imageView   = m_attachments[i].albedo.imageView;
        albedoImageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

        writeSets[2].sType           = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        writeSets[2].dstSet          = m_attachments[i].descriptorSet;
        writeSets[2].dstBinding      = 2u;
        writeSets[2].descriptorCount = 1u;
        writeSets[2].descriptorType  = VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT;
        writeSets[2].pImageInfo      = &albedoImageInfo;

        ShadowPass const* shadowPass = static_cast<ShadowPass*>(RHI::Get().GetRenderPass(ERenderStage::SHADOW).get());

        VkDescriptorImageInfo shadow2DInfo = {};

        shadow2DInfo.sampler     = shadowPass->GetShadowSampler();
        shadow2DInfo.imageView   = shadowPass->GetShadow2D(i).imageView;
        shadow2DInfo.imageLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL;

        writeSets[3].sType           = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        writeSets[3].dstSet          = m_attachments[i].descriptorSet;
        writeSets[3].dstBinding      = 3u;
        writeSets[3].descriptorCount = 1u;
        writeSets[3].descriptorType  = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        writeSets[3].pImageInfo      = &shadow2DInfo;

        VkDescriptorImageInfo shadowCubeInfo = {};

        shadowCubeInfo.sampler     = shadowPass->GetShadowSampler  ();
        shadowCubeInfo.imageView   = shadowPass->GetShadowCube(i).imageView;
        shadowCubeInfo.imageLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL;

        writeSets[4].sType           = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        writeSets[4].dstSet          = m_attachments[i].descriptorSet;
        writeSets[4].dstBinding      = 4u;
        writeSets[4].descriptorCount = 1u;
        writeSets[4].descriptorType  = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        writeSets[4].pImageInfo      = &shadowCubeInfo;

        vkUpdateDescriptorSets(device, static_cast<uint32>(writeSets.size()), writeSets.data(), 0u, nullptr);
    }
}

void    LightingPass::SetupCompositionPipelineLayout        (std::vector<Frame> const& p_frames) noexcept
{
    VkDevice const device = RHI::Get().GetDevice()->GetLogicalDevice();

    std::array<VkDescriptorSetLayout, 3> descriptorSetLayouts = {
        RHI::Get().GetCameraLayout(),
        RHI::Get().GetLightLayout (),
        m_descriptorSetLayout
    };

    std::array<VkPushConstantRange, 2> pushConstants = {};

    pushConstants[0].stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
    pushConstants[0].offset     = 0u;
    pushConstants[0].size       = sizeof(Matrix4x4);

    pushConstants[1].stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
    pushConstants[1].offset     = sizeof(Matrix4x4);
    pushConstants[1].size       = sizeof(MaterialData);

    VkPipelineLayoutCreateInfo pipelineLayoutCI = { VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO };

    pipelineLayoutCI.setLayoutCount         = static_cast<uint32>(descriptorSetLayouts.size());
    pipelineLayoutCI.pSetLayouts            = descriptorSetLayouts.data();
    pipelineLayoutCI.pushConstantRangeCount = static_cast<uint32>(pushConstants.size());
    pipelineLayoutCI.pPushConstantRanges    = pushConstants.data();

    VK_CHECK_RESULT(vkCreatePipelineLayout(device, &pipelineLayoutCI, nullptr, &m_pipelineLayout));

    Debug::SetPipelineLayoutName(device, m_pipelineLayout, "Composition_PipelineLayout");
}

void    LightingPass::SetupCompositionPipeline              (std::vector<Frame> const& p_frames) noexcept
{
    auto const& device = RHI::Get().GetDevice       ();
    auto const& cache  = RHI::Get().GetPipelineCache();

    std::array<VkPipelineShaderStageCreateInfo, 2> shaderStages = {};

    shaderStages[0].sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    shaderStages[0].stage  = VK_SHADER_STAGE_VERTEX_BIT;
    shaderStages[0].module = AssetManager::Get().Get<Shader>("Default/Shaders/fullscreen.vert", ELoadingMode::BLOCKING)->GetModule();
    shaderStages[0].pName  = "main";

    shaderStages[1].sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    shaderStages[1].stage  = VK_SHADER_STAGE_FRAGMENT_BIT;
    shaderStages[1].module = AssetManager::Get().Get<Shader>("Default/Shaders/composition.frag", ELoadingMode::BLOCKING)->GetModule();
    shaderStages[1].pName  = "main";

    VkPipelineVertexInputStateCreateInfo   vertexInputStateCI   = { VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO   };
    VkPipelineInputAssemblyStateCreateInfo inputAssemblyStateCI = { VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO };

    inputAssemblyStateCI.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;

    VkPipelineViewportStateCreateInfo viewportStateCI = { VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO };

    viewportStateCI.viewportCount = 1u;
    viewportStateCI.scissorCount  = 1u;

    VkPipelineRasterizationStateCreateInfo rasterizationStateCI = { VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO };

    rasterizationStateCI.polygonMode = VK_POLYGON_MODE_FILL;
    rasterizationStateCI.cullMode    = VK_CULL_MODE_NONE;
    rasterizationStateCI.frontFace   = VK_FRONT_FACE_CLOCKWISE;
    rasterizationStateCI.lineWidth   = 1.0f;

    VkPipelineMultisampleStateCreateInfo multisampleStateCI = { VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO };

    multisampleStateCI.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

    VkPipelineColorBlendAttachmentState colorAttachment = {};

    colorAttachment.colorWriteMask = 0xf;

    VkPipelineColorBlendStateCreateInfo colorBlendStateCI = { VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO };

    colorBlendStateCI.attachmentCount = 1u;
    colorBlendStateCI.pAttachments    = &colorAttachment;

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
    pipelineCI.pDepthStencilState  = nullptr;
    pipelineCI.pColorBlendState    = &colorBlendStateCI;
    pipelineCI.pDynamicState       = &dynamicStateCI;
    pipelineCI.layout              = m_pipelineLayout;
    pipelineCI.renderPass          = m_handle;
    pipelineCI.subpass             = 1u;

    std::array<VkSpecializationMapEntry, 3> entries = {};

    entries[0].constantID = 0u;
    entries[1].constantID = 1u;
    entries[2].constantID = 2u;

    entries[0].offset = 0u * sizeof(uint32);
    entries[1].offset = 1u * sizeof(uint32);
    entries[2].offset = 2u * sizeof(uint32);

    entries[0].size = sizeof(uint32);
    entries[1].size = sizeof(uint32);
    entries[2].size = sizeof(uint32);

    uint32 maxTriangles = (Math::Min(device->GetProperties().limits.maxGeometryOutputVertices,
                                     device->GetProperties().limits.maxGeometryTotalOutputComponents / 6u) / 18u) * 6u;

    std::array<uint32, 3> lightCounts = {
        maxTriangles / 2u,
        maxTriangles / 6u,
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
                                              &m_pipeline));

    Debug::SetPipelineName(device->GetLogicalDevice(), m_pipeline, "Composition_Pipeline");
}

// ======================================================================================= //

void    LightingPass::GBufferPass       (Frame const& p_frame) const noexcept
{
    Debug::BeginCmdBufferLabelRegion(p_frame.commandBuffer.GetHandle(), "GBuffer", Color::Red);

    size_t       index  = MAX_UINT_32;
    VkDeviceSize offset = 0;

    for (auto const& mesh : p_frame.renderList->opaqueMeshes)
    {
        if (index != std::get<0>(mesh))
        {
            index = std::get<0>(mesh);

            vkCmdBindPipeline(p_frame.commandBuffer.GetHandle(),
                              VK_PIPELINE_BIND_POINT_GRAPHICS,
                              p_frame.renderList->materials[index]->pipeline);

            vkCmdBindDescriptorSets(p_frame.commandBuffer.GetHandle(),
                                    VK_PIPELINE_BIND_POINT_GRAPHICS,
                                    p_frame.renderList->materials[index]->pipelineLayout,
                                    2u,
                                    1u,
                                    &p_frame.renderList->materials[index]->descriptorSet,
                                    0u,
                                    nullptr);
        }

        vkCmdPushConstants(p_frame.commandBuffer.GetHandle(),
                           p_frame.renderList->materials[index]->pipelineLayout,
                           VK_SHADER_STAGE_VERTEX_BIT,
                           0u,
                           sizeof(Matrix4x4),
                           &std::get<1>(mesh));

        vkCmdPushConstants(p_frame.commandBuffer.GetHandle(),
                           p_frame.renderList->materials[index]->pipelineLayout,
                           VK_SHADER_STAGE_FRAGMENT_BIT,
                           sizeof(Matrix4x4),
                           sizeof(MaterialData),
                           std::get<2>(mesh));

        vkCmdBindVertexBuffers(p_frame.commandBuffer.GetHandle(), 0u, 1u, &std::get<3>(mesh)->vertexBuffer.handle, &offset);

        vkCmdBindIndexBuffer(p_frame.commandBuffer.GetHandle(), std::get<3>(mesh)->indexBuffer.handle, 0u, VK_INDEX_TYPE_UINT32);

        vkCmdDrawIndexed(p_frame.commandBuffer.GetHandle(), std::get<3>(mesh)->indexCount, 1u, 0u, 0u, 0u);
    }

    Debug::EndCmdBufferLabelRegion(p_frame.commandBuffer.GetHandle());
}

void    LightingPass::CompositionPass   (Frame const& p_frame) const noexcept
{
    Debug::BeginCmdBufferLabelRegion(p_frame.commandBuffer.GetHandle(), "Composition", Color::Green);

    vkCmdNextSubpass (p_frame.commandBuffer.GetHandle(), VK_SUBPASS_CONTENTS_INLINE);
    vkCmdBindPipeline(p_frame.commandBuffer.GetHandle(), VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipeline);

    vkCmdBindDescriptorSets(p_frame.commandBuffer.GetHandle(),
                            VK_PIPELINE_BIND_POINT_GRAPHICS,
                            m_pipelineLayout,
                            2u,
                            1u,
                            &m_attachments[p_frame.index].descriptorSet,
                            0u,
                            nullptr);

    vkCmdDraw(p_frame.commandBuffer.GetHandle(), 3u, 1u, 0u, 0u);

    Debug::EndCmdBufferLabelRegion(p_frame.commandBuffer.GetHandle());
}

void    LightingPass::TransparentPass   (Frame const& p_frame) const noexcept
{
    Debug::BeginCmdBufferLabelRegion(p_frame.commandBuffer.GetHandle(), "Transparent", Color::Blue);

    vkCmdNextSubpass(p_frame.commandBuffer.GetHandle(), VK_SUBPASS_CONTENTS_INLINE);

    size_t       index  = MAX_UINT_32;
    VkDeviceSize offset = 0;

    for (auto const& mesh : p_frame.renderList->transparentMeshes)
    {
        if (index != std::get<0>(mesh))
        {
            index = std::get<0>(mesh);

            vkCmdBindPipeline(p_frame.commandBuffer.GetHandle(),
                              VK_PIPELINE_BIND_POINT_GRAPHICS,
                              p_frame.renderList->materials[index]->pipeline);

            vkCmdBindDescriptorSets(p_frame.commandBuffer.GetHandle(),
                                    VK_PIPELINE_BIND_POINT_GRAPHICS,
                                    p_frame.renderList->materials[index]->pipelineLayout,
                                    2u,
                                    1u,
                                    &p_frame.renderList->materials[index]->descriptorSet,
                                    0u,
                                    nullptr);
        }

        vkCmdPushConstants(p_frame.commandBuffer.GetHandle(),
                           p_frame.renderList->materials[index]->pipelineLayout,
                           VK_SHADER_STAGE_VERTEX_BIT,
                           0u,
                           sizeof(Matrix4x4),
                           &std::get<1>(mesh));

        vkCmdPushConstants(p_frame.commandBuffer.GetHandle(),
                           p_frame.renderList->materials[index]->pipelineLayout,
                           VK_SHADER_STAGE_FRAGMENT_BIT,
                           sizeof(Matrix4x4),
                           sizeof(MaterialData),
                           std::get<2>(mesh));

        vkCmdBindVertexBuffers(p_frame.commandBuffer.GetHandle(), 0u, 1u, &std::get<3>(mesh)->vertexBuffer.handle, &offset);

        vkCmdBindIndexBuffer(p_frame.commandBuffer.GetHandle(), std::get<3>(mesh)->indexBuffer.handle, 0u, VK_INDEX_TYPE_UINT32);

        vkCmdDrawIndexed(p_frame.commandBuffer.GetHandle(), std::get<3>(mesh)->indexCount, 1u, 0u, 0u, 0u);
    }

    Debug::EndCmdBufferLabelRegion(p_frame.commandBuffer.GetHandle());
}