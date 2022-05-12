#include "PCH.hpp"
#include "RHI.hpp"
#include "Renderer.hpp"
#include "AssetManager.hpp"

#include "Vulkan/RenderPasses/BloomPass.hpp"
#include "Vulkan/RenderPasses/LightingPass.hpp"

// ============================== [Public Constructor and Destructor] ============================== //

BloomPass::BloomPass    (std::vector<Frame> const& p_frames) : RenderPass()
{
    #if EDITOR

    // Loads default shaders.
    AssetManager& assetManager = AssetManager::Get();

    assetManager.FindOrAdd((std::string(SOURCE_DIRECTORY) + "Default/Shaders/fullscreen.vert.glsl") .c_str(), "Default/Shaders/");
    assetManager.FindOrAdd((std::string(SOURCE_DIRECTORY) + "Default/Shaders/brightcolor.frag.glsl").c_str(), "Default/Shaders/");
    assetManager.FindOrAdd((std::string(SOURCE_DIRECTORY) + "Default/Shaders/blur.frag.glsl")       .c_str(), "Default/Shaders/");

    #endif

    SetupRenderPass  (p_frames);
    SetupFramebuffers(p_frames);
    SetupPipelines   (p_frames);
}

BloomPass::~BloomPass   ()
{
    auto const& device    = RHI::Get().GetDevice   ();
    auto const& allocator = RHI::Get().GetAllocator();

    for (auto const& attachment : m_attachments)
    {
        vkDestroyFramebuffer(device->GetLogicalDevice(), attachment.brightColorFramebuffer,    nullptr);
        vkDestroyFramebuffer(device->GetLogicalDevice(), attachment.horizontalBlurFramebuffer, nullptr);
        vkDestroyFramebuffer(device->GetLogicalDevice(), attachment.verticalBlurFramebuffer,   nullptr);
    }

    vkDestroyDescriptorPool     (device->GetLogicalDevice(), m_brightcolor.descriptorPool,         nullptr);
    vkDestroyDescriptorSetLayout(device->GetLogicalDevice(), m_brightcolor.descriptorSetLayout,    nullptr);
    vkDestroyPipelineLayout     (device->GetLogicalDevice(), m_brightcolor.pipelineLayout,         nullptr);
    vkDestroyPipeline           (device->GetLogicalDevice(), m_brightcolor.pipeline,               nullptr);
    vkDestroyDescriptorPool     (device->GetLogicalDevice(), m_blur       .descriptorPool,         nullptr);
    vkDestroyDescriptorSetLayout(device->GetLogicalDevice(), m_blur       .descriptorSetLayout,    nullptr);
    vkDestroyPipelineLayout     (device->GetLogicalDevice(), m_blur       .pipelineLayout,         nullptr);
    vkDestroyPipeline           (device->GetLogicalDevice(), m_blur       .horizontalBlurPipeline, nullptr);
    vkDestroyPipeline           (device->GetLogicalDevice(), m_blur       .verticalBlurPipeline,   nullptr);
}

// ============================== [Public Local Methods] ============================== //

void    BloomPass::Rebuild    (std::vector<Frame> const& p_frames) noexcept
{

}

void    BloomPass::Draw       (Frame const& p_frame) noexcept
{
    VkViewport viewport = {
        0.0f,
        0.0f,
        static_cast<float>(p_frame.result.width),
        static_cast<float>(p_frame.result.height),
        0.0f,
        1.0f
    };

    VkRect2D scissor = {
        0,
        0,
        p_frame.result.width,
        p_frame.result.height
    };

    vkCmdSetViewport(p_frame.commandBuffer.GetHandle(), 0u, 1u, &viewport);
    vkCmdSetScissor (p_frame.commandBuffer.GetHandle(), 0u, 1u, &scissor);

    Debug::BeginCmdBufferLabelRegion(p_frame.commandBuffer.GetHandle(), "BloomPass", Color::Cyan);

    BrightColorPass(p_frame);
    BlurPass       (p_frame);

    Debug::EndCmdBufferLabelRegion(p_frame.commandBuffer.GetHandle());
}

// ============================== [Protected Local Methods] ============================== //

void    BloomPass::SetupRenderPass      (std::vector<Frame> const& p_frames) noexcept
{
    VkDevice const device = RHI::Get().GetDevice()->GetLogicalDevice();

    VkAttachmentDescription attachment = {};

    attachment.format         = VK_FORMAT_R16G16B16A16_SFLOAT;
    attachment.samples        = VK_SAMPLE_COUNT_1_BIT;
    attachment.loadOp         = VK_ATTACHMENT_LOAD_OP_CLEAR;
    attachment.storeOp        = VK_ATTACHMENT_STORE_OP_STORE;
    attachment.stencilLoadOp  = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    attachment.initialLayout  = VK_IMAGE_LAYOUT_UNDEFINED;
    attachment.finalLayout    = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

    VkAttachmentReference colorReference = {};

    colorReference.attachment = 0u;
    colorReference.layout     = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpass = {};

    subpass.pipelineBindPoint    = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1u;
    subpass.pColorAttachments    = &colorReference;

    std::array<VkSubpassDependency, 2> dependencies = {};

    dependencies[0].srcSubpass      = VK_SUBPASS_EXTERNAL;
    dependencies[0].dstSubpass      = 0u;
    dependencies[0].srcStageMask    = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
    dependencies[0].dstStageMask    = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependencies[0].srcAccessMask   = VK_ACCESS_MEMORY_READ_BIT;
    dependencies[0].dstAccessMask   = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
    dependencies[0].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

    dependencies[1].srcSubpass      = 0u;
    dependencies[1].dstSubpass      = VK_SUBPASS_EXTERNAL;
    dependencies[1].srcStageMask    = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependencies[1].dstStageMask    = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
    dependencies[1].srcAccessMask   = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
    dependencies[1].dstAccessMask   = VK_ACCESS_MEMORY_READ_BIT;
    dependencies[1].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

    VkRenderPassCreateInfo renderPassCI = { VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO };

    renderPassCI.attachmentCount = 1u;
    renderPassCI.pAttachments    = &attachment;
    renderPassCI.subpassCount    = 1u;
    renderPassCI.pSubpasses      = &subpass;
    renderPassCI.dependencyCount = static_cast<uint32>(dependencies.size());
    renderPassCI.pDependencies   = dependencies.data();

    VK_CHECK_RESULT(vkCreateRenderPass(device, &renderPassCI, nullptr, &m_handle));

    Debug::SetRenderPassName(device, m_handle, "BloomPass");
}

void    BloomPass::SetupFramebuffers     (std::vector<Frame> const& p_frames) noexcept
{
    auto const& device    = RHI::Get().GetDevice   ();
    auto const& allocator = RHI::Get().GetAllocator();

    LightingPass* lightingPass = static_cast<LightingPass*>(RHI::Get().GetRenderPass(ERenderStage::LIGHTING).get());

    m_attachments.resize(p_frames.size());

    for (size_t i = 0; i < m_attachments.size() && i < p_frames.size(); ++i)
    {
        m_attachments[i].horizontalBlur = &lightingPass->GetPositionImage(i);
        m_attachments[i].verticalBlur   = &lightingPass->GetNormalImage  (i);

        // Framebuffers.
        VkFramebufferCreateInfo framebufferCI = { VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO };

        framebufferCI.renderPass      = m_handle;
        framebufferCI.attachmentCount = 1u;
        framebufferCI.pAttachments    = &m_attachments[i].horizontalBlur->imageView;
        framebufferCI.width           = p_frames[i].result.width;
        framebufferCI.height          = p_frames[i].result.height;
        framebufferCI.layers          = 1u;

        VK_CHECK_RESULT(vkCreateFramebuffer(device->GetLogicalDevice(), &framebufferCI, nullptr, &m_attachments[i].brightColorFramebuffer));

        framebufferCI.pAttachments = &m_attachments[i].horizontalBlur->imageView;

        VK_CHECK_RESULT(vkCreateFramebuffer(device->GetLogicalDevice(), &framebufferCI, nullptr, &m_attachments[i].horizontalBlurFramebuffer));

        framebufferCI.pAttachments = &m_attachments[i].verticalBlur->imageView;

        VK_CHECK_RESULT(vkCreateFramebuffer(device->GetLogicalDevice(), &framebufferCI, nullptr, &m_attachments[i].verticalBlurFramebuffer));

        Debug::SetFramebufferName(device->GetLogicalDevice(), m_attachments[i].brightColorFramebuffer,    ("BrightColor_Framebuffer_"    + std::to_string(i)).c_str());
        Debug::SetFramebufferName(device->GetLogicalDevice(), m_attachments[i].horizontalBlurFramebuffer, ("HorizontalBlur_Framebuffer_" + std::to_string(i)).c_str());
        Debug::SetFramebufferName(device->GetLogicalDevice(), m_attachments[i].verticalBlurFramebuffer,   ("VerticalBlur_Framebuffer_"   + std::to_string(i)).c_str());
    }
}

void    BloomPass::SetupPipelines       (std::vector<Frame> const& p_frames) noexcept
{
    SetupBrightColorDescriptorPool     (p_frames);
    SetupBrightColorDescriptorSetLayout(p_frames);
    SetupBrightColorDescriptorSets     (p_frames);
    SetupBrightColorPipelineLayout     (p_frames);
    SetupBrightColorPipeline           (p_frames);

    SetupBlurDescriptorPool     (p_frames);
    SetupBlurDescriptorSetLayout(p_frames);
    SetupBlurDescriptorSets     (p_frames);
    SetupBlurPipelineLayout     (p_frames);
    SetupBlurPipelines          (p_frames);
}

// ======================================================================================= //

void    BloomPass::SetupBrightColorDescriptorPool       (std::vector<Frame> const& p_frames) noexcept
{
    VkDevice const device = RHI::Get().GetDevice()->GetLogicalDevice();

    VkDescriptorPoolSize descriptorPoolSize = {};

    descriptorPoolSize.type            = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    descriptorPoolSize.descriptorCount = static_cast<uint32>(p_frames.size());

    VkDescriptorPoolCreateInfo descriptorPoolCI = { VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO };

    descriptorPoolCI.maxSets       = static_cast<uint32>(p_frames.size());
    descriptorPoolCI.poolSizeCount = 1u;
    descriptorPoolCI.pPoolSizes    = &descriptorPoolSize;

    VK_CHECK_RESULT(vkCreateDescriptorPool(device, &descriptorPoolCI, nullptr, &m_brightcolor.descriptorPool));

    Debug::SetDescriptorPoolName(device, m_brightcolor.descriptorPool, "BrightColor_DescriptorPool");
}

void    BloomPass::SetupBrightColorDescriptorSetLayout  (std::vector<Frame> const& p_frames) noexcept
{
    VkDevice const device = RHI::Get().GetDevice()->GetLogicalDevice();

    VkDescriptorSetLayoutBinding descriptorSetLayoutBinding = {};

    descriptorSetLayoutBinding.binding         = 0u;
    descriptorSetLayoutBinding.descriptorType  = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    descriptorSetLayoutBinding.descriptorCount = 1u;
    descriptorSetLayoutBinding.stageFlags      = VK_SHADER_STAGE_FRAGMENT_BIT;

    VkDescriptorSetLayoutCreateInfo descriptorSetLayoutCI = { VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO };

    descriptorSetLayoutCI.bindingCount = 1u;
    descriptorSetLayoutCI.pBindings    = &descriptorSetLayoutBinding;

    VK_CHECK_RESULT(vkCreateDescriptorSetLayout(device, &descriptorSetLayoutCI, nullptr, &m_brightcolor.descriptorSetLayout));

    Debug::SetDescriptorSetLayoutName(device, m_brightcolor.descriptorSetLayout, "BrightColor_DescriptorSetLayout");
}

void    BloomPass::SetupBrightColorDescriptorSets       (std::vector<Frame> const& p_frames) noexcept
{
    VkDevice const device = RHI::Get().GetDevice()->GetLogicalDevice();

    for (size_t i = 0; i < p_frames.size(); ++i)
    {
        VkDescriptorSetAllocateInfo decriptorSetAI = { VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO };

        decriptorSetAI.descriptorPool     = m_brightcolor.descriptorPool;
        decriptorSetAI.descriptorSetCount = 1u;
        decriptorSetAI.pSetLayouts        = &m_brightcolor.descriptorSetLayout;

        VK_CHECK_RESULT(vkAllocateDescriptorSets(device, &decriptorSetAI, &m_attachments[i].brightColorDescriptorSet));

        Debug::SetDescriptorSetName(device, m_attachments[i].brightColorDescriptorSet, ("BrightColor_DescriptorSet" + std::to_string(i)).c_str());

        VkDescriptorImageInfo imageInfo = {};

        imageInfo.sampler     = RHI::Get().GetSceneSampler();
        imageInfo.imageView   = static_cast<LightingPass*>(RHI::Get().GetRenderPass(ERenderStage::LIGHTING).get())->GetResult(i).imageView;
        imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

        VkWriteDescriptorSet writeSet = {};

        writeSet.sType           = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        writeSet.dstSet          = m_attachments[i].brightColorDescriptorSet;
        writeSet.dstBinding      = 0u;
        writeSet.descriptorCount = 1u;
        writeSet.descriptorType  = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        writeSet.pImageInfo      = &imageInfo;

        vkUpdateDescriptorSets(device, 1u, &writeSet, 0u, nullptr);
    }
}

void    BloomPass::SetupBrightColorPipelineLayout       (std::vector<Frame> const& p_frames) noexcept
{
    VkDevice const device = RHI::Get().GetDevice()->GetLogicalDevice();

    VkPipelineLayoutCreateInfo pipelineLayoutCI = { VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO };

    pipelineLayoutCI.setLayoutCount = 1u;
    pipelineLayoutCI.pSetLayouts    = &m_brightcolor.descriptorSetLayout;

    VK_CHECK_RESULT(vkCreatePipelineLayout(device, &pipelineLayoutCI, nullptr, &m_brightcolor.pipelineLayout));

    Debug::SetPipelineLayoutName(device, m_brightcolor.pipelineLayout, "BrightColor_PipelineLayout");
}

void    BloomPass::SetupBrightColorPipeline             (std::vector<Frame> const& p_frames) noexcept
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
    shaderStages[1].module = AssetManager::Get().Get<Shader>("Default/Shaders/brightcolor.frag", ELoadingMode::BLOCKING)->GetModule();
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
    pipelineCI.layout              = m_brightcolor.pipelineLayout;
    pipelineCI.renderPass          = m_handle;
    pipelineCI.subpass             = 0u;

    VK_CHECK_RESULT(vkCreateGraphicsPipelines(device->GetLogicalDevice(),
                                              cache ->GetHandle       (),
                                              1u,
                                              &pipelineCI,
                                              nullptr,
                                              &m_brightcolor.pipeline));

    Debug::SetPipelineName(device->GetLogicalDevice(), m_brightcolor.pipeline, "BrightColor_Pipeline");
}

void    BloomPass::SetupBlurDescriptorPool              (std::vector<Frame> const& p_frames) noexcept
{
    VkDevice const device = RHI::Get().GetDevice()->GetLogicalDevice();

    VkDescriptorPoolSize descriptorPoolSize = {};

    descriptorPoolSize.type            = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    descriptorPoolSize.descriptorCount = static_cast<uint32>(2 * p_frames.size());

    VkDescriptorPoolCreateInfo descriptorPoolCI = { VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO };

    descriptorPoolCI.maxSets       = static_cast<uint32>(2 * p_frames.size());
    descriptorPoolCI.poolSizeCount = 1u;
    descriptorPoolCI.pPoolSizes    = &descriptorPoolSize;

    VK_CHECK_RESULT(vkCreateDescriptorPool(device, &descriptorPoolCI, nullptr, &m_blur.descriptorPool));

    Debug::SetDescriptorPoolName(device, m_blur.descriptorPool, "Blur_DescriptorPool");
}

void    BloomPass::SetupBlurDescriptorSetLayout         (std::vector<Frame> const& p_frames) noexcept
{
    VkDevice const device = RHI::Get().GetDevice()->GetLogicalDevice();

    VkDescriptorSetLayoutBinding descriptorSetLayoutBinding = {};

    descriptorSetLayoutBinding.binding         = 0u;
    descriptorSetLayoutBinding.descriptorType  = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    descriptorSetLayoutBinding.descriptorCount = 1u;
    descriptorSetLayoutBinding.stageFlags      = VK_SHADER_STAGE_FRAGMENT_BIT;

    VkDescriptorSetLayoutCreateInfo descriptorSetLayoutCI = { VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO };

    descriptorSetLayoutCI.bindingCount = 1u;
    descriptorSetLayoutCI.pBindings    = &descriptorSetLayoutBinding;

    VK_CHECK_RESULT(vkCreateDescriptorSetLayout(device, &descriptorSetLayoutCI, nullptr, &m_blur.descriptorSetLayout));

    Debug::SetDescriptorSetLayoutName(device, m_blur.descriptorSetLayout, "Blur_DescriptorSetLayout");
}

void    BloomPass::SetupBlurDescriptorSets              (std::vector<Frame> const& p_frames) noexcept
{
    VkDevice const device = RHI::Get().GetDevice()->GetLogicalDevice();

    for (size_t i = 0; i < p_frames.size(); ++i)
    {
        VkDescriptorSetAllocateInfo decriptorSetAI = { VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO };

        decriptorSetAI.descriptorPool     = m_blur.descriptorPool;
        decriptorSetAI.descriptorSetCount = 1u;
        decriptorSetAI.pSetLayouts        = &m_blur.descriptorSetLayout;

        VK_CHECK_RESULT(vkAllocateDescriptorSets(device, &decriptorSetAI, &m_attachments[i].horizontalBlurDescriptorSet));
        VK_CHECK_RESULT(vkAllocateDescriptorSets(device, &decriptorSetAI, &m_attachments[i].verticalBlurDescriptorSet));

        Debug::SetDescriptorSetName(device, m_attachments[i].horizontalBlurDescriptorSet, ("HorizontalBlur_DescriptorSet_" + std::to_string(i)).c_str());
        Debug::SetDescriptorSetName(device, m_attachments[i].verticalBlurDescriptorSet,   ("VerticalBlur_DescriptorSet_"   + std::to_string(i)).c_str());

        std::array<VkWriteDescriptorSet, 2> writeSets = {};

        VkDescriptorImageInfo verticalBlurImageInfo = {};

        verticalBlurImageInfo.sampler     = RHI::Get().GetSceneSampler();
        verticalBlurImageInfo.imageView   = m_attachments[i].verticalBlur->imageView;
        verticalBlurImageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

        writeSets[0].sType           = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        writeSets[0].dstSet          = m_attachments[i].horizontalBlurDescriptorSet;
        writeSets[0].dstBinding      = 0u;
        writeSets[0].descriptorCount = 1u;
        writeSets[0].descriptorType  = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        writeSets[0].pImageInfo      = &verticalBlurImageInfo;

        VkDescriptorImageInfo horizontalBlurImageInfo = {};

        horizontalBlurImageInfo.sampler     = RHI::Get().GetSceneSampler();
        horizontalBlurImageInfo.imageView   = m_attachments[i].horizontalBlur->imageView;
        horizontalBlurImageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

        writeSets[1].sType           = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        writeSets[1].dstSet          = m_attachments[i].verticalBlurDescriptorSet;
        writeSets[1].dstBinding      = 0u;
        writeSets[1].descriptorCount = 1u;
        writeSets[1].descriptorType  = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        writeSets[1].pImageInfo      = &horizontalBlurImageInfo;

        vkUpdateDescriptorSets(device, static_cast<uint32>(writeSets.size()), writeSets.data(), 0u, nullptr);
    }
}

void    BloomPass::SetupBlurPipelineLayout              (std::vector<Frame> const& p_frames) noexcept
{
    VkDevice const device = RHI::Get().GetDevice()->GetLogicalDevice();

    VkPipelineLayoutCreateInfo pipelineLayoutCI = { VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO };

    pipelineLayoutCI.setLayoutCount = 1u;
    pipelineLayoutCI.pSetLayouts    = &m_blur.descriptorSetLayout;

    VK_CHECK_RESULT(vkCreatePipelineLayout(device, &pipelineLayoutCI, nullptr, &m_blur.pipelineLayout));

    Debug::SetPipelineLayoutName(device, m_blur.pipelineLayout, "Blur_PipelineLayout");
}

void    BloomPass::SetupBlurPipelines                   (std::vector<Frame> const& p_frames) noexcept
{
    auto const& device = RHI::Get().GetDevice       ();
    auto const& cache  = RHI::Get().GetPipelineCache();

    uint32 horizontal;

    VkSpecializationMapEntry entry = {};

    entry.constantID = 0u;
    entry.offset     = 0u;
    entry.size       = sizeof(uint32);

    VkSpecializationInfo info = {};

    info.mapEntryCount = 1u;
    info.pMapEntries   = &entry;
    info.dataSize      = sizeof(uint32);
    info.pData         = &horizontal;

    std::array<VkPipelineShaderStageCreateInfo, 2> shaderStages = {};

    shaderStages[0].sType               = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    shaderStages[0].stage               = VK_SHADER_STAGE_VERTEX_BIT;
    shaderStages[0].module              = AssetManager::Get().Get<Shader>("Default/Shaders/fullscreen.vert", ELoadingMode::BLOCKING)->GetModule();
    shaderStages[0].pName               = "main";

    shaderStages[1].sType               = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    shaderStages[1].stage               = VK_SHADER_STAGE_FRAGMENT_BIT;
    shaderStages[1].module              = AssetManager::Get().Get<Shader>("Default/Shaders/blur.frag", ELoadingMode::BLOCKING)->GetModule();
    shaderStages[1].pName               = "main";
    shaderStages[1].pSpecializationInfo = &info;

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
    pipelineCI.layout              = m_blur.pipelineLayout;
    pipelineCI.renderPass          = m_handle;
    pipelineCI.subpass             = 0u;

    horizontal = true;

    VK_CHECK_RESULT(vkCreateGraphicsPipelines(device->GetLogicalDevice(),
                                              cache ->GetHandle       (),
                                              1u,
                                              &pipelineCI,
                                              nullptr,
                                              &m_blur.horizontalBlurPipeline));

    horizontal = false;

    VK_CHECK_RESULT(vkCreateGraphicsPipelines(device->GetLogicalDevice(),
                                              cache ->GetHandle       (),
                                              1u,
                                              &pipelineCI,
                                              nullptr,
                                              &m_blur.verticalBlurPipeline));

    Debug::SetPipelineName(device->GetLogicalDevice(), m_blur.horizontalBlurPipeline, "HorizontalBlur_Pipeline");
    Debug::SetPipelineName(device->GetLogicalDevice(), m_blur.verticalBlurPipeline,   "VerticalBlur_Pipeline");
}

// ======================================================================================= //

void    BloomPass::BrightColorPass  (Frame const& p_frame) const noexcept
{
    Debug::BeginCmdBufferLabelRegion(p_frame.commandBuffer.GetHandle(), "BrightColorPass", Color::Red);

    VkClearValue const clearValue = { 0.0f, 0.0f, 0.0f, 0.0f };

    VkRenderPassBeginInfo renderPassBI = { VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO };

    renderPassBI.renderPass               = m_handle;
    renderPassBI.framebuffer              = m_attachments[p_frame.index].brightColorFramebuffer;
    renderPassBI.renderArea.extent.width  = p_frame.result.width;
    renderPassBI.renderArea.extent.height = p_frame.result.height;
    renderPassBI.clearValueCount          = 1u;
    renderPassBI.pClearValues             = &clearValue;

    vkCmdBeginRenderPass(p_frame.commandBuffer.GetHandle(), &renderPassBI, VK_SUBPASS_CONTENTS_INLINE);

    vkCmdBindPipeline(p_frame.commandBuffer.GetHandle(), VK_PIPELINE_BIND_POINT_GRAPHICS, m_brightcolor.pipeline);

    vkCmdBindDescriptorSets(p_frame.commandBuffer.GetHandle(),
                            VK_PIPELINE_BIND_POINT_GRAPHICS,
                            m_brightcolor.pipelineLayout,
                            0u,
                            1u,
                            &m_attachments[p_frame.index].brightColorDescriptorSet,
                            0u,
                            nullptr);

    vkCmdDraw(p_frame.commandBuffer.GetHandle(), 3u, 1u, 0u, 0u);

    vkCmdEndRenderPass(p_frame.commandBuffer.GetHandle());

    Debug::EndCmdBufferLabelRegion(p_frame.commandBuffer.GetHandle());
}

void    BloomPass::BlurPass         (Frame const& p_frame) const noexcept
{
    VkClearValue const clearValue = { 0.0f, 0.0f, 0.0f, 0.0f };

    VkRenderPassBeginInfo renderPassBI = { VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO };

    renderPassBI.renderPass               = m_handle;
    renderPassBI.renderArea.extent.width  = p_frame.result.width;
    renderPassBI.renderArea.extent.height = p_frame.result.height;
    renderPassBI.clearValueCount          = 1u;
    renderPassBI.pClearValues             = &clearValue;

    bool horizontal = false;

    for (uint32 i = 0u; i < 4u; ++i, horizontal = !horizontal)
    {
        if (horizontal)
        {
            renderPassBI.framebuffer = m_attachments[p_frame.index].horizontalBlurFramebuffer;

            Debug::BeginCmdBufferLabelRegion(p_frame.commandBuffer.GetHandle(), "HorizontalBlurPass", Color::Green);

            vkCmdBeginRenderPass(p_frame.commandBuffer.GetHandle(), &renderPassBI, VK_SUBPASS_CONTENTS_INLINE);

            vkCmdBindPipeline(p_frame.commandBuffer.GetHandle(), VK_PIPELINE_BIND_POINT_GRAPHICS, m_blur.horizontalBlurPipeline);

            vkCmdBindDescriptorSets(p_frame.commandBuffer.GetHandle(),
                                    VK_PIPELINE_BIND_POINT_GRAPHICS,
                                    m_blur.pipelineLayout,
                                    0u,
                                    1u,
                                    &m_attachments[p_frame.index].horizontalBlurDescriptorSet,
                                    0u,
                                    nullptr);

            vkCmdDraw(p_frame.commandBuffer.GetHandle(), 3u, 1u, 0u, 0u);

            vkCmdEndRenderPass(p_frame.commandBuffer.GetHandle());

            Debug::EndCmdBufferLabelRegion(p_frame.commandBuffer.GetHandle());
        }

        else
        {
            renderPassBI.framebuffer = m_attachments[p_frame.index].verticalBlurFramebuffer;

            Debug::BeginCmdBufferLabelRegion(p_frame.commandBuffer.GetHandle(), "VerticalBlurPass", Color::Blue);

            vkCmdBeginRenderPass(p_frame.commandBuffer.GetHandle(), &renderPassBI, VK_SUBPASS_CONTENTS_INLINE);

            vkCmdBindPipeline(p_frame.commandBuffer.GetHandle(), VK_PIPELINE_BIND_POINT_GRAPHICS, m_blur.verticalBlurPipeline);

            vkCmdBindDescriptorSets(p_frame.commandBuffer.GetHandle(),
                                    VK_PIPELINE_BIND_POINT_GRAPHICS,
                                    m_blur.pipelineLayout,
                                    0u,
                                    1u,
                                    &m_attachments[p_frame.index].verticalBlurDescriptorSet,
                                    0u,
                                    nullptr);

            vkCmdDraw(p_frame.commandBuffer.GetHandle(), 3u, 1u, 0u, 0u);

            vkCmdEndRenderPass(p_frame.commandBuffer.GetHandle());

            Debug::EndCmdBufferLabelRegion(p_frame.commandBuffer.GetHandle());
        }
    }
}