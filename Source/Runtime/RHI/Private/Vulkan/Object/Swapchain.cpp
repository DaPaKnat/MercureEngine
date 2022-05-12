#include "PCH.hpp"
#include "RHI.hpp"
#include "Application.hpp"
#include "GameUserSettings.hpp"

// ============================== [Public Constructor and Destructor] ============================== //

Swapchain::Swapchain    () :
    m_isOutOfDate    { false },
    m_extent         { static_cast<uint32>(Application::Get().GetMainWindow()->GetExtent().width),
                       static_cast<uint32>(Application::Get().GetMainWindow()->GetExtent().height) }
{
    CreateSurface();

    LOG(LogRHI, Display, "Created : Surface");

    SwapchainSupportDetails details = QuerySwapchainDetails();

    ChooseImageCount    (details.capabilities);
    ChooseSurfaceFormat (details.formats);
    ChooseImageExtent   (details.capabilities);
    ChoosePreTransform  (details.capabilities);
    ChooseCompositeAlpha(details.capabilities);
    ChoosePresentMode   (details.presentModes);
    CreateSwapchain     ();

    Application::Get().GetMainWindow()->BindFramebufferResizeCallback<Swapchain, &Swapchain::ResizeSwapchain>(this);

    LOG(LogRHI, Display, "Created : Swapchain");
}

Swapchain::~Swapchain   ()
{
    VkDevice const device = RHI::Get().GetDevice()->GetLogicalDevice();

    for (size_t i = 0; i < m_imageViews.size(); ++i)
    {
        vkDestroyImageView(device, m_imageViews[i], nullptr);
    }

    vkDestroySwapchainKHR(device, m_swapchain,  nullptr);

    LOG(LogRHI, Display, "Destroyed : Swapchain");

    vkDestroySurfaceKHR(RHI::Get().GetInstance()->GetHandle(), m_surface, nullptr);

    LOG(LogRHI, Display, "Destroyed : Surface");
}

// ============================== [Public Local Methods] ============================== //

VkResult    Swapchain::AcquireNextImage (VkSemaphore const p_semaphore) noexcept
{
    if (m_isOutOfDate.load(std::memory_order_acquire))
    {
        if (m_extent.width == 0u || m_extent.height == 0u)
            return VK_ERROR_OUT_OF_DATE_KHR;
        
        RecreateSwapchain();

        VkResult result = vkAcquireNextImageKHR(RHI::Get().GetDevice()->GetLogicalDevice(),
                                                m_swapchain,
                                                MAX_UINT_64,
                                                p_semaphore,
                                                VK_NULL_HANDLE,
                                                &m_imageIndex);

        if (result == VK_SUCCESS || result == VK_SUBOPTIMAL_KHR)
            return VK_SUBOPTIMAL_KHR;

        return result == VK_ERROR_OUT_OF_DATE_KHR ? VK_ERROR_OUT_OF_DATE_KHR : VK_CHECK_RESULT(result);
    }

    VkResult result = vkAcquireNextImageKHR(RHI::Get().GetDevice()->GetLogicalDevice(),
                                            m_swapchain,
                                            MAX_UINT_64,
                                            p_semaphore,
                                            VK_NULL_HANDLE,
                                            &m_imageIndex);

    return result == VK_SUCCESS || result == VK_SUBOPTIMAL_KHR || result == VK_ERROR_OUT_OF_DATE_KHR ? result : VK_CHECK_RESULT(result);
}

// ============================== [Private Local Methods] ============================== //

void                    Swapchain::CreateSurface            () noexcept
{
    #if defined(GLFW)

    VK_CHECK_RESULT(glfwCreateWindowSurface(RHI        ::Get().GetInstance  ()->GetHandle(),
                                            Application::Get().GetMainWindow()->GetHandle(),
                                            nullptr,
                                            &m_surface));

    #endif

    VkBool32 presentationSupport;

    VK_CHECK_RESULT(vkGetPhysicalDeviceSurfaceSupportKHR(RHI::Get().GetDevice()->GetPhysicalDevice(),
                                                         RHI::Get().GetDevice  ()->GetPresentFamily (),
                                                         m_surface,
                                                         &presentationSupport));

    if (presentationSupport == VK_FALSE)
        LOG(LogRHI, Fatal, "Presentation is not supported by the selected physical device");
}

void                    Swapchain::CreateSwapchain          () noexcept
{
    auto const&    device       = RHI::Get().GetDevice();
    VkSwapchainKHR oldSwapchain = m_swapchain;

    VkSwapchainCreateInfoKHR swapchainCI  = { VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR };

    swapchainCI.surface          = m_surface;
    swapchainCI.minImageCount    = m_imageCount;
    swapchainCI.imageFormat      = m_surfaceFormat.format;
    swapchainCI.imageColorSpace  = m_surfaceFormat.colorSpace;
    swapchainCI.imageExtent      = m_extent;
    swapchainCI.imageArrayLayers = 1u;
    swapchainCI.imageUsage       = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    if (device->GetGraphicsFamily() != device->GetPresentFamily())
    {
        uint32 indices[2] = {
            device->GetGraphicsFamily(),
            device->GetPresentFamily ()
        };

        swapchainCI.imageSharingMode      = VK_SHARING_MODE_CONCURRENT;
        swapchainCI.queueFamilyIndexCount = 2u;
        swapchainCI.pQueueFamilyIndices   = indices;
    }

    swapchainCI.preTransform   = (VkSurfaceTransformFlagBitsKHR)m_preTransform;
    swapchainCI.compositeAlpha = m_compositeAlpha;
    swapchainCI.presentMode    = m_presentMode;
    swapchainCI.clipped        = VK_TRUE;
    swapchainCI.oldSwapchain   = oldSwapchain;

    VK_CHECK_RESULT(vkCreateSwapchainKHR(device->GetLogicalDevice(), &swapchainCI, nullptr, &m_swapchain));

    // Destroys the old swapchain when the swapchain is recreated.
    if (oldSwapchain)
    {
        device->WaitIdle();

        for (size_t i = 0u; i < m_imageViews.size(); ++i)
        {
            vkDestroyImageView(device->GetLogicalDevice(), m_imageViews[i], nullptr);
        }

        vkDestroySwapchainKHR(device->GetLogicalDevice(), oldSwapchain, nullptr);
    }

    VK_CHECK_RESULT(vkGetSwapchainImagesKHR(device->GetLogicalDevice(), m_swapchain, &m_imageCount, nullptr));

    m_images    .resize(m_imageCount);
    m_imageViews.resize(m_imageCount);

    VK_CHECK_RESULT(vkGetSwapchainImagesKHR(device->GetLogicalDevice(), m_swapchain, &m_imageCount, m_images.data()));

    VkImageViewCreateInfo imageViewCI = { VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO };

    imageViewCI.viewType                    = VK_IMAGE_VIEW_TYPE_2D;
    imageViewCI.format                      = m_surfaceFormat.format;
    imageViewCI.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    imageViewCI.subresourceRange.levelCount = 1u;
    imageViewCI.subresourceRange.layerCount = 1u;

    for (uint32 i = 0u; i < m_imageCount; ++i)
    {
        imageViewCI.image = m_images[i];

        VK_CHECK_RESULT(vkCreateImageView(device->GetLogicalDevice(), &imageViewCI, nullptr, &m_imageViews[i]));

        Debug::SetImageName    (device->GetLogicalDevice(), m_images    [i], ("SwapchainImage_"     + std::to_string(i)).c_str());
        Debug::SetImageViewName(device->GetLogicalDevice(), m_imageViews[i], ("SwapchainImageView_" + std::to_string(i)).c_str());
    }
}

void                    Swapchain::ResizeSwapchain          (int32 p_width,
                                                             int32 p_height) noexcept
{
    m_extent.width  = static_cast<uint32>(p_width);
    m_extent.height = static_cast<uint32>(p_height);

    m_isOutOfDate.store(true, std::memory_order_release);
}

void                    Swapchain::RecreateSwapchain        () noexcept
{
    SwapchainSupportDetails details = QuerySwapchainDetails();

    ChooseImageCount    (details.capabilities);
    ChooseSurfaceFormat (details.formats);
    ChooseImageExtent   (details.capabilities);
    ChoosePreTransform  (details.capabilities);
    ChooseCompositeAlpha(details.capabilities);
    ChoosePresentMode   (details.presentModes);
    CreateSwapchain     ();

    m_isOutOfDate.store(false, std::memory_order_release);
}

SwapchainSupportDetails Swapchain::QuerySwapchainDetails    () const noexcept
{
    VkPhysicalDevice const  device  = RHI::Get().GetDevice()->GetPhysicalDevice();
    SwapchainSupportDetails details = {};

    VK_CHECK_RESULT(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, m_surface, &details.capabilities));

    uint32 formatCount = 0u;

    VK_CHECK_RESULT(vkGetPhysicalDeviceSurfaceFormatsKHR(device, m_surface, &formatCount, nullptr));

    details.formats.resize(formatCount);

    VK_CHECK_RESULT(vkGetPhysicalDeviceSurfaceFormatsKHR(device, m_surface, &formatCount, details.formats.data()));

    uint32 presentCount = 0u;

    VK_CHECK_RESULT(vkGetPhysicalDeviceSurfacePresentModesKHR(device, m_surface, &presentCount, nullptr));

    details.presentModes.resize(presentCount);

    VK_CHECK_RESULT(vkGetPhysicalDeviceSurfacePresentModesKHR(device, m_surface, &presentCount, details.presentModes.data()));

    return details;
}

void                    Swapchain::ChooseSurfaceFormat      (std::vector<VkSurfaceFormatKHR> const& p_formats) noexcept
{
    // Best case scenario, the surface has no preferred format.
    if (p_formats.size() == 1u && p_formats[0].format == VK_FORMAT_UNDEFINED)
    {
        m_surfaceFormat.format     = VK_FORMAT_B8G8R8A8_UNORM;
        m_surfaceFormat.colorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
        return;
    }

    m_surfaceFormat = p_formats[0];

    // Checks if the preferred combination is available.
    for (auto const& availableFormat : p_formats)
    {
        if (availableFormat.format == VK_FORMAT_B8G8R8A8_UNORM)
            m_surfaceFormat = availableFormat;

        if (availableFormat.format     == VK_FORMAT_B8G8R8A8_UNORM &&
            availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
        {
            m_surfaceFormat = availableFormat;
            break;
        }
    }
}

void                    Swapchain::ChooseImageExtent        (VkSurfaceCapabilitiesKHR const& p_capabilities) noexcept
{
    if (p_capabilities.currentExtent.width != MAX_UINT_32)
    {
        m_extent = p_capabilities.currentExtent;
        return;
    }

    m_extent.width  = Math::Clamp(m_extent.width,  p_capabilities.minImageExtent.width,  p_capabilities.maxImageExtent.width);
    m_extent.height = Math::Clamp(m_extent.height, p_capabilities.minImageExtent.height, p_capabilities.maxImageExtent.height);
}

void                    Swapchain::ChoosePreTransform       (VkSurfaceCapabilitiesKHR const& p_capabilities) noexcept
{
    if (p_capabilities.supportedTransforms & VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR)
        m_preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
    else
        m_preTransform = p_capabilities.currentTransform;
}

void                    Swapchain::ChooseCompositeAlpha     (VkSurfaceCapabilitiesKHR const& p_capabilities) noexcept
{
    std::vector<VkCompositeAlphaFlagBitsKHR> compositeAlphas = {
        VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
        VK_COMPOSITE_ALPHA_PRE_MULTIPLIED_BIT_KHR,
        VK_COMPOSITE_ALPHA_POST_MULTIPLIED_BIT_KHR,
        VK_COMPOSITE_ALPHA_INHERIT_BIT_KHR
    };

    for (auto const& compositeAlpha : compositeAlphas) 
    {
        if (p_capabilities.supportedCompositeAlpha & compositeAlpha)
        {
            m_compositeAlpha = compositeAlpha;
            break;
        }
    }
}

void                    Swapchain::ChoosePresentMode        (std::vector<VkPresentModeKHR> const& p_presentModes) noexcept
{
    if (GEngine->GetGameUserSettings()->IsVSyncEnabled())
    {
        m_presentMode = VK_PRESENT_MODE_FIFO_KHR;
        return;
    }

    for (auto const& availablePresentMode : p_presentModes)
    {
        if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
        {
            m_presentMode = availablePresentMode;
            return;
        }

        if (availablePresentMode == VK_PRESENT_MODE_IMMEDIATE_KHR)
            m_presentMode = availablePresentMode;
    }
}

void                    Swapchain::ChooseImageCount         (VkSurfaceCapabilitiesKHR const& p_capabilities) noexcept
{
    m_imageCount = p_capabilities.minImageCount + 1u;

    if (p_capabilities.maxImageCount > 0u && p_capabilities.maxImageCount < m_imageCount)
        m_imageCount = p_capabilities.maxImageCount;
}