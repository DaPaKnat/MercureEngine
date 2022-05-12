#include "PCH.hpp"
#include "RHI.hpp"
#include "AssetManager.hpp"

#include "Vulkan/Asset/Material/Material.hpp"

// ============================== [Public Constructor] ============================== //

Material::Material  (std::string const&     p_name,
                     MaterialCreateInfo&&   p_data) : Asset(p_name)
{
    m_isPending.store(true, std::memory_order_relaxed);

    m_data.albedo    = std::move(p_data.albedo);
    m_data.metallic  = std::move(p_data.metallic);
    m_data.roughness = std::move(p_data.roughness);
    m_data.ao        = std::move(p_data.ao);

    AssetManager& assetManager = AssetManager::Get();

    m_textures[0] = assetManager.Get<Texture>(p_data.albedoMapName   .c_str(), ELoadingMode::BLOCKING);
    m_textures[1] = assetManager.Get<Texture>(p_data.normalMapName   .c_str(), ELoadingMode::BLOCKING);
    m_textures[2] = assetManager.Get<Texture>(p_data.metallicMapName .c_str(), ELoadingMode::BLOCKING);
    m_textures[3] = assetManager.Get<Texture>(p_data.roughnessMapName.c_str(), ELoadingMode::BLOCKING);
    m_textures[4] = assetManager.Get<Texture>(p_data.aoMapName       .c_str(), ELoadingMode::BLOCKING);

    for (auto& texture : m_textures)
    {
        if (!texture)
            texture = assetManager.Get<Texture>("Default/Textures/default", ELoadingMode::BLOCKING);
    }

    SetupDescriptorPool     ();
    SetupDescriptorSetLayout();
    SetupDescriptorSet      ();
    SetupPipelineLayout     ();
    SetupPipeline           ();

    m_isLoaded .store(true,  std::memory_order_release);
    m_isPending.store(false, std::memory_order_release);
}

// ============================== [Interface Private Local Methods] ============================== //

void    Material::Deserialize   (std::string const& p_path) noexcept
{
    std::ifstream asset(p_path, std::ios::in);

    if (asset.is_open())
    {
        // Checks file header.
        std::string header;

        std::getline(asset, header);

        if (header == Reflect::GetEnumName(EAssetType::MATERIAL))
        {
            // Parses the json file.
            Json json = Json::parse(asset);

            AssetManager& assetManager = AssetManager::Get();

            m_data.albedo    = json.value<Color>("Albedo",    Color::White);
            m_data.metallic  = json.value<float>("Metallic",  0.0f);
            m_data.roughness = json.value<float>("Roughness", 0.0f);
            m_data.ao        = json.value<float>("AO",        0.0f);

            m_textures[0] = assetManager.Get<Texture>(json.value("AlbedoMap",    "Default/Textures/default").c_str(), ELoadingMode::BLOCKING);
            m_textures[1] = assetManager.Get<Texture>(json.value("NormalMap",    "Default/Textures/default").c_str(), ELoadingMode::BLOCKING);
            m_textures[2] = assetManager.Get<Texture>(json.value("MetallicMap",  "Default/Textures/default").c_str(), ELoadingMode::BLOCKING);
            m_textures[3] = assetManager.Get<Texture>(json.value("RoughnessMap", "Default/Textures/default").c_str(), ELoadingMode::BLOCKING);
            m_textures[4] = assetManager.Get<Texture>(json.value("AOMap",        "Default/Textures/default").c_str(), ELoadingMode::BLOCKING);

            SetupDescriptorPool     ();
            SetupDescriptorSetLayout();
            SetupDescriptorSet      ();
            SetupPipelineLayout     ();
            SetupPipeline           ();

            m_isLoaded.store(true, std::memory_order_release);
        }

        else
            LOG(LogAssetManager, Error, "Material file corrupted : %s", p_path.c_str());
    }

    else
        LOG(LogAssetManager, Error, "Failed to open \"%s\" for deserialization", p_path.c_str());

    m_isPending.store(false, std::memory_order_release);
}

void    Material::Serialize     (std::string const& p_path) noexcept
{
    std::ofstream asset(p_path, std::ios::out);

    if (asset.is_open())
    {
        // Inserts asset type as header in the file.
        asset << EAssetType::MATERIAL << '\n';

        Json json;

        json["Albedo"]    = m_data.albedo;
        json["Metallic"]  = m_data.metallic;
        json["Roughness"] = m_data.roughness;
        json["AO"]        = m_data.ao;

        json["AlbedoMap"]    = m_textures[0] ? m_textures[0]->GetName() : "Default/Textures/default";
        json["NormalMap"]    = m_textures[1] ? m_textures[1]->GetName() : "Default/Textures/default";
        json["MetallicMap"]  = m_textures[2] ? m_textures[2]->GetName() : "Default/Textures/default";
        json["RoughnessMap"] = m_textures[3] ? m_textures[3]->GetName() : "Default/Textures/default";
        json["AOMap"]        = m_textures[4] ? m_textures[4]->GetName() : "Default/Textures/default";

        // Serializes the json on disk.
        asset << json.dump(4);
    }

    else
        LOG(LogAssetManager, Error, "Failed to open \"%s\" for serialization", p_path.c_str());

    VkDevice const device = RHI::Get().GetDevice()->GetLogicalDevice();

    vkDestroyDescriptorPool     (device, m_renderData.descriptorPool,      nullptr);
    vkDestroyDescriptorSetLayout(device, m_renderData.descriptorSetLayout, nullptr);
    vkDestroyPipelineLayout     (device, m_renderData.pipelineLayout,      nullptr);
    vkDestroyPipeline           (device, m_renderData.pipeline,            nullptr);

    for (auto& texture : m_textures)
    {
        texture.reset();
    }

    m_isLoaded .store(false, std::memory_order_release);
    m_isPending.store(false, std::memory_order_release);
}

// ============================== [Private Local Methods] ============================== //

void    Material::SetupDescriptorPool       () noexcept
{
    VkDevice const device = RHI::Get().GetDevice()->GetLogicalDevice();

    VkDescriptorPoolSize descriptorPoolSize = {};

    descriptorPoolSize.type            = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    descriptorPoolSize.descriptorCount = static_cast<uint32>(m_textures.size());

    VkDescriptorPoolCreateInfo descriptorPoolCI = { VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO };

    descriptorPoolCI.maxSets       = 1u;
    descriptorPoolCI.poolSizeCount = 1u;
    descriptorPoolCI.pPoolSizes    = &descriptorPoolSize;

    VK_CHECK_RESULT(vkCreateDescriptorPool(device, &descriptorPoolCI, nullptr, &m_renderData.descriptorPool));

    Debug::SetDescriptorPoolName(device, m_renderData.descriptorPool, (m_name + "_DescriptorPool").c_str());
}

void    Material::SetupDescriptorSetLayout  () noexcept
{
    VkDevice const device = RHI::Get().GetDevice()->GetLogicalDevice();

    std::array<VkDescriptorSetLayoutBinding, 5> descriptorSetLayoutBindings = {};

    for (uint32 i = 0u; i < static_cast<uint32>(m_textures.size()); ++i)
    {
        descriptorSetLayoutBindings[i].binding         = i;
        descriptorSetLayoutBindings[i].descriptorType  = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        descriptorSetLayoutBindings[i].descriptorCount = 1u;
        descriptorSetLayoutBindings[i].stageFlags      = VK_SHADER_STAGE_FRAGMENT_BIT;
    }

    VkDescriptorSetLayoutCreateInfo descriptorSetLayoutCI = { VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO };

    descriptorSetLayoutCI.bindingCount = static_cast<uint32>(descriptorSetLayoutBindings.size());
    descriptorSetLayoutCI.pBindings    = descriptorSetLayoutBindings.data();

    VK_CHECK_RESULT(vkCreateDescriptorSetLayout(device, &descriptorSetLayoutCI, nullptr, &m_renderData.descriptorSetLayout));

    Debug::SetDescriptorSetLayoutName(device, m_renderData.descriptorSetLayout, (m_name + "_DescriptorSetLayout").c_str());
}

void    Material::SetupDescriptorSet        () noexcept
{
    VkDevice const device = RHI::Get().GetDevice()->GetLogicalDevice();

    VkDescriptorSetAllocateInfo decriptorSetAI = { VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO };

    decriptorSetAI.descriptorPool     = m_renderData.descriptorPool;
    decriptorSetAI.descriptorSetCount = 1u;
    decriptorSetAI.pSetLayouts        = &m_renderData.descriptorSetLayout;

    VK_CHECK_RESULT(vkAllocateDescriptorSets(device, &decriptorSetAI, &m_renderData.descriptorSet));

    Debug::SetDescriptorSetName(device, m_renderData.descriptorSet, (m_name + "_DescriptorSet").c_str());

    for (uint32 i = 0u; i < static_cast<uint32>(m_textures.size()); ++i)
    {
        while (!m_textures.at(i)->IsValid());

        VkDescriptorImageInfo imageInfo = {};

        imageInfo.sampler     = RHI::Get().GetTextureSampler();
        imageInfo.imageView   = m_textures[i]->GetImage().imageView;
        imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

        VkWriteDescriptorSet writeSet = { VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET };

        writeSet.dstSet          = m_renderData.descriptorSet;
        writeSet.dstBinding      = i;
        writeSet.descriptorCount = 1u;
        writeSet.descriptorType  = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        writeSet.pImageInfo      = &imageInfo;

        vkUpdateDescriptorSets(device, 1u, &writeSet, 0u, nullptr);
    }
}

void    Material::SetupPipelineLayout       () noexcept
{
    VkDevice const device = RHI::Get().GetDevice()->GetLogicalDevice();

    std::array<VkDescriptorSetLayout, 3> descriptorSetLayouts = {
        RHI::Get().GetCameraLayout(),
        RHI::Get().GetLightLayout (),
        m_renderData.descriptorSetLayout
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

    VK_CHECK_RESULT(vkCreatePipelineLayout(device, &pipelineLayoutCI, nullptr, &m_renderData.pipelineLayout));

    Debug::SetPipelineLayoutName(device, m_renderData.pipelineLayout, (m_name + "_PipelineLayout").c_str());
}

void    Material::SetupPipeline             () noexcept
{
    auto const& device = RHI::Get().GetDevice       ();
    auto const& cache  = RHI::Get().GetPipelineCache();

    std::array<VkSpecializationMapEntry, 5> entries = {};

    entries[0].constantID = 0u;
    entries[1].constantID = 1u;
    entries[2].constantID = 2u;
    entries[3].constantID = 3u;
    entries[4].constantID = 4u;

    entries[0].offset = 0u * sizeof(uint32);
    entries[1].offset = 1u * sizeof(uint32);
    entries[2].offset = 2u * sizeof(uint32);
    entries[3].offset = 3u * sizeof(uint32);
    entries[4].offset = 4u * sizeof(uint32);

    entries[0].size = sizeof(uint32);
    entries[1].size = sizeof(uint32);
    entries[2].size = sizeof(uint32);
    entries[3].size = sizeof(uint32);
    entries[4].size = sizeof(uint32);

    std::vector<uint32> layout(m_textures.size());

    for (size_t i = 0; i < m_textures.size(); ++i)
    {
        layout[i] = (m_textures[i]->GetName() != "Default/Textures/default");
    }
    
    VkSpecializationInfo info = {};

    info.mapEntryCount = static_cast<uint32>(entries.size());
    info.pMapEntries   = entries.data();
    info.dataSize      = entries.size() * sizeof(uint32);
    info.pData         = layout .data();

    std::array<VkPipelineShaderStageCreateInfo, 2> shaderStages = {};

    shaderStages[0].sType               = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    shaderStages[0].stage               = VK_SHADER_STAGE_VERTEX_BIT;
    shaderStages[0].pName               = "main";
    shaderStages[0].pSpecializationInfo = &info;

    shaderStages[1].sType               = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    shaderStages[1].stage               = VK_SHADER_STAGE_FRAGMENT_BIT;
    shaderStages[1].pName               = "main";
    shaderStages[1].pSpecializationInfo = &info;

    if (IsOpaque())
    {
        shaderStages[0].module = AssetManager::Get().Get<Shader>("Default/Shaders/gbuffer.vert", ELoadingMode::BLOCKING)->GetModule();
        shaderStages[1].module = AssetManager::Get().Get<Shader>("Default/Shaders/gbuffer.frag", ELoadingMode::BLOCKING)->GetModule();
    }

    else
    {
        shaderStages[0].module = AssetManager::Get().Get<Shader>("Default/Shaders/transparent.vert", ELoadingMode::BLOCKING)->GetModule();
        shaderStages[1].module = AssetManager::Get().Get<Shader>("Default/Shaders/transparent.frag", ELoadingMode::BLOCKING)->GetModule();
    }

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
    rasterizationStateCI.cullMode    = VK_CULL_MODE_BACK_BIT;
    rasterizationStateCI.frontFace   = VK_FRONT_FACE_CLOCKWISE;
    rasterizationStateCI.lineWidth   = 1.0f;

    VkPipelineMultisampleStateCreateInfo multisampleStateCI = { VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO };

    multisampleStateCI.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

    VkPipelineDepthStencilStateCreateInfo depthStencilStateCI = { VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO };

    depthStencilStateCI.depthTestEnable  = VK_TRUE;
    depthStencilStateCI.depthWriteEnable = IsOpaque() ? VK_TRUE                     : VK_FALSE;
    depthStencilStateCI.depthCompareOp   = IsOpaque() ? VK_COMPARE_OP_LESS_OR_EQUAL : VK_COMPARE_OP_LESS;

    std::vector<VkPipelineColorBlendAttachmentState> colorAttachments;

    if (IsOpaque())
    {
        colorAttachments.resize(3);

        colorAttachments[0].colorWriteMask = 0xf;
        colorAttachments[1].colorWriteMask = 0xf;
        colorAttachments[2].colorWriteMask = 0xf;
    }

    else
    {
        colorAttachments.resize(1);

        colorAttachments[0].blendEnable         = VK_TRUE;
        colorAttachments[0].srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
        colorAttachments[0].dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
        colorAttachments[0].colorBlendOp        = VK_BLEND_OP_ADD;
        colorAttachments[0].srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
        colorAttachments[0].dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
        colorAttachments[0].alphaBlendOp        = VK_BLEND_OP_ADD;
        colorAttachments[0].colorWriteMask      = 0xf;
    }

    VkPipelineColorBlendStateCreateInfo colorBlendStateCI = { VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO  };

    colorBlendStateCI.attachmentCount = static_cast<uint32>(colorAttachments.size());
    colorBlendStateCI.pAttachments    = colorAttachments.data();

    std::array<VkDynamicState, 2> dynamicStates = {
        VK_DYNAMIC_STATE_VIEWPORT,
        VK_DYNAMIC_STATE_SCISSOR
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
    pipelineCI.pColorBlendState    = &colorBlendStateCI;
    pipelineCI.pDynamicState       = &dynamicStateCI;
    pipelineCI.layout              = m_renderData.pipelineLayout;
    pipelineCI.renderPass          = RHI::Get().GetRenderPass(ERenderStage::LIGHTING)->GetHandle();
    pipelineCI.subpass             = IsOpaque() ? 0u : 2u;

    VK_CHECK_RESULT(vkCreateGraphicsPipelines(device->GetLogicalDevice(),
                                              cache ->GetHandle       (),
                                              1u,
                                              &pipelineCI,
                                              nullptr,
                                              &m_renderData.pipeline));

    Debug::SetPipelineName(device->GetLogicalDevice(), m_renderData.pipeline, (m_name + "_Pipeline").c_str());
}