#include "PCH.hpp"
#include "RHI.hpp"
#include "Renderer.hpp"
#include "AssetManager.hpp"

#include "Vulkan/RenderPasses/BloomPass.hpp"
#include "Vulkan/RenderPasses/LightingPass.hpp"
#include "Vulkan/RenderPasses/TonemappingPass.hpp"

// ============================== [Public Constructor and Destructor] ============================== //

TonemappingPass::TonemappingPass    (std::vector<Frame> const& p_frames) : RenderPass()
{
    #if EDITOR

    // Loads default shaders.
    AssetManager& assetManager = AssetManager::Get();

    assetManager.FindOrAdd((std::string(SOURCE_DIRECTORY) + "Default/Shaders/fullscreen.vert.glsl") .c_str(), "Default/Shaders/");
    assetManager.FindOrAdd((std::string(SOURCE_DIRECTORY) + "Default/Shaders/tonemapping.frag.glsl").c_str(), "Default/Shaders/");

    #endif

    SetupRenderPass  (p_frames);
    SetupFramebuffers(p_frames);
    SetupPipelines   (p_frames);
}

TonemappingPass::~TonemappingPass   ()
{
    VkDevice const device = RHI::Get().GetDevice()->GetLogicalDevice();

    for (VkFramebuffer framebuffer : m_framebuffers)
    {
        vkDestroyFramebuffer(device, framebuffer, nullptr);
    }

    vkDestroyDescriptorPool     (device, m_descriptorPool,      nullptr);
    vkDestroyDescriptorSetLayout(device, m_descriptorSetLayout, nullptr);
    vkDestroyPipelineLayout     (device, m_pipelineLayout,      nullptr);
    vkDestroyPipeline           (device, m_pipeline,            nullptr);
}

// ============================== [Public Local Methods] ============================== //

void    TonemappingPass::Rebuild    (std::vector<Frame> const& p_frames) noexcept
{
    
}

void    TonemappingPass::Draw       (Frame const& p_frame) noexcept
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

    VkClearValue const clearValue = { 0.0f, 0.0f, 0.0f, 0.0f };

    VkRenderPassBeginInfo renderPassBI = { VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO };

    renderPassBI.renderPass               = m_handle;
    renderPassBI.framebuffer              = m_framebuffers[p_frame.index];
    renderPassBI.renderArea.extent.width  = p_frame.result.width;
    renderPassBI.renderArea.extent.height = p_frame.result.height;
    renderPassBI.clearValueCount          = 1u;
    renderPassBI.pClearValues             = &clearValue;

    Debug::BeginCmdBufferLabelRegion(p_frame.commandBuffer.GetHandle(), "TonemappingPass", Color::Magenta);

    vkCmdBeginRenderPass(p_frame.commandBuffer.GetHandle(), &renderPassBI, VK_SUBPASS_CONTENTS_INLINE);
    
    vkCmdBindPipeline(p_frame.commandBuffer.GetHandle(), VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipeline);

    vkCmdBindDescriptorSets(p_frame.commandBuffer.GetHandle(),
                            VK_PIPELINE_BIND_POINT_GRAPHICS,
                            m_pipelineLayout,
                            0u,
                            1u,
                            &m_descriptorSets[p_frame.index],
                            0u,
                            nullptr);

    vkCmdDraw(p_frame.commandBuffer.GetHandle(), 3u, 1u, 0u, 0u);

    vkCmdEndRenderPass(p_frame.commandBuffer.GetHandle());

    Debug::EndCmdBufferLabelRegion(p_frame.commandBuffer.GetHandle());
}

// ============================== [Protected Local Methods] ============================== //

void    TonemappingPass::SetupRenderPass    (std::vector<Frame> const& p_frames) noexcept
{
    VkDevice const device = RHI::Get().GetDevice()->GetLogicalDevice();

    VkAttachmentDescription attachment = {};

    attachment.format         = VK_FORMAT_R8G8B8A8_UNORM;
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

    Debug::SetRenderPassName(device, m_handle, "TonemappingPass");
}

void    TonemappingPass::SetupFramebuffers   (std::vector<Frame> const& p_frames) noexcept
{
    VkDevice const device = RHI::Get().GetDevice()->GetLogicalDevice();

    VkFramebufferCreateInfo framebufferCI = { VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO };

    framebufferCI.renderPass      = m_handle;
    framebufferCI.attachmentCount = 1u;
    framebufferCI.layers          = 1u;

    m_framebuffers.resize(p_frames.size());

    for (size_t i = 0; i < m_framebuffers.size() && i < p_frames.size(); ++i)
    {
        framebufferCI.pAttachments = &p_frames[i].result.imageView;
        framebufferCI.width        =  p_frames[i].result.width;
        framebufferCI.height       =  p_frames[i].result.height;

        VK_CHECK_RESULT(vkCreateFramebuffer(device, &framebufferCI, nullptr, &m_framebuffers[i]));

        Debug::SetFramebufferName(device, m_framebuffers[i], ("Tonemapping_Framebuffer_" + std::to_string(i)).c_str());
    }
}

void    TonemappingPass::SetupPipelines     (std::vector<Frame> const& p_frames) noexcept
{
    SetupDescriptorPool     (p_frames);
    SetupDescriptorSetLayout(p_frames);
    SetupDescriptorSets     (p_frames);
    SetupPipelineLayout     (p_frames);
    SetupPipeline           (p_frames);
}

// ======================================================================================= //

void    TonemappingPass::SetupDescriptorPool        (std::vector<Frame> const& p_frames) noexcept
{
    VkDevice const device = RHI::Get().GetDevice()->GetLogicalDevice();

    VkDescriptorPoolSize descriptorPoolSize = {};

    descriptorPoolSize.type            = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    descriptorPoolSize.descriptorCount = static_cast<uint32>(2 * p_frames.size());

    VkDescriptorPoolCreateInfo descriptorPoolCI = { VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO };

    descriptorPoolCI.maxSets       = static_cast<uint32>(p_frames.size());
    descriptorPoolCI.poolSizeCount = 1u;
    descriptorPoolCI.pPoolSizes    = &descriptorPoolSize;

    VK_CHECK_RESULT(vkCreateDescriptorPool(device, &descriptorPoolCI, nullptr, &m_descriptorPool));

    Debug::SetDescriptorPoolName(device, m_descriptorPool, "Tonemapping_DescriptorPool");
}

void    TonemappingPass::SetupDescriptorSetLayout   (std::vector<Frame> const& p_frames) noexcept
{
    VkDevice const device = RHI::Get().GetDevice()->GetLogicalDevice();

    std::array<VkDescriptorSetLayoutBinding, 2> descriptorSetLayoutBindings = {};

    descriptorSetLayoutBindings[0].binding         = 0u;
    descriptorSetLayoutBindings[0].descriptorType  = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    descriptorSetLayoutBindings[0].descriptorCount = 1u;
    descriptorSetLayoutBindings[0].stageFlags      = VK_SHADER_STAGE_FRAGMENT_BIT;

    descriptorSetLayoutBindings[1].binding         = 1u;
    descriptorSetLayoutBindings[1].descriptorType  = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    descriptorSetLayoutBindings[1].descriptorCount = 1u;
    descriptorSetLayoutBindings[1].stageFlags      = VK_SHADER_STAGE_FRAGMENT_BIT;

    VkDescriptorSetLayoutCreateInfo descriptorSetLayoutCI = { VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO };

    descriptorSetLayoutCI.bindingCount = static_cast<uint32>(descriptorSetLayoutBindings.size());
    descriptorSetLayoutCI.pBindings    = descriptorSetLayoutBindings.data();

    VK_CHECK_RESULT(vkCreateDescriptorSetLayout(device, &descriptorSetLayoutCI, nullptr, &m_descriptorSetLayout));

    Debug::SetDescriptorSetLayoutName(device, m_descriptorSetLayout, "Tonemapping_DescriptorSetLayout");
}

void    TonemappingPass::SetupDescriptorSets        (std::vector<Frame> const& p_frames) noexcept
{
    VkDevice const device = RHI::Get().GetDevice()->GetLogicalDevice();

    m_descriptorSets.resize(p_frames.size());

    for (size_t i = 0; i < m_descriptorSets.size() && i < p_frames.size(); ++i)
    {
        VkDescriptorSetAllocateInfo decriptorSetAI = { VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO };

        decriptorSetAI.descriptorPool     = m_descriptorPool;
        decriptorSetAI.descriptorSetCount = 1u;
        decriptorSetAI.pSetLayouts        = &m_descriptorSetLayout;

        VK_CHECK_RESULT(vkAllocateDescriptorSets(device, &decriptorSetAI, &m_descriptorSets[i]));

        Debug::SetDescriptorSetName(device, m_descriptorSets[i], ("Tonemapping_DescriptorSet_" + std::to_string(i)).c_str());

        std::array<VkWriteDescriptorSet, 2> writeSets = {};

        VkDescriptorImageInfo hdrImageInfo = {};

        hdrImageInfo.sampler     = RHI::Get().GetSceneSampler();
        hdrImageInfo.imageView   = static_cast<LightingPass*>(RHI::Get().GetRenderPass(ERenderStage::LIGHTING).get())->GetResult(i).imageView;
        hdrImageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

        writeSets[0].sType           = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        writeSets[0].dstSet          = m_descriptorSets[i];
        writeSets[0].dstBinding      = 0u;
        writeSets[0].descriptorCount = 1u;
        writeSets[0].descriptorType  = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        writeSets[0].pImageInfo      = &hdrImageInfo;

        VkDescriptorImageInfo bloomImageInfo = {};

        bloomImageInfo.sampler     = RHI::Get().GetSceneSampler();
        bloomImageInfo.imageView   = static_cast<BloomPass*>(RHI::Get().GetRenderPass(ERenderStage::BLOOM).get())->GetResult(i).imageView;
        bloomImageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

        writeSets[1].sType           = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        writeSets[1].dstSet          = m_descriptorSets[i];
        writeSets[1].dstBinding      = 1u;
        writeSets[1].descriptorCount = 1u;
        writeSets[1].descriptorType  = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        writeSets[1].pImageInfo      = &bloomImageInfo;

        vkUpdateDescriptorSets(device, static_cast<uint32>(writeSets.size()), writeSets.data(), 0u, nullptr);
    }
}

void    TonemappingPass::SetupPipelineLayout        (std::vector<Frame> const& p_frames) noexcept
{
    VkDevice const device = RHI::Get().GetDevice()->GetLogicalDevice();

    VkPipelineLayoutCreateInfo pipelineLayoutCI = { VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO };

    pipelineLayoutCI.setLayoutCount = 1u;
    pipelineLayoutCI.pSetLayouts    = &m_descriptorSetLayout;

    VK_CHECK_RESULT(vkCreatePipelineLayout(device, &pipelineLayoutCI, nullptr, &m_pipelineLayout));

    Debug::SetPipelineLayoutName(device, m_pipelineLayout, "Tonemapping_PipelineLayout");
}

void    TonemappingPass::SetupPipeline              (std::vector<Frame> const& p_frames) noexcept
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
    shaderStages[1].module = AssetManager::Get().Get<Shader>("Default/Shaders/tonemapping.frag", ELoadingMode::BLOCKING)->GetModule();
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
    pipelineCI.subpass             = 0u;

    VK_CHECK_RESULT(vkCreateGraphicsPipelines(device->GetLogicalDevice(),
                                              cache ->GetHandle       (),
                                              1u,
                                              &pipelineCI,
                                              nullptr,
                                              &m_pipeline));

    Debug::SetPipelineName(device->GetLogicalDevice(), m_pipeline, "Tonemapping_Pipeline");
}