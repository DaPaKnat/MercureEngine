#include "PCH.hpp"
#include "RHI.hpp"
#include "ThreadPool.hpp"
#include "GameUserSettings.hpp"

// ============================== [Public Constructor and Destructor] ============================== //

Device::Device  ()
{
    m_requiredExtensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);

    PickPhysicalDevice       ();
    EnumerateDeviceProperties();
    FindQueueFamilies        ();
    CreateLogicalDevice      ();
    CreateQueues             ();
    CreateCommandPools       ();

    LOG(LogRHI, Display, "Created : Device");
}

Device::~Device ()
{
    for (auto const& it : m_cmdPools.graphicsCmdPools)
    {
        vkDestroyCommandPool(m_device, it.second->m_handle, nullptr);
    }

    for (auto const& it : m_cmdPools.computeCmdPools)
    {
        vkDestroyCommandPool(m_device, it.second->m_handle, nullptr);
    }

    for (auto const& it : m_cmdPools.transferCmdPools)
    {
        vkDestroyCommandPool(m_device, it.second->m_handle, nullptr);
    }

    vkDestroyDevice(m_device, nullptr);

    LOG(LogRHI, Display, "Destroyed : Device");
}

// ============================== [Public Local Methods] ============================== //

VkFormat                Device::FindSupportedFormat     (std::vector<VkFormat> const& p_candidates,
                                                         VkImageTiling                p_tiling,
                                                         VkFormatFeatureFlags         p_features) const noexcept
{
    VkFormatProperties formatProperties;

    for (auto const& format : p_candidates)
    {
        vkGetPhysicalDeviceFormatProperties(m_physicalDevice, format, &formatProperties);

        if (p_tiling == VK_IMAGE_TILING_LINEAR  && (formatProperties.linearTilingFeatures  & p_features) == p_features)
            return format;
        if (p_tiling == VK_IMAGE_TILING_OPTIMAL && (formatProperties.optimalTilingFeatures & p_features) == p_features)
            return format;
    }

    LOG(LogRHI, Error, "Couldn't find a supported format");

    return VK_FORMAT_UNDEFINED;
}

VkSampleCountFlagBits   Device::GetMaxUsableSampleCount () const noexcept
{
    VkSampleCountFlags counts = Math::Min(m_properties.limits.framebufferColorSampleCounts,
                                          m_properties.limits.framebufferDepthSampleCounts);

    VkSampleCountFlagBits sampleCount = static_cast<VkSampleCountFlagBits>(GEngine->GetGameUserSettings()->GetMultisampleCount());

    if (counts & VK_SAMPLE_COUNT_16_BIT && sampleCount >= VK_SAMPLE_COUNT_16_BIT)   return VK_SAMPLE_COUNT_16_BIT;
    if (counts & VK_SAMPLE_COUNT_8_BIT  && sampleCount >= VK_SAMPLE_COUNT_8_BIT)    return VK_SAMPLE_COUNT_8_BIT;
    if (counts & VK_SAMPLE_COUNT_4_BIT  && sampleCount >= VK_SAMPLE_COUNT_4_BIT)    return VK_SAMPLE_COUNT_4_BIT;

    return VK_SAMPLE_COUNT_2_BIT;
}

void                    Device::WaitIdle                () const noexcept
{
    VK_CHECK_RESULT(vkDeviceWaitIdle(m_device));
}

// ============================== [Private Local Methods] ============================== //

void    Device::PickPhysicalDevice          () noexcept
{
    uint32 deviceCount = 0u;

    // Enumerates the physical devices accessible to a Vulkan instance.
    VK_CHECK_RESULT(vkEnumeratePhysicalDevices(RHI::Get().GetInstance()->GetHandle(), &deviceCount, nullptr));

    if (deviceCount == 0u)
        LOG(LogRHI, Fatal, "No GPU available");

    std::vector<VkPhysicalDevice> devices(deviceCount);

    VK_CHECK_RESULT(vkEnumeratePhysicalDevices(RHI::Get().GetInstance()->GetHandle(), &deviceCount, devices.data()));

    // Uses an ordered map to automatically sort candidates by increasing score.
    std::multimap<uint32, VkPhysicalDevice> candidates;

    for (auto const& device : devices)
        candidates.insert(std::make_pair(RateDeviceSuitability(device), device));

    // Checks if at least one candidate is suitable (from the best one to the worst one).
    for (auto const& candidate : candidates)
    {
        if (CheckDeviceExtensions(candidate.second))
        {
            m_physicalDevice = candidate.second;
            break;
        }
    }

    if (m_physicalDevice == VK_NULL_HANDLE)
        LOG(LogRHI, Fatal, "Failed to find a suitable GPU");
}

uint32  Device::RateDeviceSuitability       (VkPhysicalDevice const p_device) const noexcept
{
    uint32                           score            = 0u;

    VkPhysicalDeviceProperties       properties       = {};
    VkPhysicalDeviceFeatures         features         = {};
    VkPhysicalDeviceMemoryProperties memoryProperties = {};

    // Gets the properties, features and memory properties of the physical device.
    vkGetPhysicalDeviceProperties      (p_device, &properties);
    vkGetPhysicalDeviceFeatures        (p_device, &features);
    vkGetPhysicalDeviceMemoryProperties(p_device, &memoryProperties);

    // Discrete GPUs have a performance advantage.
    if (properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
        score += 1000u;

    // Maximum possible size of textures affects graphics quality.
    score += properties.limits.maxImageDimension2D;

    return score;
}

bool    Device::CheckDeviceExtensions       (VkPhysicalDevice const p_device) noexcept
{
    uint32 extensionCount = 0u;

    // Returns properties of available physical device extensions.
    VK_CHECK_RESULT(vkEnumerateDeviceExtensionProperties(p_device, nullptr, &extensionCount, nullptr));

    std::vector<VkExtensionProperties> supportedExtensions(extensionCount);

    VK_CHECK_RESULT(vkEnumerateDeviceExtensionProperties(p_device, nullptr, &extensionCount, supportedExtensions.data()));

    std::set<std::string> requiredExtensions(m_requiredExtensions.cbegin(), m_requiredExtensions.cend());

    // Removes the matching extensions.
    for (auto const& extension : supportedExtensions)
        requiredExtensions.erase(extension.extensionName);

    return requiredExtensions.empty();
}

void    Device::EnumerateDeviceProperties   () noexcept
{
    vkGetPhysicalDeviceProperties      (m_physicalDevice, &m_properties);
    vkGetPhysicalDeviceFeatures        (m_physicalDevice, &m_features);
    vkGetPhysicalDeviceMemoryProperties(m_physicalDevice, &m_memoryProperties);

    m_properties.limits.maxSamplerAnisotropy = Math::Min(m_properties.limits.maxSamplerAnisotropy,
                                                         GEngine->GetGameUserSettings()->GetAnisotropy());
}

// ===================================================================================== //

void    Device::FindQueueFamilies   () noexcept
{
    uint32   queueFamilyCount    = 0u;
    VkBool32 presentationSupport = VK_FALSE;

    vkGetPhysicalDeviceQueueFamilyProperties(m_physicalDevice, &queueFamilyCount, nullptr);

    std::vector<VkQueueFamilyProperties> queueFamilyProperties(queueFamilyCount);

    vkGetPhysicalDeviceQueueFamilyProperties(m_physicalDevice, &queueFamilyCount, queueFamilyProperties.data());

    // Tries to find a queue family that supports both graphics and presentation.
    for (uint32 index = 0; index < static_cast<uint32>(queueFamilyProperties.size()); ++index)
    {
        #if defined(VK_USE_PLATFORM_WIN32_KHR)
           
        presentationSupport = vkGetPhysicalDeviceWin32PresentationSupportKHR(m_physicalDevice, index);

        #endif

        // Queue supporting both.
        if ((queueFamilyProperties[index].queueFlags & VK_QUEUE_GRAPHICS_BIT) && presentationSupport == VK_TRUE)
        {
            m_queueFamilies.graphicsFamily = index;
            m_queueFamilies.presentFamily  = index;
            break;
        }

        if (queueFamilyProperties[index].queueFlags & VK_QUEUE_GRAPHICS_BIT)
            m_queueFamilies.graphicsFamily = index;

        if (presentationSupport == VK_TRUE)
            m_queueFamilies.presentFamily  = index;
    }

    // Tries to find a dedicated queue family for compute.
    for (uint32 index = 0u; index < queueFamilyCount; ++index)
    {
        // Dedicated compute queue.
        if ((queueFamilyProperties[index].queueFlags & VK_QUEUE_COMPUTE_BIT ) &&
           ((queueFamilyProperties[index].queueFlags & VK_QUEUE_GRAPHICS_BIT) == 0))
        {
            m_queueFamilies.computeFamily = index;
            break;
        }

        if ((queueFamilyProperties[index].queueFlags & VK_QUEUE_COMPUTE_BIT ))
            m_queueFamilies.computeFamily = index;
    }

    // Tries to find a dedicated queue family for transfer.
    for (uint32 index = 0u; index < queueFamilyCount; ++index)
    {
        // Dedicated transfer queue.
        if ((queueFamilyProperties[index].queueFlags & VK_QUEUE_TRANSFER_BIT) &&
           ((queueFamilyProperties[index].queueFlags & VK_QUEUE_GRAPHICS_BIT) == 0) &&
           ((queueFamilyProperties[index].queueFlags & VK_QUEUE_COMPUTE_BIT ) == 0) )
        {
            m_queueFamilies.transferFamily = index;
            break;
        }

        if ((queueFamilyProperties[index].queueFlags & VK_QUEUE_TRANSFER_BIT ))
            m_queueFamilies.transferFamily = index;
    }
}

void    Device::CreateLogicalDevice () noexcept
{
    std::set<uint32> uniqueQueueFamilies = {
        m_queueFamilies.computeFamily,
        m_queueFamilies.graphicsFamily,
        m_queueFamilies.transferFamily,
        m_queueFamilies.presentFamily
    };

    // Structures specifying parameters of a newly created device queue.
    std::vector<VkDeviceQueueCreateInfo> deviceQueueCIs;

    float const defaultQueuePriority = 0.0f;

    for (uint32 uniqueQueueFamily : uniqueQueueFamilies)
    {
        VkDeviceQueueCreateInfo queueCI = { VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO };

        queueCI.queueCount       = 1u;
        queueCI.queueFamilyIndex = uniqueQueueFamily;
        queueCI.pQueuePriorities = &defaultQueuePriority;

        deviceQueueCIs.push_back(std::move(queueCI));
    }

    // Structure specifying parameters of a newly created device.
    VkDeviceCreateInfo deviceCI = { VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO };

    deviceCI.queueCreateInfoCount    = static_cast<uint32>(deviceQueueCIs.size());
    deviceCI.pQueueCreateInfos       = deviceQueueCIs.data();
    deviceCI.enabledExtensionCount   = static_cast<uint32>(m_requiredExtensions.size());
    deviceCI.ppEnabledExtensionNames = m_requiredExtensions.data();
    deviceCI.pEnabledFeatures        = &m_features;

    VK_CHECK_RESULT(vkCreateDevice(m_physicalDevice, &deviceCI, nullptr, &m_device));
}

void    Device::CreateQueues        () noexcept
{
    VkQueue graphicsQueue, presentQueue, computeQueue, transferQueue;

    vkGetDeviceQueue(m_device, m_queueFamilies.graphicsFamily, 0u, &graphicsQueue);
    vkGetDeviceQueue(m_device, m_queueFamilies.presentFamily,  0u, &presentQueue);
    vkGetDeviceQueue(m_device, m_queueFamilies.computeFamily,  0u, &computeQueue);
    vkGetDeviceQueue(m_device, m_queueFamilies.transferFamily, 0u, &transferQueue);

    m_queues.graphicsQueue = std::make_unique<Queue>(m_queueFamilies.graphicsFamily, graphicsQueue);
    m_queues.presentQueue  = std::make_unique<Queue>(m_queueFamilies.presentFamily,  presentQueue);
    m_queues.computeQueue  = std::make_unique<Queue>(m_queueFamilies.computeFamily,  computeQueue);
    m_queues.transferQueue = std::make_unique<Queue>(m_queueFamilies.transferFamily, transferQueue);

    Debug::SetQueueName(m_device, graphicsQueue, "GraphicsQueue");
    Debug::SetQueueName(m_device, presentQueue,  "PresentQueue");
    Debug::SetQueueName(m_device, computeQueue,  "ComputeQueue");
    Debug::SetQueueName(m_device, transferQueue, "TransferQueue");
}

void    Device::CreateCommandPools  () noexcept
{
    VkCommandPool cmdPool;

    VkCommandPoolCreateInfo cmdPoolCI = { VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO };

    cmdPoolCI.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

    // Graphics command pool.
    cmdPoolCI.queueFamilyIndex = m_queueFamilies.graphicsFamily;

    VK_CHECK_RESULT(vkCreateCommandPool(m_device, &cmdPoolCI, nullptr, &cmdPool));

    m_cmdPools.graphicsCmdPools[std::this_thread::get_id()] = std::make_unique<CommandPool>(m_queueFamilies.graphicsFamily, cmdPool);

    Debug::SetCommandPoolName(m_device, cmdPool, "GraphicsCommandPool");

    // Compute command pools.
    cmdPoolCI.queueFamilyIndex = m_queueFamilies.computeFamily;

    VK_CHECK_RESULT(vkCreateCommandPool(m_device, &cmdPoolCI, nullptr, &cmdPool));

    m_cmdPools.computeCmdPools[std::this_thread::get_id()] = std::make_unique<CommandPool>(m_queueFamilies.computeFamily, cmdPool);

    Debug::SetCommandPoolName(m_device, cmdPool, "ComputeCommandPool");

    // Transfer command pool.
    cmdPoolCI.queueFamilyIndex = m_queueFamilies.transferFamily;

    VK_CHECK_RESULT(vkCreateCommandPool(m_device, &cmdPoolCI, nullptr, &cmdPool));

    m_cmdPools.transferCmdPools[std::this_thread::get_id()] = std::make_unique<CommandPool>(m_queueFamilies.transferFamily, cmdPool);

    Debug::SetCommandPoolName(m_device, cmdPool, "TransferCommandPool");

    for (auto const& thread : ThreadPool::Get().GetThreads())
    {
        // Graphics command pool.
        cmdPoolCI.queueFamilyIndex = m_queueFamilies.graphicsFamily;
        
        VK_CHECK_RESULT(vkCreateCommandPool(m_device, &cmdPoolCI, nullptr, &cmdPool));

        m_cmdPools.graphicsCmdPools[thread.get_id()] = std::make_unique<CommandPool>(m_queueFamilies.graphicsFamily, cmdPool);

        Debug::SetCommandPoolName(m_device, cmdPool, "GraphicsCommandPool");

        // Compute command pool.
        cmdPoolCI.queueFamilyIndex = m_queueFamilies.computeFamily;

        VK_CHECK_RESULT(vkCreateCommandPool(m_device, &cmdPoolCI, nullptr, &cmdPool));

        m_cmdPools.computeCmdPools[thread.get_id()] = std::make_unique<CommandPool>(m_queueFamilies.computeFamily, cmdPool);

        Debug::SetCommandPoolName(m_device, cmdPool, "ComputeCommandPool");

        // Transfer command pool.
        cmdPoolCI.queueFamilyIndex = m_queueFamilies.transferFamily;

        VK_CHECK_RESULT(vkCreateCommandPool(m_device, &cmdPoolCI, nullptr, &cmdPool));

        m_cmdPools.transferCmdPools[thread.get_id()] = std::make_unique<CommandPool>(m_queueFamilies.transferFamily, cmdPool);

        Debug::SetCommandPoolName(m_device, cmdPool, "TransferCommandPool");
    }
}