#include "PCH.hpp"
#include "RHI.hpp"

#include "Vulkan/Object/PipelineCache.hpp"

#ifndef EDITOR
    #define PIPELINE_CACHE_FILE "Intermediate/Pipeline.cache"
#else
    #define PIPELINE_CACHE_FILE "../../../Intermediate/Pipeline.cache"
#endif

// ============================== [Public Constructor and Destructor] ============================== //

PipelineCache::PipelineCache    ()
{
    VkDevice const device = RHI::Get().GetDevice()->GetLogicalDevice();

    VkPipelineCacheCreateInfo cacheCI = { VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO };

    std::ifstream file(PIPELINE_CACHE_FILE, std::ios::in | std::ios::ate | std::ios::binary);

    if (file.is_open())
    {
        std::vector<ANSICHAR> cache(file.tellg());

        file.seekg(0);

        file.read(cache.data(), cache.size());

        cacheCI.initialDataSize = cache.size();
        cacheCI.pInitialData    = cache.data();
        
        VK_CHECK_RESULT(vkCreatePipelineCache(device, &cacheCI, nullptr, &m_handle));
        
        Debug::SetPipelineCacheName(device, m_handle, "MainCache");
    }
}

PipelineCache::~PipelineCache   ()
{
    VkDevice const device = RHI::Get().GetDevice()->GetLogicalDevice();

    std::ofstream file(PIPELINE_CACHE_FILE, std::ios::out | std::ios::binary);

    if (file.is_open() && m_handle)
    {
        size_t size = 0;

        VK_CHECK_RESULT(vkGetPipelineCacheData(device, m_handle, &size, nullptr));

        std::vector<ANSICHAR> cache(size);

        VK_CHECK_RESULT(vkGetPipelineCacheData(device, m_handle, &size, cache.data()));

        file.write(reinterpret_cast<ANSICHAR*>(cache.data()), cache.size());
        
        vkDestroyPipelineCache(device, m_handle, nullptr);
    }
}