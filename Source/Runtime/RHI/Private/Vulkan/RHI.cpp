#include "PCH.hpp"
#include "RHI.hpp"
#include "Renderer.hpp"
#include "AssetManager.hpp"
#include "GameUserSettings.hpp"

#include "Vulkan/RenderPasses/BloomPass.hpp"
#include "Vulkan/RenderPasses/ShadowPass.hpp"
#include "Vulkan/RenderPasses/LightingPass.hpp"
#include "Vulkan/RenderPasses/TonemappingPass.hpp"

// ============================== [Public Constructor] ============================== //

RHI::RHI    () :
    m_currentFrame { 0u }
{

}

// ============================== [Module Public Local Methods] ============================== //

void    RHI::Initialize     (EngineKey const& p_passkey)
{
    LOG(LogRHI, Warning, "\nInitializing RHI...\n");

    // Initializes the RHI if the library could be loaded.
    if (Loader::Load())
    {
        m_instance  = std::make_unique<Instance>       ();
        m_device    = std::make_unique<Device>         ();
        m_swapchain = std::make_unique<Swapchain>      ();
        m_allocator = std::make_unique<DeviceAllocator>(m_swapchain->GetImageCount());
        m_cache     = std::make_unique<PipelineCache>  ();

        m_frames.resize(m_swapchain->GetImageCount());

        SetupSamplers            ();
        SetupFrames              ();
        SetupUniformBuffers      ();
        SetupDescriptorPool      ();
        SetupDescriptorSetLayouts();
        SetupDescriptorSets      ();

        m_renderPasses[ERenderStage::SHADOW]      = std::make_unique<ShadowPass>     (m_frames);
        m_renderPasses[ERenderStage::LIGHTING]    = std::make_unique<LightingPass>   (m_frames);
        m_renderPasses[ERenderStage::BLOOM]       = std::make_unique<BloomPass>      (m_frames);
        m_renderPasses[ERenderStage::TONEMAPPING] = std::make_unique<TonemappingPass>(m_frames);

        #if EDITOR

        // Loads default models.
        AssetManager& assetManager = AssetManager::Get();

        assetManager.FindOrAdd((std::string(SOURCE_DIRECTORY) + "Default/Models/cube.obj")  .c_str(), "Default/Models/");
        assetManager.FindOrAdd((std::string(SOURCE_DIRECTORY) + "Default/Models/sphere.obj").c_str(), "Default/Models/");

        #endif

        m_initialized = true;

        LOG(LogRHI, Warning, "\nRHI initialized\n");
    }

    else
        LOG(LogRHI, Fatal, "RHI initialization : FAILED");
}

void    RHI::Update         (EngineKey const& p_passkey)
{
    if (!m_initialized)
    {
        LOG(LogRHI, Error, "RHI was not initialized");
        return;
    }
}

void    RHI::Shutdown       (EngineKey const& p_passkey)
{
    LOG(LogRHI, Warning, "\nShutting down RHI...\n");

    // Shuts down the module if it was initialized.
    if (m_initialized)
    {
        Cleanup();

        m_renderPasses.clear();
        m_cache       .reset();
        m_swapchain   .reset();
        m_allocator   .reset();
        m_device      .reset();
        m_instance    .reset();
    }

    Loader::Free();

    m_initialized = false;

    LOG(LogRHI, Warning, "\nRHI shut down\n");
}

// ============================== [Public Local Methods] ============================== //

Frame&  RHI::NextFrame  (EngineKey const& p_passkey)
{
    m_currentFrame = (m_currentFrame + 1u) % m_swapchain->GetImageCount();
    
    return m_frames[m_currentFrame];
}

void    RHI::DrawFrame  (EngineKey const& p_passkey)
{
    if (!m_initialized)
    {
        LOG(LogRHI, Error, "RHI was not initialized");
        return;
    }

    Frame& frame = m_frames[m_currentFrame];
    
    #if EDITOR

    VK_CHECK_RESULT(vkWaitForFences(m_device->GetLogicalDevice(), 1u, &frame.fence, VK_TRUE, MAX_UINT_64));
    VK_CHECK_RESULT(vkResetFences  (m_device->GetLogicalDevice(), 1u, &frame.fence));

    frame.commandBuffer.Begin(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);

    UpdateUniformBuffers(frame);

    for (auto const& renderPass : m_renderPasses)
    {
        renderPass.second->Draw(frame);
    }

    frame.commandBuffer.End();

    m_device->GetGraphicsQueue()->Submit(frame.commandBuffer.GetHandle(), frame.fence);

    frame.renderList = nullptr;

    #else

    VkResult result = m_swapchain->AcquireNextImage(frame.imageAvailableSemaphore);

    if (result == VK_SUCCESS || result == VK_SUBOPTIMAL_KHR)
    {
        VK_CHECK_RESULT(vkWaitForFences(m_device->GetLogicalDevice(), 1u, &frame.fence, VK_TRUE, MAX_UINT_64));
        VK_CHECK_RESULT(vkResetFences  (m_device->GetLogicalDevice(), 1u, &frame.fence));

        frame.commandBuffer.Begin(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);

        UpdateUniformBuffers(frame);

        for (auto const& renderPass : m_renderPasses)
        {
            renderPass.second->Draw(frame);
        }

        frame.commandBuffer.End();

        m_device->GetGraphicsQueue()->Submit(VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
                                             frame.imageAvailableSemaphore,
                                             frame.renderFinishedSemaphore,
                                             frame.commandBuffer.GetHandle(),
                                             frame.fence);

        m_device->GetPresentQueue()->Present(m_swapchain->GetImageIndex(),
                                             m_swapchain->GetHandle    (),
                                             frame.renderFinishedSemaphore);
    }

    #endif
}

// ============================== [Private Local Methods] ============================== //

void    RHI::Cleanup                () noexcept
{
    for (Frame& frame : m_frames)
    {
        m_allocator->DestroyImage (frame.result);
        m_allocator->DestroyBuffer(frame.ubos.camera);

        if (frame.ubos.spotLight       .handle) m_allocator->DestroyBuffer(frame.ubos.spotLight);
        if (frame.ubos.pointLight      .handle) m_allocator->DestroyBuffer(frame.ubos.pointLight);
        if (frame.ubos.directionalLight.handle) m_allocator->DestroyBuffer(frame.ubos.directionalLight);

        vkDestroyImageView(m_device->GetLogicalDevice(), frame.result.imageView,        nullptr);
        vkDestroyFence    (m_device->GetLogicalDevice(), frame.fence,                   nullptr);
        vkDestroySemaphore(m_device->GetLogicalDevice(), frame.imageAvailableSemaphore, nullptr);
        vkDestroySemaphore(m_device->GetLogicalDevice(), frame.renderFinishedSemaphore, nullptr);

        m_device->GetGraphicsCommandPool()->FreeCommandBuffer(frame.commandBuffer);
    }

    vkDestroySampler            (m_device->GetLogicalDevice(), m_samplers.texture,            nullptr);
    vkDestroySampler            (m_device->GetLogicalDevice(), m_samplers.scene,              nullptr);
    vkDestroyDescriptorPool     (m_device->GetLogicalDevice(), m_descriptorPool,              nullptr);
    vkDestroyDescriptorSetLayout(m_device->GetLogicalDevice(), m_descriptorSetLayouts.scene,  nullptr);
    vkDestroyDescriptorSetLayout(m_device->GetLogicalDevice(), m_descriptorSetLayouts.camera, nullptr);
    vkDestroyDescriptorSetLayout(m_device->GetLogicalDevice(), m_descriptorSetLayouts.light,  nullptr);
}

void    RHI::UpdateUniformBuffers   (Frame& p_frame) noexcept
{
    // Camera.
    p_frame.renderList->camera.projection(1, 1) *= -1.0f;

    m_allocator->Map(p_frame.ubos.camera);

    memcpy(p_frame.ubos.camera.allocationInfo.pMappedData, &p_frame.renderList->camera, sizeof(p_frame.renderList->camera));

    m_allocator->Unmap(p_frame.ubos.camera);
    
    // Spot lights.
    uint32 count = static_cast<uint32>(p_frame.renderList->spotLights.size());

    for (auto& light : p_frame.renderList->spotLights)
    {
        light.projection(1, 1) *= -1.0f;
    }

    m_allocator->Map(p_frame.ubos.spotLight);

    memcpy(p_frame.ubos.spotLight.allocationInfo.pMappedData,
           &count,
           sizeof(uint32));

    memcpy(static_cast<ANSICHAR*>(p_frame.ubos.spotLight.allocationInfo.pMappedData) + 16,
           p_frame.renderList->spotLights.data(),
           p_frame.renderList->spotLights.size() * sizeof(SpotLightData));

    m_allocator->Unmap(p_frame.ubos.spotLight);

    // Point lights.
    count = static_cast<uint32>(p_frame.renderList->pointLights.size());

    for (auto& light : p_frame.renderList->pointLights)
    {
        light.projection(1, 1) *= -1.0f;
    }

    m_allocator->Map(p_frame.ubos.pointLight);

    memcpy(p_frame.ubos.pointLight.allocationInfo.pMappedData,
           &count,
           sizeof(uint32));

    memcpy(static_cast<ANSICHAR*>(p_frame.ubos.pointLight.allocationInfo.pMappedData) + 16,
           p_frame.renderList->pointLights.data(),
           p_frame.renderList->pointLights.size() * sizeof(PointLightData));

    m_allocator->Unmap(p_frame.ubos.pointLight);

    // Directional lights.
    count = static_cast<uint32>(p_frame.renderList->directionalLights.size());

    for (auto& light : p_frame.renderList->directionalLights)
    {
        light.projection(1, 1) *= -1.0f;
    }

    m_allocator->Map(p_frame.ubos.directionalLight);

    memcpy(p_frame.ubos.directionalLight.allocationInfo.pMappedData,
           &count,
           sizeof(uint32));

    memcpy(static_cast<ANSICHAR*>(p_frame.ubos.directionalLight.allocationInfo.pMappedData) + 16,
           p_frame.renderList->directionalLights.data(),
           p_frame.renderList->directionalLights.size() * sizeof(DirectionalLightData));

    m_allocator->Unmap(p_frame.ubos.directionalLight);
}

// ===================================================================================== //

void    RHI::SetupSamplers              () noexcept
{
    VkSamplerCreateInfo samplerCI = { VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO };

    // Scene Sampler.
    samplerCI.magFilter        = VK_FILTER_NEAREST;
    samplerCI.minFilter        = VK_FILTER_NEAREST;
    samplerCI.mipmapMode       = VK_SAMPLER_MIPMAP_MODE_LINEAR;
    samplerCI.addressModeU     = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
    samplerCI.addressModeV     = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
    samplerCI.addressModeW     = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
    samplerCI.anisotropyEnable = m_device->GetFeatures  ().samplerAnisotropy;
    samplerCI.maxAnisotropy    = m_device->GetProperties().limits.maxSamplerAnisotropy;
    samplerCI.borderColor      = VK_BORDER_COLOR_FLOAT_OPAQUE_BLACK;

    VK_CHECK_RESULT(vkCreateSampler(m_device->GetLogicalDevice(), &samplerCI, nullptr, &m_samplers.scene));

    // Texture Sampler.
    samplerCI.magFilter        = VK_FILTER_LINEAR;
    samplerCI.minFilter        = VK_FILTER_LINEAR;
    samplerCI.mipmapMode       = VK_SAMPLER_MIPMAP_MODE_LINEAR;
    samplerCI.addressModeU     = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerCI.addressModeV     = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerCI.addressModeW     = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerCI.anisotropyEnable = m_device->GetFeatures  ().samplerAnisotropy;
    samplerCI.maxAnisotropy    = m_device->GetProperties().limits.maxSamplerAnisotropy;
    samplerCI.borderColor      = VK_BORDER_COLOR_FLOAT_OPAQUE_BLACK;

    VK_CHECK_RESULT(vkCreateSampler(m_device->GetLogicalDevice(), &samplerCI, nullptr, &m_samplers.texture));

    Debug::SetSamplerName(m_device->GetLogicalDevice(), m_samplers.scene,   "Scene_Sampler");
    Debug::SetSamplerName(m_device->GetLogicalDevice(), m_samplers.texture, "Texture_Sampler");
}

void    RHI::SetupFrames                () noexcept
{
    VkFenceCreateInfo     fenceCI     = { VK_STRUCTURE_TYPE_FENCE_CREATE_INFO     };
    VkSemaphoreCreateInfo semaphoreCI = { VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO };

    fenceCI.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    for (size_t i = 0; i < m_frames.size(); ++i)
    {
        m_frames[i].index = static_cast<uint32>(i);

        VkImageCreateInfo imageCI = { VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO };

        imageCI.imageType     = VK_IMAGE_TYPE_2D;
        imageCI.format        = VK_FORMAT_R8G8B8A8_UNORM;
        imageCI.extent.width  = GEngine->GetGameUserSettings()->GetResolutionWidth ();
        imageCI.extent.height = GEngine->GetGameUserSettings()->GetResolutionHeight();
        imageCI.extent.depth  = 1u;
        imageCI.mipLevels     = 1u;
        imageCI.arrayLayers   = 1u;
        imageCI.samples       = VK_SAMPLE_COUNT_1_BIT;
        imageCI.tiling        = VK_IMAGE_TILING_OPTIMAL;
        imageCI.usage         = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;

        m_allocator->CreateImage(m_frames[i].result, imageCI, VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT, VMA_MEMORY_USAGE_GPU_ONLY, 0u);

        VkImageViewCreateInfo imageViewCI = { VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO };

        imageViewCI.image                       = m_frames[i].result.handle;
        imageViewCI.viewType                    = VK_IMAGE_VIEW_TYPE_2D;
        imageViewCI.format                      = VK_FORMAT_R8G8B8A8_UNORM;
        imageViewCI.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        imageViewCI.subresourceRange.levelCount = imageCI.mipLevels;
        imageViewCI.subresourceRange.layerCount = imageCI.arrayLayers;

        VK_CHECK_RESULT(vkCreateImageView(m_device->GetLogicalDevice(), &imageViewCI, nullptr, &m_frames[i].result.imageView));

        m_frames[i].commandBuffer = m_device->GetGraphicsCommandPool()->AllocateCommandBuffer(VK_COMMAND_BUFFER_LEVEL_PRIMARY);

        VK_CHECK_RESULT(vkCreateFence    (m_device->GetLogicalDevice(), &fenceCI,     nullptr, &m_frames[i].fence));
        VK_CHECK_RESULT(vkCreateSemaphore(m_device->GetLogicalDevice(), &semaphoreCI, nullptr, &m_frames[i].imageAvailableSemaphore));
        VK_CHECK_RESULT(vkCreateSemaphore(m_device->GetLogicalDevice(), &semaphoreCI, nullptr, &m_frames[i].renderFinishedSemaphore));

        Debug::SetImageName        (m_device->GetLogicalDevice(), m_frames[i].result.handle,             ("Frame_Image_"                   + std::to_string(i)).c_str());
        Debug::SetImageViewName    (m_device->GetLogicalDevice(), m_frames[i].result.imageView,          ("Frame_ImageView_"               + std::to_string(i)).c_str());
        Debug::SetCommandBufferName(m_device->GetLogicalDevice(), m_frames[i].commandBuffer.GetHandle(), ("Frame_CommandBuffer_"           + std::to_string(i)).c_str());
        Debug::SetFenceName        (m_device->GetLogicalDevice(), m_frames[i].fence,                     ("Frame_Fence_"                   + std::to_string(i)).c_str());
        Debug::SetSemaphoreName    (m_device->GetLogicalDevice(), m_frames[i].imageAvailableSemaphore,   ("Frame_ImageAvailableSemaphore_" + std::to_string(i)).c_str());
        Debug::SetSemaphoreName    (m_device->GetLogicalDevice(), m_frames[i].renderFinishedSemaphore,   ("Frame_RenderFinishedSemaphore_" + std::to_string(i)).c_str());
    }
}

void    RHI::SetupUniformBuffers        () noexcept
{
    uint32 const maxTriangles = (Math::Min(m_device->GetProperties().limits.maxGeometryOutputVertices,
                                           m_device->GetProperties().limits.maxGeometryTotalOutputComponents / 6u) / 18u) * 6u;

    VkBufferCreateInfo bufferCI = { VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO };

    for (size_t i = 0; i < m_frames.size(); ++i)
    {
        // Camera.
        bufferCI.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
        bufferCI.size  = sizeof(Camera);

        m_allocator->CreateBuffer(m_frames[i].ubos.camera, bufferCI, 0u, VMA_MEMORY_USAGE_CPU_TO_GPU, VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

        // Spot lights.
        bufferCI.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
        bufferCI.size  = (maxTriangles / 2u) * sizeof(SpotLightData) + 16u;

        m_allocator->CreateBuffer(m_frames[i].ubos.spotLight, bufferCI, 0u, VMA_MEMORY_USAGE_CPU_TO_GPU, VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

        // Point lights.
        bufferCI.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
        bufferCI.size  = (maxTriangles / 6u) * sizeof(PointLightData) + 16u;

        m_allocator->CreateBuffer(m_frames[i].ubos.pointLight, bufferCI, 0u, VMA_MEMORY_USAGE_CPU_TO_GPU, VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

        // Directional lights.
        bufferCI.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
        bufferCI.size  = (maxTriangles / 2u) * sizeof(DirectionalLightData) + 16u;

        m_allocator->CreateBuffer(m_frames[i].ubos.directionalLight, bufferCI, 0u, VMA_MEMORY_USAGE_CPU_TO_GPU, VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
        
        Debug::SetBufferName(m_device->GetLogicalDevice(), m_frames[i].ubos.camera          .handle, ("Camera_UBO_"           + std::to_string(i)).c_str());
        Debug::SetBufferName(m_device->GetLogicalDevice(), m_frames[i].ubos.spotLight       .handle, ("SpotLight_UBO_"        + std::to_string(i)).c_str());
        Debug::SetBufferName(m_device->GetLogicalDevice(), m_frames[i].ubos.pointLight      .handle, ("PointLight_UBO_"       + std::to_string(i)).c_str());
        Debug::SetBufferName(m_device->GetLogicalDevice(), m_frames[i].ubos.directionalLight.handle, ("DirectionalLight_UBO_" + std::to_string(i)).c_str());
    }
}

void    RHI::SetupDescriptorPool        () noexcept
{
    std::array<VkDescriptorPoolSize, 2> descriptorPoolSizes = {};

    descriptorPoolSizes[0].type            = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    descriptorPoolSizes[0].descriptorCount = static_cast<uint32>(1 * m_frames.size());
    descriptorPoolSizes[1].type            = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    descriptorPoolSizes[1].descriptorCount = static_cast<uint32>(4 * m_frames.size());
    
    VkDescriptorPoolCreateInfo descriptorPoolCI = { VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO };

    descriptorPoolCI.maxSets       = static_cast<uint32>(3 * m_frames.size());
    descriptorPoolCI.poolSizeCount = static_cast<uint32>(descriptorPoolSizes.size());
    descriptorPoolCI.pPoolSizes    = descriptorPoolSizes.data();

    VK_CHECK_RESULT(vkCreateDescriptorPool(m_device->GetLogicalDevice(), &descriptorPoolCI, nullptr, &m_descriptorPool));

    Debug::SetDescriptorPoolName(m_device->GetLogicalDevice(), m_descriptorPool, "Main_DescriptorPool");
}

void    RHI::SetupDescriptorSetLayouts  () noexcept
{
    std::vector<VkDescriptorSetLayoutBinding> descriptorSetLayoutBindings;

    VkDescriptorSetLayoutCreateInfo descriptorSetLayoutCI = { VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO };

    descriptorSetLayoutBindings.resize(1);

    descriptorSetLayoutBindings[0].binding         = 0u;
    descriptorSetLayoutBindings[0].descriptorType  = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    descriptorSetLayoutBindings[0].descriptorCount = 1u;
    descriptorSetLayoutBindings[0].stageFlags      = VK_SHADER_STAGE_FRAGMENT_BIT;

    descriptorSetLayoutCI.bindingCount = static_cast<uint32>(descriptorSetLayoutBindings.size());
    descriptorSetLayoutCI.pBindings    = descriptorSetLayoutBindings.data();

    VK_CHECK_RESULT(vkCreateDescriptorSetLayout(m_device->GetLogicalDevice(), &descriptorSetLayoutCI, nullptr, &m_descriptorSetLayouts.scene));

    descriptorSetLayoutBindings.resize(1);

    descriptorSetLayoutBindings[0].binding         = 0u;
    descriptorSetLayoutBindings[0].descriptorType  = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    descriptorSetLayoutBindings[0].descriptorCount = 1u;
    descriptorSetLayoutBindings[0].stageFlags      = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;

    descriptorSetLayoutCI.bindingCount = static_cast<uint32>(descriptorSetLayoutBindings.size());
    descriptorSetLayoutCI.pBindings    = descriptorSetLayoutBindings.data();

    VK_CHECK_RESULT(vkCreateDescriptorSetLayout(m_device->GetLogicalDevice(), &descriptorSetLayoutCI, nullptr, &m_descriptorSetLayouts.camera));
    
    descriptorSetLayoutBindings.resize(3);

    descriptorSetLayoutBindings[0].binding         = 0u;
    descriptorSetLayoutBindings[0].descriptorType  = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    descriptorSetLayoutBindings[0].descriptorCount = 1u;
    descriptorSetLayoutBindings[0].stageFlags      = VK_SHADER_STAGE_FRAGMENT_BIT;

    descriptorSetLayoutBindings[1].binding         = 1u;
    descriptorSetLayoutBindings[1].descriptorType  = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    descriptorSetLayoutBindings[1].descriptorCount = 1u;
    descriptorSetLayoutBindings[1].stageFlags      = VK_SHADER_STAGE_FRAGMENT_BIT;

    descriptorSetLayoutBindings[2].binding         = 2u;
    descriptorSetLayoutBindings[2].descriptorType  = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    descriptorSetLayoutBindings[2].descriptorCount = 1u;
    descriptorSetLayoutBindings[2].stageFlags      = VK_SHADER_STAGE_FRAGMENT_BIT;

    descriptorSetLayoutCI.bindingCount = static_cast<uint32>(descriptorSetLayoutBindings.size());
    descriptorSetLayoutCI.pBindings    = descriptorSetLayoutBindings.data();
    
    VK_CHECK_RESULT(vkCreateDescriptorSetLayout(m_device->GetLogicalDevice(), &descriptorSetLayoutCI, nullptr, &m_descriptorSetLayouts.light));

    Debug::SetDescriptorSetLayoutName(m_device->GetLogicalDevice(), m_descriptorSetLayouts.scene,  "Scene_DescriptorSetLayout");
    Debug::SetDescriptorSetLayoutName(m_device->GetLogicalDevice(), m_descriptorSetLayouts.camera, "Camera_DescriptorSetLayout");
    Debug::SetDescriptorSetLayoutName(m_device->GetLogicalDevice(), m_descriptorSetLayouts.light,  "Light_DescriptorSetLayout");
}

void    RHI::SetupDescriptorSets        () noexcept
{
    VkDescriptorSetAllocateInfo decriptorSetAI = { VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO };

    for (size_t i = 0; i < m_frames.size(); ++i)
    {
        decriptorSetAI.descriptorPool     = m_descriptorPool;
        decriptorSetAI.descriptorSetCount = 1u;
        decriptorSetAI.pSetLayouts        = &m_descriptorSetLayouts.scene;

        VK_CHECK_RESULT(vkAllocateDescriptorSets(m_device->GetLogicalDevice(), &decriptorSetAI, &m_frames[i].descriptorSets.scene));

        decriptorSetAI.descriptorPool     = m_descriptorPool;
        decriptorSetAI.descriptorSetCount = 1u;
        decriptorSetAI.pSetLayouts        = &m_descriptorSetLayouts.camera;

        VK_CHECK_RESULT(vkAllocateDescriptorSets(m_device->GetLogicalDevice(), &decriptorSetAI, &m_frames[i].descriptorSets.camera));

        decriptorSetAI.descriptorPool     = m_descriptorPool;
        decriptorSetAI.descriptorSetCount = 1u;
        decriptorSetAI.pSetLayouts        = &m_descriptorSetLayouts.light;

        VK_CHECK_RESULT(vkAllocateDescriptorSets(m_device->GetLogicalDevice(), &decriptorSetAI, &m_frames[i].descriptorSets.light));

        Debug::SetDescriptorSetName(m_device->GetLogicalDevice(), m_frames[i].descriptorSets.scene,  "Scene_DescriptorSet");
        Debug::SetDescriptorSetName(m_device->GetLogicalDevice(), m_frames[i].descriptorSets.camera, "Camera_DescriptorSet");
        Debug::SetDescriptorSetName(m_device->GetLogicalDevice(), m_frames[i].descriptorSets.light,  "Light_DescriptorSet");

        std::array<VkWriteDescriptorSet, 5> writeSets = {};

        // Scene set.
        VkDescriptorImageInfo imageInfo = {};

        imageInfo.sampler     = m_samplers.scene;
        imageInfo.imageView   = m_frames[i].result.imageView;
        imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

        writeSets[0].sType           = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        writeSets[0].dstSet          = m_frames[i].descriptorSets.scene;
        writeSets[0].dstBinding      = 0u;
        writeSets[0].descriptorCount = 1u;
        writeSets[0].descriptorType  = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        writeSets[0].pImageInfo      = &imageInfo;

        // Camera set.
        VkDescriptorBufferInfo cameraBufferInfo = {};

        cameraBufferInfo.buffer = m_frames[i].ubos.camera.handle;
        cameraBufferInfo.range  = m_frames[i].ubos.camera.size;

        writeSets[1].sType           = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        writeSets[1].dstSet          = m_frames[i].descriptorSets.camera;
        writeSets[1].dstBinding      = 0u;
        writeSets[1].descriptorCount = 1u;
        writeSets[1].descriptorType  = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        writeSets[1].pBufferInfo     = &cameraBufferInfo;

        // Light Set.
        VkDescriptorBufferInfo spotLightBufferInfo = {};

        spotLightBufferInfo.buffer = m_frames[i].ubos.spotLight.handle;
        spotLightBufferInfo.range  = m_frames[i].ubos.spotLight.size;

        writeSets[2].sType           = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        writeSets[2].dstSet          = m_frames[i].descriptorSets.light;
        writeSets[2].dstBinding      = 0u;
        writeSets[2].descriptorCount = 1u;
        writeSets[2].descriptorType  = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        writeSets[2].pBufferInfo     = &spotLightBufferInfo;

        VkDescriptorBufferInfo pointLightBufferInfo = {};

        pointLightBufferInfo.buffer = m_frames[i].ubos.pointLight.handle;
        pointLightBufferInfo.range  = m_frames[i].ubos.pointLight.size;

        writeSets[3].sType           = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        writeSets[3].dstSet          = m_frames[i].descriptorSets.light;
        writeSets[3].dstBinding      = 1u;
        writeSets[3].descriptorCount = 1u;
        writeSets[3].descriptorType  = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        writeSets[3].pBufferInfo     = &pointLightBufferInfo;

        VkDescriptorBufferInfo directionalLightBufferInfo = {};

        directionalLightBufferInfo.buffer = m_frames[i].ubos.directionalLight.handle;
        directionalLightBufferInfo.range  = m_frames[i].ubos.directionalLight.size;

        writeSets[4].sType           = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        writeSets[4].dstSet          = m_frames[i].descriptorSets.light;
        writeSets[4].dstBinding      = 2u;
        writeSets[4].descriptorCount = 1u;
        writeSets[4].descriptorType  = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        writeSets[4].pBufferInfo     = &directionalLightBufferInfo;

        vkUpdateDescriptorSets(m_device->GetLogicalDevice(), static_cast<uint32>(writeSets.size()), writeSets.data(), 0u, nullptr);
    }
}